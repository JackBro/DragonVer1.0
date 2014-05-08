
#include "internal.h"

//int CYuv420::MYuv420Sizer(char *src, char *dest, int srcw, int dstw, int dsth, 
//								 int px, int py, int order, int unit)
//{
//	
//	return 0;
//}

int CYuv420::yuv420_resizeorder(char *src, char *dst, int srcw, int srch, int dstw, int dsth, int order, int unit)
{
	this->MYuv420_444order(src, dst, srcw, srch, order, unit);
	CYuv444::Yuv444Resize(dst, src, srcw, srch, dstw, dsth, unit);
	return this->MYuv444_420order(src, dst, dstw, dsth, order, unit);
}

int CYuv420::MRgb2Yuv420order(char *src, char *dest, int width, int height, int order, int cap, int unit)
{
	int i, size, len = 0;

	size = width * unit * 6;
	height *= width * unit * 3;
	for(i = 0; i < height; i += size)
		len += this->rgb2yuv420order(src+i, dest+len, width, order, cap, unit);
	return len;
}

int CYuv420::MYuv420_Rgborder(char *src, char *dest, int width, int height, int order, int cap, int unit)
{
	int i, size, len = 0;

	size = width * unit * 3;
	height *= width * unit * 3;
	height >>= 1;
	for(i = 0; i < height; i += size)
		len += this->yuv420_rgborder(src+i, dest+len, width, order, cap, unit);
	return len;
}

int CYuv420::MYuv444_420order(char *src, char *dest, int width, int height, int order, int unit)
{
	int i, size, len = 0;

	size = width * unit * 6;
	height *= width * unit * 3;
	for(i = 0; i < height; i += size)
		len += this->yuv444_420order(src+i, dest+len, width, order, unit);
	return len;
}

int CYuv420::MYuv420_444order(char *src, char *dest, int width, int height, int order, int unit)
{
	int i, size, len = 0;

	size = width * unit * 3;
	height *= width * unit * 3;
	height >>= 1;
	for(i = 0; i < height; i += size)
		len += this->yuv420_444order(src+i, dest+len, width, order, unit);
	return len;
}

int CYuv420::rgb2yuv420order(char *src, char *dst, int width, int order, int cap, int unit)
{
	int i, j, step = unit * 3;
	int yuv[12], bgr[3];
	int gap1, gap2, offset;
	char *pbuf, *psrc = src;

	gap1 = width * 3 * unit;
	gap2 = (width << 1) * unit;
	for(i = 0, offset = 0; i < width; i += 2, psrc += step << 1, offset += unit << 2)
	{
		for(j = 0, pbuf = psrc; j < 6; j += 3, pbuf += step)
		{
			CYuv2Rgb::getbgr(pbuf, bgr, unit);
			CYuv2Rgb::rgb2yuv(bgr, yuv+j, cap);
		}
		for(j = 6, pbuf = psrc + gap1; j < 12; j += 3, pbuf += step)
		{
			CYuv2Rgb::getbgr(pbuf, bgr, unit);
			CYuv2Rgb::rgb2yuv(bgr, yuv+j, cap);
		}

		yuv[1] += yuv[4] + yuv[7] + yuv[10];
		yuv[2] += yuv[5] + yuv[8] + yuv[11];
		yuv[1] >>= 2;
		yuv[2] >>= 2;

		CYuv2Rgb::setyuv420(dst, yuv, order, offset, gap2, unit);
	}
	return width * 3 * unit;
}

int CYuv420::yuv420_rgborder(char *src, char *dst, int width, int order, int cap, int unit)
{
	int i, j, offset, gap1, gap2, step = unit * 3;
	int yuv[12], bgr[12];
	char *pdst = dst;

	gap1 = (width << 1) * unit;
	gap2 = (width * 3) * unit;
	for(i = 0, offset = 0; i < width; i += 2, offset += unit << 2)
	{
		CYuv2Rgb::getyuvfrom420(src, yuv, order, offset, gap1, unit);

		for(j = 0; j < 12; j += 3)
			CYuv2Rgb::yuv2rgb(yuv+j, bgr+j, cap);

		for(j = 0; j < 6; j += 3, pdst += step)
		{
			CYuv2Rgb::setbgr(pdst, bgr+j, unit);
			CYuv2Rgb::setbgr(pdst + gap2, bgr+6+j, unit);
		}
	}
	return width * unit * 6;
}

