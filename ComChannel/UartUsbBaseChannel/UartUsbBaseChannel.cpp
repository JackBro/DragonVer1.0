#include "internal.h"


#define WaitRdSemp(i) {if(this->m_hrdsemp[i] != NULL) \
						::WaitForSingleObject(this->m_hrdsemp[i], INFINITE);}

#define ReleaseRdSemp(i) {if(this->m_hrdsemp[i] != NULL) \
						::ReleaseSemaphore(this->m_hrdsemp[i], 1, NULL);}

#define WaitRdChlSemp() {if(this->m_hrdchanelsemp != NULL) \
						::WaitForSingleObject(this->m_hrdchanelsemp, INFINITE);}

#define ReleaseRdChlSemp() {if(this->m_hrdchanelsemp != NULL) \
						::ReleaseSemaphore(this->m_hrdchanelsemp, 1, NULL);}


CUartUsbBaseChannel::CUartUsbBaseChannel() : m_wpool(0x10000), m_rpool(0x10000)
{
	this->Init();
}

CUartUsbBaseChannel::~CUartUsbBaseChannel()
{
}

void CUartUsbBaseChannel::Init(void)
{
	this->m_hthrd = NULL;

	this->m_enrun = 0;

	this->m_hrdsemp[0] = NULL;
	this->m_hrdsemp[1] = NULL;
	m_currdmode = GetRWmode();
}

void CUartUsbBaseChannel::Open()
{
	this->m_hrdsemp[0] = CreateSemaphore( NULL,   // no security attributes
										1,   // initial count
										1,   // maximum count
										NULL);  // unnamed semaphore
	this->m_hrdsemp[1] = CreateSemaphore( NULL,   // no security attributes
										1,   // initial count
										1,   // maximum count
										NULL);  // unnamed semaphore
	this->m_hrdchanelsemp = CreateSemaphore( NULL,   // no security attributes
										1,   // initial count
										1,   // maximum count
										NULL);  // unnamed semaphore

	this->StartThrd();
}

void CUartUsbBaseChannel::Close(void)
{
	this->TerminalThrd();
	this->m_rpool.ClearPoint();
	this->m_wpool.ClearPoint();
	this->m_enrun = 0;
	if(this->m_hrdsemp[0] != NULL)
	{
		WaitRdSemp(0);
		CloseHandle(this->m_hrdsemp[0]);
		this->m_hrdsemp[0] = NULL;
	}
	if(this->m_hrdsemp[1] != NULL)
	{
		WaitRdSemp(1);
		CloseHandle(this->m_hrdsemp[1]);
		this->m_hrdsemp[1] = NULL;
	}
	if(this->m_hrdchanelsemp != NULL)
	{
		WaitRdChlSemp();
		CloseHandle(this->m_hrdchanelsemp);
		this->m_hrdchanelsemp = NULL;
	}

}

int	CUartUsbBaseChannel::IsOpened(void)
{
	return 0;
}

void CUartUsbBaseChannel::ReadUartUsb(void)
{
	CBufferBase pool;
	int len = 0;
	int size = pool.GetTotalSize();

	if(this->m_hrdsemp[1] != NULL)
	{
		if(WAIT_OBJECT_0 == WaitForSingleObject(this->m_hrdsemp[1], 0))
		{
			len = ReadChannel(pool.m_buf, size);
			ReleaseSemaphore(this->m_hrdsemp[1], 1, NULL);
		}
		else
			return;
	}
	else
	{
		WaitRdChlSemp();
		len = ReadChannel(pool.m_buf, size);
		ReleaseRdChlSemp();
	}
	if(len)
	{
		WaitRdSemp(0);
		this->m_rpool.Write(pool.m_buf, len);
		ReleaseRdSemp(0);
	}

}

void CUartUsbBaseChannel::WriteUartUsb(void)
{
	CBufferBase pool(0x10000);
	DWORD len = 0;
	char *pbuf;
	int size = pool.GetTotalSize();

	pbuf = pool.m_buf;
	size = this->m_wpool.Read(pbuf, size);
	while(size)
	{
		len = WriteChannel(pbuf, size);
		size -= len;
		pbuf += len;
	}
}

int CUartUsbBaseChannel::Read(char *buf, int size)
{
	int len;
	this->ReadUartUsb();
	WaitRdSemp(0);
	len = this->m_rpool.Read(buf, size);
	ReleaseRdSemp(0);
	return len;
}

int CUartUsbBaseChannel::Write(char *buf, int size)
{
#if 0 //future maybe used.
	size = this->m_wpool.Write(buf, size);
	this->WriteUartUsb();
	return size;
#else //now we must write current buf directly.
	DWORD	len = 0;
	DWORD	size1;
	
	size = (size > 0x10000) ? 0x10000 : size;
	size1 = size;

	while(size)
	{
		len = WriteChannel(buf, size);
		size -= len;
		buf += len;
	}
	return size1;
#endif
}

void CUartUsbBaseChannel::StartThrd(void)
{
	DWORD id;

	this->m_enrun = 1;
	this->m_hthrd = CreateThread( NULL, 
                          0,
                          (LPTHREAD_START_ROUTINE) UartUsbThrdProc,
                          (LPVOID)this, 
                          0, 
                          &id);
}

void CUartUsbBaseChannel::TerminalThrd(void)
{
	int i = 4, ret = 0;

	if(this->m_hthrd == NULL)
		return;

	while(i--)
	{
		this->m_enrun = 0;
		ret = WaitForSingleObject(this->m_hthrd, UART_USB_TERMINAL_TIME);
		if(ret == WAIT_OBJECT_0)
			break;
	}
	this->m_enrun = 0;
	this->m_hthrd = NULL;
}

void CUartUsbBaseChannel::ThrdProc(void)
{
	while(this->m_enrun)
	{
		this->ReadUartUsb();
		m_incallback = 1;
		this->m_currdmode = this->GetRWmode();
		if((this->m_currdmode == CALLBACK_MODE))
		{
			if( !this->m_rpool.IsFifoEmpty() )
			{
				if(this->m_pcall)
					this->m_pcall();
			}
		}
		m_incallback = 0;
		Sleep(10);
	}
}

DWORD UartUsbThrdProc(LPVOID *ptr)
{
	((CUartUsbBaseChannel *)ptr)->ThrdProc();
	return 0;
}


