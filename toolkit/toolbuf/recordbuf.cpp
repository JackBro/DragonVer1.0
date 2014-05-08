
#include "internal.h"

CRecordBuf::CRecordBuf(int size, int listlen) : CFifoBuf(size), m_pos(listlen)
{
}

void CRecordBuf::SetSize(int size, int listlen)
{
	CFifoBuf::SetSize(size);
	this->m_pos.SetSize(listlen);
}

void CRecordBuf::SetSize(char *buf, int size, int listlen)
{
	CFifoBuf::SetSize(buf, size);
	this->m_pos.SetSize(listlen);
}

int	CRecordBuf::Write(char *buf, int size)
{
	int len;
	
	len = CFifoBuf::Write(buf, size);
	size = this->m_pos.Write((char *)&len, sizeof(int));
	return len;
}

int CRecordBuf::Read(char *buf, int size)
{
	int len, offset, wp;

	offset = this->m_pos.GetReadPoint();
	this->m_pos.Read( (char *)&len, sizeof(int) );
	if(len > size)
	{
		wp = this->m_pos.GetWritePoint();
		size = CFifoBuf::Read(buf, size);
		this->m_pos.SetReadPoint(offset);
		this->m_pos.SetWritePoint(offset);
		len -= size;
		this->m_pos.Write( (char *)&len, sizeof(int) );
		this->SetWritePoint(wp);
		return size;
	}
	return CFifoBuf::Read(buf, len);
}

void CRecordBuf::ClearPoint(void)
{
	CFifoBuf::ClearPoint();
	this->m_pos.ClearPoint();
}


