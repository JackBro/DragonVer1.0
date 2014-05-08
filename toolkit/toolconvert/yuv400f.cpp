
#include "internal.h"

//=====================================================
//
//		rgb convert to yuv400 in memory
//
//=====================================================

void CYuv400F::Rgb2Yuv400(char *src, char *dest, int width, int height, int unit)
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
	srcbuf.SetSize(size);
	dstbuf.SetSize(size);
	len = width * height * unit * 3;
	for(i = 0; i < len; i += size)
	{
		width = fsrc.Read(srcbuf.m_buf, size);
		
			width = CYuv400::Rgb2Yuv400(srcbuf.m_buf, dstbuf.m_buf, cap, unit);
		fdst.Write(dstbuf.m_buf, width);
	}
}

void CYuv400F::Yuv400_Rgb(char *src, char *dest, int width, int height, int unit)
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
	size = unit;
	srcbuf.SetSize(size * 3);
	dstbuf.SetSize(size * 3);
	len = width * height * unit;
	for(i = 0; i < len; i += size)
	{
		fsrc.Read(srcbuf.m_buf, size);
		
			width = CYuv400::Yuv400_Rgb(srcbuf.m_buf, dstbuf.m_buf, cap, unit);
		fdst.Write(dstbuf.m_buf, width);
	}
}

void CYuv400F::Yuv444_400(char *src, char *dest, int width, int height, int unit)
{
	int i, len, size;
	CFileBin fsrc, fdst;
	CBufferBase srcbuf, dstbuf;
//	int flag = unit;

	if( (unit != 1) && (unit != 2) )
		unit = 1;
	fsrc.Open(src);
	fdst.Open(dest, "wb");
	size = unit * 3;
	len = width * height * unit * 3;
	srcbuf.SetSize(size);
	dstbuf.SetSize(size);
	for(i = 0; i < len; i += size)
	{
		width = fsrc.Read(srcbuf.m_buf, size);
		
			width = CYuv400::Yuv444_400(srcbuf.m_buf, dstbuf.m_buf, unit);
		fdst.Write(dstbuf.m_buf, width);
	}
}

void CYuv400F::Yuv400_444(char *src, char *dest, int width, int height, int unit)
{
	int i, len, size;
	CFileBin fsrc, fdst;
	CBufferBase srcbuf, dstbuf;
//	int flag = unit;

	if( (unit != 1) && (unit != 2) )
		unit = 1;
	fsrc.Open(src);
	fdst.Open(dest, "wb");
	size = unit;
	len = width * height * unit;
	srcbuf.SetSize(size * 3);
	dstbuf.SetSize(size * 3);
	for(i = 0; i < len; i += size)
	{
		fsrc.Read(srcbuf.m_buf, size);
		
			width = CYuv400::Yuv400_444(srcbuf.m_buf, dstbuf.m_buf, unit);
		fdst.Write(dstbuf.m_buf, width);
	}
}

