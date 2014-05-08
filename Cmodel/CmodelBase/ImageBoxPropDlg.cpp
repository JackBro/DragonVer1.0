#include "internal.h"



// CImageBoxPropDlg ¶Ô»°¿ò

IMPLEMENT_DYNAMIC(CImageBoxPropDlg, CBaseBoxPropDlg)
CImageBoxPropDlg::CImageBoxPropDlg(UINT Idd, CWnd* pParent /*=NULL*/)
	: CBaseBoxPropDlg(Idd, pParent)
{
}

CImageBoxPropDlg::~CImageBoxPropDlg(void)
{
}

void CImageBoxPropDlg::DoDataExchange(CDataExchange* pDX)
{
	CBaseBoxPropDlg::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CImageBoxPropDlg, CBaseBoxPropDlg)
END_MESSAGE_MAP()

void  CImageBoxPropDlg::UpdateOption(void)
{
	char str[20];
	int	 x;
	CStringTools tools;

	CBaseBoxPropDlg::UpdateOption();
	this->m_DummyEdit.GetWindowText(str, 20);
	tools.Str2Dec(str, &x);
	if(this->m_pUnit)
		((CImageUnit*)this->m_pUnit)->SetDummyline(x);
}

#define IDC_BASEALGSELECT_COMBO		5100

void CImageBoxPropDlg::DispImagePropCtrl(void)
{
	RECT rect;
	char str[20];

	rect.left = 220;
	rect.top = 20;
	rect.right = 300;
	rect.bottom = 50;
	this->m_DummyEdit.Create(ES_LEFT | WS_BORDER, rect, this, IDC_BASEALGSELECT_COMBO);
	this->m_DummyEdit.ShowWindow(SW_SHOW);
	if(this->m_pUnit)
		sprintf(str, "%d", ((CImageUnit*)this->m_pUnit)->GetDummyline());
	this->m_DummyEdit.SetWindowText(str);
}

BOOL CImageBoxPropDlg::OnInitDialog()
{
	CBaseBoxPropDlg::OnInitDialog();

	this->DispImagePropCtrl();
	return TRUE;
}

