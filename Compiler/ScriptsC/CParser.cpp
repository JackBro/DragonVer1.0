
#include "internal.h"




CCParser::CCParser(char * filename):CParseFBase()
{
	CParseFBase::m_pScanner = new CCScanner(filename);
	m_pErrfile = &m_pScanner->m_errfile;
	m_pProgram = new CSyntaxTree();
}

CCParser::~CCParser(void)
{
	delete m_pScanner;
	m_pScanner = NULL;
	delete m_pProgram;
	m_pProgram = NULL;
}


//below are parser routines:
CSyntaxTree* CCParser::Parse(void)
{
	CSyntaxTree* t = NULL;
	m_token = m_pScanner->NextToken();
	t = this->BuildSynTree();
	if ( m_token.type != _EOF )
	{
		this->WriteLog("Codes end before file.");
	}
	
	this->WriteLog( "Parser phase ends.");
	return t;
}

CSyntaxTree* CCParser::BuildSynTree(void)
{
	CSyntaxTree * t = this->GetSynUnit(), *p = NULL;
	if ( t)	p = t->LastSibling();

	while( this->m_token.type != _EOF)
	{
		CSyntaxTree *q;
		this->Match(SEMI);
        q = this->GetSynUnit();
		if ( NULL != q )
		{
			if ( NULL == t)	
			{
				t = q;
				p = q->LastSibling();
			}
			else
			{
				p->sibling = q;
				p = q->LastSibling();
			}
		}
	}

	return t;
}

//prog::= {dcl;|func|regmod|syscallmod}
CSyntaxTree* CCParser::GetSynUnit(void)
{
	// this cycle may be deleted if scanner is modified appropriately, to be decided.
	while ( this->m_token.type == _DIRECTIVE )
		m_token = m_pScanner->NextToken();

	CSyntaxTree * t = NULL;
	m_typeTok = this->m_token;

	//......在这里 + 用户自定义类型(typedef) + other module(regmode, syscmdmode, etc) 
	if ( m_typeTok.type > _VOID/*and !userdeftype*/) //not _INT, _CHAR, _DOUBLE, _ENUM, _TYPEDEF, _VOID,
	{
		this->WriteLog("Non-recognized syntax unit(declaration or main func expected): %s", m_typeTok.str);
		this->ScanTo(SEMI);
		return NULL;
	}

	if ( m_typeTok.type == _TYPEDEF)
	{
		t = DclTree();
		//......在这里将类型加入为type关键字
		return t;
	}

	m_idTok = m_token = m_pScanner->NextToken();
	if ( m_idTok.type != _ID)
	{
		this->WriteLog("Declaration error: %s %s", m_typeTok.str, m_idTok.str);
		return NULL;
	}
    
	m_token = m_pScanner->NextToken();
	if ( m_token.type == LPARAN )
		return FuncTree();
	if ( m_token.type == SEMI || m_token.type == LSQUARE || m_token.type == COMMA || m_token.type == ASSIGN)
		return DclTree();

#if 0
	//The following is used for further extension.
	if ( m_token.type == _RegMode )
		return m_pRParser->BuildSynTree;
	if ( m_token.type == _SysCmdMode )
		return m_pSysParser->BuildSynTree;
#endif

	this->WriteLog("Non-recognized top layer syntax unit: %s %s %s", m_typeTok.str, m_idTok.str, m_token.str);
	return NULL;
}