void CYuv400F::Yuv2Blk(char *src, char *dest, int width, int height, int unit)
{
	int len, size, msize;
	char* pbuf;
	CFileBin fsrc, fdst;
	CBufferBase srcbuf, dstbuf;
	CYuv2BlkBuf blkbuf(YUV400, width, unit);

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

void CYuv400F::Blk2Yuv(char *src, char *dest, int width, int height, int unit)
{
	int len, size, msize;
	char* pbuf;
	CFileBin fsrc, fdst;
	CBufferBase srcbuf, dstbuf;
	CBlk2YuvBuf blkbuf(YUV400, width, unit);

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

void CYuv400F::Yuv2Blk1(char *src, char *dest, int width, int height, int unit)
{
	int j, len, size;
	CFileBin fsrc, fdst;
	CBufferBase srcbuf, dstbuf;
//	int flag = unit;

	if( (unit != 1) && (unit != 2) )
		unit = 1;
	fsrc.Open(src);
	fdst.Open(dest, "wb");
	size = (width * unit) << 3;
	srcbuf.SetSize(size);
	dstbuf.SetSize(size);
	for(j = 0; j < height; j += 8)
	{
		fsrc.Read(srcbuf.m_buf, size);
		
			len = CYuv400::Yuv2Blk(srcbuf.m_buf, dstbuf.m_buf, width, 8, unit);
		fdst.Write(dstbuf.m_buf, len);
	}
}

void CYuv400F::Blk2Yuv1(char *src, char *dest, int width, int height, int unit)
{
	int j, len, size;
	CFileBin fsrc, fdst;
	CBufferBase srcbuf, dstbuf;
//	int flag = unit;

	if( (unit != 1) && (unit != 2) )
		unit = 1;
	fsrc.Open(src);
	fdst.Open(dest, "wb");
	size = (width * unit) << 3;
	srcbuf.SetSize(size);
	dstbuf.SetSize(size);
	for(j = 0; j < height; j += 8)
	{
		fsrc.Read(srcbuf.m_buf, size);
		
			len = CYuv400::Blk2Yuv(srcbuf.m_buf, dstbuf.m_buf, width, 8, unit);
		fdst.Write(dstbuf.m_buf, len);
	}
}

void CYuv400F::Yuv2Lbuf(char *src, char *dest, int width, int height, int unit)
{
	int j, len, size;
	CFileBin fsrc, fdst;
	CBufferBase srcbuf, dstbuf;
//	int flag = unit;

	if( (unit != 1) && (unit != 2) )
		unit = 1;
	fsrc.Open(src);
	fdst.Open(dest, "wb");
	size = (width * unit) << 3;
	srcbuf.SetSize(size);
	dstbuf.SetSize(size);
	for(j = 0; j < height; j += 8)
	{
		fsrc.Read(srcbuf.m_buf, size);
		
			len = CYuv400::Yuv2Lbuf(srcbuf.m_buf, dstbuf.m_buf, width, 8, unit);
		fdst.Write(dstbuf.m_buf, len);
	}
}

void CYuv400F::Lbuf2Yuv(char *src, char *dest, int width, int height, int unit)
{
	int j, len, size;
	CFileBin fsrc, fdst;
	CBufferBase srcbuf, dstbuf;
//	int flag = unit;

	if( (unit != 1) && (unit != 2) )
		unit = 1;
	fsrc.Open(src);
	fdst.Open(dest, "wb");
	size = (width * unit) << 3;
	srcbuf.SetSize(size);
	dstbuf.SetSize(size);
	for(j = 0; j < height; j += 8)
	{
		fsrc.Read(srcbuf.m_buf, size);
		
			len = CYuv400::Lbuf2Yuv(srcbuf.m_buf, dstbuf.m_buf, width, 8, unit);
		fdst.Write(dstbuf.m_buf, len);
	}
}

//since unit of RGB24 is 1, it is a must to let unit = 1 while calling this function.
void CYuv400F::Rgb2Yuv400Blk(char *src, char *dest, int width, int height, int unit)
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
		
	fsrc.Open(src);
	fdst.Open(dest, "wb");
	cap = 256;
	if(unit == 2)
		cap = 1024;

	msize = width * height * unit;
	dstbuf.SetSize(msize, unit);
	srcbuf.SetSize(msize*3, unit);
	
	len = fsrc.Read(srcbuf.m_buf, msize*3);
	len = CYuv400::Rgb2Yuv400Blk(srcbuf.m_buf, dstbuf.m_buf, width, height, cap, unit);
	
	fdst.Write(dstbuf.m_buf, len);
}

void CYuv400F::Blk2Rgb24(char *src, char *dest, int width, int height)
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
	srcbuf.SetSize(height*width);
	dstbuf.SetSize(msize);
	size = fsrc.Read(srcbuf.m_buf, msize);
	if (0 == size)
		return;
	
	ncount = CYuv400::Blk2Rgb24(srcbuf.m_buf, dstbuf.m_buf, width, height);
	fdst.Write(dstbuf.m_buf, ncount);	
}

