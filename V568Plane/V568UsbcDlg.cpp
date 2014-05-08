// V568UsbcDlg.cpp : 实现文件
//
#include "internal.h"
#include "..\include\v568plane\v568usbcdlg.h"

UINT32 UdcGet32Bbuf(UINT8 *buf)
{
	return ((UINT32) buf[3] ) | ((UINT32) buf[2] << 8) |
			((UINT32) buf[1] << 16) | ((UINT32) buf[0] << 24);
}

UINT16 UdcGet16Bbuf(UINT8 *buf)
{
	return ((UINT16) buf[1] ) | ((UINT16) buf[0] << 8);
}

void UdcPut32Bbuf(UINT32 val, UINT8 *buf)
{
	buf[0] = (UINT8)(val >> 24);
	buf[1] = (UINT8)(val >> 16);
	buf[2] = (UINT8)(val >> 8);
	buf[3] = (UINT8)val;
}

void UdcPut16Bbuf(UINT16 val, UINT8 *buf)
{
	buf[0] =  (UINT8)(val >> 8);
	buf[1] =  (UINT8)val ;
}

#ifndef	V578CMD_DISABLE

// CV568UsbcDlg 对话框

IMPLEMENT_DYNAMIC(CV568UsbcDlg, CBaseCtrlDlg)
CV568UsbcDlg::CV568UsbcDlg(CWnd* pParent /*=NULL*/)
	: CBaseCtrlDlg(CV568UsbcDlg::IDD, pParent)
{

	m_StSec = 0;
	m_EdSec = 0;
	m_Sel = 1;
	m_TstCnt = 10;
	m_RdSpeed = 0;
	m_WrSpeed = 0;

	m_bConnect = FALSE;

	memset(&m_Cbw, 0, sizeof(UMB_CBW));
	m_Cbw.Signature = UMB_CBW_SIGN;

}

CV568UsbcDlg::~CV568UsbcDlg()
{
}

void CV568UsbcDlg::DoDataExchange(CDataExchange* pDX)
{
	CBaseCtrlDlg::DoDataExchange(pDX);

	DDX_Control(pDX, IDC_BUTTON_CONNECT,	m_ConBtn);
	DDX_Control(pDX, IDC_COMBO_COM,	m_CombCom);
	DDX_Check(pDX, IDC_RADIO_COM,	m_Sel);
	DDX_Text(pDX, IDC_EDIT_SEC_ST, m_StSec);
	DDX_Text(pDX, IDC_EDIT_SEC_ED, m_EdSec);

	DDX_Text(pDX, IDC_EDIT_TEST_CNT, m_TstCnt);
	DDX_Text(pDX, IDC_EDIT_RD_SPEED, m_RdSpeed);
	DDX_Text(pDX, IDC_EDIT_WR_SPEED, m_WrSpeed);

}


BEGIN_MESSAGE_MAP(CV568UsbcDlg, CBaseCtrlDlg)
	ON_BN_CLICKED(IDC_BUTTON_WRITE, OnBnClickedButtonWrite)
	ON_BN_CLICKED(IDC_BUTTON_READ, OnBnClickedButtonRead)
	ON_BN_CLICKED(IDC_RADIO_COM, OnBnClickedRadioCom)
	ON_BN_CLICKED(IDC_RADIO_USB, OnBnClickedRadioUsb)

	ON_BN_CLICKED(IDC_BUTTON_CONNECT, OnBnClickedButtonConnect)
	ON_BN_CLICKED(IDC_BUTTON_SEND_CMD, OnBnClickedButtonSendCmd)
	ON_BN_CLICKED(IDC_BUTTON_TEST_NORMAL, OnBnClickedButtonTestNormal)
END_MESSAGE_MAP()


#define MAX_PACK_NUM (128)
#define PACK_LEN (512)
#define MAX_PACK_LEN (PACK_LEN*MAX_PACK_NUM)

