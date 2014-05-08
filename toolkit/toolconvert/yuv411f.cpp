
#include "internal.h"

//===============================================================
//
//		rgb convert to yuv411
//
//==============================================================
void CYuv411F::rgb2yuv411order(char *src, char *dst, int width, int height, int order, int unit)
{
	int i, len, size, cap;
	CFileBin fsrc, fdst;
	CBufferBase srcbuf, dstbuf;
//	int flag = unit;

	if( (unit != 1) && (unit != 2) )
		unit = 1;
	if(width % 4)
	{
		this->m_dbug.PrintMsg("Image width must be divided by 4\n");
		return;
	}
	fsrc.Open(src);
	fdst.Open(dst, "wb");
	cap = 256;
	if(unit == 2)
		cap = 1024;
	size = unit * 3 * 4;
	len = width * height * unit * 3;
	srcbuf.SetSize(size);
	dstbuf.SetSize(size);
	for(i = 0; i < len; i += size)
	{
		width = fsrc.Read(srcbuf.m_buf, size);
		
			width = CYuv411::rgb2yuv411order(srcbuf.m_buf, dstbuf.m_buf, order, cap, unit);
		fdst.Write(dstbuf.m_buf, width);
	}
}

//since unit of RGB24 is 1, it is a must to let unit = 1 while calling this function.
void	CYuv411F::Rgb2Yuv411Blk(char *src, char *dst, int width, int height, int unit )
{
	int j, len, size_blk;
	CFileBin fsrc, fdst;
	CBufferBase srcbuf, dstbuf;

	if( (unit != 1) && (unit != 2) ) 
		unit = 1;
	if(width % 4)
	{
		this->m_dbug.PrintMsg("Image width must be times of 4.\n");
		return;
	}
	fsrc.Open(src);
	fdst.Open(dst, "wb");
	size_blk = (width * unit * 3) << 3; //8 lines RGB24 data
	srcbuf.SetSize(size_blk);
	dstbuf.SetSize(size_blk);
	for(j = 0; j < height; j += 8)
	{
		fsrc.Read(srcbuf.m_buf, size_blk);

		len = CYuv411::Rgb2Yuv411Blk(srcbuf.m_buf, dstbuf.m_buf, width, 8, unit);
		fdst.Write(dstbuf.m_buf, len);
	}
}

//since unit of RGB24 is 1, it is a must to let unit = 1 while calling this function.
void CYuv411F::Rgb2Jpeg411(char *src, char *dest, int width, int height, int unit, int qf)
{

	CFileBin fsrc, fdst;
	CBufferBase srcbuf, dstbuf;
	int size;
	
	if(width % 4)
	{
		this->m_dbug.PrintMsg("Image width must be times of 4.\n");
		return;
	}

	size = CYuv411::getVwc(width, height);
	srcbuf.SetSize(size << 1, unit);
	dstbuf.SetSize(size, unit);
	if( (srcbuf.GetTotalSize() == 0) && (dstbuf.GetTotalSize() == 0) )
	{
		this->m_dbug.PrintMsg("Can not allocate so big buffer\n");
		return;
	}

	fsrc.Open(src);
	fsrc.Read(srcbuf.m_buf, size*unit << 1);
	
	size = CYuv411::Rgb2Jpeg411(srcbuf.m_buf, dstbuf.m_buf, width, height, unit, qf);
	fdst.Open(dest, "wb");
	fdst.Write(dstbuf.m_buf, size);
}
	
void CYuv411F::Rgb2Yyyyuv(char *src, char *dst, int width, int height, int unit)
{
	this->rgb2yuv411order(src, dst, width, height, YUV411_YYYYUV, unit);
}

void CYuv411F::Rgb2Yyyyvu(char *src, char *dst, int width, int height, int unit)
{
	this->rgb2yuv411order(src, dst, width, height, YUV411_YYYYVU, unit);
}

