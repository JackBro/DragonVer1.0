
#include "internal.h"

//====================================================
//
//					RGB convert to YUV422
//
//====================================================
void CYuv422F::rgb2yuv422order(char *src, char *dst, int width, int height, int order, int unit)
{
	int i, len, size, cap;
	CFileBin fsrc, fdst;
	CBufferBase srcbuf, dstbuf;
//	int flag = unit;

	if( (unit != 1) && (unit != 2) )
		unit = 1;
	if(width % 2)
	{
		this->m_dbug.PrintMsg("Image width must be even\n");
		return;
	}
	fsrc.Open(src);
	fdst.Open(dst, "wb");
	cap = 256;
	if(unit == 2)
		cap = 1024;
	size = unit * 3 * 2;
	len = width * height * unit * 3;
	for(i = 0; i < len; i += size)
	{
		width = fsrc.Read(srcbuf.m_buf, size);
		
			width = CYuv422::rgb2yuv422order(srcbuf.m_buf, dstbuf.m_buf, order, cap, unit);
		fdst.Write(dstbuf.m_buf, width);
	}
}

void CYuv422F::Rgb2Yyuv(char *src, char *dst, int width, int height, int unit)
{
	this->rgb2yuv422order(src, dst, width, height, YUV422_YYUV, unit);
}

void CYuv422F::Rgb2Yuyv(char *src, char *dst, int width, int height, int unit)
{
	this->rgb2yuv422order(src, dst, width, height, YUV422_YUYV, unit);
}

void CYuv422F::Rgb2Uyvy(char *src, char *dst, int width, int height, int unit)
{
	this->rgb2yuv422order(src, dst, width, height, YUV422_UYVY, unit);
}

void CYuv422F::Rgb2Vyuy(char *src, char *dst, int width, int height, int unit)
{
	this->rgb2yuv422order(src, dst, width, height, YUV422_VYUY, unit);
}

void CYuv422F::Rgb2Yvyu(char *src, char *dst, int width, int height, int unit)
{
	this->rgb2yuv422order(src, dst, width, height, YUV422_YVYU, unit);
}

void CYuv422F::Rgb2Yuvy(char *src, char *dst, int width, int height, int unit)
{
	this->rgb2yuv422order(src, dst, width, height, YUV422_YUVY, unit);
}

void CYuv422F::Rgb2Yvuy(char *src, char *dst, int width, int height, int unit)
{
	this->rgb2yuv422order(src, dst, width, height, YUV422_YVUY, unit);
}

void CYuv422F::Rgb2Uvyy(char *src, char *dst, int width, int height, int unit)
{
	this->rgb2yuv422order(src, dst, width, height, YUV422_UVYY, unit);
}

void CYuv422F::Rgb2Vuyy(char *src, char *dst, int width, int height, int unit)
{
	this->rgb2yuv422order(src, dst, width, height, YUV422_VUYY, unit);
}

void CYuv422F::Rgb2Yyvu(char *src, char *dst, int width, int height, int unit)
{
	this->rgb2yuv422order(src, dst, width, height, YUV422_YYVU, unit);
}

//since unit of RGB24 is 1, it is a must to let unit = 1 while calling this function.
void CYuv422F::Rgb2Yuv422Blk(char *src, char *dst, int width, int height, int unit)
{
	int j, len, size_blk;
	CFileBin fsrc, fdst;
	CBufferBase srcbuf, dstbuf;

	if( (unit != 1) && (unit != 2) ) 
		unit = 1;
	if(width % 2)
	{
		this->m_dbug.PrintMsg("Image width must be even\n");
		return;
	}
	fsrc.Open(src);
	fdst.Open(dst, "wb");
	size_blk = (width * unit * 3) << 3; //8 lines RGB24 data
	dstbuf.SetSize(size_blk);
			
	for(j = 0; j < height; j += 8)
	{
		fsrc.Read(srcbuf.m_buf, size_blk);

		len = CYuv422::Rgb2Yuv422Blk(srcbuf.m_buf, dstbuf.m_buf, width, 8, unit);
		fdst.Write(dstbuf.m_buf, len);
	}	
}

//since unit of RGB24 is 1, it is a must to let unit = 1 while calling this function.
void CYuv422F::Rgb2Jpeg422(char *src, char *dest, int width, int height, int unit, int qfact)
{
	CFileBin fsrc, fdst;
	CBufferBase srcbuf, dstbuf;
	int size;

	if(width % 2)
	{
		this->m_dbug.PrintMsg("Image width must be even\n");
		return;
	}

	size = CYuv422::getVwc(width, height);
	srcbuf.SetSize((size*3) >> 1, unit);
	dstbuf.SetSize(size, unit);
	if( (srcbuf.GetTotalSize() == 0) && (dstbuf.GetTotalSize() == 0) )
	{
		this->m_dbug.PrintMsg("Can not allocate so big buffer\n");
		return;
	}
	fsrc.Open(src);
	fsrc.Read(srcbuf.m_buf, (size*3) >> 1);
	size = CYuv422::Rgb2Jpeg422(srcbuf.m_buf, dstbuf.m_buf, width, height, qfact, unit);
	fdst.Open(dest, "wb");
	fdst.Write(dstbuf.m_buf, size);
}

//====================================================
//
//					yuv422 convert to rgb
//
//====================================================
void CYuv422F::yuv422_rgborder(char *src, char *dst, int width, int height, int order, int unit)
{
	int i, len, size, cap;
	CFileBin fsrc, fdst;
	CBufferBase srcbuf, dstbuf;
//	int flag = unit;

	if( (unit != 1) && (unit != 2) )
		unit = 1;
	if(width % 2)
	{
		this->m_dbug.PrintMsg("Image width must be even\n");
		return;
	}
	fsrc.Open(src);
	fdst.Open(dst, "wb");
	cap = 256;
	if(unit == 2)
		cap = 1024;
	size = unit << 2;
	len = width * height * unit * 2;
	for(i = 0; i < len; i += size)
	{
		fsrc.Read(srcbuf.m_buf, size);
		
			width = CYuv422::yuv422_rgborder(srcbuf.m_buf, dstbuf.m_buf, order, cap, unit);
		fdst.Write(dstbuf.m_buf, width);
	}
}

