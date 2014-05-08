
#include "internal.h"

/**************************************class CYiq2rgb****************************************/
CYiq2rgb::CYiq2rgb(int cap, int width, int height)
{
    this->m_cap = cap;

	this->m_imageWidth = width;
	this->m_imageHight = height;
}

int CYiq2rgb::Read(char *buf, int len)
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

int CYiq2rgb::Write(char *buf, int len)
{
	return CFifoBuf::Write(buf, len);
}

void CYiq2rgb::SetSize(int size)
{
    CFifoBuf::SetSize(size);
}

int	CYiq2rgb::GetSize(void)
{
	return CFifoBuf::GetSize();
}

int	CYiq2rgb::GetTotalSize(void)
{
	return CFifoBuf::GetTotalSize();
}

void  CYiq2rgb::SetCap(int cap)
{
	this->m_cap = cap;
}

int  CYiq2rgb::GetCap(void)
{
	return this->m_cap;
}

void CYiq2rgb::SetImageWidth(int width)
{
	this->m_imageWidth = width;
}

int  CYiq2rgb::GetImageWidth(void)
{
	return this->m_imageWidth;
}

void CYiq2rgb::SetImageHight(int hight)
{
	this->m_imageHight = hight;
}

int  CYiq2rgb::GetImageHight(void)
{
	return this->m_imageHight;
}

char CYiq2rgb::Yiq2r(void)
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

char CYiq2rgb::Yiq2g(void)
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

char CYiq2rgb::Yiq2b(void)
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

void CYiq2rgb::Process(char *yiq, char *rgb)
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