void CV568UsbcDlg::OnBnClickedButtonWrite()
{
	// TODO: 在此添加控件通知处理程序代码
	CBasePanelDlg::GetFileBoxOption(m_srcfile, m_dstfile, "dat");
	this->UpdateData();

	if(!m_bConnect)
	{
		::AfxMessageBox("Not open");
		return;
	}

	CFile file;
	char* buf;

	if( !file.Open(m_srcfile, CFile::modeRead) )
		return;

	buf = (char*)malloc(MAX_PACK_LEN);
	if(!buf)
	{
		file.Close();
		return;
	}

	ULONG len = (ULONG)file.GetLength();

	len = len/512;
	m_EdSec = len;
	this->UpdateData(FALSE);


	ULONG dwBlkSt = 0;
	ULONG dwBlkNum = 0;


	dwBlkSt = m_StSec;

	for(int i=0; i< m_EdSec; i+=MAX_PACK_NUM)
	{
		len = file.Read(buf, MAX_PACK_LEN);
		dwBlkNum = len/512;
		len = dwBlkNum*512;
		m_Cbw.Flags ++;
		m_Cbw.CbOpt = DADCMD_WR10_O;
		m_Cbw.TransferLength = len;

		UdcPut32Bbuf(dwBlkSt, m_Cbw.CbData+1);
		UdcPut16Bbuf((USHORT)dwBlkNum, m_Cbw.CbData+6);

		dwBlkSt += dwBlkNum;

		Dummy();
		if(m_UsbSer.WriteChannel((char*)&m_Cbw, sizeof(UMB_CBW)) != sizeof(UMB_CBW))
			break;
		if(m_UsbSer.WriteChannel(buf, len) != (int)len)
			break;
		if(!ReadResponse())
		{
			TRACE("Res ponse failed");
			break;
		}

		if(len < MAX_PACK_LEN)
			break;
	}

	free(buf);
	file.Close();

}

void CV568UsbcDlg::Dummy()
{
	int i, len;
	char buf[0x100];

	for(i=0; i<3; i++)
	{
		do {
			Sleep(1);
			len = m_UsbSer.ReadChannel(buf, 0x100);
		} while(len);
	}
}

BOOL CV568UsbcDlg::ReadResponse()
{
	int len;

	while(1) {
		len = m_UsbSer.ReadChannel((char*)&m_Csw, sizeof(UMB_CSW));
		if(!len)
			Sleep(1);
		else
			break;
	}
	if(len != sizeof(UMB_CSW))
		return FALSE;

	if(m_Csw.Signature != UMB_CSW_SIGN)
		return FALSE;
	if(m_Csw.Tag != m_Cbw.Tag)
		return FALSE;

	return TRUE;
}

BOOL CV568UsbcDlg::ReadData(unsigned char *buf, int size)
{
	int len;
	while(size) {
		len = m_UsbSer.ReadChannel((char*)buf, size);
		if(!len)
			Sleep(1);
		else
		{
			size -= len;
			buf += len;
		}
		
	}

	return TRUE;

}

void CV568UsbcDlg::UmscReadCap(void)
{
	unsigned char buf[100];

	m_Cbw.Flags ++;
	m_Cbw.CbOpt = DADCMD_RD_CAP_M;
	m_Cbw.TransferLength = UDC_CAP_LEN;


	Dummy();
	if(m_UsbSer.WriteChannel((char*)&m_Cbw, sizeof(UMB_CBW)) != sizeof(UMB_CBW))
		return;

	ReadData(buf, sizeof(UMB_CSW) + UDC_CAP_LEN);

}


void CV568UsbcDlg::OnBnClickedButtonRead()
{
	// TODO: 在此添加控件通知处理程序代码
	CBasePanelDlg::GetFileBoxOption(m_srcfile, m_dstfile, "dat");
	this->UpdateData();

	if(!m_bConnect)
	{
		::AfxMessageBox("Not open");
		return;
	}

	CFile file;
	char* buf;

	if( !file.Open(m_dstfile, CFile::modeCreate | CFile::modeWrite) )
		return;

	buf = (char*)malloc(MAX_PACK_LEN);
	if(!buf)
	{
		file.Close();
		return;
	}

	ULONG dwBlkSt = 0;
	ULONG dwBlkNum = 0;

	dwBlkSt = m_StSec;
	ULONG totallen = m_EdSec*512;

	ULONG len = 0;
	for(int i=0; i< m_EdSec; i+=MAX_PACK_NUM)
	{
		len = (totallen > MAX_PACK_LEN) ? MAX_PACK_LEN : totallen;
		totallen -= MAX_PACK_LEN;
		dwBlkNum = len/512;
		len = dwBlkNum*512;
		m_Cbw.Flags ++;
		m_Cbw.CbOpt = DADCMD_RD10_M;
		m_Cbw.TransferLength = (dwBlkNum * 512);

		UdcPut32Bbuf(dwBlkSt, m_Cbw.CbData+1);
		UdcPut16Bbuf((USHORT)dwBlkNum, m_Cbw.CbData+6);

		dwBlkSt += dwBlkNum;

		Dummy();
		if(m_UsbSer.WriteChannel((char*)&m_Cbw, sizeof(UMB_CBW)) != sizeof(UMB_CBW))
			break;
		if( !ReadData((unsigned char*)buf, len) )
			break;

		file.Write(buf, len);

		if(!ReadResponse())
		{
			TRACE("Res ponse failed");
			break;
		}


		if(len < MAX_PACK_LEN)
			break;
	}

	free(buf);
	file.Close();
}

