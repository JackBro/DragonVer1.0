

#include "internal.h"


//=================================================
//
//		encoder matrix operation
//
//==================================================
void CJmMatrix::fpdct8x8(int *inblk, int *oublk)
{
	int  i;
	int  sum07, sum16, sum25, sum34, dif07, dif16, dif25, dif34;

	for(i = 0; i < 8; i++)
	{
		sum07 = this->shiftFix((inblk[(i<<3)+0]<<14) + (inblk[(i<<3)+7]<<14), 1);
		sum16 = this->shiftFix((inblk[(i<<3)+1]<<14) + (inblk[(i<<3)+6]<<14), 1);
		sum25 = this->shiftFix((inblk[(i<<3)+2]<<14) + (inblk[(i<<3)+5]<<14), 1);
		sum34 = this->shiftFix((inblk[(i<<3)+3]<<14) + (inblk[(i<<3)+4]<<14), 1);
															   
		dif07 = this->shiftFix((inblk[(i<<3)+0]<<14) - (inblk[(i<<3)+7]<<14), 1);
		dif16 = this->shiftFix((inblk[(i<<3)+1]<<14) - (inblk[(i<<3)+6]<<14), 1);
		dif25 = this->shiftFix((inblk[(i<<3)+2]<<14) - (inblk[(i<<3)+5]<<14), 1);
		dif34 = this->shiftFix((inblk[(i<<3)+3]<<14) - (inblk[(i<<3)+4]<<14), 1);

		oublk[(i<<3)+0]	= this->shiftFix(this->round14(sum07) + this->round14(sum16) +
										 this->round14(sum25) + this->round14(sum34), 2);
		oublk[(i<<3)+2] = this->shiftFix(this->round12(sum07) + this->roundfc(sum16) -
										 this->roundfc(sum25) - this->round12(sum34), 2);
		oublk[(i<<3)+4] = this->shiftFix(this->round14(sum07) - this->round14(sum16) -
										 this->round14(sum25) + this->round14(sum34), 2);
		oublk[(i<<3)+6] = this->shiftFix(this->roundfc(sum07) - this->round12(sum16) +
										 this->round12(sum25) - this->roundfc(sum34), 2);
			   
		oublk[(i<<3)+1] = this->shiftFix(this->round12(dif07) + this->rounddb(dif16) +
										 this->roundeb(dif25) + this->roundgb(dif34), 2);
		oublk[(i<<3)+3] = this->shiftFix(this->rounddb(dif07) - this->roundgb(dif16) -
										 this->round12(dif25) - this->roundeb(dif34), 2);
		oublk[(i<<3)+5] = this->shiftFix(this->roundeb(dif07) - this->round12(dif16) +
										 this->roundgb(dif25) + this->rounddb(dif34), 2);
		oublk[(i<<3)+7] = this->shiftFix(this->roundgb(dif07) - this->roundeb(dif16) +
										 this->rounddb(dif25) - this->round12(dif34), 2);
	}
	for(i = 0; i < 8; i++)
	{
		sum07 = this->shiftFix(oublk[i+(0<<3)] + oublk[i+(7<<3)], 1);
		sum16 = this->shiftFix(oublk[i+(1<<3)] + oublk[i+(6<<3)], 1);
		sum25 = this->shiftFix(oublk[i+(2<<3)] + oublk[i+(5<<3)], 1);
		sum34 = this->shiftFix(oublk[i+(3<<3)] + oublk[i+(4<<3)], 1);
												
		dif07 = this->shiftFix(oublk[i+(0<<3)] - oublk[i+(7<<3)], 1);
		dif16 = this->shiftFix(oublk[i+(1<<3)] - oublk[i+(6<<3)], 1);
		dif25 = this->shiftFix(oublk[i+(2<<3)] - oublk[i+(5<<3)], 1);
		dif34 = this->shiftFix(oublk[i+(3<<3)] - oublk[i+(4<<3)], 1);

		inblk[i+(0<<3)]	= this->shiftFix(this->round14(sum07) + this->round14(sum16) +
										 this->round14(sum25) + this->round14(sum34), 2);
		inblk[i+(2<<3)] = this->shiftFix(this->round12(sum07) + this->roundfc(sum16) -
										 this->roundfc(sum25) - this->round12(sum34), 2);
		inblk[i+(4<<3)] = this->shiftFix(this->round14(sum07) - this->round14(sum16) -
										 this->round14(sum25) + this->round14(sum34), 2);
		inblk[i+(6<<3)] = this->shiftFix(this->roundfc(sum07) - this->round12(sum16) +
										 this->round12(sum25) - this->roundfc(sum34), 2);
					
		inblk[i+(1<<3)] = this->shiftFix(this->round12(dif07) + this->rounddb(dif16) +
										 this->roundeb(dif25) + this->roundgb(dif34), 2);
		inblk[i+(3<<3)] = this->shiftFix(this->rounddb(dif07) - this->roundgb(dif16) -
										 this->round12(dif25) - this->roundeb(dif34), 2);
		inblk[i+(5<<3)] = this->shiftFix(this->roundeb(dif07) - this->round12(dif16) +
										 this->roundgb(dif25) + this->rounddb(dif34), 2);
		inblk[i+(7<<3)] = this->shiftFix(this->roundgb(dif07) - this->roundeb(dif16) +
										 this->rounddb(dif25) - this->round12(dif34), 2);
	}
	for (i = 0; i < 64; i++)
		oublk[i] = this->shiftFix(inblk[i] >> 2, 1);

	/*for(i = 0; i < 8; i++)
	{
		sum07 = this->shiftFix((inblk[i+(0<<3)]<<14) + (inblk[i+(7<<3)]<<14), 1);
		sum16 = this->shiftFix((inblk[i+(1<<3)]<<14) + (inblk[i+(6<<3)]<<14), 1);
		sum25 = this->shiftFix((inblk[i+(2<<3)]<<14) + (inblk[i+(5<<3)]<<14), 1);
		sum34 = this->shiftFix((inblk[i+(3<<3)]<<14) + (inblk[i+(4<<3)]<<14), 1);

		dif07 = this->shiftFix((inblk[i+(0<<3)]<<14) - (inblk[i+(7<<3)]<<14), 1);
		dif16 = this->shiftFix((inblk[i+(1<<3)]<<14) - (inblk[i+(6<<3)]<<14), 1);
		dif25 = this->shiftFix((inblk[i+(2<<3)]<<14) - (inblk[i+(5<<3)]<<14), 1);
		dif34 = this->shiftFix((inblk[i+(3<<3)]<<14) - (inblk[i+(4<<3)]<<14), 1);

		oublk[i+(0<<3)]	= this->shiftFix(this->round14(sum07) + this->round14(sum16) +
			this->round14(sum25) + this->round14(sum34), 2);
		oublk[i+(2<<3)] = this->shiftFix(this->round12(sum07) + this->roundfc(sum16) -
			this->roundfc(sum25) - this->round12(sum34), 2);
		oublk[i+(4<<3)] = this->shiftFix(this->round14(sum07) - this->round14(sum16) -
			this->round14(sum25) + this->round14(sum34), 2);
		oublk[i+(6<<3)] = this->shiftFix(this->roundfc(sum07) - this->round12(sum16) +
			this->round12(sum25) - this->roundfc(sum34), 2);

		oublk[i+(1<<3)] = this->shiftFix(this->round12(dif07) + this->rounddb(dif16) +
			this->roundeb(dif25) + this->roundgb(dif34), 2);
		oublk[i+(3<<3)] = this->shiftFix(this->rounddb(dif07) - this->roundgb(dif16) -
			this->round12(dif25) - this->roundeb(dif34), 2);
		oublk[i+(5<<3)] = this->shiftFix(this->roundeb(dif07) - this->round12(dif16) +
			this->roundgb(dif25) + this->rounddb(dif34), 2);
		oublk[i+(7<<3)] = this->shiftFix(this->roundgb(dif07) - this->roundeb(dif16) +
			this->rounddb(dif25) - this->round12(dif34), 2);
	}
	for(i = 0; i < 8; i++)
	{
		sum07 = this->shiftFix(oublk[(i<<3)+0] + oublk[(i<<3)+7], 1);
		sum16 = this->shiftFix(oublk[(i<<3)+1] + oublk[(i<<3)+6], 1);
		sum25 = this->shiftFix(oublk[(i<<3)+2] + oublk[(i<<3)+5], 1);
		sum34 = this->shiftFix(oublk[(i<<3)+3] + oublk[(i<<3)+4], 1);

		dif07 = this->shiftFix(oublk[(i<<3)+0] - oublk[(i<<3)+7], 1);
		dif16 = this->shiftFix(oublk[(i<<3)+1] - oublk[(i<<3)+6], 1);
		dif25 = this->shiftFix(oublk[(i<<3)+2] - oublk[(i<<3)+5], 1);
		dif34 = this->shiftFix(oublk[(i<<3)+3] - oublk[(i<<3)+4], 1);

		inblk[(i<<3)+0]	= this->shiftFix(this->round14(sum07) + this->round14(sum16) +
			this->round14(sum25) + this->round14(sum34), 2);
		inblk[(i<<3)+2] = this->shiftFix(this->round12(sum07) + this->roundfc(sum16) -
			this->roundfc(sum25) - this->round12(sum34), 2);
		inblk[(i<<3)+4] = this->shiftFix(this->round14(sum07) - this->round14(sum16) -
			this->round14(sum25) + this->round14(sum34), 2);
		inblk[(i<<3)+6] = this->shiftFix(this->roundfc(sum07) - this->round12(sum16) +
			this->round12(sum25) - this->roundfc(sum34), 2);

		inblk[(i<<3)+1] = this->shiftFix(this->round12(dif07) + this->rounddb(dif16) +
			this->roundeb(dif25) + this->roundgb(dif34), 2);
		inblk[(i<<3)+3] = this->shiftFix(this->rounddb(dif07) - this->roundgb(dif16) -
			this->round12(dif25) - this->roundeb(dif34), 2);
		inblk[(i<<3)+5] = this->shiftFix(this->roundeb(dif07) - this->round12(dif16) +
			this->roundgb(dif25) + this->rounddb(dif34), 2);
		inblk[(i<<3)+7] = this->shiftFix(this->roundgb(dif07) - this->roundeb(dif16) +
			this->rounddb(dif25) - this->round12(dif34), 2);
	}
	for (i = 0; i < 64; i++)
		oublk[i] = this->shiftFix(inblk[i] >> 2, 1);*/
}

