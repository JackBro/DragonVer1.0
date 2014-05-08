

#ifndef _CMODEL_MODULEMAP_H_
#define _CMODEL_MODULEMAP_H_

#include "../../../tools/global.h"
#include "../../../toolbuf/srambuf.h"

class CModuleBase;

typedef struct tag_TModuleMap
{
	CModuleBase	*pmodule;
} TModuleMap, *PTModuleMap;


class CModuleMap : public CSramBuf
{
public:
	CModuleMap(int count = 0);

public:
	virtual void	SetSize(int size = 0);
	virtual int		Write(TModuleMap *map, int index);
	virtual int		Read(TModuleMap *map, int index);
	virtual void	DeleteModule(CModuleBase *pmodule);
	virtual void	DeleteModule(int index);

protected:
	virtual void	Clear(void);
};

#endif


