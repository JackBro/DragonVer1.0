
#include "internal.h"

//=============	class CYuv411 ==============================
int CYuv411::yuv411_resizeorder(char *src, char *dst, int srcw, int srch, int dstw, 
							   int dsth, int order, int unit)
{
	this->MYuv411_444order(src, dst, srcw, srch, order, unit);
	CYuv444::Yuv444Resize(dst, src, srcw, srch, dstw, dsth, unit);
	return this->MYuv444_411order(src, dst, dstw, dsth, order, unit);
}

int CYuv411::MYuv411Sizer(char *src, char *dest, int srcw, int dstw, int dsth, 
								 int px, int py, int order, int unit)
{
	int i, j, size, size1, offset, step = unit * 6, step1 = step << 1;
	char *psrc, *pdst, *psrc1, *pdst1;
	CBufferBase pool;

	size = (srcw * unit * 3) >> 1;
	size1 = (dstw * unit * 3) >> 1;
	offset = (px * unit * 3) >> 1;
	psrc = src + size * py;
	pdst = dest;

	for(i = 0; i < dsth; i++, psrc += size)
	{
		if(px % 4)
		{
			psrc1 = psrc;
			pdst1 = pool.m_buf;
			for(j = 0; j < srcw; j += 4, psrc1 += step, pdst1 += step1)
				this->yuv411_444order(psrc1, pdst1, order, unit);
			psrc1 = pool.m_buf + (px * unit * 3);
			for(j = 0; j < dstw; j += 4, psrc1 += step1, pdst += step)
				this->yuv444_411order(psrc1, pdst, order, unit);
			offset = 0;
		}
		else
			memcpy(pdst, psrc + offset, size1);
	}
	return size1 * py;
}

int CYuv411::MRgb2Yuv411order(char *src, char *dst, int width, int height, int order, int cap, int unit)
{
	int i, size;

	size = unit * 3 * 4;
	height *= width * unit * 3;
	width = 0;
	for(i = 0; i < height; i += size)
		width += this->rgb2yuv411order(src+i, dst + width, order, cap, unit);
	return width;
}

int CYuv411::MYuv411_Rgborder(char *src, char *dst, int width, int height, int order, int cap, int unit)
{
	int i, size;

	size = unit * 6;
	height *= width * unit * 3;
	height >>= 1;
	width = 0;
	for(i = 0; i < height; i += size)
		width += this->rgb2yuv411order(src+i, dst + width, order, cap, unit);
	return width;
}

int CYuv411::MYuv444_411order(char *src, char *dst, int width, int height, int order, int unit)
{
	int i, size;

	size = unit * 3 * 4;
	height *= width * unit * 3;
	width = 0;
	for(i = 0; i < height; i += size)
		width += this->yuv444_411order(src+i, dst + width, order, unit);
	return width;
}

int CYuv411::MYuv411_444order(char *src, char *dst, int width, int height, int order, int unit)
{
	int i, size;

	size = unit * 6;
	height *= width * unit * 3;
	height >>= 1;
	width = 0;
	for(i = 0; i < height; i += size)
		width += this->yuv411_444order(src+i, dst + width, order, unit);
	return width;
}

int CYuv411::rgb2yuv411order(char *src, char *dst, int order, int cap, int unit)
{
	int i, step = unit * 3;
	int yuv[12], bgr[3];
	char *psrc = src;

	for(i = 0; i < 12; i += 3, psrc += step)
	{
		CYuv2Rgb::getbgr(psrc, bgr, unit);
		CYuv2Rgb::rgb2yuv(bgr, yuv+i, cap);
	}

	yuv[1] += yuv[4] + yuv[7] + yuv[10];
	yuv[2] += yuv[5] + yuv[8] + yuv[11];
	yuv[1] >>= 2;
	yuv[2] >>= 2;

	CYuv2Rgb::setyuv411(dst, yuv, order, unit);

	return unit * 6;
}

int CYuv411::yuv411_rgborder(char *src, char *dst, int order, int cap, int unit)
{
	int i, step = unit * 3;
	char *pdst = dst;
	int yuv[12], bgr[3];

	CYuv2Rgb::getyuvfrom411(src, yuv, order, unit);

	for(i = 0; i < 12; i += 3, pdst += step)
	{
		CYuv2Rgb::yuv2rgb(yuv+i, bgr, cap);
		CYuv2Rgb::setbgr(pdst, bgr, unit);
	}
	return unit * 12;
}

