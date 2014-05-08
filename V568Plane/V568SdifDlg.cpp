// V568SdifDlg.cpp : 实现文件
//
#include "internal.h"



#ifndef	V868CMD_DISABLE



// CV568SdifDlg 对话框

IMPLEMENT_DYNAMIC(CV568SdifDlg, CBaseCtrlDlg)
CV568SdifDlg::CV568SdifDlg(CWnd* pParent /*=NULL*/)
	: CBaseCtrlDlg(CV568SdifDlg::IDD, pParent)
{
}

CV568SdifDlg::~CV568SdifDlg()
{
}

void CV568SdifDlg::DoDataExchange(CDataExchange* pDX)
{
	CBaseCtrlDlg::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_SDCMD_CHECK, m_cmdchkbtn);
	DDX_Control(pDX, IDC_SDCMDINDEX_CHECK, m_cmdidxchkbtn);
	DDX_Control(pDX, IDC_SDDATADIREC_CHECK, m_datdircbtn);
	DDX_Control(pDX, IDC_DATAPRES_CHECK, m_datprebtn);
	DDX_Control(pDX, IDC_AUTOCMD12_CHECK, m_cmd12enbtn);
	DDX_Control(pDX, IDC_CMDINDEX_EDIT, m_cmdidxbox);
	DDX_Control(pDX, IDC_RESP_COMBO, m_respbox);
	DDX_Control(pDX, IDC_WRITESB_BUTTON, m_wrsbbtn);
	DDX_Control(pDX, IDC_WRITEMB_BUTTON, m_wrmbbtn);
	DDX_Control(pDX, IDC_READSB_BUTTON, m_rdsbbtn);
	DDX_Control(pDX, IDC_READMB_BUTTON, m_rdmbbtn);
	DDX_Control(pDX, IDC_STOMODDMA_CHECK, m_moddmabtn);
	DDX_Control(pDX, IDC_STOMODSPI_CHECK, m_modspibtn);
	DDX_Control(pDX, IDC_STOMODTRANSWIDTH_CHECK, m_modtranswidbtn);
	DDX_Control(pDX, IDC_CMDARGU_EDIT, m_cmdargubox);
	DDX_Control(pDX, IDC_CMDRESP_EDIT, m_cmdrspbox);
	DDX_Control(pDX, IDC_BLKCNT_EDIT, m_blkcntbox);
	DDX_Control(pDX, IDC_BLKSIZE_EDIT, m_blksizebox);
	DDX_Control(pDX, IDC_TRANSADDR_EDIT, m_transaddrbox);
}


BEGIN_MESSAGE_MAP(CV568SdifDlg, CBaseCtrlDlg)
	ON_BN_CLICKED(IDC_SDOPEN_BUTTON, OnBnClickedSdopenButton)
	ON_BN_CLICKED(IDC_REFRESH_BUTTON, OnBnClickedRefreshButton)
	ON_BN_CLICKED(IDC_CMDINDEX_BUTTON, OnBnClickedCmdindexButton)
	ON_BN_CLICKED(IDC_SDCMD_CHECK, OnBnClickedSdcmdCheck)
	ON_BN_CLICKED(IDC_SDCMDINDEX_CHECK, OnBnClickedSdcmdindexCheck)
	ON_BN_CLICKED(IDC_SDDATADIREC_CHECK, OnBnClickedSddatadirecCheck)
	ON_BN_CLICKED(IDC_DATAPRES_CHECK, OnBnClickedDatapresCheck)
	ON_CBN_SELCHANGE(IDC_RESP_COMBO, OnCbnSelchangeRespCombo)
	ON_BN_CLICKED(IDC_AUTOCMD12_CHECK, OnBnClickedAutocmd12Check)
	ON_BN_CLICKED(IDC_STOMODDMA_CHECK, OnBnClickedStomoddmaCheck)
	ON_BN_CLICKED(IDC_STOMODSPI_CHECK, OnBnClickedStomodspiCheck)
	ON_BN_CLICKED(IDC_STOMODTRANSWIDTH_CHECK, OnBnClickedStomodtranswidthCheck)
	ON_BN_CLICKED(IDC_ACMDSEND_BUTTON2, OnBnClickedAcmdsendButton2)
	ON_BN_CLICKED(IDC_WRITESB_BUTTON, OnBnClickedWritesbButton)
	ON_BN_CLICKED(IDC_WRITEMB_BUTTON, OnBnClickedWritembButton)
	ON_BN_CLICKED(IDC_READSB_BUTTON, OnBnClickedReadsbButton)
	ON_BN_CLICKED(IDC_READMB_BUTTON, OnBnClickedReadmbButton)