void CYuv411F::Rgb2Yuyvyy(char *src, char *dst, int width, int height, int unit)
{
	this->rgb2yuv411order(src, dst, width, height, YUV411_YUYVYY, unit);
}

void CYuv411F::Rgb2Yvyuyy(char *src, char *dst, int width, int height, int unit)
{
	this->rgb2yuv411order(src, dst, width, height, YUV411_YVYUYY, unit);
}

void CYuv411F::Rgb2Uvyyyy(char *src, char *dst, int width, int height, int unit)
{
	this->rgb2yuv411order(src, dst, width, height, YUV411_UVYYYY, unit);
}

void CYuv411F::Rgb2Vuyyyy(char *src, char *dst, int width, int height, int unit)
{
	this->rgb2yuv411order(src, dst, width, height, YUV411_VUYYYY, unit);
}

//===============================================================
//
//		yuv411 convert to rgb
//
//==============================================================
void CYuv411F::yuv411_rgborder(char *src, char *dst, int width, int height, int order, int unit)
{
	int i, len, size, cap;
	CFileBin fsrc, fdst;
	CBufferBase srcbuf, dstbuf;
//	int flag = unit;

	if( (unit != 1) && (unit != 2) )
		unit = 1;
	if(width % 4)
	{
		this->m_dbug.PrintMsg("Image width must be divided by 4\n");
		return;
	}
	fsrc.Open(src);
	fdst.Open(dst, "wb");
	cap = 256;
	if(unit == 2)
		cap = 1024;
	size = unit * 6;
	len = (width * height * unit * 3) >> 1;
	srcbuf.SetSize(size);
	dstbuf.SetSize(size);
	for(i = 0; i < len; i += size)
	{
		width = fsrc.Read(srcbuf.m_buf, size);
		
			width = CYuv411::yuv411_rgborder(srcbuf.m_buf, dstbuf.m_buf, order, cap, unit);
		fdst.Write(dstbuf.m_buf, width);
	}

}
void CYuv411F::Yyyyuv2Rgb(char *src, char *dst, int width, int height, int unit)
{
	this->yuv411_rgborder(src, dst, width, height, YUV411_YYYYUV, unit);
}

void CYuv411F::Yyyyvu2Rgb(char *src, char *dst, int width, int height, int unit)
{
	this->yuv411_rgborder(src, dst, width, height, YUV411_YYYYVU, unit);
}

void CYuv411F::Yuyvyy2Rgb(char *src, char *dst, int width, int height, int unit)
{
	this->yuv411_rgborder(src, dst, width, height, YUV411_YUYVYY, unit);
}

void CYuv411F::Yvyuyy2Rgb(char *src, char *dst, int width, int height, int unit)
{
	this->yuv411_rgborder(src, dst, width, height, YUV411_YVYUYY, unit);
}

void CYuv411F::Uvyyyy2Rgb(char *src, char *dst, int width, int height, int unit)
{
	this->yuv411_rgborder(src, dst, width, height, YUV411_UVYYYY, unit);
}

void CYuv411F::Vuyyyy2Rgb(char *src, char *dst, int width, int height, int unit)
{
	this->yuv411_rgborder(src, dst, width, height, YUV411_VUYYYY, unit);
}

//===============================================================
//
//		yuv444 convert to yuv411
//
//==============================================================
void CYuv411F::yuv444_411order(char *src, char *dst, int width, int height, int order, int unit)
{
	int i, len, size;
	CFileBin fsrc, fdst;
	CBufferBase srcbuf, dstbuf;
//	int flag = unit;

	if( (unit != 1) && (unit != 2) )
		unit = 1;
	if(width % 4)
	{
		this->m_dbug.PrintMsg("Image width must be divided by 4\n");
		return;
	}
	fsrc.Open(src);
	fdst.Open(dst, "wb");
	size = unit * 3 * 4;
	len = width * height * unit * 3;
	srcbuf.SetSize(size);
	dstbuf.SetSize(size);
	for(i = 0; i < len; i += size)
	{
		width = fsrc.Read(srcbuf.m_buf, size);
		
			width = CYuv411::yuv444_411order(srcbuf.m_buf, dstbuf.m_buf, order, unit);
		fdst.Write(dstbuf.m_buf, width);
	}

}

