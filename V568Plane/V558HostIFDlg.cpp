// V558HostIFDlg.cpp : 实现文件
//
#include "internal.h"
#include "..\include\v568plane\v558hostifdlg.h"


#ifndef	V578CMD_DISABLE



// CV558HostIFDlg 对话框

IMPLEMENT_DYNAMIC(CV558HostIFDlg, CBaseCtrlDlg)
CV558HostIFDlg::CV558HostIFDlg(CWnd* pParent /*=NULL*/)
	: CBaseCtrlDlg(CV558HostIFDlg::IDD, pParent)
{

}

CV558HostIFDlg::~CV558HostIFDlg()
{

}

void CV558HostIFDlg::DoDataExchange(CDataExchange* pDX)
{
	CBaseCtrlDlg::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT_SRAM_ADDR, m_CtrlSramAddr);
	DDX_Control(pDX, IDC_EDIT_SRAM_SIZE, m_CtrlSramSize);
	DDX_Control(pDX, IDC_BYPASSSTARTCOORX_EDIT, m_bypassStartCoorBoxX);
	DDX_Control(pDX, IDC_BYPASSSTARTCOORY_EDIT, m_bypassStartCoorBoxY);
	DDX_Control(pDX, IDC_BYPASSENDCOORX_EDIT, m_bypassEndCoorBoxX);
	DDX_Control(pDX, IDC_BYPASSLINECOLOR_EDIT, m_bypassLineColorBox);
	DDX_Control(pDX, IDC_EDIT_SRAM_TOTAL_TIME, m_sramTotalTimeBox);
	DDX_Control(pDX, IDC_EDIT_BYTE_TIME, m_sramByteTimeBox);
	DDX_Control(pDX, IDC_SRAMWRITE_CHECK, m_sramWriteCheck);
	DDX_Control(pDX, IDC_RADIO_MUL8_HOSTIF, m_buTypeRadioMul8);
	DDX_Control(pDX, IDC_RADIO_MUL16_HOSTIF, m_buTypeRadioMul16);
	DDX_Control(pDX, IDC_RADIO_SEP_HOSTIF, m_buTypeRadioSep);
	DDX_Control(pDX, IDC_NOPORT_CHECK, m_noPortCheck);
	DDX_Control(pDX, IDC_BYPASSMODE_CHECK, m_bypassModeCheck);
    DDX_Control(pDX, IDC_GPIOSELBYPASS_CHECK, m_bypassSelByGPIOCheck);	  
	DDX_Control(pDX, IDC_LCD16_CHECK, m_LCD16Check);
	DDX_Control(pDX, IDC_EDIT_DELAY_ADDR, m_delayAddrBox);
	DDX_Control(pDX, IDC_EDIT_DELAY_DATAIN, m_delayInBox);
	DDX_Control(pDX, IDC_EDIT_DELAY_OE, m_delayOeBox);
	DDX_Control(pDX, IDC_EDIT_DELAY_DATAOUT, m_delayOutBox);
	DDX_Control(pDX, IDC_EDIT_INIT_REGADDR, m_initRegAddrBox);
	DDX_Control(pDX, IDC_EDIT_END_REGADDR, m_endRegAddrBox);
	DDX_Control(pDX, IDC_ALLREGTEST_CHECK, m_allRegTstCheck);
	DDX_Control(pDX, IDC_BYPASSINIT_BUTTON, m_bypassInitBtn);
	DDX_Control(pDX, IDC_BYPASSCSCHANGE_BUTTON, m_bypassCsChangeBtn);	
	DDX_Control(pDX, IDC_BYPASSDRAWLINE_BUTTON, m_bypassDrawLineBtn);
	DDX_Control(pDX, IDC_BYPASSDRAWBITMAP_BUTTON, m_bypassDrawBmpBtn);
	DDX_Control(pDX, IDC_HOSTIFBUSSPEED_EDIT, m_busSpeedBox);	
}


