// HostPanelDlg.cpp : 实现文件
//
#include "internal.h"


// CHostPanelDlg 对话框

IMPLEMENT_DYNAMIC(CHostPanelDlg, CBaseCtrlDlg)
CHostPanelDlg::CHostPanelDlg(CWnd* pParent /*=NULL*/)
	: CBaseCtrlDlg(CHostPanelDlg::IDD, pParent)
{
}

CHostPanelDlg::~CHostPanelDlg()
{
}

void CHostPanelDlg::DoDataExchange(CDataExchange* pDX)
{
	CBaseCtrlDlg::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_ARMFILE_COMBO, m_armfileBox);
	DDX_Control(pDX, IDC_DATALENGTH_EDIT, m_buflen);
	DDX_Control(pDX, IDC_HOSTMCLK_EDIT, m_mclkBox);
	DDX_Control(pDX, IDC_I2SCLK_EDIT, m_i2sclkEdit);
	DDX_Control(pDX, IDC_BEEP_CHECK, m_beepbtn);
	DDX_Control(pDX, IDC_KEYPROCESS_CHECK, m_keyprocbtn);
	DDX_Control(pDX, IDC_T5INTERVAL_EDIT, m_t5intbox);

	DDX_Control(pDX, IDC_EXTINT4_7_COMBO, m_exint4567Box);
	DDX_Control(pDX, IDC_EXTINT3_COMBO, m_exint3Box);
	DDX_Control(pDX, IDC_EXTINT2_COMBO, m_exint2Box);
	DDX_Control(pDX, IDC_EXTINT1_COMBO, m_exint1Box);
	DDX_Control(pDX, IDC_EXTINT0_COMBO, m_exint0Box);
	DDX_Control(pDX, IDC_TIMER_COMBO, m_timerselBox);
	DDX_Control(pDX, IDC_WDTEN_CHECK, m_wdtenbtn);
}

BEGIN_MESSAGE_MAP(CHostPanelDlg, CBaseCtrlDlg)
	ON_BN_CLICKED(IDC_WRITEBUF_BUTTON, OnBnClickedWritebufButton)
	ON_BN_CLICKED(IDC_READBUF_BUTTON, OnBnClickedReadbufButton)
	ON_BN_CLICKED(IDC_DOWNLOAD_BUTTON, OnBnClickedDownloadButton)
	ON_BN_CLICKED(IDC_SETMCLK_BUTTON, OnBnClickedSetmclkButton)
	ON_BN_CLICKED(IDC_SAVE_BUTTON, OnBnClickedSaveButton)
	ON_BN_CLICKED(IDC_LOAD_BUTTON, OnBnClickedLoadButton)
	ON_BN_CLICKED(IDC_SAVEGBUF_BUTTON, OnBnClickedSavegbufButton)
	ON_BN_CLICKED(IDC_LOADGBUF_BUTTON, OnBnClickedLoadgbufButton)
	ON_BN_CLICKED(IDC_SETI2SCLK_BUTTON, OnBnClickedSeti2sclkButton)
	ON_BN_CLICKED(IDC_BEEP_CHECK, OnBnClickedBeepCheck)
	ON_BN_CLICKED(IDC_KEYPROCESS_CHECK, OnBnClickedKeyprocessCheck)
	ON_CBN_SELCHANGE(IDC_EXTINT4_7_COMBO, OnCbnSelchangeExtint4567Combo)
	ON_CBN_SELCHANGE(IDC_EXTINT3_COMBO, OnCbnSelchangeExtint3Combo)
	ON_CBN_SELCHANGE(IDC_EXTINT2_COMBO, OnCbnSelchangeExtint2Combo)
	ON_CBN_SELCHANGE(IDC_EXTINT1_COMBO, OnCbnSelchangeExtint1Combo)
	ON_CBN_SELCHANGE(IDC_EXTINT0_COMBO, OnCbnSelchangeExtint0Combo)
	ON_CBN_SELCHANGE(IDC_TIMER_COMBO, OnCbnSelchangeTimerCombo)
	ON_BN_CLICKED(IDC_SETTIMER_BUTTON, OnBnClickedSettimerButton)
	ON_BN_CLICKED(IDC_REFRESH_BUTTON, OnBnClickedRefreshButton)
	ON_BN_CLICKED(IDC_WDT_BUTTON, OnBnClickedWdtButton)
	ON_BN_CLICKED(IDC_WDTEN_CHECK, OnBnClickedWdtenCheck)
END_MESSAGE_MAP()

