#ifndef __CMODELBASE_UNITRECT_H__
#define __CMODELBASE_UNITRECT_H__


#ifndef CMODELBASE_DLL
	#define CMODELBASE_DLL  __declspec(dllimport)
#endif

#include "baseunit.h"

#define DEFAULT_BOX_WIDTH	160

#define DEFAULT_BOX_HEIGHT	30
#define DEFAULT_PORT_SIZE	10
#define DEFAULT_PORT_STEP	10
#define PORT_GAP	(DEFAULT_PORT_SIZE + DEFAULT_PORT_STEP)

typedef struct tag_TUnitRect {
	int x;
	int y;
	int width;
	int height;
} TUnitRect, *PTUnitRect;

class CMODELBASE_DLL CUnitRect
{
public:
	CUnitRect(void);
	virtual ~CUnitRect(void);

public:
	void SetUnit(CBaseUnit *punit);
	CBaseUnit *GetUnit(void);

public:
	void UpdateInfo(void);
	void SetStartPoint(int x, int y);
	void Move(int x, int y);
	int  LocatePoint(int x, int y);
	void SetState(int set);
	int  GetState(void);

public:
	int  GetInpinPos(int index, int *px, int *py);
	int  GetOupinPos(int index, int *px, int *py);

private:
	void update(void);
	int  IsInside(int x, int y, TUnitRect *rect);

public:
	void StoreParm(CKeyTree *ptree);
	void LoadParm(CKeyTree *ptree);

public:
	TUnitRect	m_body;
	CChainTool<TUnitRect>	m_inpin, m_oupin;

private:
	CBaseUnit		*m_punit;
	int m_state;
};



#endif
