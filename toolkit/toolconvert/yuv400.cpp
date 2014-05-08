
#include "internal.h"

int CYuv400::Yuv400HorExtent(char *src, char *dest, int width, int height, int count, int unit)
{
	int i, j, size, val, pos;
	char *psrc, *pdst;

	size = width * height * unit;
	if(count == 1)
	{
		memcpy(dest, src, size);
		return size;
	}
	psrc = src;
	pdst = dest;
	count--;
	size = width * unit;
	for(i = 0; i < height; i++, psrc += size)
	{
		for(j = 0, pos = count * unit; j < count; j++, pdst += unit, pos -= unit)
		{
			val = CYuv2Rgb::getvalue(psrc+pos, unit);
			CBaseConvert::setvalue(pdst, val, unit);
		}
		memcpy(pdst, psrc, size);
		pdst += size;
		for(j = 0, pos = (width-2) * unit; j < count; j++, pdst += unit, pos -= unit)
		{
			val = CYuv2Rgb::getvalue(psrc+pos, unit);
			CBaseConvert::setvalue(pdst, val, unit);
		}
	}
	return (width + (count << 1)) * height * unit;
}

int CYuv400::Yuv400VerExtent(char *src, char *dest, int width, int height, int count, int unit)
{
	int i, size, step;
	char *psrc, *pdst;

	size = width * height * unit;
	if(count == 1)
	{
		memcpy(dest, src, size);
		return size;
	}
	count--;
	step = width * unit;
	memcpy(dest + step * count, src, size);
	psrc = src + (step * count);
	pdst = dest;
	for(i = 0; i < count; i++, psrc -= step, pdst += step)
		memcpy(pdst, psrc, step);
	psrc = src + size - (step << 1);
	pdst += size;
	for(i = 0; i < count; i++, psrc -= step, pdst += step)
		memcpy(pdst, psrc, step);
	return width * (height + (count << 1)) * unit;
}

int CYuv400::MYuv400Enlarge(char *src, char *dest, int width, int height, int count, int unit)
{
	this->Yuv400HorExtent(src, dest, width, height, count, unit);
	width += (count - 1) << 1;
	this->Yuv400VerExtent(dest, src, width, height, count, unit);
	height += (count - 1) << 1;
	width *= height * unit;
	memcpy(dest, src, width);
	return width;
}

int CYuv400::Yuv400HorPrefilter(char *src, char *dest, int width, int height, 
								int count, int *filter, int unit)
{
	int i, j, k, size, total, val, m, n;
	char *psrc, *pdst, *psrc1;
	int dat[MAXFILTERCOUNT];

	if(count > (MAXFILTERCOUNT >> 1))
		return 0;
	size = width * height * unit;
	if(count == 1)
	{
		memcpy(dest, src, size);
		return size;
	}
	total = filter[0];
	for(i = 1; i < count; i++)
		total += filter[i] << 1;
	count--;
	psrc = src;
	pdst = dest;
	size = (count * unit) << 1;
	m = width - count;
	n = (count << 1) + 1;
	for(i = 0; i < height; i++)
	{
		for(j = count; j < m; j++, pdst += unit, psrc += unit)
		{
			for(k = 0, psrc1 = psrc; k < n; k++, psrc1 += unit)
				dat[k] = CYuv2Rgb::getvalue(psrc1, unit);
			val = CYuv2Rgb::prefilter(dat, filter, count);
			val /= total;
			CBaseConvert::setvalue(pdst, val, unit);
		}
		psrc += size;
	}
	width -= count << 1;
	return width * height * unit;
}