void CV568UsbcDlg::OnBnClickedRadioCom()
{
	// TODO: 在此添加控件通知处理程序代码
	m_CombCom.EnableWindow();
}

void CV568UsbcDlg::OnBnClickedRadioUsb()
{
	// TODO: 在此添加控件通知处理程序代码
	m_CombCom.EnableWindow(FALSE);
}

BOOL CV568UsbcDlg::OnInitDialog()
{
	CBaseCtrlDlg::OnInitDialog();

	CString str;
	int i = 0;

	for(i=1; i< 10; i++)
	{
		str.Format("COM%d", i);
		m_CombCom.AddString(str);
	}

	m_CombCom.SetCurSel(3);
	
	return TRUE;
}

void CV568UsbcDlg::OnBnClickedButtonConnect()
{
	// TODO: 在此添加控件通知处理程序代码
	if(m_Sel)
	{
		if(m_bConnect)
		{
			m_UsbSer.Close();
			m_bConnect = FALSE;
			m_ConBtn.SetWindowText("Connect");
		}
		else
		{
			if(m_Sel)
			{
				m_UsbSer.ChannelSelect();
				m_UsbSer.SetPort(m_CombCom.GetCurSel()+1);
			}
			else
			{
				m_UsbSer.ChannelSelect(USB_SER_TST_USB);
				m_UsbSer.SetPort(m_CombCom.GetCurSel()+1);
			}

			m_UsbSer.Open();
			if(m_UsbSer.IsOpened())
			{
				m_ConBtn.SetWindowText("Disconnect");
				m_bConnect = TRUE;
			}
		}
	}
}


void CV568UsbcDlg::OnBnClickedButtonSendCmd()
{
	// TODO: 在此添加控件通知处理程序代码
	UmscReadCap();
}


BOOL CV568UsbcDlg::SendChkCmd(char *cmd)
{
	char buf[100];
	int len;

	len = (int)strlen(cmd);
	if(len > 100)
		len = 100;
	if(m_UsbSer.WriteChannel(cmd, len) != len)
		return FALSE;

	if( !ReadData((unsigned char *)buf, len) )
		return FALSE;

	if( 0 != memcmp(cmd, buf, len) )
		return FALSE;

	return TRUE;
}

BOOL CV568UsbcDlg::WriteBuf(int adr, char *buf, int len)
{
	char cmd[0x100];

	sprintf(cmd, "%s %d %d\n", "WriteBuf", adr, len);
	TRACE("%s\n", cmd);

	Dummy();

	if( !SendChkCmd(cmd) )
		return FALSE;

	if(m_UsbSer.WriteChannel(buf, len) != len)
		return FALSE;

	Dummy();

	return TRUE;
}

BOOL CV568UsbcDlg::ReadBuf(int adr, char *buf, int len)
{
	char cmd[0x100];

	sprintf(cmd, "%s %d %d\n", "ReadBuf", adr, len);
	TRACE("%s\n", cmd);

	Dummy();

	if( !SendChkCmd(cmd) )
		return FALSE;

	if( !ReadData((unsigned char *)buf, len) )
		return FALSE;

	Dummy();

	return TRUE;
}

//extern int WriteBuf(int adr, char *buf, int len);
//extern int ReadBuf(int adr, char *buf, int len);

#define TEST_BUF_SIZE	(10*1024)
#define TEST_CNT_SPEED	10		

