#ifndef __CMODELPANEL_OLDCMODELDLG_H__
#define __CMODELPANEL_OLDCMODELDLG_H__



#ifndef CMODELPANEL_DLL
	#define CMODELPANEL_DLL _declspec(dllimport)
#endif

#include "BaseClass/BasePanelDlg.h"
#include "BaseClass/MComboBox.h"


#define OLDCMODEL_FILENAME_LENGTH 100

typedef struct tag_TOldCmodelOption
{
	char cfgfile[OLDCMODEL_FILENAME_LENGTH];
} TOldCmodelOption, *PTOldCmodelOption;

// COldCmodelDlg 对话框

class CMODELPANEL_DLL COldCmodelDlg : public CBasePanelDlg
{
	DECLARE_DYNAMIC(COldCmodelDlg)

public:
	COldCmodelDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~COldCmodelDlg();

// 对话框数据
	enum { IDD = IDD_OLDCMODEL_DLG };

protected:
	void InitUserOption(void);
	void GetUserOption(void);

private:
	TOldCmodelOption m_option;
	CMComboBox m_cfgfilebox;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedRuncmodelButton();

public:
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedOpensrcfileBtn();
};

#endif
