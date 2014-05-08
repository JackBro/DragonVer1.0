
#include "internal.h"


CBitFifo::CBitFifo(int size, int order) : CBitStream(size, order)
{
	this->ClearPoint();
}

void CBitFifo::ClearPoint(void)
{
	this->m_rpoint = 0;
	this->m_wpoint = 0;
	this->m_valid = 0;
}

void CBitFifo::SetSize(int size)
{
	CBitStream::SetSize(size+sizeof(int));
	this->ClearPoint();
}

void CBitFifo::SetSize(char *buf, int size)
{
	ASSERT(size < sizeof(int));
	CBitStream::SetSize(buf, size-sizeof(int));
	this->ClearPoint();
}

void CBitFifo::ValidPoint(void)
{
	this->m_valid = 1;
}

void CBitFifo::SetReadPoint(int x)
{
	this->m_rpoint = x;
	this->m_valid = 1;
}

void CBitFifo::SetWritePoint(int x)
{
	this->m_wpoint = x;
	this->m_valid = 1;
}

int	CBitFifo::GetReadPoint(void)
{
	return this->m_rpoint;
}

int CBitFifo::GetWritePoint(void)
{
	return this->m_wpoint;
}

int	CBitFifo::GetDataSize(void)
{
	int len;

	if(this->m_valid == 0)
		return 0;
	if(this->m_wpoint == this->m_rpoint)
		return this->m_totalsize << 3;
	len = this->m_totalsize << 3;
	len += this->m_wpoint;
	len -= this->m_rpoint;
	len %= this->m_totalsize << 3;
	return len;
}

int	CBitFifo::GetFreeSize(void)
{
	return (this->m_totalsize << 3) - this->GetDataSize();
}

void CBitFifo::wAlignBits(int align)
{
	int len, val = 0;

	if(this->m_valid == 0)
		return;
	len = this->m_wpoint;
	len %= align;
	this->Write(val, len);
}

void CBitFifo::rAlignBits(int align)
{
	int len, val = 0;

	if(this->m_valid == 0)
		return;
	len = this->m_rpoint;
	len %= align;
	if(len != 0)
		this->Read(val, align-len);
}

int CBitFifo::IsFifoEmpty(void)
{
	if(this->m_valid)
		return 0;
	return 1;
}

int	CBitFifo::Write(char *buf, int bitlen, int offset)
{
	int len;

	if(bitlen == 0)
		return 0;
	if(this->m_valid == 0)
	{
		this->m_rpoint = 0;
		this->m_wpoint = CBitStream::Write(buf, bitlen, offset);
		len = this->m_wpoint;
		this->m_valid = 1;
		this->m_wpoint %= this->m_totalsize << 3;
		return len;
	}
	if(this->m_wpoint == this->m_rpoint)
		return 0;

	if(this->m_wpoint < this->m_rpoint)
	{
		//len = this->m_rpoint - this->m_wpoint;
		//len -= len % 8;
		//len = (offset == 0) ? len : len - offset;
		//bitlen = bitlen < len ? bitlen : len;
		len = CBitStream::Write(buf, bitlen, offset, this->m_wpoint, this->m_rpoint);
		this->m_wpoint += len;
	}
	else
	{
		len = CBitStream::Write(buf, bitlen, offset, this->m_wpoint);
		bitlen -= len;
		offset += len;
		this->m_wpoint += len;
		if(bitlen > 0)
		{
			this->m_wpoint = CBitStream::Write(buf, bitlen, offset, 0, this->m_rpoint);
			len += this->m_wpoint;
		}
	}
	this->m_wpoint %= this->m_totalsize << 3;
	return len;
}

int	CBitFifo::Write(int val, int bitlen)
{
	int len;

	if(bitlen == 0)
		return 0;
	if(this->m_valid == 0)
	{
		this->m_rpoint = 0;
		bitlen = CBitStream::Write(val, bitlen);
		this->m_valid = 1;
		this->m_wpoint = bitlen % this->m_totalsize << 3;
		return bitlen;
	}
	if(this->m_wpoint == this->m_rpoint)
		return 0;

	if(this->m_wpoint < this->m_rpoint)
	{
		if(this->m_wpoint + bitlen > this->m_rpoint)
			return 0;
		len = CBitStream::Write(val, bitlen, this->m_wpoint);
		this->m_wpoint += len;
	}
	else
	{
		if( (this->m_wpoint + bitlen > (this->m_totalsize << 3)) &&
			( ((this->m_wpoint + bitlen) % (this->m_totalsize << 3)) > this->m_rpoint ) )
			return 0;
		len = CBitStream::Write(val, bitlen, this->m_wpoint);
		this->m_wpoint += len;
	}
	this->m_wpoint %= this->m_totalsize << 3;
	return len;
}

int	CBitFifo::Read(int val, int bitlen, int vsize)
{
	if(bitlen == 0)
		return val;
	if(this->m_valid == 0)
		return val;
	if(this->m_wpoint > this->m_rpoint)
	{
		if(this->m_wpoint < this->m_rpoint + bitlen)
			return val;
		val = CBitStream::Read(val, bitlen, vsize, this->m_rpoint);
		this->m_rpoint += bitlen;
	}
	else
	{
		if( (this->m_rpoint + bitlen > (this->m_totalsize << 3)) &&
			( ((this->m_rpoint + bitlen) % (this->m_totalsize << 3)) > this->m_wpoint ) )
			return val;
		val = CBitStream::Read(val, bitlen, vsize, this->m_rpoint);
		this->m_rpoint += bitlen;
	}
	this->m_rpoint %= this->m_totalsize << 3;
	if(this->m_wpoint == this->m_rpoint)
		this->m_valid = 0;
	return val;
}

int	CBitFifo::Read(int bitlen)
{
	int val = 0;

	return this->Read(val, bitlen);
}

int	CBitFifo::Query(int bitlen)
{
	int pt, val = 0;

	//if(this->m_valid == 0)
	//	return 0;
	pt = this->m_rpoint;
	val = this->Read(val, bitlen);
	this->m_rpoint = pt;
	return val;
}

int	CBitFifo::Read(char *buf, int bitlen, int offset)
{
	int len;

	if(bitlen == 0)
		return 0;
	if(this->m_valid == 0)
		return 0;
	if(this->m_wpoint > this->m_rpoint)
	{
		len = CBitStream::Read(buf, bitlen, offset, this->m_rpoint, this->m_wpoint);
		this->m_rpoint += len;
	}
	else
	{
		len = CBitStream::Read(buf, bitlen, offset, this->m_rpoint);
		bitlen -= len;
		this->m_rpoint += len;
		offset += len;
		if(bitlen)
		{
			this->m_rpoint = CBitStream::Read(buf, bitlen, offset, 0, this->m_wpoint);
			len += this->m_rpoint;
		}
	}
	if(this->m_wpoint == this->m_rpoint)
		this->m_valid = 0;
	return len;
}