void CV568UsbcDlg::OnBnClickedButtonTestNormal()
{
	// TODO: 在此添加控件通知处理程序代码
	UINT32 msec0, msec1, msec2, msec3;
	SYSTEMTIME Time0, Time1, Time2;
	int i;

	this->UpdateData();

	if(!m_bConnect)
	{
		::AfxMessageBox("Not open");
		return;
	}

	char *psrc = (char *)malloc(TEST_BUF_SIZE);
	ASSERT(psrc!=NULL);
	char *pdst = (char *)malloc(TEST_BUF_SIZE);
	ASSERT(pdst!=NULL);

	//Test speed.
	//Read
	GetSystemTime(&Time0);
	for( i=0; i < TEST_CNT_SPEED; i++)
	{
		if( !ReadBuf(0, psrc, 0) )
			break;
	}
	GetSystemTime(&Time1);

	for( i=0; i < TEST_CNT_SPEED; i++)
	{
		if( !ReadBuf(0, psrc, TEST_BUF_SIZE) )
			break;
	}

	GetSystemTime(&Time2);

	msec0 = ((Time0.wHour*60 + Time0.wMinute)*60 + Time0.wSecond)*1000 + Time0.wMilliseconds;
	msec1 = ((Time1.wHour*60 + Time1.wMinute)*60 + Time1.wSecond)*1000 + Time1.wMilliseconds;
	msec2 = ((Time2.wHour*60 + Time2.wMinute)*60 + Time2.wSecond)*1000 + Time2.wMilliseconds;

	msec3 = ((msec2 - msec1) - (msec1 - msec0))/TEST_CNT_SPEED;
	if(msec3 == 0)
		msec3 = 1;
	TRACE("msec3 = %d\n", msec3);
	m_RdSpeed = (TEST_BUF_SIZE)/msec3;

	//Write
	GetSystemTime(&Time0);
	for( i=0; i < TEST_CNT_SPEED; i++)
	{
		if( !WriteBuf(0, psrc, 0) )
			break;
	}
	GetSystemTime(&Time1);

	for( i=0; i < TEST_CNT_SPEED; i++)
	{
		if( !WriteBuf(0, psrc, TEST_BUF_SIZE) )
			break;
	}
#if 1
	GetSystemTime(&Time2);

	msec0 = ((Time0.wHour*60 + Time0.wMinute)*60 + Time0.wSecond)*1000 + Time0.wMilliseconds;
	msec1 = ((Time1.wHour*60 + Time1.wMinute)*60 + Time1.wSecond)*1000 + Time1.wMilliseconds;
	msec2 = ((Time2.wHour*60 + Time2.wMinute)*60 + Time2.wSecond)*1000 + Time2.wMilliseconds;

	msec3 = ((msec2 - msec1) - (msec1 - msec0))/TEST_CNT_SPEED;
	if(msec3 == 0)
		msec3 = 1;
	TRACE("msec3 = %d\n", msec3);
	m_WrSpeed = (TEST_BUF_SIZE)/msec3;
#endif

	this->UpdateData(FALSE);
#if 1
	//Test read write
	for( i=0; i < m_TstCnt; i++)
	{
		int j;
		int *pint = (int *)psrc;

		/* Seed the random-number generator with current time so that
			* the numbers will be different every time we run.
			*/
		srand( (unsigned)time( NULL ) );
		for(j=0; j<TEST_BUF_SIZE/4; j++)
			*pint++ = rand();
		WriteBuf(0, psrc, TEST_BUF_SIZE);
		ReadBuf(0, pdst, TEST_BUF_SIZE);

		if(0 != memcmp(psrc, pdst, TEST_BUF_SIZE))
		{
			::AfxMessageBox("Read and write data not equal!");
			break;
		}
	}
#endif

	free(pdst);
	free(psrc);

}


/************************************************************************************************
*
* Class CUsbTestChannel
*
************************************************************************************************/
CUsbTestChannel::CUsbTestChannel()
{
	this->Init();
}

CUsbTestChannel::~CUsbTestChannel()
{
	if(this->IsOpened())
		this->Close();
}

void CUsbTestChannel::Init(void)
{
	this->m_Pipe = 0;
	this->m_HIn = NULL;
	this->m_HOut = NULL;

	this->m_hrdlap = NULL;
	this->m_hwrlap = NULL;

}

void CUsbTestChannel::Close(void)
{

	if(this->m_hrdlap != NULL)
	{
		CloseHandle(this->m_hrdlap);
		this->m_hrdlap = NULL;
	}
	if(this->m_hwrlap != NULL)
	{
		CloseHandle(this->m_hwrlap);
		this->m_hwrlap = NULL;
	}

	this->CloseUsb();
}

void CUsbTestChannel::CloseUsb(void)
{
	if(this->m_HIn)
	{
		CloseHandle( this->m_HIn);
	}
	this->m_HIn = NULL;

	if(this->m_HOut)
	{
		CloseHandle( this->m_HOut);
	}
	this->m_HOut = NULL;

	this->m_Pipe = 0;
}

