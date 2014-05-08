
#include "internal.h"

//===================================
//
//					protected function
//
//====================================================

void CRaw2RgbF::bitrgb2bmp(CFileBin *fsrc, CFileBin *fdst, int width, int height, int bitwidth)
{
	CBitFifo bits;
	CBufferBase pool;
	int dstlen, bw = bitwidth;

	pool.SetSize(width * 3);
	if( (bits.GetSize() == 0) || (pool.GetSize() == 0) )
	{
		this->m_dbug.PrintMsg("can't alloc pool\n");
		return;
	}
	if(bitwidth % 8)
	{
		bitwidth *= width;
		dstlen = ((bitwidth + 7) >> 3) << 3;
		width = ((((bitwidth + 7) >> 3) + 3) >> 2) << 2;
		while(height--)
		{
			fsrc->Read(pool.m_buf, dstlen >> 3);
			bits.Write(pool.m_buf, dstlen);
			bits.Read(pool.m_buf, bitwidth);
			if(bw < 8)
				bits.ReverseBitStream(pool.m_buf, pool.m_buf, width, bw);
			fdst->Write(pool.m_buf, width);
		}
		return;
	}
	width *= bitwidth;
	width >>= 3;
	bitwidth = ((width + 3) >> 2) << 2;
	while(height--)
	{
		fsrc->Read(pool.m_buf, width);
		fdst->Write(pool.m_buf, bitwidth);
	}
}

void CRaw2RgbF::bitbmp2rgb(CFileBin *fsrc, CFileBin *fdst, int width, int height, int bitwidth)
{
	CBitFifo bits;
	CBufferBase pool;
	int offset = 0, dstlen, bw = bitwidth;

	pool.SetSize(width * 3);
	if( (bits.GetSize() == 0) || (pool.GetSize() == 0) )
	{
		this->m_dbug.PrintMsg("can't alloc pool\n");
		return;
	}
	if(bitwidth % 8)
	{
		bitwidth *= width;
		width = ((((bitwidth + 7) >> 3) + 3) >> 2) << 2;
		while(height--)
		{
			fsrc->Read(pool.m_buf, width);
			if(bw < 8)
				bits.ReverseBitStream(pool.m_buf, pool.m_buf, width, bw);
			bits.Write(pool.m_buf, bitwidth);
			dstlen = ((bitwidth + offset) >> 3) << 3;
			bits.Read(pool.m_buf, dstlen);
			fdst->Write(pool.m_buf, dstlen >> 3);
			offset = bitwidth + offset - dstlen;
		}
		if(offset)
		{
			bits.Read(pool.m_buf, offset);
			fdst->Write(pool.m_buf, (offset + 7) >> 3);
		}
		return;
	}
	width *= bitwidth;
	width >>= 3;
	bitwidth = ((width + 3) >> 2) << 2;
	while(height--)
	{
		fsrc->Read(pool.m_buf, bitwidth);
		fdst->Write(pool.m_buf, width);
	}
}


//================== data convert RGB part =================
//
//					RAW data part
//
//====================================================
void CRaw2RgbF::rgb2bayerorder(char *src, char *dst, int width, int height, int order, int unit)
{
	int i, len, size;
	CFileBin fsrc, fdst;
	CBufferBase srcbuf, dstbuf;

	fsrc.Open(src);
	fdst.Open(dst, "wb");
	size = width * unit * 3 * 2;
	srcbuf.SetSize(size);
	dstbuf.SetSize(size);
	for(i = 0; i < height; i += 2)
	{
		len = fsrc.Read(srcbuf.m_buf, size);
		len = CRaw2Rgb::rgb2bayerorder(srcbuf.m_buf, dstbuf.m_buf, order, width, 2, unit);
		fdst.Write(dstbuf.m_buf, len);
	}
}

void CRaw2RgbF::Rgb2GB(char *src, char *dest, int width, int height, int unit)
{
	this->rgb2bayerorder(src, dest, width, height, RAW_GB, unit);
}

void CRaw2RgbF::Rgb2GR(char *src, char *dest, int width, int height, int unit)
{
	this->rgb2bayerorder(src, dest, width, height, RAW_GR, unit);
}

void CRaw2RgbF::Rgb2BG(char *src, char *dest, int width, int height, int unit)
{
	this->rgb2bayerorder(src, dest, width, height, RAW_BG, unit);
}

void CRaw2RgbF::Rgb2RG(char *src, char *dest, int width, int height, int unit)
{
	this->rgb2bayerorder(src, dest, width, height, RAW_RG, unit);
}

//=========================================
//
//		add bmp header
//
//=========================================
void CRaw2RgbF::Rgb1_Rgb24(char *src, char *dst, int width, int height)
{
	CFileBin fsrc, fdst;
	int length, i, len, val = 0;
	char pal[8], dat[3];

	if( (width == 0) || (height == 0) )
	{
		this->m_dbug.PrintMsg("error: src unit = 0 or dst unit = 0\n");
		return;
	}

	fsrc.Open(src);
	fdst.Open(dst, "wb");
	length = width * height;
	fsrc.Read(pal, 8);
	while(length)
	{
		len = (length > 8) ? 8 : length;
		fsrc.Read((char *)&val, 1);
		for(i = 0; i < len; i++, val >>= 1)
		{
			CRaw2Rgb::Rgb1_Rgb24(val & 1, dat, pal);
			fdst.Write(dat, 3);
		}
		length -= len;
	}
}

void CRaw2RgbF::Rgb2_Rgb24(char *src, char *dst, int width, int height)
{
	CFileBin fsrc, fdst;
	int length, i, len, val;
	char pal[16], dat[3];

	if( (width == 0) || (height == 0) )
	{
		this->m_dbug.PrintMsg("error: src unit = 0 or dst unit = 0\n");
		return;
	}

	fsrc.Open(src);
	fdst.Open(dst, "wb");
	length = width * height;
	fsrc.Read(pal, 16);
	while(length)
	{
		len = (length > 4) ? 4 : length;
		fsrc.Read((char *)&val, 1);
		for(i = 0; i < len; i++, val >>= 2)
		{
			CRaw2Rgb::Rgb2_Rgb24(val & 3, dat, pal);
			fdst.Write(dat, 3);
		}
		length -= len;
	}
}

void CRaw2RgbF::Rgb4_Rgb24(char *src, char *dst, int width, int height)
{
	CFileBin fsrc, fdst;
	int length, i, len, val;
	char pal[0x40], dat[3];

	if( (width == 0) || (height == 0) )
	{
		this->m_dbug.PrintMsg("error: src unit = 0 or dst unit = 0\n");
		return;
	}

	fsrc.Open(src);
	fdst.Open(dst, "wb");
	length = width * height;
	fsrc.Read(pal, 0x40);
	while(length)
	{
		len = (length > 2) ? 2 : length;
		fsrc.Read((char *)&val, 1);
		for(i = 0; i < len; i++, val >>= 4)
		{
			CRaw2Rgb::Rgb4_Rgb24(val & 0xf, dat, pal);
			fdst.Write(dat, 3);
		}
		length -= len;
	}
}

void CRaw2RgbF::Rgb8_Rgb24(char *src, char *dst, int width, int height)
{
	CFileBin fsrc, fdst;
	int length, val;
	char pal[0x400], dat[3];

	if( (width == 0) || (height == 0) )
	{
		this->m_dbug.PrintMsg("error: src unit = 0 or dst unit = 0\n");
		return;
	}

	fsrc.Open(src);
	fdst.Open(dst, "wb");
	length = width * height;
	fsrc.Read(pal, 0x400);
	while(length)
	{
		fsrc.Read((char *)&val, 1);
		CRaw2Rgb::Rgb8_Rgb24(val, dat, pal);
		fdst.Write(dat, 3);
		length--;
	}
}