int CYuv411::yuv444_411order(char *src, char *dst, int order, int unit)
{
	int i, step = unit * 3;
	int yuv[12];
	char *psrc = src;

	for(i = 0; i < 12; i += 3, psrc += step)
		CYuv2Rgb::getyuv444(psrc, yuv+i, unit);

	yuv[1] += yuv[4] + yuv[7] + yuv[10];
	yuv[2] += yuv[5] + yuv[8] + yuv[11];
	yuv[1] >>= 2;
	yuv[2] >>= 2;

	CYuv2Rgb::setyuv411(dst, yuv, order, unit);

	return unit * 6;
}

int CYuv411::yuv411_444order(char *src, char *dst, int order, int unit)
{
	int i, step = unit * 3;
	char *pdst = dst;
	int yuv[12];

	CYuv2Rgb::getyuvfrom411(src, yuv, order, unit);

	for(i = 0; i < 12; i += 3, pdst += step)
		CYuv2Rgb::setyuv444(pdst, yuv+i, unit);
	return unit * 12;
}

int CYuv411::yuv411_blkorder(char *src, char *dst, int order, int width, int height, int unit)
{
	int i, j, offset, step = unit * 6;
	int yuv[12];
	int pos, blkwidth;
	char *psrc = src;

	blkwidth = ( (((width + 31) >> 5) * 3) << 7 ) * unit;
	for(i = 0; i < height; i++)
	{
		offset = ((i >> 3) * blkwidth) + (((i % 8) << 3) * unit);
		for(j = 0; j < width; j += 4, psrc += step)
		{
			CYuv2Rgb::getyuv411(psrc, yuv, order, unit);

			pos = (((j >> 5) * 384) + (((j % 32) >> 3) << 6) + (j % 8)) * unit;
			pos += offset;
			CBaseConvert::setvalue(dst+pos, yuv[0], unit);
			CBaseConvert::setvalue(dst+pos+unit, yuv[3], unit);
			CBaseConvert::setvalue(dst+pos+(unit << 1), yuv[6], unit);
			CBaseConvert::setvalue(dst+pos+(unit * 3), yuv[9], unit);

			pos = (((j >> 5) * 384) + 256 + ((j % 32) >> 2)) * unit;
			pos += offset;
			CBaseConvert::setvalue(dst+pos, yuv[1], unit);
			CBaseConvert::setvalue(dst+pos+(unit << 6), yuv[2], unit);
		}
	}
	return blkwidth * ((height + 7) >> 3);
}

int CYuv411::blk2yuv411order(char *src, char *dst, int order, int width, int height, int unit)
{
	int i, j, offset, step = unit * 6;
	int yuv[12];
	int pos, blkwidth;
	char *pdst = dst;

	blkwidth = ( (((width + 31) >> 5) * 3) << 7 ) * unit;
	for(i = 0; i < height; i++)
	{
		offset = ((i >> 3) * blkwidth) + (((i % 8) << 3) * unit);
		for(j = 0; j < width; j += 4, pdst += step)
		{
			pos = (((j >> 5) * 384) + (((j % 32) >> 3) << 6) + (j % 8)) * unit;
			pos += offset;
			yuv[0] = CYuv2Rgb::getvalue(src+pos, unit);
			yuv[3] = CYuv2Rgb::getvalue(src+pos+unit, unit);
			yuv[6] = CYuv2Rgb::getvalue(src+pos+(unit << 1), unit);
			yuv[9] = CYuv2Rgb::getvalue(src+pos+(unit * 3), unit);

			pos = (((j >> 5) * 384) + 256 + ((j % 32) >> 2)) * unit;
			pos += offset;
			yuv[1] = CYuv2Rgb::getvalue(src+pos, unit);
			yuv[2] = CYuv2Rgb::getvalue(src+pos+(unit << 6), unit);

			CYuv2Rgb::setyuv411(pdst, yuv, order, unit);
		}
	}
	return blkwidth * ((height + 7) >> 3);
}

