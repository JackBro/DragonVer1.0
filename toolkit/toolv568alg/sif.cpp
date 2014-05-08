


#include "internal.h"

//==================== class CSifModule326 ===================

CSifModule326::CSifModule326()
{
}


/**************************************class CRgb2yiq326****************************************/
CRgb2yiq326::CRgb2yiq326(int cap, int width, int height)
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

void CRgb2yiq326::SetImageWidth(int width)
{
	this->m_imageWidth = width;
}

int  CRgb2yiq326::GetImageWidth(void)
{
	return this->m_imageWidth;
}

void CRgb2yiq326::SetImageHight(int hight)
{
	this->m_imageHight = hight;
}

int  CRgb2yiq326::GetImageHight(void)
{
	return this->m_imageHight;
}

int CRgb2yiq326::Read(char *buf, int len)
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

int CRgb2yiq326::Write(char *buf, int len)
{
	return CFifoBuf::Write(buf, len);
}

void CRgb2yiq326::SetSize(int size)
{
    CFifoBuf::SetSize(size);
}

int	CRgb2yiq326::GetSize(void)
{
	return CFifoBuf::GetSize();
}

int	CRgb2yiq326::GetTotalSize(void)
{
	return CFifoBuf::GetTotalSize();
}

void  CRgb2yiq326::SetCap(int cap)
{
	this->m_cap = cap;
}

int  CRgb2yiq326::GetCap(void)
{
	return this->m_cap;
}

char CRgb2yiq326::Rgb2y(void)
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

char CRgb2yiq326::Rgb2i(void)
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

char CRgb2yiq326::Rgb2q(void)
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

void CRgb2yiq326::Process(char *rgb, char *yiq)
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

void  CRgb2yiq326::SetGamStartPt(int *gammaStartPoint)
{
    if(gammaStartPoint != NULL)
		memcpy(this->m_gamStartPt, gammaStartPoint, sizeof(this->m_gamStartPt));
}

void  CRgb2yiq326::SetGamSlope(int *gammaSlope)
{
    if(gammaSlope != NULL)
		memcpy(this->m_gamSlope, gammaSlope, sizeof(this->m_gamSlope));
}

char  CRgb2yiq326::YGammaCorr(char yIn)
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

/**************************************class CYiq2Rgb326****************************************/
CYiq2rgb326::CYiq2rgb326(int cap, int width, int height)
{
    this->m_cap = cap;

	this->m_imageWidth = width;
	this->m_imageHight = height;
}

int CYiq2rgb326::Read(char *buf, int len)
{
	char r, g , b;

	r = this->Yiq2r();
	g = this->Yiq2g();
	b = this->Yiq2b();
	this->m_buf[0] = r;
	this->m_buf[1] = g;
	this->m_buf[2] = b;

	return CFifoBuf::Read(buf, len);
}

int CYiq2rgb326::Write(char *buf, int len)
{
	return CFifoBuf::Write(buf, len);
}

void CYiq2rgb326::SetSize(int size)
{
    CFifoBuf::SetSize(size);
}

int	CYiq2rgb326::GetSize(void)
{
	return CFifoBuf::GetSize();
}

int	CYiq2rgb326::GetTotalSize(void)
{
	return CFifoBuf::GetTotalSize();
}

void  CYiq2rgb326::SetCap(int cap)
{
	this->m_cap = cap;
}

int  CYiq2rgb326::GetCap(void)
{
	return this->m_cap;
}

void CYiq2rgb326::SetImageWidth(int width)
{
	this->m_imageWidth = width;
}

int  CYiq2rgb326::GetImageWidth(void)
{
	return this->m_imageWidth;
}

void CYiq2rgb326::SetImageHight(int hight)
{
	this->m_imageHight = hight;
}

int  CYiq2rgb326::GetImageHight(void)
{
	return this->m_imageHight;
}

char CYiq2rgb326::Yiq2r(void)
{
	int val;
	unsigned char  y, i, q;
	int cap = (this->m_cap - 1) << 8;

	y = (unsigned char)this->m_buf[0];
	i = (unsigned char)this->m_buf[1];
	q = (unsigned char)this->m_buf[2];

	val = 256 * y + 245 * i + 159 * q;

	val = val > cap ? cap : ( val < 0 ? 0 : val );	

	return (char)(val >> 8);
}

char CYiq2rgb326::Yiq2g(void)
{
	int val;
	unsigned char  y, i, q;
	int cap = (this->m_cap - 1) << 8;

	y = (unsigned char)this->m_buf[0];
	i = (unsigned char)this->m_buf[1];
	q = (unsigned char)this->m_buf[2];

	val = 256 * y - 70 * i - 166 * q;

	val = val > cap ? cap : ( val < 0 ? 0 : val );	

	return (char)(val >> 8);
}

char CYiq2rgb326::Yiq2b(void)
{
	int val;
	unsigned char  y, i, q;
	int cap = (this->m_cap - 1) << 8;

	y = (unsigned char)this->m_buf[0];
	i = (unsigned char)this->m_buf[1];
	q = (unsigned char)this->m_buf[2];

	val = 256 * y - 283 * i + 180 * q;

	val = val > cap ? cap : ( val < 0 ? 0 : val );	

	return (char)(val >> 8);
}

void CYiq2rgb326::Process(char *yiq, char *rgb)
{
	int imageSize = this->m_imageWidth * this->m_imageHight;

	this->SetSize(this->m_imageWidth * this->m_imageHight * 3);

	while(imageSize-- > 0)
	{
		this->Write(yiq, 3);
		this->Read(rgb, 3);

        rgb += 3;
		yiq += 3;
	}
}

