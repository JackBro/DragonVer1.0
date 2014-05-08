
#include "internal.h"

IMPLEMENT_DYNAMIC(CGammaCorrectPropDlg, CImageBoxPropDlg)
CGammaCorrectPropDlg::CGammaCorrectPropDlg(CWnd* pParent /*=NULL*/)
	: CImageBoxPropDlg(CGammaCorrectPropDlg::IDD, pParent)
{
}

CGammaCorrectPropDlg::~CGammaCorrectPropDlg()
{
}

void CGammaCorrectPropDlg::DoDataExchange(CDataExchange* pDX)
{
	CImageBoxPropDlg::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_GAMMA_DITHER_EDIT, m_ditherEdit);
	DDX_Control(pDX, IDC_GAMMA_DITHERVAL_EDIT, m_dithervalEdit);
	DDX_Control(pDX, IDC_GAMMA_R_EDIT, m_gammaEdit[0]);	
	DDX_Control(pDX, IDC_GAMMA_R_EDIT2, m_gammaEdit[1]);
	DDX_Control(pDX, IDC_GAMMA_R_EDIT3, m_gammaEdit[2]);
	DDX_Control(pDX, IDC_GAMMA_R_EDIT4, m_gammaEdit[3]);
	DDX_Control(pDX, IDC_GAMMA_R_EDIT5, m_gammaEdit[4]);
	DDX_Control(pDX, IDC_GAMMA_R_EDIT6, m_gammaEdit[5]);
	DDX_Control(pDX, IDC_GAMMA_R_EDIT7, m_gammaEdit[6]);
	DDX_Control(pDX, IDC_GAMMA_R_EDIT8, m_gammaEdit[7]);
	DDX_Control(pDX, IDC_GAMMA_R_EDIT9, m_gammaEdit[8]);
	DDX_Control(pDX, IDC_GAMMA_R_EDIT10, m_gammaEdit[9]);
	DDX_Control(pDX, IDC_GAMMA_R_EDIT11, m_gammaEdit[10]);
	DDX_Control(pDX, IDC_GAMMA_R_EDIT12, m_gammaEdit[11]);
	DDX_Control(pDX, IDC_GAMMA_R_EDIT13, m_gammaEdit[12]);
	DDX_Control(pDX, IDC_GAMMA_R_EDIT14, m_gammaEdit[13]);
	DDX_Control(pDX, IDC_GAMMA_R_EDIT15, m_gammaEdit[14]);
	DDX_Control(pDX, IDC_GAMMA_R_EDIT16, m_gammaEdit[15]);
	DDX_Control(pDX, IDC_GAMMA_R_EDIT17, m_gammaEdit[16]);
	DDX_Control(pDX, IDC_GAMMA_MATRIX_COMBO, m_gammaselCombo);
}

BEGIN_MESSAGE_MAP(CGammaCorrectPropDlg, CImageBoxPropDlg)
END_MESSAGE_MAP()

void CGammaCorrectPropDlg::DispUserOption(void)
{
}

BOOL CGammaCorrectPropDlg::OnInitDialog()
{
	CImageBoxPropDlg::OnInitDialog();

	this->DispUserOption();

	return TRUE;
}

