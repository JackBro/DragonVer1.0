


#include "internal.h"

const int gShortBitNumber = 16;

//{{ C9683StereoDipole
const int gLEFTBITS   = 15;

const short gCrossFilter[gTCORDER]= {
		296,    -341,  -533,   -200,	259,     435,   76,    -248,
		-444,	-183,	262,	494,	115,	-435,	-437,	120,	
		510,	199,	-86,	-208,	-157,	-196,	311,	552,	
		343,	-1044,	111,	-533,	2020,	-1876,	1684,	-3154,
		-22753,	-3320,	648,	-1685,	467,	-1677,	599,	43,	
		583,	-165,	-666,	-888,	-538,	584,	315,	415,
		-464,	-509,	-140,	392,	459,	272,	-676,	-1191,
		-111,	769,	630,	124,	-150,	-351,	58,	    266,
		161,	-540,	-1020,	-1052,	81,     885,	557,	-280,
		-761,	-202,	355,	649,	-133,	-529,	-369,	230,
		886,	531,	68,	   -501,	-191,	230,	566,	384,	
		-278,	-522,	-328,	239,	564,	345,	-247,	-548,
		-253,	225,	532,	138,	-340,	-614,	-216,	297,	
		455,	119,	-517,	-558,	-114,	427,	556,	42,	
		-443,	-453,	59,	    582,	501,	13,  	-456,	-284,
		118,    532,	277,	-101,	-505,	-291,	226,	491		
};
//}} C9683StereoDipole


//{{ C968StereoExpansion
const short gLeftHrtf[gHRTFORDER] = {
		0,	    -1, 	-1, 	-1, 	-2 ,	-1, 	-1 ,	-1,
		-1,	    -1,	    1,	    0,	    -1,   	0,   	1,  	-2,
		1,	    -1,	    -4, 	-2, 	-2, 	5,  	0,  	-4,
		4,	    14,	    13, 	-2, 	12, 	-1,     -22,     1,	
		264,    714,	497,	371,	1052,	902,	467,	849,
		818,    431,	427,	778,	1925,	4124,	5322,	3495,
		800,    -90,	-294,	-1488,	-2613,	-2750,	-2916,	-3051,	
		-2421,	-1624,	-1177,	-826,	-252,	363,	687,	933,	
		1166,	1134,	842,	582,	512,	240,	-281,	-625,
		-777,	-884,	-1051,	-1178,	-1086,	-910,	-657,	-351,
		-149,	24, 	148,	204,	221,	170,	125,	52,
		-33,	-92,	-189,	-236,	-276,	-339,	-343,	-341,	
		-346,	-352,	-306,	-194,	-116,	-61,	-25,	0,
		29,	    43,	    28, 	-29,	-77,	-88,	-101,	-126,	
		-157,	-191,	-180,	-148,	-156,	-163,	-143,	-107,	
		-70,	-53,	-52,	-69,	-97,	-95,	-98,	-112		
};

const short gRightHrtf[gHRTFORDER] = {
		-383,	440,	-517,	4384,	18504,	-1277,	-25680,	-3987,	
		13608,	4287,	-278,	18987,	22293,	-7659,	-11512,	9082,	
		8952,	-6249,	-8470,	-4744,	-9739,	-7372,	468,	-2099,	
		-8427,	-6736,	1605,	3235,	-594,	2135,	5698,	4333,
		1743,	1865,	2012,	-950,	-2011,	-1384,	-1515,	-2783,
		-3975,	-3363,	-2941,	-2009,	-323,	438,	300,	-222,	
		231,	1063,	1142,	1034,	1289,	1284,	402,	-203,
		72,  	292,	-424,	-1048,	-1125,	-1383,	-1693,	-1448,	
		-516,	-156,	-343,	-372,	-47,	362,	398,	513,	
		845,	860,	348,	-12,	-43,	-233,	-450,	-201,
		46,	   -291,	-657,	-521,	-238,	-223,	-134,	202,	
		219,	-87,	-152,	-26,	75,	    177,	305,	211,	
		-139,	-425,	-402,	-219,	-95,	-15,	-99,	-299,	
		-568,	-566,	-201,	149,	197,	-46,	-203,	-285,	
		-259,	-79,	65,	    38, 	-120,	-288,	-357,	-302,
		-153,	12,	    44,  	-49,	-189,	-189,	-4,	    174		
};

//}} C968StereoExpansion

//=======================================================================================
//			class C9683StereoDipole
//=======================================================================================

C9683StereoDipole::C9683StereoDipole(int size, int order) : CBitFifo(size, order)
{
	m_tcPos = 0;
	m_posBuf = 0;
	this->InitBuf();	
}

C9683StereoDipole::~C9683StereoDipole(void)
{

}

int C9683StereoDipole::Read(char *buf, int len)
{
	return CBitFifo::Read(buf, len<<3)>>3;
}