/**************************************class CRgb2hsv326****************************************/
CRgb2hsv326::CRgb2hsv326(int cap, int width, int height, int size, int unit)
{
	this->m_cap = cap;
	
	this->m_imageWidth = width;
	this->m_imageHight = height;

	this->m_bufF = NULL;
	this->SetSize(size, unit);
}

CRgb2hsv326::~CRgb2hsv326()
{
	this->release();
}

void CRgb2hsv326::release(void)
{
	if(this->m_bufF)
		delete [](this->m_bufF);
	this->m_size = 0;
	this->m_totalsize = 0;
	this->m_bufF = NULL;

	CFifoBuf::release();
}

void CRgb2hsv326::SetSize(int size, int unit)
{
	int len;
    
	CFifoBuf::SetSize(size);

	unit = (unit < 1) ? 1 : unit;
	len = size * unit;
	if(len == 0)
		return;
	if(unit == this->m_unit)
	{
		if(this->m_size >= size)
			return;
	}
	this->release();
	this->m_bufF = new double[len];
    if(this->m_bufF == NULL)
		return;
	this->m_unit = unit;
	this->m_size = size;
	this->m_totalsize = len;
}

int	CRgb2hsv326::ReadF(double *bufF, int size)
{
	this->Rgb2hsv();

	size = size > this->m_totalsize ? this->m_totalsize : size;
	if( (size <= 0) || (bufF == NULL) )
		return 0;
	memcpy(bufF, this->m_bufF, size);
	return size;
}

int CRgb2hsv326::Read(char *buf, int len)
{
	return CFifoBuf::Read(buf, len);
}

int CRgb2hsv326::Write(char *buf, int len)
{
	return CFifoBuf::Write(buf, len);
}

void  CRgb2hsv326::SetCap(int cap)
{
	this->m_cap = cap;
}

int  CRgb2hsv326::GetCap(void)
{
	return this->m_cap;
}

void CRgb2hsv326::SetImageWidth(int width)
{
	this->m_imageWidth = width;
}

int  CRgb2hsv326::GetImageWidth(void)
{
	return this->m_imageWidth;
}

void CRgb2hsv326::SetImageHight(int hight)
{
	this->m_imageHight = hight;
}

int  CRgb2hsv326::GetImageHight(void)
{
	return this->m_imageHight;
}

void CRgb2hsv326::Rgb2hsv(void)
{
	double r, g, b, m, v, s, h ;
	int cap = this->m_cap - 1;

	r = (double)this->m_buf[0];   g = (double)this->m_buf[1];  b = (double)this->m_buf[2];
	
	v = triMax(r, g, b);
	m = triMin(r, g, b);

	/*v*/
	this->m_bufF[2] = v > cap ? cap : ( v < 0 ? 0 : v );

	/*s*/
	if(0.0 == v)
		s = 0.0;
	else
		s = (v - m) / v;

	this->m_bufF[1] = s > cap ? cap : ( s < 0 ? 0 : s );

	/*h*/
	if(0.0 == s)
		h = 100;
	else
	{
		if(r == v)
			h = (g - b) / (v - m);
		else
		{
			if(g == v)
				h = (b - r) / (v - m) + 2;
			else
				h = (r - g) / (v - m) + 4;
		}
	}

	h *= 60;
	if(h < 0.0)
		h += 360;

	this->m_bufF[0] = h > cap ? cap : ( h < 0 ? 0 : h );
}

void  CRgb2hsv326::Process(char *rgb, double *hsv)
{
    this->SetSize(this->m_imageWidth * this->m_imageHight);

    while(rgb != NULL && hsv != NULL)
	{
		this->Write(rgb, 3);
		this->ReadF(hsv, 3);

        rgb += 3;
		hsv += 3;
	}	
}

/**************************************class CHsv2rgb326****************************************/
CHsv2rgb326::CHsv2rgb326(int cap, int width, int height, int size, int unit)
{
	this->m_cap = cap;
	
	this->m_imageWidth = width;
	this->m_imageHight = height;

	this->m_bufF = NULL;
	this->SetSize(size, unit);
}

CHsv2rgb326::~CHsv2rgb326()
{
	this->release();
}

void CHsv2rgb326::release(void)
{
	if(this->m_bufF)
		delete [](this->m_bufF);
	this->m_size = 0;
	this->m_totalsize = 0;
	this->m_bufF = NULL;

	CFifoBuf::release();
}

void CHsv2rgb326::SetSize(int size, int unit)
{
	int len;
    
	CFifoBuf::SetSize(size);

	unit = (unit < 1) ? 1 : unit;
	len = size * unit;
	if(len == 0)
		return;
	if(unit == this->m_unit)
	{
		if(this->m_size >= size)
			return;
	}
	this->release();
	this->m_bufF = new double[len];
    if(this->m_bufF == NULL)
		return;
	this->m_unit = unit;
	this->m_size = size;
	this->m_totalsize = len;
}

int	CHsv2rgb326::WriteF(double *bufF, int size)
{
	size = size > this->m_totalsize ? this->m_totalsize : size;
	if( (size <= 0) || (bufF == NULL) )
		return 0;
	memcpy(this->m_bufF, bufF, size);
	return size;
}