void CJmMatrix::zigzag(int *inblk, int *oublk)
{
	int  i;

	for (i = 0 ; i < 64 ; i++) 
		oublk[gZigZag[i]] = inblk[i];
}

void CJmMatrix::zigzag(double *inblk, int *oublk)
{
	int  i;

	for (i = 0 ; i < 64 ; i++) 
	{
		if(inblk[i] > 0)
			oublk[gZigZag[i]] = (int)(inblk[i] + 0.5);
		else
			oublk[gZigZag[i]] = (int)(inblk[i] - 0.5);
	}
}


int CJmMatrix::roundDouble(double input)
{
	int result;

	if(input > 0.0)
		result = (int)(input + 0.5);
	else
		result = (int)(input - 0.5);

	return result;
}

int CJmMatrix::shiftFix(int input, int shift)
{
	int result;

	if (shift == 0)
		return input;
	result = input >> (shift-1);

	if( (result & 3) != 0 )
		result = 1;
	else
		result = 0;

	result += (input >> (shift+1)) << 1;
	return result;
}

int CJmMatrix::roundfc(int input)
{
	int  tmp;

    tmp = (input << 1) + (input >> 5);	/* sum0 */
	tmp = (input << 1) + (tmp >> 2);	/* sum1 */
	tmp = (input << 1) + (tmp >> 2);	/* sum2 */
	tmp = (input << 1) + (tmp >> 1);	/* sum3 */
	return this->shiftFix(tmp, 4);
}

