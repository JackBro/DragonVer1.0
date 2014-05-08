
#include "internal.h"

//constructor

//===============================================================
//
//		rgb convert to yuv420
//
//==============================================================
//CYuv420F::CYuv420F()
//{
//	this->m_pcall[0] = CYuv420::Rgb2Yyuv;
//}

void CYuv420F::rgb2yuv420order(char *src, char *dst, int width, int height, int order, int unit)
{
	int i, len, size, cap;
	CFileBin fsrc, fdst;
	CBufferBase srcbuf, dstbuf;
//	int flag = unit;

	if( (unit != 1) && (unit != 2) )
		unit = 1;
	if( (width % 2) || (height % 2) )
	{
		this->m_dbug.PrintMsg("Image width & height must be even\n");
		return;
	}
	fsrc.Open(src);
	fdst.Open(dst, "wb");
	cap = 256;
	if(unit == 2)
		cap = 1024;
	size = width * unit * 6;
	for(i = 0; i < height; i+=2)
	{
		len = fsrc.Read(srcbuf.m_buf, size);
	
			len = CYuv420::rgb2yuv420order(srcbuf.m_buf, dstbuf.m_buf, width, order, cap, unit);
		fdst.Write(dstbuf.m_buf, len);
	}
}

void CYuv420F::Rgb2Yyuv(char *src, char *dest, int width, int height, int unit)
{
	this->rgb2yuv420order(src, dest, width, height, YUV420_YYUV, unit);
}

void CYuv420F::Rgb2Yuyv(char *src, char *dest, int width, int height, int unit)
{
	this->rgb2yuv420order(src, dest, width, height, YUV420_YUYV, unit);
}

void CYuv420F::Rgb2Uyvy(char *src, char *dest, int width, int height, int unit)
{
	this->rgb2yuv420order(src, dest, width, height, YUV420_UYVY, unit);
}

void CYuv420F::Rgb2Vyuy(char *src, char *dest, int width, int height, int unit)
{
	this->rgb2yuv420order(src, dest, width, height, YUV420_VYUY, unit);
}

void CYuv420F::Rgb2Yvyu(char *src, char *dest, int width, int height, int unit)
{
	this->rgb2yuv420order(src, dest, width, height, YUV420_YVYU, unit);
}

void CYuv420F::Rgb2Yuvy(char *src, char *dest, int width, int height, int unit)
{
	this->rgb2yuv420order(src, dest, width, height, YUV420_YUVY, unit);
}

void CYuv420F::Rgb2Yvuy(char *src, char *dest, int width, int height, int unit)
{
	this->rgb2yuv420order(src, dest, width, height, YUV420_YVUY, unit);
}

void CYuv420F::Rgb2Uvyy(char *src, char *dest, int width, int height, int unit)
{
	this->rgb2yuv420order(src, dest, width, height, YUV420_UVYY, unit);
}

void CYuv420F::Rgb2Vuyy(char *src, char *dest, int width, int height, int unit)
{
	this->rgb2yuv420order(src, dest, width, height, YUV420_VUYY, unit);
}

void CYuv420F::Rgb2Yyvu(char *src, char *dest, int width, int height, int unit)
{
	this->rgb2yuv420order(src, dest, width, height, YUV420_YYVU, unit);
}

//since unit of RGB24 is 1, it is a must to let unit = 1 while calling this function.
void CYuv420F::Rgb2Yuv420Blk(char *src, char *dest, int width, int height, int unit)
{
	int j, len, size;
	CFileBin fsrc, fdst;
	CBufferBase srcbuf, dstbuf;

	if( (unit != 1) && (unit != 2) ) 
		unit = 1;
	if( (width % 2) || (height % 2) )
	{
		this->m_dbug.PrintMsg("Image width & height must be even\n");
		return;
	}
	fsrc.Open(src);
	fdst.Open(dest, "wb");
	size = ((width * unit) * 3) << 4;
	srcbuf.SetSize(size);
	dstbuf.SetSize(size);
	for(j = 0; j < height; j += 16)
	{
		fsrc.Read(srcbuf.m_buf, size);

		len = CYuv420::Rgb2Yuv420Blk(srcbuf.m_buf, dstbuf.m_buf, width, 16, unit);
		fdst.Write(dstbuf.m_buf, len);
	}	
}

//since unit of RGB24 is 1, it is a must to let unit = 1 while calling this function.
void CYuv420F::Rgb2Jpeg420(char *src, char *dest, int width, int height, int unit, int qfactor )
{
	CFileBin fsrc, fdst;
	CBufferBase srcbuf, dstbuf;
	int size;

	if( (unit != 1) && (unit != 2) ) 
		unit = 1;
	if( (width % 2) || (height % 2) )
	{
		this->m_dbug.PrintMsg("Image width & height must be even\n");
		return;
	}
	
	size = CYuv420::getVwc(width, height);
	srcbuf.SetSize(size << 1, unit);
	dstbuf.SetSize(size, unit);
	if( (srcbuf.GetTotalSize() == 0) && (dstbuf.GetTotalSize() == 0) )
	{
		this->m_dbug.PrintMsg("Can not allocate so big buffer\n");
		return;
	}
	fsrc.Open(src);
	fsrc.Read(srcbuf.m_buf, size*unit << 1);
	size = CYuv420::Rgb2Jpeg420(srcbuf.m_buf, dstbuf.m_buf, width, height, unit, qfactor);

	fdst.Open(dest, "wb");
	fdst.Write(dstbuf.m_buf, size);
}