int CHsv2rgb326::Read(char *buf, int len)
{
	this->Hsv2rgb();

	return CFifoBuf::Read(buf, len);
}

int CHsv2rgb326::Write(char *buf, int len)
{
	return CFifoBuf::Write(buf, len);
}

void  CHsv2rgb326::SetCap(int cap)
{
	this->m_cap = cap;
}

int  CHsv2rgb326::GetCap(void)
{
	return this->m_cap;
}

void CHsv2rgb326::SetImageWidth(int width)
{
	this->m_imageWidth = width;
}

int  CHsv2rgb326::GetImageWidth(void)
{
	return this->m_imageWidth;
}

void CHsv2rgb326::SetImageHight(int hight)
{
	this->m_imageHight = hight;
}

int  CHsv2rgb326::GetImageHight(void)
{
	return this->m_imageHight;
}

void CHsv2rgb326::Hsv2rgb(void)
{
    double h, s, v, r, g, b, i, f, p, q, t;
	int cap = this->m_cap - 1;

	h = this->m_bufF[0];  s = this->m_bufF[1];  v = this->m_bufF[2];

	if(0 == s)
	{
		r = v; g = v; b = v;
	}
	else
	{
		h /= 60.0;
        i = (float)floor(h);
		f = h - i;
        p = v - v * s;
		q = v - v * s * f;
		t = v - v * s + v * s * f;

		switch((int)i)
		{
		    case 0:
                r = v; g = t; b = p; break;
			case 1:
				r = q; g = v; b = p; break;
			case 2:
				r = p; g = v; b = t; break;
			case 3:
				r = p; g = q; b = v; break;
			case 4:
				r = t; g = p; b = v; break;
			case 5:
				r = v; g = p; b = q; break;
			default:
				break;
		}
	}

    this->m_buf[0] = (char)(r > cap ? cap : ( r < 0 ? 0 : r ));//r
	this->m_buf[1] = (char)(g > cap ? cap : ( g < 0 ? 0 : g ));//g
	this->m_buf[2] = (char)(b > cap ? cap : ( b < 0 ? 0 : b ));//b
}

void  CHsv2rgb326::Process(double *hsv, char *rgb)
{
	this->SetSize(this->m_imageWidth * this->m_imageHight);

	while(rgb != NULL && hsv != NULL)
	{
		this->WriteF(hsv, 3);
		this->Read(rgb, 3);

        rgb += 3;
		hsv += 3;
	}	
}

/**************************************class CMotionDect326****************************************/
CMotionDect326::CMotionDect326(int width, int height, int size, int unit)
{
	this->m_imageWidth = width;
	this->m_imageHight = height;

	this->m_sleWinWidth = width >> 2;
	this->m_sleWinHight = height >> 2;

	this->m_sumInLineThre = 100;
	this->m_lineNumThre = 33;

	this->m_buf1 = NULL;
	this->SetSize(size, unit);
}

CMotionDect326::~CMotionDect326()
{
	this->release();
}

void CMotionDect326::release(void)
{
	if(this->m_buf1)
		delete [](this->m_buf1);
	this->m_size = 0;
	this->m_totalsize = 0;
	this->m_buf1 = NULL;

	CBufferBase::release();
}

void CMotionDect326::SetSize(int size, int unit)
{
	int len;
    
	CBufferBase::SetSize(size);

	unit = (unit < 1) ? 1 : unit;
	len = size * unit;
	if(len == 0)
		return;
	if(unit == this->m_unit)
	{
		if(this->m_size >= size)
			return;
	}
	this->release();
	this->m_buf1 = new char[len];
    if(this->m_buf1 == NULL)
		return;
	this->m_unit = unit;
	this->m_size = size;
	this->m_totalsize = len;
}

int CMotionDect326::Read(char *buf, int len)
{
	return CBufferBase::Read(buf, len);
}

int CMotionDect326::Write(char *buf, char *buf1, int size)
{
	CBufferBase::Write(buf, size);

	size = size > this->m_totalsize ? this->m_totalsize : size;
	if( (size <= 0) || (buf1 == NULL) )
		return 0;
	memcpy(this->m_buf1, buf1, size);
	return size;
}

void CMotionDect326::SetImageWidth(int width)
{
	this->m_imageWidth = width;
}

int  CMotionDect326::GetImageWidth(void)
{
	return this->m_imageWidth;
}

void CMotionDect326::SetImageHight(int hight)
{
	this->m_imageHight = hight;
}

int  CMotionDect326::GetImageHight(void)
{
	return this->m_imageHight;
}

void CMotionDect326::SetSleekWindowWidth(char width)
{
	this->m_sleWinWidth = width;
}

char CMotionDect326::GetSleekWindowWidth(void)
{
	return this->m_sleWinWidth;
}

void CMotionDect326::SetSleekWindowHight(char hight)
{
	this->m_sleWinHight = hight;
}

char CMotionDect326::GetSleekWindowHight(void)
{
	return this->m_sleWinHight;
}

void CMotionDect326::SetSumInLineThre(int sumThre)
{
	this->m_sumInLineThre = sumThre;
}

int  CMotionDect326::GetSumInLineThre(void)
{
	return this->m_sumInLineThre;
}

void CMotionDect326::SetLineNumThre(int lineNumThre)
{
	this->m_lineNumThre = lineNumThre;
}

int  CMotionDect326::GetLineNumThre(void)
{
	return this->m_lineNumThre;
}