void CYuv411F::Yuv444_Yyyyuv(char *src, char *dst, int width, int height, int unit)
{
	this->yuv444_411order(src, dst, width, height, YUV411_YYYYUV, unit);
}

void CYuv411F::Yuv444_Yyyyvu(char *src, char *dst, int width, int height, int unit)
{
	this->yuv444_411order(src, dst, width, height, YUV411_YYYYVU, unit);
}

void CYuv411F::Yuv444_Yuyvyy(char *src, char *dst, int width, int height, int unit)
{
	this->yuv444_411order(src, dst, width, height, YUV411_YUYVYY, unit);
}

void CYuv411F::Yuv444_Yvyuyy(char *src, char *dst, int width, int height, int unit)
{
	this->yuv444_411order(src, dst, width, height, YUV411_YVYUYY, unit);
}

void CYuv411F::Yuv444_Uvyyyy(char *src, char *dst, int width, int height, int unit)
{
	this->yuv444_411order(src, dst, width, height, YUV411_UVYYYY, unit);
}

void CYuv411F::Yuv444_Vuyyyy(char *src, char *dst, int width, int height, int unit)
{
	this->yuv444_411order(src, dst, width, height, YUV411_VUYYYY, unit);
}

//===============================================================
//
//		yuv411 convert to yuv444
//
//==============================================================
void CYuv411F::yuv411_444order(char *src, char *dst, int width, int height, int order, int unit)
{
	int i, len, size;
	CFileBin fsrc, fdst;
	CBufferBase srcbuf, dstbuf;
//	int flag = unit;

	if( (unit != 1) && (unit != 2) )
		unit = 1;
	if(width % 4)
	{
		this->m_dbug.PrintMsg("Image width must be divided by 4\n");
		return;
	}
	fsrc.Open(src);
	fdst.Open(dst, "wb");
	size = unit * 6;
	len = (width * height * unit * 3) >> 1;
	srcbuf.SetSize(size);
	dstbuf.SetSize(size);
	for(i = 0; i < len; i += size)
	{
		width = fsrc.Read(srcbuf.m_buf, size);
		
			width = CYuv411::yuv411_444order(srcbuf.m_buf, dstbuf.m_buf, order, unit);
		fdst.Write(dstbuf.m_buf, width);
	}
}

void CYuv411F::Yyyyuv_444(char *src, char *dest, int width, int height, int unit)
{
	this->yuv411_444order(src, dest, width, height, YUV411_YYYYUV, unit);
}

void CYuv411F::Yyyyvu_444(char *src, char *dest, int width, int height, int unit)
{
	this->yuv411_444order(src, dest, width, height, YUV411_YYYYVU, unit);
}

void CYuv411F::Yuyvyy_444(char *src, char *dest, int width, int height, int unit)
{
	this->yuv411_444order(src, dest, width, height, YUV411_YUYVYY, unit);
}

void CYuv411F::Yvyuyy_444(char *src, char *dest, int width, int height, int unit)
{
	this->yuv411_444order(src, dest, width, height, YUV411_YVYUYY, unit);
}

void CYuv411F::Uvyyyy_444(char *src, char *dest, int width, int height, int unit)
{
	this->yuv411_444order(src, dest, width, height, YUV411_UVYYYY, unit);
}

void CYuv411F::Vuyyyy_444(char *src, char *dest, int width, int height, int unit)
{
	this->yuv411_444order(src, dest, width, height, YUV411_VUYYYY, unit);
}

//===============================================================
//
//		yuv411 convert to block
//
//==============================================================
void CYuv411F::yuv411_blkorder(char *src, char *dst, int width, int height, int order, int unit)
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

