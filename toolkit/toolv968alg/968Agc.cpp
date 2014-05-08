
#include "internal.h"


const int gIntpcoefFbits  = 13;
const int gMaxTableIndex = 133;
const int gScaleFactorAlph = 13; 

const int gNL = 1000;  //-50dB noise threhold
const int gMaxIndex  = 26;
const int gMaxValue  = 0x7FFC;

//usb nIndex Table[0,-55dB] step size 0.5dB
const short gUsbIndexTable[134] = {
		//use 3.13 fixed-point representation
		//12db 11.5db
		32612, 30788, 29066, 27440, 25905, 24456, 23088, 21796, 20577,
		19426, 18339, 17313, 16345, 15430, 14567, 13752, 12983, 12257,
		11571, 10924, 10313, 9736,  9191,  8677,  8192,  7733,  7301,
		6892,  6507,  6143,  5799,  5475,  5168,  4879,  4606,  4349,
		4105,  3876,  3659,  3454,  3261,  3078,  2906,  2744,  2590,
		2445,  2308,  2179,  2057,  1942,  1833,  1731,  1634,  1543,
		1456,  1375,  1298,  1225,  1157,  1092,  1031,  973,   919,
		867,   819,   773,   730,   689,   650,   614,   579,   547,
		516,   487,   460,   434,   410,   387,   365,   345,   326,
		307,   290,   274,   259,   244,   230,   217,   205,   194,
		183,   173,   163,   154,   145,   137,   129,   122,   115,
		109,   103,   97,    91,    86,    81,    77,    73,    68,
		65,    61,    57,    54,    51,    48,    46,    43,    41,
		38,    36,    34,    32,    30,    29,    27,    25,    24,
		23,    21,    20,    19,    18,    17,    16,    15//-54.5dB
};

const int gMappingTable[27] = {
		0,  4,  8,  12, 16, 20, 23, 27, 30, 
		33, 36, 40, 43, 46, 49, 52, 55, 58, 
		61, 64, 67, 70, 73, 76, 79, 82, 85
};




C968Agc::C968Agc(int size , int order) : CBitFifo(size, order)
{
	m_nFrameLength = 256;
	m_nOption = 0;
	m_nIndex = 0;
	m_nUsbIndex = gOffsetZero;
	m_nAgcIndex = 0;
	m_fLimiteFactor = (float)0.1;
	m_nLastValue = 0;  //e(n-1)
}

C968Agc::~C968Agc(void)
{
}

void C968Agc::SetPara(int    nFrameLength,
					  float  fLimiteFactor,
					  int    nOption,
					  int    nIndex,
					  int    nUsbIndex,
					  int    nAgcIndex,
					  short  nLastValue)
{
	m_nFrameLength = nFrameLength;
	m_fLimiteFactor = fLimiteFactor;
	m_nOption = nOption;
	m_nIndex = nIndex;
	m_nUsbIndex = nUsbIndex;
	m_nAgcIndex = nAgcIndex;
	m_nLastValue = nLastValue;
}

int C968Agc::GetDataSize(void)
{
	return CBitFifo::GetDataSize()>>3;
}

int C968Agc::Read(char *buf, int len)
{
	return CBitFifo::Read(buf, len<<3)>>3;
}