int CJmMatrix::rounddb(int input)
{
	int  tmp;

	tmp = (input >> 4) + (input << 1);	/* sum0 */
	tmp = (tmp >> 3)   - (input << 1);	/* sum1 */
	tmp = (tmp >> 2)   - (input << 1);	/* sum2 */
	tmp = (tmp >> 3)   + (input << 1);	/* sum3 */
	return this->shiftFix(tmp, 2);
}

int CJmMatrix::roundeb(int input)
{
	int  tmp;

	tmp = (input << 1) + (input >> 5);	/* sum0 */
	tmp = (input << 1) + (tmp >> 4);	/* sum1 */
	tmp = (input << 1) + (tmp >> 3);	/* sum2 */
	return this->shiftFix(tmp, 3);
}

int CJmMatrix::roundgb(int input)
{
	int tmp, tm0;
	
	tm0 = (-input-1) & 7;
	tmp = (-input-1) - tm0;

	if (tm0 != 0)
		tmp = tmp + 4;

	tmp = (tmp >> 2) - input + 1;		/* sum0 */
	tmp = (tmp >> 3) - (input << 1);	/* sum1 */
	tmp = (tmp >> 2) + (input << 1);	/* sum2 */
	tmp = (tmp >> 2) - (input << 1);	/* sum3 */
	tmp = (tmp >> 2) + (input << 1);	/* sum4 */

	return this->shiftFix(tmp, 4);
}

