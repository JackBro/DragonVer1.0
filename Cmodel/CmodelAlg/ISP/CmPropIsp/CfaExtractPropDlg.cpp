#include "internal.h"


IMPLEMENT_DYNAMIC(CCfaExtractPropDlg, CImageBoxPropDlg)
CCfaExtractPropDlg::CCfaExtractPropDlg(CWnd* pParent /*=NULL*/)
: CImageBoxPropDlg(CCfaExtractPropDlg::IDD, pParent)
{
}

CCfaExtractPropDlg::~CCfaExtractPropDlg()
{
}

void CCfaExtractPropDlg::DoDataExchange(CDataExchange* pDX)
{
	CImageBoxPropDlg::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_CFAEXTRACT_DIVEDGE_EDIT, m_divedgeEdit);
	DDX_Control(pDX, IDC_CFAEXTRACT_FEMM_EDIT, m_femmEdit);
	DDX_Control(pDX, IDC_CFAEXTRACT_FEMP_EDIT, m_fempEdit);
	DDX_Control(pDX, IDC_CFAEXTRACT_FEMMIN_EDIT, m_femminEdit);
	DDX_Control(pDX, IDC_CFAEXTRACT_FEMMAX_EDIT, m_femmaxEdit);
	DDX_Control(pDX, IDC_CFAEXTRACT_FEMX1_EDIT, m_femx1Edit);
	DDX_Control(pDX, IDC_CFAEXTRACT_FEMX2_EDIT, m_femx2Edit);
	DDX_Control(pDX, IDC_CFAEXTRACT_MODE_EDIT, m_modeEdit);
	//DDX_Control(pDX, IDC_CFAEXTRACT_MODE_CHECK, m_modeCheck);
	DDX_Control(pDX, IDC_CFAEXTRACT_COEF_EDIT, m_coefedgeEdit[0][0]);
	DDX_Control(pDX, IDC_CFAEXTRACT_COEF_EDIT2, m_coefedgeEdit[0][1]);
	DDX_Control(pDX, IDC_CFAEXTRACT_COEF_EDIT3, m_coefedgeEdit[0][2]);
	DDX_Control(pDX, IDC_CFAEXTRACT_COEF_EDIT4, m_coefedgeEdit[0][3]);
	DDX_Control(pDX, IDC_CFAEXTRACT_COEF_EDIT5, m_coefedgeEdit[0][4]);
	DDX_Control(pDX, IDC_CFAEXTRACT_COEF_EDIT6, m_coefedgeEdit[1][0]);
	DDX_Control(pDX, IDC_CFAEXTRACT_COEF_EDIT7, m_coefedgeEdit[1][1]);
	DDX_Control(pDX, IDC_CFAEXTRACT_COEF_EDIT8, m_coefedgeEdit[1][2]);
	DDX_Control(pDX, IDC_CFAEXTRACT_COEF_EDIT9, m_coefedgeEdit[1][3]);
	DDX_Control(pDX, IDC_CFAEXTRACT_COEF_EDIT10, m_coefedgeEdit[1][4]);
	DDX_Control(pDX, IDC_CFAEXTRACT_COEF_EDIT11, m_coefedgeEdit[2][0]);
	DDX_Control(pDX, IDC_CFAEXTRACT_COEF_EDIT12, m_coefedgeEdit[2][1]);
	DDX_Control(pDX, IDC_CFAEXTRACT_COEF_EDIT13, m_coefedgeEdit[2][2]);
	DDX_Control(pDX, IDC_CFAEXTRACT_COEF_EDIT14, m_coefedgeEdit[2][3]);
	DDX_Control(pDX, IDC_CFAEXTRACT_COEF_EDIT15, m_coefedgeEdit[2][4]);
	DDX_Control(pDX, IDC_CFAEXTRACT_COEF_EDIT16, m_coefedgeEdit[3][0]);
	DDX_Control(pDX, IDC_CFAEXTRACT_COEF_EDIT17, m_coefedgeEdit[3][1]);
	DDX_Control(pDX, IDC_CFAEXTRACT_COEF_EDIT18, m_coefedgeEdit[3][2]);
	DDX_Control(pDX, IDC_CFAEXTRACT_COEF_EDIT19, m_coefedgeEdit[3][3]);
	DDX_Control(pDX, IDC_CFAEXTRACT_COEF_EDIT20, m_coefedgeEdit[3][4]);
	DDX_Control(pDX, IDC_CFAEXTRACT_COEF_EDIT21, m_coefedgeEdit[4][0]);
	DDX_Control(pDX, IDC_CFAEXTRACT_COEF_EDIT22, m_coefedgeEdit[4][1]);
	DDX_Control(pDX, IDC_CFAEXTRACT_COEF_EDIT23, m_coefedgeEdit[4][2]);
	DDX_Control(pDX, IDC_CFAEXTRACT_COEF_EDIT24, m_coefedgeEdit[4][3]);
	DDX_Control(pDX, IDC_CFAEXTRACT_COEF_EDIT25, m_coefedgeEdit[4][4]);
}