void CYuv422F::Yyuv2Rgb(char *src, char *dest, int width, int height, int unit)
{
	this->yuv422_rgborder(src, dest, width, height, YUV422_YYUV, unit);
}

void CYuv422F::Vyuy2Rgb(char *src, char *dest, int width, int height, int unit)
{
	this->yuv422_rgborder(src, dest, width, height, YUV422_VYUY, unit);
}

void CYuv422F::Yuyv2Rgb(char *src, char *dest, int width, int height, int unit)
{
	this->yuv422_rgborder(src, dest, width, height, YUV422_YUYV, unit);
}

void CYuv422F::Uyvy2Rgb(char *src, char *dest, int width, int height, int unit)
{
	this->yuv422_rgborder(src, dest, width, height, YUV422_UYVY, unit);
}

void CYuv422F::Yvyu2Rgb(char *src, char *dest, int width, int height, int unit)
{
	this->yuv422_rgborder(src, dest, width, height, YUV422_YVYU, unit);
}

void CYuv422F::Yuvy2Rgb(char *src, char *dest, int width, int height, int unit)
{
	this->yuv422_rgborder(src, dest, width, height, YUV422_YUVY, unit);
}

void CYuv422F::Yvuy2Rgb(char *src, char *dest, int width, int height, int unit)
{
	this->yuv422_rgborder(src, dest, width, height, YUV422_YVUY, unit);
}

void CYuv422F::Uvyy2Rgb(char *src, char *dest, int width, int height, int unit)
{
	this->yuv422_rgborder(src, dest, width, height, YUV422_UVYY, unit);
}

void CYuv422F::Vuyy2Rgb(char *src, char *dest, int width, int height, int unit)
{
	this->yuv422_rgborder(src, dest, width, height, YUV422_VUYY, unit);
}

void CYuv422F::Yyvu2Rgb(char *src, char *dest, int width, int height, int unit)
{
	this->yuv422_rgborder(src, dest, width, height, YUV422_YYVU, unit);
}

//====================================================
//
//					yuv422 convert to block
//
//====================================================
void CYuv422F::yuv422_blkorder1(char *src, char *dest, int width, int height, int order, int unit)
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

void CYuv422F::yuv422_blkorder(char *src, char *dest, int width, int height, int order, int unit)
{
	int j, len, size;
	CFileBin fsrc, fdst;
	CBufferBase srcbuf, dstbuf;
//	int flag = unit;

	if( (unit != 1) && (unit != 2) )
		unit = 1;
	if(width % 2)
	{
		this->m_dbug.PrintMsg("Image width must be even\n");
		return;
	}
	fsrc.Open(src);
	fdst.Open(dest, "wb");
	size = (width * unit) << 4;
	srcbuf.SetSize(size);
	dstbuf.SetSize(size);
	for(j = 0; j < height; j += 8)
	{
		fsrc.Read(srcbuf.m_buf, size);
		
			len = CYuv422::yuv422_blkorder(srcbuf.m_buf, dstbuf.m_buf, order, width, 8, unit);
		fdst.Write(dstbuf.m_buf, len);
	}
}

void CYuv422F::Yyuv2Blk(char *src, char *dest, int width, int height, int unit)
{
	this->yuv422_blkorder(src, dest, width, height, YUV422_YYUV, unit);
}

void CYuv422F::Yuyv2Blk(char *src, char *dst, int width, int height, int unit)
{
	this->yuv422_blkorder(src, dst, width, height, YUV422_YUYV, unit);
}

void CYuv422F::Uyvy2Blk(char *src, char *dst, int width, int height, int unit)
{
	this->yuv422_blkorder(src, dst, width, height, YUV422_UYVY, unit);
}

void CYuv422F::Vyuy2Blk(char *src, char *dst, int width, int height, int unit)
{
	this->yuv422_blkorder(src, dst, width, height, YUV422_VYUY, unit);
}

void CYuv422F::Yvyu2Blk(char *src, char *dst, int width, int height, int unit)
{
	this->yuv422_blkorder(src, dst, width, height, YUV422_YVYU, unit);
}

void CYuv422F::Yuvy2Blk(char *src, char *dst, int width, int height, int unit)
{
	this->yuv422_blkorder(src, dst, width, height, YUV422_YUVY, unit);
}

void CYuv422F::Yvuy2Blk(char *src, char *dst, int width, int height, int unit)
{
	this->yuv422_blkorder(src, dst, width, height, YUV422_YVUY, unit);
}

void CYuv422F::Uvyy2Blk(char *src, char *dst, int width, int height, int unit)
{
	this->yuv422_blkorder(src, dst, width, height, YUV422_UVYY, unit);
}

void CYuv422F::Vuyy2Blk(char *src, char *dst, int width, int height, int unit)
{
	this->yuv422_blkorder(src, dst, width, height, YUV422_VUYY, unit);
}

void CYuv422F::Yyvu2Blk(char *src, char *dst, int width, int height, int unit)
{
	this->yuv422_blkorder(src, dst, width, height, YUV422_YYVU, unit);
}

//====================================================
//
//					block convert to YUV422
//
//====================================================
void CYuv422F::blk2yuv422order(char *src, char *dest, int width, int height, int order, int unit)
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

void CYuv422F::blk2yuv422order1(char *src, char *dest, int width, int height, int order, int unit)
{
	int j, len, size;
	CFileBin fsrc, fdst;
	CBufferBase srcbuf, dstbuf;
//	int flag = unit;

	if( (unit != 1) && (unit != 2) )
		unit = 1;
	if(width % 2)
	{
		this->m_dbug.PrintMsg("Image width must be even\n");
		return;
	}
	fsrc.Open(src);
	fdst.Open(dest, "wb");
	size = (width * unit) << 4;
	srcbuf.SetSize(size);
	dstbuf.SetSize(size);
	for(j = 0; j < height; j += 8)
	{
		fsrc.Read(srcbuf.m_buf, size);
		
			len = CYuv422::blk2yuv422order(srcbuf.m_buf, dstbuf.m_buf, order, width, 8, unit);
		fdst.Write(dstbuf.m_buf, len);
	}
}