int CJmMatrix::round12(int input)
{
	return this->shiftFix(input, 1);
}

int CJmMatrix::round14(int input)
{
	return this->shiftFix(input, 2);
}

void CJmMatrix::quant(int *inblk, int *oublk, int *brcTable, int sft)
{
	int  i;

	for(i = 0; i < 64; i++)
	{
		oublk[i] = (inblk[i] * brcTable[i]) >> 5;
		oublk[i] >>= 13+sft;
		oublk[i] += 1;
		oublk[i] >>= 1;
	}
}

void CJmMatrix::dctquant(char *block, double *qt, double *oublk)
{
	int i;
	CDctEnc enc;

	for(i = 0; i < 64; i++)
		oublk[i] = (double)(block[i] & 0xff);
	enc.Write(oublk, 64);
	enc /= qt;
	enc.Read(oublk, 64);
}


void CJmMatrix::dct8x8(int *inblk)
{
//	int i;
	CDctEnc1 enc;
/*
	for(i = 0; i < 64; i++)
		oublk[i] = (int)block[i] & 0xff;
*/
	enc.Write(inblk, 64);
}

/*=================================================
	quantization function	
=================================================*/
void CJmMatrix::fquant(int *inblk, int *oublk, int *QTable)
{
	int i;
	
	for (i = 0; i < 64; i++)
	{		
		oublk[i] = inblk[i] * QTable[i];
		oublk[i] >>= 15;
		oublk[i] = (oublk[i] < 0) ? (oublk[i] + 1) : oublk[i];
	}
}

//=================================================
//
//		another encoder matrix operation
//		added  by mfbao    04/13/05
//
//==================================================
/*
void CJmMatrix::real_fdct8x8(int *inblk, int *oublk)
{
	int const C1 =64277 ;	//0.98078528;
	int const C2 =60547 ;	//0.92387953;
	int const C3 =54491 ;	//0.83146961;
	int const C4 =46341 ;	//0.70710678;
	int const C5 =36410 ;	//0.55557023;
	int const C6 =25080 ;	//0.38268343;
	int const C7 =12785 ;	//0.19509032;

	int i;
	int sum07, sum16, sum25, sum34, dif07, dif16, dif25, dif34;
	
	for (i = 0; i < 8; i++)
	{
		sum07 = (inblk[(i << 3) + 0] + inblk[(i << 3) + 7]);
		sum16 = (inblk[(i << 3) + 1] + inblk[(i << 3) + 6]);
		sum25 = (inblk[(i << 3) + 2] + inblk[(i << 3) + 5]);
		sum34 = (inblk[(i << 3) + 3] + inblk[(i << 3) + 4]);
															   
		dif07 = (inblk[(i << 3) + 0] - inblk[(i << 3) + 7]);
		dif16 = (inblk[(i << 3) + 1] - inblk[(i << 3) + 6]);
		dif25 = (inblk[(i << 3) + 2] - inblk[(i << 3) + 5]);
		dif34 = (inblk[(i << 3) + 3] - inblk[(i << 3) + 4]);

		oublk[(i << 3) + 0]	= (C4 * (sum07 + sum16 + sum25 + sum34)) >> 16;		
		oublk[(i << 3) + 2] = (C2 * (sum07 - sum34) + C6 * (sum16 - sum25)) >> 16;										 
		oublk[(i << 3) + 4] = (C4 * (sum07 - sum16 - sum25 + sum34)) >> 16;		
		oublk[(i << 3) + 6] = (C6 * (sum07 - sum34) - C2 * (sum16 - sum25)) >> 16;	
				   
		oublk[(i << 3) + 1] = (C1 * (dif07) + C3 * (dif16) + C5 * (dif25) + C7 * (dif34)) >> 16;										
		oublk[(i << 3) + 3] = (C3 * (dif07) - C7 * (dif16) - C1 * (dif25) - C5 * (dif34)) >> 16;		
		oublk[(i << 3) + 5] = (C5 * (dif07) - C1 * (dif16) + C7 * (dif25) + C3 * (dif34)) >> 16;		
		oublk[(i << 3) + 7] = (C7 * (dif07) - C5 * (dif16) + C3 * (dif25) - C1 * (dif34)) >> 16;
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

		oublk[i + (0 << 3)]	= C4 * (sum07 + sum16 + sum25 + sum34);		
		oublk[i + (2 << 3)] = C2 * (sum07 - sum34) + C6 * (sum16 - sum25);										 
		oublk[i + (4 << 3)] = C4 * (sum07 - sum16 - sum25 + sum34);		
		oublk[i + (6 << 3)] = C6 * (sum07 - sum34) - C2 * (sum16 - sum25);
			   
		oublk[i + (1 << 3)] = C1 * (dif07) + C3 * (dif16) + C5 * (dif25) + C7 * (dif34);										
		oublk[i + (3 << 3)] = C3 * (dif07) - C7 * (dif16) - C1 * (dif25) - C5 * (dif34);		
		oublk[i + (5 << 3)] = C5 * (dif07) - C1 * (dif16) + C7 * (dif25) + C3 * (dif34);		
		oublk[i + (7 << 3)] = C7 * (dif07) - C5 * (dif16) + C3 * (dif25) - C1 * (dif34);
	}
	
	for (i = 0; i < 64; i++)
	{
		oublk[i] >>= 18;
		oublk[i] = (oublk[i] < 0) ? (oublk[i] + 1) : oublk[i];
	}
}
*/


