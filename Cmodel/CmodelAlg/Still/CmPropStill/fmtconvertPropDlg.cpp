		

#include "internal.h"

IMPLEMENT_DYNAMIC(CFmtCvtPropDlg, CImageBoxPropDlg)
CFmtCvtPropDlg::CFmtCvtPropDlg(CWnd* pParent /*=NULL*/)
	: CImageBoxPropDlg(CFmtCvtPropDlg::IDD, pParent)
{
}

CFmtCvtPropDlg::~CFmtCvtPropDlg(void)
{
}

void CFmtCvtPropDlg::DoDataExchange(CDataExchange* pDX)
{
	CImageBoxPropDlg::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_Y2YOUTFMT_COMBO, m_OutFmtCombo);
}


BEGIN_MESSAGE_MAP(CFmtCvtPropDlg, CImageBoxPropDlg)
END_MESSAGE_MAP()


// CComGatePropDlg 消息处理程序
void CFmtCvtPropDlg::DispUserOption(void)
{
	int i, sel, set = 0;
	CTypeProp tp;

	ASSERT(this->m_pUnit);
	sel = ((CFmtConvertBox *)this->m_pUnit)->GetOutFmt();
	for(i = 0; i < g_FmtcvtCount; i++)
	{
		this->m_OutFmtCombo.InsertString(i, tp.GetFormatDesc(g_FmtcvtFormats[i]));
		if(sel == i)
			set = i;
	}
	this->m_OutFmtCombo.SetCurSel(set);
}

void CFmtCvtPropDlg::GetUserOption(void)
{
	this->m_option.outfmt = this->m_OutFmtCombo.GetCurSel();
}

void CFmtCvtPropDlg::UpdateOption(void)
{
	this->GetUserOption();
	CImageBoxPropDlg::UpdateOption();
	((CFmtConvertBox *)this->m_pUnit)->SetOutFmt(g_FmtcvtFormats[this->m_option.outfmt]);
}

BOOL CFmtCvtPropDlg::OnInitDialog()
{
	CImageBoxPropDlg::OnInitDialog();

	this->DispUserOption();

	return TRUE;
}

