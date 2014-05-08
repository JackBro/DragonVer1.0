
#include "internal.h"




CCScanner::CCScanner(char * filename):CScanFBase(filename)
{
	this->m_bEolIsSignificant = FALSE;

	//generate key word search tree---m_keywordTrie
	char typestr[sizeof(int)+1];
	for (int i=0; i<gC_ReservedKeywordListSize; i++)
	{
		sprintf(typestr, "%d", i);
		this->m_keywordTrie.Insert( gC_ReservedKeywordList[i], typestr);
	}

	//initialize head-file-tree's information
	m_pHftree = new CHeadFileTree;
	strcpy(m_pHftree->m_userData.fname, filename);
	m_pHftree->m_userData.filekind = _USERHEADFILE;
}

CCScanner::~CCScanner(void)
{
	delete m_pHftree;
}

TOKEN CCScanner::NextToken()
{
	int len = 0;
	char *str = CScannerBase::m_tokstr;
	this->m_token.type = _ERROR;
	memset( &this->m_token.str,  0, MAX_TOKEN_LENGTH+1);
	this->m_token.len = 0;
	char * keywordTypestr;

	len = CScannerBase::NextToken();
	m_token.len = len;
	strcpy(m_token.str, str);

	if ( len < 0 )
	{
		WriteLog();
		return m_token;
	}
	
	if ( 0 == len || m_token.str[0] < 0)
	{
		sprintf(this->m_errorMsg, "End of file reached");
		WriteLog();
		m_token.type = _EOF;
		return m_token;
	}

	if( m_token.str[0] == 1)
	{
		len = CScannerBase::NextToken();
		this->m_line = atoi(m_tokstr);

		//record headfile information:
		if ( this->m_line != 1 ) //scanner return from head file back to host file.
		{
			if ( this->m_pHftree->father )
			{
				sprintf(m_errorMsg, "comes here! %s %s", m_pHftree->m_userData.fname, m_pHftree->father->m_userData.fname);
				this->WriteLog();
				this->m_pHftree = this->m_pHftree->father;
			}
			else
			{
				printf(m_errorMsg, "Head file \"%s\"'s host file lost.", this->m_pHftree->m_userData.fname);
				this->WriteLog();
			}
		}

		len = CScannerBase::NextToken();
		m_token.len = len;
		strcpy(m_token.str, str);
	}
	
	if ( this->m_baseType == SBIN_STRING)
		return ReadString(len);

	if ( this->m_baseType == SBIN_CHAR )
	{
		m_token.type = _CHARACTER;
		return m_token;
	}

	if ( this->m_baseType == SBIN_IDENTIFIER )
	{
		m_token.type = _ID;

		keywordTypestr = m_keywordTrie.MatchWord(m_token.str);
		if ( keywordTypestr != NULL )
		{
			m_token.type = atoi(keywordTypestr);
			return m_token;
		}

		keywordTypestr = m_defDirectiveTrie.MatchWord(m_token.str);
		if ( keywordTypestr != NULL )
		{
			strcpy( m_token.str, keywordTypestr);
			m_token.type = _NUM; //to be extended, we only accept format:"#define string constant-num" currently.
			m_token.len = (int)strlen(m_token.str);
		}
		return m_token;
	}

	//double data or decimal interger
	if ( this->m_baseType == SBIN_DECDATA)
		return ReadDecimal(len);
	

	if ( this->m_baseType == SBIN_HEXDATA || this->m_baseType == SBIN_OCTDATA)
	{
		m_token.type = _NUM;
		return m_token;
	}

	if ( this->m_baseType == SBIN_DEFAULT)
		return ProcOperator();

	//if space or a new-line is read in, just go on next token
	if ( this->m_baseType == SBIN_SPACER)
		return this->NextToken();

	if ( this->m_baseType == SBIN_EOF)
	{
		m_token.type = _EOF;
		return m_token;
	}	

	return m_token;
}