int CYuv420::yuv420_444order(char *src, char *dst, int width, int order, int unit)
{
	int i, j, offset, gap1, gap2, step = unit * 3;
	int yuv[12];
	char *pdst = dst;

	gap1 = (width << 1) * unit;
	gap2 = (width * 3) * unit;
	for(i = 0, offset = 0; i < width; i += 2, offset += unit << 2)
	{
		CYuv2Rgb::getyuvfrom420(src, yuv, order, offset, gap1, unit);

		for(j = 0; j < 6; j += 3, pdst += step)
		{
			CYuv2Rgb::setyuv444(pdst, yuv+j, unit);
			CYuv2Rgb::setyuv444(pdst + gap2, yuv+6+j, unit);
		}
	}
	return width * unit * 6;
}

int CYuv420::yuv444_420order(char *src, char *dst, int width, int order, int unit)
{
	int i, j, step = unit * 3;
	int yuv[12];
	int gap1, gap2, offset;
	char *psrc = src;

	gap1 = width * 3 * unit;
	gap2 = (width << 1) * unit;
	for(i = 0, offset = 0; i < width; i += 2, offset += unit << 2)
	{
		for(j = 0; j < 6; j += 3, psrc += step)
		{
			CYuv2Rgb::getyuv444(psrc, yuv+j, unit);
			CYuv2Rgb::getyuv444(psrc + gap1, yuv+6+j, unit);
		}

		yuv[1] += yuv[4] + yuv[7] + yuv[10];
		yuv[2] += yuv[5] + yuv[8] + yuv[11];
		yuv[1] >>= 2;
		yuv[2] >>= 2;

		CYuv2Rgb::setyuv420(dst, yuv, order, offset, gap2, unit);
	}
	return width * 3 * unit;
}

int CYuv420::yuv420_blkorder(char *src, char *dst, int order, int width, int height, int unit)
{
	int i, j, yoffset, uoffset, pos1, gap1, gap2;
	int yuv[12];
	int pos, blkwidth;
	char *psrc = src;

	gap1 = width * 3 * unit;
	gap2 = (width * unit) << 1;
	blkwidth = ((((width + 15) >> 4) * 3) << 7) * unit;
	for(i = 0; i < height; i += 2, psrc += gap1)
	{
		yoffset = ((i >> 4) * blkwidth) + (((i % 16) >> 3) << 7) * unit + ((i % 8) << 3) * unit;
		uoffset = ((i >> 4) * blkwidth) + (((i >> 1) % 8) << 3) * unit;
		pos1 = 0;
		for(j = 0; j < width; j += 2, pos1 += unit << 2)
		{
			CYuv2Rgb::getyuv420(psrc, yuv, order, pos1, gap2, unit);

			pos = (((j >> 4) * 384) + (((j % 16) >> 3) << 6) + (j % 8)) * unit;
			pos += yoffset;
			CBaseConvert::setvalue(dst+pos, yuv[0], unit);
			CBaseConvert::setvalue(dst+pos+unit, yuv[3], unit);
			CBaseConvert::setvalue(dst+pos+(unit << 3), yuv[6], unit);
			CBaseConvert::setvalue(dst+pos+(unit << 3) + unit, yuv[9], unit);

			pos = (((j >> 4) * 384) + 256 + ((j >> 1) % 8)) * unit;
			pos += uoffset;

			CBaseConvert::setvalue(dst+pos, yuv[1], unit);
			CBaseConvert::setvalue(dst+pos+(unit << 6), yuv[2], unit);
		}
	}
	return blkwidth * ((height + 15) >> 4);
}