//pPrevFrame is point to one color channel of previous image
//pCurFrame is point to one color channel of current image
unsigned int CMotionDect326::MotionDetection(void)
{
	int i,j,k,l, linenum, thesum1, thesum2;
	char *pprevh;
	char *pcurh;
	unsigned int theflag = 0;
	char xSplit, ySplit;


	if(this->m_buf == NULL || this->m_buf1 == NULL || this->m_imageWidth == 0 || this->m_imageHight == 0 ||
	   this->m_sleWinWidth == 0 || this->m_sleWinHight == 0)
		return 0;

    /* n * m windows */
	xSplit = this->m_imageWidth / this->m_sleWinWidth;
	ySplit = this->m_imageHight / this->m_sleWinHight;

	for(i = 0; i < ySplit; i++)
		for(j = 0; j < xSplit; j++)
		{
			/*detect motion in split window*/
			linenum = 0;
			pprevh = this->m_buf + i * this->m_imageWidth * this->m_sleWinHight + j * this->m_sleWinWidth;
			pcurh  = this->m_buf1 + i * this->m_imageWidth * this->m_sleWinHight + j * this->m_sleWinWidth;

			for(k = 0; k < this->m_sleWinHight; k++)
			{
				thesum1 = 0;
				thesum2 = 0;

				for(l = 0; l < this->m_sleWinWidth; l++)
				{
					thesum1 += *(pprevh + k * this->m_imageWidth + l);
					thesum2 += *(pcurh + k * this->m_imageWidth + l);
				}
				if(abs(thesum1 - thesum2) > this->m_sumInLineThre)
					linenum++;
			}
			
			if(linenum > this->m_lineNumThre)
			{
				theflag = theflag | (1 << (i * xSplit + j));
			}
		}

	return theflag;			 	
}

unsigned int CMotionDect326::Process(char * pPrevFrame, char * pCurFrame)
{
	this->SetSize(this->m_imageWidth * this->m_imageHight * this->m_unit);

	this->Write(pPrevFrame, pCurFrame, this->m_imageWidth * this->m_imageWidth * this->m_unit);

	return this->MotionDetection();
}

/************************************************class CYuvOrdTrans326******************************************/
CYuvOrdTrans326::CYuvOrdTrans326(int width, int height, char inOrd, char outOrd)
{
	this->m_imageWidth = width;
	this->m_imageHight = height;
	this->m_inOrder = inOrd;
	this->m_outOrder = outOrd;
}

void CYuvOrdTrans326::SetInOrder(char order)
{
	this->m_inOrder = order;
}

char CYuvOrdTrans326::GetInOrder(void)
{
	return this->m_inOrder;
}

void CYuvOrdTrans326::SetOutOrder(char order)
{
	this->m_outOrder = order;
}

char CYuvOrdTrans326::GetOutOrder(void)
{
	return this->m_outOrder;
}

void CYuvOrdTrans326::SetImageWidth(int width)
{
	this->m_imageWidth = width;
}

int  CYuvOrdTrans326::GetImageWidth(void)
{
	return this->m_imageWidth;
}

void CYuvOrdTrans326::SetImageHight(int hight)
{
	this->m_imageHight = hight;
}

int  CYuvOrdTrans326::GetImageHight(void)
{
	return this->m_imageHight;
}

int CYuvOrdTrans326::Read(char *buf, int len)
{
	if(this->m_inOrder == VYUY_ORDER || this->m_outOrder == UYVY_ORDER)
	    this->Vyuy2Uyvy();
	else if(this->m_inOrder == YUYV_ORDER || this->m_outOrder == UYVY_ORDER)
	    this->Yuyv2Uyvy();
	else if(this->m_inOrder == YVYU_ORDER || this->m_outOrder == UYVY_ORDER)
	    this->Yvyu2Uyvy();

	return CFifoBuf::Read(buf, len);
}

int CYuvOrdTrans326::Write(char *buf, int len)
{
	return CFifoBuf::Write(buf, len);
}

void CYuvOrdTrans326::Vyuy2Uyvy(void)
{
	char uyvy[4];

	uyvy[0] = this->m_buf[2];
	uyvy[1] = this->m_buf[1];
	uyvy[2] = this->m_buf[0];
    uyvy[3] = this->m_buf[3];

	memcpy(this->m_buf, uyvy, sizeof(uyvy));
}

void CYuvOrdTrans326::Yuyv2Uyvy(void)
{
	char uyvy[4];

	uyvy[0] = this->m_buf[1];
	uyvy[1] = this->m_buf[0];
	uyvy[2] = this->m_buf[3];
    uyvy[3] = this->m_buf[2];

	memcpy(this->m_buf, uyvy, sizeof(uyvy));
}

void CYuvOrdTrans326::Yvyu2Uyvy(void)
{
	char uyvy[4];

	uyvy[0] = this->m_buf[3];
	uyvy[1] = this->m_buf[0];
	uyvy[2] = this->m_buf[1];
    uyvy[3] = this->m_buf[2];

	memcpy(this->m_buf, uyvy, sizeof(uyvy));
}

void CYuvOrdTrans326::Process(char *src, char *dst)
{
	char *locSrc;
	char *locDst;

	locSrc = src;
	locDst = dst;

	this->SetSize(this->m_imageWidth * this->m_imageHight);

    while(src != NULL && dst != NULL)
	{
	    this->Write(locSrc, 4);
	    this->Read(locDst, 4);

        locSrc += 4;
		locDst += 4;
	}
}

