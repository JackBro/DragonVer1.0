#ifndef __SCRIPTSBASE_SYNTAXTREE_H__
#define __SCRIPTSBASE_SYNTAXTREE_H__


#ifndef SCRIPTSBASE_DLL
	#define SCRIPTSBASE_DLL _declspec(dllimport)
#endif

#include "tooltree/MultiwayTree.h"


#define MAX_TOKEN_LENGTH 512

typedef enum { kVarDec, kVarAssgDec, kFunDec, kParam, kStmt, kExp, kRegMode, kSyscmdMode, kMapMode } SyntaxKind;
typedef enum { kIf, kWhile, kBreak, kContinue, kRead, 
				kWrite, kPrintf, kReturn, kCall, KAssg } StmtKind;
typedef enum { kOp, kConst, kID } ExpKind;

typedef struct
{
	int				lineno;
	SyntaxKind		nodekind;
	union 
	{
		int			stmt;//stmtkind / MapStmtKind
		int			exp;//expkind
	} kind;

	int				tokenType;
	char			szId[MAX_TOKEN_LENGTH + 1];
	int				bArray;					// is this an array declaration
	int				iArraySize;				// array size
}TreeData, *pTreeData;

//map module statement type
typedef enum { kFieldAssg, kSectionAssg, kVarAssg } MapStmtKind;

typedef CMultiwayTree<TreeData> CSyntaxTree;

#endif