void CYuv422F::Blk2Yyuv(char *src, char *dest, int width, int height, int unit)
{
	this->blk2yuv422order(src, dest, width, height, YUV422_YYUV, unit);
}

void CYuv422F::Blk2Yuyv(char *src, char *dst, int width, int height, int unit)
{
	this->blk2yuv422order(src, dst, width, height, YUV422_YUYV, unit);
}

void CYuv422F::Blk2Uyvy(char *src, char *dst, int width, int height, int unit)
{
	this->blk2yuv422order(src, dst, width, height, YUV422_UYVY, unit);
}

void CYuv422F::Blk2Vyuy(char *src, char *dst, int width, int height, int unit)
{
	this->blk2yuv422order(src, dst, width, height, YUV422_VYUY, unit);
}

void CYuv422F::Blk2Yvyu(char *src, char *dst, int width, int height, int unit)
{
	this->blk2yuv422order(src, dst, width, height, YUV422_YVYU, unit);
}

void CYuv422F::Blk2Yuvy(char *src, char *dst, int width, int height, int unit)
{
	this->blk2yuv422order(src, dst, width, height, YUV422_YUVY, unit);
}

void CYuv422F::Blk2Yvuy(char *src, char *dst, int width, int height, int unit)
{
	this->blk2yuv422order(src, dst, width, height, YUV422_YVUY, unit);
}

void CYuv422F::Blk2Uvyy(char *src, char *dst, int width, int height, int unit)
{
	this->blk2yuv422order(src, dst, width, height, YUV422_UVYY, unit);
}

void CYuv422F::Blk2Vuyy(char *src, char *dst, int width, int height, int unit)
{
	this->blk2yuv422order(src, dst, width, height, YUV422_VUYY, unit);
}

void CYuv422F::Blk2Yyvu(char *src, char *dst, int width, int height, int unit)
{
	this->blk2yuv422order(src, dst, width, height, YUV422_YYVU, unit);
}

void CYuv422F::Blk2Rgb24(char *src, char *dest, int width, int height)
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
	srcbuf.SetSize(width * height << 1);
	dstbuf.SetSize(msize);
	size = fsrc.Read(srcbuf.m_buf, width * height << 1);
	if (size == 0)
		return;

	ncount = CYuv422::Blk2Rgb24(srcbuf.m_buf, dstbuf.m_buf, width, height);

	fdst.Write(dstbuf.m_buf, ncount);
}


//====================================================
//
//					yuv422 convert to lbuf
//
//====================================================
void CYuv422F::yuv422_lbuforder(char *src, char *dest, int width, int height, int order, int unit)
{
	int j, len, size;
	CFileBin fsrc, fdst;
	CBufferBase srcbuf, dstbuf;
//	int flag = unit;

	if( (unit != 1) && (unit != 2) )
		unit = 1;
	if(width % 2)
	{
		this->m_dbug.PrintMsg("Image width must be even\n");
		return;
	}
	fsrc.Open(src);
	fdst.Open(dest, "wb");
	size = (width * unit) << 4;
	srcbuf.SetSize(size);
	dstbuf.SetSize(size);
	for(j = 0; j < height; j += 8)
	{
		fsrc.Read(srcbuf.m_buf, size);
	
			len = CYuv422::yuv422_lbuforder(srcbuf.m_buf, dstbuf.m_buf, order, width, 8, unit);
		fdst.Write(dstbuf.m_buf, len);
	}
}

void CYuv422F::Yyuv2Lbuf(char *src, char *dst, int width, int height, int unit)
{
	this->yuv422_lbuforder(src, dst, width, height, YUV422_YYUV, unit);
}

void CYuv422F::Yuyv2Lbuf(char *src, char *dst, int width, int height, int unit)
{
	this->yuv422_lbuforder(src, dst, width, height, YUV422_YUYV, unit);
}

void CYuv422F::Yvyu2Lbuf(char *src, char *dst, int width, int height, int unit)
{
	this->yuv422_lbuforder(src, dst, width, height, YUV422_YVYU, unit);
}

void CYuv422F::Yyvu2Lbuf(char *src, char *dst, int width, int height, int unit)
{
	this->yuv422_lbuforder(src, dst, width, height, YUV422_YYVU, unit);
}

void CYuv422F::Yuvy2Lbuf(char *src, char *dst, int width, int height, int unit)
{
	this->yuv422_lbuforder(src, dst, width, height, YUV422_YUVY, unit);
}

void CYuv422F::Yvuy2Lbuf(char *src, char *dst, int width, int height, int unit)
{
	this->yuv422_lbuforder(src, dst, width, height, YUV422_YVUY, unit);
}

void CYuv422F::Uvyy2Lbuf(char *src, char *dst, int width, int height, int unit)
{
	this->yuv422_lbuforder(src, dst, width, height, YUV422_UVYY, unit);
}

void CYuv422F::Vuyy2Lbuf(char *src, char *dst, int width, int height, int unit)
{
	this->yuv422_lbuforder(src, dst, width, height, YUV422_VUYY, unit);
}

void CYuv422F::Vyuy2Lbuf(char *src, char *dst, int width, int height, int unit)
{
	this->yuv422_lbuforder(src, dst, width, height, YUV422_VYUY, unit);
}

void CYuv422F::Uyvy2Lbuf(char *src, char *dst, int width, int height, int unit)
{
	this->yuv422_lbuforder(src, dst, width, height, YUV422_UYVY, unit);
}



