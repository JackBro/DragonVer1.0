#include "internal.h"



#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

CScannerBase::CScannerBase(int scriptmode)
{
	this->Clear();
	this->m_scriptMode = scriptmode;
	this->m_bEolIsSignificant = TRUE;
	this->m_bPushedback = FALSE;
	strcpy ( this->m_peekstr, " " );

	if(scriptmode == SCRIPT_C)
	{		
		m_comments = gC_Comments;
		m_commentsSize = gC_CommentsSize;
	}
	else // temporary
	{
		m_comments = gC_Comments;
		m_commentsSize = gC_CommentsSize;
	}

	for (int i=0; i<m_commentsSize; i++)
		m_commentsHeadTrie.Insert(m_comments[i].start, m_comments[i].start);
}

CScannerBase::~CScannerBase()
{	
}

void CScannerBase::Clear()
{
	this->m_line = 1;
}

void CScannerBase::PushBack()
{
	m_bPushedback = TRUE;
}

int CScannerBase::IsHexSeq(char * str)
{
	//0-9, a~f, A~F
	size_t len = strlen(str), rt = 1;
	for ( unsigned int i = 0; i < len; i++)
		if ( (str[i] <= '9' && str[i] >= '0') ||(str[i] >= 'a' && str[i] <= 'f') || (str[i] >= 'A' && str[i] <= 'F') )
			;
		else
		{
			rt = 0;
			break;
		}

	return (int)rt;
}

int CScannerBase::IsEol(char* str) 
{
	if( str[0] == '\n') 
		return TRUE; 

	else if( str[0] == '\r')
	{
		CCharFifo::Read(str, MAX_TOKEN_LENGTH);
		if ( str[0] == '\n' ) 
			return TRUE; 
	}
	return FALSE;
}

int CScannerBase::NextToken()
{
	if (m_bPushedback)
	{
		m_bPushedback = FALSE;
		return (int)strlen(m_tokstr);
	}
	
	char * str = m_tokstr;
	int len = 0, size = MAX_TOKEN_LENGTH;
	BOOL bEol = FALSE;

	this->m_baseType = SBIN_DEFAULT;
	memset(m_errorMsg, 0, sizeof(m_errorMsg));
	memset(m_tokstr, 0, sizeof(m_tokstr));
	strcpy(str, m_peekstr);
	len = (int)strlen(str);

	if ( 0 == len || m_peekstr[0] <= 0)
	{
		this->m_baseType = SBIN_EOF;
		return 0; //EOF encountered.
	}

	//scan comments first.
	if ( -1 == this->DelComments() )
		return -1;
	
	//process constant data
	if ( this->IsDigital(str[0]) )
		return this->ProcDigitalUnit();
        
	if ( this->IsAlpha(str[0]) )
	{
		this->m_baseType = SBIN_IDENTIFIER;
		memset(m_peekstr, 0, sizeof(m_peekstr));
		CCharFifo::Read(m_peekstr, size);

		return len;
	}

	//char
	if ( str[0] == '\'')
		return ProcChar();

	//string
	if ( str[0] == '"')
		return ProcString();
	

	if ( this->IsBlank(str[0]) )
	{
		this->m_baseType = SBIN_SPACER;
		memset(m_peekstr, 0, sizeof(m_peekstr));
		CCharFifo::Read(m_peekstr, size);		
		str[1] = 0;
		return 1;
	}

	bEol = IsEol(str);
	if (bEol)
	{
		this->m_line++;	
		if (m_bEolIsSignificant)
			this->m_baseType = SBIN_EOL;
		else
			this->m_baseType = SBIN_SPACER;

		memset(m_peekstr, 0, sizeof(m_peekstr));
		CCharFifo::Read(m_peekstr, size);
		return len;
	}

	memset(m_peekstr, 0, sizeof(m_peekstr));
	CCharFifo::Read(m_peekstr, size);
	return len;
}

