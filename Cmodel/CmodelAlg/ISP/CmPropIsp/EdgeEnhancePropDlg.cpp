#include "internal.h"


IMPLEMENT_DYNAMIC(CEdgeEnhancePropDlg, CImageBoxPropDlg)
CEdgeEnhancePropDlg::CEdgeEnhancePropDlg(CWnd* pParent /*=NULL*/)
	: CImageBoxPropDlg(CEdgeEnhancePropDlg::IDD, pParent)
{
}

CEdgeEnhancePropDlg::~CEdgeEnhancePropDlg()
{
}

void CEdgeEnhancePropDlg::DoDataExchange(CDataExchange* pDX)
{
	CImageBoxPropDlg::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CEdgeEnhancePropDlg, CImageBoxPropDlg)
	//ON_BN_CLICKED(IDOK, OnBnClickedOk)
END_MESSAGE_MAP()

void CEdgeEnhancePropDlg::DispUserOption(void)
{

}

BOOL CEdgeEnhancePropDlg::OnInitDialog()
{
	CImageBoxPropDlg::OnInitDialog();

	this->DispUserOption();

	return TRUE;
}


//void CEdgeEnhancePropDlg::OnBnClickedOk()
//{
//	// TODO: �ڴ���ӿؼ�֪ͨ����������
//}