int CYuv420::blk2yuv420order(char *src, char *dst, int order, int width, int height, int unit)
{
	int i, j, yoffset, uoffset, pos1, gap1, gap2;
	int yuv[12];
	int pos, blkwidth;
	char *pdst = dst;

	gap1 = width * 3 * unit;
	gap2 = (width * unit) << 1;
	blkwidth = ((((width + 15) >> 4) * 3) << 7) * unit;
	for(i = 0; i < height; i += 2, pdst += gap1)
	{
		yoffset = ((i >> 4) * blkwidth) + (((i % 16) >> 3) << 7) * unit + ((i % 8) << 3) * unit;
		uoffset = ((i >> 4) * blkwidth) + (((i >> 1) % 8) << 3) * unit;
		pos1 = 0;
		for(j = 0; j < width; j += 2, pos1 += unit << 2)
		{
			pos = (((j >> 4) * 384) + (((j % 16) >> 3) << 6) + (j % 8)) * unit;
			pos += yoffset;
			yuv[0] = CYuv2Rgb::getvalue(src+pos, unit);
			yuv[3] = CYuv2Rgb::getvalue(src+pos+unit, unit);
			yuv[6] = CYuv2Rgb::getvalue(src+pos+(unit << 3), unit);
			yuv[9] = CYuv2Rgb::getvalue(src+pos+(unit << 3)+unit, unit);

			pos = (((j >> 4) * 384) + 256 + ((j >> 1) % 8)) * unit;
			pos += uoffset;
			yuv[1] = CYuv2Rgb::getvalue(src+pos, unit);
			yuv[2] = CYuv2Rgb::getvalue(src+pos+(unit << 6), unit);
			CYuv2Rgb::setyuv420(pdst, yuv, order, pos1, gap2, unit);
		}
	}
	return blkwidth * ((height + 15) >> 4);
}

int CYuv420::yuv420_lbuforder(char *src, char *dst, int order, int width, int height, int unit)
{
	int i, j, k, ypos, upos, vpos;
	int yuv[12];
	int blkwidth, gap1, gap2, pos1;;
	char *psrc = src;

	blkwidth = ((((width + 15) >> 4) * 3) << 7) * unit;
	gap1 = width * 3 * unit;
	gap2 = (width * unit) << 1;
	for(i = 0; i < height; i += 16)
	{
		ypos = (i >> 4) * blkwidth;
		upos = ypos + ((blkwidth << 1) / 3);
		vpos = upos + (blkwidth / 6);
		k = ((height - i) < 16) ? (height - i) : 16;
		for(; k; k -= 2, psrc += gap1)
		{
			for(j = 0, pos1 = 0; j < width; j += 2, pos1 += unit << 2, ypos += unit << 1, upos += unit, vpos += unit)
			{
				CYuv2Rgb::getyuv420(psrc, yuv, order, pos1, gap2, unit);
				CBaseConvert::setvalue(dst + ypos, yuv[0], unit);
				CBaseConvert::setvalue(dst + ypos + unit, yuv[3], unit);
				CBaseConvert::setvalue(dst + ypos + (gap2 >> 1), yuv[6], unit);
				CBaseConvert::setvalue(dst + ypos + (gap2 >> 1) + unit, yuv[9], unit);
				CBaseConvert::setvalue(dst + upos, yuv[1], unit);
				CBaseConvert::setvalue(dst + vpos, yuv[2], unit);
			}
			ypos += gap2 >> 1;
		}
	}
	return blkwidth * ((height + 15) >> 4);
}

