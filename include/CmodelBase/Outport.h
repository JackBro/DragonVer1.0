#ifndef _CMODEL_OUTPORT_H_
#define _CMODEL_OUTPORT_H_


#ifndef CMODELBASE_DLL
	#define CMODELBASE_DLL  __declspec(dllimport)
#endif

#include "Inport.h"

class CMODELBASE_DLL COuportProp : public CPortBase
{
public:
	COuportProp(void);
	virtual ~COuportProp(void);

public:
	virtual int		Write(char *buf, int len);
	virtual void	Open(void);

public:
	void EnableSave(int sel);
	int	 IsSaveFlag(void);

public:
	void StoreParm(CKeyTree *ptree);
	void LoadParm(CKeyTree *ptree);

private:
	int		m_bsave;
};

//===============================

class CMODELBASE_DLL COutPort : public CChainTool<COuportProp>
{
public:
	COutPort(void);
	virtual ~COutPort(void);

public:
	void StoreParm(CKeyTree *ptree);
	void LoadParm(CKeyTree *ptree);
};

#endif
