
#include "internal.h"

void CYuv444F::Rgb2Yuv444(char *src, char *dest, int width, int height, int unit)
{
	int i, len, size, cap;
	CFileBin fsrc, fdst;
	CBufferBase srcbuf, dstbuf;
//	int flag = unit;

	if( (unit != 1) && (unit != 2) )
		unit = 1;
	fsrc.Open(src);
	fdst.Open(dest, "wb");
	cap = 256;
	if(unit == 2)
		cap = 1024;
	size = unit * 3;
	len = width * height * unit * 3;
	for(i = 0; i < len; i += size)
	{
		width = fsrc.Read(srcbuf.m_buf, size);
	
			width = CYuv444::Rgb2Yuv444(srcbuf.m_buf, dstbuf.m_buf, cap, unit);
		fdst.Write(dstbuf.m_buf, width);
	}
}

void CYuv444F::Yuv444_Rgb(char *src, char *dest, int width, int height, int unit)
{
	int i, len, size, cap;
	CFileBin fsrc, fdst;
	CBufferBase srcbuf, dstbuf;
//	int flag = unit;

	if( (unit != 1) && (unit != 2) )
		unit = 1;
	fsrc.Open(src);
	fdst.Open(dest, "wb");
	cap = 256;
	if(unit == 2)
		cap = 1024;
	size = unit * 3;
	len = width * height * unit * 3;
	for(i = 0; i < len; i += size)
	{
		fsrc.Read(srcbuf.m_buf, size);
		
			width = CYuv444::Yuv444_Rgb(srcbuf.m_buf, dstbuf.m_buf, cap, unit);
		fdst.Write(dstbuf.m_buf, width);
	}
}

