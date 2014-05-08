#ifndef __SETTINGPANEL_USBSETTINGDLG_H__
#define __SETTINGPANEL_USBSETTINGDLG_H__


#ifndef SETTINGPANEL_DLL
	#define SETTINGPANEL_DLL  __declspec(dllimport)
#endif

#include "BaseClass/MComboBox.h"

// CUsbSettingDlg �Ի���
#define		MAX_USBPIPE 	3


class SETTINGPANEL_DLL CUsbSettingDlg : public CDialog
{
	DECLARE_DYNAMIC(CUsbSettingDlg)

public:
	CUsbSettingDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CUsbSettingDlg();

// �Ի�������
	enum { IDD = IDD_USBSET_DLG };

public:
	int		m_Pipe;

private:
	CMComboBox m_Pipebox;

protected:
	virtual void GetUserOption(void);
	virtual void DispOption(void);

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	virtual void OnOK();
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedButtonInstUsb();
};

extern SETTINGPANEL_DLL char gCurDirectory[];

#endif