void CYuv411F::yuv411_blkorder1(char *src, char *dst, int width, int height, int order, int unit)
{
	int j, len, size;
	CFileBin fsrc, fdst;
	CBufferBase srcbuf, dstbuf;
//	int flag = unit;

	if( (unit != 1) && (unit != 2) )
		unit = 1;
	if(width % 4)
	{
		this->m_dbug.PrintMsg("Image width must be even\n");
		return;
	}
	fsrc.Open(src);
	fdst.Open(dst, "wb");
	size = (width * unit * 3) << 2;
	srcbuf.SetSize(size);
	dstbuf.SetSize(size);
	for(j = 0; j < height; j += 8)
	{
		fsrc.Read(srcbuf.m_buf, size);
		
			len = CYuv411::yuv411_blkorder(srcbuf.m_buf, dstbuf.m_buf, order, width, 8, unit);
		fdst.Write(dstbuf.m_buf, len);
	}
}

void CYuv411F::Yyyyuv2Blk(char *src, char *dest, int width, int height, int unit)
{
	this->yuv411_blkorder(src, dest, width, height, YUV411_YYYYUV, unit);
}

void CYuv411F::Yyyyvu2Blk(char *src, char *dest, int width, int height, int unit)
{
	this->yuv411_blkorder(src, dest, width, height, YUV411_YYYYVU, unit);
}

void CYuv411F::Yuyvyy2Blk(char *src, char *dest, int width, int height, int unit)
{
	this->yuv411_blkorder(src, dest, width, height, YUV411_YUYVYY, unit);
}

void CYuv411F::Yvyuyy2Blk(char *src, char *dest, int width, int height, int unit)
{
	this->yuv411_blkorder(src, dest, width, height, YUV411_YVYUYY, unit);
}

void CYuv411F::Uvyyyy2Blk(char *src, char *dest, int width, int height, int unit)
{
	this->yuv411_blkorder(src, dest, width, height, YUV411_UVYYYY, unit);
}

void CYuv411F::Vuyyyy2Blk(char *src, char *dest, int width, int height, int unit)
{
	this->yuv411_blkorder(src, dest, width, height, YUV411_VUYYYY, unit);
}

//===============================================================
//
//		block convert to yuv411
//
//==============================================================
void CYuv411F::blk2yuv411order(char *src, char *dst, int width, int height, int order, int unit)
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

void CYuv411F::blk2yuv411order1(char *src, char *dst, int width, int height, int order, int unit)
{
	int j, len, size;
	CFileBin fsrc, fdst;
	CBufferBase srcbuf, dstbuf;
//	int flag = unit;

	if( (unit != 1) && (unit != 2) )
		unit = 1;
	if(width % 4)
	{
		this->m_dbug.PrintMsg("Image width must be even\n");
		return;
	}
	fsrc.Open(src);
	fdst.Open(dst, "wb");
	size = (width * unit * 3) << 2;
	srcbuf.SetSize(size);
	dstbuf.SetSize(size);
	for(j = 0; j < height; j += 8)
	{
		fsrc.Read(srcbuf.m_buf, size);
		
			len = CYuv411::blk2yuv411order(srcbuf.m_buf, dstbuf.m_buf, order, width, 8, unit);
		fdst.Write(dstbuf.m_buf, len);
	}
}

void CYuv411F::Blk2Yyyyuv(char *src, char *dest, int width, int height, int unit)
{
	this->blk2yuv411order(src, dest, width, height, YUV411_YYYYUV, unit);
}

void CYuv411F::Blk2Yyyyvu(char *src, char *dest, int width, int height, int unit)
{
	this->blk2yuv411order(src, dest, width, height, YUV411_YYYYVU, unit);
}

void CYuv411F::Blk2Yuyvyy(char *src, char *dest, int width, int height, int unit)
{
	this->blk2yuv411order(src, dest, width, height, YUV411_YUYVYY, unit);
}

