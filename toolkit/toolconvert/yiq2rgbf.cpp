
#include "internal.h"

CYiq2rgbF::CYiq2rgbF()
{
}

CYiq2rgbF::~CYiq2rgbF()
{
}

void CYiq2rgbF::yiq2rgb(char *src, char *dst, int width, int height, int unit)
{
    int len, cap;
	CFileBin fsrc, fdst;
	CBufferBase srcbuf, dstbuf;
	CYiq2rgb yiq2rgb;


	if( (unit != 1) && (unit != 2) )
		unit = 1;
	if(width % 2)
	{
		this->m_dbug.PrintMsg("Image width must be even\n");
		return;
	}

	yiq2rgb.SetSize(width * height * 3);
	srcbuf.SetSize(width * height * 3);
	dstbuf.SetSize(width * height * 3);

	/*set size*/
	yiq2rgb.SetImageWidth(width);
	yiq2rgb.SetImageHight(height);

	fsrc.Open(src);
	fdst.Open(dst, "wb");
	cap = 256;
	if(unit == 2)
		cap = 1024;

	len = width * height * unit * 3;

	fsrc.Read(srcbuf.m_buf, len);
	yiq2rgb.Process(srcbuf.m_buf, dstbuf.m_buf);
	fdst.Write(dstbuf.m_buf, len);	
}