//=================================================
//
//		decoder matrix operation
//
//==================================================
void CJmMatrix::izigzag(int *inblk, int *oublk)
{
	int  i;

	for (i = 0 ; i < 64 ; i++) 
		oublk[i] = inblk[gZigZag[i]];
}

void CJmMatrix::izigzag(int *inblk, double *oublk)
{
	int  i;

	for (i = 0 ; i < 64 ; i++) 
		oublk[i] = (double)inblk[gZigZag[i]];
}

void CJmMatrix::izigzag(int *inblk, int *oublk, int alter)
{
	int i;
	for (i = 0; i < 64; i++)
		oublk[i] = inblk[gAlterZigZag[alter][i]];
}

void CJmMatrix::dequant(int *inblk, int *oublk, int *brcTable, int sft)
{
	int  i;

	for(i = 0; i < 64; i++)
	{
		oublk[i] = ((inblk[i] << 4) * brcTable[i]) >> 5;
		oublk[i] >>= 7 - sft;
		oublk[i] += 1;
		oublk[i] >>= 1;
		if (oublk[i] >= 65536)
			oublk[i] = 65024;	/* 0x0fe00 */
		else if (oublk[i] < -65536)
			oublk[i] = -65536;	/* 0x10000 */
	}
}

void CJmMatrix::fpidct8x8(int *inblk, int *oublk)
{
	int i, tpram[64];
	int even0, even1, even2, even3, odd0, odd1, odd2, odd3;

	for (i = 0; i < 8; i++) 
	{
		even0 = this->shiftFix(this->round14(inblk[(i<<3)+0]<<5) + this->round12(inblk[(i<<3)+2]<<5) +
			this->round14(inblk[(i<<3)+4]<<5) + this->roundfc(inblk[(i<<3)+6]<<5), 2);
		even1 = this->shiftFix(this->round14(inblk[(i<<3)+0]<<5) + this->roundfc(inblk[(i<<3)+2]<<5) -
			this->round14(inblk[(i<<3)+4]<<5) - this->round12(inblk[(i<<3)+6]<<5), 2);
		even2 = this->shiftFix(this->round14(inblk[(i<<3)+0]<<5) - this->roundfc(inblk[(i<<3)+2]<<5) -
			this->round14(inblk[(i<<3)+4]<<5) + this->round12(inblk[(i<<3)+6]<<5), 2);
		even3 = this->shiftFix(this->round14(inblk[(i<<3)+0]<<5) - this->round12(inblk[(i<<3)+2]<<5) +
			this->round14(inblk[(i<<3)+4]<<5) - this->roundfc(inblk[(i<<3)+6]<<5), 2);

		odd0  = this->shiftFix(this->round12(inblk[(i<<3)+1]<<5) + this->rounddb(inblk[(i<<3)+3]<<5) +
			this->roundeb(inblk[(i<<3)+5]<<5) + this->roundgb(inblk[(i<<3)+7]<<5), 2);
		odd1  = this->shiftFix(this->rounddb(inblk[(i<<3)+1]<<5) - this->roundgb(inblk[(i<<3)+3]<<5) -
			this->round12(inblk[(i<<3)+5]<<5) - this->roundeb(inblk[(i<<3)+7]<<5), 2);
		odd2  = this->shiftFix(this->roundeb(inblk[(i<<3)+1]<<5) - this->round12(inblk[(i<<3)+3]<<5) +
			this->roundgb(inblk[(i<<3)+5]<<5) + this->rounddb(inblk[(i<<3)+7]<<5), 2);
		odd3  = this->shiftFix(this->roundgb(inblk[(i<<3)+1]<<5) - this->roundeb(inblk[(i<<3)+3]<<5) +
			this->rounddb(inblk[(i<<3)+5]<<5) - this->round12(inblk[(i<<3)+7]<<5), 2);

		tpram[(i<<3)+0] = this->shiftFix(even0+odd0, 1);
		tpram[(i<<3)+1] = this->shiftFix(even1+odd1, 1);
		tpram[(i<<3)+2] = this->shiftFix(even2+odd2, 1);
		tpram[(i<<3)+3] = this->shiftFix(even3+odd3, 1);
		tpram[(i<<3)+4] = this->shiftFix(even3-odd3, 1);
		tpram[(i<<3)+5] = this->shiftFix(even2-odd2, 1);
		tpram[(i<<3)+6] = this->shiftFix(even1-odd1, 1);
		tpram[(i<<3)+7] = this->shiftFix(even0-odd0, 1);
	}

	for (i = 0; i < 8; i++) 
	{
		even0 = this->shiftFix(this->round14(tpram[(0<<3)+i]) + this->round12(tpram[(2<<3)+i]) +
			this->round14(tpram[(4<<3)+i]) + this->roundfc(tpram[(6<<3)+i]), 2);
		even1 = this->shiftFix(this->round14(tpram[(0<<3)+i]) + this->roundfc(tpram[(2<<3)+i]) -
			this->round14(tpram[(4<<3)+i]) - this->round12(tpram[(6<<3)+i]), 2);
		even2 = this->shiftFix(this->round14(tpram[(0<<3)+i]) - this->roundfc(tpram[(2<<3)+i]) -
			this->round14(tpram[(4<<3)+i]) + this->round12(tpram[(6<<3)+i]), 2);
		even3 = this->shiftFix(this->round14(tpram[(0<<3)+i]) - this->round12(tpram[(2<<3)+i]) +
			this->round14(tpram[(4<<3)+i]) - this->roundfc(tpram[(6<<3)+i]), 2);

		odd0  = this->shiftFix(this->round12(tpram[(1<<3)+i]) + this->rounddb(tpram[(3<<3)+i]) +
			this->roundeb(tpram[(5<<3)+i]) + this->roundgb(tpram[(7<<3)+i]), 2);
		odd1  = this->shiftFix(this->rounddb(tpram[(1<<3)+i]) - this->roundgb(tpram[(3<<3)+i]) -
			this->round12(tpram[(5<<3)+i]) - this->roundeb(tpram[(7<<3)+i]), 2);
		odd2  = this->shiftFix(this->roundeb(tpram[(1<<3)+i]) - this->round12(tpram[(3<<3)+i]) +
			this->roundgb(tpram[(5<<3)+i]) + this->rounddb(tpram[(7<<3)+i]), 2);
		odd3  = this->shiftFix(this->roundgb(tpram[(1<<3)+i]) - this->roundeb(tpram[(3<<3)+i]) +
			this->rounddb(tpram[(5<<3)+i]) - this->round12(tpram[(7<<3)+i]), 2);

		oublk[(0<<3)+i] = this->shiftFix(even0+odd0, 1);
		oublk[(1<<3)+i] = this->shiftFix(even1+odd1, 1);
		oublk[(2<<3)+i] = this->shiftFix(even2+odd2, 1);
		oublk[(3<<3)+i] = this->shiftFix(even3+odd3, 1);
		oublk[(4<<3)+i] = this->shiftFix(even3-odd3, 1);
		oublk[(5<<3)+i] = this->shiftFix(even2-odd2, 1);
		oublk[(6<<3)+i] = this->shiftFix(even1-odd1, 1);
		oublk[(7<<3)+i] = this->shiftFix(even0-odd0, 1);

	}
	for(i = 0; i < 64; i++)
		oublk[i] = ((oublk[i] >> 3) + 1) >> 1;
	
	/*for (i = 0; i < 8; i++) 
	{
		even0 = this->shiftFix(this->round14(inblk[(i<<3)+0]<<5) + this->round12(inblk[(i<<3)+2]<<5) +
			this->round14(inblk[(i<<3)+4]<<5) + this->roundfc(inblk[(i<<3)+6]<<5), 2);
		even1 = this->shiftFix(this->round14(inblk[(i<<3)+0]<<5) + this->roundfc(inblk[(i<<3)+2]<<5) -
			this->round14(inblk[(i<<3)+4]<<5) - this->round12(inblk[(i<<3)+6]<<5), 2);
		even2 = this->shiftFix(this->round14(inblk[(i<<3)+0]<<5) - this->roundfc(inblk[(i<<3)+2]<<5) -
			this->round14(inblk[(i<<3)+4]<<5) + this->round12(inblk[(i<<3)+6]<<5), 2);
		even3 = this->shiftFix(this->round14(inblk[(i<<3)+0]<<5) - this->round12(inblk[(i<<3)+2]<<5) +
			this->round14(inblk[(i<<3)+4]<<5) - this->roundfc(inblk[(i<<3)+6]<<5), 2);

		odd0  = this->shiftFix(this->round12(inblk[(i<<3)+1]<<5) + this->rounddb(inblk[(i<<3)+3]<<5) +
			this->roundeb(inblk[(i<<3)+5]<<5) + this->roundgb(inblk[(i<<3)+7]<<5), 2);
		odd1  = this->shiftFix(this->rounddb(inblk[(i<<3)+1]<<5) - this->roundgb(inblk[(i<<3)+3]<<5) -
			this->round12(inblk[(i<<3)+5]<<5) - this->roundeb(inblk[(i<<3)+7]<<5), 2);
		odd2  = this->shiftFix(this->roundeb(inblk[(i<<3)+1]<<5) - this->round12(inblk[(i<<3)+3]<<5) +
			this->roundgb(inblk[(i<<3)+5]<<5) + this->rounddb(inblk[(i<<3)+7]<<5), 2);
		odd3  = this->shiftFix(this->roundgb(inblk[(i<<3)+1]<<5) - this->roundeb(inblk[(i<<3)+3]<<5) +
			this->rounddb(inblk[(i<<3)+5]<<5) - this->round12(inblk[(i<<3)+7]<<5), 2);

		tpram[(i<<3)+0] = this->shiftFix(even0+odd0, 1);
		tpram[(i<<3)+1] = this->shiftFix(even1+odd1, 1);
		tpram[(i<<3)+2] = this->shiftFix(even2+odd2, 1);
		tpram[(i<<3)+3] = this->shiftFix(even3+odd3, 1);
		tpram[(i<<3)+4] = this->shiftFix(even3-odd3, 1);
		tpram[(i<<3)+5] = this->shiftFix(even2-odd2, 1);
		tpram[(i<<3)+6] = this->shiftFix(even1-odd1, 1);
		tpram[(i<<3)+7] = this->shiftFix(even0-odd0, 1);
	}

	for (i = 0; i < 8; i++) 
	{
		even0 = this->shiftFix(this->round14(tpram[(0<<3)+i]) + this->round12(tpram[(2<<3)+i]) +
			this->round14(tpram[(4<<3)+i]) + this->roundfc(tpram[(6<<3)+i]), 2);
		even1 = this->shiftFix(this->round14(tpram[(0<<3)+i]) + this->roundfc(tpram[(2<<3)+i]) -
			this->round14(tpram[(4<<3)+i]) - this->round12(tpram[(6<<3)+i]), 2);
		even2 = this->shiftFix(this->round14(tpram[(0<<3)+i]) - this->roundfc(tpram[(2<<3)+i]) -
			this->round14(tpram[(4<<3)+i]) + this->round12(tpram[(6<<3)+i]), 2);
		even3 = this->shiftFix(this->round14(tpram[(0<<3)+i]) - this->round12(tpram[(2<<3)+i]) +
			this->round14(tpram[(4<<3)+i]) - this->roundfc(tpram[(6<<3)+i]), 2);

		odd0  = this->shiftFix(this->round12(tpram[(1<<3)+i]) + this->rounddb(tpram[(3<<3)+i]) +
			this->roundeb(tpram[(5<<3)+i]) + this->roundgb(tpram[(7<<3)+i]), 2);
		odd1  = this->shiftFix(this->rounddb(tpram[(1<<3)+i]) - this->roundgb(tpram[(3<<3)+i]) -
			this->round12(tpram[(5<<3)+i]) - this->roundeb(tpram[(7<<3)+i]), 2);
		odd2  = this->shiftFix(this->roundeb(tpram[(1<<3)+i]) - this->round12(tpram[(3<<3)+i]) +
			this->roundgb(tpram[(5<<3)+i]) + this->rounddb(tpram[(7<<3)+i]), 2);
		odd3  = this->shiftFix(this->roundgb(tpram[(1<<3)+i]) - this->roundeb(tpram[(3<<3)+i]) +
			this->rounddb(tpram[(5<<3)+i]) - this->round12(tpram[(7<<3)+i]), 2);

		oublk[(0<<3)+i] = this->shiftFix(even0+odd0, 1);
		oublk[(1<<3)+i] = this->shiftFix(even1+odd1, 1);
		oublk[(2<<3)+i] = this->shiftFix(even2+odd2, 1);
		oublk[(3<<3)+i] = this->shiftFix(even3+odd3, 1);
		oublk[(4<<3)+i] = this->shiftFix(even3-odd3, 1);
		oublk[(5<<3)+i] = this->shiftFix(even2-odd2, 1);
		oublk[(6<<3)+i] = this->shiftFix(even1-odd1, 1);
		oublk[(7<<3)+i] = this->shiftFix(even0-odd0, 1);

	}
	for(i = 0; i < 64; i++)
		oublk[i] = ((oublk[i] >> 3) + 1) >> 1;*/
}