int CScannerBase::DelComments(void)
{	
	int bIsComents = FALSE;
	int i = 0, len, idx = 0;
	int recovery_rpoint = CFifoBuf::GetReadPoint();	
	int recovery_line = m_line;
	CTrie endTrie;
	TrieNode * pSearchNode;

	pSearchNode = m_commentsHeadTrie.GetRoot()->first;
	len = (int)strlen(m_tokstr);

	while( idx < len )
	{
		pSearchNode = m_commentsHeadTrie.MatchPrefix(m_tokstr[idx], pSearchNode);
		idx++;

		if (NULL == pSearchNode)//not match a comment head
			break;

		if(pSearchNode->kind == LEAF)//find a comment head
			bIsComents = TRUE;

		if (idx >= len)
		{
			memset(m_tokstr, 0, sizeof(m_tokstr));
			len = CCharFifo::Read(m_tokstr, MAX_TOKEN_LENGTH);
			idx = 0;	
			
			if (m_tokstr[0] <= 0 || len == 0)
				break;/*EOF*/
			
			if ( IsEol(m_tokstr) )				
				this->m_line++;
		}

		if ( bIsComents )
			break;
	}

	if (!bIsComents)
	{
		//non-comments, recovery to original status.
		CFifoBuf::SetReadPoint(recovery_rpoint);	
		strcpy(m_tokstr, m_peekstr);
		this->m_line = recovery_line;
		return 0;
	}

	//find the comment head, now. continue to find the correspond end:
	for (i = 0; i < m_commentsSize; i++)
	{
		if ( 0 == strcmp( pSearchNode->word, m_comments[i].start))
			endTrie.Insert(m_comments[i].stop, m_comments[i].stop);	
	}

	//continue to search the comment-end in the remainder of m_tokstr.
	idx = SeekCommentsEnd( endTrie, idx, 0);
	if (idx != 0)
	{
		if (idx == (int)strlen(m_tokstr))
		{
			memset(m_tokstr, 0, MAX_TOKEN_LENGTH+1);
			CCharFifo::Read(m_tokstr, MAX_TOKEN_LENGTH);
		}
		else
		{
			char tmpStr[MAX_TOKEN_LENGTH];
			strcpy(tmpStr, m_tokstr + idx);
			strcpy(m_tokstr, tmpStr);
		}
		strcpy(m_peekstr, m_tokstr);

        //before return a successful information, retry to see if it is a correct comments-head immediatly after current position.
		pSearchNode = m_commentsHeadTrie.GetRoot()->first;
		pSearchNode = m_commentsHeadTrie.MatchPrefix(m_peekstr[0], pSearchNode);
		if ( NULL != pSearchNode )
			return this->DelComments();

		return idx;
	}

#if 0
	//encounter EOF, recovery to original status.
	CFifoBuf::SetReadPoint(recovery_rpoint);
	this->m_line = recovery_line;

	//then read in error comments-head
	memset(m_tokstr, 0, sizeof(m_tokstr));
	CCharFifo::Read(m_tokstr, MAX_TOKEN_LENGTH);
#endif

	//EOF encountered. Still read in EOF to be parsed.
	memset(m_peekstr, 0, sizeof(m_peekstr));
	CCharFifo::Read(m_peekstr, MAX_TOKEN_LENGTH);
	strcpy(m_tokstr, m_peekstr);

	sprintf( this->m_errorMsg, "Error format of comments, comments end not found.");
	return -1;
}


