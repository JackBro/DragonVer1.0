
#include "internal.h"

const int g4BitNumber =     4;
const int gCharBitNumber =  8;
const int gShortBitNumber = 16;

//{{ Vimicro ADPCM Table
const int gVimicroIndexTable[16] = {
		-1, -1, -1, -1, 2, 4, 6, 8,
		-1, -1, -1, -1, 2, 4, 6, 8,
};
const int gVimicroStepsizeTable[89] = {
		7, 8, 9, 10, 11, 12, 13, 14, 16, 17,
		19, 21, 23, 25, 28, 31, 34, 37, 41, 45,
		50, 55, 60, 66, 73, 80, 88, 97, 107, 118,
		130, 143, 157, 173, 190, 209, 230, 253, 279, 307,
		337, 371, 408, 449, 494, 544, 598, 658, 724, 796,
		876, 963, 1060, 1166, 1282, 1411, 1552, 1707, 1878, 2066,
		2272, 2499, 2749, 3024, 3327, 3660, 4026, 4428, 4871, 5358,
		5894, 6484, 7132, 7845, 8630, 9493, 10442, 11487, 12635, 13899,
		15289, 16818, 18500, 20350, 22385, 24623, 27086, 29794, 32767
};
//}} Vimicro ADPCM Table

//{{YAMAHA ADPCM Table
const char gYamahaDifflookup[16] = {
		1,3,5,7,9,11,13,15,
		-1,-3,-5,-7,-9,-11,-13,-15,
};

const short gYamahaIndexScale[16] = {
		0x0e6, 0x0e6, 0x0e6, 0x0e6, 0x133, 0x199, 0x200, 0x266,
		0x0e6, 0x0e6, 0x0e6, 0x0e6, 0x133, 0x199, 0x200, 0x266 
};
//}}YAMAHA ADPCM Table



//{{Ms_adpcm Table
const int gMsAdaptCoeff1[7] ={	
		256, 512, 0, 192, 240, 460, 392
} ;

const int gMsAdaptCoeff2[7] ={	
		0, -256, 0, 64, 0, -208, -232
} ;

const int gMsAdaptationTable [16] ={	
		230, 230, 230, 230, 307, 409, 512, 614,
		768, 614, 512, 409, 307, 230, 230, 230
} ;
//}}Ms_adpcm Table


//=======================================================================================
//			class C968EncoderAdpcm
//=======================================================================================

C968EncoderAdpcm::C968EncoderAdpcm(int size, int order) : CBitFifo(size, order)
{
	
}

C968EncoderAdpcm::~C968EncoderAdpcm(void)
{

}

int C968EncoderAdpcm::Read(char *buf, int len)
{
	return CBitFifo::Read(buf, len<<3)>>3;
}

int C968EncoderAdpcm::GetDataSize(void)
{
	return CBitFifo::GetDataSize()>>3;	
}



//=======================================================================================
//			class C968DecoderAdpcm
//=======================================================================================

C968DecoderAdpcm::C968DecoderAdpcm(int size, int order) : CBitFifo(size, order)
{
}

C968DecoderAdpcm::~C968DecoderAdpcm(void)
{

}

int C968DecoderAdpcm::Read(char *buf, int len)
{
	return CBitFifo::Read(buf, len<<3)>>3;
}

int C968DecoderAdpcm::GetDataSize(void)
{
	return CBitFifo::GetDataSize()>>3;
}



//=======================================================================================
//			class C968VimicroEncoderAdpcm
//=======================================================================================

C968VimicroEncoderAdpcm::C968VimicroEncoderAdpcm(int size, int order) : C968EncoderAdpcm(size, order)
{
	InitState();
}

C968VimicroEncoderAdpcm::~C968VimicroEncoderAdpcm(void)
{
	
}

void C968VimicroEncoderAdpcm::SetAdpcmState(const ADPCMSTATE& state)
{
	this->m_state.index = state.index;
	this->m_state.valprev = state.valprev;
}

int C968VimicroEncoderAdpcm::Read(char *buf, int len)
{
	return C968EncoderAdpcm::Read(buf, len);
}


