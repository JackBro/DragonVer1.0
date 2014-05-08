#ifndef _SCANNERBASE_H_
#define _SCANNERBASE_H_


#ifndef SCRIPTSBASE_DLL
	#define SCRIPTSBASE_DLL _declspec(dllimport)
#endif


#include "toolchar/charfifo.h"
#include "tooltree/trie.h"
#include "SyntaxTree.h"
#include "ScriptsData.h"


enum SCANBASESTATEenum {
	SBIN_CHAR,
	SBIN_STRING,
	SBIN_IDENTIFIER,
	SBIN_HEXDATA,
	SBIN_DECDATA,
	SBIN_OCTDATA,
	SBIN_SPACER,//we need a one byte SBIN_SPACER to indicate a space seperator, or replace a deleted comments. e.g. "+/**/+" in c code. 
	SBIN_EOL,
	SBIN_EOF,
	SBIN_DEFAULT
};

enum SCRIPTMODEenum {
	SCRIPT_C,
	SCRIPT_REG,
	SCRIPT_MATLAB,
	SCRIPT_PERL,
	SCRIPT_SYSCMD
};

class SCRIPTSBASE_DLL CScannerBase : public CCharFifo
{
public:
	CScannerBase(int scriptMode = SCRIPT_C);
	virtual ~CScannerBase();

public:
	int NextToken();
	int DelComments(void);
	int SeekCommentsEnd( CTrie &EndTrie,  int tokstr_Idx, int bEolReplaced = FALSE );
	int ProcDigitalUnit();
	int ProcString();
	int ProcChar();

protected:
	void Clear(void);
	void PushBack(void);
	int IsHexSeq(char * str);
	int IsEol(char *str); 

public:
	char m_errorMsg[MAX_TOKEN_LENGTH + 100];
	int m_line;

protected:	
	int m_baseType;
	int m_scriptMode;
	int m_bEolIsSignificant; 
	int m_bPushedback;
	char m_tokstr[MAX_TOKEN_LENGTH+1];
	char m_peekstr[MAX_TOKEN_LENGTH+1];
	const TScanFlag *m_comments;
	int m_commentsSize;
	CTrie m_commentsHeadTrie;
};


typedef struct tag_TOKEN
{
	int type;	//enum TokenType
	char str[MAX_TOKEN_LENGTH+1];
	int len;
}TOKEN, *pTOKEN;


#endif
