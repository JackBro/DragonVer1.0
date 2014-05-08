
#ifndef __BASECLASS_MTOOLTIP_H__
#define __BASECLASS_MTOOLTIP_H__



#ifndef BASECLASS_DLL
	#define BASECLASS_DLL  __declspec(dllimport)
#endif


class BASECLASS_DLL CMToolTip : public CToolTipCtrl
{
public:
	CMToolTip();
	virtual ~CMToolTip();

protected:

	DECLARE_MESSAGE_MAP()
public:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
};


#endif