int	C968VimicroEncoderAdpcm::Write(char *buf, int len)
{
	short *inp;			     /* Input buffer pointer */
	signed char outp;	     /* output buffer pointer */
	int val;			     /* Current input sample value */
	int sign;			     /* Current adpcm sign bit */
	int delta;			     /* Current adpcm output value */
	int diff;			     /* Difference between val and valprev */
	int step;			     /* Stepsize */
	int valpred;		     /* Predicted output value */
	int vpdiff;			     /* Current change to valpred */
	int index;			     /* Current step change index */
	int outputbuffer = 0;    /* place to keep previous 4-bit value */
	int bufferstep;		     /* toggle between outputbuffer/output */
	int nLength = 0;
	len >>= 1;               //Pcm actural length,char->short   
	inp = (short *)buf;

	valpred = m_state.valprev;
	index = m_state.index;
	step = gVimicroStepsizeTable[index];
	bufferstep = 1;
	for ( ; len > 0 ; len-- ) 
	{
		val = *inp++;
		/* Step 1 - compute difference with previous value */
		diff = val - valpred;
		sign = (diff < 0) ? 8 : 0;
		if ( sign ) diff = (-diff);
		/* Step 2 - Divide and clamp */
		/* Note:
		** This code *approximately* computes:
		**    delta = diff*4/step;
		**    vpdiff = (delta+0.5)*step/4;
		** but in shift step bits are dropped. The net result of this is
		** that even if you have fast mul/div hardware you cannot put it to
		** good use since the fixup would be too expensive.
		*/
		delta = 0;
		vpdiff = (step >> 3);

		if ( diff >= step ) 
		{
			delta = 4;
			diff -= step;
			vpdiff += step;
		}
		step >>= 1;
		if ( diff >= step  ) 
		{
			delta |= 2;
			diff -= step;
			vpdiff += step;
		}
		step >>= 1;
		if ( diff >= step ) 
		{
			delta |= 1;
			vpdiff += step;
		}
		/* Step 3 - Update previous value */
		if ( sign )
			valpred -= vpdiff;
		else
			valpred += vpdiff;

		/* Step 4 - Clamp previous value to 16 bits */
		if ( valpred > 32767 )
			valpred = 32767;
		else if ( valpred < -32768 )
			valpred = -32768;

		/* Step 5 - Assemble value, update index and step values */
		delta |= sign;

		index += gVimicroIndexTable[delta];
		if ( index < 0 ) index = 0;
		if ( index > 88 ) index = 88;
		step = gVimicroStepsizeTable[index];

		if ( bufferstep ) 
		{
			outputbuffer = delta & 0x0f;
		} 
		else 
		{
			outp = (signed char)(((delta << 4) & 0xf0) | outputbuffer);
			if (CBitFifo::Write(outp, gCharBitNumber) == gCharBitNumber) 
			{
				nLength++;
			}
			else
			{
				break;
			}
		}
		bufferstep = !bufferstep;
	}
	/* Output last step, if needed */
	if ( !bufferstep )
	{
		outp = (signed char)outputbuffer;
		if (CBitFifo::Write(outp, g4BitNumber) == g4BitNumber) 
		{
			nLength++;
		}
	}
	m_state.valprev = valpred;
	m_state.index = index;
	return nLength;
}


void C968VimicroEncoderAdpcm::InitState(void)
{
	m_state.index = 0;
	m_state.valprev = 0;
}


//=======================================================================================
//			class C968VimicroDecoderAdpcm
//=======================================================================================
C968VimicroDecoderAdpcm::C968VimicroDecoderAdpcm(int size, int order) : C968DecoderAdpcm(size, order)
{
	InitState();
}

C968VimicroDecoderAdpcm::~C968VimicroDecoderAdpcm(void)
{
}

void C968VimicroDecoderAdpcm::SetAdpcmState(const ADPCMSTATE& state)
{
	this->m_state.index = state.index;
	this->m_state.valprev = state.valprev;
}


int C968VimicroDecoderAdpcm::Read(char *buf, int len)
{
	return C968DecoderAdpcm::Read(buf, len);
}