BEGIN_MESSAGE_MAP(CV558HostIFDlg, CBaseCtrlDlg)
	ON_BN_CLICKED(ID_BUTTON_TEST_DELAYSEL, OnBnClickedButtonTestDelaysel)
	ON_BN_CLICKED(IDC_BYPASSDRAWBITMAP_BUTTON, OnBnClickedBypassdrawbmpButton)
	ON_BN_CLICKED(IDC_BYPASSDRAWLINE_BUTTON, OnBnClickedBypassdrawlineButton)
	ON_BN_CLICKED(IDC_RADIO_MUL8_HOSTIF, OnBnClickedRadioMul8Hostif)
	ON_BN_CLICKED(IDC_RADIO_MUL16_HOSTIF, OnBnClickedRadioMul16Hostif)
	ON_BN_CLICKED(IDC_RADIO_SEP_HOSTIF, OnBnClickedRadioSepHostif)
	ON_BN_CLICKED(IDC_BACKWRITE_BUTTON, OnBnClickedBackwriteButton)
	ON_BN_CLICKED(IDC_BYPASSMODE_CHECK, OnBnClickedBypassmodeCheck)
	ON_BN_CLICKED(IDC_NOPORT_CHECK, OnBnClickedNoportCheck)
	ON_BN_CLICKED(IDC_SRAMWRITE_CHECK, OnBnClickedSramwriteCheck)
	ON_BN_CLICKED(IDC_BYPASSINIT_BUTTON, OnBnClickedBypassinitButton)
	ON_BN_CLICKED(IDC_BYPASSCSCHANGE_BUTTON, OnBnClickedBypasscschangeButton)
	ON_BN_CLICKED(ID_BUTTON_TEST_REGCONREAD, OnBnClickedButtonTestRegconread)
	ON_BN_CLICKED(IDC_ALLREGTEST_CHECK, OnBnClickedAllregtestCheck)
	ON_BN_CLICKED(IDC_GPIOSELBYPASS_CHECK, OnBnClickedGpioselbypassCheck)
	ON_BN_CLICKED(ID_HOSTIFREFRESH_BUTTON, OnBnClickedHostifrefreshButton)
	ON_BN_CLICKED(ID_HOSTIFBUSSPEEDTEST_BUTTON, OnBnClickedHostifbusspeedtestButton)
END_MESSAGE_MAP()


// CV558HostIFDlg 消息处理程序


UINT8 HostTestSramRwTime(UINT32 adr, UINT32 size, UINT8 w)
{
	char str[0x100];

	sprintf(str, "testSramRwTime %lx %lx, %x\n", adr, size, w);
	size = SendRetCmd(str);
	return (unsigned char)size;
}

void CV558HostIFDlg::InitUserOption(void)
{
    memset( &(this->m_option), 0, sizeof(THostIFOption) );

	this->m_option.sramAddr = 0x0;
	this->m_option.sramSize = 0x60000;
	this->m_option.bPortAcess = 1;
	this->m_option.bypassEndCoorX = 0x7f;
	this->m_option.bypassLineColor = 0xf800;
	this->m_option.busSpeed = 0x0;
}

