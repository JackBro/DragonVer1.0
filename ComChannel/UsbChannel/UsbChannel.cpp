#include "internal.h"

#define OVER_LAP_USB

#define WaitRdSemp() {if(this->m_hrdchanelsemp != NULL) \
						::WaitForSingleObject(this->m_hrdchanelsemp, INFINITE);}

#define ReleaseRdSemp() {if(this->m_hrdchanelsemp != NULL) \
						::ReleaseSemaphore(this->m_hrdchanelsemp, 1, NULL);}




CUsbChannel::CUsbChannel() : m_rdpool(0x10000)
{
	this->Init();
}

CUsbChannel::~CUsbChannel()
{
	if(this->IsOpened())
		this->Close();
}

void CUsbChannel::Init(void)
{
	this->m_Pipe = 0;
	this->m_HIn = NULL;
	this->m_HOut = NULL;

	m_enrun = 0;
	m_hthrd = NULL;
	this->m_hrdlap = NULL;
	this->m_hwrlap = NULL;

}

void CUsbChannel::Close(void)
{

	CUartUsbBaseChannel::Close();

	this->TerminalThrd();

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

void CUsbChannel::CloseUsb(void)
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

void CUsbChannel::Open()
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

	this->StartThrd();
	CUartUsbBaseChannel::Open();

}

int	CUsbChannel::IsOpened(void)
{
	if(this->m_HIn && this->m_HOut)
		return TRUE;
	return FALSE;
}

int CUsbChannel::GetPipe(void)
{
	return this->m_Pipe;
}

void CUsbChannel::SetPipe(int pipe)
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

int CUsbChannel::Read(char *buf, int size)
{
	return CUartUsbBaseChannel::Read(buf, size);
}

int CUsbChannel::Write(char *buf, int size)
{
	return CUartUsbBaseChannel::Write(buf, size);
}

int CUsbChannel::ReadChannel(char *buf, int size)
{
	return this->m_rdpool.Read(buf, size);
}

int CUsbChannel::WriteChannel(char *buf, int size)
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

void CUsbChannel::StartThrd(void)
{
	DWORD id;

	this->m_enrun = 1;
	this->m_hthrd = ::CreateThread( NULL, 
                          0,
                          (LPTHREAD_START_ROUTINE) UsbThrdProc,
                          (LPVOID)this, 
                          0, 
                          &id);
}

void CUsbChannel::TerminalThrd(void)
{
	int i = 4, ret = 0;

	if(this->m_hthrd == NULL)
		return;

	while(i--)
	{
		this->m_enrun = 0;
		ret = ::WaitForSingleObject(this->m_hthrd, UART_USB_TERMINAL_TIME);
		if(ret == WAIT_OBJECT_0)
			break;
	}
	this->m_enrun = 0;
	this->m_hthrd = NULL;
}

void CUsbChannel::ThrdProc(void)
{
	BOOL success;
	OVERLAPPED ov;
	CBufferBase pool;
	DWORD len = 0;
	DWORD size = (DWORD)pool.GetTotalSize();

	while(this->m_enrun)
	{
		if(this->m_HIn == NULL)
		{
			Sleep(10);
			continue;
		}

		memset(&ov, 0, sizeof(OVERLAPPED));
		ov.hEvent = this->m_hrdlap;
		//::ResetEvent(ov.hEvent);
		len = 0;
		success = ::ReadFile(this->m_HIn, pool.m_buf, size, &len, &ov);
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
				Sleep(10);
				break;
			}
		}
		ResetEvent(ov.hEvent);
		if(!len)
			Sleep(1);

		while(len)
		{
			WaitRdSemp();
			len -= this->m_rdpool.Write(pool.m_buf, len);
			ReleaseRdSemp();
			if(len)
				Sleep(1);
		}

	}
}

DWORD UsbThrdProc(LPVOID *ptr)
{
	((CUsbChannel *)ptr)->ThrdProc();
	return 0;
}


BOOL CUsbChannel::EnumUsbDevName(LPGUID pGuid,  char* devName)
{
	HDEVINFO                 hardwareDeviceInfo;
	ULONG NumberDevices;
	SP_DEVICE_INTERFACE_DATA deviceInfoData;
	PTUSB_DEVICE_DESCRIPTOR	usbDeviceInst;
	PTUSB_DEVICE_DESCRIPTOR	*UsbDevices = &usbDeviceInst;
	PTUSB_DEVICE_DESCRIPTOR	tempDevDesc;
	
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
				(PTUSB_DEVICE_DESCRIPTOR)realloc (*UsbDevices, (NumberDevices * sizeof (TUSB_DEVICE_DESCRIPTOR)));
            if(tempDevDesc) {
                *UsbDevices = tempDevDesc;
                tempDevDesc = NULL;
            }
            else {
                free(*UsbDevices);
                *UsbDevices = NULL;
            }
		} else {
			*UsbDevices = (PTUSB_DEVICE_DESCRIPTOR)calloc (NumberDevices, sizeof (TUSB_DEVICE_DESCRIPTOR));
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

BOOL CUsbChannel::GetOneDevName(HDEVINFO HardwareDeviceInfo, PSP_DEVICE_INTERFACE_DATA DeviceInfoData, char *devName)
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