void CRaw2RgbF::Rgb8_Rgb2(char *src, char *dst, int width, int height)
{
	CFileBin fsrc, fdst;
	int length, i, val;
	char pal[0x400];

	if( (width == 0) || (height == 0) )
	{
		this->m_dbug.PrintMsg("error: src unit = 0 or dst unit = 0\n");
		return;
	}

	fsrc.Open(src);
	fdst.Open(dst, "wb");
	length = width * height;
	fsrc.Read(pal, 0x400);
	for(i = 0; i < 0x400; i += 0x100)
		fdst.Write(pal + i, 4);
	while(length)
	{
		fsrc.Read(pal, 4);
		for(i = 0; i < 4; i++)
			CRaw2Rgb::Rgb8_Rgb2(pal[i] >> 6, (char *)&val, i << 1);
		fdst.Write((char *)&val, 1);
		length -= 4;
	}
}

void CRaw2RgbF::Rgb444_Rgb24(char *src, char *dst, int width, int height)
{
	CFileBin fsrc, fdst;
	CBitTools bits;
	int length, val, i, len;
	char srcbuf[3], dat[3];

	if( (width == 0) || (height == 0) )
	{
		this->m_dbug.PrintMsg("error: src unit = 0 or dst unit = 0\n");
		return;
	}

	fsrc.Open(src);
	fdst.Open(dst, "wb");
	length = width * height * 12;
	while(length)
	{
		len = (length > 24) ? 24 : length;
		fsrc.Read(srcbuf, 3);
		for(i = 0; i < len; i += 12)
		{
			bits.CopyBitStream(srcbuf, (char *)&val, 12, 0, i);
			CRaw2Rgb::Rgb444_Rgb24(val, dat);
			fdst.Write(dat, 3);
		}
		length -= len;
	}
}

void CRaw2RgbF::Rgb666_Rgb24(char *src, char *dst, int width, int height)
{
	CFileBin fsrc, fdst;
	CBitTools bits;
	int length, val, i, len;
	char srcbuf[9], dat[3];

	if( (width == 0) || (height == 0) )
	{
		this->m_dbug.PrintMsg("error: src unit = 0 or dst unit = 0\n");
		return;
	}

	fsrc.Open(src);
	fdst.Open(dst, "wb");
	length = width * height * 18;
	while(length)
	{
		len = (length > 72) ? 72 : length;
		fsrc.Read(srcbuf, 9);
		for(i = 0; i < len; i += 18)
		{
			bits.CopyBitStream(srcbuf, (char *)&val, 18, 0, i);
			CRaw2Rgb::Rgb666_Rgb24(val, dat);
			fdst.Write(dat, 3);
		}
		length -= len;
	}
}

void CRaw2RgbF::Rgb555_Rgb24(char *src, char *dst, int width, int height)
{
	CFileBin fsrc, fdst;
	int length;
	char srcbuf[2], dat[3];

	if( (width == 0) || (height == 0) )
	{
		this->m_dbug.PrintMsg("error: src unit = 0 or dst unit = 0\n");
		return;
	}

	fsrc.Open(src);
	fdst.Open(dst, "wb");
	length = width * height;
	while(length)
	{
		fsrc.Read(srcbuf, 2);
		CRaw2Rgb::Rgb555_Rgb24(srcbuf, dat);
		fdst.Write(dat, 3);
		length --;
	}
}

void CRaw2RgbF::Rgb565_Rgb24(char *src, char *dst, int width, int height)
{
	CFileBin fsrc, fdst;
	int length;
	char srcbuf[2], dat[3];

	if( (width == 0) || (height == 0) )
	{
		this->m_dbug.PrintMsg("error: src unit = 0 or dst unit = 0\n");
		return;
	}

	fsrc.Open(src);
	fdst.Open(dst, "wb");
	length = width * height;
	while(length)
	{
		fsrc.Read(srcbuf, 2);
		CRaw2Rgb::Rgb565_Rgb24(srcbuf, dat);
		fdst.Write(dat, 3);
		length --;
	}
}

void CRaw2RgbF::Rgb32_Rgb24(char *src, char *dst, int width, int height)
{
	CFileBin fsrc, fdst;
	int length;
	char srcbuf[4], dat[3];

	if( (width == 0) || (height == 0) )
	{
		this->m_dbug.PrintMsg("error: src unit = 0 or dst unit = 0\n");
		return;
	}

	fsrc.Open(src);
	fdst.Open(dst, "wb");
	length = width * height;
	while(length)
	{
		fsrc.Read(srcbuf, 4);
		CRaw2Rgb::Rgb32_Rgb24(srcbuf, dat);
		fdst.Write(dat, 3);
		length --;
	}
}

void CRaw2RgbF::Rgb24_Rgb444(char *src, char *dst, int width, int height, int dither)
{
	CFileBin fsrc, fdst;
	int length, size, len, offset = 0;
	CBufferBase srcbuf, dstbuf;

	if( (width == 0) || (height == 0) )
	{
		this->m_dbug.PrintMsg("error: src unit = 0 or dst unit = 0\n");
		return;
	}
	if( (srcbuf.GetSize() == 0) || (dstbuf.GetSize() == 0) )
	{
		this->m_dbug.PrintMsg("can't alloc pool\n");
		return;
	}

	fsrc.Open(src);
	fdst.Open(dst, "wb");
	length = width * height * 3;
	size = (width * 3) << 2;
	while(length)
	{
		size = length > size ? size : length;
		fsrc.Read(srcbuf.m_buf, size);
		len = CRaw2Rgb::Rgb24_Rgb444(srcbuf.m_buf, dstbuf.m_buf, width, size / (width * 3), offset, dither);
		offset += len;
		fdst.Write(dstbuf.m_buf, offset >> 3);
		dstbuf.m_buf[0] = dstbuf.m_buf[offset >> 3];
		offset %= 8;
		length -= size;
	}
	if(offset)
		fdst.Write(dstbuf.m_buf, 1);
}

void CRaw2RgbF::Rgb24_Rgb666(char *src, char *dst, int width, int height, int dither)
{
	CFileBin fsrc, fdst;
	int length, size, len, offset = 0;
	CBufferBase srcbuf, dstbuf;

	if( (width == 0) || (height == 0) )
	{
		this->m_dbug.PrintMsg("error: src unit = 0 or dst unit = 0\n");
		return;
	}
	if( (srcbuf.GetSize() == 0) || (dstbuf.GetSize() == 0) )
	{
		this->m_dbug.PrintMsg("can't alloc pool\n");
		return;
	}

	fsrc.Open(src);
	fdst.Open(dst, "wb");
	length = width * height * 3;
	size = (width * 3) << 2;
	while(length)
	{
		size = length > size ? size : length;
		fsrc.Read(srcbuf.m_buf, size);
		len = CRaw2Rgb::Rgb24_Rgb666(srcbuf.m_buf, dstbuf.m_buf, width, size / (width * 3), offset, dither);
		offset += len;
		fdst.Write(dstbuf.m_buf, offset >> 3);
		dstbuf.m_buf[0] = dstbuf.m_buf[offset >> 3];
		offset %= 8;
		length -= size;
	}
	if(offset)
		fdst.Write(dstbuf.m_buf, 1);
}