BEGIN_MESSAGE_MAP(CCfaExtractPropDlg, CImageBoxPropDlg)
END_MESSAGE_MAP()

void CCfaExtractPropDlg::InitUserOption(void)
{
	CString str;
	int i, j;

	memset( &(this->m_option), 0, sizeof(TCfaExtractOption) );

	if (this->m_pUnit) 
	{
		CCfaExtract *pCfaExtract = (CCfaExtract*)m_pUnit;
		this->m_option.mode = pCfaExtract->GetMode();
		this->m_option.femm = pCfaExtract->GetFemm();
		this->m_option.femp = pCfaExtract->GetFemp();
		this->m_option.femmin = pCfaExtract->GetFemmin();
		this->m_option.femmax = pCfaExtract->GetFemmax();
		this->m_option.femx1 = pCfaExtract->GetFemx1();
		this->m_option.femx2 = pCfaExtract->GetFemx2();
		this->m_option.divedge = pCfaExtract->GetDivEdge();
		memcpy(this->m_option.coefedge, pCfaExtract->GetCoefEdge(), sizeof(this->m_option.coefedge));
	}

	str.Format("%d", m_option.mode);
	m_modeEdit.SetWindowText(str);
	str.Format("%d", m_option.femm);
	m_femmEdit.SetWindowText(str);
	str.Format("%d", m_option.femp);
	m_fempEdit.SetWindowText(str);
	str.Format("%d", m_option.femmin);
	m_femminEdit.SetWindowText(str);
	str.Format("%d", m_option.femmax);
	m_femmaxEdit.SetWindowText(str);
	str.Format("%d", m_option.femx1);
	m_femx1Edit.SetWindowText(str);
	str.Format("%d", m_option.femx2);
	m_femx2Edit.SetWindowText(str);
	str.Format("%d", m_option.divedge);
	m_divedgeEdit.SetWindowText(str);

	for(i = 0; i < 5; i++)
	{
		for(j = 0; j < 5; j++)
		{
			str.Format("%d", m_option.coefedge[i][j]);
			m_coefedgeEdit[i][j].SetWindowText(str);
		}
	}	
}

void CCfaExtractPropDlg::GetUserOption(void)
{
	CString str;
	int i, j;

	m_modeEdit.GetWindowText(str);
	m_option.mode = atoi(str);
	m_femmEdit.GetWindowText(str);
	m_option.femm = atoi(str);
	m_fempEdit.GetWindowText(str);
	m_option.femp = atoi(str);
	m_femminEdit.GetWindowText(str);
	m_option.femmin = atoi(str);
	m_femmaxEdit.GetWindowText(str);
	m_option.femmax = atoi(str);
	m_femx1Edit.GetWindowText(str);
	m_option.femx1 = atoi(str);
	m_femx2Edit.GetWindowText(str);
	m_option.femx2 = atoi(str);
	m_divedgeEdit.GetWindowText(str);
	m_option.divedge= atoi(str);

	for(i = 0; i < 5; i++)
	{
		for(j = 0; j < 5; j++)
		{
			m_coefedgeEdit[i][j].GetWindowText(str);
			m_option.coefedge[i][j] = (char)atoi(str);
		}
	}
}

void CCfaExtractPropDlg::DispUserOption(void)
{
}

BOOL CCfaExtractPropDlg::OnInitDialog()
{
	CImageBoxPropDlg::OnInitDialog();

	this->DispUserOption();
	this->InitUserOption();

	return TRUE;
}

void CCfaExtractPropDlg::UpdateOption()
{
	CImageBoxPropDlg::UpdateOption();
	this->GetUserOption();
	if (this->m_pUnit) 
	{
		CCfaExtract *pCfaExtract = (CCfaExtract*)m_pUnit;
		pCfaExtract->SetMode(this->m_option.mode);
		pCfaExtract->SetFemm(this->m_option.femm);
		pCfaExtract->SetFemp(this->m_option.femp);
		pCfaExtract->SetFemmin(this->m_option.femmin);
		pCfaExtract->SetFemmax(this->m_option.femmax);
		pCfaExtract->SetFemx1(this->m_option.femx1);
		pCfaExtract->SetFemx2(this->m_option.femx2);
		pCfaExtract->SetDivEdge(this->m_option.divedge);
		pCfaExtract->SetCoefEdge(&this->m_option.coefedge[0][0]);
	}
}