void CUsbTestChannel::Open()
{
	char DeviceName[256];
	char inpipe[256];
	char outpipe[256];

	if(this->IsOpened())
		return;

	if( !EnumUsbDevName((LPGUID) &GUID_CLASS_VCMOBI_MOBI,  DeviceName) )
	{
		return;
	}

	sprintf(inpipe, "%s\\PIPE%d", DeviceName, m_Pipe*2);
	sprintf(outpipe, "%s\\PIPE%d", DeviceName, m_Pipe*2+1);

	m_HIn = CreateFile(inpipe,
		GENERIC_READ,
		0,
		NULL,
		OPEN_EXISTING,
#ifdef OVER_LAP_USB
		FILE_ATTRIBUTE_NORMAL|FILE_FLAG_OVERLAPPED,
#else
		FILE_ATTRIBUTE_NORMAL,
#endif		
		NULL);

	m_HOut = CreateFile(outpipe,
		GENERIC_WRITE,
		0,
		NULL,
		OPEN_EXISTING,
#ifdef OVER_LAP_USB
		FILE_ATTRIBUTE_NORMAL|FILE_FLAG_OVERLAPPED,
#else
		FILE_ATTRIBUTE_NORMAL,
#endif		
		NULL);


	if ( INVALID_HANDLE_VALUE == this->m_HIn ||  INVALID_HANDLE_VALUE == this->m_HOut)
	{
		this->m_HIn = NULL;
		this->m_HOut = NULL;
		return;
	}

	if(this->m_hrdlap == NULL)
		this->m_hrdlap = ::CreateEvent(NULL, TRUE, FALSE, NULL);
	if(this->m_hwrlap == NULL)
		this->m_hwrlap = ::CreateEvent(NULL, TRUE, FALSE, NULL);


}

int	CUsbTestChannel::IsOpened(void)
{
	if(this->m_HIn && this->m_HOut)
		return TRUE;
	return FALSE;
}

int CUsbTestChannel::GetPipe(void)
{
	return this->m_Pipe;
}

void CUsbTestChannel::SetPipe(int pipe)
{
	if(this->m_Pipe == pipe)
		return;
	if(this->IsOpened())
	{
		this->Close();
		this->m_Pipe = pipe;
		this->Open();
	}
	else
		this->m_Pipe = pipe;
}


int CUsbTestChannel::ReadChannel(char *buf, int size)
{
	BOOL success;
	OVERLAPPED ov;
	DWORD len = 0;

	if(this->m_HIn == NULL)
		return 0;

	memset(&ov, 0, sizeof(OVERLAPPED));
	ov.hEvent = this->m_hrdlap;
	//::ResetEvent(ov.hEvent);
	success = ::ReadFile(this->m_HIn, buf, size, &len, &ov);
	if( (!success) && (ERROR_IO_PENDING == ::GetLastError()) )
	{
		switch(::WaitForSingleObject(ov.hEvent, 50))
		{
		case WAIT_OBJECT_0:
			::GetOverlappedResult(this->m_HIn,&ov,&len,TRUE);

			break;
		case WAIT_TIMEOUT:
			::CancelIo(this->m_HIn);
			::GetOverlappedResult(this->m_HIn,&ov,&len,TRUE); 
			break;
		case WAIT_FAILED:
			break;
		}
	}
	ResetEvent(ov.hEvent);

	return len;

}

int CUsbTestChannel::WriteChannel(char *buf, int size)
{
	OVERLAPPED ov;
	DWORD len = 0;

	if(this->m_HOut == NULL)
		return 0;

	memset(&ov, 0, sizeof(OVERLAPPED));
	ov.hEvent = this->m_hwrlap;

	//::ResetEvent(ov.hEvent);
	::WriteFile(this->m_HOut, buf, size, &len, &ov);
	::GetOverlappedResult(this->m_HOut,&ov,&len,TRUE); 	
	::ResetEvent(ov.hEvent);

	return len;
}