//===============================================================
//
//		yuv420 convert to rgb
//
//==============================================================
void CYuv420F::yuv420_rgborder(char *src, char *dst, int width, int height, int order, int unit)
{
	int i, len, size, cap;
	CFileBin fsrc, fdst;
	CBufferBase srcbuf, dstbuf;
//	int flag = unit;

	if( (unit != 1) && (unit != 2) )
		unit = 1;
	if( (width % 2) || (height % 2) )
	{
		this->m_dbug.PrintMsg("Image width & height must be even\n");
		return;
	}
	fsrc.Open(src);
	fdst.Open(dst, "wb");
	cap = 256;
	if(unit == 2)
		cap = 1024;
	size = width * unit * 3;
	srcbuf.SetSize(size);
	dstbuf.SetSize(size << 1);
	for(i = 0; i < height; i+=2)
	{
		len = fsrc.Read(srcbuf.m_buf, size);
		
			len = CYuv420::yuv420_rgborder(srcbuf.m_buf, dstbuf.m_buf, width, order, cap, unit);
		fdst.Write(dstbuf.m_buf, len);
	}
}

void CYuv420F::Yyuv2Rgb(char *src, char *dest, int width, int height, int unit)
{
	this->yuv420_rgborder(src, dest, width, height, YUV420_YYUV, unit);
}

void CYuv420F::Yuyv2Rgb(char *src, char *dest, int width, int height, int unit)
{
	this->yuv420_rgborder(src, dest, width, height, YUV420_YUYV, unit);
}

void CYuv420F::Uyvy2Rgb(char *src, char *dest, int width, int height, int unit)
{
	this->yuv420_rgborder(src, dest, width, height, YUV420_UYVY, unit);
}

void CYuv420F::Vyuy2Rgb(char *src, char *dest, int width, int height, int unit)
{
	this->yuv420_rgborder(src, dest, width, height, YUV420_VYUY, unit);
}

void CYuv420F::Yvyu2Rgb(char *src, char *dest, int width, int height, int unit)
{
	this->yuv420_rgborder(src, dest, width, height, YUV420_YVYU, unit);
}

void CYuv420F::Yuvy2Rgb(char *src, char *dest, int width, int height, int unit)
{
	this->yuv420_rgborder(src, dest, width, height, YUV420_YUVY, unit);
}

void CYuv420F::Yvuy2Rgb(char *src, char *dest, int width, int height, int unit)
{
	this->yuv420_rgborder(src, dest, width, height, YUV420_YVUY, unit);
}

void CYuv420F::Uvyy2Rgb(char *src, char *dest, int width, int height, int unit)
{
	this->yuv420_rgborder(src, dest, width, height, YUV420_UVYY, unit);
}

void CYuv420F::Vuyy2Rgb(char *src, char *dest, int width, int height, int unit)
{
	this->yuv420_rgborder(src, dest, width, height, YUV420_VUYY, unit);
}

void CYuv420F::Yyvu2Rgb(char *src, char *dest, int width, int height, int unit)
{
	this->yuv420_rgborder(src, dest, width, height, YUV420_YYVU, unit);
}

//===============================================================
//
//		yuv420 convert to yuv444
//
//==============================================================
void CYuv420F::yuv444_420order(char *src, char *dst, int width, int height, int order, int unit)
{
	int i, len, size;
	CFileBin fsrc, fdst;
	CBufferBase srcbuf, dstbuf;
	//int flag = unit;

	if( (unit != 1) && (unit != 2) )
		unit = 1;
	if( (width % 2) || (height % 2) )
	{
		this->m_dbug.PrintMsg("Image width & height must be even\n");
		return;
	}
	fsrc.Open(src);
	fdst.Open(dst, "wb");
	size = width * unit * 6;
	srcbuf.SetSize(size);
	dstbuf.SetSize(size);
	for(i = 0; i < height; i+=2)
	{
		len = fsrc.Read(srcbuf.m_buf, size);
		
			len = CYuv420::yuv444_420order(srcbuf.m_buf, dstbuf.m_buf, width, order, unit);
		fdst.Write(dstbuf.m_buf, len);
	}
}

void CYuv420F::Yuv444_Yyuv(char *src, char *dest, int width, int height, int unit)
{
	this->yuv444_420order(src, dest, width, height, YUV420_YYUV, unit);
}

void CYuv420F::Yuv444_Yuyv(char *src, char *dest, int width, int height, int unit)
{
	this->yuv444_420order(src, dest, width, height, YUV420_YUYV, unit);
}

void CYuv420F::Yuv444_Uyvy(char *src, char *dest, int width, int height, int unit)
{
	this->yuv444_420order(src, dest, width, height, YUV420_UYVY, unit);
}

void CYuv420F::Yuv444_Vyuy(char *src, char *dest, int width, int height, int unit)
{
	this->yuv444_420order(src, dest, width, height, YUV420_VYUY, unit);
}

