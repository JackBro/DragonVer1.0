#ifndef __SCRIPTSC_CSCANNER_H__
#define __SCRIPTSC_CSCANNER_H__


#ifndef SCRIPTSC_DLL
	#define SCRIPTSC_DLL _declspec(dllimport)
#endif

#include "ScriptsBase/ScannerBase.h"

enum TokenType
{
	// reserved Keyword
	 _INT, _CHAR, _DOUBLE, _ENUM, _TYPEDEF, _VOID, _RETURN, 
	 _IF, _ELSE, _FOR,_WHILE,_CONTINUE,_BREAK,_PRINTF, _MAIN,
	/*_AUTO, _SWITCH, _CASE,_REGISTER,_EXTERN,_UNION, _CONST, _FLOAT, _SHORT, _UNSIGNED,
	  _SIGNED, _LONG, _DEFAULT, _GOTO, _SIZEOF, _VOLATILE, _STRUCT, _DO, _STATIC, _READ, _WRITE,*/

	// operations
	ASSIGN, PLUS, MINUS, TIMES, DIV, MOD,
	BITWISE_AND, BITWISE_OR, BITWISE_NOT, LOGICAL_NOT, LT, GT,

	// interpunctions
	LPARAN, RPARAN, LBRACE, RBRACE, LSQUARE, RSQUARE, COMMA, DOT, SEMI, COLON, QUSTMARK,

	// complex operations
	EQ/* == */, NEQ/* != */, PLUS_PLUS/* ++ */, MINUS_MINUS/* -- */,
	PLUS_ASSIGN/* += */, MINUS_ASSIGN/* -= */, TIMES_ASSIGN/* *= */, DIV_ASSIGN/* /= */, MOD_ASSIGN /*%=*/,
	NGT/* <= */, NLT/* >= */, LOGICAL_AND/* && */, LOGICAL_OR/* || */,

	// others
	_ID, _NUM, _STRING, _CHARACTER, _DIRECTIVE, _EOF, _ERROR, _NONE
};


enum HeadFileKind{_SYSHEADFILE,_USERHEADFILE };
typedef struct 
{
	char fname[MAX_TOKEN_LENGTH+1];
	int filekind;
}THeadFileNode, *pTHeadFileNode;

typedef CMultiwayTree<THeadFileNode> CHeadFileTree;

class SCRIPTSC_DLL CCScanner : public CScanFBase
{
public:
	CCScanner(char * filename);
	virtual ~CCScanner(void);

	//void MapKeyword();
	TOKEN NextToken();	
	TOKEN ReadDecimal( int len);
	TOKEN ReadString(int len);
	TOKEN ProcOperator();
	TOKEN ProcDirective();
	int InfileMerge(char *infilename);
	void PrintHeadFileTree(CFileBin *file, int indent = 0);

private:
	TOKEN m_token;
	CTrie m_keywordTrie;
	CTrie m_defDirectiveTrie;
	CHeadFileTree*	 m_pHftree;
};


#endif
