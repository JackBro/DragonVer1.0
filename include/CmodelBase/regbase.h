

#ifndef _TOOLCMODEL_REGBASE_H_
#define _TOOLCMODEL_REGBASE_H_


#ifndef CMODELBASE_DLL
	#define CMODELBASE_DLL  __declspec(dllimport)
#endif

#include "tools/baseitem.h"

//==========================================
//
//		class CRegBase 
//
//===================================
#define DEFAULT_REG_COUNT	0x200

typedef struct tag_TRegister
{
	int		val;
	int		mask;
} TRegister, *PTRegister;

class CMODELBASE_DLL CRegBase : public CBaseObject
{
public:
	CRegBase(int count = DEFAULT_REG_COUNT);
	virtual ~CRegBase(void);

public:
	virtual void	SetReg(int adr, int val);
	virtual int		GetReg(int adr);

protected:
	virtual void	SetRegMask(int adr, int mask);

public:
	TRegister		*m_reg;
};


#endif