void CRaw2RgbF::Rgb24_Rgb555(char *src, char *dst, int width, int height, int dither)
{
	CFileBin fsrc, fdst;
	int length, size, len;
	CBufferBase srcbuf, dstbuf;

	if( (width == 0) || (height == 0) )
	{
		this->m_dbug.PrintMsg("error: src unit = 0 or dst unit = 0\n");
		return;
	}
	if( (srcbuf.GetSize() == 0) || (dstbuf.GetSize() == 0) )
	{
		this->m_dbug.PrintMsg("can't alloc pool\n");
		return;
	}

	fsrc.Open(src);
	fdst.Open(dst, "wb");
	length = width * height * 3;
	size = (width * 3) << 2;
	while(length)
	{
		size = length > size ? size : length;
		fsrc.Read(srcbuf.m_buf, size);
		len = CRaw2Rgb::Rgb24_Rgb555(srcbuf.m_buf, dstbuf.m_buf, width, size / (width * 3), dither);
		fdst.Write(dstbuf.m_buf, len);
		length -= size;
	}
}

void CRaw2RgbF::Rgb24_Rgb565(char *src, char *dst, int width, int height, int dither)
{
	CFileBin fsrc, fdst;
	int length, size, len;
	CBufferBase srcbuf, dstbuf;

	if( (width == 0) || (height == 0) )
	{
		this->m_dbug.PrintMsg("error: src unit = 0 or dst unit = 0\n");
		return;
	}
	if( (srcbuf.GetSize() == 0) || (dstbuf.GetSize() == 0) )
	{
		this->m_dbug.PrintMsg("can't alloc pool\n");
		return;
	}

	fsrc.Open(src);
	fdst.Open(dst, "wb");
	length = width * height * 3;
	size = (width * 3) << 2;
	while(length)
	{
		size = length > size ? size : length;
		fsrc.Read(srcbuf.m_buf, size);
		len = CRaw2Rgb::Rgb24_Rgb565(srcbuf.m_buf, dstbuf.m_buf, width, size / (width * 3), dither);
		fdst.Write(dstbuf.m_buf, len);
		length -= size;
	}
}

void CRaw2RgbF::Rgb32_Rgb555(char *src, char *dst, int width, int height, int dither)
{
	CFileBin fsrc, fdst;
	int length, size, len;
	CBufferBase srcbuf, dstbuf;

	if( (width == 0) || (height == 0) )
	{
		this->m_dbug.PrintMsg("error: src unit = 0 or dst unit = 0\n");
		return;
	}
	if( (srcbuf.GetSize() == 0) || (dstbuf.GetSize() == 0) )
	{
		this->m_dbug.PrintMsg("can't alloc pool\n");
		return;
	}

	fsrc.Open(src);
	fdst.Open(dst, "wb");
	length = (width * height) << 2;
	size = width << 4;
	while(length)
	{
		size = length > size ? size : length;
		fsrc.Read(srcbuf.m_buf, size);
		len = CRaw2Rgb::Rgb32_Rgb555(srcbuf.m_buf, dstbuf.m_buf, width, size / (width * 3), dither);
		fdst.Write(dstbuf.m_buf, len);
		length -= size;
	}
}

void CRaw2RgbF::Rgb24_Rgb32(char *src, char *dst, int width, int height)
{
	CFileBin fsrc, fdst;
	int length;
	char srcbuf[3], dstbuf[4];

	if( (width == 0) || (height == 0) )
	{
		this->m_dbug.PrintMsg("error: src unit = 0 or dst unit = 0\n");
		return;
	}
	fsrc.Open(src);
	fdst.Open(dst, "wb");
	length = width * height;
	while(length--)
	{
		fsrc.Read(srcbuf, 3);
		CRaw2Rgb::Rgb24_Rgb32(srcbuf, dstbuf);
		fdst.Write(dstbuf, 4);
	}
}

void CRaw2RgbF::Rgb555_Rgb32(char *src, char *dst, int width, int height)
{
	CFileBin fsrc, fdst;
	int length;
	char srcbuf[2], dstbuf[4];

	if( (width == 0) || (height == 0) )
	{
		this->m_dbug.PrintMsg("error: src unit = 0 or dst unit = 0\n");
		return;
	}
	fsrc.Open(src);
	fdst.Open(dst, "wb");
	length = width * height;
	while(length--)
	{
		fsrc.Read(srcbuf, 2);
		CRaw2Rgb::Rgb555_Rgb32(srcbuf, dstbuf);
		fdst.Write(dstbuf, 4);
	}
}

//=========================================
//
//		add bmp header
//
//=========================================
int	CRaw2RgbF::addbmp1header(char *dstbuf, int width, int height)
{
	BITMAPFILEHEADER bmp = { 0x4d42, 0x3fe, 0x0, 0x0, 0x3e };
	BITMAPINFOHEADER info = { 0x28, 0x50, 0x50, 0x1, 0x1, 0x0, 0x3c0, 0x0, 0x0, 0x0, 0x0 };

	info.biWidth = width;
	info.biHeight = height;
	width >>= 3;
	width = ((width + 3) >> 2) << 2;
	info.biSizeImage = width * height;
	bmp.bfSize = info.biSizeImage + 0x36 + 8;

	memcpy(dstbuf, &bmp, sizeof(BITMAPFILEHEADER));
	memcpy(dstbuf+sizeof(BITMAPFILEHEADER), &info, sizeof(BITMAPINFOHEADER));
	return sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER);
}

int	CRaw2RgbF::addbmp2header(char *dstbuf, int width, int height)
{
	BITMAPFILEHEADER bmp = { 0x4d42, 0x3fe, 0x0, 0x0, 0x46 };
	BITMAPINFOHEADER info = { 0x28, 0x50, 0x50, 0x1, 0x2, 0x0, 0x3c0, 0x0, 0x0, 0x0, 0x0 };

	info.biWidth = width;
	info.biHeight = height;
	width >>= 2;
	width = ((width + 3) >> 2) << 2;
	info.biSizeImage = width * height;
	bmp.bfSize = info.biSizeImage + 0x36 + 0x10;

	memcpy(dstbuf, &bmp, sizeof(BITMAPFILEHEADER));
	memcpy(dstbuf+sizeof(BITMAPFILEHEADER), &info, sizeof(BITMAPINFOHEADER));
	return sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER);
}

int	CRaw2RgbF::addbmp4header(char *dstbuf, int width, int height)
{
	BITMAPFILEHEADER bmp = { 0x4d42, 0xcf6, 0x0, 0x0, 0x76 };
	BITMAPINFOHEADER info = { 0x28, 0x50, 0x50, 0x1, 0x4, 0x0, 0xc80, 0x0, 0x0, 0x0, 0x0 };

	info.biWidth = width;
	info.biHeight = height;
	width >>= 1;
	width = ((width + 3) >> 2) << 2;
	info.biSizeImage = width * height;
	bmp.bfSize = info.biSizeImage + 0x36 + 0x40;

	memcpy(dstbuf, &bmp, sizeof(BITMAPFILEHEADER));
	memcpy(dstbuf+sizeof(BITMAPFILEHEADER), &info, sizeof(BITMAPINFOHEADER));
	return sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER);
}