void CV558HostIFDlg::DispUserOption(void)
{
	char str[20];

	/*sram w/r*/
	sprintf(str, "%x", this->m_option.sramAddr);
	this->m_CtrlSramAddr.SetWindowText(str);

	sprintf(str, "%x", this->m_option.sramSize);
	this->m_CtrlSramSize.SetWindowText(str);

	sprintf(str, "%x", this->m_option.totalTime);
	this->m_sramTotalTimeBox.SetWindowText(str);

	sprintf(str, "%x", this->m_option.byteTime);
	this->m_sramByteTimeBox.SetWindowText(str);

	/*bus type select*/
	if(this->m_option.busTypeSel == 0)
	{
		this->m_buTypeRadioMul8.SetCheck(BST_CHECKED);
		this->m_buTypeRadioMul16.SetCheck(BST_UNCHECKED);
		this->m_buTypeRadioSep.SetCheck(BST_UNCHECKED);
	}
	else if(this->m_option.busTypeSel == 1)
	{
		this->m_buTypeRadioMul8.SetCheck(BST_UNCHECKED);
		this->m_buTypeRadioMul16.SetCheck(BST_CHECKED);
		this->m_buTypeRadioSep.SetCheck(BST_UNCHECKED);
	}
	else
	{
		this->m_buTypeRadioMul8.SetCheck(BST_UNCHECKED);
		this->m_buTypeRadioMul16.SetCheck(BST_UNCHECKED);
		this->m_buTypeRadioSep.SetCheck(BST_CHECKED);
	}


	/*bypass*/
	sprintf(str, "%x", this->m_option.bypassStartCoorX);
	this->m_bypassStartCoorBoxX.SetWindowText(str);

	sprintf(str, "%x", this->m_option.bypassStartCoorY);
	this->m_bypassStartCoorBoxY.SetWindowText(str);

	sprintf(str, "%x", this->m_option.bypassEndCoorX );
	this->m_bypassEndCoorBoxX.SetWindowText(str);

	sprintf(str, "%x", this->m_option.bypassLineColor);
	this->m_bypassLineColorBox.SetWindowText(str);


	if(this->m_option.bypassSelected)
	{
		this->m_bypassInitBtn.EnableWindow();
		this->m_bypassCsChangeBtn.EnableWindow();
	    this->m_bypassDrawLineBtn.EnableWindow();
		this->m_bypassDrawBmpBtn.EnableWindow();
     
		this->m_bypassStartCoorBoxX.EnableWindow();
		this->m_bypassStartCoorBoxY.EnableWindow();
	    this->m_bypassEndCoorBoxX.EnableWindow();
	    this->m_bypassLineColorBox.EnableWindow();

	}
	else
	{
		this->m_bypassInitBtn.EnableWindow(FALSE);
		this->m_bypassCsChangeBtn.EnableWindow(FALSE);
	    this->m_bypassDrawLineBtn.EnableWindow(FALSE);
	    this->m_bypassDrawBmpBtn.EnableWindow(FALSE);

		this->m_bypassStartCoorBoxX.EnableWindow(FALSE);
		this->m_bypassStartCoorBoxY.EnableWindow(FALSE);
	    this->m_bypassEndCoorBoxX.EnableWindow(FALSE);
	    this->m_bypassLineColorBox.EnableWindow(FALSE);
	}
   

	/*delay select*/
	sprintf(str, "%x", this->m_option.delayAddr);
	this->m_delayAddrBox.SetWindowText(str);

	sprintf(str, "%x", this->m_option.delayDatain);
	this->m_delayInBox.SetWindowText(str);

	sprintf(str, "%x", this->m_option.delayDataout);
	this->m_delayOutBox.SetWindowText(str);

	sprintf(str, "%x", this->m_option.delayOe);
	this->m_delayOeBox.SetWindowText(str);

	/*Continous read register*/
	sprintf(str, "%x", this->m_option.initRegAddr);
	this->m_initRegAddrBox.SetWindowText(str);

	sprintf(str, "%x", this->m_option.endRegAddr);
	this->m_endRegAddrBox.SetWindowText(str);

	if(this->m_option.allRegTst)
	{
		this->m_allRegTstCheck.SetCheck(BST_CHECKED);
		this->m_initRegAddrBox.EnableWindow(FALSE);
		this->m_endRegAddrBox.EnableWindow(FALSE);
	}
	else
	{
		this->m_allRegTstCheck.SetCheck(BST_UNCHECKED);
		this->m_initRegAddrBox.EnableWindow(TRUE);
		this->m_endRegAddrBox.EnableWindow(TRUE);
	}

	/*bus speed*/
	sprintf(str, "%x", this->m_option.busSpeed);
	this->m_busSpeedBox.SetWindowText(str);	
}

BOOL CV558HostIFDlg::OnInitDialog()
{
	CBaseCtrlDlg::OnInitDialog();

	this->InitUserOption( );
	this->DispUserOption( );

	return TRUE; 
}


