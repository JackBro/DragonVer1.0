
#include "internal.h"

CDctEnc::CDctEnc() : CBASEMATRIX(8, 8), m_dct(8, 8)
{
	this->Init();
}

CDctEnc::~CDctEnc()
{
}

void CDctEnc::Init(void)
{
	int	i, j;
	double sfblk[64];
	double val, pi = 3.1415926535;

	for(i = 0; i < 8; i++)
	{
		for(j = 0; j < 8; j++)
		{
			val = 1 << 6;
			val *= cos( (pi * ((j << 1) + 1.0) * i) / 16.0 );
			if(i == 0)
			{
				val *= sqrt(2.0);
				val /= 2.0;
			}
			sfblk[(i << 3) + j] = val;
		}
	}
	this->m_dct.Write(sfblk, 64);
}

int	CDctEnc::Write(double *buf, int len)
{
	CBASEMATRIX temp(8, 8);

	__super::Write(buf, len);
	*this -= 128;
	temp = this->m_dct;
	temp *= *this;
	this->Equ(temp);
	temp = this->m_dct;
	temp.MRotate(ROT_MIRROR270);
	*this *= temp;
	*this /= 1 << 14;
	return len;
}

void CDctEnc::operator = (CDctEnc &a)
{
	__super::Equ(a);
}

void CDctEnc::operator = (double *a)
{
	__super::Write(a, 64);
}


//==========================
//
//		class CDctEec1
//
//==========================
CDctEnc1::CDctEnc1() : CBASEMATRIX1(8, 8)
{	
}

CDctEnc1::~CDctEnc1()
{
}

int CDctEnc1::Write(int *buf, int len)
{
/*	__super::Write(buf, len);
	*this -= 128;
	__super::Read(buf, len);
*/	
	this->fdct8x8(buf);	
	return len;
}

void CDctEnc1::fdct8x8(int *inblk)
{
	int i;
	int oublk[64];
	int sum07, sum16, sum25, sum34, dif07, dif16, dif25, dif34;
	
	for (i = 0; i < 8; i++)
	{
		sum07 = (inblk[(i << 3) + 0] + inblk[(i << 3) + 7]) << 14;
		sum16 = (inblk[(i << 3) + 1] + inblk[(i << 3) + 6]) << 14;
		sum25 = (inblk[(i << 3) + 2] + inblk[(i << 3) + 5]) << 14;
		sum34 = (inblk[(i << 3) + 3] + inblk[(i << 3) + 4]) << 14;
															   
		dif07 = (inblk[(i << 3) + 0] - inblk[(i << 3) + 7]) << 14;
		dif16 = (inblk[(i << 3) + 1] - inblk[(i << 3) + 6]) << 14;
		dif25 = (inblk[(i << 3) + 2] - inblk[(i << 3) + 5]) << 14;
		dif34 = (inblk[(i << 3) + 3] - inblk[(i << 3) + 4]) << 14;

		oublk[(i << 3) + 0]	= this->roundc4(sum07 + sum16 + sum25 + sum34);
		oublk[(i << 3) + 2] = this->roundc2(sum07 - sum34) + this->roundc6(sum16 - sum25);
		oublk[(i << 3) + 4] = this->roundc4(sum07 - sum16 - sum25 + sum34);
		oublk[(i << 3) + 6] = this->roundc6(sum07 - sum34) - this->roundc2(sum16 - sum25);
				   
		oublk[(i << 3) + 1] = this->roundc1(dif07) + this->roundc3(dif16) + this->roundc5(dif25) + this->roundc7(dif34);
		oublk[(i << 3) + 3] = this->roundc3(dif07) - this->roundc7(dif16) - this->roundc1(dif25) - this->roundc5(dif34);
		oublk[(i << 3) + 5] = this->roundc5(dif07) - this->roundc1(dif16) + this->roundc7(dif25) + this->roundc3(dif34);
		oublk[(i << 3) + 7] = this->roundc7(dif07) - this->roundc5(dif16) + this->roundc3(dif25) - this->roundc1(dif34);
	}
	
	for (i = 0; i < 8; i++)
	{
		sum07 = (oublk[i + (0 << 3)] + oublk[i + (7 << 3)]);
		sum16 = (oublk[i + (1 << 3)] + oublk[i + (6 << 3)]);
		sum25 = (oublk[i + (2 << 3)] + oublk[i + (5 << 3)]);
		sum34 = (oublk[i + (3 << 3)] + oublk[i + (4 << 3)]);

		dif07 = (oublk[i + (0 << 3)] - oublk[i + (7 << 3)]);
		dif16 = (oublk[i + (1 << 3)] - oublk[i + (6 << 3)]);
		dif25 = (oublk[i + (2 << 3)] - oublk[i + (5 << 3)]);
		dif34 = (oublk[i + (3 << 3)] - oublk[i + (4 << 3)]);

		oublk[i + (0 << 3)]	= this->roundc4(sum07 + sum16 + sum25 + sum34);		
		oublk[i + (2 << 3)] = this->roundc2(sum07 - sum34) + this->roundc6(sum16 - sum25);
		oublk[i + (4 << 3)] = this->roundc4(sum07 - sum16 - sum25 + sum34);
		oublk[i + (6 << 3)] = this->roundc6(sum07 - sum34) - this->roundc2(sum16 - sum25);

		oublk[i + (1 << 3)] = this->roundc1(dif07) + this->roundc3(dif16) + this->roundc5(dif25) + this->roundc7(dif34);
		oublk[i + (3 << 3)] = this->roundc3(dif07) - this->roundc7(dif16) - this->roundc1(dif25) - this->roundc5(dif34);
		oublk[i + (5 << 3)] = this->roundc5(dif07) - this->roundc1(dif16) + this->roundc7(dif25) + this->roundc3(dif34);
		oublk[i + (7 << 3)] = this->roundc7(dif07) - this->roundc5(dif16) + this->roundc3(dif25) - this->roundc1(dif34);
	}
	
	for (i = 0; i < 64; i++)
	{
		oublk[i] >>= 16;		
		inblk[i] = (oublk[i] < 0) ? (oublk[i] + 1) : oublk[i];
	}
}


