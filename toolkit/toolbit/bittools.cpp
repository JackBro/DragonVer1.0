
#include "internal.h"

CBitTools::CBitTools(char chk, char fill, int chken)
{
	this->m_chk = chk;
	this->m_fill = fill;
	this->m_chken = chken;
}

CBitTools::~CBitTools()
{
}

//void CBitTools::SetBitOrder(int order)
//{
//	this->m_order = order;
//}

int CBitTools::CopyBitStream(char *src, char *dst, int bitlen, int dststart, int srcstart)
{
	int i, j, n, val, k, len = bitlen;

	i = dststart >> 3;
	j = dststart % 8;
	n = srcstart % 8;
	if(j)
	{
		dst[i] &= (1 << j) - 1;
		val = *((int *)(src + (srcstart >> 3)));
		dst[i] |= (char)((val >> n) << j);
		i++;
		j = 8 - j;
		srcstart += j;
		bitlen -= j;
		n = srcstart % 8;
	}
	while(bitlen)
	{
		k = (bitlen > 8) ? 8 : bitlen;
		val = *((int *)(src + (srcstart >> 3)));
		dst[i] &= ~((1 << k) - 1);
		dst[i] |= (char)(val >> n);
		i++;
		srcstart += k;
		bitlen -= k;
	}
	return len;
}

void CBitTools::ReverseBitStream(char *src, char *dst, int size, int bitwidth)
{
	int i, j;
	char val1, val2;

	for(i = 0; i < size; i++)
	{
		val1 = src[i];
		val2 = 0;
		if(val1 != 0)
		{
			for(j = 0; j < 8; j += bitwidth, val1 >>= bitwidth)
			{
				val2 <<= bitwidth;
				val2 |= val1 & ((1 << bitwidth) - 1);
			}
		}
		dst[i] = val2;
	}
}

//================

int CBitTools::AdjustAdr(char *dst, int adr, int *bitcur)
{
	if(this->m_chken)
	{
		if(dst[adr++] == this->m_chk)
		{
			dst[adr++] = this->m_fill;
			*bitcur += 8;
		}
	}
	else
		adr++;
	return adr;
}

int CBitTools::PutBits(char *dst, int val, int bitlen, int bitcur)
{
	int cur = bitcur;
	int adr = bitcur >> 3;
	int p = bitcur % 8;
	int len, length = bitlen;

	if(bitlen <= 0)
		return 0;
	if( (p != 0) && (p + bitlen > 7) )
	{
		len = 8 - p;
		dst[adr] &= ~((1 << len) - 1);
		dst[adr] |= (val >> (bitlen - len)) & ((1 << len) - 1);
		adr = this->AdjustAdr(dst, adr, &bitcur);
		bitlen -= len;
		p = 0;
	}
	if(p == 0)
	{
		while(bitlen > 7)
		{
			bitlen -= 8;
			dst[adr] = (char)(val >> bitlen);
			adr = this->AdjustAdr(dst, adr, &bitcur);
		}
	}
	if(bitlen != 0)
	{
		dst[adr] &= ~((1 << (8 - p)) - 1);
		dst[adr] |= ( val & ((1 << bitlen) - 1) ) << (8 - p - bitlen);
	}
	return bitcur + length - cur;
}

int	CBitTools::GetBits(char *src, int curval, int bitlen, int bitpos)
{
	int adr = bitpos >> 3;
	int p = bitpos % 8;
	int len;

	if(bitlen <= 0)
		return curval;
	if( (p != 0) && (p + bitlen > 7) )
	{
		len = 8 - p;
		curval <<= len;
		curval |= src[adr] & ((1 << len) - 1);
		bitlen -= len;
		adr++;
		p = 0;
	}
	if(p == 0)
	{
		while(bitlen > 7)
		{
			bitlen -= 8;
			curval <<= 8;
			curval |= src[adr] & 0xff;
			adr ++;
		}
	}
	if(bitlen != 0)
	{
		curval <<= bitlen;
		curval |= (src[adr] >> (8 - bitlen - p)) & ((1 << bitlen) - 1);
	}
	return curval;
}

int CBitTools::PutBufBits(char *src, char *dst, int bitlen, int dststart, int srcstart)
{
	int val, len1, len2, len = bitlen, pos = dststart;

	if(bitlen <= 0)
		return 0;
	while(len)
	{
		len1 = (len > 32) ? 32 : len;
		val = 0;
		val = this->GetBits(src, val, len1, srcstart);
		srcstart += len1;
		len2 = this->PutBits(dst, val, len1, dststart);
		dststart += len2;
		len -= len1;
	}
	return dststart - pos;
}

int	CBitTools::ValExtent(int val, int bitlen)
{
	if( (val & (1 << (bitlen - 1))) == 0 )
		return val;
	return val | (~(1 << (bitlen - 1)));
}

void CBitTools::SetChk(int chk)
{
	this->m_chk = (char)chk;
}
	
void CBitTools::SetFill(int fill)
{
	this->m_fill = (char)fill;
}

void CBitTools::SetChkEn(int chken)
{
	this->m_chken = chken;
}



