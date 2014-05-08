

#ifndef _TOOLCMODEL_MODULEBASE_H_
#define _TOOLCMODEL_MODULEBASE_H_


#ifndef CMODELBASE_DLL
	#define CMODELBASE_DLL  __declspec(dllimport)
#endif

#include "regbase.h"

class CMODELBASE_DLL CModuleBase : public CRegBase
{
public:
	CModuleBase(int regcnt = DEFAULT_REG_COUNT);
	virtual ~CModuleBase(void);
};

#endif


