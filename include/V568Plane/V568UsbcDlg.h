#ifndef __V568PLANE_V568USBCDLG_H__
#define __V568PLANE_V568USBCDLG_H__


#ifndef V568PLANE_DLL
	#define V568PLANE_DLL _declspec(dllimport)
#endif

#include "BasicFunc/BaseCtrlDlg.h"

#include "../../terminal/VCMobi/mobiusr.h"

#include <setupapi.h>


// Copy from ddk usb.h to avoid build error
typedef struct tagTUSB_DEVICE_DESCRIPTOR__1 {
    UCHAR bLength;
    UCHAR bDescriptorType;
    USHORT bcdUSB;
    UCHAR bDeviceClass;
    UCHAR bDeviceSubClass;
    UCHAR bDeviceProtocol;
    UCHAR bMaxPacketSize0;
    USHORT idVendor;
    USHORT idProduct;
    USHORT bcdDevice;
    UCHAR iManufacturer;
    UCHAR iProduct;
    UCHAR iSerialNumber;
    UCHAR bNumConfigurations;
} TUSB_DEVICE_DESCRIPTOR__1, *PTUSB_DEVICE_DESCRIPTOR__1;


class CUsbTestChannel
{
public:
	CUsbTestChannel(void);
	virtual ~CUsbTestChannel(void);

	virtual void	Open(void);
	virtual void	Close(void);
	virtual int		IsOpened(void);

protected:
	virtual int		ReadChannel(char *buf, int size);
	virtual int		WriteChannel(char *buf, int size);

public:
	void	SetPipe(int pipe = 0);
	int		GetPipe(void);

protected:
	void	Init(void);
	void	CloseUsb(void);


private:
	BOOL EnumUsbDevName(LPGUID pGuid, char* devName);
	BOOL GetOneDevName(HDEVINFO HardwareDeviceInfo, PSP_DEVICE_INTERFACE_DATA DeviceInfoData, char *devName);

private:
	HANDLE	m_HIn;
	HANDLE	m_HOut;
	int		m_Pipe;

    HANDLE		m_hrdlap;
    HANDLE		m_hwrlap;
};

#define UART_TERMINAL_TIME	2000
#define UART_INQUEUE_SIZE	4096
#define UART_OUTQUEUE_SIZE	4096

class CSerTestChannel
{
public:
	CSerTestChannel(void);
	virtual ~CSerTestChannel(void);

	virtual void	Open(void);
	virtual void	Close(void);
	virtual int		IsOpened(void);

public:
	virtual int		ReadChannel(char *buf, int size);
	virtual int		WriteChannel(char *buf, int size);

public:
	void	SetPort(int port = 1);
	int		GetPort(void);
	void	SetupBaud(int baud = CBR_115200, int dnum = 8, int enparity = 0, int stop = 0);
	void	SetupBaud(char *setting);
	int		GetBaud(void);
	void	ManualConfigPort(void);

private:
	void	Init(void);
	void	SetupTimesout(void);
	void	SetupHandShake(void);
	void	CloseUart(void);


private:
	HANDLE	m_Hcomm;
	int		m_Port;		//0 is error
	int		m_baud;
	
};

enum USB_SER_TST {
	USB_SER_TST_USB	,
	USB_SER_TST_SER
};

class CUsbSerTestChannel : public CSerTestChannel, public CUsbTestChannel
{
public:
	CUsbSerTestChannel();

public:
	virtual void	Open(void);
	virtual void	Close(void);
	virtual int		IsOpened(void);

public:
	virtual int		ReadChannel(char *buf, int size);
	virtual int		WriteChannel(char *buf, int size);

public:
	void	ChannelSelect(int sel = USB_SER_TST_SER);
	int		GetChannelSel(void);


private:

	int		m_sel;
};


#define UMB_CBW_SIGN			0x43425355
#define UMB_CSW_SIGN			0x53425355

//Udc return cmd data len.
#define UDC_FMT_CAP_LEN					12
#define UDC_CAP_LEN						8
#define UDC_SEN_LEN						18
#define UDC_MODE_SEL6_LEN				4
#define UDC_MODE_SENSE6_LEN				4
#define UDC_MODE_SEL10_LEN				8
#define UDC_MODE_SENSE10_LEN			8


