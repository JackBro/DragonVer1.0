// Tools5PanelDlg.cpp : 实现文件
//

#include "internal.h"


// CTools5PanelDlg 对话框

IMPLEMENT_DYNAMIC(CTools5PanelDlg, CBasePanelDlg)
CTools5PanelDlg::CTools5PanelDlg(CWnd* pParent /*=NULL*/)
	: CBasePanelDlg(CTools5PanelDlg::IDD, pParent)
{
}

CTools5PanelDlg::~CTools5PanelDlg()
{
}

void CTools5PanelDlg::DoDataExchange(CDataExchange* pDX)
{
	CBasePanelDlg::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT_MALLOC_SIZE, m_memsize);	
	DDX_Control(pDX, IDC_LIST_MEMMAP, m_listmap);
}


BEGIN_MESSAGE_MAP(CTools5PanelDlg, CBasePanelDlg)
	ON_EN_CHANGE(IDC_EDIT_MALLOC_SIZE, OnEnChangeEditMallocSize)
	ON_BN_CLICKED(IDC_BTN_VMALLOC, OnBnClickedBtnVmalloc)
	ON_BN_CLICKED(IDC_BTN_VFREE, OnBnClickedBtnVfree)
END_MESSAGE_MAP()


// CTools5PanelDlg 消息处理程序

BOOL CTools5PanelDlg::OnInitDialog()
{
	CBasePanelDlg::OnInitDialog();

	this->InitUserOption();
	this->DispUserOption();

	return TRUE;  // return TRUE unless you set the focus to a control
}

void CTools5PanelDlg::InitUserOption(void)
{
	////this->InitMallocMap();
	//this->m_option.memsize = 0;
	////init list control
	//this->m_listmap.ModifyStyle(0, LVS_REPORT | LVS_SHOWSELALWAYS| LVS_SORTASCENDING | LVS_SINGLESEL);
	//this->m_listmap.SendMessage(LVM_SETEXTENDEDLISTVIEWSTYLE, 0, LVS_EX_GRIDLINES | LVS_EX_FULLROWSELECT);
	////this->m_listmap.InsertColumn(0, "index", LVCFMT_LEFT, 50, 0);
	//this->m_listmap.InsertColumn(0, "addr", LVCFMT_LEFT, 70, 0);
	//this->m_listmap.InsertColumn(1, "size", LVCFMT_LEFT, 70, 0);
	//this->m_listmap.InsertColumn(2, "used", LVCFMT_LEFT, 50, 0);
	//VMallocTestCase();
}

void CTools5PanelDlg::InitMallocMap(void)
{
	CPen pen(PS_SOLID, 10, RGB(128,128,128));
	CDC *pDC = this->GetDC();
	CPen *pPen = pDC->SelectObject(&pen);

	pDC->MoveTo(5, 5);
	pDC->LineTo(25, 25);

	pDC->SelectObject(pPen);
}

void CTools5PanelDlg::DispUserOption(void)
{
	char str[100];

	sprintf(str, "%d", this->m_option.memsize);
	this->m_memsize.SetWindowText(str);
	this->UpdataList();
}

void CTools5PanelDlg::GetUserOption(void)
{
	this->GetEditValue();
	this->GetListValue();
//	this->GetCheckValue();
}

void CTools5PanelDlg::GetEditValue(void)
{
	char str[100];

	this->m_memsize.GetWindowText(str, 100);
	sscanf(str, "%d", &(this->m_option.memsize));
}

void CTools5PanelDlg::GetListValue(void)
{
	//int i, count;
	//char str[20];
	//int used;

	//if(!this->m_listmap.GetSelectedCount())
	//{
	//	this->m_option.addr = NULL;
	//	return;
	//}
	//count = this->m_listmap.GetItemCount();
	//for(i = 0; i < count; i++)
	//{
	//	if(this->m_listmap.GetItemState(i, LVIS_SELECTED))
	//		break;
	//}
	//this->m_listmap.GetItemText(i, 2, str, 20);
	//sscanf(str, "%d", &used);
	//if(!used)
	//{
	//	this->m_option.addr = NULL;
	//	return;
	//}
	//this->m_listmap.GetItemText(i, 0, str, 20);
	//sscanf(str, "%x", &this->m_option.addr);
}

void CTools5PanelDlg::OnEnChangeEditMallocSize()
{
	// TODO:  如果该控件是 RICHEDIT 控件，则它将不会
	// 发送该通知，除非重写 CBasePanelDlg::OnInitDialog()
	// 函数并调用 CRichEditCtrl().SetEventMask()，
	// 同时将 ENM_CHANGE 标志“或”运算到掩码中。

	// TODO:  在此添加控件通知处理程序代码
}

void CTools5PanelDlg::UpdataList(void)
{
	//TTools5Memmap listMap;
	//PTVmallocChunk p;
	//unsigned int i;
	//int index = 0;

	//this->ClearList();
	//
	////Updata list item
	//p = (PTVmallocChunk)gMallocState.pAddr; 
	//for(i = 0; i < gMallocState.length; )
	//{
	//	listMap.addr = (unsigned int)((char *)p);
	//	listMap.size = CHUNKSIZE(p);
	//	listMap.used = (unsigned int)INUSE(p);
	//	i = i + listMap.size;
	//	p = (PTVmallocChunk)((char*)p+listMap.size);
	//	this->InsertListItem(index, &listMap);
	//	index++;
	//}
	//this->m_listmap.SetItemState(0, LVIS_SELECTED | LVIS_FOCUSED, 0x000f);
}

void CTools5PanelDlg::InsertListItem(int index, PTTools5Memmap listmap)
{
	//LVITEM lvi;
	//char str[20];

	//lvi.mask = LVIF_TEXT;
	//lvi.iItem = index;
	////lvi.iSubItem = 0;
	////sprintf(str, "%d", index);
	////lvi.pszText = (LPTSTR)str;
	////this->m_listmap.InsertItem(&lvi);

	//lvi.iSubItem = 0;
	//sprintf(str, "%x", listmap->addr);
	//lvi.pszText = (LPTSTR)str;
	//this->m_listmap.InsertItem(&lvi);

	//lvi.iSubItem = 1;
	//sprintf(str, "%d", listmap->size);
	//lvi.pszText = (LPTSTR)str;
	//this->m_listmap.SetItem(&lvi);

	//lvi.iSubItem = 2;
	//sprintf(str, "%d", listmap->used);
	//this->m_listmap.SetItem(&lvi);
}

void CTools5PanelDlg::ClearList(void)
{
	//int i;
	//int count;

	//count = this->m_listmap.GetItemCount();

	//for(i = count-1; i >= 0; i--)
	//	this->m_listmap.DeleteItem(i);
}

void CTools5PanelDlg::OnBnClickedBtnVmalloc()
{
	//this->GetUserOption();

	//if(this->m_option.memsize <= 0)
	//	return;
	//vmalloc(this->m_option.memsize);
	//this->UpdataList();
}


void CTools5PanelDlg::OnBnClickedBtnVfree()
{
	//void *p;

	//this->GetUserOption();
	//if(this->m_option.addr == NULL)
	//	return;
	//p = CHUNK2MEM((char*)this->m_option.addr);
	//vfree(p);
	//this->UpdataList();
}
