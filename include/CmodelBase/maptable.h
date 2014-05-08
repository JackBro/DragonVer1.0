#ifndef _TOOLCMODEL_MAPTABLE_H_
#define _TOOLCMODEL_MAPTABLE_H_


#ifndef CMODELBASE_DLL
	#define CMODELBASE_DLL  __declspec(dllimport)
#endif

#include "toolchain/chaintools.h"
#include "tooltree/treebase.h"
#include "baseunit.h"
#include "mapbase.h"

typedef struct tag_TMap {
	CBaseUnit	*psrc;
	int			portout;
	CPinBase	pin;
	CBaseUnit	*pdst;
	int			portin;
	int			state;
} TMap, *PTMap;

class CMODELBASE_DLL CMapTable : public CChainTool<TMap>
{
public:
	CMapTable();
	virtual ~CMapTable();

public:
	void DeleteUnit(TMap *pmap);

public:
	void	Connect(CBaseUnit *ps, CBaseUnit *pe, int spin, int epin);
	void	DeletePin(CPinBase *pin);
	void	DeleteItem(CBaseUnit *punit);
	TMap	*GetCurrentPin(void);

public:
	void StoreParm(CKeyTree *pb);
	void LoadParm(CMapBase *pmap, CKeyTree *pb);

private:
	TMap	*FindMap(CPinBase* pin);
};

#endif


