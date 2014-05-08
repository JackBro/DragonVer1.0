#include "internal.h"
#include "..\..\include\cmodelview\basecmviewdlg.h"


// CBaseCmViewDlg 对话框

IMPLEMENT_DYNAMIC(CBaseCmViewDlg, CBaseDlg)
CBaseCmViewDlg::CBaseCmViewDlg(CWnd* pParent /*=NULL*/)
	: CBaseDlg(CBaseCmViewDlg::IDD, pParent)
{
}

CBaseCmViewDlg::~CBaseCmViewDlg()
{
}

void CBaseCmViewDlg::DoDataExchange(CDataExchange* pDX)
{
	CBaseDlg::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_SIGNALIN_COMBO, m_inpinbox);
	DDX_Control(pDX, IDC_SIGNALOUT_COMBO, m_oupinbox);
	DDX_Control(pDX, IDC_BLACKBOX_COMBO, m_indexbox);
	DDX_Control(pDX, IDC_MODULES_COMBO, m_srcfbox);
	DDX_Control(pDX, IDC_CLASSTYPE_COMBO, m_classtype);
}

BEGIN_MESSAGE_MAP(CBaseCmViewDlg, CBaseDlg)
	ON_BN_CLICKED(IDC_INSERT_BUTTON, OnBnClickedInsertButton)
	ON_WM_PAINT()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_RBUTTONDOWN()
	ON_COMMAND(ID_BOXRMENU_PROPERTY, OnBoxrmenuProperty)
	ON_COMMAND(ID_BOXRMENU_SET, OnBoxrmenuSet)
	ON_COMMAND(ID_BOXRMENU_DELETE, OnBoxrmenuDelete)
	ON_WM_LBUTTONDBLCLK()
	ON_BN_CLICKED(IDC_MODULERUN_BUTTON, OnBnClickedModulerunButton)
	ON_CBN_SELCHANGE(IDC_CLASSTYPE_COMBO, OnCbnSelchangeClasstypeCombo)
	ON_BN_CLICKED(IDC_SAVEMODULE_BUTTON, OnBnClickedSavemoduleButton)
	ON_BN_CLICKED(IDC_LOADMODULE_BUTTON, OnBnClickedLoadmoduleButton)
	ON_BN_CLICKED(IDC_OPENMODULE_BUTTON, OnBnClickedOpenmoduleButton)
END_MESSAGE_MAP()

// CBaseCmViewDlg 消息处理程序

BOOL CBaseCmViewDlg::OnInitDialog()
{
	CBaseDlg::OnInitDialog();

	// TODO:  在此添加额外的初始化
	this->InitUserOption();
	this->DispOption();

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常：OCX 属性页应返回 FALSE
}

void CBaseCmViewDlg::InitUserOption(void)
{
	char str[10];
	int i;

	memset(&this->m_option, 0, sizeof(TCmViewOption));
	for(i = 0; i < 8; i++)
	{
		sprintf(str, "%d", i);
		this->m_inpinbox.InsertString(i, str);
		this->m_oupinbox.InsertString(i, str);
	}
	
	for(i = 0; i < g_FmtGroupTotalCount; i++)
		m_classtype.InsertString(i, g_fmtGroup[i].cname);
	m_classtype.SetCurSel(0);

	this->OnCbnSelchangeClasstypeCombo();
//	for(i = 0; i < gIspTotalBoxCount; i++)
//		this->m_indexbox.InsertString(i, gIsp_BoxDesc[i]);	
//	m_indexbox.SetCurSel(0);
}

void CBaseCmViewDlg::DispOption(void)
{
	this->m_inpinbox.SetCurSel(this->m_option.inpin);
	this->m_oupinbox.SetCurSel(this->m_option.oupin);
	this->m_indexbox.SetCurSel(this->m_option.index);
}

void CBaseCmViewDlg::GetUserOption(void)
{
	CString str;

	this->m_srcfbox.GetWindowText(this->m_option.srcf, DEFAULT_FILENAME_LEN);
	this->m_option.inpin = this->m_inpinbox.GetCurSel();
	this->m_option.oupin = this->m_oupinbox.GetCurSel();
	this->m_option.index = this->m_indexbox.GetCurSel() | (this->m_classtype.GetCurSel() << CLASSSHIFT);
}

void CBaseCmViewDlg::OnBnClickedInsertButton()
{
	CWaitCursor cursor;

	this->GetUserOption();
	this->m_map.Add(this->m_option.index);
	this->Invalidate();
}

void CBaseCmViewDlg::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	Graphics graph(this->m_hWnd);
	Pen pen((DWORD)Color::Black, 1);
	int i, j, k, count;
	CUnitRect *prect;
	TMap *pmap;
	int sx, sy, ex, ey;
	char str[256];

	count = this->m_map.GetUnitCount();	//draw unit box
	for(i = 0; i < count; i++)
	{
		prect = this->m_map[i];
		if(prect->GetState())
			pen.SetColor((DWORD)Color::Blue);

		graph.DrawRectangle(&pen, prect->m_body.x, prect->m_body.y,
			prect->m_body.width, prect->m_body.height);
		k = prect->m_inpin.GetUnitCount();
		for(j = 0; j < k; j++)
			graph.DrawRectangle(&pen, prect->m_inpin[j]->x, prect->m_inpin[j]->y,
			prect->m_inpin[j]->width, prect->m_inpin[j]->height);
		k = prect->m_oupin.GetUnitCount();
		for(j = 0; j < k; j++)
			graph.DrawRectangle(&pen, prect->m_oupin[j]->x, prect->m_oupin[j]->y,
			prect->m_oupin[j]->width, prect->m_oupin[j]->height);

		sprintf(str, "U%d: %s", prect->GetUnit()->GetNum(), prect->GetUnit()->GetDesc());
		dc.TextOut(prect->m_body.x+10, prect->m_body.y + DEFAULT_PORT_STEP,
			str, (int)strlen(str));

		if(prect->GetState())
			pen.SetColor((DWORD)Color::Black);
	}

	count = this->m_map.GetPinCount();
	for(i = 0; i < count; i++)
	{
		if( this->m_map.GetPinPos(i, &sx, &sy, &ex, &ey) == 0 )
		{
			pmap = this->m_map.GetPinMap(i);
			if(pmap->state == 1)
				pen.SetColor((DWORD)Color::Blue);
			graph.DrawLine(&pen, sx, sy, ex, ey);
			if(pmap->state == 1)
				pen.SetColor((DWORD)Color::Black);
		}
	}
}

