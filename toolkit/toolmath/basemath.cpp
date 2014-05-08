
#include "internal.h"

CBaseMath::CBaseMath()
{
}

CBaseMath::~CBaseMath()
{
}

int CBaseMath::addsub(char *src1, char *src2, char *dst, int n1, int n2, int n3, int mode)
{
	int i, n, val = 0, val1, val2;

	if(n3 == 0)
		n3 = (n1 > n2) ? (n1+1) : (n2+1);
	n = (n1 > n2) ? n1 : n2;
	if(n > n3)
		n = n3;
	for(i = 0; i < n; i++)
	{
		val >>= 8;
		val1 = (i < n1) ? (unsigned char)src1[i] : 0;
		val2 = (i < n2) ? (unsigned char)src2[i] : 0;
		if(mode == ADD_OPCODE)
			val += val1 + val2;
		else if(mode == SUB_OPCODE)
			val += val1 - val2;
		dst[i] = (char)val;
	}
	if(dst[n-1] & 0x80)
	{
		for(i = n; i < n3; i++)
			dst[i] = -1;
		return NEGATIVE_DATA;
	}
	for(i = n; i < n3; i++)
		dst[i] = 0;
	return POSITIVE_DATA;
}

int CBaseMath::Add(char *src1, char *src2, char *dst, int n1, int n2, int n3)
{
	return this->addsub(src1, src2, dst, n1, n2, n3);
}

int CBaseMath::Sub(char *src1, char *src2, char *dst, int n1, int n2, int n3)
{
	return this->addsub(src1, src2, dst, n1, n2, n3, SUB_OPCODE);
}

void CBaseMath::Mul(char *src1, char *src2, char *dst, int n1, int n2, int n3)
{
	int i, j, val, k;

	if(n3 == 0)
		n3 = n1 + n2;
	for(i = 0; i < n3; i++)
		dst[i] = 0;
	for(i = 0; (i < n1) && (i < n3); i++)
	{
		for(j = 0; (j < n2) && ((i+j) < n3); j++)
		{
			val = (unsigned char)src1[i] * (unsigned char)src2[j];
			for(k = i+j; k < n3; k++)
			{
				val += (unsigned char)dst[k];
				dst[k] = (char)val;
				val >>= 8;
				if(val == 0)
					break;
			}
		}
	}
}

int CBaseMath::Abs(char *src, char *dst, int n1, int n2)
{
	int i;

	if(n2 == 0)
		n2 = n1;
	if( ((src[n1-1] & 0x80) == 0) || (n1 > n2) )
	{
		n1 = (n2 > n1) ? n1 : n2;
		memcpy(dst, src, n1);
		while(n1 < n2)
		{
			dst[n1] = 0;
			n1++;
		}
		return POSITIVE_DATA;
	}
	for(i = 0; i < n1; i++)
		dst[i] = ~src[i];
	while(n1 < n2)
	{
		dst[n1] = 0;
		n1++;
	}
	i = 1;
	this->Add(dst, (char *)&i, dst, n2, 1, n2);
	return NEGATIVE_DATA;
}

int	CBaseMath::getDataLen(char *src, int len)
{
	while( (len > 0) && (src[len-1] == 0) )
		len--;

	return len;
}

void CBaseMath::Div(char *src1, char *src2, char *result, char *res, 
					   int n1, int n2, int n3, int n4)
{
	int i, sign, val = 0, val1;
	char sbuf1[MATH_MAXCOUNT], sbuf2[MATH_MAXCOUNT];
	char sbuf3[MATH_MAXCOUNT], sbuf4[MATH_MAXCOUNT];

	sign = this->Abs(src2, sbuf2, n2) - this->Abs(src1, sbuf1, n1);
	if(n3 == 0)
		n3 = n1;
	if(n4 == 0)
		n4 = n2;
	n1 = this->getDataLen(sbuf1, n1);
	n2 = this->getDataLen(sbuf2, n2);
	//ASSERT(n2);
	if(n2 == 0)
	{
		memset(res, 0, n4);
		memset(res, 0, n3);
		memcpy(result, src1, (n1 > n3) ? n3 : n1);
		return;
	}
	memset(result, 0, n3);
	memset(res, 0, n4);
	memset(sbuf3, 0, MATH_MAXCOUNT);
	memset(sbuf4, 0, MATH_MAXCOUNT);
	for(i = n1 - n2; i >= 0; i--)
	{
		val <<= 8;
		val += (unsigned char)sbuf1[i+n2-1];
		val1 = (unsigned char)sbuf2[n2-1];
		result[i] = (char)(val / val1);
		if(result[i] == 0)
		{
			val = (unsigned char)sbuf1[i+n2-1];
			continue;
		}
		this->Mul(sbuf2, result+i, sbuf3, n2, 1, n1-i);
		if(this->Sub(sbuf1+i, sbuf3, sbuf4, n1-i, n1-i, n1-i) == NEGATIVE_DATA)
		{
			result[i]--;
			if(result[i] == 0)
			{
				val = (unsigned char)sbuf1[i+n2-1];
				continue;
			}
			this->Mul(sbuf2, result+i, sbuf3, n2, 1, n1-i);
			this->Sub(sbuf1+i, sbuf3, sbuf1+i, n1-i, n1-i, n1-i);
		}
		else
			memcpy(sbuf1+i, sbuf4, n1-i);
		val = (unsigned char)sbuf1[i+n2-1];
	}
	i = 0;
	if(sign)
	{
		this->Sub((char *)&i, result, result, 1, n3, n3);
		this->Sub((char *)&i, sbuf1, res, 1, n4, n4);
	}
	else
		this->Add(sbuf1, (char *)&i, res, n4, 1, n4);
}

void CBaseMath::Val2DecStr(char *src, int n, char *str)
{
	int i = 0, k, val = 10, n1;
	char *psrc[2], sbuf[MATH_MAXCOUNT];

	psrc[0] = src;
	psrc[1] = sbuf;
	str[0] = 0;
	n1 = this->getDataLen(src, n);
	while(n1)
	{
		this->Div(psrc[i % 2], (char *)&val, psrc[(i+1) % 2], (char *)&k, n, 4, n, 4);
		memcpy(str+1, str, i+1);
		str[0] = (char)(0x30 + k);
		i++;
		n1 = this->getDataLen(psrc[i%2], n);
	}
}


