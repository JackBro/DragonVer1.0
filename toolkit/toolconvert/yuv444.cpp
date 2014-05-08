
#include "internal.h"

//=============	class CYuv444 ==============================
int CYuv444::MYuv444Sizer(char *src, char *dest, int srcw, int dstw, int dsth, int px, int py, int unit)
{
	int i, size, size1, offset;
	char *psrc, *pdst;

	size = srcw * unit * 3;
	size1 = dstw * unit * 3;
	offset = px * unit * 3;

	psrc = src + size * py;
	pdst = dest;
	for(i = 0; i < dsth; i++, psrc += size, pdst += size1)
		memcpy(pdst, psrc+offset, size1);

	return size1 * dsth;
}

int CYuv444::MRgb2Yuv444(char *src, char *dest, int width, int height, int cap, int unit)
{
	int i, size;

	size = unit * 3;
	height *= width * unit * 3;
	width = 0;
	for(i = 0; i < height; i += size)
		width += this->Rgb2Yuv444(src + i, dest + width, cap, unit);
	return width;
}

int CYuv444::MYuv444_Rgb(char *src, char *dest, int width, int height, int cap, int unit)
{
	int i, size;

	size = unit * 3;
	height *= width * unit * 3;
	width = 0;
	for(i = 0; i < height; i += size)
		width += this->Yuv444_Rgb(src + i, dest + width, cap, unit);
	return width;
}

int CYuv444::Rgb2Yuv444(char *src, char *dst, int cap, int unit)
{
	int yuv[3], bgr[3];

	CYuv2Rgb::getbgr(src, bgr, unit);

	CYuv2Rgb::rgb2yuv(bgr, yuv, cap);

	CYuv2Rgb::setyuv444(dst, yuv, unit);

	return unit * 3;
}

int CYuv444::Yuv444_Rgb(char *src, char *dst, int cap, int unit)
{
	int yuv[3], bgr[3];

	CYuv2Rgb::getyuv444(src, yuv, unit);

	CYuv2Rgb::yuv2rgb(yuv, bgr, cap);

	CYuv2Rgb::setbgr(dst, bgr, unit);

	return unit * 3;
}

int CYuv444::Yuv444_Blk(char *src, char *dst, int width, int height, int unit)
{
	int i, j, offset, step = unit * 3;
	int yuv[3];
	int pos, blkwidth;
	char *psrc = src;

	blkwidth = (((width + 7) >> 3) * 192) * unit;
	for(i = 0; i < height; i++)
	{
		offset = ((i >> 3) * blkwidth) + ((i % 8) << 3) * unit;
		for(j = 0; j < width; j++, psrc += step)
		{
			CYuv2Rgb::getyuv444(psrc, yuv, unit);

			pos = (((j >> 3) * 192) + (j % 8)) * unit;
			pos += offset;
			CBaseConvert::setvalue(dst+pos, yuv[0], unit);
			CBaseConvert::setvalue(dst+pos+(unit << 6), yuv[1], unit);
			CBaseConvert::setvalue(dst+pos+(unit << 7), yuv[2], unit);
		}
	}
	return blkwidth * ((height + 7) >> 3);
}

int CYuv444::Blk2Yuv444(char *src, char *dst, int width, int height, int unit)
{
	int i, j, offset, step = unit * 3;
	int yuv[3];
	int pos, blkwidth;
	char *pdst = dst;

	blkwidth = (((width + 7) >> 3) * 192) * unit;
	for(i = 0; i < height; i++)
	{
		offset = ((i >> 3) * blkwidth) + ((i % 8) << 3) * unit;
		for(j = 0; j < width; j++, pdst += step)
		{
			pos = (((j >> 3) * 192) + (j % 8)) * unit;
			pos += offset;
			yuv[0] = CYuv2Rgb::getvalue(src+pos, unit);
			yuv[1] = CYuv2Rgb::getvalue(src+pos+(unit << 6), unit);
			yuv[2] = CYuv2Rgb::getvalue(src+pos+(unit << 7), unit);
			CYuv2Rgb::setyuv444(pdst, yuv, unit);
		}
	}
	return blkwidth * ((height + 7) >> 3);
}

