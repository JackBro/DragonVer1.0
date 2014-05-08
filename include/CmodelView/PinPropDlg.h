#ifndef __CMODELVIEW_PINPROPDLG_H__
#define __CMODELVIEW_PINPROPDLG_H__

#ifndef CMODELVIEW_DLL
	#define CMODELVIEW_DLL  __declspec(dllimport)
#endif

#include "BaseClass/BaseDlg.h"
#include "CmodelBase/pinbase.h"


// CPinPropDlg 对话框
class CMODELVIEW_DLL CPinPropDlg : public CBaseDlg
{
	DECLARE_DYNAMIC(CPinPropDlg)

public:
	CPinPropDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CPinPropDlg();

// 对话框数据
	enum { IDD = IDD_PINPROP_DLG };

public:
	void SetPin(CPinBase *pin);

private:
	void InitUserOption(void);

private:
	CPinBase *m_pin;
	CStatic	m_descStatic;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	DECLARE_MESSAGE_MAP()
protected:
	virtual BOOL OnInitDialog();
};

#endif
