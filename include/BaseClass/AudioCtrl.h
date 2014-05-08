
#ifndef __BASECLASS_AUDIOCTRL_H__
#define __BASECLASS_AUDIOCTRL_H__


#ifndef BASECLASS_DLL
	#define BASECLASS_DLL  __declspec(dllimport)
#endif



class BASECLASS_DLL CAudioCtrl : public CWnd
{
	DECLARE_DYNAMIC(CAudioCtrl)

public:
	CAudioCtrl();
	virtual ~CAudioCtrl();

protected:
	DECLARE_MESSAGE_MAP()

public:
	virtual BOOL Create(CWnd* pParentWnd, UINT nID = 0xffff);
};



#endif
