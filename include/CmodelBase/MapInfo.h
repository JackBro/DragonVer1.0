#ifndef __CMODELBASE_MAPINFO_H__
#define __CMODELBASE_MAPINFO_H__


#ifndef CMODELBASE_DLL
	#define CMODELBASE_DLL  __declspec(dllimport)
#endif

#include "toolchain/chaintools.h"
#include "UnitRect.h"
#include "maptable.h"

enum UNITPINSELECTenum
{
	NOUP_SELECTED	=	0,
	UNIT_SELECTED	=	BIT0,
	PIN_SELECTED	=	BIT1,
	MODIFIED_SELECT	=	BIT2
};

class CMODELBASE_DLL CMapInfo : public CChainTool<CUnitRect>
{
public:
	CMapInfo(void);
	virtual ~CMapInfo(void);

public:
	virtual void DeleteUnit(CUnitRect* unit);

public:
	int CheckLdown(int x, int y);
	int CheckLup(int x, int y, int *start, int *end);
	CUnitRect * GetCurrentUnit(void);
	int  Connect(int start, int end);

public:
	int  GetPinPos(int index, int *sx, int *sy, int *ex, int *ey);
	int	 GetPinCount(void);
	TMap *GetPinMap(int index);
	TMap *GetCurrentPinMap(void);
	void DelCurrent(void);

public:
	void StoreParm(CKeyTree *ptree);

private:
	int	CheckUnitLdown(int x, int y);
	int CheckPinLdown(int x, int y);
	CUnitRect * GetUnitRect(CBaseUnit *punit);

private:
	int m_posx;
	int m_posy;

protected:
	CMapTable m_table;
};



#endif
