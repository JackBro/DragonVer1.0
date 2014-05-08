// RegListDlg.cpp : 实现文件
//
#include "internal.h"
#ifndef _AFX_NO_AFXCMN_SUPPORT
#include <afxcmn.h>			// Windows 公共控件的 MFC 支持
#endif // _AFX_NO_AFXCMN_SUPPORT
#include <afxpriv.h>		// required for WM_SETMESSAGESTRING




#define LISTOFFSET_X	10
#define LISTOFFSET_Y	10

// CRegListDlg 对话框

IMPLEMENT_DYNAMIC(CRegListDlg, CDialog)
CRegListDlg::CRegListDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CRegListDlg::IDD, pParent)
{
	this->m_title[0] = 0;
}

CRegListDlg::~CRegListDlg()
{
}

void CRegListDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_REGLISTCTRL, m_reglist);
	DDX_Control(pDX, IDC_REFRESH_BTN, m_refreshbtn);
	DDX_Control(pDX, IDC_TESTREG_BTN, m_testregbtn);
	DDX_Control(pDX, IDC_SAVEREG_BTN, m_saveregbtn);
}

BEGIN_MESSAGE_MAP(CRegListDlg, CDialog)
	ON_WM_DRAWITEM()
	ON_WM_SIZE()
	ON_BN_CLICKED(IDC_REFRESH_BTN, OnBnClickedRefreshBtn)
	ON_BN_CLICKED(IDC_TESTREG_BTN, OnBnClickedTestregBtn)
	ON_BN_CLICKED(IDC_SAVEREG_BTN, OnBnClickedSaveregBtn)
END_MESSAGE_MAP()


void CRegListDlg::SetRegListPos(void)
{
	RECT rect, rect1;

	this->GetClientRect(&rect);
	if(this->m_reglist.m_hWnd)
	{
		rect1.left = rect.left + LISTOFFSET_X;
		rect1.top = rect.top + LISTOFFSET_Y;
		rect1.bottom = rect.bottom - 70;
		rect1.right = rect.right - 10;
		this->m_reglist.SetWindowPos(&wndTop, rect1.left, rect1.top, 
			rect1.right-rect1.left, rect1.bottom-rect1.top, SWP_SHOWWINDOW);
	}
	if(this->m_refreshbtn.m_hWnd)
		this->m_refreshbtn.SetWindowPos(&wndTop, (rect.right+rect.left + 100) >> 1, rect.bottom-50, 0, 0,
			SWP_NOSIZE | SWP_SHOWWINDOW);
	if(this->m_testregbtn.m_hWnd)
		this->m_testregbtn.SetWindowPos(&wndTop, (rect.right+rect.left - 100) >> 1, rect.bottom-50, 0, 0,
			SWP_NOSIZE | SWP_SHOWWINDOW);
	if(this->m_saveregbtn.m_hWnd)
		this->m_saveregbtn.SetWindowPos(&wndTop, (rect.right+rect.left - 300) >> 1, rect.bottom-50, 0, 0,
			SWP_NOSIZE | SWP_SHOWWINDOW);
}

BOOL CRegListDlg::OnInitDialog()
{
	DWORD style;

	CDialog::OnInitDialog();

	// TODO:  在此添加额外的初始化
	style = this->m_reglist.GetExtendedStyle();
	this->m_reglist.SetExtendedStyle(style | LVS_EX_GRIDLINES | LVS_EX_FULLROWSELECT);
	this->SetRegListPos();
	this->m_reglist.InitRegList();
	return TRUE;  // return TRUE unless you set the focus to a control
}

void CRegListDlg::SetContent(PTRegUnit pContent, int count, char *title)
{
	int len;

	this->m_reglist.SetContent(pContent, count);
	if(title == NULL)
		return;
	len = (int)strlen(title);
	len = (len > REGLIST_TITLE_LEN-1) ? REGLIST_TITLE_LEN-1 : len;
	memcpy(this->m_title, title, len);
	this->m_title[len] = 0;
}

void CRegListDlg::OnSize(UINT nType, int cx, int cy)
{
	CDialog::OnSize(nType, cx, cy);

	// TODO: 在此添加消息处理程序代码
	this->SetRegListPos();
}

void CRegListDlg::OnBnClickedRefreshBtn()
{
	int total, i;
	TRegUnit regunit;
	CWaitCursor cursor;

	total = this->m_reglist.GetItemCount();
	for(i = 0; i < total; i++)
	{
		this->m_reglist.GetUnitItem(i, &regunit);
		this->ReadReg(&regunit);
		this->m_reglist.SetUnitItem(i, regunit.regval);
	}
}

void CRegListDlg::OnBnClickedTestregBtn()
{
	int total, i, val = 0xaa;
	char str[100];
	TRegUnit regunit;
	CWaitCursor cursor;

	total = this->m_reglist.GetItemCount();
	for(i = 0; i < total; i++)
	{
		this->m_reglist.GetUnitItem(i, &regunit);
		regunit.regval = val;
		this->WriteReg(&regunit);
		this->ReadReg(&regunit);
		this->m_reglist.SetUnitItem(i, regunit.regval);
		if(regunit.regval != val)
		{
			sprintf(str, "reg: 0x%x, get is not same as write value\n", regunit.regaddr);
			MessageBox(str);
		}
	}
}

void CRegListDlg::OnBnClickedSaveregBtn()
{
	int total, i;
	TRegUnit regunit;
	CWaitCursor cursor;
	CFileBin fb;
	char str[200];

	sprintf(str, "%s_reglist.txt", this->m_title);
	fb.Open(str, "wt");
	total = this->m_reglist.GetItemCount();
	for(i = 0; i < total; i++)
	{
		this->m_reglist.GetUnitItem(i, &regunit);
		this->ReadReg(&regunit);
		this->m_reglist.SetUnitItem(i, regunit.regval);
		fb.Printf("%x@%x\n", regunit.regval, regunit.regaddr);
	}
}

