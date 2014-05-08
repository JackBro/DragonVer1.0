#ifndef _TOOLCMODEL_MAPBASE_H_
#define _TOOLCMODEL_MAPBASE_H_


#ifndef CMODELBASE_DLL
	#define CMODELBASE_DLL  __declspec(dllimport)
#endif

#include "toolfile/dbugtools.h"
#include "toolchain/ptrchaintools.h"


class CMODELBASE_DLL CMapBase : public CPChainTool <CBaseUnit>
{
public:
	CMapBase();
	virtual ~CMapBase();

public:
	void	Process(void);
	int		PreProcess(void);

private:
	void	Open(void);
	void	PostProcess(void);

private:
	CDbugTools m_dbg;
};

#endif