int C968Agc::Write(char *buf, int len)
{
	len = CBitFifo::Write(buf, len<<3)>>4; //>>3>>1
	short *pSrcBuf = (short*)this->m_buf;
	short nRefValue = 0;
	short LL,UL,UUL;
	float fFactor_1_5dB = float(pow(10.0,-1.5/20.0)); //-1.5dB
	LL  =  (short)(gMaxValue*(m_fLimiteFactor-0.2));
	UL  =  (short)(gMaxValue*(m_fLimiteFactor+0.1));
	UUL =  (short)(gMaxValue*0.60);
	short nRangeValue = (short)(gMaxValue*m_fLimiteFactor*fFactor_1_5dB);

	while (len > 0)
	{	
		//Calculate adjust factor for left channel
		if (len >= m_nFrameLength*2) 
		{
			nRefValue = CalcRefValue(pSrcBuf, m_nFrameLength);  
		}
		else
		{
			nRefValue = CalcRefValue(pSrcBuf, len);  
		}
		m_nIndex = gMappingTable[m_nAgcIndex]; 

		if(nRefValue < LL && nRefValue > gNL)
		{
			if(nRefValue < nRangeValue)
				m_nAgcIndex++;
		}
		else if(nRefValue > UUL) 
			m_nAgcIndex -= 2;
		else if(nRefValue > UL)
			m_nAgcIndex --;
		if(m_nAgcIndex > gMaxIndex)
			m_nAgcIndex = gMaxIndex;
		else if(m_nAgcIndex < 0)
			m_nAgcIndex = 0;
		if (len >= m_nFrameLength) 
		{
			VolControl(m_nUsbIndex, m_nIndex+6, pSrcBuf, m_nFrameLength);
			len -= m_nFrameLength;
			pSrcBuf += m_nFrameLength;
		}
		else
		{
			VolControl(m_nUsbIndex, m_nIndex+6, pSrcBuf, len);
			len = 0;
		}
	}
	return len;
}



void C968Agc::VolControl(int nUsbIndex, int nIndex, short* pOutPcmBuf,int nLength)
{
	short* pSample = (short *)pOutPcmBuf;
	int i;
	int value;
	short factor;

	nIndex += nUsbIndex;
	if(nIndex < 0)
		nIndex = 0;
	else if(nIndex > gMaxTableIndex)
		nIndex = gMaxTableIndex;
	factor = gUsbIndexTable[nIndex];
	for(i = 0; i < nLength; i++)
	{
		value=(int)*pSample;
		value=(value*factor)>>gIntpcoefFbits;
		if(value > 0x7fff)
			value = 0x7fff;
		if(value <- 32768)
			value = -32768;
		*pSample++ = (short)value;
	}
}




short C968Agc::CalcRefValue(short* pBuf, int nLength)
{
	short nFrameLast;
	short nCoef;
	short nFaction = (short)((1L<< gScaleFactorAlph)-1);
	int temp;
	int nCurValue; //e(n)
	int i;
	CBufferBase newBuf;
	newBuf.SetSize(nLength<<1);
	short* pNewBuf = (short*)newBuf.m_buf;
	memset(pNewBuf, 0, nLength*sizeof(short));
	//low pass filter for envelope detection
	nFrameLast = m_nLastValue;
	for(i = 0; i < nLength; i++)
	{
		temp=abs(pBuf[i]); //|x(n)|
		if(temp>nFrameLast)
			//alph=(short)(0.2*pow(2.0,gScaleFactorAlph));        //attack mode
			nCoef = (short)((26.0/128)*pow(2.0,gScaleFactorAlph));//attack mode
		else
			//alph=(short)(0.01*pow(2.0,gScaleFactorAlph)); 
			nCoef = (short)((1.0/128)*pow(2.0,gScaleFactorAlph)); 
		temp = (int)nCoef*temp+(nFaction-nCoef)*(int)nFrameLast;
		nCurValue = temp >> gScaleFactorAlph;
		pNewBuf[i] = (short)nCurValue;
		nFrameLast = (short)nCurValue;
	}
	m_nLastValue = nFrameLast;

	switch(m_nOption)
	{
	case 0: //max value
		{
			short maxValue = 0;
			short temp;
			for(i=0;i<m_nFrameLength;i++)
			{
				temp = pNewBuf[i];
				if(temp > maxValue)
					maxValue = temp; 
			}
			return maxValue;
		}
	case 1: //mean value
		{
			int sum=0;
			short meanValue;
			for(i = 0; i < m_nFrameLength; i += 2)
				sum += pNewBuf[i];
			meanValue = (short)(sum/(m_nFrameLength/2));
			return meanValue;
		}
	case 2://mean square root value		  
		{
			__int64 sum=0;
			short meanValue;
			int temp;
			for(i = 0; i < m_nFrameLength; i+=2)
			{
				temp = pNewBuf[i];
				sum += temp*temp;
			}
			meanValue = (short)(sqrt((float)sum)/(m_nFrameLength/2));
			return meanValue;
		}
	default:
		return 0;
	}
}