void CV558HostIFDlg::GetEditBoxValue(void)
{
	char str[20];
	int x;

	/*sram w/r*/
	this->m_CtrlSramAddr.GetWindowText(str, 20);
	sscanf(str, "%x", &x);
	this->m_option.sramAddr = x;

	this->m_CtrlSramSize.GetWindowText(str, 20);
	sscanf(str, "%x", &x);
	this->m_option.sramSize = x;


	/*bypass*/
	/*get the start point x coordinate of line or bmp on panel*/
	this->m_bypassStartCoorBoxX.GetWindowText(str, 20);
	sscanf(str, "%x", &x);
	this->m_option.bypassStartCoorX = (unsigned char)x;

	/*get the start point y coordinate of line or bmp on panel*/
	this->m_bypassStartCoorBoxY.GetWindowText(str, 20);
	sscanf(str, "%x", &x);
	this->m_option.bypassStartCoorY = (unsigned char)x;
 
	/*get the end point x coordinate of line or bmp on panel*/
	this->m_bypassEndCoorBoxX.GetWindowText(str, 20);
	sscanf(str, "%x", &x);
	this->m_option.bypassEndCoorX = (unsigned char)x;

	/*get line color*/
	this->m_bypassLineColorBox.GetWindowText(str, 20);
	sscanf(str, "%x", &x);
	this->m_option.bypassLineColor = (unsigned short)x;


	/*delay select*/
	this->m_delayAddrBox.GetWindowText(str, 20);
	sscanf(str, "%x", &x);
	this->m_option.delayAddr = x;

	this->m_delayInBox.GetWindowText(str, 20);
	sscanf(str, "%x", &x);
	this->m_option.delayDatain = x;

	this->m_delayOutBox.GetWindowText(str, 20);
	sscanf(str, "%x", &x);
	this->m_option.delayDataout = x;

	this->m_delayOeBox.GetWindowText(str, 20);
	sscanf(str, "%x", &x);
	this->m_option.delayOe = x;

	/*Continous read register*/
	this->m_option.allRegTst = this->m_allRegTstCheck.GetCheck();

	this->m_initRegAddrBox.GetWindowText(str, 20);
	sscanf(str, "%x", &x);
	this->m_option.initRegAddr = (unsigned int)x;

	this->m_endRegAddrBox.GetWindowText(str, 20);
	sscanf(str, "%x", &x);
	this->m_option.endRegAddr = (unsigned int)x;
}

void CV558HostIFDlg::GetComboValue( )
{
	CBasePanelDlg::GetFileBoxOption(this->m_option.srcfile, this->m_option.dstfile);
}

void CV558HostIFDlg::GetCheckAndRadioBtnValue( )
{
    /*sram w/r*/
	this->m_option.bSramTimeW = (char)m_sramWriteCheck.GetCheck( );	 

	/*access type*/
	if( m_noPortCheck.GetCheck( ) )
	{
		this->m_option.bPortAcess = 0; 
	}
	else
	{
		this->m_option.bPortAcess = 1; 
	}

	/*bypass*/
	this->m_option.bypassSelected = (char)m_bypassModeCheck.GetCheck( );
	this->m_option.bypassSelByGPIO = (char)this->m_bypassSelByGPIOCheck.GetCheck( ); 
	this->m_option.lcdType = (char)m_LCD16Check.GetCheck( );
}

void CV558HostIFDlg::GetUserOption( )
{
	this->GetEditBoxValue( );
	this->GetComboValue( );
	this->GetCheckAndRadioBtnValue( );
}

#define _CHECK_TIME 0x100
void CV558HostIFDlg::OnBnClickedHostifbusspeedtestButton()
{
	UINT32 msec0, msec1, msec2, msec3, msec4;
	SYSTEMTIME Time0, Time1, Time2;
	char wt;
	int i = 0;

	wt = this->m_option.bSramTimeW > 0 ? 1 : 0;

	this->GetUserOption( );

	if(this->m_option.sramSize == 0)
	{
		::AfxMessageBox("Sram size is zero!");
		return;
	}

	GetSystemTime(&Time0);
	for(i=0; i<_CHECK_TIME; i++)
	{
		if(HostTestSramRwTime( (UINT32)this->m_option.sramAddr, 0, wt) != 1)
		{
			::AfxMessageBox("error for test no size r/w!");
			return;
		}
	}

	GetSystemTime(&Time1);
	for(i=0; i<_CHECK_TIME; i++)
	{
		if(HostTestSramRwTime( (UINT32)this->m_option.sramAddr, (UINT32)this->m_option.sramSize, wt) != 0)
		{
			::AfxMessageBox("error for test size r/w!");
			return;
		}
	}
	GetSystemTime(&Time2);

	msec0 = ((Time0.wHour*60 + Time0.wMinute)*60 + Time0.wSecond)*1000 + Time0.wMilliseconds;
	msec1 = ((Time1.wHour*60 + Time1.wMinute)*60 + Time1.wSecond)*1000 + Time1.wMilliseconds;
	msec2 = ((Time2.wHour*60 + Time2.wMinute)*60 + Time2.wSecond)*1000 + Time2.wMilliseconds;

	msec3 = ((msec2 - msec1) - (msec1 - msec0))/_CHECK_TIME;
	msec4 = (msec3*1000000)/( (UINT32)this->m_option.sramSize );
	
	this->m_option.totalTime = msec3;
	this->m_option.byteTime = msec4;
	if(msec3 == 0)
		return;
	this->m_option.busSpeed = ( (UINT32)this->m_option.sramSize )/msec3;

	this->DispUserOption();
}