CSyntaxTree* CCParser::DclTree(void)
{
	CSyntaxTree * t = this->newNode(kVarDec, m_typeTok.type, m_idTok.str);
	
	if (m_token.type == LSQUARE)
	{
		m_token = m_pScanner->NextToken();
		if (m_token.type == RSQUARE)//char p[] = "..."
		{
			t->m_userData.bArray = 1;
			m_token = m_pScanner->NextToken();
			if ( Match(ASSIGN))
			{
				if ( this->m_typeTok.type != _CHAR || m_token.type != _STRING ) //non-format of: char p[] = "..."
				{
					WriteLog("Declaration format error: %s %s [] = %s", m_typeTok.str, m_idTok.str, m_token.str);
					this->ScanTo(SEMI);
					return t;
				}

				//CSyntaxTree * assgTree = this->newStmtNode(KAssg, m_token.str);
				CSyntaxTree * assgTree = this->newStmtNode(KAssg,"=");
				strcpy(assgTree->m_userData.szId, m_token.str);
				
				//the strlen of m_token.str includes 2 double-quotation-marks.
				t->m_userData.iArraySize = (int)strlen(m_token.str) - 2;
				t->child[0] = assgTree;
				assgTree->father = t;
				t->m_userData.nodekind = kVarAssgDec;//this is a special declaration with an assignment.
				m_token = m_pScanner->NextToken();
			}
			else
			{
				this->WriteLog( "the char array: %s %s[] is expected to be initialized.", m_typeTok.str, m_idTok.str);
				this->ScanTo(SEMI);
				return t;
			}
		}
		else
		{
			if ( Match(_NUM))
			{
				t->m_userData.bArray = 1;
				t->m_userData.iArraySize = atoi(m_token.str);//need to be converted by hand for octadata, hexadata.
			}

			Match(RSQUARE);
		}
	}
	else if( m_token.type == ASSIGN)
	{
		this->Match(ASSIGN);
		CSyntaxTree * assgTree = this->newStmtNode(KAssg,"=");
		strcpy(assgTree->m_userData.szId, m_token.str);
		t->child[0] = assgTree;
		assgTree->father = t;
		t->m_userData.nodekind = kVarAssgDec;//this is a special declaration with an assignment.
		m_token = m_pScanner->NextToken();
	}
	
	if ( m_token.type == COMMA) 
	{
		CSyntaxTree * t2 = NULL;
		m_idTok = m_token = m_pScanner->NextToken();
		if ( m_idTok.type != _ID)
		{
			this->WriteLog("Declaration error: %s %s", m_typeTok.str, m_idTok.str);
			this->ScanTo(SEMI);
		}
		else
		{
    		m_token = m_pScanner->NextToken();
			if ( m_token.type == SEMI || m_token.type == LSQUARE || m_token.type == COMMA )
				t2 = DclTree();
		}

		t->sibling = t2;
	}

	return t;
}

CSyntaxTree* CCParser::FuncTree(void)
{
	CSyntaxTree * t = this->newNode( kFunDec, m_typeTok.type, m_idTok.str);

	t->child[0] = Params();

	if ( !Match(RPARAN) )
	{
		this->WriteLog("Error format of function declaration.");
		this->ScanTo(SEMI);
		return t;
	}

	t->child[1] = CompoundStmt();
	
	return t;
}

CSyntaxTree* CCParser::Params(void)
{
	CSyntaxTree *t = NULL, *p;

	//assert(m_token.type == LPARAN);
	//this->Match(LPARAN);
	//if ( m_token.type == RPARAN || m_token.type == _VOID)
	//{
	//	t = this->newNode( kParam, _VOID, "");
	//	if ( m_token.type == _VOID )
	//		m_pScanner->NextToken();

	//	return t;
	//}
	//
	//while (m_token.type != RPARAN)
	//{
	//	m_typeTok = m_token;
	//	m_idTok = m_pScanner->NextToken();
	//	p = this->newNode(kParam, m_typeTok.type, m_idTok.str);
	//	if ( t == NULL)
	//		t = p;
	//	p = p->sibling;

	//	m_token = m_pScanner->NextToken();
	//	if ( m_token.type == LSQUARE )
	//	{
	//		this->m_pScanner->NextToken();
	//		if ( !this->Match(RSQUARE) )
	//		{
	//			this->ScanTo(RPARAN);
	//			return NULL;
	//		}			
	//		p->m_userData.bArray = TRUE;
	//		m_pScanner->NextToken();
	//	}

	//	if (m_token.type != COMMA && m_token.type != RPARAN)
	//		this->WriteLog("Error format of function params, comma or right-parenthese expected.");
	//	if ( m_token.type == COMMA)
 //           m_pScanner->NextToken();
	//}

	return t;
}

