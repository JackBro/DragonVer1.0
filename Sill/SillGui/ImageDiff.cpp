
#include "internal.h"



#define BMP24HEADERLENG	(sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER))

CImageDiff::CImageDiff()
{	
	this->m_imgwidth = 0;
	this->m_imgheight = 0;
	this->m_format = RGB24;
	this->m_simgsize = 0;
	this->m_curpixel_x = -1;
	this->m_curpixel_y = -1;
}

CImageDiff::~CImageDiff()
{
}

void CImageDiff::SetImgParm(int width, int height)
{
	this->m_imgwidth = width;
	this->m_imgheight = height;
	this->m_simgsize = this->m_imgwidth * this->m_imgheight * 3 ;
}

void CImageDiff::LoadRawImg(UINT8 *buf, int len)
{
	CRaw2RgbF trgb;
	char  *p = NULL;

	if(len < this->m_simgsize)
		return;
	len = this->m_simgsize;
    this->m_pool.SetSize( len + BMP24HEADERLENG);
	trgb.addbmp24header(this->m_pool.m_buf, this->m_imgwidth, 0 - this->m_imgheight);
	p = this->m_pool.m_buf + BMP24HEADERLENG;
	
	if (p != (char *) buf)
		memcpy(p, buf, len);

	m_bDiffOn  = FALSE;
}

void CImageDiff::LoadRawImg(char *src)
{
	CFileBin fb;
	CRgb24Convert tools;
	int len;
	CBufferBase pool(this->m_simgsize);

	fb.Open(src);
	len = fb.Read(pool.m_buf, this->m_simgsize);
	if(len)
	{
		tools.Img2Rgb24(pool.m_buf, this->m_pool.m_buf + BMP24HEADERLENG,
		this->m_imgwidth, this->m_imgheight, this->m_format);

		this->LoadRawImg((UINT8 *) (this->m_pool.m_buf + BMP24HEADERLENG), this->m_simgsize);
	}
}

void CImageDiff::ImageDiff(UINT8 *buf1, UINT8 *buf2, int len)
{
	CRaw2RgbF trgb; 
	char  *p = NULL, *startp = NULL;
	int i;

	if(len < this->m_simgsize)
		return;
    len = this->m_simgsize;
    this->m_pool.SetSize( len + BMP24HEADERLENG);
	trgb.addbmp24header(this->m_pool.m_buf, this->m_imgwidth, 0 - this->m_imgheight);
	
	p = this->m_pool.m_buf + BMP24HEADERLENG;
	memset(p, 0xff, len);

	startp = this->m_pool.m_buf + BMP24HEADERLENG;
	for (i = 0; i < len ; i = i + 3)
	{
		p = startp + i;
		if ((buf1[i] - buf2[i]) || (buf1[i+1] - buf2[i+1]) || (buf1[i+2] - buf2[i+2]))
			memset(p, 0, 3);
	}

	GotoFirstDiffPixel();
	m_bDiffOn = TRUE;
}



void CImageDiff::GotoFirstDiffPixel(void)
{
	char *p, *FirstPos;
	int i, j;

	FirstPos = this->m_pool.m_buf + BMP24HEADERLENG;
	for (i = 0; i < this->m_imgheight; i++)
		for (j = 0; j < this->m_imgwidth; j++)
		{
			p = FirstPos + ( i*m_imgwidth + j) * 3;
			if (!(*p || *(p+1) || *(p+2)))
			{
				this->m_curpixel_x = j;
				this->m_curpixel_y = i;
				return ;
			}
		}

}

void CImageDiff::GotoLastDiffPixel(void)
{
	char *p, *FirstPos;
	int i, j;

	FirstPos = this->m_pool.m_buf + BMP24HEADERLENG;
	for(i = this->m_imgheight - 1; i >= 0; i-- )
		for(j = this->m_imgwidth - 1; j >= 0; j-- )
		{
			p = FirstPos + ( i*m_imgwidth + j) * 3;
			if (!(*p || *(p+1) || *(p+2)))
			{
				this->m_curpixel_x = j;
				this->m_curpixel_y = i;
				return ;
			}
		}
}

void CImageDiff::GotoPixel(int x, int y)
{
	this->m_curpixel_x = x;
	this->m_curpixel_y = y;
}

void CImageDiff::GotoPreviousDiffPixel(void)
{
	char *p;
	int startp, endp;

   	if (m_curpixel_x == -1) 
		return;

	startp = (this->m_imgwidth * this->m_curpixel_y  + this->m_curpixel_x - 1) * 3 ;
    endp = 0;

	while (startp >= endp)
	{
		p = startp + this->m_pool.m_buf + BMP24HEADERLENG;
		if (!(*p || *(p+1) || *(p+2)))
		{
			this->m_curpixel_y = startp/(this->m_imgwidth * 3);
			this->m_curpixel_x = (startp - m_curpixel_y * this->m_imgwidth * 3) /3;
			return;
		}
		startp -= 3;
	}
}

void CImageDiff::GotoNextDiffPixel(void)
{
	char *p;
	int startp, endp;

	if (m_curpixel_x == -1) 
		return;

	startp = (this->m_imgwidth * this->m_curpixel_y  + this->m_curpixel_x + 1) * 3 ;
    endp = this->m_simgsize;

	while (startp < endp)
	{
		p = startp + this->m_pool.m_buf + BMP24HEADERLENG;
		if (!(*p || *(p+1) || *(p+2)))
		{
			this->m_curpixel_y = startp/(this->m_imgwidth * 3);
			this->m_curpixel_x = (startp - m_curpixel_y * this->m_imgwidth * 3) /3;
			return;
		}
		startp += 3;
	}
}


int CImageDiff::GetAllDiffPixelText(char *buf, int len)
{

	char *p, *FirstPos, *pbuf;
	int i, j;

	FirstPos = this->m_pool.m_buf + BMP24HEADERLENG;
	pbuf = buf;

	for (i = 0; i < this->m_imgheight; i++)
		for (j = 0; j < this->m_imgwidth; j++)
		{
			if (pbuf> (buf + len) )
				return  (int)(pbuf - buf +1) ;
			p = FirstPos + ( i*m_imgwidth + j) * 3; 
			if (!(*p || *(p+1) || *(p+2)))
			{
				len = sprintf(pbuf, "(%d,%d)\n", j, i);
				pbuf += len;
			}
		}

	return  (int)(pbuf - buf +1) ;
}


int CImageDiff::GetImgWidth(void)
{
	return m_imgwidth;
}

int  CImageDiff::GetImgHeight(void)
{
	return m_imgheight;
}

BOOL CImageDiff::IsDiffOn()
{
	return m_bDiffOn;
}

void CImageDiff::GetCurrentPixel(int *x, int *y)
{
	*x = m_curpixel_x;
	*y = m_curpixel_y;
}

char* CImageDiff::GetPoolBuf(void)
{
	return (m_pool.m_buf);
}

int	 CImageDiff::GetImgSize(void)
{
	return m_simgsize;
}