int CYuv411::yuv411_lbuforder(char *src, char *dst, int order, int width, int height, int unit)
{
	int i, j, k, ypos, upos, vpos, step = unit * 6;
	int yuv[12];
	int blkwidth;
	char *psrc = src;

	blkwidth = ( (((width + 31) >> 5) * 3) << 7 ) * unit;
	for(i = 0; i < height; i += 8)
	{
		ypos = (i >> 3) * blkwidth;
		upos = ypos + ((blkwidth << 1) / 3);
		vpos = upos + (blkwidth / 6);
		k = ((height - i) < 8) ? (height - i) : 8;
		for(; k; k--)
		{
			for(j = 0; j < width; j += 4, psrc += step , ypos += unit << 2, upos += unit, vpos += unit)
			{
				CYuv2Rgb::getyuv411(psrc, yuv, order, unit);
				CBaseConvert::setvalue(dst + ypos, yuv[0], unit);
				CBaseConvert::setvalue(dst + ypos + unit, yuv[3], unit);
				CBaseConvert::setvalue(dst + ypos + (unit << 1), yuv[6], unit);
				CBaseConvert::setvalue(dst + ypos + (step >> 1), yuv[9], unit);
				CBaseConvert::setvalue(dst + upos, yuv[1], unit);
				CBaseConvert::setvalue(dst + vpos, yuv[2], unit);
			}
		}
	}
	return blkwidth * ((height + 7) >> 3);
}

int CYuv411::lbuf2yuv411order(char *src, char *dst, int order, int width, int height, int unit)
{
	int i, j, k, ypos, upos, vpos, step = unit * 6;
	int yuv[12];
	int blkwidth;
	char *pdst = dst;

	blkwidth = ( (((width + 31) >> 5) * 3) << 7 ) * unit;
	for(i = 0; i < height; i += 8)
	{
		ypos = (i >> 3) * blkwidth;
		upos = ypos + ((blkwidth << 1) / 3);
		vpos = upos + (blkwidth / 6);
		k = ((height - i) < 8) ? (height - i) : 8;
		for(; k; k--)
		{
			for(j = 0; j < width; j += 4, pdst += step, ypos += unit << 2, upos += unit, vpos += unit)
			{
				yuv[0] = CBaseConvert::getvalue(src + ypos, unit);
				yuv[3] = CBaseConvert::getvalue(src + ypos + unit, unit);
				yuv[6] = CBaseConvert::getvalue(src + ypos + (unit << 1), unit);
				yuv[9] = CBaseConvert::getvalue(src + ypos + (step >> 1), unit);
				yuv[1] = CBaseConvert::getvalue(src + upos, unit);
				yuv[2] = CBaseConvert::getvalue(src + vpos, unit);
				CYuv2Rgb::setyuv411(pdst, yuv, order, unit);
			}
		}
	}
	return blkwidth * ((height + 7) >> 3);
}

//===============================================================
//
//		Rgb convert to YUV411
//
//==============================================================
int CYuv411::Rgb2Yyyyuv(char *src, char *dst, int cap, int unit)
{
	return this->rgb2yuv411order(src, dst, YUV411_YYYYUV, cap, unit);
}

int CYuv411::Rgb2Yyyyvu(char *src, char *dst, int cap, int unit)
{
	return this->rgb2yuv411order(src, dst, YUV411_YYYYVU, cap, unit);
}

int CYuv411::Rgb2Yuyvyy(char *src, char *dst, int cap, int unit)
{
	return this->rgb2yuv411order(src, dst, YUV411_YUYVYY, cap, unit);
}

int CYuv411::Rgb2Yvyuyy(char *src, char *dst, int cap, int unit)
{
	return this->rgb2yuv411order(src, dst, YUV411_YVYUYY, cap, unit);
}

int CYuv411::Rgb2Uvyyyy(char *src, char *dst, int cap, int unit)
{
	return this->rgb2yuv411order(src, dst, YUV411_UVYYYY, cap, unit);
}

int CYuv411::Rgb2Vuyyyy(char *src, char *dst, int cap, int unit)
{
	return this->rgb2yuv411order(src, dst, YUV411_VUYYYY, cap, unit);
}

//===============================================================
//
//		yuv411 convert to RGB
//
//==============================================================

int CYuv411::Yyyyuv2Rgb(char *src, char *dst, int cap, int unit)
{
	return this->yuv411_rgborder(src, dst, YUV411_YYYYUV, cap, unit);
}

