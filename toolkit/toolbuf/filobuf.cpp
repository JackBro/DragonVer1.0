
#include "internal.h"

CFiloBuf::CFiloBuf(int size, int unit) : CSramBuf(size, unit)
{
	this->ClearPoint();
}

CFiloBuf::~CFiloBuf()
{
}

void CFiloBuf::ClearPoint(void)
{
	this->m_wpoint = 0;
}

int	CFiloBuf::Write(char *buf, int size)
{
	int len;

	len = CSramBuf::Write(buf, size, this->m_wpoint);
	this->m_wpoint += len;
	return len;
}

int	CFiloBuf::Write(int *buf, int size)
{
	return CFiloBuf::Write( (char *)buf, size << 2);
}

int	CFiloBuf::Write(short int *buf, int size)
{
	return CFiloBuf::Write( (char *)buf, size << 1);
}

int	CFiloBuf::Read(char *buf, int size)
{
	//int i;

	//size = size > this->m_wpoint ? this->m_wpoint : size;
	//for(i = 0; i < size; i++, this->m_wpoint--)
	//	CSramBuf::Read(buf+i, 1, this->m_wpoint - 1);
	//return size;
	return this->Read(buf, size, 1);
}

int	CFiloBuf::Read(char *buf, int size, int bw)
{
	int i;

	if( (bw <= 0) || (bw > 4) )
		bw = 1;
	size = size > this->m_wpoint ? this->m_wpoint : size;
	for(i = 0; i < size; i += bw, this->m_wpoint -= bw)
		CSramBuf::Read(buf+i, bw, this->m_wpoint - bw);

	return i;
}

int	CFiloBuf::Read(int *buf, int size)
{
	return this->Read( (char *)buf, size << 2, sizeof(int) );
}

int	CFiloBuf::Read(short int *buf, int size)
{
	return this->Read( (char *)buf, size << 1, sizeof(short int) );
}

