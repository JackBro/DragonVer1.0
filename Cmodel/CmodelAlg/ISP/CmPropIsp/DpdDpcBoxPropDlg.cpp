	
#include "internal.h"



IMPLEMENT_DYNAMIC(CDpdDpcBoxPropDlg, CImageBoxPropDlg)
CDpdDpcBoxPropDlg::CDpdDpcBoxPropDlg(CWnd* pParent /*=NULL*/)
	: CImageBoxPropDlg(CDpdDpcBoxPropDlg::IDD, pParent)
{
}

CDpdDpcBoxPropDlg::~CDpdDpcBoxPropDlg()
{
}

void CDpdDpcBoxPropDlg::DoDataExchange(CDataExchange* pDX)
{
	CImageBoxPropDlg::DoDataExchange(pDX);

	DDX_Control(pDX, IDC_DPDDPC_DPDTHD_EDIT, m_dpdthdEdit);
	DDX_Control(pDX, IDC_NOISETAB_EDIT, m_noisetabEdit[0]);
	DDX_Control(pDX, IDC_NOISETAB_EDIT1, m_noisetabEdit[1]);
	DDX_Control(pDX, IDC_NOISETAB_EDIT2, m_noisetabEdit[2]);
	DDX_Control(pDX, IDC_NOISETAB_EDIT3, m_noisetabEdit[3]);
	DDX_Control(pDX, IDC_NOISETAB_EDIT4, m_noisetabEdit[4]);
	DDX_Control(pDX, IDC_NOISETAB_EDIT5, m_noisetabEdit[5]);
	DDX_Control(pDX, IDC_NOISETAB_EDIT6, m_noisetabEdit[6]);
	DDX_Control(pDX, IDC_NOISETAB_EDIT7, m_noisetabEdit[7]);
	DDX_Control(pDX, IDC_NOISETAB_EDIT8, m_noisetabEdit[8]);
	DDX_Control(pDX, IDC_NOISETAB_EDIT9, m_noisetabEdit[9]);
	DDX_Control(pDX, IDC_NOISETAB_EDIT10, m_noisetabEdit[10]);
	DDX_Control(pDX, IDC_NOISETAB_EDIT11, m_noisetabEdit[11]);
	DDX_Control(pDX, IDC_NOISETAB_EDIT12, m_noisetabEdit[12]);
	DDX_Control(pDX, IDC_NOISETAB_EDIT13, m_noisetabEdit[13]);
	DDX_Control(pDX, IDC_NOISETAB_EDIT14, m_noisetabEdit[14]);
	DDX_Control(pDX, IDC_NOISETAB_EDIT15, m_noisetabEdit[15]);
	DDX_Control(pDX, IDC_NOISETAB_EDIT16, m_noisetabEdit[16]);
}

BEGIN_MESSAGE_MAP(CDpdDpcBoxPropDlg, CImageBoxPropDlg)
END_MESSAGE_MAP()

void CDpdDpcBoxPropDlg::InitUserOption(void)
{
	CString str;
	int i;
	memset( &(this->m_option), 0, sizeof(TDpdDpcOption) );

	if (this->m_pUnit) 
	{
		CDpdDpc *pDpdDpc = (CDpdDpc *)m_pUnit;
		m_option.dpdth = pDpdDpc->GetDpdThd();
		memcpy(m_option.noisetable, pDpdDpc->GetNoiseTable(), sizeof(int)*gTotalNoiseTable);
	}
	str.Format("%d", m_option.dpdth);
	m_dpdthdEdit.SetWindowText(str);
	
	for(i = 0; i < gTotalNoiseTable; i++)
	{
		str.Format("%d", m_option.noisetable[i]);
		m_noisetabEdit[i].SetWindowText(str);
	}
}

void CDpdDpcBoxPropDlg::GetUserOption(void)
{
	CString str;
	int i;

	m_dpdthdEdit.GetWindowText(str);
	m_option.dpdth = atoi(str);

	for(i = 0; i < gTotalNoiseTable; i++)
	{
		m_noisetabEdit[i].GetWindowText(str);
		m_option.noisetable[i] = atoi(str); 
	}
}


void CDpdDpcBoxPropDlg::DispUserOption(void)
{
}

BOOL CDpdDpcBoxPropDlg::OnInitDialog()
{
	CImageBoxPropDlg::OnInitDialog();

	this->DispUserOption();
	this->InitUserOption();

	return TRUE;
}

void CDpdDpcBoxPropDlg::UpdateOption()
{
	CImageBoxPropDlg::UpdateOption();
	this->GetUserOption();
	if (this->m_pUnit) 
	{
		CDpdDpc *pDpdDpc = (CDpdDpc *)m_pUnit;
		pDpdDpc->SetDpdThd(this->m_option.dpdth);
		int i;
		for(i = 0; i < gTotalNoiseTable; i++)
		{
			pDpdDpc->SetNoiseTable(this->m_option.noisetable);
		}
	}
}