//====================================================
//
//					yuv422 convert to lbuf
//
//====================================================
void CYuv422F::lbuf2yuv422order(char *src, char *dest, int width, int height, int order, int unit)
{
	int j, len, size;
	CFileBin fsrc, fdst;
	CBufferBase srcbuf, dstbuf;
//	int flag = unit;

	if( (unit != 1) && (unit != 2) )
		unit = 1;
	if(width % 2)
	{
		this->m_dbug.PrintMsg("Image width must be even\n");
		return;
	}
	fsrc.Open(src);
	fdst.Open(dest, "wb");
	size = (width * unit) << 4;
	srcbuf.SetSize(size);
	dstbuf.SetSize(size);
	for(j = 0; j < height; j += 8)
	{
		fsrc.Read(srcbuf.m_buf, size);
		
			len = CYuv422::lbuf2yuv422order(srcbuf.m_buf, dstbuf.m_buf, order, width, 8, unit);
		fdst.Write(dstbuf.m_buf, len);
	}
}

void CYuv422F::lbuf4442yuv422order(char *src, char *dest, int width, int height, int order, int unit)
{
	int j, len, size;
	CFileBin fsrc, fdst;
	CBufferBase srcbuf, dstbuf;
//	int flag = unit;

	if( (unit != 1) && (unit != 2) )
		unit = 1;
	if(width % 2)
	{
		this->m_dbug.PrintMsg("Image width must be even\n");
		return;
	}
	fsrc.Open(src);
	fdst.Open(dest, "wb");
	size = (width * unit) << 4;
	srcbuf.SetSize(size);
	dstbuf.SetSize(size);
	for(j = 0; j < height; j += 8)
	{
		fsrc.Read(srcbuf.m_buf, size);
		
			len = CYuv422::lbuf4442yuv422order(srcbuf.m_buf, dstbuf.m_buf, order, width, 8, unit);
		fdst.Write(dstbuf.m_buf, len);
	}
}


void CYuv422F::Lbuf2Yyuv(char *src, char *dst, int width, int height, int unit)
{
	this->lbuf2yuv422order(src, dst, width, height, YUV422_YYUV, unit);
}

void CYuv422F::Lbuf2Yuyv(char *src, char *dst, int width, int height, int unit)
{
	this->lbuf2yuv422order(src, dst, width, height, YUV422_YUYV, unit);
}

void CYuv422F::Lbuf2Yvyu(char *src, char *dst, int width, int height, int unit)
{
	this->lbuf2yuv422order(src, dst, width, height, YUV422_YVYU, unit);
}

void CYuv422F::Lbuf2Yyvu(char *src, char *dst, int width, int height, int unit)
{
	this->lbuf2yuv422order(src, dst, width, height, YUV422_YYVU, unit);
}

void CYuv422F::Lbuf2Yuvy(char *src, char *dst, int width, int height, int unit)
{
	this->lbuf2yuv422order(src, dst, width, height, YUV422_YUVY, unit);
}

void CYuv422F::Lbuf2Yvuy(char *src, char *dst, int width, int height, int unit)
{
	this->lbuf2yuv422order(src, dst, width, height, YUV422_YVUY, unit);
}

void CYuv422F::Lbuf2Uvyy(char *src, char *dst, int width, int height, int unit)
{
	this->lbuf2yuv422order(src, dst, width, height, YUV422_UVYY, unit);
}

void CYuv422F::Lbuf2Vuyy(char *src, char *dst, int width, int height, int unit)
{
	this->lbuf2yuv422order(src, dst, width, height, YUV422_VUYY, unit);
}

void CYuv422F::Lbuf2Vyuy(char *src, char *dst, int width, int height, int unit)
{
	this->lbuf2yuv422order(src, dst, width, height, YUV422_VYUY, unit);
}

void CYuv422F::Lbuf2Uyvy(char *src, char *dst, int width, int height, int unit)
{
	this->lbuf2yuv422order(src, dst, width, height, YUV422_UYVY, unit);
}


////////////////////////////////////////////////////
void CYuv422F::Lbuf444422_2Yyuv(char *src, char *dst, int width, int height, int unit)
{
	this->lbuf4442yuv422order(src, dst, width, height, YUV422_YYUV, unit);
}

void CYuv422F::Lbuf444422_2Yuyv(char *src, char *dst, int width, int height, int unit)
{
	this->lbuf4442yuv422order(src, dst, width, height, YUV422_YUYV, unit);
}

void CYuv422F::Lbuf444422_2Yvyu(char *src, char *dst, int width, int height, int unit)
{
	this->lbuf4442yuv422order(src, dst, width, height, YUV422_YVYU, unit);
}

void CYuv422F::Lbuf444422_2Yyvu(char *src, char *dst, int width, int height, int unit)
{
	this->lbuf4442yuv422order(src, dst, width, height, YUV422_YYVU, unit);
}

void CYuv422F::Lbuf444422_2Yuvy(char *src, char *dst, int width, int height, int unit)
{
	this->lbuf4442yuv422order(src, dst, width, height, YUV422_YUVY, unit);
}

void CYuv422F::Lbuf444422_2Yvuy(char *src, char *dst, int width, int height, int unit)
{
	this->lbuf4442yuv422order(src, dst, width, height, YUV422_YVUY, unit);
}

void CYuv422F::Lbuf444422_2Uvyy(char *src, char *dst, int width, int height, int unit)
{
	this->lbuf4442yuv422order(src, dst, width, height, YUV422_UVYY, unit);
}

void CYuv422F::Lbuf444422_2Vuyy(char *src, char *dst, int width, int height, int unit)
{
	this->lbuf4442yuv422order(src, dst, width, height, YUV422_VUYY, unit);
}

void CYuv422F::Lbuf444422_2Vyuy(char *src, char *dst, int width, int height, int unit)
{
	this->lbuf4442yuv422order(src, dst, width, height, YUV422_VYUY, unit);
}

void CYuv422F::Lbuf444422_2Uyvy(char *src, char *dst, int width, int height, int unit)
{
	this->lbuf4442yuv422order(src, dst, width, height, YUV422_UYVY, unit);
}