int CYuv411::Yyyyvu2Rgb(char *src, char *dst, int cap, int unit)
{
	return this->yuv411_rgborder(src, dst, YUV411_YYYYVU, cap, unit);
}

int CYuv411::Yuyvyy2Rgb(char *src, char *dst, int cap, int unit)
{
	return this->yuv411_rgborder(src, dst, YUV411_YUYVYY, cap, unit);
}

int CYuv411::Yvyuyy2Rgb(char *src, char *dst, int cap, int unit)
{
	return this->yuv411_rgborder(src, dst, YUV411_YVYUYY, cap, unit);
}

int CYuv411::Uvyyyy2Rgb(char *src, char *dst, int cap, int unit)
{
	return this->yuv411_rgborder(src, dst, YUV411_UVYYYY, cap, unit);
}

int CYuv411::Vuyyyy2Rgb(char *src, char *dst, int cap, int unit)
{
	return this->yuv411_rgborder(src, dst, YUV411_VUYYYY, cap, unit);
}

//===============================================================
//
//		Yuv444 convert to yuv411
//
//==============================================================
int CYuv411::Yuv444_Yyyyuv(char *src, char *dst, int unit)
{
	return this->yuv444_411order(src, dst, YUV411_YYYYUV, unit);
}

int CYuv411::Yuv444_Yyyyvu(char *src, char *dst, int unit)
{
	return this->yuv444_411order(src, dst, YUV411_YYYYVU, unit);
}

int CYuv411::Yuv444_Yuyvyy(char *src, char *dst, int unit)
{
	return this->yuv444_411order(src, dst, YUV411_YUYVYY, unit);
}

int CYuv411::Yuv444_Yvyuyy(char *src, char *dst, int unit)
{
	return this->yuv444_411order(src, dst, YUV411_YVYUYY, unit);
}

int CYuv411::Yuv444_Uvyyyy(char *src, char *dst, int unit)
{
	return this->yuv444_411order(src, dst, YUV411_UVYYYY, unit);
}

int CYuv411::Yuv444_Vuyyyy(char *src, char *dst, int unit)
{
	return this->yuv444_411order(src, dst, YUV411_VUYYYY, unit);
}

//===============================================================
//
//		yuv411 convert to yuv444
//
//==============================================================

int CYuv411::Yyyyuv_444(char *src, char *dst, int unit)
{
	return this->yuv411_444order(src, dst, YUV411_YYYYUV, unit);
}

int CYuv411::Yyyyvu_444(char *src, char *dst, int unit)
{
	return this->yuv411_444order(src, dst, YUV411_YYYYVU, unit);
}

int CYuv411::Yuyvyy_444(char *src, char *dst, int unit)
{
	return this->yuv411_444order(src, dst, YUV411_YUYVYY, unit);
}

int CYuv411::Yvyuyy_444(char *src, char *dst, int unit)
{
	return this->yuv411_444order(src, dst, YUV411_YVYUYY, unit);
}

int CYuv411::Uvyyyy_444(char *src, char *dst, int unit)
{
	return this->yuv411_444order(src, dst, YUV411_UVYYYY, unit);
}

int CYuv411::Vuyyyy_444(char *src, char *dst, int unit)
{
	return this->yuv411_444order(src, dst, YUV411_VUYYYY, unit);
}

//===============================================================
//
//		Block convert to yuv411
//
//==============================================================
int CYuv411::Blk2Yyyyuv(char *src, char *dst, int width, int height, int unit)
{
	return this->blk2yuv411order(src, dst, YUV411_YYYYUV, width, height, unit);
}

int CYuv411::Blk2Yyyyvu(char *src, char *dst, int width, int height, int unit)
{
	return this->blk2yuv411order(src, dst, YUV411_YYYYVU, width, height, unit);
}

int CYuv411::Blk2Yuyvyy(char *src, char *dst, int width, int height, int unit)
{
	return this->blk2yuv411order(src, dst, YUV411_YUYVYY, width, height, unit);
}

int CYuv411::Blk2Yvyuyy(char *src, char *dst, int width, int height, int unit)
{
	return this->blk2yuv411order(src, dst, YUV411_YVYUYY, width, height, unit);
}

int CYuv411::Blk2Uvyyyy(char *src, char *dst, int width, int height, int unit)
{
	return this->blk2yuv411order(src, dst, YUV411_UVYYYY, width, height, unit);
}

