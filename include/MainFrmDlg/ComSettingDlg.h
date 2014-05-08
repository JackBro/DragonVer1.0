#ifndef __SETTINGPANEL_COMSETTINGDLG_H__
#define __SETTINGPANEL_COMSETTINGDLG_H__



#ifndef SETTINGPANEL_DLL
	#define SETTINGPANEL_DLL  __declspec(dllimport)
#endif

#include "BaseClass/MComboBox.h"


#define		MAX_COMPORT		10


class SETTINGPANEL_DLL CComSettingDlg : public CDialog
{
	DECLARE_DYNAMIC(CComSettingDlg)

public:
	CComSettingDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CComSettingDlg();

// 对话框数据
	enum { IDD = IDD_COMSET_DLG };

public:
	int		m_Baud;
	int		m_Port;

private:
	CMComboBox m_Baudbox;
	CMComboBox m_Portbox;

protected:
	virtual void GetUserOption(void);
	virtual void DispOption(void);

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	virtual void OnOK();
	virtual BOOL OnInitDialog();
};

#endif