END_MESSAGE_MAP()


// CV568SdifDlg 消息处理程序

BOOL CV568SdifDlg::OnInitDialog()
{
	CBaseCtrlDlg::OnInitDialog();

	this->InitUserOption();
	this->DispUserOption();

	return TRUE;
}

void CV568SdifDlg::InitUserOption(void)
{
	int i;
	const char *resp[] = 
	{
		"SD_NORESP",
		"SD_RESP136",
		"SD_RESP48",
		"SD_RESP48BUSY"
	};

	memset( &(this->m_option), 0, sizeof(TSdcOption) );

	for(i = 0; i < sizeof(resp) / sizeof(char *); i++)
		this->m_respbox.InsertString(i, resp[i]);
	this->m_respbox.SetCurSel(0);

	/*set crc and index checkBox default value is 1*/
	this->m_option.cmdl |= SD_CHKCRCEN;
    this->m_option.cmdl |= SD_CHKINXEN;
}

void CV568SdifDlg::DispUserOption(void)
{
	int i, j;
	char str[30] = {0};
	

	i = this->m_option.cmdh;
	sprintf(str, "%x", i);
	this->m_cmdidxbox.SetWindowText(str);

	sprintf(str, "%x", this->m_option.blkCnt );
	this->m_blkcntbox.SetWindowText(str);

	sprintf(str, "%x", this->m_option.blkSize);
	this->m_blksizebox.SetWindowText(str);

	sprintf(str, "%x", this->m_option.transaddr);
	this->m_transaddrbox.SetWindowText(str);

	this->m_cmdargubox.SetWindowText("0");
	this->m_cmdrspbox.SetWindowText("");

	i = this->m_option.cmdl & (SD_CHKCRCEN - 1);
	this->m_respbox.SetCurSel(i);

	if(this->m_option.cmdl & SD_CHKCRCEN)
		this->m_cmdchkbtn.SetCheck(BST_CHECKED);
	else
		this->m_cmdchkbtn.SetCheck(BST_UNCHECKED);

	if(this->m_option.cmdl & SD_CHKINXEN)
		this->m_cmdidxchkbtn.SetCheck(BST_CHECKED);
	else
		this->m_cmdidxchkbtn.SetCheck(BST_UNCHECKED);

	if(this->m_option.cmdl & SD_RDDIRC)
	{
		this->m_datdircbtn.SetCheck(BST_CHECKED);
	}
	else
	{
		this->m_datdircbtn.SetCheck(BST_UNCHECKED);
	}

	if(this->m_option.cmdl & SD_DATAPRENT)
		this->m_datprebtn.SetCheck(BST_CHECKED);
	else
		this->m_datprebtn.SetCheck(BST_UNCHECKED);


	/*get response*/
    for(i = 0; i < 4; i++)
	{
		for(j = 0; j < 4; j++)
		{
			sprintf( str + i*5 +j, "%x ", this->m_option.rsp[i] );
		}
        
		*( str + 5 * (i +1) ) =' '; 
	}

	this->m_cmdrspbox.SetWindowText(str);

	this->m_respbox.EnableWindow(FALSE);
	this->m_datdircbtn.EnableWindow(FALSE);
	this->m_datprebtn.EnableWindow(FALSE);
	this->m_cmd12enbtn.EnableWindow(FALSE);
}

void CV568SdifDlg::GetUserOption(char *tail)
{
	this->GetComboValue(tail);
	this->GetEditBoxValue();
}

void CV568SdifDlg::GetComboValue(char *tail)
{
	CBasePanelDlg::GetFileBoxOption(this->m_option.srcfile, this->m_option.dstfile, tail);
}

