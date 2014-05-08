#ifndef _RGB2YIQ_H_
#define _RGB2YIQ_H_


#ifndef TOOLCONVERT_DLL
	#define TOOLCONVERT_DLL  __declspec(dllimport)
#endif


#include "toolbuf/fifobuf.h"

//#define IMAGEWIDTH 640
//#define IMAGEHEIGHT 480

//=============== class CRgb2yiq =================
class TOOLCONVERT_DLL CRgb2yiq : public CFifoBuf
{
public:
	CRgb2yiq(int cap = 256, int width = 640, int height = 480);

public:
	virtual int     Read(char *buf, int len);
	virtual int     Write(char *buf, int len);

public:
    int		GetSize(void);
	int		GetTotalSize(void);
	void	SetSize(int size = BUFFER_DEFAULT_SIZE);

	void  Process(char *rgb, char *yiq);

	void  SetCap(int cap);
	int   GetCap(void);
	void  SetImageWidth(int width);
	int   GetImageWidth(void);
	void  SetImageHight(int hight);
	int   GetImageHight(void);

	void  SetGamStartPt(int *gammaStartPoint);
	void  SetGamSlope(int *gammaSlope);
	
private:
	char  YGammaCorr(char yIn);
    char  Rgb2y(void);
	char  Rgb2i(void);
	char  Rgb2q(void);

private:
	int  m_cap;
	int  m_imageWidth;
	int  m_imageHight;
	int  m_gamStartPt[9];
	int  m_gamSlope[9];
};

#endif


