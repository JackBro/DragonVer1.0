#include "internal.h"


IMPLEMENT_DYNAMIC(CColorCorrectPropDlg, CImageBoxPropDlg)
CColorCorrectPropDlg::CColorCorrectPropDlg(CWnd* pParent /*=NULL*/)
	: CImageBoxPropDlg(CColorCorrectPropDlg::IDD, pParent)
{
}

CColorCorrectPropDlg::~CColorCorrectPropDlg()
{
}

void CColorCorrectPropDlg::DoDataExchange(CDataExchange* pDX)
{
	CImageBoxPropDlg::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COLORMATRIX_EDIT, m_colormatrixEdit[0][0]);
	DDX_Control(pDX, IDC_COLORMATRIX_EDIT2, m_colormatrixEdit[0][1]);
	DDX_Control(pDX, IDC_COLORMATRIX_EDIT3, m_colormatrixEdit[0][2]);
	DDX_Control(pDX, IDC_COLORMATRIX_EDIT4, m_colormatrixEdit[1][0]);
	DDX_Control(pDX, IDC_COLORMATRIX_EDIT5, m_colormatrixEdit[1][1]);
	DDX_Control(pDX, IDC_COLORMATRIX_EDIT6, m_colormatrixEdit[1][2]);
	DDX_Control(pDX, IDC_COLORMATRIX_EDIT7, m_colormatrixEdit[2][0]);
	DDX_Control(pDX, IDC_COLORMATRIX_EDIT8, m_colormatrixEdit[2][1]);
	DDX_Control(pDX, IDC_COLORMATRIX_EDIT9, m_colormatrixEdit[2][2]);
	DDX_Control(pDX, IDC_OFFSET_EDIT, m_offsetEdit[0]);
	DDX_Control(pDX, IDC_OFFSET_EDIT2, m_offsetEdit[1]);
	DDX_Control(pDX, IDC_OFFSET_EDIT3, m_offsetEdit[2]);
}

BEGIN_MESSAGE_MAP(CColorCorrectPropDlg, CImageBoxPropDlg)
END_MESSAGE_MAP()

void CColorCorrectPropDlg::InitUserOption(void)
{
	CString str;
	int i, j;

	memset( &(this->m_option), 0, sizeof(TColorCorrectOption) );

	if (this->m_pUnit) 
	{
		CColorCorrect *pColorCorrect = (CColorCorrect *)m_pUnit;
	
		int *p = pColorCorrect ->GetColorMatrix();
		for(i = 0; i < 3; i++)
			for(j = 0; j < 3; j++)
				this->m_option.colormatrix[i][j] = p[i*3+j]&0xff;

		p = pColorCorrect->GetColorOffset();
		for(i = 0; i < 3; i++)
			this->m_option.offset[i] = p[i]&0xff;
	}
	for(i = 0; i < 3; i++)
	{
		for(j = 0; j < 3; j++)
		{
			str.Format("%d", m_option.colormatrix[i][j]);
			this->m_colormatrixEdit[i][j].SetWindowText(str);
		}
	}
	for(i = 0; i < 3; i++)
	{
		str.Format("%d", this->m_option.offset[i]);
		this->m_offsetEdit[i].SetWindowText(str);
	}
}

void CColorCorrectPropDlg::GetUserOption(void)
{
	CString str;
	int i, j;
	for(i = 0; i < 3; i++)
	{
		for(j = 0; j < 3; j++)
		{
			str.Format("%d", m_option.colormatrix[i][j]);
			this->m_colormatrixEdit[i][j].SetWindowText(str);
		}
	}
	for(i = 0; i < 3; i++)
	{
		str.Format("%d", this->m_option.offset[i]);
		this->m_offsetEdit[i].SetWindowText(str);
	}
}


void CColorCorrectPropDlg::DispUserOption(void)
{
}

BOOL CColorCorrectPropDlg::OnInitDialog()
{
	CImageBoxPropDlg::OnInitDialog();

	this->DispUserOption();
	this->InitUserOption();

	return TRUE;
}

void CColorCorrectPropDlg::UpdateOption(void)
{
	CImageBoxPropDlg::UpdateOption();
	this->GetUserOption();
	if (this->m_pUnit) 
	{
		CColorCorrect *pColorCorrect = (CColorCorrect *)m_pUnit;
		pColorCorrect->SetColorMatrix(&this->m_option.colormatrix[0][0]);
		pColorCorrect->SetColorOffset(&this->m_option.offset[0]);
	}
}