#if 0
int CScannerBase::delComments(void)
{
	char * commentDelimeter1, *commentDelimeter2, oriStrHead[size*2], *tmpDelimeter = 0;
	if ( this->m_scriptMode == SCRIPT_C )
	{
		for (i=0; i<gC_CommentsSize; i++)
			if( (tmpDelimeter = strstr(str, gC_Comments[i].start) ) != NULL )
			{
				if (commentDelimeter1 == 0)
				{
					icomment_type = i;
					commentDelimeter1 = tmpDelimeter;
				}
				if (tmpDelimeter < commentDelimeter1) //find out former occurence of start-of-commets-delimeter,e.g. //*+/*/
				{
					icomment_type = i;
					commentDelimeter1 = tmpDelimeter;
				}
			}

		if( commentDelimeter1 != NULL )
		{
			this->m_baseType = SBIN_COMMENTS;
			strncpy( oriStrHead, str, commentDelimeter1 - str ); //some type data other than SBIN_COMMENTS may exists in str's head

			commentDelimeter2 = strstr( commentDelimeter1 + strlen(gC_Comments[icomment_type].start), gC_Comments[icomment_type].stop );
			//if end-of-comment-delimeter exists in str too.
			if ( NULL != commentDelimeter2 )
			{
				char * tmpstr[strlen(str)];
				strcpy(tmpstr, commentDelimeter2 + strlen(gC_Comments[icomment_type].stop) );
				strcpy(commentDelimeter1, tmpstr);//now, comments in str is deleted.

				if ( '\0' == str[0] ) //str was right a complete comments itself, then it's null string now.
					return len;
				//else go on to process str.
			}
			else //keep read in data until 
				while (1)
				{
					len += CCharFifo::Read(str, size);
					
					bEol = IsEol(str);
					if(bEol)
						this->m_line++;
			
					if( (commentDelimeter2 = strstr(str, gC_Comments[icomment_type].stop)) != NULL)
					{
						strcpy(oriStrHead + strlen(oriStrHead), commentDelimeter2 + strlen(gC_Comments[icomment_type].stop) );
						strcpy(str, oriStrHead);
						if( 0 == str) //no other type of data except SBIN_COMMENTS exists in original str
							return len;
						else 
							break; //else go on to process str's new contents outside "for" loop.
					}

					if (str[0] <= 0 /*EOF*/)
					{
						;//report error messages here
						return 0;
					}
				}
			
		}
	}
	else 
	{
		//other script mode here.
	}
	
	return 1;
}
#endif

int CScannerBase::SeekCommentsEnd(CTrie &EndTrie,  int tokstr_Idx, int bEolReplaced)
{
	ASSERT(tokstr_Idx >= 0);
	TrieNode * pSearchNode = EndTrie.GetRoot()->first;
	int len = (int)strlen(m_tokstr), idx = tokstr_Idx, len0 = len;
	int recovery_rpoint = CFifoBuf::GetReadPoint();	
	int recovery_line = m_line;
	char *recoverystr = new char[len+1];
	strcpy(recoverystr, m_tokstr);

	while(idx < len) 
	{
		pSearchNode = EndTrie.MatchPrefix(m_tokstr[idx], pSearchNode);
		idx++;

		if (NULL == pSearchNode)
		{
			//return to the position next to the original point, seek comments-end again.
			CFifoBuf::SetReadPoint(recovery_rpoint);
			m_line = recovery_line;

			if (tokstr_Idx < len0-1)
			{
				idx = tokstr_Idx + 1;
				strcpy(m_tokstr, recoverystr);
				delete recoverystr;
				recoverystr = 0;
			}
			else
			{
				memset(m_tokstr, 0, sizeof(m_tokstr));
				CCharFifo::Read(m_tokstr, MAX_TOKEN_LENGTH);
				idx = 0;

				if( m_tokstr[0] <= 0 /*EOF*/)
				{
					if(bEolReplaced)
						return 0;
					else
					{
						//in case of end-of-comments is '\n', we replace EOF with EOL in the first time.
						strcpy(m_tokstr, "\n");
						bEolReplaced = TRUE;
					}
				}

				if (IsEol(m_tokstr))
					m_line++;
			}			
			return SeekCommentsEnd( EndTrie, idx, bEolReplaced);
		}

		if( pSearchNode->kind == LEAF)//find a comment end
			return idx;	//return matched number of bytes in current m_tokstr;

		if (idx >= len)
		{
			memset(m_tokstr, 0, sizeof(m_tokstr));
			len = CCharFifo::Read(m_tokstr, MAX_TOKEN_LENGTH);
			idx = 0;	
			if(m_tokstr[0] <= 0 /*EOF*/)
				break;

			if ( IsEol(m_tokstr) )
				this->m_line++;
		}
	}

	return 0;
}


