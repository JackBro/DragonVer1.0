// V558TestFpgaDlg.cpp : 实现文件
//
#include "internal.h"

// CBasePanelDlg 对话框

#define IDC_BASESRCFILE_COMBO		5000
#define IDC_BASEDESTFILE_COMBO		5001
#define IDC_BASEAUTOFILE_CHECK		5002
#define IDC_BASEOPENSRCFILE_BTN		5003
#define IDC_BASEOPENDESTFILE_BTN	5004
#define IDC_BASEUPFILE_BTN			5005

IMPLEMENT_DYNAMIC(CBasePanelDlg, CBaseDlg)
CBasePanelDlg::CBasePanelDlg(UINT Idd, CWnd* pParent /*=NULL*/)
	: CBaseDlg(Idd, pParent)
{
}

CBasePanelDlg::~CBasePanelDlg()
{
}

void CBasePanelDlg::DoDataExchange(CDataExchange* pDX)
{
	CBaseDlg::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CBasePanelDlg, CBaseDlg)
	ON_BN_CLICKED(IDC_BASEOPENSRCFILE_BTN, OnBnClickOpensrcfileBtn)
	ON_BN_CLICKED(IDC_BASEOPENDESTFILE_BTN, OnBnClickOpendestfileBtn)
	ON_BN_CLICKED(IDC_BASEUPFILE_BTN, OnBnClickUpfileBtn)
	ON_BN_CLICKED(IDC_BASEAUTOFILE_CHECK, OnBnClickAutofileCheck)
	ON_CBN_SELCHANGE(IDC_BASESRCFILE_COMBO, OnCbnChangeSrcfCombo)
END_MESSAGE_MAP()


// CBasePanelDlg 消息处理程序

void CBasePanelDlg::OnBnClickOpensrcfileBtn()
{
	CString str;

	if(CBaseDlg::OpenFileDlg(&str))
	{
		this->m_SrcfCombo.addString(str.GetBuffer());
		this->OnCbnChangeSrcfCombo();
	}
}

void CBasePanelDlg::OnBnClickOpendestfileBtn()
{
	CString str;

	if(CBaseDlg::OpenFileDlg(&str))
	{
		this->m_DestfCombo.addString(str.GetBuffer());
	}
}

void CBasePanelDlg::OnBnClickAutofileCheck()
{
	if(this->m_AutoSelChkBtn.GetCheck())
	{
		this->m_DestfCombo.EnableWindow(FALSE);
		this->m_OpenDstfBtn.EnableWindow(FALSE);
	}
	else
	{
		this->m_DestfCombo.EnableWindow();
		this->m_OpenDstfBtn.EnableWindow();
	}
}