int	C968VimicroDecoderAdpcm::Write(char *buf, int len)
{
	signed char *inp;	/* Input buffer pointer */
	short outp;		    /* output buffer pointer */
	int sign;			/* Current adpcm sign bit */
	int delta;			/* Current adpcm output value */
	int step;			/* Stepsize */
	int valpred;		/* Predicted value */
	int vpdiff;			/* Current change to valpred */
	int index;			/* Current step change index */
	int inputbuffer = 0;/* place to keep next 4-bit value */
	int bufferstep;		/* toggle between inputbuffer/input */
	int j;
	int nLength = 0;

	len <<= 1;         //adpcm actural length,multiply 2 ,char->4bit   
	inp = (signed char *)buf;
	valpred = m_state.valprev;
	index = m_state.index;
	step = gVimicroStepsizeTable[index];
	bufferstep = 0;	
	j = 0;

	for ( ; len > 0 ; len--, j++) 
	{
		/* Step 1 - get the delta value */
		if ( bufferstep ) 
		{
			delta = (inputbuffer >> 4) & 0xf;
		} 
		else 
		{
			inputbuffer = *inp++;
			delta = inputbuffer & 0xf;
		}

		bufferstep = !bufferstep;

		/* Step 2 - Find new index value (for later) */
		index += gVimicroIndexTable[delta];
		if ( index < 0 ) index = 0;
		if ( index > 88 ) index = 88;

		/* Step 3 - Separate sign and magnitude */
		sign = delta & 8;
		delta = delta & 7;

		/* Step 4 - Compute difference and new predicted value */
		/*
		** Computes 'vpdiff = (delta+0.5)*step/4', but see comment
		** in adpcm_coder.
		*/
		vpdiff = step >> 3;
		if ( delta & 4 ) vpdiff += step;
		if ( delta & 2 ) vpdiff += step>>1;
		if ( delta & 1 ) vpdiff += step>>2;

		if ( sign )
			valpred -= vpdiff;
		else
			valpred += vpdiff;

		/* Step 5 - clamp output value */
		if ( valpred > 32767 )
			valpred = 32767;
		else if ( valpred < -32768 )
			valpred = -32768;

		/* Step 6 - Update step value */
		step = gVimicroStepsizeTable[index];

		/* Step 7 - Output value */
		outp = (short)valpred;
		if (CBitFifo::Write(outp, gShortBitNumber) == gShortBitNumber) 
		{
			nLength += 2;
		}
		else
		{
			break;
		}
	}
	m_state.valprev = valpred;
	m_state.index = index;

	return nLength;

}



void C968VimicroDecoderAdpcm::InitState(void)
{
	m_state.index = 0;
	m_state.valprev = 0;
}

//=======================================================================================
//			class C968YamahaEncoderAdpcm
//=======================================================================================
C968YamahaEncoderAdpcm::C968YamahaEncoderAdpcm(int size,int bMorePrecise, int order) : C968EncoderAdpcm(size, order)
{
	m_bMorePrecise = bMorePrecise;
}

C968YamahaEncoderAdpcm::~C968YamahaEncoderAdpcm(void)
{

}

int C968YamahaEncoderAdpcm::Read(char *buf, int len)
{
	return C968EncoderAdpcm::Read(buf, len);
}

int C968YamahaEncoderAdpcm::Write(char *buf, int len)
{
	int signal,step;
	int data,val,diff;
	signed char dst;
	int length = len >> 2;
	int nLength = 0;
	step = 0x7f;
	signal = 0;
	short* src = (short*)buf;
	do 
	{
		if (m_bMorePrecise) 
		{
			diff = *src++ - signal;
			if ( diff < 0 )
			{
				val = ( (diff * 8) - step ) / ( 2 * step );
				val = -val;
				if ( val > 7 ) val = 7;
				val += 8;
				signal += ( (step * gYamahaDifflookup[val]) - 4 ) >> 3;
			}
			else
			{
				val = ( ( diff * 8 ) + step ) / ( 2 * step );
				if ( val > 7 ) val = 7;
				signal += ( (step*gYamahaDifflookup[val]) + 4 ) >> 3;
			}
		}
		else
		{
			diff = *src++ - signal;
			diff = (diff*8)/step;
			val = (abs(diff)) >> 1;
			if (val>7) val = 7;
			if (diff<0) val+=8;
			signal += (step*gYamahaDifflookup[val]) >> 3;
		}
		signal = LimitValue(signal,-32768,32767);     
		step = (step * gYamahaIndexScale[val]) >> 8;
		step = LimitValue(step,0x7F,0x6000);		
		data = val;
		if (m_bMorePrecise)
		{
			diff = *src++ - signal;
			if ( diff < 0 )
			{
				val = ( (diff * 8) - step ) / ( 2 * step );
				val = -val;
				if ( val > 7 ) val = 7;
				val += 8;

				signal += ( (step * gYamahaDifflookup[val]) - 4 ) >> 3;
			}
			else
			{
				val = ( ( diff * 8 ) + step ) / ( 2 * step );
				if ( val > 7 ) val = 7;
				signal += ( (step*gYamahaDifflookup[val]) + 4 ) >> 3;
			}
		}
		else
		{
			diff = *src++ - signal;
			diff = (diff*8)/step;

			val = (abs(diff))>>1;
			if (val>7) val = 7;
			if (diff<0) val+=8;
			signal += (step*gYamahaDifflookup[val]) >> 3;
		}
		signal = LimitValue(signal,-32768,32767);
		step = (step * gYamahaIndexScale[val]) >> 8;
		step = LimitValue(step,0x7F,0x6000);
		data = data + (val<<4);
		dst = (unsigned char)data;
		if (CBitFifo::Write(dst, gCharBitNumber) == gCharBitNumber) 
		{
			nLength++;
		}
		else
		{
			break;
		}
		length--;
	} while(length > 0);

	return nLength;
}