int CScannerBase::ProcDigitalUnit()
{
	char *str = this->m_tokstr;
	int len = (int)strlen(str), len1, i;

	ASSERT(this->IsDigital(str[0]));

	if(str[0] != '0')
	{
		this->m_baseType = SBIN_DECDATA;
		memset(m_peekstr, 0, MAX_TOKEN_LENGTH+1);
		CCharFifo::Read(m_peekstr, MAX_TOKEN_LENGTH);
	}

	else if ( len > 1) 
	{
		for ( i=0; i<len; i++ )
			if ( str[i] < '0' || str[i] >= '8')
			{
				sprintf(m_errorMsg, "error format of octbal number: %s. ", str);
				memset(m_peekstr, 0, MAX_TOKEN_LENGTH+1);
				CCharFifo::Read(m_peekstr, MAX_TOKEN_LENGTH);
				return -1;
			}
		this->m_baseType = SBIN_OCTDATA;
		memset(m_peekstr, 0, MAX_TOKEN_LENGTH+1);
		CCharFifo::Read(m_peekstr, MAX_TOKEN_LENGTH);
	}
	else
	{
		memset(m_peekstr, 0, MAX_TOKEN_LENGTH+1);
		len1 = CCharFifo::Read(m_peekstr, MAX_TOKEN_LENGTH);
		len += len1;

		if( m_peekstr[0] == 'x'|| m_peekstr[0] == 'X')
		{
			if(!IsHexSeq(m_peekstr+1))
			{
				sprintf(m_errorMsg, "error fomat of hexdecimal number: 0%s", m_peekstr); 
				sprintf(str, "0%s", m_peekstr);	
				memset(m_peekstr, 0, MAX_TOKEN_LENGTH+1);
				CCharFifo::Read(m_peekstr, MAX_TOKEN_LENGTH);					
				return -1;					
			}

			do
			{										
				sprintf(str, "%s", m_peekstr);	
				memset(m_peekstr, 0, MAX_TOKEN_LENGTH+1);
				str += len1;
				len1 = CCharFifo::Read(m_peekstr, MAX_TOKEN_LENGTH);
				len += len1;
			}
			while (IsHexSeq(m_peekstr));

			char tmpstr[MAX_TOKEN_LENGTH+1];
			strcpy(tmpstr, m_tokstr);
			sprintf(m_tokstr, "0%s", tmpstr);	
			len = len - len1 +1;
			this->m_baseType = SBIN_HEXDATA;		
			//return len;
		}

		else if ( IsAlpha(m_peekstr[0]) )
		{
			sprintf(m_errorMsg, "error fomat of identifier: 0%s", m_peekstr); 
			sprintf(str, "0%s", m_peekstr);	
			memset(m_peekstr, 0, MAX_TOKEN_LENGTH+1);
			CCharFifo::Read(m_peekstr, MAX_TOKEN_LENGTH);					
			return -1;
		}

		else //decimal data: 0
		{				
			this->m_baseType = SBIN_DECDATA;
			len = 1;
			//return len;
		}
	}

	if ( IsAlpha(m_peekstr[0]) && m_peekstr[0] != 'e' && m_peekstr[0] != 'E')
	{
		sprintf(m_errorMsg, "Base Layer - Error fomat of num: %s%s", m_tokstr, m_peekstr);
		memset(m_peekstr, 0, MAX_TOKEN_LENGTH+1);
		CCharFifo::Read(m_peekstr, MAX_TOKEN_LENGTH);
		return -1;
	}

	return len;
}

int CScannerBase::ProcString()
{
	ASSERT(m_tokstr[0] == '"');

	char * str = m_tokstr;
	int len = 1, len1, size = MAX_TOKEN_LENGTH;

	memset(m_peekstr, 0, sizeof(m_peekstr));
	len1 = CCharFifo::Read(m_peekstr, size);

	while( !strchr(m_peekstr, '"') )
	{
		if ( m_peekstr[0] == '\\')
		{
			strcpy(str + len, m_peekstr);
			len += len1;	
			memset(m_peekstr, 0, MAX_TOKEN_LENGTH+1);
			len1 = CCharFifo::Read(m_peekstr, size);
			if(IsEol(m_peekstr))
			{
				this->m_line++; 
				if(m_bEolIsSignificant) 
				{
					sprintf( m_errorMsg, "The end of string(\") not found.");
					return -1;
				}
			}
		}

		strcpy(str + len, m_peekstr);
		len += len1;	
		memset(m_peekstr, 0, MAX_TOKEN_LENGTH+1);
		len1 = CCharFifo::Read(m_peekstr, size);
		
		if(IsEol(m_peekstr))
		{
			this->m_line++; 
			if(m_bEolIsSignificant) 
			{
				sprintf( m_errorMsg, "The end of string(\") not found.");
				return -1;
			}
		}

		if(m_peekstr[0] <= 0)
		{
			this->m_baseType = SBIN_EOF;
			CCharFifo::Read(m_peekstr, size);
			sprintf(m_errorMsg, "EOF encountered when reading string.");
			return -1;
		}

	}

	this->m_baseType = SBIN_STRING;
	strcpy(str + len, m_peekstr);
	memset(m_peekstr, 0, MAX_TOKEN_LENGTH+1);
	CCharFifo::Read(m_peekstr, size);
	return len+len1;
}