void CYuv411F::Blk2Yvyuyy(char *src, char *dest, int width, int height, int unit)
{
	this->blk2yuv411order(src, dest, width, height, YUV411_YVYUYY, unit);
}

void CYuv411F::Blk2Uvyyyy(char *src, char *dest, int width, int height, int unit)
{
	this->blk2yuv411order(src, dest, width, height, YUV411_UVYYYY, unit);
}

void CYuv411F::Blk2Vuyyyy(char *src, char *dest, int width, int height, int unit)
{
	this->blk2yuv411order(src, dest, width, height, YUV411_VUYYYY, unit);
}

void CYuv411F::Blk2Rgb24(char * src, char * dest, int width, int height)
{
	int size, msize, ncount = 0;
	CFileBin fsrc, fdst;
	CBufferBase srcbuf, dstbuf;

	if( (width % 4) || (height % 2) )
	{
		this->m_dbug.PrintMsg("Image width must be divided by 4;\n height must be even\n");
		return;
	}
	fsrc.Open(src);
	fdst.Open(dest, "wb");
	msize = (height * width) * 3;
	srcbuf.SetSize(msize >> 1);
	dstbuf.SetSize(msize);
	size = fsrc.Read(srcbuf.m_buf, msize);
	if(size == 0)
		return;
	
	ncount = CYuv411::Blk2Rgb24(srcbuf.m_buf, dstbuf.m_buf, width, height);
	fdst.Write(dstbuf.m_buf, ncount);
}

//===============================================================
//
//		yuv411 convert to lbuf
//
//==============================================================
void CYuv411F::yuv411_lbuforder(char *src, char *dst, int width, int height, int order, int unit)
{
	int j, len, size;
	CFileBin fsrc, fdst;
	CBufferBase srcbuf, dstbuf;
//	int flag = unit;

	if( (unit != 1) && (unit != 2) )
		unit = 1;
	if(width % 4)
	{
		this->m_dbug.PrintMsg("Image width must be even\n");
		return;
	}
	fsrc.Open(src);
	fdst.Open(dst, "wb");
	size = (width * unit * 3) << 2;
	srcbuf.SetSize(size);
	dstbuf.SetSize(size);
	for(j = 0; j < height; j += 8)
	{
		fsrc.Read(srcbuf.m_buf, size);
		
			len = CYuv411::yuv411_lbuforder(srcbuf.m_buf, dstbuf.m_buf, order, width, 8, unit);
		fdst.Write(dstbuf.m_buf, len);
	}
}

void CYuv411F::Yyyyuv2Lbuf(char *src, char *dest, int width, int height, int unit)
{
	this->yuv411_lbuforder(src, dest, width, height, YUV411_YYYYUV, unit);
}

void CYuv411F::Yyyyvu2Lbuf(char *src, char *dest, int width, int height, int unit)
{
	this->yuv411_lbuforder(src, dest, width, height, YUV411_YYYYVU, unit);
}

void CYuv411F::Yuyvyy2Lbuf(char *src, char *dest, int width, int height, int unit)
{
	this->yuv411_lbuforder(src, dest, width, height, YUV411_YUYVYY, unit);
}

void CYuv411F::Yvyuyy2Lbuf(char *src, char *dest, int width, int height, int unit)
{
	this->yuv411_lbuforder(src, dest, width, height, YUV411_YVYUYY, unit);
}

void CYuv411F::Uvyyyy2Lbuf(char *src, char *dest, int width, int height, int unit)
{
	this->yuv411_lbuforder(src, dest, width, height, YUV411_UVYYYY, unit);
}

void CYuv411F::Vuyyyy2Lbuf(char *src, char *dest, int width, int height, int unit)
{
	this->yuv411_lbuforder(src, dest, width, height, YUV411_VUYYYY, unit);
}

