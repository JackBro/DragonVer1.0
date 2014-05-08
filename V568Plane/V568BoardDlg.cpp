// V568UsbcDlg.cpp : 实现文件
//

#include "internal.h"


#ifndef	V578CMD_DISABLE



// CV568BoardDlg 对话框

IMPLEMENT_DYNAMIC(CV568BoardDlg, CBaseCtrlDlg)
CV568BoardDlg::CV568BoardDlg(CWnd* pParent /*=NULL*/)
	: CBaseCtrlDlg(CV568BoardDlg::IDD, pParent)
{
}

CV568BoardDlg::~CV568BoardDlg()
{
}

void CV568BoardDlg::DoDataExchange(CDataExchange* pDX)
{
	CBaseCtrlDlg::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LENGTH_EDIT, m_length);
	DDX_Control(pDX, IDC_ADDRESS_EDIT, m_ramaddr);
	DDX_Radio(pDX, IDC_RADIO_MUL8, this->m_option.bustype);
	DDX_Control(pDX, IDC_RADIO_MUL8, m_mul8Btn);
	DDX_Control(pDX, IDC_RADIO_MUL16, m_mul16Btn);
	DDX_Control(pDX, IDC_RADIO_SEP, m_sepBtn);
	DDX_Control(pDX, IDC_CHECK_AUTOSELBUSTYPE, m_autoBtn);	
	DDX_Control(pDX, IDC_REGADDR_EDIT, m_regaddr);
}


BEGIN_MESSAGE_MAP(CV568BoardDlg, CBaseCtrlDlg)
	ON_BN_CLICKED(IDC_BUTTON_TEST_SENSOR, OnBnClickedButtonTestSensor)
	ON_BN_CLICKED(IDC_WRITESRAM_BUTTON, OnBnClickedWritesramButton)
	ON_BN_CLICKED(IDC_READSRAM_BUTTON, OnBnClickedReadsramButton)
	ON_BN_CLICKED(IDC_CHECK_AUTOSELBUSTYPE, OnBnClickedCheckAutoselbustype)
	ON_BN_CLICKED(IDC_RADIO_MUL8, OnBnClickedRadioMul8)
	ON_BN_CLICKED(IDC_RADIO_MUL16, OnBnClickedRadioMul16)
	ON_BN_CLICKED(IDC_RADIO_SEP, OnBnClickedRadioSep)
	ON_BN_CLICKED(IDC_TESTREGRW_BUTTON, OnBnClickedTestregrwButton)
END_MESSAGE_MAP()


// CV568BoardDlg 消息处理程序



void CV568BoardDlg::InitUserOption(void)
{
	this->m_ramaddr.SetWindowText("0");
	this->m_length.SetWindowText("0");
	this->m_regaddr.SetWindowText("0");

	this->GetDlgItem(IDC_BUSTYPE_EDIT)->SetWindowText(gV568BusTypeDesc[V5_BUS_TYPE-1]);
	this->GetDlgItem(IDC_BUSMODE_EDIT)->SetWindowText(gV568BusModeDesc[V5_BUS_MODE]);
	//this->GetDlgItem(IDC_SENSOR_EDIT )->SetWindowText(gV568CurSensorDesc[V5_SNR_TYPE]);
	this->GetDlgItem(IDC_LCD_EDIT    )->SetWindowText(gV568CurLcdDesc[V5_LCD_TYPE-1]);

	m_mul8Btn.EnableWindow(FALSE);
	m_mul16Btn.EnableWindow(FALSE);
	m_sepBtn.EnableWindow(FALSE);

	m_autoBtn.SetCheck(BST_CHECKED);
	m_mul16Btn.SetCheck(BST_CHECKED);
	m_mul8Btn.SetCheck(BST_UNCHECKED);
	m_sepBtn.SetCheck(BST_UNCHECKED);

	this->GetDlgItem(IDC_BUSTYPE_EDIT)->SetWindowText("V5_BUS_MULTI16");

}


void CV568BoardDlg::GetUserOption(char *tail)
{
	char str[20];

	this->m_length.GetWindowText(str, 20);
	sscanf(str, "%x", &(this->m_option.length));
	this->m_ramaddr.GetWindowText(str, 20);
	sscanf(str, "%x", &(this->m_option.ramaddr));
	this->m_regaddr.GetWindowText(str, 20);
	sscanf(str, "%x", &(this->m_option.regaddr));

	CBasePanelDlg::GetFileBoxOption(this->m_option.srcfile, this->m_option.dstfile, tail);
}