int CYuv420::lbuf2yuv420order(char *src, char *dst, int order, int width, int height, int unit)
{
	int i, j, k, ypos, upos, vpos;
	int yuv[12];
	int blkwidth, gap1, gap2, pos1;
	char *pdst = dst;

	blkwidth = ((((width + 15) >> 4) * 3) << 7) * unit;
	gap1 = width * 3 * unit;
	gap2 = (width * unit) << 1;
	for(i = 0; i < height; i += 16)
	{
		ypos = (i >> 4) * blkwidth;
		upos = ypos + ((blkwidth << 1) / 3);
		vpos = upos + (blkwidth / 6);
		k = ((height - i) < 16) ? (height - i) : 16;
		for(; k; k -= 2, pdst += gap1)
		{
			for(j = 0, pos1 = 0; j < width; j += 2, pos1 += unit << 2, ypos += unit << 1, upos += unit, vpos += unit)
			{
				yuv[0] = CBaseConvert::getvalue(src + ypos, unit);
				yuv[3] = CBaseConvert::getvalue(src + ypos + unit, unit);
				yuv[6] = CBaseConvert::getvalue(src + ypos + (gap2 >> 1), unit);
				yuv[9] = CBaseConvert::getvalue(src + ypos + (gap2 >> 1) + unit, unit);
				yuv[1] = CBaseConvert::getvalue(src + upos, unit);
				yuv[2] = CBaseConvert::getvalue(src + vpos, unit);
				CYuv2Rgb::setyuv420(pdst, yuv, order, pos1, gap2, unit);
			}
			ypos += gap2 >> 1;
		}
	}
	return blkwidth * ((height + 15) >> 4);
}

//===============================================================
//
//		rgb convert to yuv420
//
//==============================================================
int CYuv420::Rgb2Yyuv(char *src, char *dst, int width, int cap, int unit)
{
	return this->rgb2yuv420order(src, dst, width, YUV420_YYUV, cap, unit);
}

int CYuv420::Rgb2Yuyv(char *src, char *dst, int width, int cap, int unit)
{
	return this->rgb2yuv420order(src, dst, width, YUV420_YUYV, cap, unit);
}

int CYuv420::Rgb2Uyvy(char *src, char *dst, int width, int cap, int unit)
{
	return this->rgb2yuv420order(src, dst, width, YUV420_UYVY, cap, unit);
}

int CYuv420::Rgb2Vyuy(char *src, char *dst, int width, int cap, int unit)
{
	return this->rgb2yuv420order(src, dst, width, YUV420_VYUY, cap, unit);
}

int CYuv420::Rgb2Yvyu(char *src, char *dst, int width, int cap, int unit)
{
	return this->rgb2yuv420order(src, dst, width, YUV420_YVYU, cap, unit);
}

int CYuv420::Rgb2Yuvy(char *src, char *dst, int width, int cap, int unit)
{
	return this->rgb2yuv420order(src, dst, width, YUV420_YUVY, cap, unit);
}

int CYuv420::Rgb2Yvuy(char *src, char *dst, int width, int cap, int unit)
{
	return this->rgb2yuv420order(src, dst, width, YUV420_YVUY, cap, unit);
}

int CYuv420::Rgb2Uvyy(char *src, char *dst, int width, int cap, int unit)
{
	return this->rgb2yuv420order(src, dst, width, YUV420_UVYY, cap, unit);
}

int CYuv420::Rgb2Vuyy(char *src, char *dst, int width, int cap, int unit)
{
	return this->rgb2yuv420order(src, dst, width, YUV420_VUYY, cap, unit);
}

int CYuv420::Rgb2Yyvu(char *src, char *dst, int width, int cap, int unit)
{
	return this->rgb2yuv420order(src, dst, width, YUV420_YYVU, cap, unit);
}

//===============================================================
//
//		yuv420 convert to rgb
//
//==============================================================
int CYuv420::Yyuv2Rgb(char *src, char *dst, int width, int cap, int unit)
{
	return this->yuv420_rgborder(src, dst, width, YUV420_YYUV, cap, unit);
}

int CYuv420::Yuyv2Rgb(char *src, char *dst, int width, int cap, int unit)
{
	return this->yuv420_rgborder(src, dst, width, YUV420_YUYV, cap, unit);
}