int CYuv411::Blk2Vuyyyy(char *src, char *dst, int width, int height, int unit)
{
	return this->blk2yuv411order(src, dst, YUV411_VUYYYY, width, height, unit);
}

int CYuv411::Blk2Rgb24(char * src, char * dest, int width, int height)
{
	int len, size, msize, cap, i, ncount = 0;
	char* psrcbuf, *pdstbuf, *ptmpbuf;
	CBlk2YuvBuf blkbuf((char)YUV411_UVYYYY, width, 1);
	CBufferBase tmpbuf;

	size = msize = (height * width) * 3 >> 1;
	tmpbuf.SetSize(msize);
	ptmpbuf = tmpbuf.m_buf;
	psrcbuf = src;

	//Blk to YUV411_UVYYYY	
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

	//YUV411_UVYYYY to Rgb24
	ptmpbuf = tmpbuf.m_buf;
	pdstbuf = dest;
	cap = 256;
	size = 6;
	len = (width * height * 3) >> 1;
	
	for(i = 0; i < len; i += size)
	{
		width = CYuv411::yuv411_rgborder(ptmpbuf, pdstbuf, YUV411_UVYYYY, cap, 1);
		pdstbuf += width;
		ncount += width;
		ptmpbuf += size;
	}

	return ncount;

}
//===============================================================
//
//		yuv411 convert to block
//
//==============================================================
int CYuv411::Yyyyuv2Blk(char *src, char *dst, int width, int height, int unit)
{
	return this->yuv411_blkorder(src, dst, YUV411_YYYYUV, width, height, unit);
}

int CYuv411::Yyyyvu2Blk(char *src, char *dst, int width, int height, int unit)
{
	return this->yuv411_blkorder(src, dst, YUV411_YYYYVU, width, height, unit);
}

int CYuv411::Yuyvyy2Blk(char *src, char *dst, int width, int height, int unit)
{
	return this->yuv411_blkorder(src, dst, YUV411_YUYVYY, width, height, unit);
}

int CYuv411::Yvyuyy2Blk(char *src, char *dst, int width, int height, int unit)
{
	return this->yuv411_blkorder(src, dst, YUV411_YVYUYY, width, height, unit);
}

int CYuv411::Uvyyyy2Blk(char *src, char *dst, int width, int height, int unit)
{
	return this->yuv411_blkorder(src, dst, YUV411_UVYYYY, width, height, unit);
}

int CYuv411::Vuyyyy2Blk(char *src, char *dst, int width, int height, int unit)
{
	return this->yuv411_blkorder(src, dst, YUV411_VUYYYY, width, height, unit);
}


int CYuv411::Rgb2Yuv411Blk(char *src, char *dst, int width, int height, int unit)
{
	int i, len, size_blk, size, cap;
	CBufferBase tmpBuf;

	cap = 256;
	if(unit == 2)
		cap = 1024;
	size = unit * 3 * 4;
	size_blk = width * height * unit * 3;
	tmpBuf.SetSize(size_blk);

	char * pBuf = tmpBuf.m_buf;
	
	for(i = 0; i < size_blk; i += size)
	{
		len = this->rgb2yuv411order(src, pBuf, YUV411_YYYYUV, cap, unit);
		pBuf += len; //Output data in block sequence is stored in tmpBuf.m_buf.
		src += size;
	}

	len = this->yuv411_blkorder(tmpBuf.m_buf, dst, YUV411_YYYYUV, width, height, unit);
	return len;
}

int CYuv411::Rgb2Jpeg411(char *src, char *dst, int width, int height, int unit, int qf)
{
	int j, len, size_blk, size, step;
	CBufferBase tmpBuf;

	if( (unit != 1) && (unit != 2) )
		unit = 1;

	//convert sequence to block order first:
	size = this->getVwc(width, height);
	tmpBuf.SetSize(size, unit);
	char * pBuf = tmpBuf.m_buf;
	size_blk = (width * unit * 3) << 3; //8 lines RGB24 data
			
	for(j = 0; j < height; j += 8)
	{
		step = ((height-j) > 8)? 8 : (height-j);
		len = this->Rgb2Yuv411Blk(src, pBuf, width, step, unit);
		pBuf += len;
		//size_blk = ((height-j) > 8)? size_blk : (width*unit*3)*(height - j);
		src += size_blk;
	}

	len = this->Blk2Jpeg(tmpBuf.m_buf, dst, width, height, qf);
	return len;
}

