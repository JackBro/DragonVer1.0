#include "internal.h"



CUartChannel::CUartChannel()
{
	this->Init();
}

CUartChannel::~CUartChannel()
{
	if(this->IsOpened())
		this->Close();
}

void CUartChannel::Init(void)
{
	this->m_Port = 1;
	this->m_baud = CBR_115200;
	this->m_Hcomm = NULL;
}

void CUartChannel::Close(void)
{
	CUartUsbBaseChannel::Close();
	this->CloseUart();
}

void CUartChannel::CloseUart(void)
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

void CUartChannel::Open(void)
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

	CUartUsbBaseChannel::Open();
}

int	CUartChannel::IsOpened(void)
{
	if(this->m_Hcomm)
		return TRUE;
	return FALSE;
}

void CUartChannel::SetPort(int port)
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

void CUartChannel::SetupHandShake(void)
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

void CUartChannel::SetupTimesout(void)
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

int CUartChannel::GetPort(void)
{
	return this->m_Port;
}

void CUartChannel::SetupBaud(char *setting)
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

int CUartChannel::GetBaud(void)
{
	return this->m_baud;
}

void CUartChannel::SetupBaud(int baud, int dnum, int enparity, int stop)
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

void CUartChannel::ManualConfigPort(void)
{
	COMMCONFIG conf;
	char str[20];

	if(this->IsOpened() == FALSE)
		return;
	sprintf(str, "COM%d", this->m_Port);
	CommConfigDialog(str, NULL, &conf);
}

int CUartChannel::Read(char *buf, int size)
{
	return CUartUsbBaseChannel::Read(buf, size);
}

int CUartChannel::Write(char *buf, int size)
{
	return CUartUsbBaseChannel::Write(buf, size);
}

int CUartChannel::ReadChannel(char *buf, int size)
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

int CUartChannel::WriteChannel(char *buf, int size)
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