//===============================================================
//
//		lbuf convert to yuv411
//
//==============================================================
void CYuv411F::lbuf2yuv411order(char *src, char *dst, int width, int height, int order, int unit)
{
	int j, len, size;
	CFileBin fsrc, fdst;
	CBufferBase srcbuf, dstbuf;
//	int flag = unit;

	if( (unit != 1) && (unit != 2) )
		unit = 1;
	if(width % 4)
	{
		this->m_dbug.PrintMsg("Image width must be even\n");
		return;
	}
	fsrc.Open(src);
	fdst.Open(dst, "wb");
	size = (width * unit * 3) << 2;
	srcbuf.SetSize(size);
	dstbuf.SetSize(size);
	for(j = 0; j < height; j += 8)
	{
		fsrc.Read(srcbuf.m_buf, size);
		
			len = CYuv411::lbuf2yuv411order(srcbuf.m_buf, dstbuf.m_buf, order, width, 8, unit);
		fdst.Write(dstbuf.m_buf, len);
	}
}

void CYuv411F::Lbuf2Yyyyuv(char *src, char *dest, int width, int height, int unit)
{
	this->lbuf2yuv411order(src, dest, width, height, YUV411_YYYYUV, unit);
}

void CYuv411F::Lbuf2Yyyyvu(char *src, char *dest, int width, int height, int unit)
{
	this->lbuf2yuv411order(src, dest, width, height, YUV411_YYYYVU, unit);
}

void CYuv411F::Lbuf2Yuyvyy(char *src, char *dest, int width, int height, int unit)
{
	this->lbuf2yuv411order(src, dest, width, height, YUV411_YUYVYY, unit);
}

void CYuv411F::Lbuf2Yvyuyy(char *src, char *dest, int width, int height, int unit)
{
	this->lbuf2yuv411order(src, dest, width, height, YUV411_YVYUYY, unit);
}

void CYuv411F::Lbuf2Uvyyyy(char *src, char *dest, int width, int height, int unit)
{
	this->lbuf2yuv411order(src, dest, width, height, YUV411_UVYYYY, unit);
}

void CYuv411F::Lbuf2Vuyyyy(char *src, char *dest, int width, int height, int unit)
{
	this->lbuf2yuv411order(src, dest, width, height, YUV411_VUYYYY, unit);
}


//===============================================================
//
//		yuv411 image resize
//
//==============================================================
void CYuv411F::yuv411_resizeorder(char *src, char *dest, int srcw, int srch, 
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
	
	size = (srcw * srch * 3 * unit) >> 1;
	fsrc.Read(srcbuf.m_buf, size);
	
		size = CYuv411::yuv411_resizeorder(srcbuf.m_buf, dstbuf.m_buf, srcw, srch, dstw, dsth, order, unit);
	fdst.Write(dstbuf.m_buf, size);
}

void CYuv411F::YyyyuvResize(char *src, char *dest, int srcw, int srch, int dstw, int dsth, int unit)
{
	this->yuv411_resizeorder(src, dest, srcw, srch, dstw, dsth, YUV411_YYYYUV, unit);
}

void CYuv411F::YyyyvuResize(char *src, char *dest, int srcw, int srch, int dstw, int dsth, int unit)
{
	this->yuv411_resizeorder(src, dest, srcw, srch, dstw, dsth, YUV411_YYYYVU, unit);
}

void CYuv411F::YuyvyyResize(char *src, char *dest, int srcw, int srch, int dstw, int dsth, int unit)
{
	this->yuv411_resizeorder(src, dest, srcw, srch, dstw, dsth, YUV411_YUYVYY, unit);
}

void CYuv411F::YvyuyyResize(char *src, char *dest, int srcw, int srch, int dstw, int dsth, int unit)
{
	this->yuv411_resizeorder(src, dest, srcw, srch, dstw, dsth, YUV411_YVYUYY, unit);
}