void CYuv420F::Yuv444_Yvyu(char *src, char *dest, int width, int height, int unit)
{
	this->yuv444_420order(src, dest, width, height, YUV420_YVYU, unit);
}

void CYuv420F::Yuv444_Yuvy(char *src, char *dest, int width, int height, int unit)
{
	this->yuv444_420order(src, dest, width, height, YUV420_YUVY, unit);
}

void CYuv420F::Yuv444_Yvuy(char *src, char *dest, int width, int height, int unit)
{
	this->yuv444_420order(src, dest, width, height, YUV420_YVUY, unit);
}

void CYuv420F::Yuv444_Uvyy(char *src, char *dest, int width, int height, int unit)
{
	this->yuv444_420order(src, dest, width, height, YUV420_UVYY, unit);
}

void CYuv420F::Yuv444_Vuyy(char *src, char *dest, int width, int height, int unit)
{
	this->yuv444_420order(src, dest, width, height, YUV420_VUYY, unit);
}

void CYuv420F::Yuv444_Yyvu(char *src, char *dest, int width, int height, int unit)
{
	this->yuv444_420order(src, dest, width, height, YUV420_YYVU, unit);
}

//===============================================================
//
//		yuv444 convert to yuv420
//
//==============================================================
void CYuv420F::yuv420_444order(char *src, char *dest, int width, int height, int order, int unit)
{
	int i, len, size;
	CFileBin fsrc, fdst;
	CBufferBase srcbuf, dstbuf;
	//int flag = unit;

	if( (unit != 1) && (unit != 2) )
		unit = 1;
	if( (width % 2) || (height % 2) )
	{
		this->m_dbug.PrintMsg("Image width & height must be even\n");
		return;
	}
	fsrc.Open(src);
	fdst.Open(dest, "wb");
	size = width * unit * 3;
	srcbuf.SetSize(size);
	dstbuf.SetSize(size);
	for(i = 0; i < height; i+=2)
	{
		len = fsrc.Read(srcbuf.m_buf, size);
		
			len = CYuv420::yuv420_444order(srcbuf.m_buf, dstbuf.m_buf, width, order, unit);
		fdst.Write(dstbuf.m_buf, len);
	}
}

void CYuv420F::Yyuv2Yuv444(char *src, char *dest, int width, int height, int unit)
{
	this->yuv420_444order(src, dest, width, height, YUV420_YYUV, unit);
}

void CYuv420F::Yuyv2Yuv444(char *src, char *dest, int width, int height, int unit)
{
	this->yuv420_444order(src, dest, width, height, YUV420_YUYV, unit);
}

void CYuv420F::Uyvy2Yuv444(char *src, char *dest, int width, int height, int unit)
{
	this->yuv420_444order(src, dest, width, height, YUV420_UYVY, unit);
}

void CYuv420F::Vyuy2Yuv444(char *src, char *dest, int width, int height, int unit)
{
	this->yuv420_444order(src, dest, width, height, YUV420_VYUY, unit);
}

void CYuv420F::Yvyu2Yuv444(char *src, char *dest, int width, int height, int unit)
{
	this->yuv420_444order(src, dest, width, height, YUV420_YVYU, unit);
}

void CYuv420F::Yuvy2Yuv444(char *src, char *dest, int width, int height, int unit)
{
	this->yuv420_444order(src, dest, width, height, YUV420_YUVY, unit);
}

void CYuv420F::Yvuy2Yuv444(char *src, char *dest, int width, int height, int unit)
{
	this->yuv420_444order(src, dest, width, height, YUV420_YVUY, unit);
}

void CYuv420F::Uvyy2Yuv444(char *src, char *dest, int width, int height, int unit)
{
	this->yuv420_444order(src, dest, width, height, YUV420_UVYY, unit);
}

void CYuv420F::Vuyy2Yuv444(char *src, char *dest, int width, int height, int unit)
{
	this->yuv420_444order(src, dest, width, height, YUV420_VUYY, unit);
}

void CYuv420F::Yyvu2Yuv444(char *src, char *dest, int width, int height, int unit)
{
	this->yuv420_444order(src, dest, width, height, YUV420_YYVU, unit);
}


//===============================================================
//
//		yuv420 convert to block
//
//==============================================================
void CYuv420F::yuv420_blkorder(char *src, char *dest, int width, int height, int order, int unit)
{
	int len, size, msize;
	char* pbuf;
	CFileBin fsrc, fdst;
	CBufferBase srcbuf, dstbuf;
	CYuv2BlkBuf blkbuf((char)order, width, unit);

	if( (unit != 1) && (unit != 2) )
		unit = 1;
	if( (width % 2) || (height % 2) )
	{
		this->m_dbug.PrintMsg("Image width & height must be even\n");
		return;
	}
	fsrc.Open(src);
	fdst.Open(dest, "wb");
	msize = (height * width * unit) * 3;
	srcbuf.SetSize(msize);
	dstbuf.SetSize(msize);
	size = fsrc.Read(srcbuf.m_buf, msize);
	pbuf = srcbuf.m_buf;
	while(size)
	{
		blkbuf.ClearPoint();
		len = blkbuf.Write(pbuf, size);
		if(len == 0)
			break;
		pbuf += len;
		size -= len;
		len = blkbuf.Read(dstbuf.m_buf, msize);
		fdst.Write(dstbuf.m_buf, len);
	}
}

