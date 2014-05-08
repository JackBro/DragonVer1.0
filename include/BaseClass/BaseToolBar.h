#ifndef __BASECLASS_BASETOOLBAR_H__
#define __BASECLASS_BASETOOLBAR_H__


#ifndef BASECLASS_DLL
	#define BASECLASS_DLL  __declspec(dllimport)
#endif

// CBaseToolBar

class BASECLASS_DLL CBaseToolBar : public CToolBar
{
	DECLARE_DYNAMIC(CBaseToolBar)

public:
	CBaseToolBar();
	virtual ~CBaseToolBar();

protected:
	DECLARE_MESSAGE_MAP()
};



#endif