int	CRaw2RgbF::addbmp8header(char *dstbuf, int width, int height)
{
	BITMAPFILEHEADER bmp = { 0x4d42, 0x1d36, 0x0, 0x0, 0x436 };
	BITMAPINFOHEADER info = { 0x28, 0x50, 0x50, 0x1, 0x8, 0x0, 0x1900, 0x0, 0x0, 0x0, 0x0 };

	info.biWidth = width;
	info.biHeight = height;
	width = ((width + 3) >> 2) << 2;
	info.biSizeImage = width * height;
	bmp.bfSize = info.biSizeImage + 0x36 + 0x400;

	memcpy(dstbuf, &bmp, sizeof(BITMAPFILEHEADER));
	memcpy(dstbuf+sizeof(BITMAPFILEHEADER), &info, sizeof(BITMAPINFOHEADER));
	return sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER);
}

int CRaw2RgbF::addbmp444header(char *dstbuf, int width, int height)
{
	BITMAPFILEHEADER bmp = { 0x4d42, 0x38436, 0, 0, 0x36	};
	BITMAPINFOHEADER info = { 0x28, 0x140, 0xf0, 0x1, 0xc, 0, 0x38400, 0, 0, 0, 0 };

	info.biWidth = width;
	info.biHeight = height;
	width = width * 3 + 1;
	width >>= 1;
	width = ((width + 3) >> 2) << 2;
	info.biSizeImage = width * height;
	bmp.bfSize = info.biSizeImage + 0x36;

	memcpy(dstbuf, &bmp, sizeof(BITMAPFILEHEADER));
	memcpy(dstbuf+sizeof(BITMAPFILEHEADER), &info, sizeof(BITMAPINFOHEADER));
	return sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER);
}

int	CRaw2RgbF::addbmp555header(char *dstbuf, int width, int height)
{
	BITMAPFILEHEADER bmp = { 0x4d42, 0x38436, 0, 0, 0x36	};
	BITMAPINFOHEADER info = { 0x28, 0x140, 0xf0, 0x1, 0x10, 0, 0x38400, 0, 0, 0, 0 };

	info.biWidth = width;
	info.biHeight = height;
	width <<= 1;
	width = ((width + 3) >> 2) << 2;
	info.biSizeImage = width * height;
	info.biCompression = BI_RGB;
	bmp.bfSize = info.biSizeImage + 0x36;// + 0xc;

	memcpy(dstbuf, &bmp, sizeof(BITMAPFILEHEADER));
	memcpy(dstbuf+sizeof(BITMAPFILEHEADER), &info, sizeof(BITMAPINFOHEADER));
	return sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER);
}

int	CRaw2RgbF::addbmp565header(char *dstbuf, int width, int height)
{
	BITMAPFILEHEADER bmp = { 0x4d42, 0x38436, 0, 0, 0x42	};
	BITMAPINFOHEADER info = { 0x28, 0x140, 0xf0, 0x1, 0x10, 3, 0x38400, 0, 0, 0, 0 };

	info.biWidth = width;
	info.biHeight = height;
	width <<= 1;
	width = ((width + 3) >> 2) << 2;
	info.biSizeImage = width * height;
	info.biCompression = BI_BITFIELDS;
	bmp.bfSize = info.biSizeImage + 0x36 + 0xc;

	memcpy(dstbuf, &bmp, sizeof(BITMAPFILEHEADER));
	memcpy(dstbuf+sizeof(BITMAPFILEHEADER), &info, sizeof(BITMAPINFOHEADER));
	return sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER);
}

int CRaw2RgbF::addbmp666header(char *dstbuf, int width, int height)
{
	BITMAPFILEHEADER bmp = { 0x4d42, 0x38436, 0, 0, 0x36	};
	BITMAPINFOHEADER info = { 0x28, 0x140, 0xf0, 0x1, 0x12, 0, 0x38400, 0, 0, 0, 0 };

	info.biWidth = width;
	info.biHeight = height;
	width = width * 18 + 17;
	width >>= 3;
	width = ((width + 3) >> 2) << 2;
	info.biSizeImage = width * height;
	bmp.bfSize = info.biSizeImage + 0x36;

	memcpy(dstbuf, &bmp, sizeof(BITMAPFILEHEADER));
	memcpy(dstbuf+sizeof(BITMAPFILEHEADER), &info, sizeof(BITMAPINFOHEADER));
	return sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER);
}

int CRaw2RgbF::addbmp32header(char *dstbuf, int width, int height)
{
	BITMAPFILEHEADER bmp = { 0x4d42, 0x38436, 0, 0, 0x36	};
	BITMAPINFOHEADER info = { 0x28, 0x140, 0xf0, 0x1, 0x20, 0, 0x38400, 0, 0, 0, 0 };

	info.biWidth = width;
	info.biHeight = height;
	width <<= 2;
	info.biSizeImage = width * height;
	bmp.bfSize = info.biSizeImage + 0x36;

	memcpy(dstbuf, &bmp, sizeof(BITMAPFILEHEADER));
	memcpy(dstbuf+sizeof(BITMAPFILEHEADER), &info, sizeof(BITMAPINFOHEADER));
	return sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER);
}


int CRaw2RgbF::addbmp24header(char *dstbuf, int width, int height)
{
	BITMAPFILEHEADER bmp = { 0x4d42, 0x38436, 0, 0, 0x36	};
	BITMAPINFOHEADER info = { 0x28, 0x140, 0xf0, 0x1, 0x18, 0, 0x38400, 0, 0, 0, 0 };

	info.biWidth = width;
	info.biHeight = height;
	width *= 3;
	width = ((width + 3) >> 2) << 2;
	info.biSizeImage = width * height;
	bmp.bfSize = info.biSizeImage + 0x36;

	memcpy(dstbuf, &bmp, sizeof(BITMAPFILEHEADER));
	memcpy(dstbuf+sizeof(BITMAPFILEHEADER), &info, sizeof(BITMAPINFOHEADER));
	return sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER);
}

//=========================================
//
//		add palette
//
//=========================================
int CRaw2RgbF::add1palette(char *dstbuf)
{
	int buf[2] = { 0x0, 0xffffff };

	memcpy(dstbuf, buf, sizeof(buf));
	return sizeof(buf);
}

int CRaw2RgbF::add2palette(char *dstbuf)
{
	int buf[4] = { 0x0, 0x808080, 0xc0c0c0, 0xffffff };

	memcpy(dstbuf, buf, sizeof(buf));
	return sizeof(buf);
}

int CRaw2RgbF::add4palette(char *dstbuf)
{
	int buf[16] = 
	{ 
		0x0, 0x800000, 0x8000, 0x808000, 0x80, 0x800080, 0x8080, 0x808080,
		0xc0c0c0, 0xff0000, 0xff00, 0xffff00, 0xff, 0xff00ff, 0xffff, 0xffffff
	};

	memcpy(dstbuf, buf, sizeof(buf));
	return sizeof(buf);
}