int CYuv444::Yuv444_Lbuf(char *src, char *dst, int width, int height, int unit)
{
	int i, j, k, ypos, upos, vpos, step = unit * 3;
	int yuv[3];
	int blkwidth;
	char *psrc = src;

	blkwidth = (((width + 7) >> 3) * 192) * unit;
	for(i = 0; i < height; i += 8)
	{
		ypos = (i >> 3) * blkwidth;
		upos = ypos + (blkwidth / 3);
		vpos = upos + (blkwidth / 3);
		k = ((height - i) < 8) ? (height - i) : 8;
		for(; k; k--)
		{
			for(j = 0; j < width; j++, psrc += step, ypos += unit, upos += unit, vpos += unit)
			{
				CYuv2Rgb::getyuv444(psrc, yuv, unit);
				CBaseConvert::setvalue(dst+ypos, yuv[0], unit);
				CBaseConvert::setvalue(dst+upos, yuv[1], unit);
				CBaseConvert::setvalue(dst+vpos, yuv[2], unit);
			}
		}
	}
	return blkwidth * ((height + 7) >> 3);
}

int CYuv444::Lbuf2Yuv444(char *src, char *dst, int width, int height, int unit)
{
	int i, j, k, ypos, upos, vpos, step = unit * 3;
	int yuv[3];
	int blkwidth;
	char *pdst = dst;

	blkwidth = (((width + 7) >> 3) * 192) * unit;
	for(i = 0; i < height; i += 8)
	{
		ypos = (i >> 3) * blkwidth;
		upos = ypos + (blkwidth / 3);
		vpos = upos + (blkwidth / 3);
		k = ((height - i) < 8) ? (height - i) : 8;
		for(; k; k--)
		{
			for(j = 0; j < width; j++, pdst += step, ypos += unit, upos += unit, vpos += unit)
			{
				yuv[0] = CBaseConvert::getvalue(src+ypos, unit);
				yuv[1] = CBaseConvert::getvalue(src+upos, unit);
				yuv[2] = CBaseConvert::getvalue(src+vpos, unit);
				CYuv2Rgb::setyuv444(pdst, yuv, unit);
			}
		}
	}
	return blkwidth * ((height + 7) >> 3);
}

void CYuv444::Yuv444ZoutHorResize(char *src, char *dst, int srcw, int srch, int dstw, int unit)
{
	int i, j, up, down, k, step = unit * 3;
	int val0, val1, yuv;
	char *psrc = src;
	char *pdst = dst;

	if(srcw == dstw)
	{
		if(dst != src)
			memcpy(dst, src, srcw * srch * 3 * unit);
		return;
	}
	for(j = 0; j < srch; j++)
	{
		memcpy(pdst, psrc, step);
		psrc += step;
		pdst += step;
		up = dstw - 1;
		for(i = 1; i < srcw; i++, up += dstw - 1, psrc += step)
		{
			if(up < (srcw - 1))
				continue;
			up -= srcw - 1;
			down = dstw - 1 - up;
			for(k = 0; k < step; k += unit, pdst += unit)
			{
				if(up == 0)
				{
					yuv = CYuv2Rgb::getvalue(psrc + k, unit);
					CBaseConvert::setvalue(pdst, yuv, unit);
				}
				else
				{
					val0 = CYuv2Rgb::getvalue(psrc + k - step, unit);
					val1 = CYuv2Rgb::getvalue(psrc + k, unit);
					yuv = (val0 * up + val1 * down) / (dstw - 1);
					CBaseConvert::setvalue(pdst, yuv, unit);
				}
			}
		}
		//memcpy(pdst, psrc, step);
		//psrc += step;
		//pdst += step;
	}
}

void CYuv444::Yuv444ZinHorResize(char *src, char *dst, int srcw, int srch, int dstw, int unit)
{
	int i, j, up, down, k, gap, step = unit * 3, offset = 0;
	int val0, val1, yuv;
	char *psrc = src;
	char *pdst = dst;

	if(srcw == dstw)
	{
		if(dst != src)
			memcpy(dst, src, srcw * srch * 3 * unit);
		return;
	}
	gap = srcw * step;
	for(j = 0; j < srch; j++, psrc += gap)
	{
		up = 0;
		offset = 0;
		for(i = 0; i < dstw; i++, up += srcw - 1)
		{
			if(up >= dstw - 1)
			{
				up -= dstw - 1;
				offset += step;
			}
			down = dstw - 1 - up;
			for(k = 0; k < step; k += unit, pdst += unit)
			{
				if(up == 0)
				{
					yuv = CYuv2Rgb::getvalue(psrc + offset + k, unit);
					CBaseConvert::setvalue(pdst, yuv, unit);
				}
				else
				{
					val0 = CYuv2Rgb::getvalue(psrc + offset + k, unit);
					val1 = CYuv2Rgb::getvalue(psrc + offset + k + step, unit);
					yuv = (val0 * down + val1 * up) / (dstw - 1);
					CBaseConvert::setvalue(pdst, yuv, unit);
				}
			}
		}
	}
}