int CYuv400::Yuv400VerPrefilter(char *src, char *dest, int width, int height, 
								int count, int *filter, int unit)
{
	int i, j, k, size, total, val, m, n;
	char *psrc, *pdst, *pdst1, *psrc1;
	int dat[MAXFILTERCOUNT];

	if(count > (MAXFILTERCOUNT >> 1))
		return 0;
	size = width * height * unit;
	if(count == 1)
	{
		memcpy(dest, src, size);
		return size;
	}
	total = filter[0];
	for(i = 1; i < count; i++)
		total += filter[i] << 1;
	psrc = src;
	pdst = dest;
	size = width * unit;
	count--;
	m = height - count;
	n = (count << 1) + 1;
	for(i = 0; i < width; i++, psrc += unit, pdst += unit)
	{
		for(j = count, pdst1 = pdst, psrc1 = psrc; j < m; j++, pdst1 += size, psrc1 += size)
		{
			for(k = 0, total = 0; k < n; k++, total += size)
				dat[k] = CYuv2Rgb::getvalue(psrc1+total, unit);
			val = CYuv2Rgb::prefilter(dat, filter, count);
			val /= total;
			CBaseConvert::setvalue(pdst1, val, unit);
		}
	}
	height -= count << 1;
	return width * height * unit;
}

int CYuv400::MYuv400Prefilter(char *src, char *dest, int width, int height, 
					int count, int *filter, int unit)
{
	this->MYuv400Enlarge(src, dest, width, height, count, unit);
	this->Yuv400HorPrefilter(src, dest, width + ((count - 1) << 1), height + ((count - 1) << 1), 
			count, filter, unit);
	this->Yuv400VerPrefilter(dest, src, width, height + ((count - 1) << 1), count, filter, unit);
	width *= height * unit;
	memcpy(src, dest, width);
	return width;
}

int CYuv400::MRgb2Yuv400(char *src, char *dest, int width, int height, int cap, int unit)
{
	int i, size;

	size = unit * 3;
	height *= width * unit * 3;
	width = 0;
	for(i = 0; i < height; i += size)
		width += this->Rgb2Yuv400(src + i, dest + width, cap, unit);
	return width;
}

int CYuv400::MYuv400_Rgb(char *src, char *dest, int width, int height, int cap, int unit)
{
	int i;

	height *= width * unit;
	width = 0;
	for(i = 0; i < height; i += unit)
		width += this->Yuv400_Rgb(src + i, dest + width, cap, unit);
	return width;
}

int CYuv400::MYuv444_400(char *src, char *dest, int width, int height, int unit)
{
	int i, size;

	size = unit * 3;
	height *= width * unit * 3;
	width = 0;
	for(i = 0; i < height; i += size)
		width += this->Yuv444_400(src + i, dest + width, unit);
	return width;
}

int CYuv400::MYuv400_444(char *src, char *dest, int width, int height, int unit)
{
	int i;

	height *= width * unit;
	width = 0;
	for(i = 0; i < height; i += unit)
		width += this->Yuv400_444(src + i, dest + width, unit);
	return width;
}

int CYuv400::MYuv400Sizer(char *src, char *dest, int srcw, int dstw, int dsth, int px, int py, int unit)
{
	int i, size, size1, offset;
	char *psrc, *pdst;

	size = srcw * unit;
	size1 = dstw * unit;
	offset = px * unit;

	psrc = src + size * py;
	pdst = dest;
	for(i = 0; i < dsth; i++, psrc += size, pdst += size1)
		memcpy(pdst, psrc+offset, size1);

	return size1 * dsth;
}

int CYuv400::Rgb2Yuv400(char *src, char *dst, int cap, int unit)
{
	int y, bgr[3];

	CYuv2Rgb::getbgr(src, bgr, unit);

	y = CYuv2Rgb::rgb2y(bgr, cap);

	CBaseConvert::setvalue(dst, y, unit);

	return unit;
}

int CYuv400::Yuv400_Rgb(char *src, char *dst, int cap, int unit)
{
	int yuv[3], bgr[3];

	CYuv2Rgb::getyuvfrom400(src, yuv, unit);

	CYuv2Rgb::yuv2rgb(yuv, bgr, cap);

	CYuv2Rgb::setbgr(dst, bgr, unit);

	return unit * 3;
}