BOOL CUsbTestChannel::EnumUsbDevName(LPGUID pGuid,  char* devName)
{
	HDEVINFO                 hardwareDeviceInfo;
	ULONG NumberDevices;
	SP_DEVICE_INTERFACE_DATA deviceInfoData;
	PTUSB_DEVICE_DESCRIPTOR__1	usbDeviceInst;
	PTUSB_DEVICE_DESCRIPTOR__1	*UsbDevices = &usbDeviceInst;
	PTUSB_DEVICE_DESCRIPTOR__1	tempDevDesc;
	
	ULONG                    i;
	BOOLEAN                  done;

	*UsbDevices = NULL;
	tempDevDesc = NULL;
	NumberDevices = 0;
	
	//
	// Open a handle to the plug and play dev node.
	// SetupDiGetClassDevs() returns a device information set that contains info on all
	// installed devices of a specified class.
	//
	hardwareDeviceInfo = SetupDiGetClassDevs (
		pGuid,
		NULL, // Define no enumerator (global)
		NULL, // Define no
		(DIGCF_PRESENT | // Only Devices present
		DIGCF_DEVICEINTERFACE)); // Function class devices.
	
	//
	// Take a wild guess at the number of devices we have;
	// Be prepared to realloc and retry if there are more than we guessed
	//
	NumberDevices = 4;
	done = FALSE;
	deviceInfoData.cbSize = sizeof (SP_DEVICE_INTERFACE_DATA);
	
	i=0;
	while (!done) {
		NumberDevices *= 2;
		
		if (*UsbDevices) {
            tempDevDesc = 
				(PTUSB_DEVICE_DESCRIPTOR__1)realloc (*UsbDevices, (NumberDevices * sizeof (TUSB_DEVICE_DESCRIPTOR__1)));
            if(tempDevDesc) {
                *UsbDevices = tempDevDesc;
                tempDevDesc = NULL;
            }
            else {
                free(*UsbDevices);
                *UsbDevices = NULL;
            }
		} else {
			*UsbDevices = (PTUSB_DEVICE_DESCRIPTOR__1)calloc (NumberDevices, sizeof (TUSB_DEVICE_DESCRIPTOR__1));
		}
		
		if (NULL == *UsbDevices) {
			
			// SetupDiDestroyDeviceInfoList destroys a device information set
			// and frees all associated memory.
			
			SetupDiDestroyDeviceInfoList (hardwareDeviceInfo);
			return FALSE;
		}
		
		usbDeviceInst = *UsbDevices + i;
		
		for (; i < NumberDevices; i++) {
			
			// SetupDiEnumDeviceInterfaces() returns information about device interfaces
			// exposed by one or more devices. Each call returns information about one interface;
			// the routine can be called repeatedly to get information about several interfaces
			// exposed by one or more devices.
			
			if (SetupDiEnumDeviceInterfaces (hardwareDeviceInfo,
				0, // We don't care about specific PDOs
				pGuid,
				i,
				&deviceInfoData)) {
				
				if ( GetOneDevName (hardwareDeviceInfo, &deviceInfoData, devName) ) {
					done = TRUE;
					break;
				}
			} else {
				if (ERROR_NO_MORE_ITEMS == GetLastError()) {
					done = TRUE;
					break;
				}
			}
		}
	}
	
	NumberDevices = i;
	
	// SetupDiDestroyDeviceInfoList() destroys a device information set
	// and frees all associated memory.
	
	SetupDiDestroyDeviceInfoList (hardwareDeviceInfo);
	free ( *UsbDevices );

	return TRUE;

}

BOOL CUsbTestChannel::GetOneDevName(HDEVINFO HardwareDeviceInfo, PSP_DEVICE_INTERFACE_DATA DeviceInfoData, char *devName)
{
	PSP_DEVICE_INTERFACE_DETAIL_DATA     functionClassDeviceData = NULL;
	ULONG                                predictedLength = 0;
	ULONG                                requiredLength = 0;
	
	//
	// allocate a function class device data structure to receive the
	// goods about this particular device.
	//
	SetupDiGetDeviceInterfaceDetail (
		HardwareDeviceInfo,
		DeviceInfoData,
		NULL, // probing so no output buffer yet
		0, // probing so output buffer length of zero
		&requiredLength,
		NULL); // not interested in the specific dev-node
	
	
    predictedLength = requiredLength;
    // sizeof (SP_FNCLASS_DEVICE_DATA) + 512;
	
    functionClassDeviceData = (PSP_DEVICE_INTERFACE_DETAIL_DATA)malloc (predictedLength);
    if(NULL == functionClassDeviceData) {
        return FALSE;
    }
    functionClassDeviceData->cbSize = sizeof (SP_DEVICE_INTERFACE_DETAIL_DATA);
	
    //
    // Retrieve the information from Plug and Play.
    //
    if (! SetupDiGetDeviceInterfaceDetail (
		HardwareDeviceInfo,
		DeviceInfoData,
		functionClassDeviceData,
		predictedLength,
		&requiredLength,
		NULL)) {
		free( functionClassDeviceData );
        return FALSE;
    }
	
		strcpy( devName,functionClassDeviceData->DevicePath) ;
	
		free( functionClassDeviceData );

		return TRUE;
}


/************************************************************************************************
*
* Class CSerTestChannel
*
************************************************************************************************/
CSerTestChannel::CSerTestChannel()
{
	this->Init();
}

CSerTestChannel::~CSerTestChannel()
{
	if(this->IsOpened())
		this->Close();
}

void CSerTestChannel::Init(void)
{
	this->m_Port = 1;
	this->m_baud = CBR_115200;
	this->m_Hcomm = NULL;
}

void CSerTestChannel::Close(void)
{
	this->CloseUart();
}

