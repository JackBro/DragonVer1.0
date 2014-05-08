		

#include "internal.h"

IMPLEMENT_DYNAMIC(CYuv2YuvPropDlg, CImageBoxPropDlg)
CYuv2YuvPropDlg::CYuv2YuvPropDlg(CWnd* pParent /*=NULL*/)
	: CImageBoxPropDlg(CYuv2YuvPropDlg::IDD, pParent)
{
}

CYuv2YuvPropDlg::~CYuv2YuvPropDlg(void)
{
}

void CYuv2YuvPropDlg::DoDataExchange(CDataExchange* pDX)
{
	CImageBoxPropDlg::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_Y2YOUTFMT_COMBO, m_OutFmtCombo);
}


BEGIN_MESSAGE_MAP(CYuv2YuvPropDlg, CImageBoxPropDlg)
END_MESSAGE_MAP()


// CComGatePropDlg 消息处理程序
void CYuv2YuvPropDlg::DispUserOption(void)
{
	int i, sel, set = 0;
	CTypeProp tp;

	ASSERT(this->m_pUnit);
	sel = ((CYuv2yuvBox *)this->m_pUnit)->GetOutFmt();
	for(i = 0; i < g_Yuv2YuvFmtCount; i++)
	{
		this->m_OutFmtCombo.InsertString(i, tp.GetFormatDesc(g_Yuv2YuvFormats[i]));
		if(sel == i)
			set = i;
	}
	this->m_OutFmtCombo.SetCurSel(set);
}

void CYuv2YuvPropDlg::GetUserOption(void)
{
	this->m_option.outfmt = this->m_OutFmtCombo.GetCurSel();
}

void CYuv2YuvPropDlg::UpdateOption(void)
{
	this->GetUserOption();
	CImageBoxPropDlg::UpdateOption();
	((CYuv2yuvBox *)this->m_pUnit)->SetOutFmt(g_Yuv2YuvFormats[this->m_option.outfmt]);
}

BOOL CYuv2YuvPropDlg::OnInitDialog()
{
	CImageBoxPropDlg::OnInitDialog();

	this->DispUserOption();

	return TRUE;
}

