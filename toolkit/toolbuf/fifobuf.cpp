
#include "internal.h"

CFifoBuf::CFifoBuf(int size, int unit) : CSramBuf(size, unit)
{
	this->ClearPoint();
}

CFifoBuf::~CFifoBuf()
{
}

void CFifoBuf::ClearPoint(void)
{
	this->m_rpoint = 0;
	this->m_wpoint = 0;
	this->m_valid = 0;

	this->m_temprp = 0;
	this->m_tempvalid = 0;
	this->m_tempwp = 0;
}

void CFifoBuf::SetSize(int size, int unit)
{
	CSramBuf::SetSize(size, unit);
	this->ClearPoint();
}

void CFifoBuf::SetSize(char *buf, int size, int unit)
{
	CSramBuf::SetSize(buf, size, unit);
	this->ClearPoint();
}

void CFifoBuf::ValidPoint(void)
{
	this->m_valid = 1;
}

int	CFifoBuf::Write(char *buf, int size)
{
	int len;
	char *pbuf = buf;

	if(size == 0)
		return 0;
	if(this->m_valid == 0)
	{
		this->m_rpoint = 0;
		this->m_wpoint = CSramBuf::Write(buf, size, 0);
		len = this->m_wpoint; 
		this->m_valid = 1;
		this->m_wpoint %= this->m_totalsize;
		return len;
	}
	if(this->m_wpoint == this->m_rpoint)
		return 0;

	if(this->m_wpoint < this->m_rpoint)
	{
		len = CSramBuf::Write(buf, size, this->m_wpoint, this->m_rpoint);
		this->m_wpoint += len;
	}
	else
	{
		len = CSramBuf::Write(pbuf, size, this->m_wpoint);
		size -= len;
		this->m_wpoint += len;
		pbuf += len;
		if(size)
		{
			this->m_wpoint = CSramBuf::Write(pbuf, size, 0, this->m_rpoint);
			len += this->m_wpoint;
		}
	}
	this->m_wpoint %= this->m_totalsize;
	return len;
}

void CFifoBuf::SetReadPoint(int x)
{
	this->m_rpoint = x;
	this->m_rpoint %= this->m_totalsize;
	this->m_valid = 1;
}

void CFifoBuf::SetWritePoint(int x)
{
	this->m_wpoint = x;
	this->m_wpoint %= this->m_totalsize;
	this->m_valid = 1;
}

int	CFifoBuf::GetReadPoint(void)
{
	return this->m_rpoint;
}

int CFifoBuf::GetWritePoint(void)
{
	return this->m_wpoint;
}

int	CFifoBuf::GetDataSize(void)
{
	int size;

	if(this->m_valid == 0)
		return 0;
	size = this->GetTotalSize();
	if(this->m_wpoint == this->m_rpoint)
		return size;
	return (this->m_wpoint + size - this->m_rpoint) % size;
}

int	CFifoBuf::GetFreeSize(void)
{
	return this->GetTotalSize() - this->GetDataSize();
}

int	CFifoBuf::Read(char *buf, int size)
{
	int len;
	char *pbuf = buf;

	if(size == 0)
		return 0;
	if(this->m_valid == 0)
		return 0;
	if(this->m_wpoint > this->m_rpoint)
	{
		len = CSramBuf::Read(buf, size, this->m_rpoint, this->m_wpoint);
		this->m_rpoint += len;
	}
	else
	{
		len = CSramBuf::Read(pbuf, size, this->m_rpoint);
		size -= len;
		this->m_rpoint += len;
		pbuf += len;
		if( (size) && (this->m_wpoint) )
		{
			this->m_rpoint = CSramBuf::Read(pbuf, size, 0, this->m_wpoint);
			len += this->m_rpoint;
		}
	}
	this->m_rpoint %= this->m_totalsize;
	if(this->m_wpoint == this->m_rpoint)
		this->m_valid = 0;
	return len;
}

int CFifoBuf::IsFifoEmpty(void)
{
	if(this->m_valid)
		return 0;
	return 1;
}

void CFifoBuf::PushState(void)
{
	this->m_temprp = this->m_rpoint;
	this->m_tempvalid = this->m_valid;
	this->m_tempwp = this->m_wpoint;
}

void CFifoBuf::PopState(void)
{
	this->m_rpoint = this->m_temprp;
	this->m_valid = this->m_tempvalid;
	this->m_wpoint = this->m_tempwp;
}