//====================================================
//
//					yuv444 convert to YUV422
//
//====================================================
void CYuv422F::yuv422_444order(char *src, char *dst, int width, int height, int order, int unit)
{
	int i, len, size;
	CFileBin fsrc, fdst;
	CBufferBase srcbuf, dstbuf;
//	int flag = unit;

	if( (unit != 1) && (unit != 2) )
		unit = 1;
	if(width % 2)
	{
		this->m_dbug.PrintMsg("Image width must be even\n");
		return;
	}
	fsrc.Open(src);
	fdst.Open(dst, "wb");
	size = unit << 2;
	len = width * height * unit * 2;
	for(i = 0; i < len; i += size)
	{
		width = fsrc.Read(srcbuf.m_buf, size);
	
			width = CYuv422::yuv422_444order(srcbuf.m_buf, dstbuf.m_buf, order, unit);
		fdst.Write(dstbuf.m_buf, width);
	}
}

void CYuv422F::Yyuv2Yuv444(char *src, char *dst, int width, int height, int unit)
{
	this->yuv422_444order(src, dst, width, height, YUV422_YYUV, unit);
}

void CYuv422F::Yuyv2Yuv444(char *src, char *dst, int width, int height, int unit)
{
	this->yuv422_444order(src, dst, width, height, YUV422_YUYV, unit);
}

void CYuv422F::Uyvy2Yuv444(char *src, char *dst, int width, int height, int unit)
{
	this->yuv422_444order(src, dst, width, height, YUV422_UYVY, unit);
}

void CYuv422F::Vyuy2Yuv444(char *src, char *dst, int width, int height, int unit)
{
	this->yuv422_444order(src, dst, width, height, YUV422_VYUY, unit);
}

void CYuv422F::Yvyu2Yuv444(char *src, char *dst, int width, int height, int unit)
{
	this->yuv422_444order(src, dst, width, height, YUV422_YVYU, unit);
}

void CYuv422F::Yuvy2Yuv444(char *src, char *dst, int width, int height, int unit)
{
	this->yuv422_444order(src, dst, width, height, YUV422_YUVY, unit);
}

void CYuv422F::Yvuy2Yuv444(char *src, char *dst, int width, int height, int unit)
{
	this->yuv422_444order(src, dst, width, height, YUV422_YVUY, unit);
}

void CYuv422F::Uvyy2Yuv444(char *src, char *dst, int width, int height, int unit)
{
	this->yuv422_444order(src, dst, width, height, YUV422_UVYY, unit);
}

void CYuv422F::Vuyy2Yuv444(char *src, char *dst, int width, int height, int unit)
{
	this->yuv422_444order(src, dst, width, height, YUV422_VUYY, unit);
}

void CYuv422F::Yyvu2Yuv444(char *src, char *dst, int width, int height, int unit)
{
	this->yuv422_444order(src, dst, width, height, YUV422_YYVU, unit);
}


//====================================================
//
//					yuv422 convert to YUV444
//
//====================================================
void CYuv422F::yuv444_422order(char *src, char *dst, int width, int height, int order, int unit)
{
	int i, len, size;
	CFileBin fsrc, fdst;
	CBufferBase srcbuf, dstbuf;
//	int flag = unit;

	if( (unit != 1) && (unit != 2) )
		unit = 1;
	if(width % 2)
	{
		this->m_dbug.PrintMsg("Image width must be even\n");
		return;
	}
	fsrc.Open(src);
	fdst.Open(dst, "wb");
	size = unit * 6;
	len = width * height * unit * 3;
	for(i = 0; i < len; i += size)
	{
		fsrc.Read(srcbuf.m_buf, size);
	
			width = CYuv422::yuv444_422order(srcbuf.m_buf, dstbuf.m_buf, order, unit);
		fdst.Write(dstbuf.m_buf, width);
	}
}

void CYuv422F::Yuv444_Yyuv(char *src, char *dst, int width, int height, int unit)
{
	this->yuv444_422order(src, dst, width, height, YUV422_YYUV, unit);
}

void CYuv422F::Yuv444_Yuyv(char *src, char *dst, int width, int height, int unit)
{
	this->yuv444_422order(src, dst, width, height, YUV422_YUYV, unit);
}

void CYuv422F::Yuv444_Uyvy(char *src, char *dst, int width, int height, int unit)
{
	this->yuv444_422order(src, dst, width, height, YUV422_UYVY, unit);
}

void CYuv422F::Yuv444_Vyuy(char *src, char *dst, int width, int height, int unit)
{
	this->yuv444_422order(src, dst, width, height, YUV422_VYUY, unit);
}

void CYuv422F::Yuv444_Yvyu(char *src, char *dst, int width, int height, int unit)
{
	this->yuv444_422order(src, dst, width, height, YUV422_YVYU, unit);
}

void CYuv422F::Yuv444_Yuvy(char *src, char *dst, int width, int height, int unit)
{
	this->yuv444_422order(src, dst, width, height, YUV422_YUVY, unit);
}

void CYuv422F::Yuv444_Yvuy(char *src, char *dst, int width, int height, int unit)
{
	this->yuv444_422order(src, dst, width, height, YUV422_YVUY, unit);
}

void CYuv422F::Yuv444_Uvyy(char *src, char *dst, int width, int height, int unit)
{
	this->yuv444_422order(src, dst, width, height, YUV422_UVYY, unit);
}

void CYuv422F::Yuv444_Vuyy(char *src, char *dst, int width, int height, int unit)
{
	this->yuv444_422order(src, dst, width, height, YUV422_VUYY, unit);
}

void CYuv422F::Yuv444_Yyvu(char *src, char *dst, int width, int height, int unit)
{
	this->yuv444_422order(src, dst, width, height, YUV422_YYVU, unit);
}

//===============================================================
//
//		yuv422 image resize
//
//==============================================================
void CYuv422F::yuv422_resizeorder(char *src, char *dest, int srcw, int srch, 
								 int dstw, int dsth, int order, int unit)
{
	CFileBin fsrc, fdst;
	CBufferBase srcbuf, dstbuf;
	int size, w, h;
//	int flag = unit;

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
	
	size = (srcw * srch * unit) << 1;
	fsrc.Read(srcbuf.m_buf, size);
	
		size = CYuv422::yuv422_resizeorder(srcbuf.m_buf, dstbuf.m_buf, srcw, srch, dstw, dsth, order, unit);
	fdst.Write(dstbuf.m_buf, size);
}