CSyntaxTree* CCParser::CompoundStmt(void)
{
	return NULL;
}

CSyntaxTree* CCParser::IfStmt(void)
{
	return NULL;
}

CSyntaxTree* CCParser::WhileStmt(void)
{
	return NULL;
}

CSyntaxTree* CCParser::ForStmt(void)
{
	return NULL;
}

CSyntaxTree* CCParser::AssignStmt(void)
{
	return NULL;
}

CSyntaxTree* CCParser::FunCallStmt(void)
{
	return NULL;
}

CSyntaxTree* CCParser::Exp(void)
{
	return NULL;
}

//*****************************************************************************//

void CCParser::PrintTree( CFileBin* pf, CSyntaxTree* pNode )
{
	int i, indent = 0;
	indent++;
	while( pNode != NULL ) {
		for( i = 0; i < indent; i++ ) Write( pf, "\t" );
		switch( pNode->m_userData.nodekind ) 
		{
		case kFunDec:
			Write( pf, "Function declaration: " );
			Write(pf, m_synStr,gC_ReservedKeywordList[(int)pNode->m_userData.tokenType] );
			Write( pf, " " );
			Write( pf, pNode->m_userData.szId );
			Write( pf, "\n" );
			break;
		case kVarDec:
		case kVarAssgDec:
			Write( pf, "Variable declaration: " );
			strcpy( m_synStr,gC_ReservedKeywordList[(int)pNode->m_userData.tokenType]); 
			Write( pf, m_synStr );
			Write( pf, " " );
			Write( pf, pNode->m_userData.szId );
			if( pNode->m_userData.bArray )	
				Write( pf, "[%d]", pNode->m_userData.iArraySize );
			if ( pNode->m_userData.nodekind == kVarAssgDec )
				Write( pf, "= %s", pNode->child[0]->m_userData.szId );
			Write( pf, "\n" );
			break;
		case kParam:
			Write( pf, "parameter: " );
			strcpy( m_synStr,gC_ReservedKeywordList[(int)pNode->m_userData.tokenType]); 
			Write( pf, m_synStr );
			Write( pf, " " );
			Write( pf, pNode->m_userData.szId );
			if( pNode->m_userData.bArray ) Write( pf, "[]" );
			Write( pf, "\n" );
			break;
		case kStmt:
			switch( pNode->m_userData.kind.stmt ) {
			case kRead:
				Write( pf, "call write(), args:\n" );
				break;
			case kWrite:
				Write( pf, "call read(), args:\n" );
				break;
			case kPrintf:
				Write( pf, "printf( \"%s\" )\n", pNode->m_userData.szId );
				break;
			case kCall:
				Write( pf, "call %s(), args:\n", pNode->m_userData.szId );
				break;
			case kIf:		Write( pf, "if\n" );				break;
			case kWhile:	Write( pf, "while\n" );			break;
			case kBreak:	Write( pf, "break\n" );			break;
			case kContinue:	Write( pf, "continue\n" );		break;
			case kReturn:	Write( pf, "return\n" );			break;
			default:		Write( pf, "Unknown node kind\n" );
			}
			break;
		case kExp:
			switch( pNode->m_userData.kind.exp ) {
			case kOp:
				Write( pf, "Op: %s\n", pNode->m_userData.szId );
				break;
			case kConst:
				Write( pf, "const: %s\n", pNode->m_userData.szId );
				break;
			case kID:
				Write( pf, "ID: %s", pNode->m_userData.szId );
				if( pNode->m_userData.bArray ) {
					pNode = pNode->child[0];
					Write( pf, "[%s]", pNode->m_userData.szId );
				}
				Write( pf, "\n" );
				break;
			default:
				Write( pf, "Unkown node kind\n" );
			}
			break;
		default:
			Write( pf, "Unkown node kind\n" );
		}

		for( i = 0; i < MAX_CHILDREN; i++ ) PrintTree( pf, pNode->child[i] );
		pNode = pNode->sibling;
	}
	indent--;
}