/*=================================================
    cos pi/16 = 16069/16384 = 
    (2^14 - 2^8 - 2^6 + 2^2 + 1)/2^14 =
     1 - 1/2^6[1 + 1/2^2(1 - 1/2^4(1 + 1/2^2))]
=================================================*/
int CDctEnc1::roundc1(int input)
{
	int tmp;
	
	tmp = input + (input >> 2);
	tmp = input - (tmp >> 4);
	tmp = input + (tmp >> 2);
	tmp = input - (tmp >> 6);
	
	return tmp;	
}

/*=================================================
    cos 2*pi/16 = 15137/16384 = 
    (2^14 - 2^10 - 2^8 + 2^5 + 1)/2^14 =
    1 - 1/2^4(1 + 1/2^2(1 - 1/2^3(1 + 1/2^5)))
=================================================*/
int CDctEnc1::roundc2(int input)
{
	int tmp;
	
	tmp = input + (input >> 5);
	tmp = input - (tmp >> 3);
	tmp = input + (tmp >> 2);
	tmp = input - (tmp >> 4);
	
	return tmp;
}

/*=================================================
	cos 3*pi/16 = 13623/16384 =
	(2^14 +- 2^11 - 2^9 - 2^7 - 2^6 - 2^3 -1)/2^14 =
	1 - 1/2^3(1 + 1/2^2(1 + 1/2^2(1 + 1/2(1 + 1/2^3(1 + 2^3)))))
=================================================*/
int CDctEnc1::roundc3(int input)
{
	int tmp;
	
	tmp = input + (input >> 3);
	tmp = input + (tmp >> 3);
	tmp = input + (tmp >> 1);
	tmp = input + (tmp >> 2);
	tmp = input + (tmp >> 2);
	tmp = input - (tmp >> 3);	
	
	return tmp;
}

/*=================================================
	cos 4*pi/16 = 11585/16384 = 
	(2^14 - 2^12 - 2^10 + 2^8 + 2^6 + 1)/2^14 =	
	1 - 1/2^2(1 + 1/2^2(1 - 1/2^2(1 + 1/2^2(1 + 1/2^6))))
=================================================*/
int CDctEnc1::roundc4(int input)
{
	int tmp;
	
	tmp = input + (input >> 6);
	tmp = input + (tmp >> 2);
	tmp = input - (tmp >> 2);
	tmp = input + (tmp >> 2);
	tmp = input - (tmp >> 2);		
	
	return tmp;
}

/*=================================================
	cos 5*pi/16 = 9102/16384 = 
	(2^13 + 2^10 - 2^7 + 2^4 - 2)/2^14 =
	1/2*[1 + 1/2^3(1 - 1/2^3(1 - 1/2^3(1 - 1/2^3)))]
=================================================*/
int CDctEnc1::roundc5(int input)
{
	int tmp;
	
	tmp = input - (input >> 3);
	tmp = input - (tmp >> 3);
	tmp = input - (tmp >> 3);
	tmp = input + (tmp >> 3);		
	
	return (tmp >> 1);
}