/************************************************class CHistogram326******************************************/
CHistogram326::CHistogram326(int width, int height, int size, int unit)
{
	this->m_imageWidth = width;
	this->m_imageHight = height;
	this->m_areaMaxVal = 256;
	this->m_areaNum = 64;
	this->m_critPerVal = 1;
	this->m_minDelVal = 192;
	this->m_totalAreaLen = 256;
	this->m_preFramNum = 3;


	this->m_buf1 = NULL;
	this->SetSize(size, unit);
}

CHistogram326::~CHistogram326()
{
	this->release();
}

void CHistogram326::release(void)
{
	if(this->m_buf1)
		delete [](this->m_buf1);
	this->m_size = 0;
	this->m_totalsize = 0;
	this->m_buf1 = NULL;

	CBufferBase::release();
}

void CHistogram326::SetSize(int size, int unit)
{
	int len;
    
	CBufferBase::SetSize(size);

	unit = (unit < 1) ? 1 : unit;
	len = size * unit;
	if(len == 0)
		return;
	if(unit == this->m_unit)
	{
		if(this->m_size >= size)
			return;
	}
	this->release();
	this->m_buf1 = new char[len];
    if(this->m_buf1 == NULL)
		return;
	this->m_unit = unit;
	this->m_size = size;
	this->m_totalsize = len;
}

int CHistogram326::Read(char *buf, int size)
{
	return CBufferBase::Read(buf, size);
}

int CHistogram326::Write(char *buf, char *buf1, int size)
{
	CBufferBase::Write(buf, size);

	size = size > this->m_totalsize ? this->m_totalsize : size;
	if( (size <= 0) || (buf1 == NULL) )
		return 0;
	memcpy(this->m_buf1, buf1, size);
	return size;
}

void CHistogram326::SetImageWidth(int width)
{
	this->m_imageWidth = width;
}

int  CHistogram326::GetImageWidth(void)
{
	return this->m_imageWidth;
}

void CHistogram326::SetImageHight(int hight)
{
	this->m_imageHight = hight;
}

int  CHistogram326::GetImageHight(void)
{
	return this->m_imageHight;
}

void CHistogram326::SetAreaMaxVal(int areaMaxVal)
{
	this->m_areaMaxVal = areaMaxVal;
}

int  CHistogram326::GetAreaMaxVal(void)
{
	return this->m_areaMaxVal;
}

void CHistogram326::SetCritPerVal(char critPerVal)
{
	this->m_critPerVal = critPerVal;
}

char CHistogram326::GetCritPerVal(void)
{
	return this->m_critPerVal;
}

void CHistogram326::SetTotalAreaLen(int areaTotLen)
{
	this->m_totalAreaLen = areaTotLen;
}

int CHistogram326::GetTotalAreaLen(void)
{
	return this->m_totalAreaLen;
}

void CHistogram326::SetAreaNum(int areaNum)
{
	this->m_areaNum = areaNum;
}

int CHistogram326::GetAreaNum(void)
{
	return this->m_areaNum;
}

void CHistogram326::SetMinDelVal(int minDelVal)
{
	this->m_minDelVal = minDelVal;
}

int CHistogram326::GetMinDelVal(void)
{
	return this->m_minDelVal;
}

void CHistogram326::SetPreFramNum(char preFramNum)
{
	this->m_preFramNum = preFramNum;
}

char CHistogram326::GetPreFramNum(void)
{
	return this->m_preFramNum;
}

void CHistogram326::AreaPixStat(int *statRes)
{
    char areaLen, areaIdx;
	char *locImageData = this->m_buf;

	if(this->m_areaNum == 0 || this->m_totalAreaLen == 0 || this->m_buf == NULL || statRes == NULL)
		return;

	areaLen = this->m_totalAreaLen / this->m_areaNum;
	if(areaLen == 0)
		return;

	while(locImageData != NULL)
	{
		areaIdx = (this->m_areaMaxVal - *locImageData) / areaLen;
			
		if(areaIdx < this->m_areaNum)
            statRes[areaIdx]++;

		locImageData++;
	}
}

char CHistogram326::GetAreaIdx(int *statRes)
{
    char areaIdx = 0;
	int  statAccu = 0;
	int *locAreaStat = statRes;
 
	if(this->m_buf == NULL || this->m_imageWidth == 0 || this->m_imageHight == 0 || this->m_critPerVal == 0)
		return 0;

	while(locAreaStat != NULL)
	{
		statAccu += *locAreaStat;
		if(statAccu > (this->m_critPerVal * this->m_imageWidth * this->m_imageHight / 100))
			return areaIdx;

		locAreaStat++;
		areaIdx++;
	}

	return areaIdx;
}

char CHistogram326::GetCorCoef(void)
{
	char areaLen, areaIdx, corCoef;
	int  areaPixNumStat[256];
	
	if(this->m_buf == NULL || this->m_imageWidth == 0 || this->m_imageHight == 0 || this->m_critPerVal == 0 
		|| this->m_minDelVal == 0 || this->m_areaNum == 0)
		return 0;
	
	areaLen = (256 - this->m_minDelVal) / this->m_areaNum;

	this->AreaPixStat(areaPixNumStat);
	areaIdx = this->GetAreaIdx(areaPixNumStat);

	if(areaIdx >= this->m_areaNum)
		corCoef = this->m_minDelVal;
	else
		corCoef = 256 - areaIdx * areaLen; 

	return corCoef;
}