void CV558HostIFDlg::OnBnClickedNoportCheck()
{
	if(this->m_option.bPortAcess)
	{
        V5_PortSwitchTest(0x1);
	}
	else
	{
        V5_PortSwitchTest(0x0);
	}
}

void CV558HostIFDlg::OnBnClickedButtonTestDelaysel()
{
	this->GetUserOption( );

	V5B_BiuSetAddrDelaySel((unsigned char)this->m_option.delayAddr);
	V5B_BiuSetDataOutDelaySel((unsigned char)this->m_option.delayDataout);
	V5B_BiuSetDataInDelaySel((unsigned char)this->m_option.delayDatain);
	V5B_BiuSetDataOenDelaySel((unsigned char)this->m_option.delayOe);

}

static void SetArmPortE0ForByGPIOSwitch(UINT8 bypassSel)
{
	int  val;

    /*Set ARM port E0 as output*/
	val =  ReadMemory(REG_PCONE);
	val &= ~(0x3);
	val |= 0x1;
    WriteMemory(REG_PCONE, val);

	/*Set ARM port E0 to 0/1 according to bypassSel*/
	val =  ReadMemory(REG_PDATE);
	if(bypassSel)
		val |= 0x1;
	else
	    val &= ~(0x1);
    WriteMemory(REG_PDATE, val);
}

void CV558HostIFDlg::OnBnClickedBypassmodeCheck()
{
	this->GetUserOption( );

	if(this->m_bypassModeCheck.GetCheck())
	{
		this->m_option.bypassSelected = 1;
		V5_CtrlNormalToBypass();
	}
	else
	{
		this->m_option.bypassSelected = 0;
		V5_CtrlBypassToNormal();
	}
	this->DispUserOption();
}

void CV558HostIFDlg::OnBnClickedGpioselbypassCheck()
{
	if(this->m_bypassSelByGPIOCheck.GetCheck())
	{
		this->m_option.bypassSelByGPIO = 1;
		SetArmPortE0ForByGPIOSwitch(0);
		V5B_BiuSetBypassSelType(BY_GPIO);
	}
	else
	{
		this->m_option.bypassSelByGPIO = 0;
		V5B_BiuSetBypassSelType(BY_REG);
	}
}

void CV558HostIFDlg::OnBnClickedBypassinitButton()
{
	PanelDbgInitContext();
	PanelDbgSetMode(BYPASS_MODE);
}

void CV558HostIFDlg::OnBnClickedBypassdrawlineButton()
{
    CWaitCursor cursor;
    TPoint pt;
	UINT16 lineLen;

	this->GetUserOption( );

	pt.x = this->m_option.bypassStartCoorX;
	pt.y = this->m_option.bypassStartCoorY;
	lineLen = this->m_option.bypassEndCoorX - this->m_option.bypassStartCoorX + 1;

	PanelWriteGram(pt, this->m_option.bypassLineColor, lineLen);
}

void CV558HostIFDlg::OnBnClickedBypassdrawbmpButton()
{
	CWaitCursor cursor;
	CFileBin fb;
	CBufferBase pool;
	TBmpInfo info;
	int size = 0;


	this->GetUserOption();

	fb.Open( (char*)( this->m_option.srcfile ) );
	size = fb.GetFileLength();
	if(size == 0)
	{
		return;
	}

	pool.SetSize(size);
	if( pool.GetSize() < size )
	{
		return;
	}

	fb.Read(pool.m_buf, size);

	if( ParseBmpData( (UINT8 *)pool.m_buf, &info ) )
	{
		return;
	}

	PanelWriteImage( (unsigned short*)(pool.m_buf + 0x42), (size - 0x42) >> 1 );
}

void CV558HostIFDlg::OnBnClickedRadioMul8Hostif()
{
	CWaitCursor cursor;
	char str[0x100];

	sprintf(str, "V5B_SetBusType %x\n", V5_BUS_MULTI8);
	SendCmd(str);

	this->m_option.busTypeSel = 0;
	this->DispUserOption();
}