void CYuv420F::yuv420_blkorder1(char *src, char *dest, int width, int height, int order, int unit)
{
	int j, len, size;
	CFileBin fsrc, fdst;
	CBufferBase srcbuf, dstbuf;
	//int flag = unit;

	if( (unit != 1) && (unit != 2) )
		unit = 1;
	if( (width % 2) || (height % 2) )
	{
		this->m_dbug.PrintMsg("Image width & height must be even\n");
		return;
	}
	fsrc.Open(src);
	fdst.Open(dest, "wb");
	size = ((width * unit) * 3) << 3;
	srcbuf.SetSize(size);
	dstbuf.SetSize(size);
	for(j = 0; j < height; j += 16)
	{
		fsrc.Read(srcbuf.m_buf, size);
		
			len = CYuv420::yuv420_blkorder(srcbuf.m_buf, dstbuf.m_buf, order, width, 16, unit);
		fdst.Write(dstbuf.m_buf, len);
	}
}

void CYuv420F::Yyuv2Blk(char *src, char *dest, int width, int height, int unit)
{
	this->yuv420_blkorder(src, dest, width, height, YUV420_YYUV, unit);
}

void CYuv420F::Yuyv2Blk(char *src, char *dest, int width, int height, int unit)
{
	this->yuv420_blkorder(src, dest, width, height, YUV420_YUYV, unit);
}

void CYuv420F::Uyvy2Blk(char *src, char *dest, int width, int height, int unit)
{
	this->yuv420_blkorder(src, dest, width, height, YUV420_UYVY, unit);
}

void CYuv420F::Vyuy2Blk(char *src, char *dest, int width, int height, int unit)
{
	this->yuv420_blkorder(src, dest, width, height, YUV420_VYUY, unit);
}

void CYuv420F::Yvyu2Blk(char *src, char *dest, int width, int height, int unit)
{
	this->yuv420_blkorder(src, dest, width, height, YUV420_YVYU, unit);
}

void CYuv420F::Yuvy2Blk(char *src, char *dest, int width, int height, int unit)
{
	this->yuv420_blkorder(src, dest, width, height, YUV420_YUVY, unit);
}

void CYuv420F::Yvuy2Blk(char *src, char *dest, int width, int height, int unit)
{
	this->yuv420_blkorder(src, dest, width, height, YUV420_YVUY, unit);
}

void CYuv420F::Uvyy2Blk(char *src, char *dest, int width, int height, int unit)
{
	this->yuv420_blkorder(src, dest, width, height, YUV420_UVYY, unit);
}

void CYuv420F::Vuyy2Blk(char *src, char *dest, int width, int height, int unit)
{
	this->yuv420_blkorder(src, dest, width, height, YUV420_VUYY, unit);
}

void CYuv420F::Yyvu2Blk(char *src, char *dest, int width, int height, int unit)
{
	this->yuv420_blkorder(src, dest, width, height, YUV420_YYVU, unit);
}

//===============================================================
//
//		block convert to yuv420
//
//==============================================================
void CYuv420F::blk2yuv420order(char *src, char *dst, int width, int height, int order, int unit)
{
	int len, size, msize;
	char* pbuf;
	CFileBin fsrc, fdst;
	CBufferBase srcbuf, dstbuf;
	CBlk2YuvBuf blkbuf((char)order, width, unit);

	if( (unit != 1) && (unit != 2) )
		unit = 1;
	if( (width % 2) || (height % 2) )
	{
		this->m_dbug.PrintMsg("Image width & height must be even\n");
		return;
	}
	fsrc.Open(src);
	fdst.Open(dst, "wb");
	msize = (height * width * unit) * 3;
	srcbuf.SetSize(msize);
	dstbuf.SetSize(msize);
	size = fsrc.Read(srcbuf.m_buf, msize);
	pbuf = srcbuf.m_buf;
	while(size)
	{
		blkbuf.ClearPoint();
		len = blkbuf.Write(pbuf, size);
		if(len == 0)
			break;
		pbuf += len;
		size -= len;
		len = blkbuf.Read(dstbuf.m_buf, msize);
		fdst.Write(dstbuf.m_buf, len);
	}
}

void CYuv420F::blk2yuv420order1(char *src, char *dst, int width, int height, int order, int unit)
{
	int j, len, size;
	CFileBin fsrc, fdst;
	CBufferBase srcbuf, dstbuf;
	//int flag = unit;

	if( (unit != 1) && (unit != 2) )
		unit = 1;
	if( (width % 2) || (height % 2) )
	{
		this->m_dbug.PrintMsg("Image width & height must be even\n");
		return;
	}
	fsrc.Open(src);
	fdst.Open(dst, "wb");
	size = ((width * unit) * 3) << 3;
	srcbuf.SetSize(size);
	dstbuf.SetSize(size);
	for(j = 0; j < height; j += 16)
	{
		fsrc.Read(srcbuf.m_buf, size);
		
			len = CYuv420::blk2yuv420order(srcbuf.m_buf, dstbuf.m_buf, order, width, 16, unit);
		fdst.Write(dstbuf.m_buf, len);
	}
}

