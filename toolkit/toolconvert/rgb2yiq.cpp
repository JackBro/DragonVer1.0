
#include "internal.h"

/**************************************class CRgb2yiq****************************************/
CRgb2yiq::CRgb2yiq(int cap,  int width, int height)
{
	this->m_cap = cap;

	this->m_imageWidth = width;
	this->m_imageHight = height;

	this->m_gamStartPt[0] = 0x00;
	this->m_gamStartPt[1] = 0x15;
	this->m_gamStartPt[2] = 0x20;
	this->m_gamStartPt[3] = 0x30;
	this->m_gamStartPt[4] = 0x49;
	this->m_gamStartPt[5] = 0x5e;
	this->m_gamStartPt[6] = 0x6f;
	this->m_gamStartPt[7] = 0xa9;
	this->m_gamStartPt[8] = 0xd7;

	this->m_gamSlope[0] = 0x30;
	this->m_gamSlope[1] = 0x2b;
	this->m_gamSlope[2] = 0x22;
	this->m_gamSlope[3] = 0x1a;
	this->m_gamSlope[4] = 0x14;
	this->m_gamSlope[5] = 0x11;
	this->m_gamSlope[6] = 0x0e;
	this->m_gamSlope[7] = 0x0b;
	this->m_gamSlope[8] = 0x0a;
}

int CRgb2yiq::Read(char *buf, int len)
{
	char y, i ,q;

	y = this->Rgb2y();
	i = this->Rgb2i();
	q = this->Rgb2q();
	y = this->YGammaCorr(y);
	this->m_buf[0] = y;
	this->m_buf[1] = i;
	this->m_buf[2] = q;

	return CFifoBuf::Read(buf, len);
}

int CRgb2yiq::Write(char *buf, int len)
{
	return CFifoBuf::Write(buf, len);
}

void CRgb2yiq::SetSize(int size)
{
    CFifoBuf::SetSize(size);
}

int	CRgb2yiq::GetSize(void)
{
	return CFifoBuf::GetSize();
}

int	CRgb2yiq::GetTotalSize(void)
{
	return CFifoBuf::GetTotalSize();
}

void  CRgb2yiq::SetCap(int cap)
{
	this->m_cap = cap;
}

int  CRgb2yiq::GetCap(void)
{
	return this->m_cap;
}

void  CRgb2yiq::SetImageWidth(int width)
{
	this->m_imageWidth = width;
}

int   CRgb2yiq::GetImageWidth(void)
{
	return this->m_imageWidth;
}

void  CRgb2yiq::SetImageHight(int hight)
{
	this->m_imageHight = hight;
}

int   CRgb2yiq::GetImageHight(void)
{
	return this->m_imageHight;
}

char CRgb2yiq::Rgb2y(void)
{
	int val;
	unsigned char  r, g, b;
	int cap = (this->m_cap - 1) << 8;

	r = (unsigned char)this->m_buf[0];
	g = (unsigned char)this->m_buf[1];
	b = (unsigned char)this->m_buf[2];

	val = 77 * r + 150 * g + 29 * b;
	val = val > cap ? cap : ( val < 0 ? 0 : val );	

	return (char)(val >> 8);
}

char CRgb2yiq::Rgb2i(void)
{
	int val;
	unsigned char  r, g, b;
	int cap = (this->m_cap - 1) << 8;

	r = (unsigned char)this->m_buf[0];
	g = (unsigned char)this->m_buf[1];
	b = (unsigned char)this->m_buf[2];

	val = 153 * r - 70 * g - 82 * b;	
	val = val > cap ? cap : ( val < 0 ? 0 : val );

	return (char)(val >> 8);
}

char CRgb2yiq::Rgb2q(void)
{
	int val;
	unsigned char  r, g, b;
	int cap = (this->m_cap - 1) << 8;

	r = (unsigned char)this->m_buf[0];
	g = (unsigned char)this->m_buf[1];
	b = (unsigned char)this->m_buf[2];

	val = 54 * r - 134 * g + 80 * b;
	val = val > cap ? cap : ( val < 0 ? 0 : val );

	return (char)(val >> 8);
}

void CRgb2yiq::Process(char *rgb, char *yiq)
{
	int imageSize = this->m_imageWidth * this->m_imageHight;

	this->SetSize(this->m_imageWidth * this->m_imageHight * 3);

	while(imageSize-- > 0)
	{
		this->Write(rgb, 3);
		this->Read(yiq, 3);

        rgb += 3;
		yiq += 3;
	}	
}

void  CRgb2yiq::SetGamStartPt(int *gammaStartPoint)
{
    if(gammaStartPoint != NULL)
		memcpy(this->m_gamStartPt, gammaStartPoint, sizeof(this->m_gamStartPt));
}

void  CRgb2yiq::SetGamSlope(int *gammaSlope)
{
    if(gammaSlope != NULL)
		memcpy(this->m_gamSlope, gammaSlope, sizeof(this->m_gamSlope));
}

char  CRgb2yiq::YGammaCorr(char yIn)
{
	 int  yStartPt, ySlope, yAdapt;
	 int  yMul, aXy, yOutW;

     int ySbBlk = yIn & 0x3f;
	 int yBSel0 = (yIn & 0xc3) & ySbBlk;
	 int yBSel1 = (yIn & 0xc7) & ySbBlk;
	 int yBSel2 = (yIn & 0xcf) & ySbBlk;
	 int yBSel3 = (yIn & 0xdf) & ySbBlk;
	 int yBSel4 = (yIn & 0xef) & ySbBlk;
	 int yBSel5 =  ySbBlk;

	 int yBSel6 = yIn & 0x7f;
	 int yBSel7 = yIn & 0xbf;

	 yStartPt = yBSel0 ? this->m_gamStartPt[0] :
	            yBSel1 ? this->m_gamStartPt[1] :
                yBSel2 ? this->m_gamStartPt[2] :
                yBSel3 ? this->m_gamStartPt[3] :
                yBSel4 ? this->m_gamStartPt[4] :
                yBSel5 ? this->m_gamStartPt[5] :
                yBSel6 ? this->m_gamStartPt[6] :
                yBSel7 ? this->m_gamStartPt[7] : this->m_gamStartPt[8];

	 ySlope   = yBSel0 ? this->m_gamSlope[0] :
	            yBSel1 ? this->m_gamSlope[1] :
                yBSel2 ? this->m_gamSlope[2] :
                yBSel3 ? this->m_gamSlope[3] :
                yBSel4 ? this->m_gamSlope[4] :
                yBSel5 ? this->m_gamSlope[5] :
                yBSel6 ? this->m_gamSlope[6] :
                yBSel7 ? this->m_gamSlope[7] : this->m_gamSlope[8];

	 yAdapt   = (!ySbBlk)         ? (yIn & 0x3f) :
	            yBSel2            ? (yIn & 0x7)  :
	            (yBSel1 | yBSel0) ? (yIn & 0x3)  : (yIn & 0xf);

	 yMul = ySlope * yAdapt;
	 aXy  = yBSel0 ? (yMul & 0x1ff8) : (yMul & 0x1ff0);
	 yOutW = yStartPt + aXy;

	 return (char)((yOutW & 0x300) ? 0xff : (yOutW & 0xff));
}