void CBaseCmViewDlg::OnLButtonDown(UINT nFlags, CPoint point)
{
	if(this->m_map.CheckLdown(point.x, point.y) & MODIFIED_SELECT)
		this->Invalidate();
	CBaseDlg::OnLButtonDown(nFlags, point);
}

void CBaseCmViewDlg::OnLButtonUp(UINT nFlags, CPoint point)
{
	int start = -1, end = -1;

	if(this->m_map.CheckLup(point.x, point.y, &start, &end))	//only move
		this->Invalidate();
	if(this->m_map.Connect(start, end))
		this->Invalidate();
	CBaseDlg::OnLButtonUp(nFlags, point);
}

void CBaseCmViewDlg::OnRButtonDown(UINT nFlags, CPoint point)
{
	int pos;
	CMenu PopMenu, *pMenu;
	POINT pt;

	pos = this->m_map.CheckLdown(point.x, point.y);
	if(pos & (UNIT_SELECTED | PIN_SELECTED))
	{
		this->Invalidate();
		pt.x = point.x;
		pt.y = point.y;
		PopMenu.LoadMenu(IDR_BOX_MENU);
		this->ClientToScreen(&pt);
		pMenu = PopMenu.GetSubMenu(0);
		pMenu->TrackPopupMenu(TPM_LEFTALIGN | TPM_RIGHTBUTTON, pt.x, pt.y, this);
	}
	else if(pos & MODIFIED_SELECT)
		this->Invalidate();
	CBaseDlg::OnRButtonDown(nFlags, point);
}

void CBaseCmViewDlg::OnBoxrmenuProperty()
{
	CUnitRect *pur;
	CBaseUnit *punit;
	CBaseBoxPropDlg *pdlg;
	CPortBase *port;
	TMap *pmap;
	int state;

	pur = this->m_map.GetCurrentUnit();
	if(pur == NULL)
	{
		pmap = this->m_map.GetCurrentPinMap();
		if(pmap == NULL)
			return;
		this->m_PinDlg.SetPin(&(pmap->pin));
		this->m_PinDlg.DoModal();
		return;
	}
	state = pur->GetState();
	punit = pur->GetUnit();
	if(state & INPUT_PIN_FLAG)
	{
		port = punit->GetInport(state & PIN_MASK);
		this->m_InpDlg.SetPort(port);
		this->m_InpDlg.DoModal();
	}
	else if(state & OUTPUT_PIN_FLAG)
	{
		port = punit->GetOuport(state & PIN_MASK);
		this->m_OupDlg.SetPort(port);
		this->m_OupDlg.DoModal();
	}
	else if(state == INSIDE_BOX_FLAG)
	{
		pdlg = this->m_map.GetPropDlg(punit->GetID());
		pdlg->SetUnit(punit);
		pdlg->DoModal();
		this->Invalidate();
	}
	this->m_map.PreProcess();
}

void CBaseCmViewDlg::OnBoxrmenuSet()
{
}

void CBaseCmViewDlg::OnBoxrmenuDelete()
{
	this->m_map.DelCurrent();
	this->Invalidate();
}

void CBaseCmViewDlg::OnLButtonDblClk(UINT nFlags, CPoint point)
{
	int pos;

	pos = this->m_map.CheckLdown(point.x, point.y);
	if(pos & (UNIT_SELECTED | PIN_SELECTED))
		this->OnBoxrmenuProperty();

	CBaseDlg::OnLButtonDblClk(nFlags, point);
}

void CBaseCmViewDlg::OnBnClickedModulerunButton()
{
	CWaitCursor cursor;

	this->m_map.Process();
}

void CBaseCmViewDlg::OnCbnSelchangeClasstypeCombo()
{
	int i, index;

	m_indexbox.ResetContent();
	index = m_classtype.GetCurSel();
	for(i = 0; i < g_fmtGroup[index].fmtcount; i++)
		this->m_indexbox.InsertString(i, g_fmtGroup[index].format[i]);	
	m_indexbox.SetCurSel(0);
}

void CBaseCmViewDlg::OnBnClickedSavemoduleButton()
{
	CWaitCursor cursor;
	CFileBin fb;

	this->GetUserOption();
	this->m_parmtree.Clear();
	this->m_map.StoreParm(&this->m_parmtree);
	fb.Open(this->m_option.srcf, "wb");
	this->m_parmtree.Save(&fb);
}

void CBaseCmViewDlg::OnBnClickedLoadmoduleButton()
{
	CWaitCursor cursor;

	this->GetUserOption();
	this->m_map.LoadParm(&this->m_parmtree);
	this->Invalidate();
}

void CBaseCmViewDlg::OnBnClickedOpenmoduleButton()
{
	CString str;

	if(CBaseDlg::OpenFileDlg(&str))
		this->m_srcfbox.addString(str.GetBuffer());
}

void CBaseCmViewDlg::OnOK()
{
}

void CBaseCmViewDlg::OnCancel()
{
}