void CYuv420F::Blk2Yyuv(char *src, char *dest, int width, int height, int unit)
{
	this->blk2yuv420order(src, dest, width, height, YUV420_YYUV, unit);
}

void CYuv420F::Blk2Yuyv(char *src, char *dest, int width, int height, int unit)
{
	this->blk2yuv420order(src, dest, width, height, YUV420_YUYV, unit);
}

void CYuv420F::Blk2Uyvy(char *src, char *dest, int width, int height, int unit)
{
	this->blk2yuv420order(src, dest, width, height, YUV420_UYVY, unit);
}

void CYuv420F::Blk2Vyuy(char *src, char *dest, int width, int height, int unit)
{
	this->blk2yuv420order(src, dest, width, height, YUV420_VYUY, unit);
}

void CYuv420F::Blk2Yvyu(char *src, char *dest, int width, int height, int unit)
{
	this->blk2yuv420order(src, dest, width, height, YUV420_YVYU, unit);
}

void CYuv420F::Blk2Yuvy(char *src, char *dest, int width, int height, int unit)
{
	this->blk2yuv420order(src, dest, width, height, YUV420_YUVY, unit);
}

void CYuv420F::Blk2Yvuy(char *src, char *dest, int width, int height, int unit)
{
	this->blk2yuv420order(src, dest, width, height, YUV420_YVUY, unit);
}

void CYuv420F::Blk2Uvyy(char *src, char *dest, int width, int height, int unit)
{
	this->blk2yuv420order(src, dest, width, height, YUV420_UVYY, unit);
}

void CYuv420F::Blk2Vuyy(char *src, char *dest, int width, int height, int unit)
{
	this->blk2yuv420order(src, dest, width, height, YUV420_VUYY, unit);
}

void CYuv420F::Blk2Yyvu(char *src, char *dest, int width, int height, int unit)
{
	this->blk2yuv420order(src, dest, width, height, YUV420_YYVU, unit);
}

void CYuv420F::Blk2Rgb24(char *src, char *dest, int width, int height)
{
	int size, msize, ncount = 0;
	CFileBin fsrc, fdst;
	CBufferBase srcbuf, dstbuf;

	if( (width % 2) || (height % 2) )
	{
		this->m_dbug.PrintMsg("Image width & height must be even\n");
		return;
	}
	fsrc.Open(src);
	fdst.Open(dest, "wb");

	msize = (height * width) * 3;
	srcbuf.SetSize(msize >> 1);
	dstbuf.SetSize(msize);
	size = fsrc.Read(srcbuf.m_buf, msize);
	if (0 == size)
		return;

	ncount = CYuv420::Blk2Rgb24(srcbuf.m_buf, dstbuf.m_buf, width, height);
	fdst.Write(dstbuf.m_buf, ncount);
	
}

//===============================================================
//
//		yuv420 convert to lbuf
//
//==============================================================
void CYuv420F::yuv420_lbuforder(char *src, char *dest, int width, int height, int order, int unit)
{
	int j, len, size;
	CFileBin fsrc, fdst;
	CBufferBase srcbuf, dstbuf;
	//int flag = unit;

	if( (unit != 1) && (unit != 2) )
		unit = 1;
	if( (width % 2) || (height % 2) )
	{
		this->m_dbug.PrintMsg("Image width & height must be even\n");
		return;
	}
	fsrc.Open(src);
	fdst.Open(dest, "wb");
	size = ((width * unit) * 3) << 3;
	srcbuf.SetSize(size);
	dstbuf.SetSize(size);
	for(j = 0; j < height; j += 16)
	{
		fsrc.Read(srcbuf.m_buf, size);
		
			len = CYuv420::yuv420_lbuforder(srcbuf.m_buf, dstbuf.m_buf, order, width, 16, unit);
		fdst.Write(dstbuf.m_buf, len);
	}
}

void CYuv420F::Yyuv2Lbuf(char *src, char *dest, int width, int height, int unit)
{
	this->yuv420_lbuforder(src, dest, width, height, YUV420_YYUV, unit);
}

void CYuv420F::Yuyv2Lbuf(char *src, char *dest, int width, int height, int unit)
{
	this->yuv420_lbuforder(src, dest, width, height, YUV420_YUYV, unit);
}

void CYuv420F::Uyvy2Lbuf(char *src, char *dest, int width, int height, int unit)
{
	this->yuv420_lbuforder(src, dest, width, height, YUV420_UYVY, unit);
}

void CYuv420F::Vyuy2Lbuf(char *src, char *dest, int width, int height, int unit)
{
	this->yuv420_lbuforder(src, dest, width, height, YUV420_VYUY, unit);
}

void CYuv420F::Yvyu2Lbuf(char *src, char *dest, int width, int height, int unit)
{
	this->yuv420_lbuforder(src, dest, width, height, YUV420_YVYU, unit);
}

void CYuv420F::Yuvy2Lbuf(char *src, char *dest, int width, int height, int unit)
{
	this->yuv420_lbuforder(src, dest, width, height, YUV420_YUVY, unit);
}

void CYuv420F::Yvuy2Lbuf(char *src, char *dest, int width, int height, int unit)
{
	this->yuv420_lbuforder(src, dest, width, height, YUV420_YVUY, unit);
}