int CScannerBase::ProcChar()
{
	ASSERT(m_tokstr[0] == '\'');

	char * str = m_tokstr;
	int len1 = 0, size = MAX_TOKEN_LENGTH;

	memset(m_peekstr, 0, sizeof(m_peekstr));
	len1 = CCharFifo::Read(m_peekstr, size);
	str[1] = m_peekstr[0];
	if ( m_peekstr[0] == '\'')
	{
		this->m_baseType = SBIN_CHAR;
		memset(m_peekstr, 0, sizeof(m_peekstr));
		CCharFifo::Read(m_peekstr, size);
        return 2;
	}

	if( m_peekstr[0] == '\\') 
	{
		memset(m_peekstr, 0, sizeof(m_peekstr));
		len1 = CCharFifo::Read(m_peekstr, size);
		if ( len1 > 1 )
		{
			sprintf(m_errorMsg, "Error format of escape char: '\\%s'.", m_peekstr);
			return -1;
		}
		if ( IsEol(m_peekstr))
			this->m_line++;
		
		str[2] = m_peekstr[0];

  //      switch(m_peekstr[0])
		//{
		//case 'n': 
		//case 'r':        
		//case 't':        
		//case 'a':        
		//case 'b':        
		//case 'f':        
		//case 'v':		
		//case '\'':       
		//case '\"':       
		//case '\\': 
		//case '0':
		//case '1':
		//case '2':
		//case '3':
		//case '4':
		//case '5':
		//case '6':
		//case '7':
		//	break;
		//default:
		//	sprintf(m_errorMsg, "Error format of escape char: '\\%c'", m_peekstr[0]);
		//	return -1;
		//	break;
		//}

		memset(m_peekstr, 0, sizeof(m_peekstr));
		CCharFifo::Read(m_peekstr, size);
		if ( IsEol(m_peekstr))
			this->m_line++;
		
		if ( m_peekstr[0] != '\'' )
		{
			sprintf(m_errorMsg, "Error format of escape char: '\\%c'", m_peekstr[0]);
			return -1;
		}

		str[3] = m_peekstr[0];
		this->m_baseType = SBIN_CHAR;
		memset(m_peekstr, 0, sizeof(m_peekstr));
		CCharFifo::Read(m_peekstr, size);
		if ( IsEol(m_peekstr))
			this->m_line++;

		return 4;
	}
	
	if ( len1 > 1 )
	{
		sprintf(m_errorMsg, "Error format of char: '%s.", m_peekstr);
		return -1;
	}
	if ( IsEol(m_peekstr))
		this->m_line++;

	memset(m_peekstr, 0, sizeof(m_peekstr));
	len1 = CCharFifo::Read(m_peekstr, size);
	if ( IsEol(m_peekstr))
		this->m_line++;

	if ( m_peekstr[0] == '\'')
	{
		str[2] = '\'';
		this->m_baseType = SBIN_CHAR;

		memset(m_peekstr, 0, sizeof(m_peekstr));
		CCharFifo::Read(m_peekstr, size);
		
		return 3;
	}		
	else 
	{
		sprintf(m_errorMsg, "Error format of constant char: '%s", m_peekstr);
		memset(m_peekstr, 0, sizeof(m_peekstr));
		CCharFifo::Read(m_peekstr, size);

		return -1;
	}
}