int C968YamahaEncoderAdpcm::LimitValue(int val,int min,int max)
{
	if (val<min)
		return min;
	else if(val>max)
		return max;
	else
		return val;
}

//=======================================================================================
//			class C968YamahaDecoderAdpcm
//=======================================================================================
C968YamahaDecoderAdpcm::C968YamahaDecoderAdpcm(int size, int bMorePrecise, int order) : C968DecoderAdpcm(size, order)
{
	m_bMorePrecise = bMorePrecise;
}

C968YamahaDecoderAdpcm::~C968YamahaDecoderAdpcm(void)
{
}

int C968YamahaDecoderAdpcm::Read(char *buf, int len)
{
	return C968DecoderAdpcm::Read(buf, len);
}

int C968YamahaDecoderAdpcm::Write(char *buf, int len)
{
	int signal, step;      
	int data, val;
	step = 0x7f;
	val = 0;
	data = 0;			
	signal = 0;
	unsigned char *src = (unsigned char *)buf;
	int length = len;
	int nLength = 0;
	short dst;
	do 
	{
		data = *src;
		src++;
		/* high nibble */
		val = data & 0x0f;
		if (m_bMorePrecise) 
		{
			if ( val & 8 ) 
				signal += ( (step * gYamahaDifflookup[val]) - 4 ) >> 3;
			else           
				signal += ( (step * gYamahaDifflookup[val]) + 4 ) >> 3;
		}
		else
		{
			signal += (step*gYamahaDifflookup[val]) >> 3;
		}
		signal = LimitValue(signal,-32768,32767);
		step = (step * gYamahaIndexScale[val])>>8;
		step = LimitValue(step,0x7f,0x6000);

		dst =  (short)signal;
		if (CBitFifo::Write(dst, gShortBitNumber) == gShortBitNumber)
		{
			nLength++;
		}
		else
		{
			break;
		}
		/* low nibble */
		val = (data>>4)&0x0f;
		if (m_bMorePrecise)
		{
			if ( val & 8 ) 
				signal += ( (step * gYamahaDifflookup[val]) - 4 ) >> 3;
			else
				signal += ( (step * gYamahaDifflookup[val]) + 4 ) >> 3;
		}
		else
		{
			signal += (step*gYamahaDifflookup[val]) >> 3;
		}
		signal = LimitValue(signal,-32768,32767);
		step = (step * gYamahaIndexScale[val])>>8;
		step = LimitValue(step,0x7f,0x6000);
		dst = (short)signal;
		if (CBitFifo::Write(dst, gShortBitNumber) == gShortBitNumber)
		{
			nLength++;
		}
		else
		{
			break;
		}
		length--;
	} while(length > 0);
	return nLength;
}

int C968YamahaDecoderAdpcm::LimitValue(int val,int min,int max)
{
	if (val<min)
		return min;
	else if(val>max)
		return max;
	else
		return val;
}


//=======================================================================================
//			class C968MsEncoderAdpcm
//=======================================================================================
//C968MsEncoderAdpcm::C968MsEncoderAdpcm(void)
//{
//
//}
//
//C968MsEncoderAdpcm::~C968MsEncoderAdpcm(void)
//{
//
//}
//
//int C968MsEncoderAdpcm::Read(char *buf, int len)
//{
//	if (len >= m_length) 
//	{
//		memcpy(buf, CBitFifo::m_buf, m_length);
//		return m_length;
//	}
//	else
//	{
//
//		memcpy(buf, CBitFifo::m_buf, len);
//		return len;
//	}
//}
//
//int C968MsEncoderAdpcm::Write(char *buf, int len)
//{
//
//	return 0;
//}


