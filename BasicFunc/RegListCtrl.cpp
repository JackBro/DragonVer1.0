// RegListCtrl.cpp : 实现文件
//
#include "internal.h"


#define IDC_LISTUNIT_EDITBOX	6000

// CRegListCtrl

IMPLEMENT_DYNAMIC(CRegListCtrl, CListCtrl)
CRegListCtrl::CRegListCtrl()
{
	this->ClearContext();
}

CRegListCtrl::~CRegListCtrl()
{
}

BEGIN_MESSAGE_MAP(CRegListCtrl, CListCtrl)
	ON_NOTIFY_REFLECT(NM_CLICK, OnNMClick)
	ON_WM_MOUSEMOVE()
END_MESSAGE_MAP()

// CRegListCtrl 消息处理程序

void CRegListCtrl::InitRegList(void)//(CMToolTip *ptip)
{
	RECT rect;

	rect.left = 10;
	rect.top = 10;
	rect.right = 20;
	rect.bottom = 20;
	this->m_pRegUnit = NULL;
	this->ClearContext();
	this->m_unit.Create(BS_PUSHBUTTON, rect, this, IDC_LISTUNIT_EDITBOX);
	this->m_unit.ShowWindow(SW_HIDE);
	this->InsertColumn(REGLIST_REGTYPE, "Reg Type", LVCFMT_LEFT, 80);
	this->InsertColumn(REGLIST_REGNAME, "Reg Name", LVCFMT_LEFT, 240);
	this->InsertColumn(REGLIST_REGADDR, "Reg Addr", LVCFMT_LEFT, 80);
	this->InsertColumn(REGLIST_VALHEX, "Value(Hex)", LVCFMT_LEFT, 80);
	this->InsertColumn(REGLIST_VALDEC, "Value(Dec)", LVCFMT_LEFT, 80);
	this->InsertColumn(REGLIST_VALBINERY, "Value(Bin)", LVCFMT_LEFT, 80);
	this->InsertColumn(REGLIST_DESC, "Description", LVCFMT_LEFT, 400);
	this->InsertColumn(REGILST_NOTES, "Notes", LVCFMT_LEFT, 80);
	EnableToolTips(TRUE); 
	this->m_tip.Create(this); 
	this->m_tip.Activate(TRUE);
	this->m_tip.AddTool(this);
}

void CRegListCtrl::ClearContext(void)
{
	this->m_item = -1;
//	this->m_tipindex = -1;
}

void CRegListCtrl::SetContent(PTRegUnit pContent, int count)
{
	int i;
	char str[100];

	for(i = 0; i < count; i++)
	{
		sprintf(str, "0x%x", i);
		this->InsertItem(i, str);
		this->SetUnitItem(i, pContent+i);
	}
	this->m_pRegUnit = pContent;
}

void CRegListCtrl::GetUnitItem(int index, PTRegUnit pContent)
{
	//int i;
	char str[100];

	*pContent = this->m_pRegUnit[index];
	//pContent->delay = 0;
	//this->GetItemText(index, REGLIST_REGADDR, str, 100);
	//sscanf(str, "0x%x", &(pContent->regaddr));
	this->GetItemText(index, REGLIST_VALHEX, str, 100);
	sscanf(str, "%x", &(pContent->regval));
	//this->GetItemText(index, REGLIST_REGTYPE, str, 100);
	//for(i = 0; i < gRegTotalType; i++)
	//{
	//	if(strcmp(str, gRegType[i]) == 0)
	//	{
	//		pContent->regtype = i;
	//		break;
	//	}
	//}
}

void CRegListCtrl::SetUnitItem(int index, int val)
{
	char str[100];
	CStringTools tools;

	sprintf(str, "%x", val);
	this->SetItemText(index, REGLIST_VALHEX, str);
	sprintf(str, "%d", val);
	this->SetItemText(index, REGLIST_VALDEC, str);
	tools.Val2Binery(val, str);
	this->SetItemText(index, REGLIST_VALBINERY, str);
}

void CRegListCtrl::SetUnitItem(int index, PTRegUnit pContent)
{
	char str[100];

	this->SetItemText(index, REGLIST_REGTYPE, gRegType[pContent->regtype]);
	this->SetItemText(index, REGLIST_REGNAME, pContent->regname);
	sprintf(str, "0x %x", pContent->regaddr);
	this->SetItemText(index, REGLIST_REGADDR, str);
	this->SetUnitItem(index, pContent->regval);
	if(pContent->desc)
		this->SetItemText(index, REGLIST_DESC, pContent->desc);
}

void CRegListCtrl::OnNMClick(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE temp = (LPNMITEMACTIVATE) pNMHDR;
	CRect rect;
	int x, y;
	char str[100];

	*pResult = 0;
	x = temp->iItem;
	y = temp->iSubItem;
	if( (x == -1) || (y != REGLIST_VALHEX) )
	{
		this->m_unit.ShowWindow(SW_HIDE);
		this->ModifyRegVal();
		return;
	}
	if( (this->m_item != x) || (this->m_subitem != y) )
		this->ModifyRegVal();
	this->m_item = x;
	this->m_subitem = y;
	this->GetItemText(this->m_item, this->m_subitem, str, 100);
	this->GetSubItemRect(this->m_item, this->m_subitem, LVIR_BOUNDS, rect);

	this->m_unit.SetWindowPos(&wndTop, rect.left+1, rect.top, 
		rect.right-rect.left-1, rect.bottom-rect.top+4, SWP_SHOWWINDOW);
	this->m_unit.SetFocus();
	this->m_unit.SetWindowText(str);
	this->m_unit.SetSel(0, -1);
}

void CRegListCtrl::ModifyRegVal(void)
{
	char str[100];
	TRegUnit regunit;
	CWaitCursor cursor;

	if(this->m_item == -1)
		return;
	this->GetUnitItem(this->m_item, &regunit);
	this->m_unit.GetWindowText(str, 100);
	sscanf(str, "%x", &(regunit.regval));

	this->WriteReg(&regunit);
	this->ReadReg(&regunit);
	this->SetUnitItem(this->m_item, regunit.regval);
	this->m_item = -1;
}

void CRegListCtrl::OnMouseMove(UINT nFlags, CPoint point)
{
	int index;
	char str[0x300] = "";

	index = this->HitTest(point);
//	if(index != this->m_tipindex)
	{
		if(index != -1)
			this->GetItemText(index, REGLIST_DESC, str, 0x300);
		this->m_tip.UpdateTipText(str, this);
//		this->m_tipindex = index;
	}
	__super::OnMouseMove(nFlags, point);
}

BOOL CRegListCtrl::PreTranslateMessage(MSG* pMsg)
{
	// TODO: 在此添加专用代码和/或调用基类

	this->m_tip.RelayEvent(pMsg);
	return __super::PreTranslateMessage(pMsg);
}