void CHostPanelDlg::InitUserOption(void)
{
	char str[20];
	int i;
	const char *ext_int[] = {"Low     Level",
							 "High    Level",
							 "Falling Edge ",
							 "Rising  Edge ",
							 "Both    Edge "};
	const char *timer[] = {"TIMER 0", "TIMER 1", "TIMER 2", "TIMER 3", "TIMER 4", "TIMER 5"};

	memset(&(this->m_option), 0, sizeof(THostOption));
	this->m_buflen.SetWindowText("0");
	this->m_t5intbox.SetWindowText("0");
	sprintf(str, "%d", DEFAULT_MCLK);
	this->m_mclkBox.SetWindowText(str);
	sprintf(str, "%d", I2S_DEFAULT_CLK);
	this->m_i2sclkEdit.SetWindowText(str);
	this->m_keyprocbtn.SetCheck(BST_CHECKED);

	for(i = 0; i < sizeof(ext_int) / sizeof(char *); i++)
		this->m_exint4567Box.InsertString(i, ext_int[i]);
	for(i = 0; i < sizeof(ext_int) / sizeof(char *); i++)
		this->m_exint3Box.InsertString(i, ext_int[i]);
	for(i = 0; i < sizeof(ext_int) / sizeof(char *); i++)
		this->m_exint2Box.InsertString(i, ext_int[i]);
	for(i = 0; i < sizeof(ext_int) / sizeof(char *); i++)
		this->m_exint1Box.InsertString(i, ext_int[i]);
	for(i = 0; i < sizeof(ext_int) / sizeof(char *); i++)
		this->m_exint0Box.InsertString(i, ext_int[i]);
	for(i = 0; i < sizeof(timer) / sizeof(char *); i++)
		this->m_timerselBox.InsertString(i, timer[i]);

	this->m_exint3Box.SetCurSel(0);
	this->m_exint1Box.SetCurSel(0);
	this->m_exint0Box.SetCurSel(0);
	this->m_timerselBox.SetCurSel(0);
	this->m_wdtenbtn.SetCheck(BST_CHECKED);
}

void CHostPanelDlg::GetUserOption(char *tail)
{
	GetEditboxValue();
	GetComboValue(tail);
}

void CHostPanelDlg::OnBnClickedWritebufButton()
{
	int length;
	CWaitCursor	cursor;
	CFileBin fb;
	CBufferBase pool;

	this->GetUserOption(FILE_TAIL_DAT);
	fb.Open(this->m_option.srcfile);
	length = fb.GetFileLength();
	length = (length > this->m_option.buflen) ? this->m_option.buflen : length;
	pool.SetSize(length);
	if(pool.GetSize() < length)
		return;
	fb.Read(pool.m_buf, length);
	WriteBuf(0, pool.m_buf, length);
}

void CHostPanelDlg::OnBnClickedReadbufButton()
{
	CWaitCursor	cursor;
	CFileBin fb;
	CBufferBase pool;

	this->GetUserOption(FILE_TAIL_DAT);
	pool.SetSize(this->m_option.buflen);
	if(pool.GetSize() < this->m_option.buflen)
		return;
	ReadBuf(0, pool.m_buf, this->m_option.buflen);
	fb.Open(this->m_option.dstfile, "wb");
	fb.Write(pool.m_buf, this->m_option.buflen);
}

BOOL CHostPanelDlg::OnInitDialog()
{
	CBaseCtrlDlg::OnInitDialog();

	this->InitUserOption();

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常：OCX 属性页应返回 FALSE
}

void CHostPanelDlg::OnBnClickedDownloadButton()
{
	CWaitCursor	cursor;

	this->GetUserOption(FILE_TAIL_DAT);
	DownloadFile(this->m_option.srcfile);
}

void CHostPanelDlg::OnBnClickedSaveButton()
{
	CWaitCursor	cursor;

	this->GetUserOption();
	PcFile2Arm(this->m_option.srcfile, this->m_option.armfile);
}

void CHostPanelDlg::OnBnClickedLoadButton()
{
	CWaitCursor	cursor;

	this->GetUserOption();
	ArmFile2Pc(this->m_option.armfile, this->m_option.dstfile);
}

void CHostPanelDlg::OnBnClickedSavegbufButton()
{
	CWaitCursor	cursor;

	this->GetUserOption();
	Gbuf2File(this->m_option.armfile, this->m_option.buflen, 0);
}

void CHostPanelDlg::OnBnClickedLoadgbufButton()
{
	CWaitCursor	cursor;

	this->GetUserOption();
	File2Gbuf(this->m_option.armfile, 0);
}

void CHostPanelDlg::OnBnClickedSetmclkButton()
{
	CWaitCursor	cursor;
	char str[100];

	this->GetUserOption();
	sprintf(str, "AdjustHostMclk %d\n", this->m_option.mclk);
	SendCmd(str);
	this->OnBnClickedRefreshButton();
}

void CHostPanelDlg::OnBnClickedSeti2sclkButton()
{
	CWaitCursor	cursor;
	char str[100];

	this->GetUserOption();
	sprintf(str, "AdjustI2sClk %d\n", this->m_option.i2sclk);
	SendCmd(str);
	this->OnBnClickedRefreshButton();
}