void CYuv420F::Uvyy2Lbuf(char *src, char *dest, int width, int height, int unit)
{
	this->yuv420_lbuforder(src, dest, width, height, YUV420_UVYY, unit);
}

void CYuv420F::Vuyy2Lbuf(char *src, char *dest, int width, int height, int unit)
{
	this->yuv420_lbuforder(src, dest, width, height, YUV420_VUYY, unit);
}

void CYuv420F::Yyvu2Lbuf(char *src, char *dest, int width, int height, int unit)
{
	this->yuv420_lbuforder(src, dest, width, height, YUV420_YYVU, unit);
}


//===============================================================
//
//		lbuf convert to yuv420
//
//==============================================================
void CYuv420F::lbuf2yuv420order(char *src, char *dst, int width, int height, int order, int unit)
{
	int j, len, size;
	CFileBin fsrc, fdst;
	CBufferBase srcbuf, dstbuf;
	//int flag = unit;

	if( (unit != 1) && (unit != 2) )
		unit = 1;
	if( (width % 2) || (height % 2) )
	{
		this->m_dbug.PrintMsg("Image width & height must be even\n");
		return;
	}
	fsrc.Open(src);
	fdst.Open(dst, "wb");
	size = ((width * unit) * 3) << 3;
	srcbuf.SetSize(size);
	dstbuf.SetSize(size);
	for(j = 0; j < height; j += 16)
	{
		fsrc.Read(srcbuf.m_buf, size);
		
			len = CYuv420::lbuf2yuv420order(srcbuf.m_buf, dstbuf.m_buf, order, width, 16, unit);
		fdst.Write(dstbuf.m_buf, len);
	}
}

void CYuv420F::Lbuf2Yyuv(char *src, char *dest, int width, int height, int unit)
{
	this->lbuf2yuv420order(src, dest, width, height, YUV420_YYUV, unit);
}

void CYuv420F::Lbuf2Yuyv(char *src, char *dest, int width, int height, int unit)
{
	this->lbuf2yuv420order(src, dest, width, height, YUV420_YUYV, unit);
}

void CYuv420F::Lbuf2Uyvy(char *src, char *dest, int width, int height, int unit)
{
	this->lbuf2yuv420order(src, dest, width, height, YUV420_UYVY, unit);
}

void CYuv420F::Lbuf2Vyuy(char *src, char *dest, int width, int height, int unit)
{
	this->lbuf2yuv420order(src, dest, width, height, YUV420_VYUY, unit);
}

void CYuv420F::Lbuf2Yvyu(char *src, char *dest, int width, int height, int unit)
{
	this->lbuf2yuv420order(src, dest, width, height, YUV420_YVYU, unit);
}

void CYuv420F::Lbuf2Yuvy(char *src, char *dest, int width, int height, int unit)
{
	this->lbuf2yuv420order(src, dest, width, height, YUV420_YUVY, unit);
}

void CYuv420F::Lbuf2Yvuy(char *src, char *dest, int width, int height, int unit)
{
	this->lbuf2yuv420order(src, dest, width, height, YUV420_YVUY, unit);
}

void CYuv420F::Lbuf2Uvyy(char *src, char *dest, int width, int height, int unit)
{
	this->lbuf2yuv420order(src, dest, width, height, YUV420_UVYY, unit);
}

void CYuv420F::Lbuf2Vuyy(char *src, char *dest, int width, int height, int unit)
{
	this->lbuf2yuv420order(src, dest, width, height, YUV420_VUYY, unit);
}

void CYuv420F::Lbuf2Yyvu(char *src, char *dest, int width, int height, int unit)
{
	this->lbuf2yuv420order(src, dest, width, height, YUV420_YYVU, unit);
}


//===============================================================
//
//		yuv420 image resize
//
//==============================================================
void CYuv420F::yuv420_resizeorder(char *src, char *dest, int srcw, int srch, 
								 int dstw, int dsth, int order, int unit)
{
	CFileBin fsrc, fdst;
	CBufferBase srcbuf, dstbuf;
	int size, w, h;
	//int flag = unit;

	if( (unit != 1) && (unit != 2) )
		unit = 1;
	w = (srcw > dstw) ? srcw : dstw;
	h = (srch > dsth) ? srch : dsth;
	size = w * h * 3;
	srcbuf.SetSize(size, unit);
	dstbuf.SetSize(size, unit);
	if( (srcbuf.GetTotalSize() == 0) || (dstbuf.GetTotalSize() == 0) )
	{
		this->m_dbug.PrintMsg("Can not allocate so big buffer\n");
		return;
	}
	fsrc.Open(src);
	fdst.Open(dest, "wb");
	
	size = (srcw * srch * unit * 3) >> 1;
	fsrc.Read(srcbuf.m_buf, size);
	
		size = CYuv420::yuv420_resizeorder(srcbuf.m_buf, dstbuf.m_buf, srcw, srch, dstw, dsth, order, unit);
	fdst.Write(dstbuf.m_buf, size);
}

void CYuv420F::YyuvResize(char *src, char *dest, int srcw, int srch, int dstw, int dsth, int unit)
{
	this->yuv420_resizeorder(src, dest, srcw, srch, dstw, dsth, YUV420_YYUV, unit);
}

