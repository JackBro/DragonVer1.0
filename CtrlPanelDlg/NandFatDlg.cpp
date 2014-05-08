// V568SensorDlg.cpp : 实现文件
//

#include "internal.h"



#ifndef	HOSTCMD_DISABLE

CEmuHd	gHardDisk;

// CNandFatDlg 对话框

IMPLEMENT_DYNAMIC(CNandFatDlg, CBaseCtrlDlg)
CNandFatDlg::CNandFatDlg(CWnd* pParent /*=NULL*/)
	: CBaseCtrlDlg(CNandFatDlg::IDD, pParent)
{
}

CNandFatDlg::~CNandFatDlg()
{
}

void CNandFatDlg::DoDataExchange(CDataExchange* pDX)
{
	CBaseCtrlDlg::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_MAKERID_EDIT	, m_makerid	);
	DDX_Control(pDX, IDC_DEVICEID_EDIT	, m_devid	);
	DDX_Control(pDX, IDC_BLOCK_EDIT		, m_ndblock	);
	DDX_Control(pDX, IDC_PAGE_EDIT		, m_ndpage	);
	DDX_Control(pDX, IDC_DATASTART_EDIT	, m_ndstart	);
	DDX_Control(pDX, IDC_DATALEN_EDIT	, m_ndlen	);
}


BEGIN_MESSAGE_MAP(CNandFatDlg, CBaseCtrlDlg)
	ON_BN_CLICKED(IDC_REFRESH_BUTTON	, OnBnClickedRefreshButton		)
	ON_BN_CLICKED(IDC_INITNAND_BUTTON	, OnBnClickedInitnandButton		)
	ON_BN_CLICKED(IDC_DATAREAD_BUTTON	, OnBnClickedDatareadButton		)
	ON_BN_CLICKED(IDC_DATAWRITE_BUTTON	, OnBnClickedDatawriteButton	)
	ON_BN_CLICKED(IDC_SPAREREAD_BUTTON	, OnBnClickedSparereadButton	)
	ON_BN_CLICKED(IDC_SPAREWRITE_BUTTON	, OnBnClickedSparewriteButton	)
	ON_BN_CLICKED(IDC_ERASE_BUTTON		, OnBnClickedEraseButton		)
	ON_BN_CLICKED(IDC_BLKUPDATE_BUTTON, OnBnClickedBlkupdateButton)
	ON_BN_CLICKED(IDC_OPEN_EMU_BUTTON, OnBnClickedOpenEmuButton)
	ON_BN_CLICKED(IDC_CLOSE_EMU_BUTTON, OnBnClickedCloseEmuButton)
	ON_BN_CLICKED(IDC_FS_MOUNT_BUTTON, OnBnClickedFsMountButton)
END_MESSAGE_MAP()


BOOL CNandFatDlg::OnInitDialog()
{
	CBaseCtrlDlg::OnInitDialog();

	this->InitUserOption();
	this->DispOption();

	return TRUE;  // return TRUE unless you set the focus to a control
}

void CNandFatDlg::InitUserOption(void)
{
	memset(&(this->m_option), 0, sizeof(TNandFatOption));
	this->m_option.initflag = NAND_UNINIT;
}

void CNandFatDlg::GetEditboxValue(void)
{
	char str[20];

	this->m_ndstart.GetWindowText(str, 20);
	sscanf(str, "%x", &(this->m_option.start));
	this->m_ndlen.GetWindowText(str, 20);
	sscanf(str, "%x", &(this->m_option.len));
	this->m_ndblock.GetWindowText(str, 20);
	sscanf(str, "%x", &(this->m_option.nblock));
	this->m_ndpage.GetWindowText(str, 20);
	sscanf(str, "%x", &(this->m_option.npage));
}

void CNandFatDlg::GetCheckBtnValue(void)
{
}

void CNandFatDlg::GetBarValue(void)
{
}

void CNandFatDlg::DispBarEditBox(void)
{
}

void CNandFatDlg::AdjustScrollBar(void)
{
	this->GetBarValue();
	this->DispBarEditBox();
}

void CNandFatDlg::GetComboValue(char *tail)
{
	CBasePanelDlg::GetFileBoxOption(this->m_option.srcfile, this->m_option.dstfile, tail);
}

void CNandFatDlg::GetUserOption(char *tail)
{
	this->GetEditboxValue();
	this->GetCheckBtnValue();
	this->GetComboValue(tail);
}

void CNandFatDlg::DispOption(void)
{
	char str[20];

	GetDlgItem(IDC_FLASHTYPE_STATIC)->SetWindowText(gNandTypeList[NAND_TYPE]);

	this->AdjustScrollBar();

	sprintf(str, "%x", this->m_option.makerid);
	this->m_makerid.SetWindowText(str);
	sprintf(str, "%x", this->m_option.deviceid);
	this->m_devid.SetWindowText(str);
	sprintf(str, "%x", this->m_option.nblock);
	this->m_ndblock.SetWindowText(str);
	sprintf(str, "%x", this->m_option.npage);
	this->m_ndpage.SetWindowText(str);
	sprintf(str, "%x", this->m_option.start);
	this->m_ndstart.SetWindowText(str);
	sprintf(str, "%x", this->m_option.len);
	this->m_ndlen.SetWindowText(str);
}