void CYuv422F::YyuvResize(char *src, char *dest, int srcw, int srch, int dstw, int dsth, int unit)
{
	this->yuv422_resizeorder(src, dest, srcw, srch, dstw, dsth, YUV422_YYUV, unit);
}

void CYuv422F::YuyvResize(char *src, char *dest, int srcw, int srch, int dstw, int dsth, int unit)
{
	this->yuv422_resizeorder(src, dest, srcw, srch, dstw, dsth, YUV422_YUYV, unit);
}

void CYuv422F::UyvyResize(char *src, char *dest, int srcw, int srch, int dstw, int dsth, int unit)
{
	this->yuv422_resizeorder(src, dest, srcw, srch, dstw, dsth, YUV422_UYVY, unit);
}

void CYuv422F::VyuyResize(char *src, char *dest, int srcw, int srch, int dstw, int dsth, int unit)
{
	this->yuv422_resizeorder(src, dest, srcw, srch, dstw, dsth, YUV422_VYUY, unit);
}

void CYuv422F::YvyuResize(char *src, char *dest, int srcw, int srch, int dstw, int dsth, int unit)
{
	this->yuv422_resizeorder(src, dest, srcw, srch, dstw, dsth, YUV422_YVYU, unit);
}

void CYuv422F::YuvyResize(char *src, char *dest, int srcw, int srch, int dstw, int dsth, int unit)
{
	this->yuv422_resizeorder(src, dest, srcw, srch, dstw, dsth, YUV422_YUVY, unit);
}

void CYuv422F::YvuyResize(char *src, char *dest, int srcw, int srch, int dstw, int dsth, int unit)
{
	this->yuv422_resizeorder(src, dest, srcw, srch, dstw, dsth, YUV422_YVUY, unit);
}

void CYuv422F::UvyyResize(char *src, char *dest, int srcw, int srch, int dstw, int dsth, int unit)
{
	this->yuv422_resizeorder(src, dest, srcw, srch, dstw, dsth, YUV422_UVYY, unit);
}

void CYuv422F::VuyyResize(char *src, char *dest, int srcw, int srch, int dstw, int dsth, int unit)
{
	this->yuv422_resizeorder(src, dest, srcw, srch, dstw, dsth, YUV422_VUYY, unit);
}

void CYuv422F::YyvuResize(char *src, char *dest, int srcw, int srch, int dstw, int dsth, int unit)
{
	this->yuv422_resizeorder(src, dest, srcw, srch, dstw, dsth, YUV422_YYVU, unit);
}

//===============================================================
//
//		yuv422 image sizer
//
//==============================================================
void CYuv422F::yuv422sizerorder(char *src, char *dest, int srcw, int srch, int dstw, int dsth, 
								 int px, int py, int order, int unit)
{
	CFileBin fsrc, fdst;
	CBufferBase srcbuf, dstbuf;
	int i, size, size1;
	//int flag = unit;

	if( (unit != 1) && (unit != 2) )
		unit = 1;
	if( ((dstw + px) > srcw) || ((dsth + py) > srch) )
	{
		this->m_dbug.PrintMsg("Output Image exceed Input Image\n");
		return;
	}

	if( (srcbuf.GetTotalSize() == 0) && (dstbuf.GetTotalSize() == 0) )
	{
		this->m_dbug.PrintMsg("Can not allocate so big buffer\n");
		return;
	}
	if(dstw % 2)
	{
		this->m_dbug.PrintMsg("Output Image sizer can not be divided by 4\n");
		return;
	}
	fsrc.Open(src);
	fdst.Open(dest, "wb");
	size = (srcw * unit) << 1;
	for(i = 0; i < py; i++)
		fsrc.Read(srcbuf.m_buf, size);
	for(i = 0; i < dsth; i++)
	{
		fsrc.Read(srcbuf.m_buf, size);
	
			size1 = CYuv422::MYuv422Sizer(srcbuf.m_buf, dstbuf.m_buf, srcw, dstw, 1, px, 0, order, unit);
		fdst.Write(dstbuf.m_buf, size1);
	}
}

void CYuv422F::YyuvSizer(char *src, char *dest, int srcw, int srch, int dstw, int dsth, int px, int py, int unit)
{
	this->yuv422sizerorder(src, dest, srcw, srch, dstw, dsth, px, py, YUV422_YYUV, unit);
}

void CYuv422F::YyvuSizer(char *src, char *dest, int srcw, int srch, int dstw, int dsth, int px, int py, int unit)
{
	this->yuv422sizerorder(src, dest, srcw, srch, dstw, dsth, px, py, YUV422_YYVU, unit);
}

void CYuv422F::YuyvSizer(char *src, char *dest, int srcw, int srch, int dstw, int dsth, int px, int py, int unit)
{
	this->yuv422sizerorder(src, dest, srcw, srch, dstw, dsth, px, py, YUV422_YUYV, unit);
}

void CYuv422F::YvyuSizer(char *src, char *dest, int srcw, int srch, int dstw, int dsth, int px, int py, int unit)
{
	this->yuv422sizerorder(src, dest, srcw, srch, dstw, dsth, px, py, YUV422_YVYU, unit);
}

void CYuv422F::UyvySizer(char *src, char *dest, int srcw, int srch, int dstw, int dsth, int px, int py, int unit)
{
	this->yuv422sizerorder(src, dest, srcw, srch, dstw, dsth, px, py, YUV422_UYVY, unit);
}

void CYuv422F::VyuySizer(char *src, char *dest, int srcw, int srch, int dstw, int dsth, int px, int py, int unit)
{
	this->yuv422sizerorder(src, dest, srcw, srch, dstw, dsth, px, py, YUV422_VYUY, unit);
}

