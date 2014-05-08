#ifndef _NODEBASE_H_
#define _NODEBASE_H_


#ifndef TOOLTREE_DLL
	#define TOOLTREE_DLL  __declspec(dllimport)
#endif


enum KEYNODEPROPenum
{
	LEAF_NODE,
	NODE_NODE
};

class TOOLTREE_DLL CNodeBase
{
public:
	CNodeBase(void);
	virtual ~CNodeBase(void);

public:
	int  GetNodeProp(void);
	void SetNodeProp(int sel);

private:
	int m_prop;
};


#endif

