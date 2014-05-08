// PortPropDlg.cpp : 实现文件
//

#include "stdafx.h"

#include "internal.h"


// CInportPropDlg 对话框

IMPLEMENT_DYNAMIC(CInportPropDlg, CBasePortPropDlg)
CInportPropDlg::CInportPropDlg(CWnd* pParent /*=NULL*/)
	: CBasePortPropDlg(CInportPropDlg::IDD, pParent)
{
}

CInportPropDlg::~CInportPropDlg()
{
}

void CInportPropDlg::DoDataExchange(CDataExchange* pDX)
{
	CBasePortPropDlg::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_FORMAT_COMBO, m_fmtCombo);
	DDX_Control(pDX, IDC_FILESEL_COMBO, m_SrcfCombo);
	DDX_Control(pDX, IDC_STREAMCLASS_COMBO, m_fmtclassCombo);
	DDX_Control(pDX, IDC_OPENFILE_BUTTON, m_openfileBtn);
	DDX_Control(pDX, IDC_UNIT_COMBO, m_unitCombo);
	DDX_Control(pDX, IDC_WIDTH_EDIT, m_widthEdit);
	DDX_Control(pDX, IDC_HEIGHT_EDIT, m_heightEdit);
}

BEGIN_MESSAGE_MAP(CInportPropDlg, CBasePortPropDlg)
	ON_BN_CLICKED(IDC_OPENFILE_BUTTON, OnBnClickedOpenfileButton)
	ON_CBN_SELCHANGE(IDC_STREAMCLASS_COMBO, OnCbnSelchangeStreamclassCombo)
END_MESSAGE_MAP()


void CInportPropDlg::ClearCombo(void)
{
	this->m_fmtclassCombo.ResetContent();
	this->m_fmtCombo.ResetContent();
	this->m_unitCombo.ResetContent();
	this->m_SrcfCombo.ResetContent();
}

void CInportPropDlg::SetFmtCombo(int fmt)
{
	int i;

	this->m_fmtclassCombo.ResetContent();
	this->m_fmtCombo.ResetContent();
	for(i = 0; i < gTotalStreamClassType-1; i++)
		this->m_fmtclassCombo.InsertString(i, gStreamClassType[i]);
	this->m_fmtclassCombo.SetCurSel((fmt & CLASSMASK) >> CLASSSHIFT);
	this->OnCbnSelchangeStreamclassCombo();
	this->m_fmtCombo.SetCurSel(fmt & ((1 << CLASSSHIFT) - 1));
}

void CInportPropDlg::InitUserOption(void)
{
	int i, count, *fmts, fmt, index = 0;
	CInportProp *port = (CInportProp *)this->m_port;
	CTypeProp tp;
	CButton *ptn;
	char str[100];

	ASSERT(this->m_port);
	this->ClearCombo();
	count = port->GetUnit();
	count = count > 2 ? count : 2;
	for(i = 0; i < count; i++)
	{
		sprintf(str, "%d", i+1);
		this->m_unitCombo.InsertString(i, str);
	}
	this->m_unitCombo.SetCurSel(port->GetUnit()-1);

	for(i = 0; i < gTotalStreamClassType; i++)
		this->m_fmtclassCombo.InsertString(i, gStreamClassType[i]);
	this->m_fmtclassCombo.SetCurSel(0);

	sprintf(str, "%d", port->GetWidth());
	this->m_widthEdit.SetWindowText(str);
	sprintf(str, "%d", port->GetHeight());
	this->m_heightEdit.SetWindowText(str);

	this->m_SrcfCombo.SetWindowText(port->GetFilename());

	fmt = port->GetFormat();
	if(port->GetPin())
	{
		ptn = (CButton *)this->GetDlgItem(IDOK);
		if(ptn)
			ptn->EnableWindow(FALSE);
		this->m_fmtclassCombo.EnableWindow(FALSE);
		this->m_unitCombo.EnableWindow(FALSE);
		this->m_widthEdit.EnableWindow(FALSE);
		this->m_heightEdit.EnableWindow(FALSE);
		this->m_fmtCombo.EnableWindow(FALSE);
		this->m_SrcfCombo.EnableWindow(FALSE);
		this->m_openfileBtn.EnableWindow(FALSE);
		this->m_fmtCombo.InsertString(0, tp.GetFormatDesc(fmt));
		this->m_fmtCombo.SetCurSel(0);

	}
	else
	{
		this->m_fmtclassCombo.EnableWindow();
		this->m_unitCombo.EnableWindow();
		this->m_widthEdit.EnableWindow();
		this->m_heightEdit.EnableWindow();
		this->m_fmtCombo.EnableWindow();
		if(port->IsCtrlPort())
		{
			this->m_SrcfCombo.EnableWindow(FALSE);
			this->m_openfileBtn.EnableWindow(FALSE);
		}
		else
		{
			this->m_openfileBtn.EnableWindow();
			this->m_SrcfCombo.EnableWindow();
		}
		if(port->HasUnknowType())
		{
			this->SetFmtCombo(fmt);
			return;
		}
		count = port->GetFmtCount();
		fmts = port->GetFormats();
		for(i = 0; i < count; i++)
		{
			if(fmt == fmts[i])
				index = i;
			this->m_fmtCombo.InsertString(i, tp.GetFormatDesc(fmts[i]));
		}
		this->m_fmtCombo.SetCurSel(index);
		this->m_fmtclassCombo.EnableWindow(FALSE);
	}
}