int CYuv420::Uyvy2Rgb(char *src, char *dst, int width, int cap, int unit)
{
	return this->yuv420_rgborder(src, dst, width, YUV420_UYVY, cap, unit);
}

int CYuv420::Vyuy2Rgb(char *src, char *dst, int width, int cap, int unit)
{
	return this->yuv420_rgborder(src, dst, width, YUV420_VYUY, cap, unit);
}

int CYuv420::Yvyu2Rgb(char *src, char *dst, int width, int cap, int unit)
{
	return this->yuv420_rgborder(src, dst, width, YUV420_YVYU, cap, unit);
}

int CYuv420::Yuvy2Rgb(char *src, char *dst, int width, int cap, int unit)
{
	return this->yuv420_rgborder(src, dst, width, YUV420_YUVY, cap, unit);
}

int CYuv420::Yvuy2Rgb(char *src, char *dst, int width, int cap, int unit)
{
	return this->yuv420_rgborder(src, dst, width, YUV420_YVUY, cap, unit);
}

int CYuv420::Uvyy2Rgb(char *src, char *dst, int width, int cap, int unit)
{
	return this->yuv420_rgborder(src, dst, width, YUV420_UVYY, cap, unit);
}

int CYuv420::Vuyy2Rgb(char *src, char *dst, int width, int cap, int unit)
{
	return this->yuv420_rgborder(src, dst, width, YUV420_VUYY, cap, unit);
}

int CYuv420::Yyvu2Rgb(char *src, char *dst, int width, int cap, int unit)
{
	return this->yuv420_rgborder(src, dst, width, YUV420_YYVU, cap, unit);
}

//===============================================================
//
//		yuv420 convert to yuv444
//
//==============================================================
int CYuv420::Yyuv2Yuv444(char *src, char *dst, int width, int unit)
{
	return this->yuv420_444order(src, dst, width, YUV420_YYUV, unit);
}

int CYuv420::Yuyv2Yuv444(char *src, char *dst, int width, int unit)
{
	return this->yuv420_444order(src, dst, width, YUV420_YUYV, unit);
}

int CYuv420::Uyvy2Yuv444(char *src, char *dst, int width, int unit)
{
	return this->yuv420_444order(src, dst, width, YUV420_UYVY, unit);
}

int CYuv420::Vyuy2Yuv444(char *src, char *dst, int width, int unit)
{
	return this->yuv420_444order(src, dst, width, YUV420_VYUY, unit);
}

int CYuv420::Yvyu2Yuv444(char *src, char *dst, int width, int unit)
{
	return this->yuv420_444order(src, dst, width, YUV420_YVYU, unit);
}

int CYuv420::Yuvy2Yuv444(char *src, char *dst, int width, int unit)
{
	return this->yuv420_444order(src, dst, width, YUV420_YUVY, unit);
}

int CYuv420::Yvuy2Yuv444(char *src, char *dst, int width, int unit)
{
	return this->yuv420_444order(src, dst, width, YUV420_YVUY, unit);
}

int CYuv420::Uvyy2Yuv444(char *src, char *dst, int width, int unit)
{
	return this->yuv420_444order(src, dst, width, YUV420_UVYY, unit);
}

int CYuv420::Vuyy2Yuv444(char *src, char *dst, int width, int unit)
{
	return this->yuv420_444order(src, dst, width, YUV420_VUYY, unit);
}

int CYuv420::Yyvu2Yuv444(char *src, char *dst, int width, int unit)
{
	return this->yuv420_444order(src, dst, width, YUV420_YYVU, unit);
}

//===============================================================
//
//		yuv444 convert to yuv420
//
//==============================================================
int CYuv420::Yuv444_Yyuv(char *src, char *dst, int width, int unit)
{
	return this->yuv444_420order(src, dst, width, YUV420_YYUV, unit);
}

int CYuv420::Yuv444_Yuyv(char *src, char *dst, int width, int unit)
{
	return this->yuv444_420order(src, dst, width, YUV420_YUYV, unit);
}

