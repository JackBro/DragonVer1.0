
#include "internal.h"


CMemBase::CMemBase(int size, int unit) : CBaseItem()
{
	this->Clear();
	CMemBase::SetSize(size, unit);
}

CMemBase::~CMemBase()
{
	this->release();
}

void CMemBase::Clear(void)
{
	this->m_MemPool = NULL;
	this->m_buf = NULL;
	this->m_unit = 1;
	this->m_size = 0;
	this->m_totalsize = 0;
}

void CMemBase::release(void)
{
	if(this->m_MemPool)
	{
		delete [](this->m_MemPool);
		this->m_buf = NULL;
		this->m_MemPool = NULL;
	}
	this->m_size = 0;
	this->m_totalsize = 0;
}

void CMemBase::SetSize(int size, int unit)
{
	int len;

	unit = (unit < 1) ? 1 : unit;
	len = size * unit;
	if(len == 0)
		return;
	if(unit == this->m_unit)
	{
		if( (this->m_size >= size) && (this->m_MemPool == this->m_buf) )
			return;
	}
	this->release();
	this->m_MemPool = new char[len];
    if(this->m_MemPool == NULL)
		return;
	this->m_unit = unit;
	this->m_size = size;
	this->m_totalsize = len;
	this->m_buf = this->m_MemPool;
}

void CMemBase::SetSize(char *buf, int size, int unit)
{
	unit = (unit < 1) ? 1 : unit;
	this->release();
	this->m_buf = buf;
	this->m_size = size;
	this->m_unit = unit;
	this->m_totalsize = size * unit;
}

int	CMemBase::GetSize(void)
{
	return this->m_size;
}

int CMemBase::GetTotalSize(void)
{
	return this->m_totalsize;
}

int	CMemBase::Write(char *buf, int size)
{
	size = size > this->m_totalsize ? this->m_totalsize : size;
	if( (size <= 0) || (buf == NULL) )
		return 0;
	memcpy(this->m_buf, buf, size);
	return size;
}

int	CMemBase::Read(char *buf, int size)
{
	size = size > this->m_totalsize ? this->m_totalsize : size;
	if( (size <= 0) || (buf == NULL) )
		return 0;
	memcpy(buf, this->m_buf, size);
	return size;
}

//================================================

CBufferBase::CBufferBase(int size, int unit) : CMemBase(size, unit)
{
}

CBufferBase::~CBufferBase()
{
}

