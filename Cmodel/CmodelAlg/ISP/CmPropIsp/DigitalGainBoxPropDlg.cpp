
#include "internal.h"



// CDigitalGainBoxPropDlg ¶Ô»°¿ò

IMPLEMENT_DYNAMIC(CDigitalGainBoxPropDlg, CImageBoxPropDlg)
CDigitalGainBoxPropDlg::CDigitalGainBoxPropDlg(CWnd* pParent /*=NULL*/)
	: CImageBoxPropDlg(CDigitalGainBoxPropDlg::IDD, pParent)
{
}

CDigitalGainBoxPropDlg::~CDigitalGainBoxPropDlg()
{
}

void CDigitalGainBoxPropDlg::DoDataExchange(CDataExchange* pDX)
{
	CImageBoxPropDlg::DoDataExchange(pDX);

	DDX_Control(pDX, IDC_DGAIN_RGAIN_EDIT, m_rgainEdit);
	DDX_Control(pDX, IDC_DGAIN_GGAIN_EDIT, m_ggainEdit);
	DDX_Control(pDX, IDC_DGAIN_BGAIN_EDIT, m_bgainEdit);
	DDX_Control(pDX, IDC_DGAIN_GLOBALGAIN_EDIT, m_globalgainEdit);
	DDX_Control(pDX, IDC_DGAIN_GLOBLE_CHECK, m_globalCheck);
	DDX_Control(pDX, IDC_DGAIN_RGB_CHECK, m_rgbCheck);
	DDX_Control(pDX, IDC_GAINSTEP_EDIT, m_gainStep);
}


void CDigitalGainBoxPropDlg::InitUserOption(void)
{
	CString str;

	memset( &(this->m_option), 0, sizeof(TDigitalGainOption) );

	if (this->m_pUnit) 
	{
		CDigitalGain *pDigitalGain = (CDigitalGain *)m_pUnit;
		m_option.rgain = pDigitalGain->GetRGain();
		m_option.ggain = pDigitalGain->GetGGain();
		m_option.bgain = pDigitalGain->GetBGain();
		m_option.globalgain = pDigitalGain->GetGlobalGain();
		m_option.gainstep = pDigitalGain->GetGainStep();
	}

	str.Format("%d", m_option.rgain);
	m_rgainEdit.SetWindowText(str);
	str.Format("%d", m_option.ggain);
	m_ggainEdit.SetWindowText(str);
	str.Format("%d", m_option.bgain);
	m_bgainEdit.SetWindowText(str);
	str.Format("%d", m_option.globalgain);
	m_globalgainEdit.SetWindowText(str);
	str.Format("%d", m_option.gainstep);
	m_gainStep.SetWindowText(str);
	
}

void CDigitalGainBoxPropDlg::GetUserOption(void)
{
	CString str;

	m_rgainEdit.GetWindowText(str);
	m_option.rgain = atoi(str);
	m_ggainEdit.GetWindowText(str);
	m_option.ggain = atoi(str);
	m_bgainEdit.GetWindowText(str);
	m_option.bgain = atoi(str);
	m_globalgainEdit.GetWindowText(str);
	m_option.globalgain = atoi(str);
	m_gainStep.GetWindowText(str);
	m_option.gainstep = atoi(str);
}

void CDigitalGainBoxPropDlg::DispUserOption(void)
{
}


BOOL CDigitalGainBoxPropDlg::OnInitDialog()
{
	CImageBoxPropDlg::OnInitDialog();

	this->DispUserOption();
	this->InitUserOption();

	return TRUE;
}

void CDigitalGainBoxPropDlg::UpdateOption()
{
	CImageBoxPropDlg::UpdateOption();
	this->GetUserOption();
	if (this->m_pUnit) 
	{
		CDigitalGain *pDigitalGain = (CDigitalGain *)m_pUnit;
		if (this->m_globalCheck.GetCheck()) 
		{
			pDigitalGain->SetGlobalEn(0x1);
			pDigitalGain->SetGlobalGain(this->m_option.globalgain);
		}
		else
		{
			pDigitalGain->SetGlobalEn(0);
		}
		if (this->m_rgbCheck.GetCheck()) 
		{
			pDigitalGain->SetRgbEn(0x1);
			pDigitalGain->SetRGain(this->m_option.rgain);
			pDigitalGain->SetGGain(this->m_option.ggain);
			pDigitalGain->SetBGain(this->m_option.bgain);
		}
	}
}