/*=================================================
	cos 6*pi/16 = 6270/16384 = 
	(2^12 + 2^11 + 2^7 - 2)/2^14 =
	1/2^2*[1 + 1/2(1 + 1/2^4(1 - 1/2^6))]
=================================================*/
int CDctEnc1::roundc6(int input)
{
	int tmp;
	
	tmp = input - (input >> 6);
	tmp = input + (tmp >> 4);
	tmp = input + (tmp >> 1);			
	
	return (tmp >> 2);
}

/*=================================================
	cos 7*pi/16 = 3196/16384 = 
	(2^11 + 2^10 + 2^7 - 2^2)/2^14 =
	1/2^3*[1 + 1/2(1 + 1/2^3(1 - 1/2^5))]
=================================================*/
int CDctEnc1::roundc7(int input)
{
	int tmp;
	
	tmp = input - (input >> 5);
	tmp = input + (tmp >> 3);
	tmp = input + (tmp >> 1);			
	
	return (tmp >> 3);
}


//============
//
//		class CDctDec
//
//==============

CDctDec::CDctDec() : CBASEMATRIX(8, 8), m_dct(8, 8)
{
	this->Init();
}

CDctDec::~CDctDec()
{
}

void CDctDec::Init(void)
{
	int	i, j;
	double sfblk[64];
	double val, pi = 3.1415926535;

	for(i = 0; i < 8; i++)
	{
		for(j = 0; j < 8; j++)
		{
			val = 1 << 6;
			val *= cos( (pi * ((i << 1) + 1.0) * j) / 16.0 );
			if(j == 0)
			{
				val *= sqrt(2.0);
				val /= 2.0;
			}
			sfblk[(i << 3) + j] = val;
		}
	}
	this->m_dct.Write(sfblk, 64);
}

int	CDctDec::Read(double *buf, int len)
{
	CBASEMATRIX temp(8, 8);

	temp = this->m_dct;
	temp *= *this;
	this->Equ(temp);
	temp = this->m_dct;
	temp.MRotate(ROT_MIRROR270);
	*this *= temp;
	*this /= 1 << 14;
	*this += 128;

	return __super::Read(buf, len);
}

void CDctDec::operator = (CDctDec &a)
{
	__super::Equ(a);
}

void CDctDec::operator = (double *a)
{
	__super::Write(a, 64);
}


//==========================
//
//		class CDctDec1
//
//==========================

CDctDec1::CDctDec1() : CBASEMATRIX1(8, 8)
{
}

CDctDec1::~CDctDec1()
{
}

int CDctDec1::Read(int *buf, int len)
{
	this->fidct8x8(buf);
/*
	__super::Write(buf, len);
	*this += 128;
	__super::Read(buf, len);
*/
	return len;	
}