void CSerTestChannel::CloseUart(void)
{
	if(this->m_Hcomm)
	{
		SetCommMask(this->m_Hcomm, 0);
		EscapeCommFunction(this->m_Hcomm, CLRDTR ) ;
		PurgeComm( this->m_Hcomm, PURGE_TXABORT | PURGE_RXABORT |
									PURGE_TXCLEAR | PURGE_RXCLEAR ) ;
		CloseHandle( this->m_Hcomm );
	}
	this->m_Hcomm = NULL;
}

void CSerTestChannel::Open(void)
{
	char str[20];

	if(this->IsOpened())
		return;
	//this->Close();
	sprintf(str, "COM%d", this->m_Port);
	this->m_Hcomm = CreateFile( str,  
                    GENERIC_READ | GENERIC_WRITE, 
                    0, 
                    NULL, 
                    OPEN_EXISTING,
                    FILE_ATTRIBUTE_NORMAL | FILE_FLAG_OVERLAPPED,
                    NULL );
	if ( INVALID_HANDLE_VALUE == this->m_Hcomm )
	{
		this->m_Hcomm = NULL;
		return;
	}
	SetCommMask(this->m_Hcomm, EV_BREAK|EV_ERR|EV_RXCHAR);
	SetupComm(this->m_Hcomm, UART_INQUEUE_SIZE, UART_OUTQUEUE_SIZE);
	PurgeComm(this->m_Hcomm, PURGE_TXABORT | PURGE_RXABORT | PURGE_TXCLEAR | PURGE_RXCLEAR);
	this->SetupHandShake();
	//this->SetupBaud(UART_DEFAULTSET);
	this->SetupBaud(this->m_baud);
	this->SetupTimesout();

}

int	CSerTestChannel::IsOpened(void)
{
	if(this->m_Hcomm)
		return TRUE;
	return FALSE;
}

void CSerTestChannel::SetPort(int port)
{
	if(this->m_Port == port)
		return;
	if(this->IsOpened())
	{
		this->Close();
		this->m_Port = port;
		this->Open();
	}
	else
		this->m_Port = port;
}

void CSerTestChannel::SetupHandShake(void)
{
	DCB dcb;

	if(this->IsOpened() == FALSE)
		return;
	if (!GetCommState(this->m_Hcomm, &dcb))
		return;
	dcb.fOutxCtsFlow = false;					// Disable CTS monitoring
	dcb.fOutxDsrFlow = false;					// Disable DSR monitoring
	dcb.fDtrControl = DTR_CONTROL_DISABLE;		// Disable DTR monitoring
	dcb.fOutX = false;							// Disable XON/XOFF for transmission
	dcb.fInX = false;							// Disable XON/XOFF for receiving
	dcb.fRtsControl = RTS_CONTROL_DISABLE;		// Disable RTS (Ready To Send)
	SetCommState(this->m_Hcomm, &dcb);
}

void CSerTestChannel::SetupTimesout(void)
{
	COMMTIMEOUTS  CommTimeOuts ;

	if(this->IsOpened() == FALSE)
		return;
	if (!GetCommTimeouts(this->m_Hcomm,&CommTimeOuts))
		return;
	CommTimeOuts.ReadIntervalTimeout = 20;//MAXDWORD;//0xFFFFFFFF ;
	CommTimeOuts.ReadTotalTimeoutMultiplier = 10;//MAXDWORD;//0 ;
	CommTimeOuts.ReadTotalTimeoutConstant = 100;//MAXDWORD-1;//1000 ;
	CommTimeOuts.WriteTotalTimeoutMultiplier = 10;//2*CBR_9600/DBUG_BAUD ;
	CommTimeOuts.WriteTotalTimeoutConstant = 100 ;
	SetCommTimeouts(this->m_Hcomm, &CommTimeOuts);
}

int CSerTestChannel::GetPort(void)
{
	return this->m_Port;
}

void CSerTestChannel::SetupBaud(char *setting)
{
	DCB dcb;

	sscanf(setting, "%d", &this->m_baud);
	if(this->IsOpened() == FALSE)
		return;
	memset( &dcb, 0, sizeof(DCB) );
	dcb.DCBlength = sizeof(DCB);
	if (!BuildCommDCB(setting, &dcb))    
		return;
	SetCommState(this->m_Hcomm, &dcb);
}

int CSerTestChannel::GetBaud(void)
{
	return this->m_baud;
}

void CSerTestChannel::SetupBaud(int baud, int dnum, int enparity, int stop)
{
	DCB dcb;

	this->m_baud = baud;
	if(this->IsOpened() == FALSE)
		return;

	memset( &dcb, 0, sizeof(dcb) );
	if( !GetCommState(this->m_Hcomm, &dcb) )
		return;
	dcb.BaudRate = baud;
	dcb.ByteSize = (BYTE)dnum;
	dcb.Parity = (BYTE)enparity;
	dcb.StopBits = (BYTE)stop;
//	dcb.fParity  = (enparity != NOPARITY);
	SetCommState(this->m_Hcomm, &dcb);
}