TOKEN CCScanner::ReadDecimal(int len)  	//sample: 12.3e-12
{
	int len1;
	char * str = CScannerBase::m_tokstr;
	m_token.type = _NUM;
	m_token.len = len;
	strcpy(m_token.str, str);
	
	len1 = CScannerBase::NextToken();
	
	if( str[0] == '.')
	{
		strcpy(m_token.str+len, str);
		len += len1;
		m_token.len = len;
		len1 = CScannerBase::NextToken();
		if ( len1 < 0 )
		{
			sprintf(m_errorMsg, "Error fomat of num: %s%s", m_token.str, str);
			WriteLog();
			m_token.type = _ERROR;
			strcpy(m_token.str+len, str);
			return m_token;
		}

		if( this->m_baseType == SBIN_DECDATA || this->m_baseType == SBIN_OCTDATA)
		{
			strcpy(m_token.str+len, str);
			len += len1;
			m_token.len = len;		
			len1 = CScannerBase::NextToken();			
		}
	}

	if( strcmp( str, "e") == 0 ||strcmp( str, "E") == 0)
	{
		strcpy(m_token.str+len, str);
		len += len1;
		m_token.len = len;		
		len1 = CScannerBase::NextToken();
	}
	else if(str[0] =='e' || str[0] == 'E')  //eg. 1.1e01
	{
		unsigned int i;
		for(i = 1; i<strlen(str); i++ )
			if(!IsDigital(str[i]) )
			{
				sprintf(m_errorMsg, "Error fomat of exponent num: %s%s", m_token.str, str);
				WriteLog();
				m_token.type = _ERROR;
				strcpy(m_token.str+len, str);
				return m_token;
			}
			
		strcpy(m_token.str+len, str);
		len += len1;
		m_token.len = len;
		return m_token;
	}
	else if ( this->m_baseType == SBIN_IDENTIFIER)
	{
		m_token.type = _ERROR;
		strcpy(m_token.str+len, str);
		len += len1;
		m_token.len = len;
		sprintf(m_errorMsg, "Error fomat1 of decimal data: %s", m_token.str);
		return m_token;
	}
	else
	{
		CScannerBase::PushBack();
		return m_token;
	}

	if ( strcmp (str, "+") == 0 || strcmp (str, "-") == 0 )
	{
		strcpy(m_token.str+len, str);
		len += len1;
		m_token.len = len;		
		len1 = CScannerBase::NextToken();	
	}

	if ( (m_baseType == SBIN_DECDATA || m_baseType == SBIN_OCTDATA) && len1>0 )
	{
		strcpy(m_token.str+len, str);
		len += len1;
		m_token.len = len;	
	}
	else
	{
		sprintf(m_errorMsg, "Error fomat2 of decimal data: %s%s", m_token.str, str);
		m_token.type = _ERROR;
		//CScannerBase::PushBack();
		return m_token;
	}
		
	return m_token;
}

TOKEN CCScanner::ReadString( int len)
{
	int len1;
	char * str = CScannerBase::m_tokstr;
	m_token.type = _STRING;
	
	for(;;)
	{
		do
		{
			len1 = CScannerBase::NextToken();
			if ( 0 == len1)
			{
				sprintf(m_errorMsg, "EOF encountered while read in string :%s", m_token.str);//report error
				return m_token;
			}
		}
		while (this->m_baseType == SBIN_SPACER);

		if(this->m_baseType == SBIN_STRING && (len + len1 - 2) <= MAX_TOKEN_LENGTH)
			strcpy(m_token.str + len-1, str+1); //the end quatation mark of first string and the start quatation mark of next string should be deleted when concatenated.
		else
		{
			CScannerBase::PushBack();
			if (this->m_baseType == SBIN_STRING)
				sprintf(m_errorMsg, "too long string: %s", m_token.str);
			return m_token;
		}
		len += len1 - 2;
	}
			
}

