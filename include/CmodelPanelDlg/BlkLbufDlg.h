#ifndef __CMODELPANEL_BLKLBUFDLG_H__
#define __CMODELPANEL_BLKLBUFDLG_H__


#ifndef CMODELPANEL_DLL
	#define CMODELPANEL_DLL _declspec(dllimport)
#endif


#include "BaseClass/BasePanelDlg.h"
#include "BaseClass/MDecEdit.h"

typedef struct tag_TLbufOption {
	char	srcfile[DEFAULT_FILENAME_LEN];
	char	dstfile[DEFAULT_FILENAME_LEN];
	int		width;
	int		height;	
} TLbufOption, *PTLbufOption;


// CBlkLbufDlg 对话框

class CMODELPANEL_DLL CBlkLbufDlg : public CBasePanelDlg
{
	DECLARE_DYNAMIC(CBlkLbufDlg)

public:
	CBlkLbufDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CBlkLbufDlg();

// 对话框数据
	enum { IDD = IDD_BLK_LBUF_DLG };

private:
	TLbufOption		m_option;
	CMDecEdit		m_widthbox;
	CMDecEdit		m_heightbox;

private:
	void InitUserOption(void);
	void DispOption(void);
	void GetUserOption(char *tail = FILE_TAIL_BIN);
	
	void decbin2txt(char *filename, char *buf, int len);
	void encbin2txt(char *filename, char *buf, int len);

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();

	afx_msg void OnBnClickedLbufencButton();
	afx_msg void OnBnClickedLbufdecButton();
};

#endif