int CYuv400::Yuv444_400(char *src, char *dst, int unit)
{
	int yuv[3];

	CYuv2Rgb::getyuv444(src, yuv, unit);

	CBaseConvert::setvalue(dst, yuv[0], unit);

	return unit;
}

int CYuv400::Yuv400_444(char *src, char *dst, int unit)
{
	int yuv[3];

	CYuv2Rgb::getyuvfrom400(src, yuv, unit);

	CYuv2Rgb::setyuv444(dst, yuv, unit);

	return unit * 3;
}

int	CYuv400::Yuv2Blk(char *src, char *dst, int width, int height, int unit)
{
	int i, j, offset;
	int y;
	int pos, blkwidth;
	char *psrc = src;

	blkwidth = (((width + 7) >> 3) << 6) * unit;
	for(i = 0; i < height; i++)
	{
		offset = ((i >> 3) * blkwidth) + ((i % 8) << 3) * unit;
		for(j = 0; j < width; j++, psrc += unit)
		{
			y = CYuv2Rgb::getvalue(psrc, unit);

			pos = (((j >> 3) << 6) + (j % 8)) * unit;
			pos += offset;

			CBaseConvert::setvalue(dst+pos, y, unit);
		}
	}
	return blkwidth * ((height + 7) >> 3);
}

int	CYuv400::Blk2Yuv(char *src, char *dst, int width, int height, int unit)
{
	int i, j, offset;
	int y;
	int pos, blkwidth;
	char *pdst = dst;

	blkwidth = (((width + 7) >> 3) << 6) * unit;
	for(i = 0; i < height; i++)
	{
		offset = ((i >> 3) * blkwidth) + ((i % 8) << 3) * unit;
		for(j = 0; j < width; j++, pdst += unit)
		{
			pos = ( ((j >> 3) << 6) + (j % 8) ) * unit;
			pos += offset;
			y = CYuv2Rgb::getvalue(src+pos, unit);

			CBaseConvert::setvalue(pdst, y, unit);
		}
	}
	return blkwidth * ((height + 7) >> 3);
}

int	CYuv400::Yuv2Lbuf(char *src, char *dst, int width, int height, int unit)
{
	width *= height;
	width *= unit;
	memcpy(dst, src, width);
	return width;
}

int	CYuv400::Lbuf2Yuv(char *src, char *dst, int width, int height, int unit)
{
	width *= height;
	width *= unit;
	memcpy(dst, src, width);
	return width;
}

//======================================================
//
//			Yuv image resize
//
//=================================================
void CYuv400::Yuv400ZoutHorResize(char *src, char *dst, int srcw, int srch, int dstw, int unit)
{
	int i, j, up, down;
	int val0, val1, yuv;
	char *psrc = src;
	char *pdst = dst;

	if(srcw == dstw)
	{
		if(dst != src)
			memcpy(dst, src, srcw * srch * unit);
		return;
	}
	for(j = 0; j < srch; j++)
	{
		yuv = CYuv2Rgb::getvalue(psrc, unit);
		CBaseConvert::setvalue(pdst, yuv, unit);
		psrc += unit;
		pdst += unit;
		up = dstw - 1;
		for(i = 1; i < srcw; i++, up += dstw - 1, psrc += unit)
		{
			if(up < srcw - 1)
				continue;
			up -= srcw - 1;
			down = dstw - 1 - up;
			if(up == 0)
			{
				yuv = CYuv2Rgb::getvalue(psrc, unit);
				CBaseConvert::setvalue(pdst, yuv, unit);
			}
			else
			{
				val0 = CYuv2Rgb::getvalue(psrc - unit, unit);
				val1 = CYuv2Rgb::getvalue(psrc, unit);
				yuv = (val0 * down + val1 * up) / (dstw - 1);
				CBaseConvert::setvalue(pdst, yuv, unit);
			}
			pdst += unit;
		}
	}
}