void CJmMatrix::idctdequant(double *inblk, double *qt, char *block)
{
	int i;
	double iiblk[64];
	CDctDec dec;

	dec.Write(inblk, 64);
	dec *= qt;
	dec.Read(iiblk, 64);
	for(i = 0; i < 64; i++)
	{
		//if(iiblk[i] > 0)
		//	iiblk[i] += 0.5;
		//else
		//	iiblk[i] -= 0.5;
		//iiblk[i] += 128;
		iiblk[i] = (iiblk[i] < 0) ? 0 : ((iiblk[i] > 255) ? -1 : iiblk[i]);
		block[i] = (char)iiblk[i];
	}
}

void CJmMatrix::idct8x8(int *inblk)
{
//	int i;
	CDctDec1 dec;

	dec.Read(inblk, 64);
/*
	for(i = 0; i < 64; i++)									
	{
		inblk[i] = (inblk[i] < 0) ? 0 : ((inblk[i] > 255) ? -1 : inblk[i]);		//very important!
		block[i] = (char)inblk[i];
	}
*/
}

void CJmMatrix::idct8x8(double *inblk)
{
	int i;
	CDctDec dec;

	dec.Write(inblk, 64);
	dec.Read(inblk, 64);

	for (i = 0; i < 64; i++)
		inblk[i] = inblk[i] - 128;
}