char CHistogram326::GetCorCoefMulFram(void)
{
	int  wSum, wCorSum;
	char *locPreCorCoef = this->m_buf;
	char *locW = this->m_buf1;

	if(locPreCorCoef != NULL || locW != NULL)
		return 0;

	wSum = 0; wCorSum = 0;

	while(locW != NULL)
	{
		wSum += *locW;
		locW++;
	}

	locW = this->m_buf1;
	while(locPreCorCoef != NULL && locW != NULL)
	{
		wCorSum += (*locW) * (*locPreCorCoef);
		locPreCorCoef++;
		locW++;
	}

    if(wSum == 0)
		return 0;
	
    return wCorSum / wSum;
}

char CHistogram326::ProcessGetCorCoef(char *imageData)
{
	this->SetSize(this->m_imageWidth * this->m_imageHight * this->m_unit);
	this->Write(imageData, NULL, this->m_imageWidth * this->m_imageHight * this->m_unit);
	return this->GetCorCoef();
}

char CHistogram326::ProcessGetCorCoefMulFram(char *preCorCoef, char *w)
{
	this->SetSize(this->m_imageWidth * this->m_imageHight * this->m_unit);
    this->Write(preCorCoef, w, this->m_preFramNum);
	return this->GetCorCoefMulFram();
}

void CHistogram326::ProcessCalHistpgram(char *imageData, int *histSta)
{
	this->SetSize(this->m_imageWidth * this->m_imageHight * this->m_unit);
    this->Write(imageData, NULL, this->m_imageWidth * this->m_imageHight * this->m_unit);
	this->AreaPixStat(histSta);
}

/************************************************class CAe326******************************************/
CAe326::CAe326(int width, int height)
{
	this->m_imageWidth = width;
	this->m_imageHight = height;
	this->m_imageWidth = width >> 2;
	this->m_sleWinHight = height >> 2;

	this->m_sleWinWeight[0] = 16;
	this->m_sleWinWeight[1] = 16;
	this->m_sleWinWeight[2] = 16;
	this->m_sleWinWeight[3] = 16;
	this->m_sleWinWeight[4] = 16;
	this->m_sleWinWeight[5] = 16;
	this->m_sleWinWeight[6] = 16;
	this->m_sleWinWeight[7] = 16;
	this->m_sleWinWeight[8] = 16;
	this->m_sleWinWeight[9] = 16;
	this->m_sleWinWeight[10] = 16;
	this->m_sleWinWeight[11] = 16;
	this->m_sleWinWeight[12] = 16;
	this->m_sleWinWeight[13] = 16;
	this->m_sleWinWeight[14] = 16;
	this->m_sleWinWeight[15] = 16;
}

CAe326::~CAe326()
{

}

int CAe326::Read(char *buf, int len)
{
	return CBufferBase::Read(buf, len);
}

int CAe326::Write(char *buf, int len)
{
	return CBufferBase::Write(buf, len);
}

void CAe326::SetImageWidth(int width)
{
	this->m_imageWidth = width;
}

int  CAe326::GetImageWidth(void)
{
	return this->m_imageWidth;
}

void CAe326::SetImageHight(int hight)
{
	this->m_imageHight = hight;
}

int  CAe326::GetImageHight(void)
{
	return this->m_imageHight;
}

void CAe326::SetSleekWindowWidth(char width)
{
	this->m_sleWinWidth = width;
}

char CAe326::GetSleekWindowWidth(void)
{
	return this->m_sleWinWidth;
}

void CAe326::SetSleekWindowHight(char hight)
{
	this->m_sleWinHight = hight;
}

char CAe326::GetSleekWindowHight(void)
{
	return this->m_sleWinHight;
}

void CAe326::SetSleekWindowWeight(char *weight)
{
	if(weight != NULL)
	    memcpy(this->m_sleWinWeight, weight, SLEEKWINDOWNUM);
}

void CAe326::GetSleekWindowWeight(char *weight)
{
	if(weight != NULL)
	    memcpy(weight, this->m_sleWinWeight, SLEEKWINDOWNUM);
}

int  CAe326::GetSumVal(char *pData, int eleNum)
{
	int  sumVal = 0;
	char *locData = pData;

	while(eleNum--)
	{
		sumVal += *locData;
		locData++;
	}

	return sumVal;
}

char CAe326::GetImageDatMean(void)
{
    int i, j, k, l, temSumVal, sumVal, totalMean, theMean[SLEEKWINDOWNUM];
	char *pLocData;
	char xSplit, ySplit;


	if(this->m_buf == NULL || this->m_imageWidth == 0 || this->m_imageHight == 0 ||
	   this->m_sleWinWidth == 0 || this->m_sleWinHight == 0 || SLEEKWINDOWNUM == 0)
		return 0;

    /* n * m windows */
	xSplit = this->m_imageWidth / this->m_sleWinWidth;
	ySplit = this->m_imageHight / this->m_sleWinHight;

	for(i = 0; i < ySplit; i++)
		for(j = 0; j < xSplit; j++)
		{
			/*Get y mean statics in split window*/
			pLocData = this->m_buf + i * this->m_imageWidth * this->m_sleWinHight + j * this->m_sleWinWidth;
	
			sumVal = 0;

			for(k = 0; k < this->m_sleWinHight; k++)
			{
				temSumVal = GetSumVal(pLocData + k * this->m_imageWidth, this->m_sleWinWidth);
			    sumVal += temSumVal;
			}

			theMean[j + i * xSplit] = temSumVal / (this->m_sleWinHight * this->m_sleWinWidth);
		}

	for(l = 0; l < SLEEKWINDOWNUM; l++)
		sumVal = (theMean[l] * this->m_sleWinWeight[l]) >> 8;

    totalMean = sumVal / SLEEKWINDOWNUM;

	return totalMean;	
}