void CYuv400::Yuv400ZinHorResize(char *src, char *dst, int srcw, int srch, int dstw, int unit)
{
	int i, j, up, down, gap, offset = 0;
	int val0, val1, yuv;
	char *psrc = src;
	char *pdst = dst;

	if(srcw == dstw)
	{
		if(dst != src)
			memcpy(dst, src, srcw * srch * unit);
		return;
	}
	gap = srcw * unit;
	for(j = 0; j < srch; j++, psrc += gap)
	{
		up = 0;
		offset = 0;
		for(i = 1; i < dstw; i++, up += srcw, pdst += unit)
		{
			if(up >= dstw + 1)
			{
				up -= dstw + 1;
				offset += unit;
			}
			down = dstw + 1 - up;
			if(up == 0)
			{
				yuv = CYuv2Rgb::getvalue(psrc + offset, unit);
				CBaseConvert::setvalue(pdst, yuv, unit);
			}
			else
			{
				val0 = CYuv2Rgb::getvalue(psrc + offset, unit);
				val1 = CYuv2Rgb::getvalue(psrc + offset + unit, unit);
				yuv = (val0 * down + val1 * up) / (dstw + 1);
				CBaseConvert::setvalue(pdst, yuv, unit);
			}
		}
	}
}

void CYuv400::Yuv400ZoutVerResize(char *src, char *dst, int srcw, int srch, int dsth, int unit)
{
	int i, j, up, down, gap;
	int yuv, val0, val1;
	char *psrc = src;
	char *pdst = dst;

	if(srch == dsth)
	{
		if(dst != src)
			memcpy(dst, src, srcw * srch * unit);
		return;
	}
	up = srch + 1;
	gap = srcw * unit;
	memcpy(pdst, psrc, gap);
	psrc += gap;
	pdst += gap;
	up = dsth - 1;
	for(j = 1; j < srch; j++, up += dsth - 1)
	{
		if(up < (srch - 1))
		{
			psrc += gap;
			continue;
		}
		up -= srch - 1;
		down = dsth - 1 - up;
		for(i = 0; i < srcw; i++, psrc += unit, pdst += unit)
		{
			if(up == 0)
			{
				yuv = CYuv2Rgb::getvalue(psrc, unit);
				CBaseConvert::setvalue(pdst, yuv, unit);
			}
			else
			{
				val0 = CYuv2Rgb::getvalue(psrc + gap, unit);
				val1 = CYuv2Rgb::getvalue(psrc, unit);
				yuv = (val0 * down + val1 * up) / (dsth - 1);
				CBaseConvert::setvalue(pdst, yuv, unit);
			}
		}
	}
}

void CYuv400::Yuv400ZinVerResize(char *src, char *dst, int srcw, int srch, int dsth, int unit)
{
	int i, j, up, down, gap;
	int yuv, val0, val1;
	char *psrc = src;
	char *pdst = dst;

	if(srch == dsth)
	{
		if(dst != src)
			memcpy(dst, src, srcw * srch * 3);
		return;
	}
	up = 0;
	gap = srcw * unit;
	for(j = 0; j < dsth; j++, up += srch)
	{
		if(up >= (dsth+1))
		{
			up -= dsth + 1;
			psrc += gap;
		}
		down = dsth + 1 - up;
		for(i = 0; i < gap; i += unit, pdst += unit)
		{
			if(up == 0)
			{
				yuv = CYuv2Rgb::getvalue(psrc + i, unit);
				CBaseConvert::setvalue(pdst, yuv, unit);
			}
			else
			{
				val0 = CYuv2Rgb::getvalue(psrc + i, unit);
				val1 = CYuv2Rgb::getvalue(psrc + gap + i, unit);
				yuv = (val0 * down + val1 * up) / (dsth + 1);
				CBaseConvert::setvalue(pdst, yuv, unit);
			}
		}
	}
}