void CHostPanelDlg::OnBnClickedBeepCheck()
{
	if(this->m_beepbtn.GetCheck())
		beep(1);
	else
		beep(0);
}

void CHostPanelDlg::OnBnClickedKeyprocessCheck()
{
	if(this->m_keyprocbtn.GetCheck())
		KeyScanStart();
	else
		KeyScanStop();
}

void CHostPanelDlg::OnCbnSelchangeTimerCombo()
{
	this->m_option.timersel = this->m_timerselBox.GetCurSel();
}

void CHostPanelDlg::OnBnClickedSettimerButton()
{
	CWaitCursor	cursor;

	this->GetUserOption();
	SetHostTimer(this->m_option.timersel, this->m_option.tmrint);
}

void CHostPanelDlg::OnCbnSelchangeExtint4567Combo()
{
	//int type;

	//type = this->m_exint4567Box.GetCurSel();
	//SetExtIntTrgType(EXINT4567, type);
}

void CHostPanelDlg::OnCbnSelchangeExtint3Combo()
{
	int type;

	type = this->m_exint3Box.GetCurSel();
	SetExtIntTrgType(EXINT3, type);
}

void CHostPanelDlg::OnCbnSelchangeExtint2Combo()
{
	//int type;

	//type = this->m_exint2Box.GetCurSel();
	//SetExtIntTrgType(EXINT2, type);
}

void CHostPanelDlg::OnCbnSelchangeExtint1Combo()
{
	int type;

	type = this->m_exint1Box.GetCurSel();
	SetExtIntTrgType(EXINT1, type);
}

void CHostPanelDlg::OnCbnSelchangeExtint0Combo()
{
	int type;

	type = this->m_exint0Box.GetCurSel();
	SetExtIntTrgType(EXINT0, type);
}

void CHostPanelDlg::GetEditboxValue(void)
{
	char str[20];

	this->m_buflen.GetWindowText(str, 20);
	sscanf(str, "%x", &(this->m_option.buflen));
	this->m_mclkBox.GetWindowText(str, 20);
	sscanf(str, "%d", &(this->m_option.mclk));
	this->m_i2sclkEdit.GetWindowText(str, 20);
	sscanf(str, "%d", &(this->m_option.i2sclk));
	this->m_t5intbox.GetWindowText(str, 20);
	sscanf(str, "%d", &(this->m_option.tmrint));
}

void CHostPanelDlg::GetComboValue(char *tail)
{
	this->m_armfileBox.GetWindowText(this->m_option.armfile, DEFAULT_FILENAME_LEN);
	CBasePanelDlg::GetFileBoxOption(this->m_option.srcfile, this->m_option.dstfile, tail);
}

void CHostPanelDlg::DispOption(void)
{
	char str[20];

	sprintf(str, "%d", this->m_option.tmrint);
	this->m_t5intbox.SetWindowText(str);
	sprintf(str, "%d", this->m_option.mclk);
	this->m_mclkBox.SetWindowText(str);
	sprintf(str, "%d", this->m_option.i2sclk);
	this->m_i2sclkEdit.SetWindowText(str);
}

void CHostPanelDlg::OnBnClickedRefreshButton()
{
	this->m_option.tmrint = GetHostTimer(this->m_timerselBox.GetCurSel());
	this->m_option.i2sclk = GetI2sClk();
	this->m_option.mclk = GetHostMclk();

	DispOption();

	this->m_exint3Box.SetCurSel(GetExtIntTrgType(EXINT3));
	this->m_exint1Box.SetCurSel(GetExtIntTrgType(EXINT1));
	this->m_exint0Box.SetCurSel(GetExtIntTrgType(EXINT0));
}

void CHostPanelDlg::OnBnClickedWdtButton()
{
	int time = 0;
	char str[20];

	if(this->m_option.flag == 0)
	{
		this->m_option.wdtcnt = WdtGetCount();

		this->m_option.flag = 1;
		GetDlgItem(IDC_WDT_EDIT)->SetWindowText("");
		GetDlgItem(IDC_WDT_BUTTON)->SetWindowText("Stop");

		return;
	}

	time = WdtGetCount();	// ms
	time = (time - this->m_option.wdtcnt) / 10;

	this->m_option.flag = 0;
	GetDlgItem(IDC_WDT_BUTTON)->SetWindowText("Start");
	sprintf(str, "%d", time);
	GetDlgItem(IDC_WDT_EDIT)->SetWindowText(str);
}

void CHostPanelDlg::OnBnClickedWdtenCheck()
{
	if(this->m_wdtenbtn.GetCheck())
	{
		EnableWdtInt();
		GetDlgItem(IDC_WDT_BUTTON)->EnableWindow(TRUE);
		return;
	}

	WdtStop();
	DisableWdtInt();
	GetDlgItem(IDC_WDT_BUTTON)->EnableWindow(FALSE);
}
