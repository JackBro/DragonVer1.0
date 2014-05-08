#ifndef __V568PLANE_V568SDIFDLG_H__
#define __V568PLANE_V568SDIFDLG_H__


#ifndef V568PLANE_DLL
	#define V568PLANE_DLL _declspec(dllimport)
#endif

#include "BasicFunc/BaseCtrlDlg.h"
#include "BaseClass/MComboBox.h"


// CV568SdifDlg 对话框

typedef struct tag_TSdcOption {
	char	srcfile[DEFAULT_FILENAME_LEN];
	char	dstfile[DEFAULT_FILENAME_LEN];
	char    rsp[20];
	short   blkCnt;
	short   blkSize;
	char    stoMode;
	char	cmdh;
	char	cmdl;
	int	    cmdargu;
	int	    transaddr;
} TSdcOption, *PTSdcOption;

class V568PLANE_DLL CV568SdifDlg : public CBaseCtrlDlg
{
	DECLARE_DYNAMIC(CV568SdifDlg)

public:
	CV568SdifDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CV568SdifDlg();

// 对话框数据
	enum { IDD = IDD_V568SDIF_DLG };

protected:
	void InitUserOption(void);
	void DispUserOption(void);
	void GetUserOption(char *tail = FILE_TAIL_BIN);
	void GetComboValue(char *tail = FILE_TAIL_BIN);
	void GetEditBoxValue(void);

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

private:
	TSdcOption	  m_option;

	CMComboBox	m_respbox;
	CButton		m_cmdchkbtn;
	CButton		m_cmdidxchkbtn;
	CButton		m_datdircbtn;
	CButton		m_datprebtn;
	CButton		m_cmd12enbtn;
	CMHexEdit	m_cmdidxbox;
    CMHexEdit   m_cmdargubox;
    CMHexEdit   m_cmdrspbox;
	CMHexEdit   m_blkcntbox;
	CMHexEdit   m_blksizebox;
	CMHexEdit   m_transaddrbox;

	CButton		m_wrsbbtn;
	CButton		m_wrmbbtn;
	CButton		m_rdsbbtn;
	CButton		m_rdmbbtn;

	CButton    m_moddmabtn;
	CButton    m_modspibtn;
	CButton    m_modtranswidbtn;

	DECLARE_MESSAGE_MAP()

public:
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedSdopenButton();
	afx_msg void OnBnClickedRefreshButton();
	afx_msg void OnBnClickedCmdindexButton();
	afx_msg void OnBnClickedSdcmdCheck();
	afx_msg void OnBnClickedSdcmdindexCheck();
	afx_msg void OnBnClickedSddatadirecCheck();
	afx_msg void OnBnClickedDatapresCheck();
	afx_msg void OnCbnSelchangeRespCombo();
	afx_msg void OnBnClickedAutocmd12Check();
	afx_msg void OnBnClickedStomoddmaCheck();
	afx_msg void OnBnClickedStomodspiCheck();
	afx_msg void OnBnClickedStomodtranswidthCheck();
	afx_msg void OnBnClickedAcmdsendButton2();	
	afx_msg void OnBnClickedWritesbButton();
	afx_msg void OnBnClickedWritembButton();
	afx_msg void OnBnClickedReadsbButton();
	afx_msg void OnBnClickedReadmbButton();	
};

#endif