int CYuv400::Yuv400Resize(char *src, char *dst, int srcw, int srch, int dstw, int dsth, int unit)
{
	int len = dstw * dsth * unit;

	if(dsth <= srch)
	{
		this->Yuv400ZoutVerResize(src, src, srcw, srch, dsth, unit);
		if(dstw <= srcw)
			this->Yuv400ZoutHorResize(src, dst, srcw, dsth, dstw, unit);
		else
			this->Yuv400ZinHorResize(src, dst, srcw, dsth, dstw, unit);
	}
	else
	{
		this->Yuv400ZinVerResize(src, dst, srcw, srch, dsth, unit);
		if(dstw <= srcw)
			this->Yuv400ZoutHorResize(dst, src, srcw, dsth, dstw, unit);
		else
			this->Yuv400ZinHorResize(dst, src, srcw, dsth, dstw, unit);
		memcpy(dst, src, len);
	}

	return len;
}

//=======================================================
//
//		YUV400 encode
//
//=======================================================
int CYuv400::getVwc(int width, int height)
{
	width = ((width + 7) >> 3) << 3;
	height = ((height + 7) >> 3) << 3;
	width *= height;
	return width;
}

int CYuv400::Blk2Rgb24(char *src, char *dest, int width, int height)
{
	int len, size, i, cap, msize, ncount = 0;
	char* psrcbuf, *pdstbuf, *ptmpbuf;
	CBlk2YuvBuf blkbuf(YUV400, width, 1);
	CBufferBase tmpbuf;

	//Blk to Yuv400:
	size = msize = width * height;
	tmpbuf.SetSize(msize);
	ptmpbuf = tmpbuf.m_buf;
	psrcbuf = src;
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

	//YUV400 to Rgb24:
	ptmpbuf = tmpbuf.m_buf;
	pdstbuf = dest;
	cap = 256;
	size = 1;
	len = width * height;
	for(i = 0; i < len; i += size)
	{
		width = CYuv400::Yuv400_Rgb(ptmpbuf, pdstbuf, cap, 1);
		pdstbuf += width;
		ncount += width;
		ptmpbuf += size;
	}	
	return ncount;
}

int CYuv400::Rgb2Yuv400Blk(char *src, char *dest, int width, int height, int cap, int unit)
{
	int i, len, size, msize, nCount = 0;
	char *pbuf, *pDstbuf;
	CBufferBase tmpbuf;
	CYuv2BlkBuf blkbuf(YUV400, width, unit);
	
	size = unit * 3;
	msize = width * height * unit;
	tmpbuf.SetSize(msize);
	pbuf = tmpbuf.m_buf;
	
	for(i = 0; i < msize*3; i += size)
	{
		len = CYuv400::Rgb2Yuv400(src, pbuf, cap, unit);
		pbuf += len;
		src += size;
	}
	
	pbuf = tmpbuf.m_buf;
	size = msize;
	pDstbuf = dest;
	
	while(size)
	{
		blkbuf.ClearPoint();
		len = blkbuf.Write(pbuf, size);
		if(len == 0)
			break;
		pbuf += len;
		size -= len;
		len = blkbuf.Read(pDstbuf, msize);
		pDstbuf += len;
		nCount += len; //the number of bytes stored in dest is added up.
	}
	
	return nCount;
}

int CYuv400::Blk2Jpeg(char *src, char *dest, int width, int height, int qf)
{
	CJpegEncoder enc;
	int len;

	enc.SetJpegQf(qf);
	enc.SetJpegSize(width, height);
	enc.SetYuvFmt(YUV400BLK);
	enc.Open();
	len = enc.getVwc();
	enc.Write(src, len);
	len = enc.Read(dest, len);
	return len;
}


int CYuv400::Rgb2Jpeg400(char * src, char * dst, int width, int height, int unit, int qf)
{
	int len, cap, size;
	CBufferBase tmpbuf;

	size = this->getVwc(width, height);
	tmpbuf.SetSize(size, unit);
	cap = 256;
	if(unit == 2)
		cap = 1024;
	
	len = CYuv400::Rgb2Yuv400Blk(src, tmpbuf.m_buf, width, height, cap, unit);
	len = CYuv400::Blk2Jpeg(tmpbuf.m_buf, dst, width, height, qf);
	return len;
}
