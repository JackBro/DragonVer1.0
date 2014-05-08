
#include "internal.h"

CRgb2yiqF::CRgb2yiqF()
{
}

CRgb2yiqF::~CRgb2yiqF()
{
}

void CRgb2yiqF::rgb2yiq(char *src, char *dst, int width, int height, int unit)
{
    int len, cap;
	int yGammaP[9] = {0x00, 0x15, 0x20, 0x30, 0x49, 0x5e, 0x6f, 0xa9, 0xd7};
	int yGammaS[9] = {0x30, 0x2b, 0x22, 0x1a, 0x14, 0x11, 0x0e, 0x0b, 0x0a};
	CFileBin fsrc, fdst;
	CBufferBase srcbuf, dstbuf;
	CRgb2yiq rgb2yiq;


	if( (unit != 1) && (unit != 2) )
		unit = 1;
	if(width % 2)
	{
		this->m_dbug.PrintMsg("Image width must be even\n");
		return;
	}

	rgb2yiq.SetSize(width * height * 3);
	srcbuf.SetSize(width * height * 3);
	dstbuf.SetSize(width * height * 3);

	/*set size*/
	rgb2yiq.SetImageWidth(width);
	rgb2yiq.SetImageHight(height);

	/*set gamma start point and slope*/
	rgb2yiq.SetGamStartPt(yGammaP);
	rgb2yiq.SetGamSlope(yGammaS);

	fsrc.Open(src);
	fdst.Open(dst, "wb");
	cap = 256;
	if(unit == 2)
		cap = 1024;

	len = width * height * 3 * unit;

	fsrc.Read(srcbuf.m_buf, len);
	rgb2yiq.Process(srcbuf.m_buf, dstbuf.m_buf);
	fdst.Write(dstbuf.m_buf, len);	
}