int CRaw2RgbF::add8palette(char *dstbuf)
{
	int buf[256] = 
	{ 
		0x0, 0x800000, 0x8000, 0x808000, 0x80, 0x800080, 0x8080, 0xc0c0c0, 
		0xc0dcc0, 0xa6caf0, 0x402000, 0x602000, 0x802000, 0xa02000, 0xc02000, 0xe02000, 
		0x4000, 0x204000, 0x404000, 0x604000, 0x804000, 0xa04000, 0xc04000, 0xe04000, 
		0x6000, 0x206000, 0x406000, 0x606000, 0x806000, 0xa06000, 0xc06000, 0xe06000, 
		0x8000, 0x208000, 0x408000, 0x608000, 0x808000, 0xa08000, 0xc08000, 0xe08000, 
		0xa000, 0x20a000, 0x40a000, 0x60a000, 0x80a000, 0xa0a000, 0xc0a000, 0xe0a000, 
		0xc000, 0x20c000, 0x40c000, 0x60c000, 0x80c000, 0xa0c000, 0xc0c000, 0xe0c000, 
		0xe000, 0x20e000, 0x40e000, 0x60e000, 0x80e000, 0xa0e000, 0xc0e000, 0xe0e000, 
		0x40, 0x200040, 0x400040, 0x600040, 0x800040, 0xa00040, 0xc00040, 0xe00040, 
		0x2040, 0x202040, 0x402040, 0x602040, 0x802040, 0xa02040, 0xc02040, 0xe02040, 
		0x4040, 0x204040, 0x404040, 0x604040, 0x804040, 0xa04040, 0xc04040, 0xe04040, 
		0x6040, 0x206040, 0x406040, 0x606040, 0x806040, 0xa06040, 0xc06040, 0xe06040, 
		0x8040, 0x208040, 0x408040, 0x608040, 0x808040, 0xa08040, 0xc08040, 0xe08040, 
		0xa040, 0x20a040, 0x40a040, 0x60a040, 0x80a040, 0xa0a040, 0xc0a040, 0xe0a040, 
		0xc040, 0x20c040, 0x40c040, 0x60c040, 0x80c040, 0xa0c040, 0xc0c040, 0xe0c040, 
		0xe040, 0x20e040, 0x40e040, 0x60e040, 0x80e040, 0xa0e040, 0xc0e040, 0xe0e040, 
		0x80, 0x200080, 0x400080, 0x600080, 0x800080, 0xa00080, 0xc00080, 0xe00080, 
		0x2080, 0x202080, 0x402080, 0x602080, 0x802080, 0xa02080, 0xc02080, 0xe02080, 
		0x4080, 0x204080, 0x404080, 0x604080, 0x804080, 0xa04080, 0xc04080, 0xe04080, 
		0x6080, 0x206080, 0x406080, 0x606080, 0x806080, 0xa06080, 0xc06080, 0xe06080, 
		0x8080, 0x208080, 0x408080, 0x608080, 0x808080, 0xa08080, 0xc08080, 0xe08080, 
		0xa080, 0x20a080, 0x40a080, 0x60a080, 0x80a080, 0xa0a080, 0xc0a080, 0xe0a080, 
		0xc080, 0x20c080, 0x40c080, 0x60c080, 0x80c080, 0xa0c080, 0xc0c080, 0xe0c080, 
		0xe080, 0x20e080, 0x40e080, 0x60e080, 0x80e080, 0xa0e080, 0xc0e080, 0xe0e080, 
		0xc0, 0x2000c0, 0x4000c0, 0x6000c0, 0x8000c0, 0xa000c0, 0xc000c0, 0xe000c0, 
		0x20c0, 0x2020c0, 0x4020c0, 0x6020c0, 0x8020c0, 0xa020c0, 0xc020c0, 0xe020c0, 
		0x40c0, 0x2040c0, 0x4040c0, 0x6040c0, 0x8040c0, 0xa040c0, 0xc040c0, 0xe040c0, 
		0x60c0, 0x2060c0, 0x4060c0, 0x6060c0, 0x8060c0, 0xa060c0, 0xc060c0, 0xe060c0, 
		0x80c0, 0x2080c0, 0x4080c0, 0x6080c0, 0x8080c0, 0xa080c0, 0xc080c0, 0xe080c0, 
		0xa0c0, 0x20a0c0, 0x40a0c0, 0x60a0c0, 0x80a0c0, 0xa0a0c0, 0xc0a0c0, 0xe0a0c0, 
		0xc0c0, 0x20c0c0, 0x40c0c0, 0x60c0c0, 0x80c0c0, 0xa0c0c0, 0xfffbf0, 0xa0a0a4, 
		0x808080, 0xff0000, 0xff00, 0xffff00, 0xff, 0xff00ff, 0xffff, 0xffffff
	};

	memcpy(dstbuf, buf, sizeof(buf));
	return sizeof(buf);
}

int CRaw2RgbF::add565palette(char *dstbuf)
{
	int buf[3] = { 0xf800, 0x7e0, 0x1f };

	memcpy(dstbuf, buf, sizeof(buf));
	return sizeof(buf);
}

int CRaw2RgbF::add555palette(char *dstbuf)
{
	int buf[4] = { 0x7f00, 0x3e0, 0x1f };

	memcpy(dstbuf, buf, sizeof(buf));
	return sizeof(buf);
}

//=========================================
//
//		rgb convert to bmp
//
//=========================================
void CRaw2RgbF::Rgb32_Bmp(char *src, char *dest, int width, int height)
{
	CFileBin fsrc, fdst;
	CBufferBase pool;
	int length;

	if( (width == 0) || (height == 0) )
	{
		this->m_dbug.PrintMsg("error: src unit = 0 or dst unit = 0\n");
		return;
	}
	if(pool.GetSize() == 0)
	{
		this->m_dbug.PrintMsg("can't alloc pool\n");
		return;
	}

	length = this->addbmp32header(pool.m_buf, width, height);
	fdst.Open(dest, "wb");
	fdst.Write(pool.m_buf, length);
	fsrc.Open(src);
	this->bitrgb2bmp(&fsrc, &fdst, width, height, 32);
}

void CRaw2RgbF::Rgb24_Bmp(char *src, char *dest, int width, int height)
{
	CFileBin fsrc, fdst;
	CBufferBase pool;
	int length;

	if( (width == 0) || (height == 0) )
	{
		this->m_dbug.PrintMsg("error: src unit = 0 or dst unit = 0\n");
		return;
	}
	if(pool.GetSize() == 0)
	{
		this->m_dbug.PrintMsg("can't alloc pool\n");
		return;
	}

	length = this->addbmp24header(pool.m_buf, width, height);
	fdst.Open(dest, "wb");
	fdst.Write(pool.m_buf, length);
	fsrc.Open(src);
	this->bitrgb2bmp(&fsrc, &fdst, width, height, 24);
}

void CRaw2RgbF::Rgb666_Bmp(char *src, char *dest, int width, int height)
{
	CFileBin fsrc, fdst;
	CBufferBase pool;
	int length;

	if( (width == 0) || (height == 0) )
	{
		this->m_dbug.PrintMsg("error: src unit = 0 or dst unit = 0\n");
		return;
	}
	if(pool.GetSize() == 0)
	{
		this->m_dbug.PrintMsg("can't alloc pool\n");
		return;
	}

	length = this->addbmp666header(pool.m_buf, width, height);
	fdst.Open(dest, "wb");
	fdst.Write(pool.m_buf, length);
	fsrc.Open(src);
	this->bitrgb2bmp(&fsrc, &fdst, width, height, 18);
}

void CRaw2RgbF::Rgb565_Bmp(char *src, char *dest, int width, int height)
{
	CFileBin fsrc, fdst;
	CBufferBase pool;
	int length;

	if( (width == 0) || (height == 0) )
	{
		this->m_dbug.PrintMsg("error: src unit = 0 or dst unit = 0\n");
		return;
	}
	if(pool.GetSize() == 0)
	{
		this->m_dbug.PrintMsg("can't alloc pool\n");
		return;
	}

	length = this->addbmp565header(pool.m_buf, width, height);
	fdst.Open(dest, "wb");
	fdst.Write(pool.m_buf, length);
	length = this->add565palette(pool.m_buf);
	fdst.Write(pool.m_buf, length);
	fsrc.Open(src);
	this->bitrgb2bmp(&fsrc, &fdst, width, height, 16);
}

