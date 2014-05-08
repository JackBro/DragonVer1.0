// V558TestFpgaDlg.cpp : 实现文件
//
#include "internal.h"

// CBaseBoxPropDlg 对话框


IMPLEMENT_DYNAMIC(CBaseBoxPropDlg, CBaseDlg)
CBaseBoxPropDlg::CBaseBoxPropDlg(UINT Idd, CWnd* pParent /*=NULL*/)
	: CBaseDlg(Idd, pParent)
{
	this->m_pUnit = NULL;
}

CBaseBoxPropDlg::~CBaseBoxPropDlg()
{
}

void CBaseBoxPropDlg::DoDataExchange(CDataExchange* pDX)
{
	CBaseDlg::DoDataExchange(pDX);
}

#define IDC_BASEALGSELECT_COMBO		5000

BEGIN_MESSAGE_MAP(CBaseBoxPropDlg, CBaseDlg)
END_MESSAGE_MAP()

void  CBaseBoxPropDlg::UpdateOption(void)
{
	int sel;

	sel = this->m_AlgTypeCombo.GetCurSel();
	if(this->m_pUnit)
		this->m_pUnit->ChoiceAlg(sel);
}

void CBaseBoxPropDlg::SetUnit(void *unit)
{
	this->m_pUnit = (CBaseUnit*)unit;
}

void CBaseBoxPropDlg::OnOK()
{
	this->UpdateOption();
	CBaseDlg::OnOK();
}

void CBaseBoxPropDlg::DispBasePropCtrl(void)
{
	RECT rect;
	int i;

	rect.left = 10;
	rect.top = 20;
	rect.right = 180;
	rect.bottom = 300;
	this->m_AlgTypeCombo.Create(CBS_DROPDOWNLIST | CBS_AUTOHSCROLL | CBS_HASSTRINGS, 
		rect, this, IDC_BASEALGSELECT_COMBO);
	this->m_AlgTypeCombo.ShowWindow(SW_SHOW);

	if(this->m_pUnit)
	{
		for(i = 0; i < this->m_pUnit->GetTotalAlg(); i++)
			this->m_AlgTypeCombo.InsertString(i, this->m_pUnit->GetAlgDesc(i));
		this->m_AlgTypeCombo.SetCurSel(this->m_pUnit->GetCurAlg());
	}
}

BOOL CBaseBoxPropDlg::OnInitDialog()
{
	CBaseDlg::OnInitDialog();

	this->DispBasePropCtrl();

	return TRUE;
}

