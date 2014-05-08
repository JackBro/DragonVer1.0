// ReglistView.cpp : 实现文件
//
#include "internal.h"


// CBaseReglistView

IMPLEMENT_DYNCREATE(CBaseReglistView, CBaseTabCtrlView)

CBaseReglistView::CBaseReglistView()
{
	this->m_index = V568_REGGROUP;
	this->m_pdlg = NULL;
}

CBaseReglistView::~CBaseReglistView()
{
	if(this->m_pdlg)
		delete [](this->m_pdlg);
}

void CBaseReglistView::DoDataExchange(CDataExchange* pDX)
{
	CBaseTabCtrlView::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CBaseReglistView, CBaseTabCtrlView)
END_MESSAGE_MAP()


// CBaseReglistView 诊断

#ifdef _DEBUG
void CBaseReglistView::AssertValid() const
{
	CBaseTabCtrlView::AssertValid();
}

void CBaseReglistView::Dump(CDumpContext& dc) const
{
	CBaseTabCtrlView::Dump(dc);
}
#endif //_DEBUG

void CBaseReglistView::SetRegIndex(int index)
{
	if(index > V578_REGGROUP)
		index = V578_REGGROUP;
	this->m_index = index;
}

// CBaseReglistView 消息处理程序
void CBaseReglistView::OnInitialUpdate()
{
	int i;

	if(this->m_pdlg)
		delete [](this->m_pdlg);
	this->m_pdlg = NULL;
	this->m_pdlg = new CRegListDlg[gRegGroup[this->m_index].listcount];
	if(this->m_pdlg)
	{
		for(i = 0; i < gRegGroup[this->m_index].listcount; i++)
		{
			this->AddDlg(this->m_pdlg+i, IDD_BASEREGLIST_DLG, 
				gRegGroup[this->m_index].reglist[i].regclass);
		}
	}
	CBaseTabCtrlView::OnInitialUpdate();
	if(this->m_pdlg)
	{
		for(i = 0; i < gRegGroup[this->m_index].listcount; i++)
		{
			this->m_pdlg[i].SetContent(gRegGroup[this->m_index].reglist[i].parray, 
				gRegGroup[this->m_index].reglist[i].count,
				gRegGroup[this->m_index].reglist[i].regclass );
		}
	}
}