void CRaw2RgbF::Rgb555_Bmp(char *src, char *dest, int width, int height)
{
	CFileBin fsrc, fdst;
	CBufferBase pool;
	int length;

	if( (width == 0) || (height == 0) )
	{
		this->m_dbug.PrintMsg("error: src unit = 0 or dst unit = 0\n");
		return;
	}
	if(pool.GetSize() == 0)
	{
		this->m_dbug.PrintMsg("can't alloc pool\n");
		return;
	}

	length = this->addbmp555header(pool.m_buf, width, height);
	fdst.Open(dest, "wb");
	fdst.Write(pool.m_buf, length);
//	length = this->add555palette(pool.m_buf);
//	fdst.Write(pool.m_buf, length);
	fsrc.Open(src);
	this->bitrgb2bmp(&fsrc, &fdst, width, height, 16);
}

void CRaw2RgbF::Rgb444_Bmp(char *src, char *dest, int width, int height)
{
	CFileBin fsrc, fdst;
	CBufferBase pool;
	int length;

	if( (width == 0) || (height == 0) )
	{
		this->m_dbug.PrintMsg("error: src unit = 0 or dst unit = 0\n");
		return;
	}
	if(pool.GetSize() == 0)
	{
		this->m_dbug.PrintMsg("can't alloc pool\n");
		return;
	}

	length = this->addbmp444header(pool.m_buf, width, height);
	fdst.Open(dest, "wb");
	fdst.Write(pool.m_buf, length);
	fsrc.Open(src);
	this->bitrgb2bmp(&fsrc, &fdst, width, height, 12);
}

void CRaw2RgbF::Rgb8_Bmp(char *src, char *dest, int width, int height)
{
	CFileBin fsrc, fdst;
	CBufferBase pool;
	int length;

	if( (width == 0) || (height == 0) )
	{
		this->m_dbug.PrintMsg("error: src unit = 0 or dst unit = 0\n");
		return;
	}
	if(pool.GetSize() == 0)
	{
		this->m_dbug.PrintMsg("can't alloc pool\n");
		return;
	}

	fdst.Open(dest, "wb");
	length = this->addbmp8header(pool.m_buf, width, height);
	fdst.Write(pool.m_buf, length);
	fsrc.Open(src);
	fsrc.Read(pool.m_buf, 256 << 2);
	fdst.Write(pool.m_buf, 256 << 2);
	this->bitrgb2bmp(&fsrc, &fdst, width, height, 8);
}

void CRaw2RgbF::Rgb4_Bmp(char *src, char *dest, int width, int height)
{
	CFileBin fsrc, fdst;
	CBufferBase pool;
	int length;

	if( (width == 0) || (height == 0) )
	{
		this->m_dbug.PrintMsg("error: src unit = 0 or dst unit = 0\n");
		return;
	}
	if(pool.GetSize() == 0)
	{
		this->m_dbug.PrintMsg("can't alloc pool\n");
		return;
	}

	fdst.Open(dest, "wb");
	length = this->addbmp4header(pool.m_buf, width, height);
	fdst.Write(pool.m_buf, length);
	fsrc.Open(src);
	fsrc.Read(pool.m_buf, 16 << 2);
	fdst.Write(pool.m_buf, 16 << 2);
	this->bitrgb2bmp(&fsrc, &fdst, width, height, 4);
}

void CRaw2RgbF::Rgb2_Bmp(char *src, char *dest, int width, int height)
{
	CFileBin fsrc, fdst;
	CBufferBase pool;
	int length;

	if( (width == 0) || (height == 0) )
	{
		this->m_dbug.PrintMsg("error: src unit = 0 or dst unit = 0\n");
		return;
	}
	if(pool.GetSize() == 0)
	{
		this->m_dbug.PrintMsg("can't alloc pool\n");
		return;
	}

	fdst.Open(dest, "wb");
	length = this->addbmp2header(pool.m_buf, width, height);
	fdst.Write(pool.m_buf, length);
	fsrc.Open(src);
	fsrc.Read(pool.m_buf, 4 << 2);
	fdst.Write(pool.m_buf, 4 << 2);
	this->bitrgb2bmp(&fsrc, &fdst, width, height, 2);
}

void CRaw2RgbF::Rgb1_Bmp(char *src, char *dest, int width, int height)
{
	CFileBin fsrc, fdst;
	CBufferBase pool;
	int length;

	if( (width == 0) || (height == 0) )
	{
		this->m_dbug.PrintMsg("error: src unit = 0 or dst unit = 0\n");
		return;
	}
	if(pool.GetSize() == 0)
	{
		this->m_dbug.PrintMsg("can't alloc pool\n");
		return;
	}

	fdst.Open(dest, "wb");
	length = this->addbmp1header(pool.m_buf, width, height);
	fdst.Write(pool.m_buf, length);
	fsrc.Open(src);
	fsrc.Read(pool.m_buf, 2 << 2);
	fdst.Write(pool.m_buf, 2 << 2);
	this->bitrgb2bmp(&fsrc, &fdst, width, height, 1);
}

//=========================================
//
//		bmp convert to rgb
//
//=========================================
void CRaw2RgbF::Bmp32_Rgb(char *src, char *dest)
{
	CFileBin fsrc, fdst;
	CBufferBase pool;
	BITMAPINFOHEADER *info = (BITMAPINFOHEADER *)(pool.m_buf+sizeof(BITMAPFILEHEADER));

	if(pool.GetSize() == 0)
	{
		this->m_dbug.PrintMsg("can't alloc pool\n");
		return;
	}
	fsrc.Open(src);
	fdst.Open(dest, "wb");
	fsrc.Read(pool.m_buf, sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER));
	this->bitbmp2rgb(&fsrc, &fdst, info->biWidth, info->biHeight, 32);
}

void CRaw2RgbF::Bmp24_Rgb(char *src, char *dest)
{
	CFileBin fsrc, fdst;
	CBufferBase pool;
	BITMAPINFOHEADER *info = (BITMAPINFOHEADER *)(pool.m_buf+sizeof(BITMAPFILEHEADER));

	if(pool.GetSize() == 0)
	{
		this->m_dbug.PrintMsg("can't alloc pool\n");
		return;
	}
	fsrc.Open(src);
	fdst.Open(dest, "wb");
	fsrc.Read(pool.m_buf, sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER));
	this->bitbmp2rgb(&fsrc, &fdst, info->biWidth, info->biHeight, 24);
}

void CRaw2RgbF::Bmp666_Rgb(char *src, char *dest)
{
	CFileBin fsrc, fdst;
	CBufferBase pool;
	BITMAPINFOHEADER *info = (BITMAPINFOHEADER *)(pool.m_buf+sizeof(BITMAPFILEHEADER));

	if(pool.GetSize() == 0)
	{
		this->m_dbug.PrintMsg("can't alloc pool\n");
		return;
	}
	fsrc.Open(src);
	fdst.Open(dest, "wb");
	fsrc.Read(pool.m_buf, sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER));
	this->bitbmp2rgb(&fsrc, &fdst, info->biWidth, info->biHeight, 18);
}