void CYuv420F::YuyvResize(char *src, char *dest, int srcw, int srch, int dstw, int dsth, int unit)
{
	this->yuv420_resizeorder(src, dest, srcw, srch, dstw, dsth, YUV420_YUYV, unit);
}

void CYuv420F::UyvyResize(char *src, char *dest, int srcw, int srch, int dstw, int dsth, int unit)
{
	this->yuv420_resizeorder(src, dest, srcw, srch, dstw, dsth, YUV420_UYVY, unit);
}

void CYuv420F::VyuyResize(char *src, char *dest, int srcw, int srch, int dstw, int dsth, int unit)
{
	this->yuv420_resizeorder(src, dest, srcw, srch, dstw, dsth, YUV420_VYUY, unit);
}

void CYuv420F::YvyuResize(char *src, char *dest, int srcw, int srch, int dstw, int dsth, int unit)
{
	this->yuv420_resizeorder(src, dest, srcw, srch, dstw, dsth, YUV420_YVYU, unit);
}

void CYuv420F::YuvyResize(char *src, char *dest, int srcw, int srch, int dstw, int dsth, int unit)
{
	this->yuv420_resizeorder(src, dest, srcw, srch, dstw, dsth, YUV420_YUVY, unit);
}

void CYuv420F::YvuyResize(char *src, char *dest, int srcw, int srch, int dstw, int dsth, int unit)
{
	this->yuv420_resizeorder(src, dest, srcw, srch, dstw, dsth, YUV420_YVUY, unit);
}

void CYuv420F::UvyyResize(char *src, char *dest, int srcw, int srch, int dstw, int dsth, int unit)
{
	this->yuv420_resizeorder(src, dest, srcw, srch, dstw, dsth, YUV420_UVYY, unit);
}

void CYuv420F::VuyyResize(char *src, char *dest, int srcw, int srch, int dstw, int dsth, int unit)
{
	this->yuv420_resizeorder(src, dest, srcw, srch, dstw, dsth, YUV420_VUYY, unit);
}

void CYuv420F::YyvuResize(char *src, char *dest, int srcw, int srch, int dstw, int dsth, int unit)
{
	this->yuv420_resizeorder(src, dest, srcw, srch, dstw, dsth, YUV420_YYVU, unit);
}

//=======================================================
//
//		YUV420 Jpeg encode
//
//=======================================================
void CYuv420F::Blk2Jpeg(char *src, char *dest, int width, int height, int qf)
{
	CFileBin fsrc, fdst;
	CBufferBase srcbuf, dstbuf;
	int size;

	size = CYuv420::getVwc(width, height);
	srcbuf.SetSize(size);
	dstbuf.SetSize(size);
	if( (srcbuf.GetTotalSize() == 0) && (dstbuf.GetTotalSize() == 0) )
	{
		this->m_dbug.PrintMsg("Can not allocate so big buffer\n");
		return;
	}
	fsrc.Open(src);
	fsrc.Read(srcbuf.m_buf, size);
	size = CYuv420::Blk2Jpeg(srcbuf.m_buf, dstbuf.m_buf, width, height, qf);
	fdst.Open(dest, "wb");
	fdst.Write(dstbuf.m_buf, size);
}

//=======================================================
//
//		YUV420 Mpeg2 I frame encode
//
//=======================================================
void CYuv420F::Blk2Mpeg2I(char *src, char *dest, int width, int height)
{
	CFileBin fsrc, fdst;
	CBufferBase srcbuf, dstbuf;
	int size;

	size = CYuv420::getVwc(width, height);
	srcbuf.SetSize(size);
	dstbuf.SetSize(size);
	if( (srcbuf.GetTotalSize() == 0) && (dstbuf.GetTotalSize() == 0) )
	{
		this->m_dbug.PrintMsg("Can not allocate so big buffer\n");
		return;
	}
	fsrc.Open(src);
	fsrc.Read(srcbuf.m_buf, size);
	size = CYuv420::Blk2Mpeg2I(srcbuf.m_buf, dstbuf.m_buf, width, height);
	fdst.Open(dest, "wb");
	fdst.Write(dstbuf.m_buf, size);
}

//=======================================================
//
//		YUV420 to frame
//
//=======================================================
void CYuv420F::yuv420_frmorder(char *src, char *dst, int width, int height, int order, int unit)
{
	int size;
	CFileBin fsrc, fdst;
	CBufferBase srcbuf, dstbuf;

	if( (unit != 1) && (unit != 2) )
		unit = 1;
	if( (width % 2) || (height % 2) )
	{
		this->m_dbug.PrintMsg("Image width and height must be even\n");
		return;
	}
	size = (width * height * unit * 3) >> 1;
	srcbuf.SetSize(size);
	dstbuf.SetSize(size);
	if( (srcbuf.GetSize() == 0) || (dstbuf.GetSize() == 0) )
	{
		this->m_dbug.PrintMsg("Can not alloc memory\n");
		return;
	}
	fsrc.Open(src);
	fdst.Open(dst, "wb");
	fsrc.Read(srcbuf.m_buf, size);
	size = CYuv420::MYuv420_FrmOrder(srcbuf.m_buf, dstbuf.m_buf, width, height, order, unit);
	fdst.Write(dstbuf.m_buf, size);
}

