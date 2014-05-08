#include "internal.h"

CSimRegSram *gpRegSram = NULL;

#define CONFIG_FILE "config.txt"

CSimRegSram::CSimRegSram(void) // : m_pc(0x2000, 0x400), m_host(0x2000, 0x400)
{
	gpRegSram = this;
	this->m_status = SIMREG_NOTINITED;
}

CSimRegSram::~CSimRegSram(void)
{
	this->Close();
}

void CSimRegSram::Open(void)
{
	this->Close();
	ShellOpen();
	this->m_status = SIMREG_INITED;
	return;
}

void CSimRegSram::Close(void)
{
	ShellClose();
	this->m_status = SIMREG_NOTINITED;
}

int CSimRegSram::IsOpened(void)
{
	if(this->m_status)
		return 0;
	return 1;
}

int CSimRegSram::Write(char *buf, int len)
{
	int size;

	if(this->m_status)
		return 0;

	size = this->m_host.Write(buf, len);
	Shell();
	return size;
}

int CSimRegSram::Read(char *buf, int len)
{
	if(this->m_status)
		return 0;

	return this->m_pc.Read(buf, len);
}

int	CSimRegSram::hostWriteData(char *buf, int len)
{
	if(this->m_status)
		return 0;

	len = this->m_pc.Write(buf, len);
	if(this->m_pcall)
		this->m_pcall();
	return len;
}

int	CSimRegSram::hostReadData(char *buf, int len)
{
	if(this->m_status)
		return 0;

	return this->m_host.Read(buf, len);
}