void CSerTestChannel::ManualConfigPort(void)
{
	COMMCONFIG conf;
	char str[20];

	if(this->IsOpened() == FALSE)
		return;
	sprintf(str, "COM%d", this->m_Port);
	CommConfigDialog(str, NULL, &conf);
}

int CSerTestChannel::ReadChannel(char *buf, int size)
{
	DWORD len = 0;
    OVERLAPPED osReader = {0};  // overlapped structure for read operations
    DWORD dwErrorFlags;
    COMSTAT comStat;

	if(this->IsOpened() == FALSE)
		return 0;
    osReader.hEvent = CreateEvent(NULL, TRUE, FALSE, NULL);
    if(osReader.hEvent == NULL)
		return 0;
	ClearCommError(this->m_Hcomm, &dwErrorFlags, &comStat);
	if(comStat.cbInQue)
	{
		size = comStat.cbInQue > (DWORD)size ? size : comStat.cbInQue;
		ReadFile(this->m_Hcomm, buf, size, &len, &osReader);
	}
	CloseHandle(osReader.hEvent);
	return len;
}

int CSerTestChannel::WriteChannel(char *buf, int size)
{
	DWORD len = 0;
	OVERLAPPED osWrite = {0};  // overlapped structure for read operations

	if(this->IsOpened() == FALSE)
		return 0;
	osWrite.hEvent = CreateEvent(NULL, TRUE, FALSE, NULL);
	if(osWrite.hEvent == NULL)
		return 0;

	if( !WriteFile(this->m_Hcomm, buf, size, &len, &osWrite) )
	{
		if(WaitForSingleObject(osWrite.hEvent, INFINITE) == WAIT_OBJECT_0)
			GetOverlappedResult( this->m_Hcomm, &osWrite, (DWORD *)&len, TRUE );
		else
			len = 0;
	}
	CloseHandle(osWrite.hEvent);
	return len;
}


/************************************************************************************************
*
* Class CUsbSerTestChannel
*
************************************************************************************************/


CUsbSerTestChannel::CUsbSerTestChannel(void)
{
	this->m_sel = USB_SER_TST_SER;
}

//==========================================
//
//		general function
//
//===================================
void CUsbSerTestChannel::Open(void)
{
	switch(this->m_sel)
	{
	case USB_SER_TST_SER:
		CSerTestChannel::Open();
		break;
	case USB_SER_TST_USB:
		CUsbTestChannel::Open();
		break;
	default:
		break;
	}
}

int	CUsbSerTestChannel::IsOpened(void)
{
	switch(this->m_sel)
	{
	//case CHANNEL_CMODEL:
	//	return CSimChannel::IsOpened();
	case USB_SER_TST_SER:
		return CSerTestChannel::IsOpened();
	case USB_SER_TST_USB:
		return CUsbTestChannel::IsOpened();
	default:
		break;
	}
	return FALSE;
}

void CUsbSerTestChannel::Close(void)
{
	switch(this->m_sel)
	{
	case USB_SER_TST_SER:
		CSerTestChannel::Close();
		break;
	case USB_SER_TST_USB:
		CUsbTestChannel::Close();
		break;
	default:
		break;
	}
}

void CUsbSerTestChannel::ChannelSelect(int sel)
{
	if(sel > 2)
		sel = USB_SER_TST_SER;
	if(sel == this->m_sel)
		return;
	if(this->IsOpened())
	{
		this->Close();
		this->m_sel = sel;
		this->Open();
	}
	this->m_sel = sel;
}


int	CUsbSerTestChannel::ReadChannel(char *buf, int size)
{
	switch(this->m_sel)
	{
	case USB_SER_TST_SER:
		return CSerTestChannel::ReadChannel(buf, size);
		break;
	case USB_SER_TST_USB:
		return CUsbTestChannel::ReadChannel(buf, size);
		break;
	default:
		return 0;
		break;
	}
}

int	CUsbSerTestChannel::WriteChannel(char *buf, int size)
{
	switch(this->m_sel)
	{
	case USB_SER_TST_SER:
		return CSerTestChannel::WriteChannel(buf, size);
		break;
	case USB_SER_TST_USB:
		return CUsbTestChannel::WriteChannel(buf, size);
		break;
	default:
		return 0;
		break;
	}
}

#endif //V578CMD_DISABLE