void CYuv420F::Yyuv2Frame(char *src, char *dest, int width, int height, int unit)
{
	this->yuv420_frmorder(src, dest, width, height, YUV420_YYUV, unit);
}

void CYuv420F::Yyvu2Frame(char *src, char *dest, int width, int height, int unit)
{
	this->yuv420_frmorder(src, dest, width, height, YUV420_YYVU, unit);
}

void CYuv420F::Yuyv2Frame(char *src, char *dest, int width, int height, int unit)
{
	this->yuv420_frmorder(src, dest, width, height, YUV420_YUYV, unit);
}

void CYuv420F::Yvyu2Frame(char *src, char *dest, int width, int height, int unit)
{
	this->yuv420_frmorder(src, dest, width, height, YUV420_YVYU, unit);
}

void CYuv420F::Yuvy2Frame(char *src, char *dest, int width, int height, int unit)
{
	this->yuv420_frmorder(src, dest, width, height, YUV420_YUVY, unit);
}

void CYuv420F::Yvuy2Frame(char *src, char *dest, int width, int height, int unit)
{
	this->yuv420_frmorder(src, dest, width, height, YUV420_YVUY, unit);
}

void CYuv420F::Uvyy2Frame(char *src, char *dest, int width, int height, int unit)
{
	this->yuv420_frmorder(src, dest, width, height, YUV420_UVYY, unit);
}

void CYuv420F::Vuyy2Frame(char *src, char *dest, int width, int height, int unit)
{
	this->yuv420_frmorder(src, dest, width, height, YUV420_VUYY, unit);
}

void CYuv420F::Uyvy2Frame(char *src, char *dest, int width, int height, int unit)
{
	this->yuv420_frmorder(src, dest, width, height, YUV420_UYVY, unit);
}

void CYuv420F::Vyuy2Frame(char *src, char *dest, int width, int height, int unit)
{
	this->yuv420_frmorder(src, dest, width, height, YUV420_VYUY, unit);
}

//=======================================================
//
//		Frame to Yuv420
//
//=======================================================
void CYuv420F::frm2yuv420order(char *src, char *dst, int width, int height, int order, int unit)
{
	int size;
	CFileBin fsrc, fdst;
	CBufferBase srcbuf, dstbuf;

	if( (unit != 1) && (unit != 2) )
		unit = 1;
	if( (width % 2) || (height % 2) )
	{
		this->m_dbug.PrintMsg("Image width and height must be even\n");
		return;
	}
	size = (width * height * unit * 3) >> 1;
	srcbuf.SetSize(size);
	dstbuf.SetSize(size);
	if( (srcbuf.GetSize() == 0) || (dstbuf.GetSize() == 0) )
	{
		this->m_dbug.PrintMsg("Can not alloc memory\n");
		return;
	}
	fsrc.Open(src);
	fdst.Open(dst, "wb");
	fsrc.Read(srcbuf.m_buf, size);
	size = CYuv420::MFrm2Yuv420Order(srcbuf.m_buf, dstbuf.m_buf, width, height, order, unit);
	fdst.Write(dstbuf.m_buf, size);
}

void CYuv420F::Frame2Yyuv(char *src, char *dest, int width, int height, int unit)
{
	this->frm2yuv420order(src, dest, width, height, YUV420_YYUV, unit);
}

void CYuv420F::Frame2Yyvu(char *src, char *dest, int width, int height, int unit)
{
	this->frm2yuv420order(src, dest, width, height, YUV420_YYVU, unit);
}

void CYuv420F::Frame2Yuvy(char *src, char *dest, int width, int height, int unit)
{
	this->frm2yuv420order(src, dest, width, height, YUV420_YUVY, unit);
}

void CYuv420F::Frame2Yvuy(char *src, char *dest, int width, int height, int unit)
{
	this->frm2yuv420order(src, dest, width, height, YUV420_YVUY, unit);
}

void CYuv420F::Frame2Uvyy(char *src, char *dest, int width, int height, int unit)
{
	this->frm2yuv420order(src, dest, width, height, YUV420_UVYY, unit);
}

void CYuv420F::Frame2Vuyy(char *src, char *dest, int width, int height, int unit)
{
	this->frm2yuv420order(src, dest, width, height, YUV420_VUYY, unit);
}

void CYuv420F::Frame2Uyvy(char *src, char *dest, int width, int height, int unit)
{
	this->frm2yuv420order(src, dest, width, height, YUV420_UYVY, unit);
}

void CYuv420F::Frame2Vyuy(char *src, char *dest, int width, int height, int unit)
{
	this->frm2yuv420order(src, dest, width, height, YUV420_VYUY, unit);
}

void CYuv420F::Frame2Yuyv(char *src, char *dest, int width, int height, int unit)
{
	this->frm2yuv420order(src, dest, width, height, YUV420_YUYV, unit);
}

void CYuv420F::Frame2Yvyu(char *src, char *dest, int width, int height, int unit)
{
	this->frm2yuv420order(src, dest, width, height, YUV420_YVYU, unit);
}

