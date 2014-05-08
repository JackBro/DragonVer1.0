
#include "internal.h"

CFileBin::CFileBin(char *filename, char *rwmode, int size) 
	: CFileBase(filename, rwmode), CBufferBase(size)
{
	this->m_wpoint = 0;
	this->m_rpoint = 0;
}

CFileBin::~CFileBin()
{
	this->Close();
}

void CFileBin::Close(void)
{
	if(this->m_wpoint)
		CFileBase::Write(this->m_buf, this->m_wpoint);
	this->m_wpoint = 0;
	CFileBase::Close();
}

void CFileBin::Open(char *filename, char *rwmode, int size)
{
	this->Close();
	this->ClearPoint();
	CFileBase::Open(filename, rwmode);
	CBufferBase::SetSize(size);
}

void CFileBin::ClearPoint(void)
{
	this->m_wpoint = 0;
	this->m_rpoint = 0;
}

int CFileBin::Read(char *buf, int len)
{
	int size, flag = 1;
	int leng, length = 0;
	char *pbuf = buf;

	if(buf == NULL)
		return 0;
	if(len <= 0)
		return 0;

	while(flag)
	{
		size = this->m_wpoint - this->m_rpoint;
		if(size == 0)
		{
			if(len >= this->m_totalsize)
			{
				leng = CFileBase::Read(pbuf, len);
				return leng+length;
			}
			leng = CFileBase::Read(this->m_buf, this->m_totalsize);
			if(leng == 0)
				return length;
			this->m_wpoint = leng;
			this->m_rpoint = 0;
			size = this->m_wpoint;
		}
		size = size > len ? len : size;
		memcpy(pbuf, this->m_buf+this->m_rpoint, size);
		pbuf += size;
		len -= size;
		this->m_rpoint += size;
		length += size;
		if(len == 0)
			flag = 0;
	}
	return length;
}

void CFileBin::Fseek(long pos, int start)
{
	CFileBase::Write(this->m_buf, this->m_wpoint);
	CFileBase::Fseek(pos, start);
	this->ClearPoint();
}

int CFileBin::Write(char *buf, int len)
{
	int size, flag = 1;
	int leng, length = 0;
	char *pbuf = buf;

	if(buf == NULL)
		return 0;
	if(len <= 0)
		return 0;
	while(flag)
	{
		size = this->m_totalsize - this->m_wpoint;
		if(size == 0)
		{
			leng = CFileBase::Write(this->m_buf, this->m_totalsize);
			if(leng == 0)
				return length;
			this->m_wpoint = 0;
			size = this->m_totalsize;
		}
		if((this->m_wpoint == 0) && (len >= this->m_totalsize))
		{
			leng = CFileBase::Write(pbuf, len);
			return leng+length;
		}
		size = size > len ? len : size;
		memcpy(this->m_buf+this->m_wpoint, pbuf, size);
		pbuf += size;
		len -= size;
		this->m_wpoint += size;
		length += size;
		if(len == 0)
			flag = 0;
	}
	return length;
}