char  CAe326::Process(char *imageData)
{    
	this->SetSize(this->m_imageWidth * this->m_imageHight * this->m_unit);
	this->Write(this->m_buf, this->m_imageWidth * this->m_imageHight);

	return this->GetImageDatMean();
}

/************************************************class CAwb326******************************************/
CAwb326::CAwb326(int width, int height)
{
	this->m_imageWidth = width;
	this->m_imageHight = height;

	this->m_yTop = 0xd0;
	this->m_yBot = 0x0;
	this->m_iTop = 0x10;
	this->m_qTop = 0x18;
	this->m_rGain = 0x30;
	this->m_bGain = 0x30;
	this->m_gainTop = 0x3f;
	this->m_gainBot = 0x14;
}

CAwb326::~CAwb326()
{

}

int CAwb326::Read(char *buf, int len)
{
	char rgbMean[3];

	this->GetRGBMean(rgbMean);
	this->GetRBGain(rgbMean);
    memcpy(this->m_buf, rgbMean, 3);

	return CBufferBase::Read(buf, len);
}

int CAwb326::Write(char *buf, int len)
{
	return CBufferBase::Write(buf, len);
}

void CAwb326::SetImageWidth(int width)
{
	this->m_imageWidth = width;
}

int  CAwb326::GetImageWidth(void)
{
	return this->m_imageWidth; 
}

void CAwb326::SetImageHight(int hight)
{
	this->m_imageHight = hight;
}

int  CAwb326::GetImageHight(void)
{
	return this->m_imageHight;
}

void CAwb326::SetYTopVal(int yTop)
{
	this->m_yTop = yTop;
}

int CAwb326::GetYTopVal(void)
{
    return this->m_yTop;
}

void CAwb326::SetYBotVal(int yBot)
{
	this->m_yBot = yBot;
}

int CAwb326::GetYBotVal(void)
{
	return this->m_yBot;
}

void CAwb326::SetITopVal(int iTop)
{
	this->m_iTop = iTop;
}

int CAwb326::GetITopVal(void)
{
	return this->m_iTop;
}

void CAwb326::SetQTopVal(int qTop)
{
	this->m_qTop = qTop;
}

int CAwb326::GetQTopVal(void)
{
	return this->m_qTop;
}

void CAwb326::SetRGainVal(char rGain)
{
	this->m_rGain = rGain;
}

char CAwb326::GetRGainVal(void)
{
	return this->m_rGain;
}

void CAwb326::SetBGainVal(char bGain)
{
	this->m_bGain = bGain;
}

char CAwb326::GetBGainVal(void)
{
	return this->m_bGain;
}

void CAwb326::SetGainTopVal(char gainTop)
{
	this->m_gainTop = gainTop;
}

char CAwb326::GetGainTopVal(void)
{
	return this->m_gainTop;
}

void CAwb326::SetGainBotVal(char gainBot)
{
	this->m_gainBot = gainBot;
}

char CAwb326::GetGainBotVal(void)
{
	return this->m_gainBot;
}

char  CAwb326::CheckSumConValid(char *rgb)
{
	CRgb2yiq326 m_rgb2yiq;

	char *locRgb = rgb;
	char yiq[3];

	m_rgb2yiq.Process(locRgb, yiq);

	/*y valid judge*/
	if(yiq[0] > this->m_yTop || yiq[0] < this->m_yBot)
		return FALSE;

	/*i valid judge*/
	if(yiq[1] > this->m_iTop)
		return FALSE;

	/*q valid judge*/
	if(yiq[2] > this->m_qTop)
		return FALSE;

	return TRUE;
}

void CAwb326::GetRGBMean(char *rgbMean)
{
	int rSum = 0, gSum = 0, bSum = 0, awbCnt = 0;
	char *locRgb = this->m_buf;

    while(locRgb != NULL)
	{
		if(TRUE == CheckSumConValid(locRgb))
		{
            rSum += *locRgb;
			gSum += *(locRgb + 1);
			bSum += *(locRgb + 2);
            awbCnt++;
		}

		locRgb += 3;
	}

	if(0 != awbCnt)
	{
        rgbMean[0] = rSum / awbCnt;   //rMean
		rgbMean[1] = gSum / awbCnt;   //gMean
		rgbMean[2] = bSum / awbCnt;   //bMean
	}
	else
	{
        rgbMean[0] = 0;
		rgbMean[1] = 0;
		rgbMean[2] = 0;
	}
}

void  CAwb326::GetRBGain(char *rgbMean)
{
    char rGainNew, bGainNew;
	char *locRgbMean = rgbMean;

	if(locRgbMean[0] > locRgbMean[1])   //rMean > gMean
		rGainNew = this->m_rGain + 1;
	else
		rGainNew = this->m_rGain - 1;

	if(locRgbMean[2] > locRgbMean[1])   //bMean > gMean
		bGainNew = this->m_bGain + 1;
	else
		bGainNew = this->m_bGain - 1;

	if(rGainNew > this->m_gainTop)      //new r gain
        this->m_rGain = this->m_gainTop;
	else if(rGainNew < this->m_gainBot)
        this->m_rGain = this->m_gainBot;
	else
		this->m_rGain = rGainNew;

	if(bGainNew > this->m_gainTop)      //new b gain 
        this->m_bGain = this->m_gainTop;
	else if(bGainNew < this->m_gainBot)
        this->m_bGain = this->m_gainBot;
	else
		this->m_bGain = bGainNew;
}

