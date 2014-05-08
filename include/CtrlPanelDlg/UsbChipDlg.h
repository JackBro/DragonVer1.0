#ifndef __CTRLPLANE_USBCHIPDLG_H__
#define __CTRLPLANE_USBCHIPDLG_H__


#ifndef CTRLPLANE_DLL
	#define CTRLPLANE_DLL _declspec(dllimport)
#endif

#include "BasicFunc/BaseCtrlDlg.h"

// CUsbChipDlg 对话框

typedef struct tag_TUsbOption
{
	char srcfile[DEFAULT_FILENAME_LEN];
	char dstfile[DEFAULT_FILENAME_LEN];
	int buflen;
	int wep;
	int rep;
	int rwcnt;
	int rwtime;
} TUsbOption, *PTUsbOption;

class CTRLPLANE_DLL CUsbChipDlg : public CBaseCtrlDlg
{
	DECLARE_DYNAMIC(CUsbChipDlg)

public:
	CUsbChipDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CUsbChipDlg();

// 对话框数据
	enum { IDD = IDD_USBCHIP_DLG };

protected:
	void InitUserOption(void);
	void DispUserOption(void);
	void GetUserOption(char *tail = FILE_TAIL_BIN);

private:
	TUsbOption m_option;
	CMComboBox m_wepbox;
	CMComboBox m_repbox;
	CMHexEdit m_buflenedit;
	CMHexEdit m_rwcntedit;
	CMHexEdit m_rwtimeedit;
	CButton m_atnbtn;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedWritefifoBtn();
	afx_msg void OnBnClickedReadfifoBtn();
	afx_msg void OnBnClickedAttachnodeCheck();
	afx_msg void OnBnClickedDbugprintBtn();
	afx_msg void OnBnClickedUsbchipMiscBtn();
	afx_msg void OnBnClickedButtonRead();
	afx_msg void OnBnClickedButtonWrite();
	afx_msg void OnBnClickedButtonReadReg();
	afx_msg void OnBnClickedButtonWrReg();
};


#endif