void CYuv411F::UvyyyyResize(char *src, char *dest, int srcw, int srch, int dstw, int dsth, int unit)
{
	this->yuv411_resizeorder(src, dest, srcw, srch, dstw, dsth, YUV411_UVYYYY, unit);
}

void CYuv411F::VuyyyyResize(char *src, char *dest, int srcw, int srch, int dstw, int dsth, int unit)
{
	this->yuv411_resizeorder(src, dest, srcw, srch, dstw, dsth, YUV411_VUYYYY, unit);
}

//===============================================================
//
//		yuv411 image sizer
//
//==============================================================
void CYuv411F::yuv411sizerorder(char *src, char *dest, int srcw, int srch, int dstw, int dsth, 
								 int px, int py, int order, int unit)
{
	CFileBin fsrc, fdst;
	CBufferBase srcbuf, dstbuf;
	int i, size, size1;
//	int flag = unit;

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
	if(dstw % 4)
	{
		this->m_dbug.PrintMsg("Output Image sizer can not be divided by 4\n");
		return;
	}
	fsrc.Open(src);
	fdst.Open(dest, "wb");
	size = (srcw * unit * 3) >> 1;
	srcbuf.SetSize(size);
	dstbuf.SetSize(size);
	for(i = 0; i < py; i++)
		fsrc.Read(srcbuf.m_buf, size);
	for(i = 0; i < dsth; i++)
	{
		fsrc.Read(srcbuf.m_buf, size);
		
			size1 = CYuv411::MYuv411Sizer(srcbuf.m_buf, dstbuf.m_buf, srcw, dstw, 1, px, 0, order, unit);
		fdst.Write(dstbuf.m_buf, size1);
	}
}

void CYuv411F::YyyyuvSizer(char *src, char *dest, int srcw, int srch, int dstw, int dsth, int px, int py, int unit)
{
	this->yuv411sizerorder(src, dest, srcw, srch, dstw, dsth, px, py, YUV411_YYYYUV, unit);
}

void CYuv411F::YyyyvuSizer(char *src, char *dest, int srcw, int srch, int dstw, int dsth, int px, int py, int unit)
{
	this->yuv411sizerorder(src, dest, srcw, srch, dstw, dsth, px, py, YUV411_YYYYVU, unit);
}

void CYuv411F::YuyvyySizer(char *src, char *dest, int srcw, int srch, int dstw, int dsth, int px, int py, int unit)
{
	this->yuv411sizerorder(src, dest, srcw, srch, dstw, dsth, px, py, YUV411_YUYVYY, unit);
}

void CYuv411F::YvyuyySizer(char *src, char *dest, int srcw, int srch, int dstw, int dsth, int px, int py, int unit)
{
	this->yuv411sizerorder(src, dest, srcw, srch, dstw, dsth, px, py, YUV411_YVYUYY, unit);
}

void CYuv411F::UvyyyySizer(char *src, char *dest, int srcw, int srch, int dstw, int dsth, int px, int py, int unit)
{
	this->yuv411sizerorder(src, dest, srcw, srch, dstw, dsth, px, py, YUV411_UVYYYY, unit);
}

void CYuv411F::VuyyyySizer(char *src, char *dest, int srcw, int srch, int dstw, int dsth, int px, int py, int unit)
{
	this->yuv411sizerorder(src, dest, srcw, srch, dstw, dsth, px, py, YUV411_VUYYYY, unit);
}

//===============================================================
//
//		yuv411 image prefilter
//
//==============================================================
void CYuv411F::Yuv411Prefilterorder(char *src, char *dest, int width, int height, int order,
					int count, int *filter, int unit)
{
	CFileBin fsrc, fdst;
	CBufferBase srcbuf, dstbuf;
	int size;
//	int flag = unit;

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
		CYuv411::MYuv411_444order(srcbuf.m_buf, dstbuf.m_buf, width, height, order, unit);
		CYuv444::MYuv444Prefilter(dstbuf.m_buf, srcbuf.m_buf, width, height, count, filter, unit);
		size = CYuv411::MYuv444_411order(srcbuf.m_buf, dstbuf.m_buf, width, height, order, unit);
	}
	fdst.Open(dest, "wb");
	fdst.Write(dstbuf.m_buf, size);
}