void CJmMatrix::fiquant(int *inblk, int *oublk, int *QTable)
{
	int i;
	
	for (i = 0; i < 64; i++)
	{		
		oublk[i] = inblk[i] * QTable[i];
		//oublk[i] >>= 15;
		//oublk[i] = (oublk[i] < 0) ? (oublk[i] + 1) : oublk[i];
	}
}

void CJmMatrix::mpeg2iquant(int *inblk, int *oublk, int *QTable, int mbtype, char dcprecision)
{
	int i, sum = 0;	
	
	if (mbtype)											//intra macroblock
	{
		oublk[0] = inblk[0] << (3 - dcprecision);
		for (i = 1; i < 64; i++)
		{
			oublk[i] = inblk[i] * QTable[i] << 1;
		}
	}
	else												//nonintra macroblock
	{
		for (i = 0; i < 64; i++)
		{
			oublk[i] = ( (inblk[i] << 1) + ((inblk[i] > 0) ? 1 : ((inblk[i] < 0) ? -1 : 0)) ) * QTable[i];
		}
	}
	
	for (i = 0; i < 64; i++)
	{
		oublk[i] = (oublk[i] > 2047) ? 2047 : ((oublk[i] < -2048) ? -2048 : oublk[i]);
		sum += oublk[i];
	}
/*
	if ( !(sum & 1))
	{
		if (oublk[63] & 1)
			oublk[63] -= 1;
		else
			oublk[63] += 1;
	}
*/
	if ((sum & 1) == 0)
		oublk[63] ^= 1;
}

int	CJmMatrix::ValAdjust(int val, int index)
{
	if( (val & (1 << (index - 1))) != 0 )
		return val;
	val += 0 - (1 << index);
	return val + 1;
}

