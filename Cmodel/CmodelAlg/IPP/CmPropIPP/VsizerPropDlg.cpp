

#include "internal.h"


IMPLEMENT_DYNAMIC(CVsizerBoxPropDlg, CImageBoxPropDlg)
CVsizerBoxPropDlg::CVsizerBoxPropDlg(CWnd* pParent /*=NULL*/)
	: CImageBoxPropDlg(CVsizerBoxPropDlg::IDD, pParent)
{

}

CVsizerBoxPropDlg::~CVsizerBoxPropDlg()
{
}

void CVsizerBoxPropDlg::DoDataExchange(CDataExchange* pDX)
{
	CImageBoxPropDlg::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMBO_PAL, m_palbox);
	
}

BEGIN_MESSAGE_MAP(CVsizerBoxPropDlg, CImageBoxPropDlg)

END_MESSAGE_MAP()


BOOL CVsizerBoxPropDlg::OnInitDialog()
{
	CImageBoxPropDlg::OnInitDialog();

	this->InitUserOption();
	this->DispUserOption();

	return TRUE;
}


void CVsizerBoxPropDlg::InitUserOption(void)
{
	const char *pal[] = { "NTSC", "PAL" };
	int i;

	//memset(&this->m_option, 0, sizeof(THsizerOption));
	for(i = 0; i < sizeof(pal) / sizeof(char *); i++)
		this->m_palbox.InsertString(i, pal[i]);
}

void CVsizerBoxPropDlg::DispUserOption(void)
{
	this->m_palbox.SetCurSel(this->m_option.pal);
}

void CVsizerBoxPropDlg::GetUserOption(void)
{
	this->m_option.pal = this->m_palbox.GetCurSel();
}

void CVsizerBoxPropDlg::UpdateOption(void)
{
	CImageBoxPropDlg::UpdateOption();
	this->GetUserOption();
	if (this->m_pUnit) 
	{
		CVSizerBox *pCVSizerBox = (CVSizerBox *)m_pUnit;
		pCVSizerBox->SetOutMode(this->m_option.pal);		
	}

}