void CYuv411F::Yuv411Prefilter(char *src, char *dest, int width, int height,
					int count, int *filter, int unit)
{
	CFileBin fsrc, fdst;
	CBufferBase srcbuf, dstbuf;
	int size;
//	int flag = unit;

	if( (unit != 1) && (unit != 2) )
		unit = 1;
	width >>= 2;
	size = width + ((count-1) << 1);
	size *= height + ((count-1) << 1);
	size *= unit * 6;
	srcbuf.SetSize(size);
	dstbuf.SetSize(size);
	if( (srcbuf.GetTotalSize() == 0) && (dstbuf.GetTotalSize() == 0) )
	{
		this->m_dbug.PrintMsg("Can not allocate so big buffer\n");
		return;
	}
	fsrc.Open(src);
	fsrc.Read(srcbuf.m_buf, size);
	
		size = CBaseConvert::Prefilter(srcbuf.m_buf, dstbuf.m_buf, width, height, 6, count, filter, unit);
	fdst.Open(dest, "wb");
	fdst.Write(dstbuf.m_buf, size);
}

void CYuv411F::YyyyuvPrefilter(char *src, char *dest, int width, int height, 
					int count, int *filter, int unit)
{
	this->Yuv411Prefilterorder(src, dest, width, height, YUV411_YYYYUV, 
				count, filter, unit);
}

void CYuv411F::YyyyvuPrefilter(char *src, char *dest, int width, int height, 
					int count, int *filter, int unit)
{
	this->Yuv411Prefilterorder(src, dest, width, height, YUV411_YYYYVU, 
				count, filter, unit);
}

void CYuv411F::YuyvyyPrefilter(char *src, char *dest, int width, int height, 
					int count, int *filter, int unit)
{
	this->Yuv411Prefilterorder(src, dest, width, height, YUV411_YUYVYY, 
				count, filter, unit);
}

void CYuv411F::YvyuyyPrefilter(char *src, char *dest, int width, int height, 
					int count, int *filter, int unit)
{
	this->Yuv411Prefilterorder(src, dest, width, height, YUV411_YVYUYY, 
				count, filter, unit);
}

void CYuv411F::UvyyyyPrefilter(char *src, char *dest, int width, int height, 
					int count, int *filter, int unit)
{
	this->Yuv411Prefilterorder(src, dest, width, height, YUV411_UVYYYY, 
				count, filter, unit);
}

void CYuv411F::VuyyyyPrefilter(char *src, char *dest, int width, int height, 
					int count, int *filter, int unit)
{
	this->Yuv411Prefilterorder(src, dest, width, height, YUV411_VUYYYY, 
				count, filter, unit);
}


//=======================================================
//
//		YUV411 encode
//
//=======================================================
void CYuv411F::Blk2Jpeg(char *src, char *dest, int width, int height, int qf)
{
	CFileBin fsrc, fdst;
	CBufferBase srcbuf, dstbuf;
	int size;

	size = CYuv411::getVwc(width, height);
	srcbuf.SetSize(size);
	dstbuf.SetSize(size);
	if( (srcbuf.GetTotalSize() == 0) && (dstbuf.GetTotalSize() == 0) )
	{
		this->m_dbug.PrintMsg("Can not allocate so big buffer\n");
		return;
	}
	fsrc.Open(src);
	fsrc.Read(srcbuf.m_buf, size);
	size = CYuv411::Blk2Jpeg(srcbuf.m_buf, dstbuf.m_buf, width, height, qf);
	fdst.Open(dest, "wb");
	fdst.Write(dstbuf.m_buf, size);
}

