// UsbChipDlg.cpp : 实现文件
//
#include "internal.h"

#ifndef	USBCMD_DISABLE



// CUsbChipDlg 对话框

IMPLEMENT_DYNAMIC(CUsbChipDlg, CBaseCtrlDlg)
CUsbChipDlg::CUsbChipDlg(CWnd* pParent /*=NULL*/)
	: CBaseCtrlDlg(CUsbChipDlg::IDD, pParent)
{
}

CUsbChipDlg::~CUsbChipDlg()
{
}

void CUsbChipDlg::DoDataExchange(CDataExchange* pDX)
{
	CBaseCtrlDlg::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_WRITEEP_COMBO, m_wepbox);
	DDX_Control(pDX, IDC_READEP_COMBO, m_repbox);
	DDX_Control(pDX, IDC_DATALENGTH_EDIT, m_buflenedit);
	DDX_Control(pDX, IDC_ATTACHNODE_CHECK, m_atnbtn);
	DDX_Control(pDX, IDC_EDIT_RW_TIMES, m_rwcntedit);
	DDX_Control(pDX, IDC_EDIT_RW_USETIMES, m_rwtimeedit);
}


BEGIN_MESSAGE_MAP(CUsbChipDlg, CBaseCtrlDlg)
	ON_BN_CLICKED(IDC_WRITEFIFO_BTN, OnBnClickedWritefifoBtn)
	ON_BN_CLICKED(IDC_READFIFO_BTN, OnBnClickedReadfifoBtn)
	ON_BN_CLICKED(IDC_ATTACHNODE_CHECK, OnBnClickedAttachnodeCheck)
	ON_BN_CLICKED(IDC_DBUGPRINT_BTN, OnBnClickedDbugprintBtn)
	ON_BN_CLICKED(IDC_USBCHIP_MISC_BTN, OnBnClickedUsbchipMiscBtn)
	ON_BN_CLICKED(IDC_BUTTON_READ, OnBnClickedButtonRead)
	ON_BN_CLICKED(IDC_BUTTON_WRITE, OnBnClickedButtonWrite)
	ON_BN_CLICKED(IDC_BUTTON_READ_REG, OnBnClickedButtonReadReg)
	ON_BN_CLICKED(IDC_BUTTON_WR_REG, OnBnClickedButtonWrReg)
END_MESSAGE_MAP()


// CUsbChipDlg 消息处理程序

BOOL CUsbChipDlg::OnInitDialog()
{
	CBaseCtrlDlg::OnInitDialog();

	// TODO:  在此添加额外的初始化
	this->InitUserOption();
	this->m_wepbox.SetCurSel(0);
	this->m_repbox.SetCurSel(0);

	return TRUE;  // return TRUE unless you set the focus to a control
}

void CUsbChipDlg::InitUserOption(void)
{
	int i;
	char str[10];

	memset(&(this->m_option), 0, sizeof(TUsbOption));
	for(i = 0; i < 4; i++)
	{
		sprintf(str, "%d", i);
		this->m_wepbox.InsertString(i, str);
		this->m_repbox.InsertString(i, str);
	}
	this->m_buflenedit.SetMaxCount(2);
	this->m_buflenedit.SetWindowText("0");
	this->m_rwcntedit.SetWindowText("0");
	this->m_rwtimeedit.SetWindowText("0");
}

void CUsbChipDlg::DispUserOption(void)
{
	char str[20];

	sprintf(str, "%x", this->m_option.rwtime);
	this->m_rwtimeedit.SetWindowText(str);
}

void CUsbChipDlg::GetUserOption(char *tail)
{
	char str[20];

	this->m_buflenedit.GetWindowText(str, 20);
	sscanf(str, "%x", &(this->m_option.buflen));
	this->m_wepbox.GetWindowText(str, 20);
	sscanf(str, "%x", &(this->m_option.wep));
	this->m_repbox.GetWindowText(str, 20);
	sscanf(str, "%x", &(this->m_option.rep));
	CBasePanelDlg::GetFileBoxOption(this->m_option.srcfile, this->m_option.dstfile, tail);
	this->m_rwcntedit.GetWindowText(str, 20);
	sscanf(str, "%x", &(this->m_option.buflen));
	this->m_rwcntedit.GetWindowText(str, 20);
	sscanf(str, "%x", &(this->m_option.rwcnt));
}

void CUsbChipDlg::OnBnClickedWritefifoBtn()
{
	int length;
	const UINT8 adr[] = {USB_REG_TXD0, USB_REG_TXD1, USB_REG_TXD2, USB_REG_TXD3};
	CWaitCursor	cursor;
	CFileBin fb;
	CBufferBase pool;

	this->GetUserOption();
	fb.Open(this->m_option.srcfile);
	length = fb.GetFileLength();
	length = (length > this->m_option.buflen) ? this->m_option.buflen : length;
	if(length <= 0)
		return;

	pool.SetSize(length);
	if(pool.GetSize() < length)
		return;
	fb.Read(pool.m_buf, length);
	Usb_WriteFifo(adr[this->m_option.wep], (UINT8 *)pool.m_buf, (UINT8)length);
}