void CYuv444::Yuv444ZoutVerResize(char *src, char *dst, int srcw, int srch, int dsth, int unit)
{
	int i, j, up, down, k, gap, step = unit * 3;
	int yuv, val0, val1;
	char *psrc = src;
	char *pdst = dst;

	if(srch == dsth)
	{
		if(dst != src)
			memcpy(dst, src, srcw * srch * 3 * unit);
		return;
	}
	gap = srcw * 3 * unit;
	memcpy(pdst, psrc, gap);
	psrc += gap;
	pdst += gap;
	up = dsth - 1;
	for(j = 1; j < srch-1; j++, up += dsth - 1)
	{
		if(up < (srch - 1))
		{
			psrc += gap;
			continue;
		}
		up -= srch - 1;
		down = dsth - 1 - up;
		for(i = 0; i < srcw; i++)
		{
			for(k = 0; k < step; k += unit, psrc += unit, pdst += unit)
			{
				if(up == 0)
				{
					yuv = CYuv2Rgb::getvalue(psrc, unit);
					CBaseConvert::setvalue(pdst, yuv, unit);
				}
				else
				{
					val0 = CYuv2Rgb::getvalue(psrc - gap, unit);
					val1 = CYuv2Rgb::getvalue(psrc, unit);
					yuv = (val0 * up + val1 * down) / (dsth - 1);
					CBaseConvert::setvalue(pdst, yuv, unit);
				}
			}
		}
	}
	memcpy(pdst, psrc, gap);
}

void CYuv444::Yuv444ZinVerResize(char *src, char *dst, int srcw, int srch, int dsth, int unit)
{
	int i, j, up, down, k, gap, step = unit * 3;
	int yuv, val0, val1;
	char *psrc = src;
	char *pdst = dst;

	if(srch == dsth)
	{
		if(dst != src)
			memcpy(dst, src, srcw * srch * 3 * unit);
		return;
	}
	up = 0;
	gap = srcw * step;
	for(j = 0; j < dsth; j++, up += srch - 1)
	{
		if(up >= (dsth - 1))
		{
			up -= dsth - 1;
			psrc += gap;
		}
		down = dsth - 1 - up;
		for(i = 0; i < gap; i += step)
		{
			for(k = 0; k < step; k += unit, pdst += unit)
			{
				if(up == 0)
				{
					yuv = CYuv2Rgb::getvalue(psrc + k + i, unit);
					CBaseConvert::setvalue(pdst, yuv, unit);
				}
				else
				{
					val0 = CYuv2Rgb::getvalue(psrc + k + i, unit);
					val1 = CYuv2Rgb::getvalue(psrc + gap + i + k, unit);
					yuv = (val0 * down + val1 * up) / (dsth - 1);
					CBaseConvert::setvalue(pdst, yuv, unit);
				}
			}
		}
	}
}

int CYuv444::Yuv444Resize(char *src, char *dst, int srcw, int srch, int dstw, int dsth, int unit)
{
	int len = dstw * dsth * unit * 3;

	if(dsth <= srch)
	{
		this->Yuv444ZoutVerResize(src, src, srcw, srch, dsth, unit);
		if(dstw <= srcw)
			this->Yuv444ZoutHorResize(src, dst, srcw, dsth, dstw, unit);
		else
			this->Yuv444ZinHorResize(src, dst, srcw, dsth, dstw, unit);
	}
	else
	{
		this->Yuv444ZinVerResize(src, dst, srcw, srch, dsth, unit);
		if(dstw <= srcw)
			this->Yuv444ZoutHorResize(dst, src, srcw, dsth, dstw, unit);
		else
			this->Yuv444ZinHorResize(dst, src, srcw, dsth, dstw, unit);
		memcpy(dst, src, len);
	}

	return len;
}


//=============	yuv444 prefilter ==============================
int CYuv444::MYuv444Prefilter1(char *src, char *dest, int width, int height, 
					int count, int *filter, int unit)
{
	return CBaseConvert::Prefilter(src, dest, width, height, 3, count, filter, unit);
}

