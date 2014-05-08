#ifndef __TOOLSPANEL_TOOLS5PANELDLG_H__
#define __TOOLSPANEL_TOOLS5PANELDLG_H__


#ifndef _WINDLL
#define TOOLSPANEL_DLL
#endif

#ifndef TOOLSPANEL_DLL
#define TOOLSPANEL_DLL _declspec(dllimport)
#endif


#include "../BaseClass/BasePanelDlg.h"

// CTools5PanelDlg 对话框

typedef struct tag_TTools5Option
{
	int memsize;
	unsigned int addr;
} TTools5Option, *PTTools5Option;

typedef struct tag_TTools5Memmap
{
	unsigned int addr;
	unsigned int size;
	unsigned int used;
} TTools5Memmap, *PTTools5Memmap;

class TOOLSPANEL_DLL CTools5PanelDlg : public CBasePanelDlg
{
	DECLARE_DYNAMIC(CTools5PanelDlg)

public:
	CTools5PanelDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CTools5PanelDlg();

// 对话框数据
	enum { IDD = IDD_TOOLS5_DLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();

private:
	TTools5Option m_option;
	CMHexEdit	m_memsize;
	CListCtrl	m_listmap;

protected:
	void InitUserOption(void);
	void GetUserOption(void);
	void DispUserOption(void);
	void GetEditValue(void);
	void InitMallocMap(void);
	void GetListValue(void);
	void UpdataList(void);
	void InsertListItem(int index, PTTools5Memmap listmap);
	void ClearList(void);


public:
	afx_msg void OnEnChangeEditMallocSize();
	afx_msg void OnBnClickedBtnVmalloc();
	afx_msg void OnBnClickedBtnVfree();
};

#endif