BOOL CInportPropDlg::OnInitDialog()
{
	CBasePortPropDlg::OnInitDialog();

	this->InitUserOption();

	return TRUE;  // return TRUE unless you set the focus to a control
}

void CInportPropDlg::GetUserOption(void)
{
	char str[20];
	CStringTools tools;

	this->m_option.fmt = this->m_fmtCombo.GetCurSel();
	this->m_SrcfCombo.GetWindowText(this->m_option.srcf, DEFAULT_FILENAME_LEN);
	this->m_unitCombo.GetWindowText(str, 20);
	tools.Str2Dec(str, &(this->m_option.unit));
	this->m_widthEdit.GetWindowText(str, 20);
	tools.Str2Dec(str, &(this->m_option.width));
	this->m_heightEdit.GetWindowText(str, 20);
	tools.Str2Dec(str, &(this->m_option.height));
	this->m_option.sc = this->m_fmtclassCombo.GetCurSel();
}

void CInportPropDlg::OnBnClickedOpenfileButton()
{
	CString str;

	if(CBasePortPropDlg::OpenFileDlg(&str))
		this->m_SrcfCombo.addString(str.GetBuffer());
}

void CInportPropDlg::OnOK()
{
	CInportProp *port = (CInportProp *)this->m_port;
	int *fmts;

	this->GetUserOption();
	port->SetFilename(this->m_option.srcf);
	port->SetWidth(this->m_option.width);
	port->SetHeight(this->m_option.height);
	port->SetUnit(this->m_option.unit);

	if(port->HasUnknowType())
		this->m_option.fmt |= this->m_option.sc << CLASSSHIFT;
	else
	{
		fmts = port->GetFormats();
		this->m_option.fmt = fmts[this->m_option.fmt];
	}
	port->SetFormat(this->m_option.fmt);

	CBasePortPropDlg::OnOK();
}

void CInportPropDlg::OnCbnSelchangeStreamclassCombo()
{
	int i, sel;

	sel = this->m_fmtclassCombo.GetCurSel();
	switch(sel)
	{
	case VIDEOCLASS:
		for(i = 0; i < gTotalImageStreamType; i++)
			this->m_fmtCombo.InsertString(i, gImageStreamType[i]);
		break;
	case AUDIOCLASS:
		for(i = 0; i < gTotalAudStreamType; i++)
			this->m_fmtCombo.InsertString(i, gAudStreamType[i]);
		break;
	case SYSTEMCLASS:
		for(i = 0; i < gTotalSystemStreamType; i++)
			this->m_fmtCombo.InsertString(i, gSystemStreamType[i]);
		break;
	case CTRLCLASS:
	default:
		for(i = 0; i < gTotalCtrlStreamType; i++)
			this->m_fmtCombo.InsertString(i, gCtrlStreamType[i]);
		break;
	}
	this->m_fmtCombo.SetCurSel(0);
}