void CNandFatDlg::OnBnClickedInitnandButton()
{
	NandInit();
	this->m_option.initflag = NAND_INITED;
}

bool CNandFatDlg::CheckInitFlg(void)
{
	if(this->m_option.initflag)
	{
		MessageBox("Please \"Initial\" first !", "Warning", MB_OK | MB_ICONWARNING);
		return false;
	}
	return true;
}

void CNandFatDlg::OnBnClickedRefreshButton()
{
	int id = 0;

	if(false == this->CheckInitFlg())
		return;

	id = NandGetId();
	this->m_option.deviceid = id & 0xff;
	this->m_option.makerid  = (id >> 8) & 0xff;

	this->DispOption();
}

void CNandFatDlg::OnBnClickedDatareadButton()
{
	CBufferBase pool;
	CFileBin fb;

	if(false == this->CheckInitFlg())
		return;

	this->GetUserOption();
	pool.SetSize(this->m_option.len);
	NandRead(this->m_option.start, pool.m_buf, this->m_option.len);
	fb.Open(this->m_option.dstfile, "wb");
	fb.Write(pool.m_buf, this->m_option.len);
}

void CNandFatDlg::OnBnClickedDatawriteButton()
{
	CBufferBase pool;
	CFileBin fb;

	if(false == this->CheckInitFlg())
		return;

	this->GetUserOption();
	pool.SetSize(this->m_option.len);
	fb.Open(this->m_option.srcfile, "rb");
	fb.Read(pool.m_buf, this->m_option.len);
	NandWrite(this->m_option.start, pool.m_buf, this->m_option.len);
}

void CNandFatDlg::OnBnClickedSparereadButton()
{
	int size = 0, num = 0;
	CBufferBase pool;
	CFileBin fb;

	if(false == this->CheckInitFlg())
		return;

	this->GetUserOption();
	num  = gNandInfo.tnum - gNandInfo.dnum;
	size = gNandInfo.unit * num;
	pool.SetSize(size);
	NandReadSpare(this->m_option.nblock, this->m_option.npage, 0, pool.m_buf, num);
	fb.Open(this->m_option.dstfile, "wb");
	fb.Write(pool.m_buf, size);
}

void CNandFatDlg::OnBnClickedSparewriteButton()
{
	int size = 0, num = 0;
	CBufferBase pool;
	CFileBin fb;

	if(false == this->CheckInitFlg())
		return;

	this->GetUserOption();
	num  = gNandInfo.tnum - gNandInfo.dnum;
	size = gNandInfo.unit * num;
	pool.SetSize(size);
	fb.Open(this->m_option.srcfile, "rb");
	fb.Read(pool.m_buf, size);
	NandWriteSpare(this->m_option.nblock, this->m_option.npage, 0, pool.m_buf, num);
}

void CNandFatDlg::OnBnClickedEraseButton()
{
	int err= 0;

	if(false == this->CheckInitFlg())
		return;

	this->GetUserOption();
	err = NandErase(this->m_option.nblock);
	if(err != NAND_OK)
		MessageBox("Erase Block Failed !", "Error", MB_OK | MB_ICONERROR);
}

void CNandFatDlg::OnBnClickedBlkupdateButton()
{
	CBufferBase pool;
	CFileBin fb;

	if(false == this->CheckInitFlg())
		return;

	this->GetUserOption();
	pool.SetSize(BLOCK_MAP_SIZE);
	NandUpdateMap(pool.m_buf);
	fb.Open(this->m_option.dstfile, "wb");
	fb.Write(pool.m_buf, BLOCK_MAP_SIZE);
}

void CNandFatDlg::OnBnClickedOpenEmuButton()
{
	this->GetUserOption(FILE_TAIL_WDK);
	::gHardDisk.Open(this->m_option.srcfile);
}

void CNandFatDlg::OnBnClickedCloseEmuButton()
{
	::gHardDisk.Close();
}

static int DeviceRd(char *buf, UINT32 s_idx, UINT32 s_cnt)
{
	return ::gHardDisk.ReadEx(buf, s_idx, s_cnt);
}

static int DeviceWr(char *buf, UINT32 s_idx, UINT32 s_cnt)
{
	return ::gHardDisk.WriteEx(buf, s_idx, s_cnt);
}

void CNandFatDlg::OnBnClickedFsMountButton()
{
	TDevInfo dev = { DeviceRd,
					 DeviceWr	};
	VFatMount(&dev);

	VFatUnmount();		// to avoid ram leak
}

// CNandFatDlg 消息处理程序
#else
void nullNandFat(void)
{
}
#endif //DEVICECMD_DISABLE