void CRaw2RgbF::Bmp565_Rgb(char *src, char *dest)
{
	CFileBin fsrc, fdst;
	CBufferBase pool;
	BITMAPINFOHEADER *info = (BITMAPINFOHEADER *)(pool.m_buf+sizeof(BITMAPFILEHEADER));

	if(pool.GetSize() == 0)
	{
		this->m_dbug.PrintMsg("can't alloc pool\n");
		return;
	}
	fsrc.Open(src);
	fdst.Open(dest, "wb");
	fsrc.Read(pool.m_buf, sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER));
	fsrc.Read(pool.m_buf, 12);		//palette
	this->bitbmp2rgb(&fsrc, &fdst, info->biWidth, info->biHeight, 16);
}

void CRaw2RgbF::Bmp555_Rgb(char *src, char *dest)
{
	CFileBin fsrc, fdst;
	CBufferBase pool;
	BITMAPINFOHEADER *info = (BITMAPINFOHEADER *)(pool.m_buf+sizeof(BITMAPFILEHEADER));

	if(pool.GetSize() == 0)
	{
		this->m_dbug.PrintMsg("can't alloc pool\n");
		return;
	}
	fsrc.Open(src);
	fdst.Open(dest, "wb");
	fsrc.Read(pool.m_buf, sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER));
	if(info->biCompression)
		fsrc.Read(pool.m_buf, 12);		//palette
	this->bitbmp2rgb(&fsrc, &fdst, info->biWidth, info->biHeight, 16);
}

void CRaw2RgbF::Bmp444_Rgb(char *src, char *dest)
{
	CFileBin fsrc, fdst;
	CBufferBase pool;
	BITMAPINFOHEADER *info = (BITMAPINFOHEADER *)(pool.m_buf+sizeof(BITMAPFILEHEADER));

	if(pool.GetSize() == 0)
	{
		this->m_dbug.PrintMsg("can't alloc pool\n");
		return;
	}
	fsrc.Open(src);
	fdst.Open(dest, "wb");
	fsrc.Read(pool.m_buf, sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER));
	this->bitbmp2rgb(&fsrc, &fdst, info->biWidth, info->biHeight, 12);
}

void CRaw2RgbF::Bmp8_Rgb(char *src, char *dest)
{
	CFileBin fsrc, fdst;
	CBufferBase pool, pool1;
	int gap;
	BITMAPINFOHEADER *info = (BITMAPINFOHEADER *)(pool1.m_buf+sizeof(BITMAPFILEHEADER));

	if((pool.GetSize() == 0) || (pool1.GetSize() == 0))
	{
		this->m_dbug.PrintMsg("can't alloc pool\n");
		return;
	}
	fsrc.Open(src);
	fdst.Open(dest, "wb");
	fsrc.Read(pool1.m_buf, sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER));

	gap = 0x400;
	if(info->biClrUsed)
		gap = info->biClrUsed << 2;
	fsrc.Read(pool.m_buf, gap);		//palette
	fdst.Write(pool.m_buf, 0x400);

	this->bitbmp2rgb(&fsrc, &fdst, info->biWidth, info->biHeight, 8);
}

void CRaw2RgbF::Bmp4_Rgb(char *src, char *dest)
{
	CFileBin fsrc, fdst;
	CBufferBase pool, pool1;
	int len;
	BITMAPINFOHEADER *info = (BITMAPINFOHEADER *)(pool1.m_buf+sizeof(BITMAPFILEHEADER));

	if((pool.GetSize() == 0) || (pool1.GetSize() == 0))
	{
		this->m_dbug.PrintMsg("can't alloc pool\n");
		return;
	}
	fsrc.Open(src);
	fdst.Open(dest, "wb");
	fsrc.Read(pool1.m_buf, sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER));

	len = 0x40;
	if(info->biClrUsed)
		len = info->biClrUsed << 2;
	fsrc.Read(pool.m_buf, len);		//palette
	fdst.Write(pool.m_buf, 0x40);
	this->bitbmp2rgb(&fsrc, &fdst, info->biWidth, info->biHeight, 4);
}

void CRaw2RgbF::Bmp2_Rgb(char *src, char *dest)
{
	CFileBin fsrc, fdst;
	CBufferBase pool, pool1;
	int len;
	BITMAPINFOHEADER *info = (BITMAPINFOHEADER *)(pool1.m_buf+sizeof(BITMAPFILEHEADER));

	if((pool.GetSize() == 0) || (pool1.GetSize() == 0))
	{
		this->m_dbug.PrintMsg("can't alloc pool\n");
		return;
	}
	fsrc.Open(src);
	fdst.Open(dest, "wb");
	fsrc.Read(pool1.m_buf, sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER));

	len = 32;
	if(info->biClrUsed)
		len = info->biClrUsed << 2;
	fsrc.Read(pool.m_buf, len);		//palette
	fdst.Write(pool.m_buf, 32);

	this->bitbmp2rgb(&fsrc, &fdst, info->biWidth, info->biHeight, 2);
}

void CRaw2RgbF::Bmp1_Rgb(char *src, char *dest)
{
	CFileBin fsrc, fdst;
	CBufferBase pool, pool1;
	int len;
	BITMAPINFOHEADER *info = (BITMAPINFOHEADER *)(pool1.m_buf+sizeof(BITMAPFILEHEADER));

	if((pool.GetSize() == 0) || (pool1.GetSize() == 0))
	{
		this->m_dbug.PrintMsg("can't alloc pool\n");
		return;
	}
	fsrc.Open(src);
	fdst.Open(dest, "wb");
	fsrc.Read(pool1.m_buf, sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER));

	len = 8;
	if(info->biClrUsed)
		len = info->biClrUsed << 2;
	fsrc.Read(pool.m_buf, len);		//palette
	fdst.Write(pool.m_buf, 8);
	
	this->bitbmp2rgb(&fsrc, &fdst, info->biWidth, info->biHeight, 1);
}

//=========================================
//
//		print bmp file information
//
//=========================================
void CRaw2RgbF::PrintBmpFileInfo(char *src, char *dest, TBitMap *info, int print)
{
	CFileBin fsrc;
	CBufferBase pool;

	if(pool.GetSize() == 0)
	{
		this->m_dbug.PrintMsg("can't alloc pool: 0x%x\n", pool.GetSize());
		return;
	}
	fsrc.Open(src);
	fsrc.Read(pool.m_buf, sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER) + 1024);
	if(print)
		this->printbmpinfo(dest, pool.m_buf);
	this->getbmpinfo(pool.m_buf, info);
}

void CRaw2RgbF::printbmpinfo(char *dest, char *buf)
{
	CFileBin fdst;
	char str[0x100];
	int i, count;
	BITMAPFILEHEADER *bmp = (BITMAPFILEHEADER *)buf;
	BITMAPINFOHEADER *info = (BITMAPINFOHEADER *)(buf+sizeof(BITMAPFILEHEADER));
	int *pbuf = (int *)(buf + sizeof(BITMAPINFOHEADER) + sizeof(BITMAPFILEHEADER));

	fdst.Open(dest);
	fdst.Open(dest, "wb");
	sprintf(str, "{ 0x%x, 0x%x, 0x%x, 0x%x, 0x%x };\n", (int)bmp->bfType, (int)bmp->bfSize, 
		(int)bmp->bfReserved1, (int)bmp->bfReserved2, (int)bmp->bfOffBits);
	fdst.Write(str, (int)strlen(str));
	sprintf(str, "{ 0x%x, 0x%x, 0x%x, 0x%x, 0x%x, 0x%x, 0x%x, 0x%x, 0x%x, 0x%x, 0x%x };\n", 
		(int)info->biSize, (int)info->biWidth, (int)info->biHeight, (int)info->biPlanes, 
		(int)info->biBitCount, (int)info->biCompression, (int)info->biSizeImage, 
		(int)info->biXPelsPerMeter, (int)info->biYPelsPerMeter, (int)info->biClrUsed, 
		(int)info->biClrImportant);
	fdst.Write(str, (int)strlen(str));
	if(info->biBitCount > 8)
		return;
	count = 1 << info->biBitCount;
	sprintf(str, "{ ");
	fdst.Write(str, (int)strlen(str));
	for(i = 0; i < count; i++)
	{
		if( (i % 8) == 0 )
		{
			sprintf(str, "\n\t");
			fdst.Write(str, (int)strlen(str));
		}
		sprintf(str, "0x%x, ", pbuf[i]);
		fdst.Write(str, (int)strlen(str));
	}
	sprintf(str, "\n};");
	fdst.Write(str, (int)strlen(str));
}


