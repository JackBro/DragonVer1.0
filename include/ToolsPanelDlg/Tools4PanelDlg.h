#ifndef __TOOLSPANEL_TOOLS4PANELDLG_H__
#define __TOOLSPANEL_TOOLS4PANELDLG_H__



#ifndef TOOLSPANEL_DLL
	#define TOOLSPANEL_DLL _declspec(dllimport)
#endif

#include "BaseClass/BasePanelDlg.h"
#include "BaseClass/MHexEdit.h"
#include "BaseClass/PictureBox.h"
#include "SillGui/ImageBox.h"

// CTools4PanelDlg 对话框
typedef struct tag_TUserT4Option {
	char src[DEFAULT_FILENAME_LEN];
	char dest[DEFAULT_FILENAME_LEN];
	int imgwidth;
	int imgheight;
	CBufferBase pool1;
	CBufferBase pool2;
} TUserT4Option, *PTUserT4Option;

class TOOLSPANEL_DLL CTools4PanelDlg : public CBasePanelDlg
{
	DECLARE_DYNAMIC(CTools4PanelDlg)

public:
	CTools4PanelDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CTools4PanelDlg();

// 对话框数据
	enum { IDD = IDD_TOOLS4_DLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

protected:
	void InitUserOption(void);
	void GetUserOption(char *tail = FILE_TAIL_BIN);
	void AutoSetSrcfile(void);
	void DispOption(void);
	void GetEditBoxValue(void);

private:
	CPictureBox		m_pic;
	CImageBox		m_imagebox;
	TUserT4Option	m_option;
	CMHexEdit		m_imgwidthbox;
	CMHexEdit		m_imgheightbox;
	

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedTools4miscButton();
	afx_msg void OnBnClickedButton2();
};

#endif