void CDctDec1::fidct8x8(int *inblk)
{
	int i, temp[64];
	int even0, even1, even2, even3, odd0, odd1, odd2, odd3;

	for (i = 0; i < 8; i++) 
	{
		even0 = CDctEnc1::roundc4(inblk[(i << 3) + 0] << 14) + CDctEnc1::roundc2(inblk[(i << 3) + 2] << 14) +
				CDctEnc1::roundc4(inblk[(i << 3) + 4] << 14) + CDctEnc1::roundc6(inblk[(i << 3) + 6] << 14);
		even1 = CDctEnc1::roundc4(inblk[(i << 3) + 0] << 14) + CDctEnc1::roundc6(inblk[(i << 3) + 2] << 14) -
				CDctEnc1::roundc4(inblk[(i << 3) + 4] << 14) - CDctEnc1::roundc2(inblk[(i << 3) + 6] << 14);
		even2 = CDctEnc1::roundc4(inblk[(i << 3) + 0] << 14) - CDctEnc1::roundc6(inblk[(i << 3) + 2] << 14) -
				CDctEnc1::roundc4(inblk[(i << 3) + 4] << 14) + CDctEnc1::roundc2(inblk[(i << 3) + 6] << 14);
		even3 = CDctEnc1::roundc4(inblk[(i << 3) + 0] << 14) - CDctEnc1::roundc2(inblk[(i << 3) + 2] << 14) +
				CDctEnc1::roundc4(inblk[(i << 3) + 4] << 14) - CDctEnc1::roundc6(inblk[(i << 3) + 6] << 14);

		odd0 = CDctEnc1::roundc1(inblk[(i << 3) + 1] << 14) + CDctEnc1::roundc3(inblk[(i << 3) + 3] << 14) +
				CDctEnc1::roundc5(inblk[(i << 3) + 5] << 14) + CDctEnc1::roundc7(inblk[(i << 3) + 7] << 14);
		odd1 = CDctEnc1::roundc3(inblk[(i << 3) + 1] << 14) - CDctEnc1::roundc7(inblk[(i << 3) + 3] << 14) -
				CDctEnc1::roundc1(inblk[(i << 3) + 5] << 14) - CDctEnc1::roundc5(inblk[(i << 3) + 7] << 14);
		odd2 = CDctEnc1::roundc5(inblk[(i << 3) + 1] << 14) - CDctEnc1::roundc1(inblk[(i << 3) + 3] << 14) +
				CDctEnc1::roundc7(inblk[(i << 3) + 5] << 14) + CDctEnc1::roundc3(inblk[(i << 3) + 7] << 14);
		odd3 = CDctEnc1::roundc7(inblk[(i << 3) + 1] << 14) - CDctEnc1::roundc5(inblk[(i << 3) + 3] << 14) +
				CDctEnc1::roundc3(inblk[(i << 3) + 5] << 14) - CDctEnc1::roundc1(inblk[(i << 3) + 7] << 14);
		
		temp[(i << 3) + 0] = even0 + odd0;
		temp[(i << 3) + 1] = even1 + odd1;
		temp[(i << 3) + 2] = even2 + odd2;
		temp[(i << 3) + 3] = even3 + odd3;
		temp[(i << 3) + 4] = even3 - odd3;
		temp[(i << 3) + 5] = even2 - odd2;
		temp[(i << 3) + 6] = even1 - odd1;
		temp[(i << 3) + 7] = even0 - odd0;
	}

	for (i = 0; i < 8; i++) 
	{
		even0 = CDctEnc1::roundc4(temp[i + (0 << 3)]) + CDctEnc1::roundc2(temp[i + (2 << 3)]) +
				CDctEnc1::roundc4(temp[i + (4 << 3)]) + CDctEnc1::roundc6(temp[i + (6 << 3)]);
		even1 = CDctEnc1::roundc4(temp[i + (0 << 3)]) + CDctEnc1::roundc6(temp[i + (2 << 3)]) -
				CDctEnc1::roundc4(temp[i + (4 << 3)]) - CDctEnc1::roundc2(temp[i + (6 << 3)]);
		even2 = CDctEnc1::roundc4(temp[i + (0 << 3)]) - CDctEnc1::roundc6(temp[i + (2 << 3)]) -
				CDctEnc1::roundc4(temp[i + (4 << 3)]) + CDctEnc1::roundc2(temp[i + (6 << 3)]);
		even3 = CDctEnc1::roundc4(temp[i + (0 << 3)]) - CDctEnc1::roundc2(temp[i + (2 << 3)]) +
				CDctEnc1::roundc4(temp[i + (4 << 3)]) - CDctEnc1::roundc6(temp[i + (6 << 3)]);

		odd0 = CDctEnc1::roundc1(temp[i + (1 << 3)]) + CDctEnc1::roundc3(temp[i + (3 << 3)]) +
				CDctEnc1::roundc5(temp[i + (5 << 3)]) + CDctEnc1::roundc7(temp[i + (7 << 3)]);
		odd1 = CDctEnc1::roundc3(temp[i + (1 << 3)]) - CDctEnc1::roundc7(temp[i + (3 << 3)]) -
				CDctEnc1::roundc1(temp[i + (5 << 3)]) - CDctEnc1::roundc5(temp[i + (7 << 3)]);
		odd2 = CDctEnc1::roundc5(temp[i + (1 << 3)]) - CDctEnc1::roundc1(temp[i + (3 << 3)]) +
				CDctEnc1::roundc7(temp[i + (5 << 3)]) + CDctEnc1::roundc3(temp[i + (7 << 3)]);
		odd3 = CDctEnc1::roundc7(temp[i + (1 << 3)]) - CDctEnc1::roundc5(temp[i + (3 << 3)]) +
				CDctEnc1::roundc3(temp[i + (5 << 3)]) - CDctEnc1::roundc1(temp[i + (7 << 3)]);		

		inblk[i + (0 << 3)] = even0 + odd0;
		inblk[i + (1 << 3)] = even1 + odd1;
		inblk[i + (2 << 3)] = even2 + odd2;
		inblk[i + (3 << 3)] = even3 + odd3;
		inblk[i + (4 << 3)] = even3 - odd3;
		inblk[i + (5 << 3)] = even2 - odd2;
		inblk[i + (6 << 3)] = even1 - odd1;
		inblk[i + (7 << 3)] = even0 - odd0;

	}

	for (i = 0; i < 64; i++)
		inblk[i] >>= 16;
}