TOKEN CCScanner::ProcDirective()
{
	assert (m_tokstr[0] == '#');

	int len1, len0 = 0;
	char infilename[MAX_FILE_NAME_LENGTH+1], tmpErrMsg[MAX_TOKEN_LENGTH+100], szDefStr[MAX_TOKEN_LENGTH+1], szDefVal[MAX_TOKEN_LENGTH+1];
	THeadFileNode tmpFilenode;

	memset(infilename, 0, MAX_FILE_NAME_LENGTH+1);
	memset(tmpErrMsg, 0, MAX_TOKEN_LENGTH+100);
	memset(szDefStr, 0, MAX_TOKEN_LENGTH+1);
	memset(szDefVal, 0, MAX_TOKEN_LENGTH+1);

	len1 = CScannerBase::NextToken();

	if ( strcmp (m_tokstr, "define") == 0 )
	{
		int len = 0;
		m_token.type = _DIRECTIVE;
		sprintf(m_token.str, "#define");

		do
		{
			len1 = CScannerBase::NextToken();
		}
		while(this->m_baseType == SBIN_SPACER);
		
		if (m_tokstr[0] != '\n')
			strcpy( szDefStr, m_tokstr);
		else
			return m_token;
		
		do
		{
			len1 = CScannerBase::NextToken();
		}
		while(this->m_baseType == SBIN_SPACER);

		while(m_tokstr[0] != '\n')
		{
			strcpy( szDefVal + len, m_tokstr);
			len += len1;
			
			if (len >= MAX_TOKEN_LENGTH)
				break;

			len1 = CScannerBase::NextToken();
		}
		
		//insert defined-string into CTrie-map for later replace.
		m_defDirectiveTrie.Insert( szDefStr, szDefVal );

		return m_token;
	}

	else if ( strcmp(m_tokstr, "include") == 0 )
	{
		m_token.type = _DIRECTIVE;
		strcpy(m_token.str, "#include");

		//read in file to m_buf
		do
		{
			len1 = CScannerBase::NextToken();
		}
		while(this->m_baseType == SBIN_SPACER);

		if ( this->m_baseType != SBIN_STRING && m_tokstr[0] != '<')
		{
			m_token.type = _ERROR;
			strcpy(tmpErrMsg, m_tokstr);
		
			while ( m_tokstr[0] != '\n')
			{
				CScannerBase::NextToken();
				if(m_tokstr[0] <= 0) //EOF
					return m_token;
			}

			sprintf(m_errorMsg, "Directive format error in str: %s", tmpErrMsg);
			return m_token;
		}

		if (this->m_baseType == SBIN_STRING)//"infilename"
		{
			strncpy(infilename, m_tokstr+1, strlen(m_tokstr)-2);
			tmpFilenode.filekind = _USERHEADFILE;
		}
		else //<infilename>
		{
			len1 = CScannerBase::NextToken();
			while( m_tokstr[0] != '>')
			{
				strcpy(infilename+len0, m_tokstr);
				len0 += len1;
				len1 = CScannerBase::NextToken();
				if (m_tokstr[0] == '\n' || m_tokstr[0] <= 0 )
				{
					m_token.type = _ERROR;
					return m_token;
				}
			}
			tmpFilenode.filekind = _SYSHEADFILE;
		}

		strcpy(tmpFilenode.fname, infilename);
		if ( m_pHftree->child[0])
            m_pHftree = m_pHftree->child[0]->AddSibNode( tmpFilenode);
		else
		{
			m_pHftree->child[0] = m_pHftree->NewNode(tmpFilenode);
			m_pHftree->child[0]->father = m_pHftree;
			m_pHftree = m_pHftree->child[0];
		}

		//becareful, we use m_peekstr here, to make sure the cycle stops at the read-point located 
		//immediately following the newline '\n'.
		while ( m_peekstr[0] != '\n') 
		{
			CScannerBase::NextToken();
			if(m_tokstr[0] <= 0) //EOF
				break;
		}
		
		if (!InfileMerge(infilename))
		{
			if ( m_pHftree->father) m_pHftree = m_pHftree->father;
			m_token.type = _ERROR;
		}
		return m_token;
	}
	
	else
		m_token.type = _ERROR;//print error messages

	return m_token;
}