void CYuv400F::Yuv400Resize(char *src, char *dest, int srcw, int srch, int dstw, int dsth, int unit)
{
	CFileBin fsrc, fdst;
	CBufferBase srcbuf, dstbuf;
	int size, w, h;
//	int flag = unit;

	if( (unit != 1) && (unit != 2) )
		unit = 1;
	w = (srcw > dstw) ? srcw : dstw;
	h = (srch > dsth) ? srch : dsth;
	size = w * h;
	srcbuf.SetSize(size, unit);
	dstbuf.SetSize(size, unit);
	if( (srcbuf.GetTotalSize() == 0) || (dstbuf.GetTotalSize() == 0) )
	{
		this->m_dbug.PrintMsg("Can not allocate so big buffer\n");
		return;
	}
	fsrc.Open(src);
	fdst.Open(dest, "wb");
	
	size = srcw * srch * unit;

	fsrc.Read(srcbuf.m_buf, size);

	
		size = CYuv400::Yuv400Resize(srcbuf.m_buf, dstbuf.m_buf, srcw, srch, dstw, dsth, unit);

	fdst.Write(dstbuf.m_buf, size);
}

void CYuv400F::Yuv400Sizer(char *src, char *dest, int srcw, int srch, int dstw, int dsth, int px, int py, int unit)
{
	CFileBin fsrc, fdst;
	CBufferBase srcbuf, dstbuf;
	int i, size, size1, offset;

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
	fsrc.Open(src);
	fdst.Open(dest, "wb");
	size = srcw * unit;
	size1 = dstw * unit;
	offset = px * unit;
	srcbuf.SetSize(size);
	for(i = 0; i < py; i++)
		fsrc.Read(srcbuf.m_buf, size);
	for(i = 0; i < dsth; i++)
	{
		fsrc.Read(srcbuf.m_buf, size);
		fdst.Write(srcbuf.m_buf + offset, size1);
	}
}

void CYuv400F::Yuv400Prefilter(char *src, char *dest, int width, int height, 
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
	size *= unit;
	srcbuf.SetSize(size);
	dstbuf.SetSize(size);
	if( (srcbuf.GetTotalSize() == 0) && (dstbuf.GetTotalSize() == 0) )
	{
		this->m_dbug.PrintMsg("Can not allocate so big buffer\n");
		return;
	}
	fsrc.Open(src);
	fsrc.Read(srcbuf.m_buf, size);
	
		size = CYuv400::MYuv400Prefilter(srcbuf.m_buf, dstbuf.m_buf, width, height, count, filter, unit);
	fdst.Open(dest, "wb");
	fdst.Write(dstbuf.m_buf, size);
}

//=======================================================
//
//		YUV400 encode
//
//=======================================================
void CYuv400F::Blk2Jpeg(char *src, char *dest, int width, int height, int qf)
{
	CFileBin fsrc, fdst;
	CBufferBase srcbuf, dstbuf;
	int size;

	size = CYuv400::getVwc(width, height);
	srcbuf.SetSize(size);
	dstbuf.SetSize(size);
	if( (srcbuf.GetTotalSize() == 0) && (dstbuf.GetTotalSize() == 0) )
	{
		this->m_dbug.PrintMsg("Can not allocate so big buffer\n");
		return;
	}
	fsrc.Open(src);
	fsrc.Read(srcbuf.m_buf, size);
	size = CYuv400::Blk2Jpeg(srcbuf.m_buf, dstbuf.m_buf, width, height, qf);
	fdst.Open(dest, "wb");
	fdst.Write(dstbuf.m_buf, size);
}

//since unit of RGB24 is 1, it is a must to let unit = 1 while calling this function.
void CYuv400F::Rgb2Jpeg400(char *src, char *dest, int width, int height, int unit, int qf)
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

	size = CYuv400::getVwc(width, height);
	srcbuf.SetSize(size*3, unit);
	dstbuf.SetSize(size, unit);
	if( (srcbuf.GetTotalSize() == 0) && (dstbuf.GetTotalSize() == 0) )
	{
		this->m_dbug.PrintMsg("Can not allocate so big buffer\n");
		return;
	}
	fsrc.Open(src);
	
	fsrc.Read(srcbuf.m_buf, size*3*unit);
	size = CYuv400::Rgb2Jpeg400(srcbuf.m_buf, dstbuf.m_buf, width, height, unit, qf);
	
	fdst.Open(dest, "wb");
	fdst.Write(dstbuf.m_buf, size);
}