int CYuv444::MYuv444Prefilter(char *src, char *dst, int width, int height, 
					int count, int *filter, int unit)
{
	int size = width * height * unit * 3, pos = 0, pos1 = 0, size1;
	CFilter444 tools(width, unit);

	size1 = size;
	count = size;
	tools.SetFilter(filter);
	while(size)
	{
		width = tools.Write(src+pos, size);
		pos += width;
		size -= width;
		width = tools.Read(dst+pos1, size1);
		pos1 += width;
		size1 -= width;
	}
	while(width)
	{
		width = tools.Read(dst+pos1, size1);
		pos1 += width;
		size1 -= width;
	}
	return count;
}

//=======================================================
//
//		YUV444 encode
//
//=======================================================
int CYuv444::getVwc(int width, int height)
{
	width = ((width + 7) >> 3) << 3;
	height = ((height + 7) >> 3) << 3;
	width *= height * 3;
	return width;
}

int CYuv444::Blk2Rgb24(char *src, char *dest, int width, int height)
{
	int len, size, msize, i, cap, ncount = 0;
	char* psrcbuf, *pdstbuf, *ptmpbuf;
	CBufferBase tmpbuf;
	CBlk2YuvBuf blkbuf(YUV444, width, 1);
	size = msize = (height * width) * 3;
	tmpbuf.SetSize(msize);
	
	//YuvBlk to YUV444:
	psrcbuf = src;
	ptmpbuf = tmpbuf.m_buf;
	while(size)
	{
		blkbuf.ClearPoint();
		len = blkbuf.Write(psrcbuf, size);
		if(len == 0)
			break;
		psrcbuf += len;
		size -= len;
		len = blkbuf.Read(ptmpbuf, msize);
		ptmpbuf += len;
	}

	//YUV444 to Rgb24:
	ptmpbuf = tmpbuf.m_buf;
	pdstbuf = dest;
	cap = 256;
	size = 3;
	len = width * height * 3;
	for(i = 0; i < len; i += size)
	{
		width = CYuv444::Yuv444_Rgb(ptmpbuf, pdstbuf, cap, 1);
		pdstbuf += width;
		ncount += width;
		ptmpbuf += size;
	}	
	return ncount;
}

int CYuv444::Rgb2Yuv444Blk(char *src, char *dest, int width, int height, int cap, int unit)
{
	int i, len, size, msize, ncount = 0;
	char* pbuf;
	CBufferBase tmpbuf;
	CYuv2BlkBuf blkbuf(YUV444, width, unit);

	msize = (height * width * unit) * 3;
	size = unit * 3;
	tmpbuf.SetSize(msize);
	pbuf = tmpbuf.m_buf;
	
	for(i = 0; i < msize; i += size)
	{
		len = CYuv444::Rgb2Yuv444(src, pbuf, cap, unit);
		src += size;
		pbuf += len;
		ncount += len;
	}

	pbuf = tmpbuf.m_buf;
	size = ncount;
	ncount = 0;
	
	while(size)
	{
		blkbuf.ClearPoint();
		len = blkbuf.Write(pbuf, size);
		if(len == 0)
			break;
		pbuf += len;
		size -= len;
		len = blkbuf.Read(dest, msize);
		dest += len;
		ncount += len;
	}
	return ncount;
}

int CYuv444::Blk2Jpeg(char *src, char *dest, int width, int height, int qf)
{
	CJpegEncoder enc;
	int len;

	enc.SetJpegQf(qf);
	enc.SetJpegSize(width, height);
	enc.SetYuvFmt(YUV444BLK);
	enc.Open();
	len = enc.getVwc();
	enc.Write(src, len);
	len = enc.Read(dest, len);
	return len;
}

int CYuv444::Rgb2Jpeg444(char *src, char *dest, int width, int height, int unit, int qfactor)
{
	int len, size;
	CBufferBase tmpbuf;
	int cap = 256;
	if(unit == 2)
		cap = 1024;
	
	//this size is to be used for YuvBlk444 size (including head & tail), so is greater than width*height*3.
	size = width*height << 2; 
	tmpbuf.SetSize(size, unit);
	
	len = this->Rgb2Yuv444Blk(src, tmpbuf.m_buf, width, height, cap, unit);
	len = this->Blk2Jpeg(tmpbuf.m_buf, dest, width, height, qfactor);
	return len;
}
//=======================================================
//
//		YUV444 rotation
//
//=======================================================
void CYuv444::Yuv444Rotate(char *src, char *dest, int width, int height, int rotate, int unit)
{
	CBaseConvert::MRotate(src, dest, width, height, rotate, unit * 3);
}