int CCScanner::InfileMerge(char *infilename)
{
	int len0, len1, len, pos = CFifoBuf::GetReadPoint();
	struct stat statbuf;
	CBufferBase ifbuf;
	char strline[5+1];
	
	if (stat (infilename, &statbuf) == -1) 
	{
		sprintf(m_errorMsg, "Error occurs when stat file %s.", infilename);
		return 0;
	}
	
	len0 = statbuf.st_size;	
	if (len0 == -1) 
	{
		sprintf(m_errorMsg, "Error occurs when stat file %s.", infilename);
		return 0;
	}

	//load scanned part of main source file
	len1 = CCharFifo::GetTotalSize();
	ifbuf.SetSize(len0+len1+12);
	CFifoBuf::SetReadPoint(0);
	len = CFifoBuf::Read(ifbuf.m_buf, pos);

	//The mark here indicates the beginging of a new include file, and a new counted m_line.
	ifbuf.m_buf[len] = 1;
	strcpy(ifbuf.m_buf+len+1, "1 ");
	len += 3;
	
	//load included file
	CFileBin ifile(infilename, "rb");
	len0 = ifile.Read(ifbuf.m_buf+len, len0);
	if (len0 == 0)
		sprintf(m_errorMsg, "Error occurs when read file %s", infilename);

	//the mark here indicates the oringinal line number.
	ifbuf.m_buf[len+len0] = '\n';
	ifbuf.m_buf[len+len0+1] = 1;
	sprintf(strline, "%d ", this->m_line);
	strcpy(ifbuf.m_buf+len+len0+2,strline);
	len += 2 + (int)strlen(strline);

	//load the remainder of main source file.
	CFifoBuf::SetReadPoint(pos-1);//go back for 1 char to retrieve '\n' character.
	len = CFifoBuf::Read(ifbuf.m_buf+len+len0, len1 - pos + 4);

	if (len0 + len1 + 12 > CCharFifo::m_totalsize) 
		CCharFifo::SetSize(len0+len1+12);
	
	CCharFifo::Write(ifbuf.m_buf, len0+len1+12);
	CFifoBuf::SetReadPoint(pos);
	return 1;
}