void CBasePanelDlg::DispFileCtrl(void)
{
	RECT rect;

	rect.left = 367;
	rect.top = 9;
	rect.right = 389;
	rect.bottom = 39;
	this->m_OpenSrcfBtn.Create("...", BS_PUSHBUTTON, rect, this, IDC_BASEOPENSRCFILE_BTN);

	rect.left = 84;
	rect.top = 9;
	rect.right = 365;
	rect.bottom = 39;
	this->m_SrcfCombo.Create(CBS_DROPDOWN | CBS_AUTOHSCROLL | CBS_SORT | CBS_HASSTRINGS, 
		rect, this, IDC_BASESRCFILE_COMBO);
	this->m_SrcfCombo.ModifyStyleEx(0, WS_EX_ACCEPTFILES | WS_EX_NOPARENTNOTIFY);

	rect.left = 391;
	rect.top = 9;
	rect.right = 485;
	rect.bottom = 39;
	this->m_flenStatic.Create("FileLen(Hex)", SS_CENTER , rect, this);//, IDC_BASESRC_STATIC);

	rect.left = 9;
	rect.top = 9;
	rect.right = 82;
	rect.bottom = 39;
	this->m_SrcfStatic.Create("Src file:", SS_CENTER , rect, this);//, IDC_BASESRC_STATIC);

	rect.left = 84;
	rect.top = 39;
	rect.right = 294;
	rect.bottom = 156;
	this->m_DestfCombo.Create(CBS_DROPDOWN | CBS_AUTOHSCROLL | CBS_SORT | CBS_HASSTRINGS, 
		rect, this, IDC_BASEDESTFILE_COMBO);
	this->m_DestfCombo.ModifyStyleEx(0, WS_EX_ACCEPTFILES | WS_EX_NOPARENTNOTIFY);

	rect.left = 9;
	rect.top = 39;
	rect.right = 82;
	rect.bottom = 69;
	this->m_DstfStatic.Create("Dst file:", SS_CENTER , rect, this);//, IDC_BASESRC_STATIC);

	rect.left = 295;
	rect.top = 39;
	rect.right = 327;
	rect.bottom = 69;
	this->m_OpenDstfBtn.Create("...", BS_PUSHBUTTON, rect, this, IDC_BASEOPENDESTFILE_BTN);

	rect.left = 329;
	rect.top = 39;
	rect.right = 390;
	rect.bottom = 69;
	this->m_AutoSelChkBtn.Create("auto", BS_AUTOCHECKBOX, rect, this, IDC_BASEAUTOFILE_CHECK);

	rect.left = 391;
	rect.top = 39;
	rect.right = 420;
	rect.bottom = 69;
	this->m_UpfBtn.Create("up", BS_PUSHBUTTON, rect, this, IDC_BASEUPFILE_BTN);

	rect.left = 424;
	rect.top = 39;
	rect.right = 485;
	rect.bottom = 69;
	this->m_filelenStatic.Create("0", SS_CENTER , rect, this);//, IDC_BASESRC_STATIC);

	this->m_AutoSelChkBtn.ShowWindow(SW_SHOW);
	this->m_OpenDstfBtn.ShowWindow(SW_SHOW);
	this->m_OpenSrcfBtn.ShowWindow(SW_SHOW);
	this->m_DestfCombo.ShowWindow(SW_SHOW);
	this->m_SrcfCombo.ShowWindow(SW_SHOW);
	this->m_SrcfStatic.ShowWindow(SW_SHOW);
	this->m_DstfStatic.ShowWindow(SW_SHOW);
	this->m_flenStatic.ShowWindow(SW_SHOW);
	this->m_UpfBtn.ShowWindow(SW_SHOW);
	this->m_filelenStatic.ShowWindow(SW_SHOW);
}

void CBasePanelDlg::OnCbnChangeSrcfCombo()
{
	CWaitCursor	cursor;
	CFileBin fb;
	int len;
	char str[DEFAULT_FILENAME_LEN];

	len = this->m_SrcfCombo.GetCurSel();
	this->m_SrcfCombo.SetCurSel(len);
	this->m_SrcfCombo.GetWindowText(str, DEFAULT_FILENAME_LEN);
	fb.Open(str);
	len = fb.GetFileLength();
	sprintf(str, "%x", len);
	this->m_filelenStatic.SetWindowText(str);
}

void CBasePanelDlg::OnBnClickUpfileBtn()
{
	CWaitCursor	cursor;
	char str[DEFAULT_FILENAME_LEN];

	this->m_DestfCombo.GetWindowText(str, DEFAULT_FILENAME_LEN);
	this->m_SrcfCombo.addString(str);;
	this->OnCbnChangeSrcfCombo();
}

BOOL CBasePanelDlg::OnInitDialog()
{
	CBaseDlg::OnInitDialog();

	this->DispFileCtrl();

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常：OCX 属性页应返回 FALSE
}

void CBasePanelDlg::GetFileBoxOption(char *src, char *dest, char *tail)
{
	CStringTools tools;
	CFileFind find;

	this->m_SrcfCombo.GetWindowText(src, DEFAULT_FILENAME_LEN);
	if(this->m_AutoSelChkBtn.GetCheck())
	{
		tools.AutoIndex2Str(src, tail, dest);
		this->m_DestfCombo.addString(dest);
		if(find.FindFile(dest))
			DeleteFile(dest);
	}
	else
		this->m_DestfCombo.GetWindowText(dest, DEFAULT_FILENAME_LEN);
}

void CBasePanelDlg::AutoSetFileBox(char *dest)
{
	CFileFind find;

	if(this->m_AutoSelChkBtn.GetCheck())
	{
		if(find.FindFile(dest) == 0)
		{
			MessageBox("Function not support now\n");
			return;
		}
		this->m_SrcfCombo.addString(dest);
		this->OnCbnChangeSrcfCombo();
	}
}

void CBasePanelDlg::OnOK()
{
//	CBaseDlg::OnOK();
}

void CBasePanelDlg::OnCancel()
{
//	CBaseDlg::OnCancel();
}