int CYuv420::Yuv444_Uyvy(char *src, char *dst, int width, int unit)
{
	return this->yuv444_420order(src, dst, width, YUV420_UYVY, unit);
}

int CYuv420::Yuv444_Vyuy(char *src, char *dst, int width, int unit)
{
	return this->yuv444_420order(src, dst, width, YUV420_VYUY, unit);
}

int CYuv420::Yuv444_Yvyu(char *src, char *dst, int width, int unit)
{
	return this->yuv444_420order(src, dst, width, YUV420_YVYU, unit);
}

int CYuv420::Yuv444_Yuvy(char *src, char *dst, int width, int unit)
{
	return this->yuv444_420order(src, dst, width, YUV420_YUVY, unit);
}

int CYuv420::Yuv444_Yvuy(char *src, char *dst, int width, int unit)
{
	return this->yuv444_420order(src, dst, width, YUV420_YVUY, unit);
}

int CYuv420::Yuv444_Uvyy(char *src, char *dst, int width, int unit)
{
	return this->yuv444_420order(src, dst, width, YUV420_UVYY, unit);
}

int CYuv420::Yuv444_Vuyy(char *src, char *dst, int width, int unit)
{
	return this->yuv444_420order(src, dst, width, YUV420_VUYY, unit);
}

int CYuv420::Yuv444_Yyvu(char *src, char *dst, int width, int unit)
{
	return this->yuv444_420order(src, dst, width, YUV420_YYVU, unit);
}

//===============================================================
//
//		yuv420 convert to block
//
//==============================================================
int CYuv420::Yyuv2Blk(char *src, char *dst, int width, int height, int unit)
{
	return this->yuv420_blkorder(src, dst, YUV420_YYUV, width, height, unit);
}

int CYuv420::Yuyv2Blk(char *src, char *dst, int width, int height, int unit)
{
	return this->yuv420_blkorder(src, dst, YUV420_YUYV, width, height, unit);
}

int CYuv420::Uyvy2Blk(char *src, char *dst, int width, int height, int unit)
{
	return this->yuv420_blkorder(src, dst, YUV420_UYVY, width, height, unit);
}

int CYuv420::Vyuy2Blk(char *src, char *dst, int width, int height, int unit)
{
	return this->yuv420_blkorder(src, dst, YUV420_VYUY, width, height, unit);
}

int CYuv420::Yvyu2Blk(char *src, char *dst, int width, int height, int unit)
{
	return this->yuv420_blkorder(src, dst, YUV420_YVYU, width, height, unit);
}

int CYuv420::Yuvy2Blk(char *src, char *dst, int width, int height, int unit)
{
	return this->yuv420_blkorder(src, dst, YUV420_YUVY, width, height, unit);
}

int CYuv420::Yvuy2Blk(char *src, char *dst, int width, int height, int unit)
{
	return this->yuv420_blkorder(src, dst, YUV420_YVUY, width, height, unit);
}

int CYuv420::Uvyy2Blk(char *src, char *dst, int width, int height, int unit)
{
	return this->yuv420_blkorder(src, dst, YUV420_UVYY, width, height, unit);
}

int CYuv420::Vuyy2Blk(char *src, char *dst, int width, int height, int unit)
{
	return this->yuv420_blkorder(src, dst, YUV420_VUYY, width, height, unit);
}

int CYuv420::Yyvu2Blk(char *src, char *dst, int width, int height, int unit)
{
	return this->yuv420_blkorder(src, dst, YUV420_YYVU, width, height, unit);
}

int CYuv420::Rgb2Yuv420Blk(char *src, char *dst, int width, int height, int unit)
{
	int i, len, size, cap;
	CBufferBase srcbuf, dstbuf, tmpbuf;
	char * pbuf = tmpbuf.m_buf;

	cap = 256;
	if(unit == 2)
		cap = 1024;
	tmpbuf.SetSize(width*height*3 >> 1, unit);
	size = width * unit * 6;
	for(i = 0; i < height; i+=2)
	{
		len = this->rgb2yuv420order(src, pbuf, width, YUV420_UVYY, cap, unit);
		pbuf += len;
		src += size;
	}

	len = this->yuv420_blkorder(tmpbuf.m_buf, dst, YUV420_UVYY, width, 16, unit);
	return len;
}

