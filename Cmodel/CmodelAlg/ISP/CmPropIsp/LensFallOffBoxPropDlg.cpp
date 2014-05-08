	

#include "internal.h"




IMPLEMENT_DYNAMIC(CLensFallOffBoxPropDlg, CImageBoxPropDlg)
CLensFallOffBoxPropDlg::CLensFallOffBoxPropDlg(CWnd* pParent /*=NULL*/)
	: CImageBoxPropDlg(CLensFallOffBoxPropDlg::IDD, pParent)
{
	
}

CLensFallOffBoxPropDlg::~CLensFallOffBoxPropDlg()
{
}

void CLensFallOffBoxPropDlg::DoDataExchange(CDataExchange* pDX)
{
	CImageBoxPropDlg::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LENSCX_EDIT, m_lenscxEdit);
	DDX_Control(pDX, IDC_LENSCY_EDIT, m_lenscyEdit);
	DDX_Control(pDX, IDC_LENSFOCUS_EDIT, m_lensfocusEdit);
}

BEGIN_MESSAGE_MAP(CLensFallOffBoxPropDlg, CImageBoxPropDlg)
	
END_MESSAGE_MAP()


void CLensFallOffBoxPropDlg::InitUserOption(void)
{
	CString str;

	memset( &(this->m_option), 0, sizeof(TLensFallOffOption) );
	
	if (this->m_pUnit) 
	{
		CLensFallOff *pLensFallOff = (CLensFallOff *)m_pUnit;
		m_option.lensCx = pLensFallOff->Get568LensCx();
		m_option.lensCy = pLensFallOff->Get568LensCy();
		m_option.lensFocus = pLensFallOff->Get568LensFocus();
	}

	str.Format("%d", m_option.lensCx);
	m_lenscxEdit.SetWindowText(str);
	str.Format("%d", m_option.lensCy);
	m_lenscyEdit.SetWindowText(str);
	str.Format("%d", m_option.lensFocus);
	m_lensfocusEdit.SetWindowText(str);
}

void CLensFallOffBoxPropDlg::GetUserOption(void)
{
	CString str;

	m_lenscxEdit.GetWindowText(str);
	m_option.lensCx = atoi(str);
	m_lenscyEdit.GetWindowText(str);
	m_option.lensCy = atoi(str);
	m_lensfocusEdit.GetWindowText(str);
	m_option.lensFocus = atoi(str);
}

void CLensFallOffBoxPropDlg::DispUserOption(void)
{
}

BOOL CLensFallOffBoxPropDlg::OnInitDialog()
{
	CImageBoxPropDlg::OnInitDialog();

	this->InitUserOption();
	this->DispUserOption();
	return TRUE;
}

void CLensFallOffBoxPropDlg::UpdateOption()
{
	CImageBoxPropDlg::UpdateOption();
	this->GetUserOption();
	if (this->m_pUnit) 
	{
		CLensFallOff *pLensFallOff = (CLensFallOff *)m_pUnit;
		pLensFallOff->Set568LensCx(this->m_option.lensCx);
		pLensFallOff->Set568LensCy(this->m_option.lensCy);
		pLensFallOff->Set568LensFocus(this->m_option.lensFocus);
	}
}