//===============================================================
//
//		yuv411 image resize
//
//==============================================================
int CYuv411::VuyyyyResize(char *src, char *dest, int srcw, int srch, int dstw, int dsth, int unit)
{
	return this->yuv411_resizeorder(src, dest, srcw, srch, dstw, dsth, YUV411_VUYYYY, unit);
}

int CYuv411::UvyyyyResize(char *src, char *dest, int srcw, int srch, int dstw, int dsth, int unit)
{
	return this->yuv411_resizeorder(src, dest, srcw, srch, dstw, dsth, YUV411_UVYYYY, unit);
}

int CYuv411::YuyvyyResize(char *src, char *dest, int srcw, int srch, int dstw, int dsth, int unit)
{
	return this->yuv411_resizeorder(src, dest, srcw, srch, dstw, dsth, YUV411_YUYVYY, unit);
}

int CYuv411::YvyuyyResize(char *src, char *dest, int srcw, int srch, int dstw, int dsth, int unit)
{
	return this->yuv411_resizeorder(src, dest, srcw, srch, dstw, dsth, YUV411_YVYUYY, unit);
}

int CYuv411::YyyyuvResize(char *src, char *dest, int srcw, int srch, int dstw, int dsth, int unit)
{
	return this->yuv411_resizeorder(src, dest, srcw, srch, dstw, dsth, YUV411_YYYYUV, unit);
}

int CYuv411::YyyyvuResize(char *src, char *dest, int srcw, int srch, int dstw, int dsth, int unit)
{
	return this->yuv411_resizeorder(src, dest, srcw, srch, dstw, dsth, YUV411_YYYYVU, unit);
}

//===============================================================
//
//		yuv411 image sizer
//
//==============================================================
int CYuv411::YyyyuvSizer(char *src, char *dest, int srcw, int dstw, int dsth, int px, int py, int unit)
{
	return this->MYuv411Sizer(src, dest, srcw, dstw, dsth, px, py, YUV411_YYYYUV, unit);
}

int CYuv411::YyyyvuSizer(char *src, char *dest, int srcw, int dstw, int dsth, int px, int py, int unit)
{
	return this->MYuv411Sizer(src, dest, srcw, dstw, dsth, px, py, YUV411_YYYYVU, unit);
}

int CYuv411::YuyvyySizer(char *src, char *dest, int srcw, int dstw, int dsth, int px, int py, int unit)
{
	return this->MYuv411Sizer(src, dest, srcw, dstw, dsth, px, py, YUV411_YUYVYY, unit);
}

int CYuv411::YvyuyySizer(char *src, char *dest, int srcw, int dstw, int dsth, int px, int py, int unit)
{
	return this->MYuv411Sizer(src, dest, srcw, dstw, dsth, px, py, YUV411_YVYUYY, unit);
}

int CYuv411::UvyyyySizer(char *src, char *dest, int srcw, int dstw, int dsth, int px, int py, int unit)
{
	return this->MYuv411Sizer(src, dest, srcw, dstw, dsth, px, py, YUV411_UVYYYY, unit);
}

int CYuv411::VuyyyySizer(char *src, char *dest, int srcw, int dstw, int dsth, int px, int py, int unit)
{
	return this->MYuv411Sizer(src, dest, srcw, dstw, dsth, px, py, YUV411_VUYYYY, unit);
}

//=======================================================
//
//		YUV411 encode
//
//=======================================================
int CYuv411::getVwc(int width, int height)
{
	width = ((width + 31) >> 5) << 5;
	height = ((height + 7) >> 3) << 3;
	width *= height * 3;
	width >>= 1;
	return width;
}

int CYuv411::Blk2Jpeg(char *src, char *dest, int width, int height, int qf)
{
	CJpegEncoder enc;
	int len;

	enc.SetJpegQf(qf);
	enc.SetJpegSize(width, height);
	enc.SetYuvFmt(YUV411BLK);
	enc.Open();
	len = enc.getVwc();
	enc.Write(src, len);
	len = enc.Read(dest, len);
	return len;
}





