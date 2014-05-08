//#include <stdio.h>
//#include <string.h>

#include "internal.h"

CBitStream::CBitStream(int size, int order) : CSramBuf(size, 1)
{
	this->adjustsizeinfo();
	this->m_bitorder = SMALLBIT_ORDER;
	if(order)
		this->m_bitorder = BIGBIT_ORDER;
}

void CBitStream::SetSize(int size)
{
	CBufferBase::SetSize(size, 1);
	this->adjustsizeinfo();
}

void CBitStream::SetSize(char *buf, int size)
{
	CBufferBase::SetSize(buf, size, 1);
	this->adjustsizeinfo();
}

void CBitStream::SetBitOrder(int order)
{
	this->m_bitorder = SMALLBIT_ORDER;
	if(order)
		this->m_bitorder = BIGBIT_ORDER;
}

void CBitStream::adjustsizeinfo(void)
{
	if(this->m_size)
	{
		this->m_size -= 4;
		this->m_totalsize -= 4;
	}
}

int	CBitStream::Write(char *buf, int bitlen, int offset, int wstart, int wend)
{
	int *pstart, *pend;

	if( (wend == 0) || (wend > (this->m_totalsize << 3)) )
		wend = this->m_totalsize << 3;
	if( (bitlen <= 0) || (wstart < 0) || (wstart >= wend) || (buf == NULL) )
		return 0;
	wend -= wstart;
	bitlen = bitlen > wend ? wend : bitlen;
	if(this->m_bitorder)
		bitlen = CBitTools::PutBufBits(buf, this->m_buf, bitlen, wstart, offset);
	else
		bitlen = CBitTools::CopyBitStream(buf, this->m_buf, bitlen, wstart, offset);

	pstart = (int *)(this->m_buf);
	pend = (int *)(this->m_buf + this->m_totalsize);
	*pend = *pstart;
//	memcpy(this->m_buf + this->m_totalsize, this->m_buf, 4);
	return bitlen;
}

int	CBitStream::Read(char *buf, int bitlen, int offset, int rstart, int rend)
{
	if( (rend == 0) || (rend > this->m_totalsize << 3) )
		rend = this->m_totalsize << 3;
	if( (bitlen <= 0) || (rstart < 0) || (rstart >= rend) || (buf == NULL) )
		return 0;
	rend -= rstart;
	bitlen = bitlen > rend ? rend : bitlen;

	if(this->m_bitorder)
		CBitTools::PutBufBits(this->m_buf, buf, bitlen, offset, rstart);
	else
		CBitTools::CopyBitStream(this->m_buf, buf, bitlen, offset, rstart);
	return bitlen;
}

int	CBitStream::Write(int val, int bitlen, int wstart)
{
	if(this->m_bitorder)
		return CBitTools::PutBits(this->m_buf, val, bitlen, wstart);
	return this->Write((char *)&val, bitlen, 0, wstart);
}

int	CBitStream::Read(int val, int bitlen, int vsize, int rstart)
{
	if(this->m_bitorder)
		return CBitTools::GetBits(this->m_buf, val, bitlen, rstart);
	this->Read((char *)&val, bitlen, vsize, rstart);
	return val;
}

