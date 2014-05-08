// V568MarbDlg.cpp : 实现文件
//

#include "internal.h"


#ifndef	V578CMD_DISABLE



// CV568MarbDlg 对话框

IMPLEMENT_DYNAMIC(CV568MarbDlg, CBaseCtrlDlg)
CV568MarbDlg::CV568MarbDlg(CWnd* pParent /*=NULL*/)
	: CBaseCtrlDlg(CV568MarbDlg::IDD, pParent)
{
}

CV568MarbDlg::~CV568MarbDlg()
{
}

void CV568MarbDlg::DoDataExchange(CDataExchange* pDX)
{
	CBaseCtrlDlg::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_A1STARTEDIT, m_a1startbox);
	DDX_Control(pDX, IDC_A1END_EDIT, m_a1endbox);
	DDX_Control(pDX, IDC_A0START_EDIT, m_a0startbox);
	DDX_Control(pDX, IDC_A0END_EDIT, m_a0endbox);
	DDX_Control(pDX, IDC_JBUFSTART_EDIT, m_jbufstartbox);
	DDX_Control(pDX, IDC_JBUFEND_EDIT, m_jbufendbox);
	DDX_Control(pDX, IDC_INDEXSTART_EDIT, m_indexstartbox);
	DDX_Control(pDX, IDC_INDEXEND_EDIT, m_indexendbox);
	DDX_Control(pDX, IDC_THUMBSTART_EDIT, m_thumbstartbox);
	DDX_Control(pDX, IDC_THUMBEND_EDIT, m_thumbendbox);
	DDX_Control(pDX, IDC_LBUF1START_EDIT, m_lbuf1startbox);
	DDX_Control(pDX, IDC_LBUF1END_EDIT, m_lbuf1endbox);
	DDX_Control(pDX, IDC_LBUF0START_EDIT, m_lbuf0startbox);
	DDX_Control(pDX, IDC_LBUF0END_EDIT, m_lbuf0endbox);
	DDX_Control(pDX, IDC_B1START_EDIT, m_b1startbox);
	DDX_Control(pDX, IDC_B1END_EDIT, m_b1endbox);
	DDX_Control(pDX, IDC_B0START_EDIT, m_b0startbox);
	DDX_Control(pDX, IDC_B0END_EDIT, m_b0endbox);
	DDX_Control(pDX, IDC_WORKMODE_COMBO, m_workmodebox);
	DDX_Control(pDX, IDC_ALLOCMODE_COMBO, m_allocmodebox);
	DDX_Control(pDX, IDC_GAPRATIO_COMBO, m_gapratiobox);
	DDX_Control(pDX, IDC_IBUFSIZE_EDIT, m_ibufsizebox);
	DDX_Control(pDX, IDC_RAMSTART_EDIT, m_ramstartbox);
	DDX_Control(pDX, IDC_RAMEND_EDIT, m_ramendbox);
}


BEGIN_MESSAGE_MAP(CV568MarbDlg, CBaseCtrlDlg)
	ON_BN_CLICKED(IDC_REFRESHMAP_BUTTON, OnBnClickedRefreshmapButton)
	ON_BN_CLICKED(IDC_IBUFSIZE_BUTTON, OnBnClickedIbufsizeButton)
	ON_CBN_SELCHANGE(IDC_GAPRATIO_COMBO, OnCbnSelchangeGapratioCombo)
	ON_CBN_SELCHANGE(IDC_WORKMODE_COMBO, OnCbnSelchangeWorkmodeCombo)
	ON_CBN_SELCHANGE(IDC_ALLOCMODE_COMBO, OnCbnSelchangeAllocmodeCombo)
	ON_BN_CLICKED(IDC_RAMSTART_BUTTON, OnBnClickedRamstartButton)
	ON_BN_CLICKED(IDC_RAMEND_BUTTON, OnBnClickedRamendButton)
END_MESSAGE_MAP()


// CV568MarbDlg 消息处理程序
void CV568MarbDlg::InitUserOption(void)
{
	int i;
	char str[20];

	memset(&(this->m_option), 0, sizeof(this->m_option));
	for(i = 0; i < gV568WmodeCount; i++)
		this->m_workmodebox.InsertString(i, gV568WorkModeDesc[i]);
	for(i = 0; i < gV568AllocModeCnt; i++)
		this->m_allocmodebox.InsertString(i, gV568MemAllocmodeDesc[i]);
	for(i = 1; i < MAX_GAPRATIO+1; i++)
	{
		sprintf(str, "%x", i);
		this->m_gapratiobox.InsertString(i-1, str);
	}
	this->m_option.isize = DEFAULT_INDEXSIZE;
	this->m_option.gapratio = DEFAULT_GAPRATIO;
	this->m_option.ramend = V5_SRAM_TOTALSIZE;
	this->m_option.ramstart = 0;
}

void CV568MarbDlg::GetUserOption(void)
{
	char str[20];
	CStringTools tools;

	this->m_option.mode = this->m_workmodebox.GetCurSel();
	this->m_option.almode = this->m_allocmodebox.GetCurSel();
	this->m_option.gapratio = this->m_gapratiobox.GetCurSel() + 1;
	this->m_ibufsizebox.GetWindowText(str, 20);
	tools.Str2Hex(str, &(this->m_option.isize));
	this->m_ramendbox.GetWindowText(str, 20);
	tools.Str2Hex(str, &(this->m_option.ramend));
	this->m_ramstartbox.GetWindowText(str, 20);
	tools.Str2Hex(str, &(this->m_option.ramstart));
}