BOOL CV568BoardDlg::OnInitDialog()
{
	CBaseCtrlDlg::OnInitDialog();

	this->InitUserOption();

	return TRUE;  // return TRUE unless you set the focus to a control
}


void CV568BoardDlg::OnBnClickedButtonTestSensor()
{
	CWaitCursor cursor;

	V5_Open();
	V5_CameraOnForSnrTest(0);
}

void CV568BoardDlg::OnBnClickedWritesramButton()
{
	CBufferBase pool;
	CFileBin fb;

	this->GetUserOption(FILE_TAIL_DAT);
	pool.SetSize(this->m_option.length);
	fb.Open(this->m_option.srcfile, "rb");
	fb.Read(pool.m_buf, this->m_option.length);
	V5_WriteSram(this->m_option.ramaddr, (UINT8 *)pool.m_buf, this->m_option.length);
}

void CV568BoardDlg::OnBnClickedReadsramButton()
{
	CBufferBase pool;
	CFileBin fb;

	this->GetUserOption(FILE_TAIL_DAT);
	pool.SetSize(this->m_option.length);
	V5_ReadSram(this->m_option.ramaddr, (UINT8 *)pool.m_buf, this->m_option.length);
	fb.Open(this->m_option.dstfile, "wb");
	fb.Write(pool.m_buf, this->m_option.length);
}

void CV568BoardDlg::OnBnClickedCheckAutoselbustype()
{
	unsigned char busType;

	if(BST_CHECKED == m_autoBtn.GetCheck())
	{
		m_mul8Btn.EnableWindow(FALSE);
		m_mul16Btn.EnableWindow(FALSE);
		m_sepBtn.EnableWindow(FALSE);

		busType = V5_SetBusTypeMulOrSep();
       
		if(busType == V5_BUS_MULTI16)
		{
		    this->GetDlgItem(IDC_BUSTYPE_EDIT)->SetWindowText("V5_BUS_MULTI16");
			m_mul16Btn.SetCheck(BST_CHECKED);
			m_mul8Btn.SetCheck(BST_UNCHECKED);
			m_sepBtn.SetCheck(BST_UNCHECKED);
		}
		else
		{
			this->GetDlgItem(IDC_BUSTYPE_EDIT)->SetWindowText("V5_BUS_SEPERATE");
			m_sepBtn.SetCheck(BST_CHECKED);
			m_mul8Btn.SetCheck(BST_UNCHECKED);
			m_mul16Btn.SetCheck(BST_UNCHECKED);
		}
	}
	else
	{
		m_mul8Btn.EnableWindow(TRUE);
		m_mul16Btn.EnableWindow(TRUE);
		m_sepBtn.EnableWindow(TRUE);
	}
}

void CV568BoardDlg::OnBnClickedRadioMul8()
{
	CWaitCursor cursor;
	char str[0x100];

	sprintf(str, "V5B_SetBusType %x\n", V5_BUS_MULTI8);
	SendCmd(str);

	this->GetDlgItem(IDC_BUSTYPE_EDIT)->SetWindowText("V5_BUS_MULTI8");
}

void CV568BoardDlg::OnBnClickedRadioMul16()
{
	CWaitCursor cursor;
	char str[0x100];

	sprintf(str, "V5B_SetBusType %x\n", V5_BUS_MULTI16);
	SendCmd(str);
   
	this->GetDlgItem(IDC_BUSTYPE_EDIT)->SetWindowText("V5_BUS_MULTI16");
}

void CV568BoardDlg::OnBnClickedRadioSep()
{
	CWaitCursor cursor;
	char str[0x100];

	sprintf(str, "V5B_SetBusType %x\n", V5_BUS_SEPERATE);
	SendCmd(str);

	this->GetDlgItem(IDC_BUSTYPE_EDIT)->SetWindowText("V5_BUS_SEPERATE");
}

void CV568BoardDlg::OnBnClickedTestregrwButton()
{
	CWaitCursor cursor;

	this->GetUserOption(FILE_TAIL_DAT);
	testRegRw(this->m_option.regaddr, 1000);
}


#endif //V578CMD_DISABLE