void CV568SdifDlg::GetEditBoxValue(void)
{
	char str[20];
	int x;

	/*get command index*/
	this->m_cmdidxbox.GetWindowText(str, 20);
	sscanf(str, "%d", &x);
	this->m_option.cmdh = (char)x;
 
	/*get command argumnet*/
	this->m_cmdargubox.GetWindowText(str, 20);
	sscanf(str, "%x", &x);
	this->m_option.cmdargu = x;

	/*get block count*/
	this->m_blkcntbox.GetWindowText(str, 20);
	sscanf(str, "%d", &x);
	this->m_option.blkCnt = (short)x;

	/*get block size*/
	this->m_blksizebox.GetWindowText(str, 20);
	sscanf(str, "%d", &x);
	this->m_option.blkSize = (short)x;

	/*get trans address*/
	this->m_transaddrbox.GetWindowText(str, 20);
	sscanf(str, "%d", &x);
	this->m_option.transaddr = x;
	
}

void CV568SdifDlg::OnBnClickedSdopenButton()
{
	CWaitCursor cursor;

	V8_SdcOpen( 1 );
}

void CV568SdifDlg::OnBnClickedRefreshButton()
{
	CWaitCursor cursor;

	this->m_option.cmdh = V8B_SdifGetCmdCtrl0();
	this->m_option.cmdl = V8B_SdifGetCmdCtrl1();

	this->DispUserOption();
}

void CV568SdifDlg::OnBnClickedCmdindexButton()
{
	CWaitCursor cursor;

	this->GetUserOption();
	V8_SdcSendCmd( (UINT8)( this->m_option.cmdh ), (UINT32)( this->m_option.cmdargu ) );

	/*show response register content*/
	this->DispUserOption( );
}

void CV568SdifDlg::OnBnClickedSdcmdCheck()
{
	CWaitCursor cursor;

	if(this->m_cmdchkbtn.GetCheck())
	{
		this->m_option.cmdl |= SD_CHKCRCEN;
		V8B_SdifEnableCrcChk( );
	}
	else
	{
		this->m_option.cmdl &= ~SD_CHKCRCEN;
		V8B_SdifDisableCrcChk( );
	}
}

void CV568SdifDlg::OnBnClickedSdcmdindexCheck()
{
	CWaitCursor cursor;

	if(this->m_cmdidxchkbtn.GetCheck())
	{
		this->m_option.cmdl |= SD_CHKINXEN;
	}
	else
	{
		this->m_option.cmdl &= ~SD_CHKINXEN;
	}

}

void CV568SdifDlg::OnBnClickedSddatadirecCheck()
{
	CWaitCursor cursor;

	if(this->m_datdircbtn.GetCheck())
	{
		this->m_option.cmdl |= SD_RDDIRC;
	}
	else
	{
		this->m_option.cmdl &= ~SD_RDDIRC;
	}
}

void CV568SdifDlg::OnBnClickedDatapresCheck()
{
	CWaitCursor cursor;

	if(this->m_datprebtn.GetCheck())
	{
		this->m_option.cmdl |= SD_DATAPRENT;
	}
	else
	{
		this->m_option.cmdl &= ~SD_DATAPRENT;
	}
}

void CV568SdifDlg::OnCbnSelchangeRespCombo()
{
	int x;

	CWaitCursor cursor;

	x = this->m_respbox.GetCurSel();

	this->m_option.cmdl &= 0xfc;
	this->m_option.cmdl |= (UINT8)x;
}

void CV568SdifDlg::OnBnClickedAutocmd12Check()
{
	CWaitCursor cursor;

	if(this->m_cmd12enbtn.GetCheck())
	{
		this->m_option.cmdl |= SD_AUTOCMD12EN;
	}
	else
	{
		this->m_option.cmdl &= ~SD_AUTOCMD12EN;
	}

}

void CV568SdifDlg::OnBnClickedStomoddmaCheck()
{
    CWaitCursor cursor;

	if(this->m_moddmabtn.GetCheck())
	{
		this->m_option.stoMode |= SD_DMAMODE;
        V8B_SdifEnableDmaMode( );
	}
	else
	{
		this->m_option.stoMode &= ~SD_DMAMODE;
		V8B_SdifDisableDmaMode( );
	}

}

void CV568SdifDlg::OnBnClickedStomodspiCheck()
{
	CWaitCursor cursor;

	if(this->m_modspibtn.GetCheck())
	{
		this->m_option.stoMode |= SD_SPIEN;
		V8B_SdifEnableSpi( );
	}
	else
	{
		this->m_option.stoMode &= ~SD_SPIEN;
		V8B_SdifDisableSpi( );
	}

}