int CRaw2RgbF::getbmpinfo(char *srcbuf, TBitMap *pinfo)
{
	TBitMap bminfo;
	BITMAPFILEHEADER *bmp = (BITMAPFILEHEADER *)srcbuf;
	BITMAPINFOHEADER *info = (BITMAPINFOHEADER *)(srcbuf+sizeof(BITMAPFILEHEADER));
	int *pPal = (int *)(srcbuf + sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER));
	const int BmpFormat[] = { 1, 2, 4, 8, 12, 16, 16, 18, 24, 32 };
	const int RgbFormat[] = { RGB1, RGB2, RGB4, RGB8, RGB444, RGB555, RGB565, RGB666, RGB24, RGB32 };
	int i;

	if(pinfo == NULL)
		pinfo = &bminfo;
	pinfo->width = info->biWidth;
	pinfo->height = info->biHeight;
	pinfo->index = RGB32 + 1;
	if(bmp->bfType != 0x4d42)
		return 0;
	if(info->biBitCount == 16)
	{
		pinfo->index = RGB555;
		if(info->biCompression == 3)
		{
			if(pPal[0] == 0x7c00)
				return RGB555;
			pinfo->index = RGB565;
			return RGB565;
		}
		return RGB555;
	}
	for(i = 0; i < sizeof(BmpFormat); i++)
	{
		if(info->biBitCount == BmpFormat[i])
		{
			pinfo->index = RgbFormat[i];
			return RgbFormat[i];
		}
	}
	return 0;
}

int CRaw2RgbF::GetBmpFileFormat(char *src)
{
	CFileBin fsrc;
	CBufferBase pool;

	if(pool.GetSize() == 0)
	{
		this->m_dbug.PrintMsg("can't alloc pool\n");
		return 0;
	}
	fsrc.Open(src);
	fsrc.Read(pool.m_buf, sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER) + 0xc);

	return this->getbmpinfo(pool.m_buf);
}

//Compute the width of per pixel in a bmp file.
int CRaw2RgbF::GetBmpFileBitWidth(char * src)
{
	int bitwidth = 0, srcfmt = this->GetBmpFileFormat(src);

	switch(srcfmt)
	{
	case RGB1:
		bitwidth = 1;
		break;
	case RGB2:
		bitwidth = 2;
		break;
	case RGB4:
		bitwidth = 4;
		break;
	case RGB8:
		bitwidth = 8;
		break;
	case RGB444:
		bitwidth = 12;
		break;
	case RGB666:
		bitwidth = 18;
		break;
	case RGB32:
		bitwidth = 32;
		break;
	case RGB24:
		bitwidth = 24;
		break;
	case RGB565:
		bitwidth = 16;
		break;
	case RGB555:
		bitwidth = 16;
		break;
	default:
		break;
	}

	return bitwidth;
}

void CRaw2RgbF::RgbResize(char *src, char *dest, int srcw, int srch, int dstw, int dsth, int unit)
{
	CFileBin fsrc, fdst;
	CBufferBase srcbuf, dstbuf;
	int size, w, h;

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

	size = CRaw2Rgb::RgbResize(srcbuf.m_buf, dstbuf.m_buf, srcw, srch, dstw, dsth, unit);

	fdst.Write(dstbuf.m_buf, size);
}

void CRaw2RgbF::RgbSizer(char *src, char *dest, int srcw, int srch, int dstw, int dsth, int px, int py, int unit)
{
	CFileBin fsrc, fdst;
	CBufferBase srcbuf;
	int i, size, size1, offset;

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

void CRaw2RgbF::RgbPrefilter(char *src, char *dest, int width, int height, 
					int count, int *filter, int unit)
{
	CFileBin fsrc, fdst;
	CBufferBase srcbuf, dstbuf;
	int size;

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
	size = CRaw2Rgb::MRgbPrefilter(srcbuf.m_buf, dstbuf.m_buf, width, height, count, filter, unit);
	fdst.Open(dest, "wb");
	fdst.Write(dstbuf.m_buf, size);
}

//=======================================================
//
//		Rgb rotation
//
//=======================================================
void CRaw2RgbF::Rgb24Rotate(char *src, char *dest, int width, int height, int rotate, int unit)
{
	CFileBin fsrc, fdst;
	CBufferBase srcbuf, dstbuf;
	int size;

	size = width * height * unit * 3;
	srcbuf.SetSize(size);
	dstbuf.SetSize(size);
	if( (srcbuf.GetTotalSize() == 0) && (dstbuf.GetTotalSize() == 0) )
	{
		this->m_dbug.PrintMsg("Can not allocate so big buffer\n");
		return;
	}
	fsrc.Open(src);
	fsrc.Read(srcbuf.m_buf, size);
	CRaw2Rgb::Rgb24Rotate(srcbuf.m_buf, dstbuf.m_buf, width, height, rotate, unit);
	fdst.Open(dest, "wb");
	fdst.Write(dstbuf.m_buf, size);
}

//=======================================================
//
//		Rgb rotation
//
//=======================================================
void CRaw2RgbF::bayer2rgborder(char *src, char *dest, int order, int width, int height, int unit)
{
	CBufferBase	srcbuf, dstbuf;
	CFileBin fsrc, fdst;
	int size;

	if( (unit != 1) && (unit != 2) )
		unit = 1;
	if(width % 2)
		return;
	size = width * height * unit;
	srcbuf.SetSize(size, 1);
	fsrc.Open(src);
	fsrc.Read(srcbuf.m_buf, size);
	size *= 3;
	dstbuf.SetSize(size);
	size = CRaw2Rgb::bayer2rgborder(srcbuf.m_buf, dstbuf.m_buf, order, width, height, unit);
	fdst.Open(dest, "wb");
	fdst.Write(dstbuf.m_buf, size);
}

void CRaw2RgbF::GB2Rgb(char *src, char *dest, int width, int height, int unit)
{
	this->bayer2rgborder(src, dest, RAW_GB, width, height, unit);
}

void CRaw2RgbF::BG2Rgb(char *src, char *dest, int width, int height, int unit)
{
	this->bayer2rgborder(src, dest, RAW_BG, width, height, unit);
}

void CRaw2RgbF::GR2Rgb(char *src, char *dest, int width, int height, int unit)
{
	this->bayer2rgborder(src, dest, RAW_GR, width, height, unit);
}

void CRaw2RgbF::RG2Rgb(char *src, char *dest, int width, int height, int unit)
{
	this->bayer2rgborder(src, dest, RAW_RG, width, height, unit);
}
