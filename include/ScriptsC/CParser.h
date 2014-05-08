#ifndef __SCRIPTSC_CPARSER_H__
#define __SCRIPTSC_CPARSER_H__


#ifndef SCRIPTSC_DLL
	#define SCRIPTSC_DLL _declspec(dllimport)
#endif

#include "ScriptsBase/ParseFBase.h"
#include "toolfile/filebin.h"
#include "ScriptsBase/SyntaxTree.h"

class SCRIPTSC_DLL CCParser : public CParseFBase
{
public:
	CCParser(char * filename = NULL);
	virtual ~CCParser(void);

public:
	//auxiliary routines
	void PrintTree( CFileBin * pf, CSyntaxTree* t);

public:
	CSyntaxTree* Parse(void);
	CSyntaxTree* BuildSynTree(void);
	CSyntaxTree* GetSynUnit(void);
	CSyntaxTree* DclTree(void);
	CSyntaxTree* FuncTree(void);
	CSyntaxTree* Params(void);

public:
	CSyntaxTree* CompoundStmt(void);
	CSyntaxTree* IfStmt(void);
	CSyntaxTree* WhileStmt(void);
	CSyntaxTree* ForStmt(void);
	CSyntaxTree* AssignStmt(void);
	CSyntaxTree* FunCallStmt(void);
	CSyntaxTree* Exp(void);

public:
	CSyntaxTree* m_pProgram;
	
#if 0 //used in syntax parser for regmod module and syscommand module.
	CSyntaxTree* m_pMapParser;
	CRParser* m_pRParser; 
	CSysParser* m_pSysParser;
#endif

};

#endif