void CV568SdifDlg::OnBnClickedStomodtranswidthCheck()
{
    CWaitCursor cursor;

	if(this->m_modtranswidbtn.GetCheck())
	{
		this->m_option.stoMode|= SD_TRANS4;
		V8B_SdifEnableTrans4( );
	}
	else
	{
		this->m_option.stoMode &= ~SD_TRANS4;
		V8B_SdifDisableTrans4( );
	}

}

void CV568SdifDlg::OnBnClickedAcmdsendButton2()
{
	CWaitCursor   cursor;

	this->GetUserOption();
	V8_SdcSendACmd( (UINT8)( this->m_option.cmdh ), 
		            (UINT32)( this->m_option.cmdargu ) );

	/*show response register content*/
	V8B_SdifGetResp( (UINT8*)this->m_option.rsp );
	this->DispUserOption( );
}

void CV568SdifDlg::OnBnClickedWritesbButton()
{
	CWaitCursor	cursor;
	CBufferBase pool;
	short i = 0;
	int temp = 0;
	FILE *fp = NULL;


	pool.SetSize(this->m_option.blkSize);
	if(pool.GetSize() < this->m_option.blkSize)
	{	
		return;
	}

    this->GetUserOption();
	fp = fopen(this->m_option.srcfile, "r+");

	for(i = 0; i < this->m_option.blkSize; i++)
	{
	   temp = fgetc(fp);
	   pool.m_buf[i] = (char)temp - '0';
	}

	fclose(fp);

	V8_SdcHostWriteData( (UINT32)this->m_option.transaddr, (UINT8*)pool.m_buf, this->m_option.blkSize );
}

void CV568SdifDlg::OnBnClickedWritembButton()
{
	CWaitCursor	cursor;
	CBufferBase pool;
	short i = 0;
	int temp = 0;
	FILE *fp = NULL;	

	pool.SetSize( this->m_option.blkCnt * this->m_option.blkSize );
	if( pool.GetSize() < this->m_option.blkCnt * this->m_option.blkSize )
	{	
		return;
	}

	this->GetUserOption( );
	fp = fopen(this->m_option.srcfile, "r+");

	for(i = 0; i < this->m_option.blkCnt * this->m_option.blkSize; i++)
	{
	   temp = fgetc(fp);
	   pool.m_buf[i] = (char)temp - '0';
	}

	fclose(fp);

	V8_SdcHostWriteData( (UINT32)this->m_option.transaddr, (UINT8*)pool.m_buf, this->m_option.blkCnt * this->m_option.blkSize );
}

void CV568SdifDlg::OnBnClickedReadsbButton()
{
	CWaitCursor	cursor;
	CBufferBase pool;
	short i = 0;
	FILE *fp = NULL;

	pool.SetSize(this->m_option.blkSize);
	if(pool.GetSize() < this->m_option.blkSize)
	{	
		return;
	}

    this->GetUserOption();
	V8_SdcHostReadData( (UINT32)this->m_option.transaddr, (UINT8*)pool.m_buf, this->m_option.blkSize );

	fp = fopen(this->m_option.dstfile, "w+");
	for(i = 0; i < this->m_option.blkSize; i++)
	{
	   fprintf(fp, "%x", pool.m_buf[i]);
	}

	fprintf(fp, "\n");

	fclose(fp);
}

void CV568SdifDlg::OnBnClickedReadmbButton()
{
    CWaitCursor	cursor;
	CBufferBase pool;
	short i = 0;
	FILE *fp = NULL;	

	pool.SetSize( this->m_option.blkCnt * this->m_option.blkSize );
	if( pool.GetSize() < this->m_option.blkCnt * this->m_option.blkSize )
	{	
		return;
	}

	this->GetUserOption( );
    V8_SdcHostReadData( (UINT32)this->m_option.transaddr, (UINT8*)pool.m_buf, this->m_option.blkCnt * this->m_option.blkSize );

	fp = fopen(this->m_option.dstfile, "w+");
	for(i = 0; i < this->m_option.blkCnt * this->m_option.blkSize; i++)
	{
       fprintf(fp, "%x", pool.m_buf[i]);
	}

	fprintf(fp, "\n");

	fclose(fp);

}

#endif //V868CMD_DISABLE