//=======================================================
//
//		Block convert to YUV420 format
//
//=======================================================

int CYuv420::Blk2Yyuv(char *src, char *dst, int width, int height, int unit)
{
	return this->blk2yuv420order(src, dst, YUV420_YYUV, width, height, unit);
}

int CYuv420::Blk2Yuyv(char *src, char *dst, int width, int height, int unit)
{
	return this->blk2yuv420order(src, dst, YUV420_YUYV, width, height, unit);
}

int CYuv420::Blk2Uyvy(char *src, char *dst, int width, int height, int unit)
{
	return this->blk2yuv420order(src, dst, YUV420_UYVY, width, height, unit);
}

int CYuv420::Blk2Vyuy(char *src, char *dst, int width, int height, int unit)
{
	return this->blk2yuv420order(src, dst, YUV420_VYUY, width, height, unit);
}

int CYuv420::Blk2Yvyu(char *src, char *dst, int width, int height, int unit)
{
	return this->blk2yuv420order(src, dst, YUV420_YVYU, width, height, unit);
}

int CYuv420::Blk2Yuvy(char *src, char *dst, int width, int height, int unit)
{
	return this->blk2yuv420order(src, dst, YUV420_YUVY, width, height, unit);
}

int CYuv420::Blk2Yvuy(char *src, char *dst, int width, int height, int unit)
{
	return this->blk2yuv420order(src, dst, YUV420_YVUY, width, height, unit);
}

int CYuv420::Blk2Uvyy(char *src, char *dst, int width, int height, int unit)
{
	return this->blk2yuv420order(src, dst, YUV420_UVYY, width, height, unit);
}

int CYuv420::Blk2Vuyy(char *src, char *dst, int width, int height, int unit)
{
	return this->blk2yuv420order(src, dst, YUV420_VUYY, width, height, unit);
}

int CYuv420::Blk2Yyvu(char *src, char *dst, int width, int height, int unit)
{
	return this->blk2yuv420order(src, dst, YUV420_YYVU, width, height, unit);
}

//=======================================================
//
//		YUV420 encode
//
//=======================================================
int CYuv420::getVwc(int width, int height)
{
	width = ((width + 15) >> 4) << 4;
	height = ((height + 15) >> 4) << 4;
	width *= height * 3;
	width >>= 1;
	return width;
}


int CYuv420::Blk2Rgb24(char *src, char *dest, int width, int height)
{
	int len, size, i, cap, msize, ncount = 0;
	char* psrcbuf, *pdstbuf, *ptmpbuf;
	CBlk2YuvBuf blkbuf((char)YUV420_UVYY, width, 1);	
	CBufferBase tmpbuf;

	size = msize = (height * width) * 3 >> 1;
	tmpbuf.SetSize(msize);
	
	//YuvBlk to YUV420_UVYY
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

	//YUV420_UVYY to Rgb24: srcbuf used as destination & dstbuf used as source, now.
	ptmpbuf = tmpbuf.m_buf;
	pdstbuf = dest;	
	cap = 256;
	size = width * 3;
	
	for(i = 0; i < height; i+=2)
	{
		len = CYuv420::yuv420_rgborder(ptmpbuf, pdstbuf, width, YUV420_UVYY, cap, 1);
		pdstbuf += len;
		ncount += len;
		ptmpbuf += size;		
	}

	return ncount;
}

int CYuv420::Blk2Jpeg(char *src, char *dest, int width, int height, int qf)
{
	CJpegEncoder enc;
	int len;

	enc.SetJpegQf(qf);
	enc.SetJpegSize(width, height);
	enc.SetYuvFmt(YUV420BLK);
	enc.Open();
	len = enc.getVwc();
	enc.Write(src, len);
	len = enc.Read(dest, len);
	return len;
}

