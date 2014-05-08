#include <afx.h>
#include "SimChannel.h"
#include "../../V568Cmodel/cmodelenv/main.h"
#include "../../V568Cmodel/cmodelenv/shell.h"

CSimChannel *gpChannel = NULL;

#define CONFIG_FILE "config.txt"

CSimChannel::CSimChannel(void)// : m_pc(0x2000, 0x400), m_host(0x2000, 0x400)
{
	gpChannel = this;
	this->m_hCmodel = NULL;
	this->m_status = SIMCMD_NOTINITED;
}

CSimChannel::~CSimChannel(void)
{
	this->Close();
}

void CSimChannel::Open(void)
{
	DWORD dwSimId;

	this->Close();

	this->m_hCmodel = CreateThread( NULL, 
                          0,
                          (LPTHREAD_START_ROUTINE) OpenCmodel,
                          CONFIG_FILE, 
                          0, 
                          &dwSimId);
	if(this->m_hCmodel == NULL)
		return;

	this->m_status = SIMCMD_INITED;
	return;
}

void CSimChannel::Close(void)
{
	int i = 4, ret;
	if( this->m_hCmodel == NULL )
		return;
	while(i--)
	{
		CloseCmodel();
		ret = WaitForSingleObject(this->m_hCmodel, 2000);
		if(ret == WAIT_OBJECT_0)
			break;
	}
	this->m_hCmodel = NULL;
	this->m_status = SIMCMD_NOTINITED;
}

int CSimChannel::IsOpened(void)
{
	if(this->m_status)
		return 0;
	return 1;
}

int CSimChannel::Write(char *buf, int len)
{
	int size;

	if(this->m_status)
		return 0;

	size = this->m_host.Write(buf, len);
	shell();
	return size;
}

int CSimChannel::Read(char *buf, int len)
{
	if(this->m_status)
		return 0;

	return this->m_pc.Read(buf, len);
}

int	CSimChannel::hostWriteData(char *buf, int len)
{
	if(this->m_status)
		return 0;

	len = this->m_pc.Write(buf, len);
	if(this->m_pcall)
		this->m_pcall();
	return len;
}

int	CSimChannel::hostReadData(char *buf, int len)
{
	if(this->m_status)
		return 0;

	return this->m_host.Read(buf, len);
}