void CV558HostIFDlg::OnBnClickedRadioMul16Hostif()
{
	CWaitCursor cursor;
	char str[0x100];

	sprintf(str, "V5B_SetBusType %x\n", V5_BUS_MULTI16);
	SendCmd(str);

	this->m_option.busTypeSel = 1;
	this->DispUserOption();
}

void CV558HostIFDlg::OnBnClickedRadioSepHostif()
{
	CWaitCursor cursor;
	char str[0x100];

	sprintf(str, "V5B_SetBusType %x\n", V5_BUS_SEPERATE);
	SendCmd(str);

	this->m_option.busTypeSel = 2;
	this->DispUserOption();
}

void CV558HostIFDlg::OnBnClickedBackwriteButton()
{
	static unsigned char i = 0;
	char str[0x100];
	unsigned long adr, size;

	size = 0x50000;
	
	if(i)
	{
		i = 0;
		adr = 0;
	}
	else
	{
		i = 1;
		adr = 8;
	}

	sprintf(str, "testSramWriteBackRw %lx %lx\n", adr, size);
	SendCmd(str);
}

void CV558HostIFDlg::OnBnClickedSramwriteCheck()
{
	/*access type*/
	this->m_option.bSramTimeW = (char)m_sramWriteCheck.GetCheck( );	 
}

void CV558HostIFDlg::OnBnClickedBypasscschangeButton()
{
	static char panelSel = 0x0;

	if(panelSel == 0x0)
	{
		panelSel = 0x1;
	}
	else
	{
		panelSel = 0x0;
	}

	V5_BySetPanelSel(panelSel);
}

void CV558HostIFDlg::OnBnClickedButtonTestRegconread()
{
	unsigned int i;

    this->GetUserOption();

	if(this->m_option.allRegTst)
	{
		for(i = BIU_REG_INIT_ADDR; i <= BIU_REG_END_ADDR; i++ )
			testRegConRead(i, 0x10000);

		for(i = GE_REG_INIT_ADDR; i <= GE_REG_END_ADDR; i++ )
			testRegConRead(i, 0x10000);

		for(i = SIF_REG_INIT_ADDR; i <= SIF_REG_END_ADDR; i++ )
			testRegConRead(i, 0x10000);

		for(i = CTRL_REG_INIT_ADDR; i <= CTRL_REG_END_ADDR; i++ )
			testRegConRead(i, 0x10000);

		for(i = ISP_REG_INIT_ADDR; i <= ISP_REG_END_ADDR; i++ )
			testRegConRead(i, 0x10000);

		for(i = IPP_REG_INIT_ADDR; i <= IPP_REG_END_ADDR; i++ )
			testRegConRead(i, 0x10000);

		for(i = LCDIF_REG_INIT_ADDR; i <= LCDIF_REG_END_ADDR; i++ )
			testRegConRead(i, 0x10000);

		for(i = JPEG_REG_INIT_ADDR; i <= JPEG_REG_END_ADDR; i++ )
			testRegConRead(i, 0x10000);

		for(i = LBUF_REG_INIT_ADDR; i <= LBUF_REG_END_ADDR; i++ )
			testRegConRead(i, 0x10000);

		for(i = LCDC_REG_INIT_ADDR; i <= LCDC_REG_END_ADDR; i++ )
			testRegConRead(i, 0x10000);

		for(i = MARB_REG_INIT_ADDR; i <= MARB_REG_END_ADDR; i++ )
			testRegConRead(i, 0x10000);
	}
	else
	{
	    for(i = this->m_option.initRegAddr; i <= this->m_option.endRegAddr; i++ )
	        testRegConRead(i, 0x10000);
	}
}

void CV558HostIFDlg::OnBnClickedAllregtestCheck()
{
    this->m_option.allRegTst = this->m_allRegTstCheck.GetCheck();

	if(this->m_option.allRegTst)
	{
		this->m_initRegAddrBox.EnableWindow(FALSE);
		this->m_endRegAddrBox.EnableWindow(FALSE);
	}
	else
	{
		this->m_initRegAddrBox.EnableWindow(TRUE);
		this->m_endRegAddrBox.EnableWindow(TRUE);
	}
}

void CV558HostIFDlg::OnBnClickedHostifrefreshButton()
{
	this->DispUserOption();
}

#endif //V578CMD_DISABLE