void CYuv444F::Yuv444_Blk(char *src, char *dest, int width, int height, int unit)
{
	int len, size, msize;
	char* pbuf;
	CFileBin fsrc, fdst;
	CBufferBase srcbuf, dstbuf;
	CYuv2BlkBuf blkbuf(YUV444, width, unit);

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

void CYuv444F::Blk2Yuv444(char *src, char *dest, int width, int height, int unit)
{
	int len, size, msize;
	char* pbuf;
	CFileBin fsrc, fdst;
	CBufferBase srcbuf, dstbuf;
	CBlk2YuvBuf blkbuf(YUV444, width, unit);

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

void CYuv444F::Yuv444_Blk1(char *src, char *dest, int width, int height, int unit)
{
	int j, len, size;
	CFileBin fsrc, fdst;
	CBufferBase srcbuf, dstbuf;
//	int flag = unit;

	if( (unit != 1) && (unit != 2) )
		unit = 1;
	fsrc.Open(src);
	fdst.Open(dest, "wb");
	size = (width * unit * 3) << 3;
	srcbuf.SetSize(size);
	dstbuf.SetSize(size);
	for(j = 0; j < height; j += 8)
	{
		fsrc.Read(srcbuf.m_buf, size);
		
			len = CYuv444::Yuv444_Blk(srcbuf.m_buf, dstbuf.m_buf, width, 8, unit);
		fdst.Write(dstbuf.m_buf, len);
	}
}

void CYuv444F::Blk2Yuv4441(char *src, char *dest, int width, int height, int unit)
{
	int j, len, size;
	CFileBin fsrc, fdst;
	CBufferBase srcbuf, dstbuf;
//	int flag = unit;

	if( (unit != 1) && (unit != 2) )
		unit = 1;
	fsrc.Open(src);
	fdst.Open(dest, "wb");
	size = (width * unit * 3) << 3;
	srcbuf.SetSize(size);
	dstbuf.SetSize(size);
	for(j = 0; j < height; j += 8)
	{
		fsrc.Read(srcbuf.m_buf, size);
		
			len = CYuv444::Blk2Yuv444(srcbuf.m_buf, dstbuf.m_buf, width, 8, unit);
		fdst.Write(dstbuf.m_buf, len);
	}
}

void CYuv444F::Yuv444_Lbuf(char *src, char *dest, int width, int height, int unit)
{
	int j, len, size;
	CFileBin fsrc, fdst;
	CBufferBase srcbuf, dstbuf;
//	int flag = unit;

	if( (unit != 1) && (unit != 2) )
		unit = 1;
	fsrc.Open(src);
	fdst.Open(dest, "wb");
	size = (width * unit * 3) << 3;
	srcbuf.SetSize(size);
	dstbuf.SetSize(size);
	for(j = 0; j < height; j += 8)
	{
		fsrc.Read(srcbuf.m_buf, size);
		
			len = CYuv444::Yuv444_Lbuf(srcbuf.m_buf, dstbuf.m_buf, width, 8, unit);
		fdst.Write(dstbuf.m_buf, len);
	}
}

void CYuv444F::Lbuf2Yuv444(char *src, char *dest, int width, int height, int unit)
{
	int j, len, size;
	CFileBin fsrc, fdst;
	CBufferBase srcbuf, dstbuf;
//	int flag = unit;

	if( (unit != 1) && (unit != 2) )
		unit = 1;
	fsrc.Open(src);
	fdst.Open(dest, "wb");
	size = (width * unit * 3) << 3;
	srcbuf.SetSize(size);
	dstbuf.SetSize(size);
	for(j = 0; j < height; j += 8)
	{
		fsrc.Read(srcbuf.m_buf, size);
		
			len = CYuv444::Lbuf2Yuv444(srcbuf.m_buf, dstbuf.m_buf, width, 8, unit);
		fdst.Write(dstbuf.m_buf, len);
	}
}

void CYuv444F::Yuv444Sizer(char *src, char *dest, int srcw, int srch, int dstw, int dsth, int px, int py, int unit)
{
	CFileBin fsrc, fdst;
	CBufferBase srcbuf;
	int i, size, size1, offset;

	if( (unit != 1) && (unit != 2) )
		unit = 1;
	if( ((dstw + px) > srcw) || ((dsth + py) > srch) )
	{
		this->m_dbug.PrintMsg("Output Image exceed Input Image\n");
		return;
	}

	if(srcbuf.GetTotalSize() == 0)
	{
		this->m_dbug.PrintMsg("Can not allocate so big buffer\n");
		return;
	}
	fsrc.Open(src);
	fdst.Open(dest, "wb");
	size = srcw * unit * 3;
	srcbuf.SetSize(size);
	size1 = dstw * unit * 3;
	offset = px * unit * 3;
	for(i = 0; i < py; i++)
		fsrc.Read(srcbuf.m_buf, size);
	for(i = 0; i < dsth; i++)
	{
		fsrc.Read(srcbuf.m_buf, size);
		fdst.Write(srcbuf.m_buf + offset, size1);
	}
}

void CYuv444F::Yuv444Resize(char *src, char *dest, int srcw, int srch, int dstw, int dsth, int unit)
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
	
	size = srcw * srch * 3 * unit;

	fsrc.Read(srcbuf.m_buf, size);

	
		size = CYuv444::Yuv444Resize(srcbuf.m_buf, dstbuf.m_buf, srcw, srch, dstw, dsth, unit);

	fdst.Write(dstbuf.m_buf, size);
}

void CYuv444F::Yuv444Prefilter(char *src, char *dest, int width, int height, 
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
	
		size = CYuv444::MYuv444Prefilter(srcbuf.m_buf, dstbuf.m_buf, width, height, count, filter, unit);
	fdst.Open(dest, "wb");
	fdst.Write(dstbuf.m_buf, size);
}

//since unit of RGB24 is 1, it's a must to let unit = 1 while calling this function.
void CYuv444F::Rgb2Yuv444Blk(char *src, char *dest, int width, int height, int unit)
{
	int len, cap, msize;
	CFileBin fsrc, fdst;
	CBufferBase srcbuf, dstbuf;
	
	if( (unit != 1) && (unit != 2) ) 
		unit = 1;
	if( (width % 2) || (height % 2) )
	{
		this->m_dbug.PrintMsg("Image width & height must be even\n");
		return;
	}
	
	cap = 256;
	if(unit == 2)
		cap = 1024;
	
	fsrc.Open(src);
	fdst.Open(dest, "wb");
	msize = (height * width * unit) * 3;
	srcbuf.SetSize(msize);
	dstbuf.SetSize( (msize / 3) << 2); //this size is to be used for YuvBlk444 size (including head & tail), so is greater than "msize".
	
	len = fsrc.Read(srcbuf.m_buf, msize);
	len = CYuv444::Rgb2Yuv444Blk(srcbuf.m_buf, dstbuf.m_buf, width, height, cap, unit);

	fdst.Write(dstbuf.m_buf, len);
}

void CYuv444F::Blk2Rgb24(char *src, char *dest, int width, int height)
{
	int size, msize, ncount = 0;
	CBufferBase srcbuf, dstbuf;
	CFileBin fsrc, fdst;
	CBlk2YuvBuf blkbuf(YUV444, width, 1);

	if( (width % 2) || (height % 2) )
	{
		this->m_dbug.PrintMsg("Image width & height must be even\n");
		return;
	}
	fsrc.Open(src);
	fdst.Open(dest, "wb");

	msize = (height * width) * 3;
	srcbuf.SetSize(msize);
	dstbuf.SetSize(msize);
	size = fsrc.Read(srcbuf.m_buf, msize);

	ncount = CYuv444::Blk2Rgb24(srcbuf.m_buf, dstbuf.m_buf, width, height);
	fdst.Write(dstbuf.m_buf, ncount);	
}
	
//=======================================================
//
//		YUV444 encode
//
//=======================================================
void CYuv444F::Blk2Jpeg(char *src, char *dest, int width, int height, int qf)
{
	CFileBin fsrc, fdst;
	CBufferBase srcbuf, dstbuf;
	int size;

	size = CYuv444::getVwc(width, height);
	srcbuf.SetSize(size);
	dstbuf.SetSize(size);
	if( (srcbuf.GetTotalSize() == 0) && (dstbuf.GetTotalSize() == 0) )
	{
		this->m_dbug.PrintMsg("Can not allocate so big buffer\n");
		return;
	}
	fsrc.Open(src);
	fsrc.Read(srcbuf.m_buf, size);
	size = CYuv444::Blk2Jpeg(srcbuf.m_buf, dstbuf.m_buf, width, height, qf);
	fdst.Open(dest, "wb");
	fdst.Write(dstbuf.m_buf, size);
}


//since unit of RGB24 is 1, it is a must to let unit = 1 while calling this function.
void CYuv444F::Rgb2Jpeg444(char *src, char *dest, int width, int height, int unit, int qfactor)
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
	
	size = CYuv444::getVwc(width, height);
	srcbuf.SetSize(size, unit);
	dstbuf.SetSize(size, unit);
	if( (srcbuf.GetTotalSize() == 0) && (dstbuf.GetTotalSize() == 0) )
	{
		this->m_dbug.PrintMsg("Can not allocate so big buffer\n");
		return;
	}
	fsrc.Open(src);
	fsrc.Read(srcbuf.m_buf, size*unit);

	size = CYuv444::Rgb2Jpeg444(srcbuf.m_buf, dstbuf.m_buf, width, height, unit, qfactor);
	
	fdst.Open(dest, "wb");
	fdst.Write(dstbuf.m_buf, size);
}

//=======================================================
//
//		YUV444 rotation
//
//=======================================================
void CYuv444F::Yuv444Rotate(char *src, char *dest, int width, int height, int rotate, int unit)
{
	CFileBin fsrc, fdst;
	CBufferBase srcbuf, dstbuf;
	int size;

	size = CYuv444::getVwc(width, height);
	srcbuf.SetSize(size);
	dstbuf.SetSize(size);
	if( (srcbuf.GetTotalSize() == 0) && (dstbuf.GetTotalSize() == 0) )
	{
		this->m_dbug.PrintMsg("Can not allocate so big buffer\n");
		return;
	}
	fsrc.Open(src);
	fsrc.Read(srcbuf.m_buf, size);
	CYuv444::Yuv444Rotate(srcbuf.m_buf, dstbuf.m_buf, width, height, rotate, unit);
	fdst.Open(dest, "wb");
	fdst.Write(dstbuf.m_buf, size);
}