void CAwb326::Process(char *pImageDat, char *pAwbOut)
{
	if(this->m_buf == NULL || this->m_imageWidth == 0 || this->m_imageHight == 0)
		return;

	this->SetSize(this->m_imageWidth * this->m_imageHight * this->m_unit);
	this->Write(pImageDat, this->m_imageWidth * this->m_imageHight * 3);
	this->Read(pAwbOut, 3);  //output r,g,b mean	
}

/************************************************class CAutoFlick326*****************************************/
CAutoFlick326::CAutoFlick326(int width, int height, int size, int unit)
{
	this->m_imageWidth = width;
	this->m_imageHight = height;
	this->m_afMode = 1;
	this->m_afStep = 1;
	this->m_afStart = 0;
	this->m_afThre = 0xff;

	this->m_buf1 = NULL;
	this->SetSize(size, unit);
}

CAutoFlick326::~CAutoFlick326()
{
	this->release();
}

void CAutoFlick326::release(void)
{
	if(this->m_buf1)
		delete [](this->m_buf1);
	this->m_size = 0;
	this->m_totalsize = 0;
	this->m_buf1 = NULL;

	CBufferBase::release();
}

void CAutoFlick326::SetSize(int size, int unit)
{
	int len;
    
	CBufferBase::SetSize(size);

	unit = (unit < 1) ? 1 : unit;
	len = size * unit;
	if(len == 0)
		return;
	if(unit == this->m_unit)
	{
		if(this->m_size >= size)
			return;
	}
	this->release();
	this->m_buf1 = new char[len];
    if(this->m_buf1 == NULL)
		return;
	this->m_unit = unit;
	this->m_size = size;
	this->m_totalsize = len;
}

int CAutoFlick326::Read(char *buf, int size)
{
	return CBufferBase::Read(buf, size);
}

int CAutoFlick326::Write(char *buf, char *buf1, int size)
{
	CBufferBase::Write(buf, size);

	size = size > this->m_totalsize ? this->m_totalsize : size;
	if( (size <= 0) || (buf1 == NULL) )
		return 0;
	memcpy(this->m_buf1, buf1, size);
	return size;
}

void CAutoFlick326::SetImageWidth(int width)
{
	this->m_imageWidth = width;
}

int  CAutoFlick326::GetImageWidth(void)
{
	return this->m_imageWidth; 
}

void CAutoFlick326::SetImageHight(int hight)
{
	this->m_imageHight = hight;
}

int  CAutoFlick326::GetImageHight(void)
{
	return this->m_imageHight;
}

void CAutoFlick326::SetAFMode(char afMode)
{
	this->m_afMode = afMode;
}

char CAutoFlick326::GetAFMode(void)
{
	return this->m_afMode;
}

void CAutoFlick326::SetAFStep(char afStep)
{
	this->m_afStep = afStep;
}

char CAutoFlick326::GetAFStep(void)
{
	return this->m_afStep;
}

void CAutoFlick326::SetAFStartLine(int afStart)
{
	this->m_afStart = afStart;
}

int  CAutoFlick326::GetAFStartLine(void)
{
	return this->m_afStart;
}

void CAutoFlick326::SetAFThreshold(int afThre)
{
	this->m_afThre = afThre;
}

int  CAutoFlick326::GetAFThreshold(void)
{
	return this->m_afThre;
}

int  CAutoFlick326::GetSumVal(char *pData, int eleNum)
{
	int  sumVal = 0;
	char *locData = pData;

	while(eleNum--)
	{
		sumVal += *locData;
		locData++;
	}

	return sumVal;
}

int CAutoFlick326::FlickCheck(void)
{
    int lineSum, lineSum1, flickSum = 0;
	char *locBuf  = this->m_buf + this->m_afStart * this->m_imageWidth;
	char *locBuf1 = this->m_buf1 + this->m_afStart * this->m_imageWidth;
	

	if(this->m_buf == NULL || this->m_buf1 == NULL || this->m_imageWidth == 0 || this->m_imageHight == 0 ||
		this->m_afStart < 0 || this->m_afStep == 0)
		return 0;

	while(locBuf != NULL && locBuf1 != NULL)
	{
		lineSum = GetSumVal(locBuf, this->m_imageWidth);
		lineSum1 = GetSumVal(locBuf1, this->m_imageWidth);

		flickSum += abs(lineSum -lineSum1);

		if(this->m_afMode)    //seperate line check
		{
			locBuf += this->m_afStep * this->m_imageWidth;
			locBuf1 += this->m_afStep * this->m_imageWidth;
		}
		else
		{
            locBuf += this->m_imageWidth;
			locBuf1 += this->m_imageWidth;
		}

	}

	return flickSum;
}

int CAutoFlick326:: Process(char *preData, char *curData)
{
	int sumFlick;

	this->SetSize(this->m_imageWidth * this->m_imageHight * this->m_unit);
	this->Write(preData, curData, this->m_imageHight * this->m_imageWidth);
	sumFlick = this->FlickCheck();

	if(sumFlick > this->m_afThre)
		return TRUE;

	return FALSE;
}