int CYuv420::Rgb2Jpeg420(char *src, char *dest, int width, int height, int unit, int qf)
{
	int j, len, size, step;
	CBufferBase tmpbuf;

	int blk_size = this->getVwc(width, height);
	tmpbuf.SetSize(blk_size, unit);
	char *pbuf = tmpbuf.m_buf;
	
	size = ((width * unit) * 3) << 4;	
	for(j = 0; j < height; j += 16)
	{
		step = (16 < (height - j))? 16 : (height-j);
		len = this->Rgb2Yuv420Blk(src, pbuf, width, step, unit);
		pbuf += len;
		//size = (16 < (height - j))? size : (width*unit*3)*(height-j);
		src += size;
	}

	len =  this->Blk2Jpeg(tmpbuf.m_buf, dest, width, height, qf);
	return len;
}
	
int CYuv420::Blk2Mpeg2I(char *src, char *dest, int width, int height)
{
	CMpeg2IEncoder enc;
	int len;

	enc.Open();
	enc.SetImageSize(width, height);
	enc.SetYuvFmt(YUV420BLK);
	len = enc.getVwc();
	enc.Write(src, len);
	len = enc.Read(dest, len);
	return len;
}

//=======================================================
//
//		YUV420 to frame
//
//=======================================================
int CYuv420::MYuv420_FrmOrder(char *src, char *dst, int width, int height, int order, int unit)
{
	int i, j, ypos, upos, vpos, offset, step, gap;
	int yuv[12];
	char *psrc = src;

	ypos = 0;
	upos = width * height * unit;
	vpos = upos + (upos >> 2);
	step = width * unit;
	gap = step * 3;
	for(i = 0; i < height; i += 2, psrc += gap, ypos += step)
	{
		offset = 0;
		for(j = 0; j < width; j += 2, offset += unit << 2, ypos += unit << 1, upos++, vpos++)
		{
			CYuv2Rgb::getyuv420(psrc, yuv, order, offset, step << 1, unit);
			CBaseConvert::setvalue(dst + ypos, yuv, 1, unit);
			CBaseConvert::setvalue(dst + ypos + 1, yuv+3, 1, unit);
			CBaseConvert::setvalue(dst + ypos + step, yuv+6, 1, unit);
			CBaseConvert::setvalue(dst + ypos + step + 1, yuv+9, 1, unit);
			CBaseConvert::setvalue(dst + upos, yuv+1, 1, unit);
			CBaseConvert::setvalue(dst + vpos, yuv+2, 1, unit);
		}
	}

	return vpos;
}

int CYuv420::MFrm2Yuv420Order(char *src, char *dst, int width, int height, int order, int unit)
{
	int i, j, ypos, upos, vpos, offset, step, gap;
	int yuv[12];
	char *pdst = dst;

	ypos = 0;
	upos = width * height * unit;
	vpos = upos + (upos >> 2);
	step = width * unit;
	gap = step * 3;
	for(i = 0; i < height; i += 2, pdst += gap, ypos += step)
	{
		offset = 0;
		for(j = 0; j < width; j += 2, offset += unit << 2, ypos += unit << 1, upos++, vpos++)
		{
			yuv[0] = CBaseConvert::getvalue(src + ypos, unit);
			yuv[3] = CBaseConvert::getvalue(src + ypos + 1, unit);
			yuv[6] = CBaseConvert::getvalue(src + ypos + step, unit);
			yuv[9] = CBaseConvert::getvalue(src + ypos + step + 1, unit);
			yuv[1] = CBaseConvert::getvalue(src + upos, unit);
			yuv[2] = CBaseConvert::getvalue(src + vpos, unit);
			CYuv2Rgb::setyuv420(pdst, yuv, order, offset, step << 1, unit);
		}
	}

	return vpos;
}