void CV568MarbDlg::DispOption(void)
{
	char str[20];

	this->m_workmodebox.SetCurSel(this->m_option.mode);
	this->m_allocmodebox.SetCurSel(this->m_option.almode);
	this->m_gapratiobox.SetCurSel(this->m_option.gapratio-1);
	sprintf(str, "%x", this->m_option.isize);
	this->m_ibufsizebox.SetWindowText(str);
	sprintf(str, "%x", this->m_option.ramend);
	this->m_ramendbox.SetWindowText(str);
	sprintf(str, "%x", this->m_option.ramstart);
	this->m_ramstartbox.SetWindowText(str);

	sprintf(str, "0x%x", this->m_option.map.indexend);
	this->m_indexendbox.SetWindowText(str);
	sprintf(str, "0x%x", this->m_option.map.indexstart);
	this->m_indexstartbox.SetWindowText(str);
	sprintf(str, "0x%x", this->m_option.map.jbufend);
	this->m_jbufendbox.SetWindowText(str);
	sprintf(str, "0x%x", this->m_option.map.jbufstart);
	this->m_jbufstartbox.SetWindowText(str);
	sprintf(str, "0x%x", this->m_option.map.layerA0end);
	this->m_a0endbox.SetWindowText(str);
	sprintf(str, "0x%x", this->m_option.map.layerA0start);
	this->m_a0startbox.SetWindowText(str);
	sprintf(str, "0x%x", this->m_option.map.layerA1end);
	this->m_a1endbox.SetWindowText(str);
	sprintf(str, "0x%x", this->m_option.map.layerA1start);
	this->m_a1startbox.SetWindowText(str);

	sprintf(str, "0x%x", this->m_option.map.layerB0end);
	this->m_b0endbox.SetWindowText(str);
	sprintf(str, "0x%x", this->m_option.map.layerB0start);
	this->m_b0startbox.SetWindowText(str);
	sprintf(str, "0x%x", this->m_option.map.layerB1end);
	this->m_b1endbox.SetWindowText(str);
	sprintf(str, "0x%x", this->m_option.map.layerB1start);
	this->m_b1startbox.SetWindowText(str);
	sprintf(str, "0x%x", this->m_option.map.lbuf0start);
	this->m_lbuf0startbox.SetWindowText(str);
	sprintf(str, "0x%x", this->m_option.map.lbuf1start);
	this->m_lbuf0endbox.SetWindowText(str);
	sprintf(str, "0x%x", this->m_option.map.lbuf1start);
	this->m_lbuf1startbox.SetWindowText(str);
	sprintf(str, "0x%x", this->m_option.map.lbufend);
	this->m_lbuf1endbox.SetWindowText(str);
	sprintf(str, "0x%x", this->m_option.map.thumbend);
	this->m_thumbendbox.SetWindowText(str);
	sprintf(str, "0x%x", this->m_option.map.thumbstart);
	this->m_thumbstartbox.SetWindowText(str);
}

BOOL CV568MarbDlg::OnInitDialog()
{
	CBaseCtrlDlg::OnInitDialog();

	// TODO:  在此添加额外的初始化
	this->InitUserOption();
	this->DispOption();

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常：OCX 属性页应返回 FALSE
}


void CV568MarbDlg::OnBnClickedRefreshmapButton()
{
	CWaitCursor cursor;

	V5_MarbGetMap(&(this->m_option.map));
	this->m_option.mode = V5_GetMode();
	this->m_option.almode = V5_MarbGetAllocMode();
	this->m_option.gapratio = V5_MarbGetGapRatio();
	this->m_option.isize = V5_MarbGetIndexMem();
	this->m_option.ramend = V5_MarbGetMemEnd();
	this->m_option.ramstart = V5_MarbGetMemStart();
	this->DispOption();
}

void CV568MarbDlg::OnBnClickedIbufsizeButton()
{
	CWaitCursor cursor;

	this->GetUserOption();
	V5_MarbSetIndexMem((UINT32)this->m_option.isize);
	V5_MarbAutoSetMem();
	this->OnBnClickedRefreshmapButton();
}

void CV568MarbDlg::OnCbnSelchangeGapratioCombo()
{
	CWaitCursor cursor;

	this->GetUserOption();
	V5_MarbSetGapRatio((UINT8)this->m_option.gapratio);
	this->OnBnClickedRefreshmapButton();
}

void CV568MarbDlg::OnCbnSelchangeWorkmodeCombo()
{
	CWaitCursor cursor;

	this->GetUserOption();
	V5_SetMode((UINT8)(this->m_option.mode));
	V5_MarbAutoSetMem();
	this->OnBnClickedRefreshmapButton();
}

void CV568MarbDlg::OnCbnSelchangeAllocmodeCombo()
{
	CWaitCursor cursor;

	this->GetUserOption();
	V5_MarbSetAllocMode((UINT8)this->m_option.almode);
	V5_MarbAutoSetMem();
	this->OnBnClickedRefreshmapButton();
}

void CV568MarbDlg::OnBnClickedRamstartButton()
{
	CWaitCursor cursor;

	this->GetUserOption();
	V5_MarbSetMemStart((UINT32)this->m_option.ramstart);
	V5_MarbAutoSetMem();
	this->OnBnClickedRefreshmapButton();
}

void CV568MarbDlg::OnBnClickedRamendButton()
{
	CWaitCursor cursor;

	this->GetUserOption();
	V5_MarbSetMemEnd((UINT32)this->m_option.ramend);
	V5_MarbAutoSetMem();
	this->OnBnClickedRefreshmapButton();
}

#endif //V578CMD_DISABLE
