#ifndef _TOOLV968ALG_AGC_H_
#define _TOOLV968ALG_AGC_H_


#ifndef TOOLV968ALG_DLL
	#define TOOLV968ALG_DLL  __declspec(dllimport)
#endif

#include "toolbit/bitfifo.h"


const int gOffsetZero     = 24;

class TOOLV968ALG_DLL C968Agc : public CBitFifo
{
public:
	C968Agc(int size = BUFFER_DEFAULT_SIZE, int order = SMALLBIT_ORDER);
	~C968Agc(void);

public:
	void           SetPara(int    nFrameLength = 256,
						   float  fLimiteFactor = 0,
						   int    nOption = 0,
						   int    nIndex = 0,
						   int    nUsbIndex = gOffsetZero,
						   int    nAgcIndex = 0,
						   short  nLastValue = 0);

	int            GetDataSize(void);
	virtual int	   Read(char *buf, int len);
	virtual int	   Write(char *buf, int len);

private:
	short CalcRefValue(short *pBuf, int nLength);
	void  VolControl(int nUsbIndex,int nIndex, short* pOutPcmBuf,int nNewLength);
private:
	int   m_nFrameLength;    //frame length,  m_nFrameLength = 128,256,...,1024,...
	float m_fLimiteFactor;   //limite parameter, it adjust swing value
	int   m_nOption;         //0--max value, 1--meanValue, 2--meanValue
	int   m_nIndex;          //
	int   m_nUsbIndex;       //
	int   m_nAgcIndex;       //
	short m_nLastValue;      //e(n-1)
};






#endif