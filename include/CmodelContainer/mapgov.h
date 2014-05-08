#ifndef _CMODEL_MAPGOV_H_
#define _CMODEL_MAPGOV_H_


#ifndef CMODELCONTAINER_DLL
	#define CMODELCONTAINER_DLL  __declspec(dllimport)
#endif

#include "CmodelBase/MapInfo.h"
#include "CmodelBase/mapbase.h"
#include "unitcontainer.h"
#include "propcontainer.h"


class CMODELCONTAINER_DLL CMapGov : public CMapInfo, public CPropContainer
{
public:
	CMapGov(void);
	virtual ~CMapGov(void);

public:
	CUnitRect * Add(int sel);
	void DelCurrent(void);

public:
	void	Process(void);
	int		PreProcess(void);

public:
	void LoadParm(CKeyTree *ptree);

private:
	void	SetNum(void);

private:
	CMapBase		m_map;		//process entrance
	CUnitContainer	m_units;
};

#endif
