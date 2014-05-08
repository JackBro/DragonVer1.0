#ifndef _TREEBASE_H_
#define _TREEBASE_H_


#ifndef TOOLTREE_DLL
	#define TOOLTREE_DLL  __declspec(dllimport)
#endif

#include "toolchain/chaintools.h"
#include "toolfile/filebin.h"

enum LEAFVALUETYPEenum
{
	LEAF_INTVALUE,
	LEAF_STRVALUE,
	LEAF_CONNECT,
	LEAF_INVALID
};

class TOOLTREE_DLL CLeafBase
{
public:
	CLeafBase(void);
	~CLeafBase(void);

public:
	void	SetLevel(int level);
	int		GetLevel(void);

protected:
	void	saveLeafInit(char *str);

private:
	int		m_level;
};

//==========================

class TOOLTREE_DLL CKeyLeaf : public CLeafBase
{
public:
	CKeyLeaf(void);
	virtual ~CKeyLeaf(void);

public:
	void SetKey(char *key, int val);
	void SetKey(char *key, char *vstr);
	int	 GetLeafMode(void);
	char *GetKey(void);
	int	 GetKeyVal(void);
	char *GetKeyStr(void);

public:
	void	saveLeaf(CFileBin *fb);

protected:
	void	saveFoldHeader(CFileBin *fb);
	void	saveFoldTail(CFileBin *fb);

protected:
	char	m_key[20];
	char	m_vstr[100];
	int		m_value;
	int		m_mode;
};

//======================================

class TOOLTREE_DLL CCntLeaf : public CKeyLeaf
{
public:
	CCntLeaf(void);
	virtual ~CCntLeaf(void);

public:
	void Connect(int src, int oupin, int dst, int inpin);
	void GetConnect(int *src, int *oupin, int *dst, int *inpin);

public:
	void	saveLeaf(CFileBin *fb);

private:
	int m_src;
	int m_outpin;
	int m_dst;
	int m_inpin;
};

//============================================

class TOOLTREE_DLL CKeyTree : public CKeyLeaf
{
public:
	CKeyTree(void);
	virtual ~CKeyTree(void);

public:
	int		GetLeafCount(void);
	CCntLeaf *GetLeaf(int index);
	CCntLeaf *GetLeaf(char *key);
	CCntLeaf *AddLeaf(void);
	void AddLeaf(char *key, int val);
	void AddLeaf(char *key, char *str);
	void	AddLeaf(int src, int oupin, int dst, int inpin);

	int		GetFoldCount(void);
//	CKeyTree *GetFold(int index);
	CKeyTree *GetFold(char *key, int val);
	CKeyTree *GetFold(char *key, char *str);
	CKeyTree *AddFold(void);
	CKeyTree *AddFold(char *key, int val);
	CKeyTree *AddFold(char *key, char *str);

public:
	void	Clear(void);

public:
	void	Save(CFileBin *fb);

private:
	CChainTool<CCntLeaf> m_leaf;
	CChainTool<CKeyTree> m_fold;
};

#endif