TOKEN CCScanner::ProcOperator()
{
	int len1 = 0;
	char * keywordTypestr, *str = CScannerBase::m_tokstr;

	keywordTypestr = m_keywordTrie.MatchWord(m_token.str);
	if ( keywordTypestr != NULL )
		m_token.type = atoi(keywordTypestr);
	else
		m_token.type = _NONE;

	switch (str[0])
	{
	case '+':
		len1 = CScannerBase::NextToken();
		//Note: the following code should be used in syntax parser.
		//if (this->m_baseType == SBIN_DECDATA || this->m_baseType == SBIN_HEXDATA ||this->m_baseType == SBIN_OCTDATA)
		//{
		//	PushBack();
		//	return this->NextToken();
		//}
		if (m_tokstr[0] == '+')
		{
			sprintf ( m_token.str, "++");
			m_token.len = 2;
			m_token.type = PLUS_PLUS;
			return m_token;
		}
		if (m_tokstr[0] == '=')
		{
			sprintf ( m_token.str, "+=");
			m_token.len = 2;
			m_token.type = PLUS_ASSIGN;
			return m_token;			
		}		
		else
		{
			PushBack();
			return m_token;
		}
		
	case '-':
		len1 = CScannerBase::NextToken();
		//Note: the following code should be used in syntax parser.
		//if (this->m_baseType == SBIN_DECDATA || this->m_baseType == SBIN_HEXDATA ||this->m_baseType == SBIN_OCTDATA)
		//{
		//	PushBack();
		//	this->NextToken();
		//	strcpy(tmpstr, m_token.str);
		//	sprintf( m_token.str, "-%s", tmpstr); 
		//	m_token.len += 1;
		//	return m_token;
		//}
		if (m_tokstr[0] == '-')
		{
			sprintf ( m_token.str, "--");
			m_token.len = 2;
			m_token.type = MINUS_MINUS;
			return m_token;
		}
		if (m_tokstr[0] == '=')
		{
			sprintf ( m_token.str, "-=");
			m_token.len = 2;
			m_token.type = MINUS_ASSIGN;
			return m_token;			
		}		
		else
		{
			PushBack();
			return m_token;
		}
		
	case '*':
		len1 = CScannerBase::NextToken();
		if (m_tokstr[0] == '=')
		{
			sprintf ( m_token.str, "*=");
			m_token.len = 2;
			m_token.type = TIMES_ASSIGN;
			return m_token;			
		}		
		else
		{
			PushBack();
			return m_token;
		}
		
	case '/':
		len1 = CScannerBase::NextToken();
		if (m_tokstr[0] == '=')
		{
			sprintf ( m_token.str, "/=");
			m_token.len = 2;
			m_token.type = DIV_ASSIGN;
			return m_token;			
		}		
		else
		{
			PushBack();
			return m_token;
		}
		
	case '%':
		len1 = CScannerBase::NextToken();
		if (m_tokstr[0] == '=')
		{
			sprintf ( m_token.str, "%=");
			m_token.len = 2;
			m_token.type = MOD_ASSIGN;
			return m_token;			
		}		
		else
		{
			PushBack();
			return m_token;
		}
		
	case '=':
		len1 = CScannerBase::NextToken();
		if (m_tokstr[0] == '=')
		{
			sprintf ( m_token.str, "==");
			m_token.len = 2;
			m_token.type = EQ;
			return m_token;			
		}		
		else
		{
			PushBack();
			return m_token;
		}

	case '!':
		len1 = CScannerBase::NextToken();
		if (m_tokstr[0] == '=')
		{
			sprintf ( m_token.str, "!=");
			m_token.len = 2;
			m_token.type = NEQ;
			return m_token;			
		}		
		else
		{
			PushBack();
			return m_token;
		}
		
	case '<':
		len1 = CScannerBase::NextToken();
		if (m_tokstr[0] == '=')
		{
			sprintf ( m_token.str, "<=");
			m_token.len = 2;
			m_token.type = NGT;
			return m_token;			
		}		
		else
		{
			PushBack();
			return m_token;
		}

	case '>':
		len1 = CScannerBase::NextToken();
		if (m_tokstr[0] == '=')
		{
			sprintf ( m_token.str, ">=");
			m_token.len = 2;
			m_token.type = NLT;
			return m_token;			
		}		
		else
		{
			PushBack();
			return m_token;
		}
		
	case '&':
		len1 = CScannerBase::NextToken();
		if (m_tokstr[0] == '&')
		{
			sprintf ( m_token.str, "&&");
			m_token.len = 2;
			m_token.type = LOGICAL_AND;
			return m_token;			
		}		
		else
		{
			PushBack();
			return m_token;
		}
		
	case '|':
		len1 = CScannerBase::NextToken();
		if (m_tokstr[0] == '|')
		{
			sprintf ( m_token.str, "||");
			m_token.len = 2;
			m_token.type = LOGICAL_OR;
			return m_token;			
		}		
		else
		{
			PushBack();
			return m_token;
		}

	case '#':
		return ProcDirective();

	default:
		return m_token;
	}
}

//the parameter of indent must be 0 if this function is firstly invoked.
void CCScanner::PrintHeadFileTree(CFileBin* file, int indent)
{
	//0 value of indent means:1.the m_pHftree is located in first layer of the tree; 2. the PrintHeadFile is firstly invoked.
	if (indent == 0 && m_pHftree)
	{
		//get root of m_pHftree
		while (m_pHftree->father)
			m_pHftree = m_pHftree->father;
	
		indent++;
	}	

	while(m_pHftree)
	{
		for ( int i = 0; i < indent; i++) file->Write("\t", 1);
		file->Write(m_pHftree->m_userData.fname, (int)strlen(m_pHftree->m_userData.fname) );
		file->Write("\n", 1);

		//#include <filename>. if filename is included using<>,its inner included head file shouldn't be printed.
		if ( m_pHftree->m_userData.filekind == _SYSHEADFILE)
		{
			m_pHftree = m_pHftree->sibling;
			PrintHeadFileTree(file, indent);
			return;
		}

		//no inner included file in the current source file.
		if ( !m_pHftree->child[0] )
		{
			m_pHftree = m_pHftree->sibling;
			PrintHeadFileTree(file, indent);
			return;
		}

		CHeadFileTree* t = m_pHftree;
		m_pHftree = m_pHftree->child[0];            
		indent++;
		PrintHeadFileTree(file, indent);
        
		//child tree printing finished, continue to print father's sibling's tree
		indent--;
		m_pHftree= t->sibling;

		PrintHeadFileTree(file, indent);
	}

}