void CUsbChipDlg::OnBnClickedReadfifoBtn()
{
	const UINT8 adr[] = {USB_REG_RXD0, USB_REG_RXD1, USB_REG_RXD2, USB_REG_RXD3};
	CWaitCursor	cursor;
	CFileBin fb;
	CBufferBase pool;

	this->GetUserOption();
	if(this->m_option.buflen <= 0)
		return;
	pool.SetSize(this->m_option.buflen);
	if(pool.GetSize() < this->m_option.buflen)
		return;
	Usb_ReadFifo(adr[this->m_option.rep], (UINT8 *)pool.m_buf, (UINT8)this->m_option.buflen);
	fb.Open(this->m_option.dstfile, "wb");
	fb.Write(pool.m_buf, this->m_option.buflen);
}

void CUsbChipDlg::OnBnClickedAttachnodeCheck()
{
	CWaitCursor	cursor;

	if(this->m_atnbtn.GetCheck())
		//OpenUsb();
		SendCmd("OpenUsb\n");
	else
		CloseUsb();
}

void CUsbChipDlg::OnBnClickedDbugprintBtn()
{
//	UsbStructPrint();
//	SendCmd("UsbStructPrint\n");
}

//#include "hidsdi.h"

void CUsbChipDlg::OnBnClickedUsbchipMiscBtn()
{
//	HidD_GetHidGuid();
}




void CUsbChipDlg::OnBnClickedButtonRead()
{
	CWaitCursor	cursor;
	SYSTEMTIME time, time1;
	int i = 0;
	char str[0x100];
	this->GetUserOption();

	GetSystemTime(&time);
	for(i=0; i<this->m_option.rwcnt; i++)
	{
		gComChnl.Read(str, 0x100);
	}
	GetSystemTime(&time1);
	this->m_option.rwtime = ( ((time1.wHour*60 + time1.wMinute)*60 + time1.wSecond)*1000 + time1.wMilliseconds )
		- ( ((time.wHour*60 + time.wMinute)*60 + time.wSecond)*1000 + time.wMilliseconds );

	this->DispUserOption();
}

void CUsbChipDlg::OnBnClickedButtonWrite()
{
	CWaitCursor	cursor;
	SYSTEMTIME time, time1;
	int i = 0;
	char str[0x100];
	this->GetUserOption();

	memset(str, 0, 0x100);
	GetSystemTime(&time);
	for(i=0; i<this->m_option.rwcnt; i++)
	{
		gComChnl.Write(str, 0x100);
	}
	GetSystemTime(&time1);
	this->m_option.rwtime = ( ((time1.wHour*60 + time1.wMinute)*60 + time1.wSecond)*1000 + time1.wMilliseconds )
		- ( ((time.wHour*60 + time.wMinute)*60 + time.wSecond)*1000 + time.wMilliseconds );

	this->DispUserOption();
}

void CUsbChipDlg::OnBnClickedButtonReadReg()
{
	UINT32 adr = 0;
	UINT32 val;
	int i = 0;
	CWaitCursor	cursor;
	SYSTEMTIME time, time1;

	this->GetUserOption();

	GetSystemTime(&time);
	for(i=0; i<this->m_option.rwcnt; i++)
	{
#ifndef V578CMD_DISABLE
		val = V5_GetReg(adr, BIT_8);
#endif
		adr++;
	}
	GetSystemTime(&time1);
	this->m_option.rwtime = ( ((time1.wHour*60 + time1.wMinute)*60 + time1.wSecond)*1000 + time1.wMilliseconds )
		- ( ((time.wHour*60 + time.wMinute)*60 + time.wSecond)*1000 + time.wMilliseconds );

	this->DispUserOption();

}

void CUsbChipDlg::OnBnClickedButtonWrReg()
{
	UINT32 adr = 1802;
	UINT32 val = 0;
	int i = 0;
	CWaitCursor	cursor;
	SYSTEMTIME time, time1;

	this->GetUserOption();

	GetSystemTime(&time);
	for(i=0; i<this->m_option.rwcnt; i++)
	{
#ifndef V578CMD_DISABLE
		V5_SetReg(adr, val, BIT_8);
#endif
		val++;
	}
	GetSystemTime(&time1);
	this->m_option.rwtime = ( ((time1.wHour*60 + time1.wMinute)*60 + time1.wSecond)*1000 + time1.wMilliseconds )
		- ( ((time.wHour*60 + time.wMinute)*60 + time.wSecond)*1000 + time.wMilliseconds );

	this->DispUserOption();
}

#endif //USBCMD_DISABLE




