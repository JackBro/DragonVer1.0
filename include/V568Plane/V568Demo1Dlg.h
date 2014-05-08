#ifndef __V568PLANE_V568DEMO1DLG_H__
#define __V568PLANE_V568DEMO1DLG_H__


#ifndef V568PLANE_DLL
	#define V568PLANE_DLL _declspec(dllimport)
#endif

#include "BasicFunc/BaseCtrlDlg.h"
#include "BaseClass/MComboBox.h"
#include "BaseClass/MDecEdit.h"
#include "BaseClass/MHexEdit.h"

// CV568Demo1Dlg 对话框

typedef enum FILTER_FUNCenum {
	FUNC_DISP_SRC,
	FUNC_DISP_DISP,
	FUNC_CAP_SRC,
	FUNC_CAP_CAP
};

typedef struct tag_TDemo1Option {
	char	srcfile[DEFAULT_FILENAME_LEN];
	char	dstfile[DEFAULT_FILENAME_LEN];
	TSize	srcsize;
	TSize	dstsize;
	int		sleepcnt;
	int		step;
	int		filterfunc;
	TJpegIndex info;

} TDemo1Option, *PTDemo1Option;

class V568PLANE_DLL CV568Demo1Dlg : public CBaseCtrlDlg
{
	DECLARE_DYNAMIC(CV568Demo1Dlg)

public:
	CV568Demo1Dlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CV568Demo1Dlg();

// 对话框数据
	enum { IDD = IDD_V568DEMO1_DLG };

private:
	TDemo1Option	m_option;

	CMComboBox m_jpgtypebox;

	CMDecEdit m_jpgimgwidthbox;
	CMDecEdit m_jpgimgheightbox;
	CMDecEdit m_jpgjpgwidth;
	CMDecEdit m_jpgjpgheight;
	CMDecEdit m_jpgsopbox;
	CMDecEdit m_jpgoffsetbox;

	CMHexEdit m_dstwidthBox;
	CMHexEdit m_dstheightBox;
	CMHexEdit m_imgwidthBox;
	CMHexEdit m_imgheightBox;

	CMDecEdit m_sleepBox;
	CMDecEdit m_stepBox;
	CMComboBox m_filfuncbox;

	CStatic   m_sticsrc;
	CStatic   m_sticdst;

private:
	void DoDisplay(int bSrc);
	void DoCapture(int bSrc);
	void InitOption(void);
	void DispOption(void);
	void GetUserOption(void);

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedDemo1displaybutton();
	virtual BOOL OnInitDialog();
	afx_msg void OnCbnSelchangeComboFiltFunc();
	afx_msg void OnBnClickedParserjpgButton();
};

#endif