int	C9683StereoDipole::Write(char *buf, int len)
{
	int i,j,k;
	long left,right;
	int leftTemp,rightTemp;
	int length = len>>2;
	int nLength = 0;
	short dstLeft;
	short dstRight;
	short* srcBuf = (short*)buf;
	for (i = 0; i < length; i++)
	{
		m_tcBuf[0][m_tcPos] = srcBuf[2*i];
		m_tcBuf[1][m_tcPos] = srcBuf[2*i+1];
		left = right =0;
		k= (m_tcPos +1 )% gTCORDER;

		for(j = gTCORDER-1; j >= 0; j--)
		{
			left += (int)m_tcBuf[1][k]*gCrossFilter[j];
			right += (int)m_tcBuf[0][k]*gCrossFilter[j];
			k = (++k)%gTCORDER;
		}
		left = (left>>gLEFTBITS);
		right = (right>>gLEFTBITS);
		k = m_tcPos-30;
		if(k<0) k+= gTCORDER;
		left += m_tcBuf[0][k];
		right += m_tcBuf[1][k];

		m_tcPos=(++m_tcPos)%gTCORDER;

		leftTemp = (left>>1)+(left>>2);
		rightTemp = (right>>1)+(right>>2);
		if(leftTemp>32767) leftTemp =32767;
		else if(leftTemp<-32768) leftTemp =-32768;
		if(rightTemp>32767) rightTemp = 32767;
		else if(rightTemp<-32768) rightTemp =-32768;

		dstLeft = (short)leftTemp;
		if (CBitFifo::Write(dstLeft, gShortBitNumber ) == gShortBitNumber)
		{
			nLength++;
		}
		else
		{
			break; 
		}
		dstRight = (short)rightTemp;
		if (CBitFifo::Write(dstRight, gShortBitNumber ) == gShortBitNumber)
		{
			nLength++;
		}
		else
		{
			break;
		}
	}
	return nLength;
}

int C9683StereoDipole::GetDataSize(void)
{
	return CBitFifo::GetDataSize()>>3;
}

void C9683StereoDipole::InitBuf(void)
{
	short i;
	for (i = 0; i < gTCORDER; i++)
	{
		m_tcBuf[0][i] = m_tcBuf[1][i] = 0;
	}
	for(i = 0; i < gHRTFORDER; i++)
		m_hrtfBuf[0] = 0;
}

//=======================================================================================
//			class C968ThreeDPosition
//=======================================================================================

C968ThreeDPosition::C968ThreeDPosition(int size, int order) : CBitFifo(size, order)
{
	m_posBuf = 0;
	m_bSuccess = FALSE;
	this->InitBuf();
}

C968ThreeDPosition::~C968ThreeDPosition(void)
{
}

int C968ThreeDPosition::Read(char *buf, int len)
{
	return CBitFifo::Read(buf, len<<3)>>3;
}

int C968ThreeDPosition::Write(char *buf, int len)
{
	long left,right;
	int i,j,k;
	int length = len >> 2;
	int nLength = 0;
	short* srcBuf = (short*)buf;
	short dstLeft;
	short dstRight;
	for(i = 0;i < length; i++)
	{
		m_hrtfBuf[m_posBuf]= (srcBuf [2*i]>>1)+(srcBuf [2*i+1]>>1);
		left = right =0;
		k=(m_posBuf+1)%gHRTFORDER;
		if (m_bSuccess) 
		{
			short* leftRightHrtfBuf = (short*)m_outHrtfBuf.m_buf;
			for(j = gHRTFORDER-1; j >= 0; j--)
			{
				left += (int)(leftRightHrtfBuf[2*j]*m_hrtfBuf[k]);
				right += (int)(leftRightHrtfBuf[2*j+1]*m_hrtfBuf[k]);
				k= (++k)%gHRTFORDER;
			}
		}
		else
		{
			for(j = gHRTFORDER-1; j >= 0; j--)
			{
				left += (int)gLeftHrtf[j]*m_hrtfBuf[k];
				right += (int)gRightHrtf[j]*m_hrtfBuf[k];
				k= (++k)%gHRTFORDER;
			}
		}
		m_posBuf = (++m_posBuf)%gHRTFORDER;
		left =(left>>16);
		right =(right>>16);
		left += (left>>1);
		right += (right>>1);
		if(left>32767) left =32767;
		else if(left<-32768) left=-32768;
		if(right>32767) right =32767;
		else if(right<-32768) right=-32768;
		dstLeft = (short)left;
		if (CBitFifo::Write(dstLeft, gShortBitNumber ) == gShortBitNumber)
		{
			nLength++;
		}
		else
		{
			break;
		}	
		dstRight = (short)right;
		if (CBitFifo::Write(dstRight, gShortBitNumber ) == gShortBitNumber)
		{
			nLength++;
		}
		else
		{
			break;
		}
	}
	return nLength;
}

void C968ThreeDPosition::SetHrtfData(char* buf, int len)
{
	if (len == 512) 
	{
		m_outHrtfBuf.SetSize(len);
		m_outHrtfBuf.Write(buf, len);
		m_bSuccess = TRUE;	
	}
	else
	{
		m_bSuccess = FALSE;		
	}	
}

int C968ThreeDPosition::GetDataSize()
{
	return CBitFifo::GetDataSize()>>3;
}

void C968ThreeDPosition::InitBuf(void)
{
	short i;
	for(i = 0; i < gHRTFORDER; i++)
		m_hrtfBuf[0] = 0;
}