void CYuv422F::YuvySizer(char *src, char *dest, int srcw, int srch, int dstw, int dsth, int px, int py, int unit)
{
	this->yuv422sizerorder(src, dest, srcw, srch, dstw, dsth, px, py, YUV422_YUVY, unit);
}

void CYuv422F::YvuySizer(char *src, char *dest, int srcw, int srch, int dstw, int dsth, int px, int py, int unit)
{
	this->yuv422sizerorder(src, dest, srcw, srch, dstw, dsth, px, py, YUV422_YVUY, unit);
}

void CYuv422F::UvyySizer(char *src, char *dest, int srcw, int srch, int dstw, int dsth, int px, int py, int unit)
{
	this->yuv422sizerorder(src, dest, srcw, srch, dstw, dsth, px, py, YUV422_UVYY, unit);
}

void CYuv422F::VuyySizer(char *src, char *dest, int srcw, int srch, int dstw, int dsth, int px, int py, int unit)
{
	this->yuv422sizerorder(src, dest, srcw, srch, dstw, dsth, px, py, YUV422_VUYY, unit);
}

//===============================================================
//
//		yuv422 image prefilter
//
//==============================================================
void CYuv422F::Yuv422Prefilterorder(char *src, char *dest, int width, int height, int order,
					int count, int *filter, int unit)
{
	CFileBin fsrc, fdst;
	CBufferBase srcbuf, dstbuf;
	int size;
	//int flag = unit;

	if( (unit != 1) && (unit != 2) )
		unit = 1;
	size = width + ((count-1) << 1);
	size *= height + ((count-1) << 1);
	size *= unit * 3;
	srcbuf.SetSize(size);
	dstbuf.SetSize(size);
	if( (srcbuf.GetTotalSize() == 0) && (dstbuf.GetTotalSize() == 0) )
	{
		this->m_dbug.PrintMsg("Can not allocate so big buffer\n");
		return;
	}
	fsrc.Open(src);
	fsrc.Read(srcbuf.m_buf, size);
	
	{
		CYuv422::MYuv422_444order(srcbuf.m_buf, dstbuf.m_buf, width, height, order, unit);
		CYuv444::MYuv444Prefilter(dstbuf.m_buf, srcbuf.m_buf, width, height, count, filter, unit);
		size = CYuv422::MYuv444_422order(srcbuf.m_buf, dstbuf.m_buf, width, height, order, unit);
	}
	fdst.Open(dest, "wb");
	fdst.Write(dstbuf.m_buf, size);
}

void CYuv422F::Yuv422Prefilter(char *src, char *dest, int width, int height,
					int count, int *filter, int unit)
{
	CFileBin fsrc, fdst;
	CBufferBase srcbuf, dstbuf;
	int size;
//	int flag = unit;

	width >>= 1;
	size = width + ((count-1) << 1);
	size *= height + ((count-1) << 1);
	size *= unit << 2;
	srcbuf.SetSize(size);
	dstbuf.SetSize(size);
	if( (srcbuf.GetTotalSize() == 0) && (dstbuf.GetTotalSize() == 0) )
	{
		this->m_dbug.PrintMsg("Can not allocate so big buffer\n");
		return;
	}
	fsrc.Open(src);
	fsrc.Read(srcbuf.m_buf, size);
	
		size = CBaseConvert::Prefilter(srcbuf.m_buf, dstbuf.m_buf, width, height, 4, count, filter, unit);
	fdst.Open(dest, "wb");
	fdst.Write(dstbuf.m_buf, size);
}

void CYuv422F::YyuvPrefilter(char *src, char *dest, int width, int height, int count, int *filter, int unit)
{
	this->Yuv422Prefilterorder(src, dest, width, height, YUV422_YYUV, count, filter, unit);
}

void CYuv422F::YyvuPrefilter(char *src, char *dest, int width, int height, int count, int *filter, int unit)
{
	this->Yuv422Prefilterorder(src, dest, width, height, YUV422_YYVU, count, filter, unit);
}

void CYuv422F::YuyvPrefilter(char *src, char *dest, int width, int height, int count, int *filter, int unit)
{
	this->Yuv422Prefilterorder(src, dest, width, height, YUV422_YUYV, count, filter, unit);
}

void CYuv422F::YvyuPrefilter(char *src, char *dest, int width, int height, int count, int *filter, int unit)
{
	this->Yuv422Prefilterorder(src, dest, width, height, YUV422_YVYU, count, filter, unit);
}

void CYuv422F::UyvyPrefilter(char *src, char *dest, int width, int height, int count, int *filter, int unit)
{
	this->Yuv422Prefilterorder(src, dest, width, height, YUV422_UYVY, count, filter, unit);
}

void CYuv422F::VyuyPrefilter(char *src, char *dest, int width, int height, int count, int *filter, int unit)
{
	this->Yuv422Prefilterorder(src, dest, width, height, YUV422_VYUY, count, filter, unit);
}

void CYuv422F::UvyyPrefilter(char *src, char *dest, int width, int height, int count, int *filter, int unit)
{
	this->Yuv422Prefilterorder(src, dest, width, height, YUV422_UVYY, count, filter, unit);
}

void CYuv422F::VuyyPrefilter(char *src, char *dest, int width, int height, int count, int *filter, int unit)
{
	this->Yuv422Prefilterorder(src, dest, width, height, YUV422_VUYY, count, filter, unit);
}

void CYuv422F::YuvyPrefilter(char *src, char *dest, int width, int height, int count, int *filter, int unit)
{
	this->Yuv422Prefilterorder(src, dest, width, height, YUV422_YUVY, count, filter, unit);
}

void CYuv422F::YvuyPrefilter(char *src, char *dest, int width, int height, int count, int *filter, int unit)
{
	this->Yuv422Prefilterorder(src, dest, width, height, YUV422_YVUY, count, filter, unit);
}