enum {
	DADCMD_CHG_DEF_O					= 0x40,
	DADCMD_COMP_O						= 0x39,
	DADCMD_COPY_O						= 0x18,
	DADCMD_COPY_VERIFY_O				= 0x3A,
	DADCMD_FORMAT_UNIT_M				= 0x04,
	DADCMD_INQUIRY_M					= 0x12,
	DADCMD_LOCK_UNLOCK_CACHE_O			= 0x36,
	DADCMD_LOG_SEL_O					= 0x4C,
	DADCMD_LOG_SENSE_O					= 0x4D,
	DADCMD_MOD_SEL6_O					= 0x15,
	DADCMD_MOD_SEL10_O					= 0x55,
	DADCMD_MOD_SENSE6_O					= 0x1A,
	DADCMD_MOD_SENSE10_O				= 0x5A,
	DADCMD_MOV_MEDIUM_O					= 0xA7,
	DADCMD_PERS_IN_O					= 0x5E,
	DADCMD_PERS_OUT_O					= 0x5F,
	DADCMD_PRE_FETCH_O					= 0x34,
	DADCMD_P_A_REMOVAL_O				= 0x1E,
	DADCMD_RD6_M						= 0x08,
	DADCMD_RD10_M						= 0x28,
	DADCMD_RD12_O						= 0xA8,
	DADCMD_RD_BUF_O						= 0x3C,
	DADCMD_RD_FMT_CAP_M					= 0x23,
	DADCMD_RD_CAP_M						= 0x25,
	DADCMD_RD_DFT_DATA10_O				= 0x37,
	DADCMD_RD_DFT_DATA12_O				= 0xB7,
	DADCMD_RD_ELEM_STATUS_O				= 0xB4,
	DADCMD_RD_LONG_O					= 0x3E,
	DADCMD_REASSIGN_BLK_O				= 0x07,
	DADCMD_REBUILD_O					= 0x81,
	DADCMD_REC_DIAG_RET_O				= 0x1C,
	DADCMD_REGENERATE_O					= 0x82,
	DADCMD_RELEASE6_O					= 0x17,
	DADCMD_RELEASE10_M					= 0x57,
	DADCMD_REPORT_LUNS_O				= 0xA0,
	DADCMD_REQ_SENSE_M					= 0x03,
	DADCMD_RESERVE6_O					= 0x16,
	DADCMD_RESERVE10_M					= 0x56,
	DADCMD_SEEK10_O						= 0x2B,
	DADCMD_SEND_DIAG_M					= 0x1D,
	DADCMD_SET_LIMITS10_O				= 0x33,
	DADCMD_SET_LIMITS12_O				= 0xB3,
	DADCMD_START_STOP_UNIT_O			= 0x1B,
	DADCMD_SYNC_CACHE_O					= 0x35,
	DADCMD_TEST_UNIT_READY_M			= 0x00,
	DADCMD_VERIFY_O						= 0x2F,
	DADCMD_WR6_O						= 0x0A,
	DADCMD_WR10_O						= 0x2A,
	DADCMD_WR12_O						= 0xAa,
	DADCMD_WR_VERIFY_O					= 0x2E,
	DADCMD_WR_BUF_O						= 0x3B,
	DADCMD_WR_LONG_O					= 0x3F,
	DADCMD_WR_SAME_O					= 0x41,
	DADCMD_XDRD_O						= 0x52,
	DADCMD_XDWR_O						= 0x50,
	DADCMD_XDWR_EXT_O					= 0x80,
	DADCMD_XPWR_O						= 0x51
};

#pragma pack(1)

typedef struct _UMB_CBW
{
	ULONG	Signature;
	ULONG	Tag;
	ULONG	TransferLength;
	UCHAR	Flags;
	UCHAR	Resv_Lun;
	UCHAR	Resv_Length;
	UCHAR	CbOpt;
	UCHAR	CbData[15 + 1]; //Additional one byte is a dummy data. 
}UMB_CBW, *PUMB_CBW;

typedef struct _UMB_CSW
{
	ULONG	Signature;
	ULONG	Tag;
	ULONG	DataResidue;
	UCHAR	Status;
}UMB_CSW, *PUMB_CSW;

#pragma pack()

// CV568UsbcDlg 对话框

class V568PLANE_DLL CV568UsbcDlg : public CBaseCtrlDlg
{
	DECLARE_DYNAMIC(CV568UsbcDlg)

public:
	CV568UsbcDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CV568UsbcDlg();

// 对话框数据
	enum { IDD = IDD_V568USBC_DLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	virtual BOOL OnInitDialog();

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButtonWrite();
	afx_msg void OnBnClickedButtonRead();
	afx_msg void OnBnClickedRadioCom();
	afx_msg void OnBnClickedRadioUsb();
	afx_msg void OnBnClickedButtonConnect();

private:
	CMComboBox	m_CombCom;
	CButton	m_ConBtn;
	
	int	m_Sel;
	int m_StSec;
	int m_EdSec;
	int m_TstCnt;
	long m_RdSpeed;
	long m_WrSpeed;
	BOOL m_bConnect;

	char m_srcfile[DEFAULT_FILENAME_LEN];
	char m_dstfile[DEFAULT_FILENAME_LEN];

	CUsbSerTestChannel m_UsbSer;

	UMB_CBW m_Cbw;
	UMB_CSW m_Csw;

public:
	afx_msg void OnBnClickedButtonSendCmd();

private:
	void Dummy();
	BOOL ReadResponse();
	BOOL ReadData(unsigned char *buf, int size);
	void CV568UsbcDlg::UmscReadCap(void);
	BOOL SendChkCmd(char *cmd);
	BOOL WriteBuf(int adr, char *buf, int len);
	BOOL ReadBuf(int adr, char *buf, int len);


public:
	afx_msg void OnBnClickedButtonTestNormal();
};




#endif