//=======================================================
//
//		YUV422 encode
//
//=======================================================
int CYuv422F::Blk2Jpeg(char *src, char *dest, int width, int height, int qf)
{
	CFileBin fsrc, fdst;
	CBufferBase srcbuf, dstbuf;
	int size, q;

	size = CYuv422::getVwc(width, height);
	srcbuf.SetSize(size);
	dstbuf.SetSize(size);
	if( (srcbuf.GetTotalSize() == 0) && (dstbuf.GetTotalSize() == 0) )
	{
		this->m_dbug.PrintMsg("Can not allocate so big buffer\n");
		return qf;
	}
	fsrc.Open(src);
	fsrc.Read(srcbuf.m_buf, size);
	size = CYuv422::Blk2Jpeg(srcbuf.m_buf, dstbuf.m_buf, width, height, &q, qf);
	fdst.Open(dest, "wb");
	fdst.Write(dstbuf.m_buf, size);

	return q;
}
	
//=======================================================
//
//		YUV422 to frame
//
//=======================================================
void CYuv422F::yuv422_frmorder(char *src, char *dst, int width, int height, int order, int unit)
{
	int size;
	CFileBin fsrc, fdst;
	CBufferBase srcbuf, dstbuf;

	if( (unit != 1) && (unit != 2) )
		unit = 1;
	if(width % 2)
	{
		this->m_dbug.PrintMsg("Image width must be even\n");
		return;
	}
	size = (width * height * unit) << 1;
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
	size = CYuv422::MYuv422_FrmOrder(srcbuf.m_buf, dstbuf.m_buf, width, height, order, unit);
	fdst.Write(dstbuf.m_buf, size);
}

void CYuv422F::Yyuv2Frame(char *src, char *dest, int width, int height, int unit)
{
	this->yuv422_frmorder(src, dest, width, height, YUV422_YYUV, unit);
}

void CYuv422F::Yyvu2Frame(char *src, char *dest, int width, int height, int unit)
{
	this->yuv422_frmorder(src, dest, width, height, YUV422_YYVU, unit);
}

void CYuv422F::Yuyv2Frame(char *src, char *dest, int width, int height, int unit)
{
	this->yuv422_frmorder(src, dest, width, height, YUV422_YUYV, unit);
}

void CYuv422F::Yvyu2Frame(char *src, char *dest, int width, int height, int unit)
{
	this->yuv422_frmorder(src, dest, width, height, YUV422_YVYU, unit);
}

void CYuv422F::Yuvy2Frame(char *src, char *dest, int width, int height, int unit)
{
	this->yuv422_frmorder(src, dest, width, height, YUV422_YUVY, unit);
}

void CYuv422F::Yvuy2Frame(char *src, char *dest, int width, int height, int unit)
{
	this->yuv422_frmorder(src, dest, width, height, YUV422_YVUY, unit);
}

void CYuv422F::Uvyy2Frame(char *src, char *dest, int width, int height, int unit)
{
	this->yuv422_frmorder(src, dest, width, height, YUV422_UVYY, unit);
}

void CYuv422F::Vuyy2Frame(char *src, char *dest, int width, int height, int unit)
{
	this->yuv422_frmorder(src, dest, width, height, YUV422_VUYY, unit);
}

void CYuv422F::Vyuy2Frame(char *src, char *dest, int width, int height, int unit)
{
	this->yuv422_frmorder(src, dest, width, height, YUV422_VYUY, unit);
}

void CYuv422F::Uyvy2Frame(char *src, char *dest, int width, int height, int unit)
{
	this->yuv422_frmorder(src, dest, width, height, YUV422_UYVY, unit);
}

//=======================================================
//
//		frame to YUV422
//
//=======================================================
void CYuv422F::frm2yuv422order(char *src, char *dst, int width, int height, int order, int unit)
{
	int size;
	CFileBin fsrc, fdst;
	CBufferBase srcbuf, dstbuf;

	if( (unit != 1) && (unit != 2) )
		unit = 1;
	if(width % 2)
	{
		this->m_dbug.PrintMsg("Image width must be even\n");
		return;
	}
	size = (width * height * unit) << 1;
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
	size = CYuv422::MFrm2Yuv422Order(srcbuf.m_buf, dstbuf.m_buf, width, height, order, unit);
	fdst.Write(dstbuf.m_buf, size);
}

void CYuv422F::Frame2Uyvy(char *src, char *dest, int width, int height, int unit)
{
	this->frm2yuv422order(src, dest, width, height, YUV422_UYVY, unit);
}

void CYuv422F::Frame2Vyuy(char *src, char *dest, int width, int height, int unit)
{
	this->frm2yuv422order(src, dest, width, height, YUV422_VYUY, unit);
}

void CYuv422F::Frame2Uvyy(char *src, char *dest, int width, int height, int unit)
{
	this->frm2yuv422order(src, dest, width, height, YUV422_UVYY, unit);
}

void CYuv422F::Frame2Vuyy(char *src, char *dest, int width, int height, int unit)
{
	this->frm2yuv422order(src, dest, width, height, YUV422_VUYY, unit);
}

void CYuv422F::Frame2Yyuv(char *src, char *dest, int width, int height, int unit)
{
	this->frm2yuv422order(src, dest, width, height, YUV422_YYUV, unit);
}

void CYuv422F::Frame2Yyvu(char *src, char *dest, int width, int height, int unit)
{
	this->frm2yuv422order(src, dest, width, height, YUV422_YYVU, unit);
}

void CYuv422F::Frame2Yuyv(char *src, char *dest, int width, int height, int unit)
{
	this->frm2yuv422order(src, dest, width, height, YUV422_YUYV, unit);
}

void CYuv422F::Frame2Yvyu(char *src, char *dest, int width, int height, int unit)
{
	this->frm2yuv422order(src, dest, width, height, YUV422_YVYU, unit);
}

void CYuv422F::Frame2Yuvy(char *src, char *dest, int width, int height, int unit)
{
	this->frm2yuv422order(src, dest, width, height, YUV422_YUVY, unit);
}

void CYuv422F::Frame2Yvuy(char *src, char *dest, int width, int height, int unit)
{
	this->frm2yuv422order(src, dest, width, height, YUV422_YVUY, unit);
}



