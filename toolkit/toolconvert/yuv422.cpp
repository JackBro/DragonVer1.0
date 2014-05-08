
#include "internal.h"

int CYuv422::yuv422_resizeorder(char *src, char *dst, int srcw, int srch, int dstw, int dsth, int order, int unit)
{
//	return Yuv422_Sizer(src, dst, srcw, srch, dstw, dsth);
	this->MYuv422_444order(src, dst, srcw, srch, order, unit);
	CYuv444::Yuv444Resize(dst, src, srcw, srch, dstw, dsth, unit);
	return this->MYuv444_422order(src, dst, dstw, dsth, order, unit);
}

int CYuv422::MYuv422Sizer(char *src, char *dest, int srcw, int dstw, int dsth, 
								 int px, int py, int order, int unit)
{
	int i, j, size, size1, offset, step = unit << 2, step1 = unit * 6;
	char *psrc, *pdst, *psrc1, *pdst1;
	CBufferBase pool;

	size = (srcw * unit) << 1;
	size1 = (dstw * unit) << 1;
	offset = (px * unit) << 1;
	psrc = src + size * py;
	pdst = dest;

	for(i = 0; i < dsth; i++, psrc += size)
	{
		if(px % 2)
		{
			psrc1 = psrc;
			pdst1 = pool.m_buf;
			for(j = 0; j < srcw; j += 2, psrc1 += step, pdst1 += step1)
				this->yuv422_444order(psrc1, pdst1, order, unit);
			psrc1 = pdst + (px * unit * 3);
			for(j = 0; j < dstw; j += 2, psrc1 += step1, pdst += step)
				this->yuv444_422order(psrc1, pdst, order, unit);
		}
		else
			memcpy(pdst, psrc + offset, size1);
	}
	return size1 * py;
}

int CYuv422::MRgb2Yuv422order(char *src, char *dest, int width, int height, int order, int cap, int unit)
{
	int i, size;

	size = unit * 3 * 2;
	height *= width * unit * 3;
	width = 0;
	for(i = 0; i < height; i += size)
		width += this->rgb2yuv422order(src + i, dest + width, order, cap, unit);
	return width;
}

int CYuv422::MYuv422_Rgborder(char *src, char *dest, int width, int height, int order, int cap, int unit)
{
	int i, size;

	size = unit << 2;
	height *= (width * unit) << 1;
	width = 0;
	for(i = 0; i < height; i += size)
		width += this->yuv422_rgborder(src + i, dest + width, order, cap, unit);
	return width;
}

int CYuv422::MYuv444_422order(char *src, char *dest, int width, int height, int order, int unit)
{
	int i, size;

	size = unit * 3 * 2;
	height *= width * unit * 3;
	width = 0;
	for(i = 0; i < height; i += size)
		width += this->yuv444_422order(src + i, dest + width, order, unit);
	return width;
}

int CYuv422::MYuv422_444order(char *src, char *dest, int width, int height, int order, int unit)
{
	int i, size;

	size = unit << 2;
	height *= (width * unit) << 1;
	width = 0;
	for(i = 0; i < height; i += size)
		width += this->yuv422_444order(src + i, dest + width, order, unit);
	return width;
}

int CYuv422::rgb2yuv422order(char *src, char *dst, int order, int cap, int unit)
{
	int i, step = unit * 3;
	int bgr[3], yuv[6];
	char *psrc = src;

	for(i = 0; i < 6; i += 3, psrc += step)
	{
		CYuv2Rgb::getbgr(psrc, bgr, unit);
		CYuv2Rgb::rgb2yuv(bgr, yuv+i, cap);
	}
	yuv[1] = (yuv[1] + yuv[4]) >> 1;
	yuv[2] = (yuv[2] + yuv[5]) >> 1;

	//yuv format data are all one byte width(eg. unit == 1):
	//unit = 1;
	CYuv2Rgb::setyuv422(dst, yuv, order, unit);

	return unit << 2;
}

int CYuv422::Rgb2Yuv422Blk(char * src, char *dst, int width, int blkHeight, int unit)
{
	//convert rgb24data in srcbuf.m_buf into yuv422:
	int size_blk = width * blkHeight; //blkHeight lines' RGB24 data  
	char *tmpBuf = new char[unit * 3 * size_blk];
	char *pBuf = tmpBuf;			
	int i, len, cap = 256;
	if(unit == 2)
		cap = 1024;
	int size422 = unit * 3 * 2;

	for (i = 0; i < size_blk; i += 2)
	{
		len = this->rgb2yuv422order(src, pBuf, YUV422_UVYY, cap, unit);
		pBuf += len;		
		src += size422;
	}

	//convert yuv422data to block sequence:
	if ( (unit != 1) && (unit != 2) )
		unit = 1; 
	len = this->yuv422_blkorder(tmpBuf, dst, YUV422_UVYY, width, blkHeight, unit);
	delete []tmpBuf;
	tmpBuf = NULL;
	return len;
}

int CYuv422::Rgb2Jpeg422(char *src, char *dst, int width, int height, int qfact, int unit)
{
	//convert to block sequence first:
	int j, len, step, size_blk = (width * unit * 3) << 3; //8 lines RGB24 data
	CBufferBase tmpBuf;
	int size = this->getVwc(width, height);
	tmpBuf.SetSize(size, unit);
	char * pBuf = tmpBuf.m_buf;
	int qtmp;				//next qfactor
	
	for(j = 0; j < height; j += 8)
	{
		step =  ((height - j) > 8)? 8 : (height-j);
		len = this->Rgb2Yuv422Blk(src, pBuf, width, step, unit);
		pBuf += len;
		//size_blk = ((height - j) > 8)? size_blk : (width*unit*3)*(height - j);
		src += size_blk;
	}

	len = this->Blk2Jpeg(tmpBuf.m_buf, dst, width, height, &qtmp, qfact);		//modify by mfbao
	return len;	
}

int CYuv422::yuv422_rgborder(char *src, char *dst, int order, int cap, int unit)
{
	int i, step = unit * 3;
	char *pdst = dst;
	int yuv[6], bgr[3];

	CYuv2Rgb::getyuvfrom422(src, yuv, order, unit);

	for(i = 0; i < 6; i += 3, pdst += step)
	{
		CYuv2Rgb::yuv2rgb(yuv + i, bgr, cap);
		CYuv2Rgb::setbgr(pdst, bgr, unit);;
	}
	return unit * 6;
}

int CYuv422::yuv444_422order(char *src, char *dst, int order, int unit)
{
	int i, step = unit * 3;
	int yuv[6];
	char *psrc = src;

	for(i = 0; i < 6; i += 3, psrc += step)
	{
		CYuv2Rgb::getyuv444(psrc, yuv+i, unit);
	}
	yuv[1] = (yuv[1] + yuv[4]) >> 1;
	yuv[2] = (yuv[2] + yuv[5]) >> 1;

	CYuv2Rgb::setyuv422(dst, yuv, order, unit);

	return unit << 2;
}

int CYuv422::yuv422_444order(char *src, char *dst, int order, int unit)
{
	int i, step = unit * 3;
	char *pdst = dst;
	int yuv[6];

	CYuv2Rgb::getyuvfrom422(src, yuv, order, unit);

	for(i = 0; i < 6; i += 3, pdst += step)
	{
		CYuv2Rgb::setyuv444(pdst, yuv+i, unit);;
	}
	return unit * 6;
}

int CYuv422::yuv422_blkorder(char *src, char *dst, int order, int width, int height, int unit)
{
	int i, j, offset;
	int yuv[4];
	int pos, blkwidth;
	char *psrc = src;

	blkwidth = (((width + 15) >> 4) << 8) * unit;
	for(i = 0; i < height; i++)
	{
		offset = ((i >> 3) * blkwidth) + ((i % 8) << 3) * unit;
		for(j = 0; j < width; j += 2, psrc += unit << 2)
		{
			CYuv2Rgb::getyuv422(psrc, yuv, order, unit);

			pos = (((j >> 4) << 8) + (((j % 16) >> 3) << 6) + (j % 8)) * unit;
			pos += offset;
			CBaseConvert::setvalue(dst+pos, yuv[0], unit);
			CBaseConvert::setvalue(dst+pos+unit, yuv[3], unit);

			pos = (((j >> 4) << 8) + 128 + ((j % 16) >> 1)) * unit;
			pos += offset;

			CBaseConvert::setvalue(dst+pos, yuv[1], unit);
			CBaseConvert::setvalue(dst+pos+(unit << 6), yuv[2], unit);
		}
	}
	return blkwidth * ((height + 7) >> 3);
}

int CYuv422::blk2yuv422order(char *src, char *dst, int order, int width, int height, int unit)
{
	int i, j, offset;
	int yuv[4];
	int pos, blkwidth;
	char *pdst = dst;

	blkwidth = (((width + 15) >> 4) << 8) * unit;
	for(i = 0; i < height; i++)
	{
		offset = ((i >> 3) * blkwidth) + ((i % 8) << 3) * unit;
		for(j = 0; j < width; j += 2, pdst += unit << 2)
		{
			pos = (((j >> 4) << 8) + (((j % 16) >> 3) << 6) + (j % 8)) * unit;
			pos += offset;
			yuv[0] = CYuv2Rgb::getvalue(src+pos, unit);
			yuv[3] = CYuv2Rgb::getvalue(src+pos+unit, unit);
			pos = (((j >> 4) << 8) + 128 + ((j % 16) >> 1)) * unit;
			pos += offset;
			yuv[1] = CYuv2Rgb::getvalue(src+pos, unit);
			yuv[2] = CYuv2Rgb::getvalue(src+pos+(unit << 6), unit);

			CYuv2Rgb::setyuv422(pdst, yuv, order, unit);
		}
	}
	return blkwidth * ((height + 7) >> 3);
}

int CYuv422::yuv422_lbuforder(char *src, char *dst, int order, int width, int height, int unit)
{
	int i, j, k, ypos, upos, vpos;
	int yuv[4];
	int blkwidth;
	char *psrc = src;

	blkwidth = (((width + 15) >> 4) << 8) * unit;
	for(i = 0; i < height; i += 8)
	{
		ypos = (i >> 3) * blkwidth;
		upos = ypos + (blkwidth >> 1);
		vpos = upos + (blkwidth >> 2);
		k = ((height - i) < 8) ? (height - i) : 8;
		for(; k; k--)
		{
			for(j = 0; j < width; j += 2, psrc += unit << 2, ypos += unit << 1, upos += unit, vpos += unit)
			{
				CYuv2Rgb::getyuv422(psrc, yuv, order, unit);
				CBaseConvert::setvalue(dst+ypos, yuv[0], unit);
				CBaseConvert::setvalue(dst+ypos+unit, yuv[3], unit);
				CBaseConvert::setvalue(dst+upos, yuv[1], unit);
				CBaseConvert::setvalue(dst+vpos, yuv[2], unit);
			}
		}
	}
	return blkwidth * ((height + 7) >> 3);
}

int CYuv422::lbuf2yuv422order(char *src, char *dst, int order, int width, int height, int unit)
{
	int i, j, k, ypos, upos, vpos;
	int yuv[4];
	int blkwidth;
	char *pdst = dst;

	blkwidth = (((width + 15) >> 4) << 8) * unit;
	for(i = 0; i < height; i += 8)
	{
		ypos = (i >> 3) * blkwidth;
		upos = ypos + (blkwidth >> 1);
		vpos = upos + (blkwidth >> 2);
		k = ((height - i) < 8) ? (height - i) : 8;
		for(; k; k--)
		{
			for(j = 0; j < width; j += 2, pdst += unit << 2, ypos += unit << 1, upos += unit, vpos += unit)
			{
				yuv[0] = CBaseConvert::getvalue(src+ypos, unit);
				yuv[3] = CBaseConvert::getvalue(src+ypos+unit, unit);
				yuv[1] = CBaseConvert::getvalue(src+upos, unit);
				yuv[2] = CBaseConvert::getvalue(src+vpos, unit);
				CYuv2Rgb::setyuv422(pdst, yuv, order, unit);
			}
		}
	}
	return blkwidth * ((height + 7) >> 3);
}

int CYuv422::lbuf4442yuv422order(char *src, char *dst, int order, int width, int height, int unit)
{
	int i, j, k, ypos, upos, vpos;
	int yuv[4];
	int blkwidth;
	char *pdst = dst;

	blkwidth = (((width + 7) >> 3) << 7) * unit;
	for(i = 0; i < height; i += 8)
	{
		ypos = (i >> 3) * blkwidth;
		upos = ypos + (blkwidth >> 1);
		vpos = upos + (blkwidth >> 2);
		k = ((height - i) < 8) ? (height - i) : 8;
		for(; k; k--)
		{
			for(j = 0; j < width; j += 2, pdst += unit << 2, ypos += unit << 1, upos += unit, vpos += unit)
			{
				yuv[0] = CBaseConvert::getvalue(src+ypos, unit);
				yuv[3] = CBaseConvert::getvalue(src+ypos+unit, unit);
				yuv[1] = CBaseConvert::getvalue(src+upos, unit);
				yuv[2] = CBaseConvert::getvalue(src+vpos, unit);
				CYuv2Rgb::setyuv422(pdst, yuv, order, unit);
			}
		}
	}
	return blkwidth * ((height + 7) >> 3);
}

//======================= global interface ======================
//
//			RGB convert to different YUV422 format
//
//=========================================

int CYuv422::Rgb2Yyuv(char *src, char *dst, int cap, int unit)
{
	return this->rgb2yuv422order(src, dst, YUV422_YYUV, cap, unit);
}

int CYuv422::Rgb2Yuyv(char *src, char *dst, int cap, int unit)
{
	return this->rgb2yuv422order(src, dst, YUV422_YUYV, cap, unit);
}

int CYuv422::Rgb2Uyvy(char *src, char *dst, int cap, int unit)
{
	return this->rgb2yuv422order(src, dst, YUV422_UYVY, cap, unit);
}

int CYuv422::Rgb2Vyuy(char *src, char *dst, int cap, int unit)
{
	return this->rgb2yuv422order(src, dst, YUV422_VYUY, cap, unit);
}

int CYuv422::Rgb2Yvyu(char *src, char *dst, int cap, int unit)
{
	return this->rgb2yuv422order(src, dst, YUV422_YVYU, cap, unit);
}

int CYuv422::Rgb2Yuvy(char *src, char *dst, int cap, int unit)
{
	return this->rgb2yuv422order(src, dst, YUV422_YUVY, cap, unit);
}

int CYuv422::Rgb2Yvuy(char *src, char *dst, int cap, int unit)
{
	return this->rgb2yuv422order(src, dst, YUV422_YVUY, cap, unit);
}

int CYuv422::Rgb2Uvyy(char *src, char *dst, int cap, int unit)
{
	return this->rgb2yuv422order(src, dst, YUV422_UVYY, cap, unit);
}

int CYuv422::Rgb2Vuyy(char *src, char *dst, int cap, int unit)
{
	return this->rgb2yuv422order(src, dst, YUV422_VUYY, cap, unit);
}

int CYuv422::Rgb2Yyvu(char *src, char *dst, int cap, int unit)
{
	return this->rgb2yuv422order(src, dst, YUV422_YYVU, cap, unit);
}


//=======================================================
//
//		YUV422 format convert to RGB
//
//=======================================================
int CYuv422::Yyuv2Rgb(char *src, char *dst, int cap, int unit)
{
	return yuv422_rgborder(src, dst, YUV422_YYUV, cap, unit);
}

int CYuv422::Vyuy2Rgb(char *src, char *dst, int cap, int unit)
{
	return yuv422_rgborder(src, dst, YUV422_VYUY, cap, unit);
}

int CYuv422::Yuyv2Rgb(char *src, char *dst, int cap, int unit)
{
	return yuv422_rgborder(src, dst, YUV422_YUYV, cap, unit);
}

int CYuv422::Uyvy2Rgb(char *src, char *dst, int cap, int unit)
{
	return yuv422_rgborder(src, dst, YUV422_UYVY, cap, unit);
}

int CYuv422::Yvyu2Rgb(char *src, char *dst, int cap, int unit)
{
	return yuv422_rgborder(src, dst, YUV422_YVYU, cap, unit);
}

int CYuv422::Yuvy2Rgb(char *src, char *dst, int cap, int unit)
{
	return yuv422_rgborder(src, dst, YUV422_YUVY, cap, unit);
}

int CYuv422::Yvuy2Rgb(char *src, char *dst, int cap, int unit)
{
	return yuv422_rgborder(src, dst, YUV422_YVUY, cap, unit);
}

int CYuv422::Uvyy2Rgb(char *src, char *dst, int cap, int unit)
{
	return yuv422_rgborder(src, dst, YUV422_UVYY, cap, unit);
}

int CYuv422::Vuyy2Rgb(char *src, char *dst, int cap, int unit)
{
	return yuv422_rgborder(src, dst, YUV422_VUYY, cap, unit);
}

int CYuv422::Yyvu2Rgb(char *src, char *dst, int cap, int unit)
{
	return yuv422_rgborder(src, dst, YUV422_YYVU, cap, unit);
}

//=======================================================
//
//		YUV422 format convert to block
//
//=======================================================
int CYuv422::Yyuv2Blk(char *src, char *dst, int width, int height, int unit)
{
	return this->yuv422_blkorder(src, dst, YUV422_YYUV, width, height, unit);
}

int CYuv422::Yuyv2Blk(char *src, char *dst, int width, int height, int unit)
{
	return this->yuv422_blkorder(src, dst, YUV422_YUYV, width, height, unit);
}

int CYuv422::Uyvy2Blk(char *src, char *dst, int width, int height, int unit)
{
	return this->yuv422_blkorder(src, dst, YUV422_UYVY, width, height, unit);
}

int CYuv422::Vyuy2Blk(char *src, char *dst, int width, int height, int unit)
{
	return this->yuv422_blkorder(src, dst, YUV422_VYUY, width, height, unit);
}

int CYuv422::Yvyu2Blk(char *src, char *dst, int width, int height, int unit)
{
	return this->yuv422_blkorder(src, dst, YUV422_YVYU, width, height, unit);
}

int CYuv422::Yuvy2Blk(char *src, char *dst, int width, int height, int unit)
{
	return this->yuv422_blkorder(src, dst, YUV422_YUVY, width, height, unit);
}

int CYuv422::Yvuy2Blk(char *src, char *dst, int width, int height, int unit)
{
	return this->yuv422_blkorder(src, dst, YUV422_YVUY, width, height, unit);
}

int CYuv422::Uvyy2Blk(char *src, char *dst, int width, int height, int unit)
{
	return this->yuv422_blkorder(src, dst, YUV422_UVYY, width, height, unit);
}

int CYuv422::Vuyy2Blk(char *src, char *dst, int width, int height, int unit)
{
	return this->yuv422_blkorder(src, dst, YUV422_VUYY, width, height, unit);
}

int CYuv422::Yyvu2Blk(char *src, char *dst, int width, int height, int unit)
{
	return this->yuv422_blkorder(src, dst, YUV422_YYVU, width, height, unit);
}

//=======================================================
//
//		Block convert to YUV422 format
//
//=======================================================

int CYuv422::Blk2Yyuv(char *src, char *dst, int width, int height, int unit)
{
	return this->blk2yuv422order(src, dst, YUV422_YYUV, width, height, unit);
}

int CYuv422::Blk2Yuyv(char *src, char *dst, int width, int height, int unit)
{
	return this->blk2yuv422order(src, dst, YUV422_YUYV, width, height, unit);
}
int CYuv422::Blk2Uyvy(char *src, char *dst, int width, int height, int unit)
{
	return this->blk2yuv422order(src, dst, YUV422_UYVY, width, height, unit);
}
int CYuv422::Blk2Vyuy(char *src, char *dst, int width, int height, int unit)
{
	return this->blk2yuv422order(src, dst, YUV422_VYUY, width, height, unit);
}
int CYuv422::Blk2Yvyu(char *src, char *dst, int width, int height, int unit)
{
	return this->blk2yuv422order(src, dst, YUV422_YVYU, width, height, unit);
}

int CYuv422::Blk2Yuvy(char *src, char *dst, int width, int height, int unit)
{
	return this->blk2yuv422order(src, dst, YUV422_YUVY, width, height, unit);
}

int CYuv422::Blk2Yvuy(char *src, char *dst, int width, int height, int unit)
{
	return this->blk2yuv422order(src, dst, YUV422_YVUY, width, height, unit);
}
int CYuv422::Blk2Uvyy(char *src, char *dst, int width, int height, int unit)
{
	return this->blk2yuv422order(src, dst, YUV422_UVYY, width, height, unit);
}
int CYuv422::Blk2Vuyy(char *src, char *dst, int width, int height, int unit)
{
	return this->blk2yuv422order(src, dst, YUV422_VUYY, width, height, unit);
}
int CYuv422::Blk2Yyvu(char *src, char *dst, int width, int height, int unit)
{
	return this->blk2yuv422order(src, dst, YUV422_YYVU, width, height, unit);
}


//the parameter  "char * buffer" is used as both input-buffer-data and output-buffer-data
int CYuv422::Blk2Rgb24(char *src, char * dst, int width, int height)
{
	int len, size, msize, cap, ncount = 0;
	char* psrcbuf, *pdstbuf, *ptmpbuf;
	CBlk2YuvBuf blkbuf((char)YUV422_YYVU, width, 1);
	CBufferBase tmpbuf;

	size = msize =  (height * width) << 1;
	tmpbuf.SetSize(msize);
	psrcbuf = src;
	ptmpbuf = tmpbuf.m_buf;
	
	//Yuvblk to YUV422_YYVU 
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

	//YUV422_YYVU to Rgb24
	cap = 256;
	size = 4;
	len = width * height * 2;

	pdstbuf = dst;
	ptmpbuf = tmpbuf.m_buf;
	
	for(int i = 0; i < len; i += size)
	{
		width = CYuv422::yuv422_rgborder(ptmpbuf, pdstbuf, YUV422_YYVU, cap, 1);
		pdstbuf += width;
		ptmpbuf += size;
		ncount += width;
	}

	return ncount;
}

//=======================================================
//
//		YUV444 convert to YUV422 format
//
//=======================================================


int CYuv422::Yuv444_Yyuv(char *src, char *dst, int unit)
{
	return this->yuv444_422order(src, dst, YUV422_YYUV, unit);
}

int CYuv422::Yuv444_Yuyv(char *src, char *dst, int unit)
{
	return this->yuv444_422order(src, dst, YUV422_YUYV, unit);
}

int CYuv422::Yuv444_Uyvy(char *src, char *dst, int unit)
{
	return this->yuv444_422order(src, dst, YUV422_UYVY, unit);
}

int CYuv422::Yuv444_Vyuy(char *src, char *dst, int unit)
{
	return this->yuv444_422order(src, dst, YUV422_VYUY, unit);
}

int CYuv422::Yuv444_Yvyu(char *src, char *dst, int unit)
{
	return this->yuv444_422order(src, dst, YUV422_YVYU, unit);
}

int CYuv422::Yuv444_Yuvy(char *src, char *dst, int unit)
{
	return this->yuv444_422order(src, dst, YUV422_YUVY, unit);
}

int CYuv422::Yuv444_Yvuy(char *src, char *dst, int unit)
{
	return this->yuv444_422order(src, dst, YUV422_YVUY, unit);
}

int CYuv422::Yuv444_Uvyy(char *src, char *dst, int unit)
{
	return this->yuv444_422order(src, dst, YUV422_UVYY, unit);
}

int CYuv422::Yuv444_Vuyy(char *src, char *dst, int unit)
{
	return this->yuv444_422order(src, dst, YUV422_VUYY, unit);
}

int CYuv422::Yuv444_Yyvu(char *src, char *dst, int unit)
{
	return this->yuv444_422order(src, dst, YUV422_YYVU, unit);
}

//=======================================================
//
//		YUV422 convert to YUV444 format
//
//=======================================================

int CYuv422::Yyuv2Yuv444(char *src, char *dst, int unit)
{
	return this->yuv422_444order(src, dst, YUV422_YYUV, unit);
}

int CYuv422::Yuyv2Yuv444(char *src, char *dst, int unit)
{
	return this->yuv422_444order(src, dst, YUV422_YUYV, unit);
}

int CYuv422::Uyvy2Yuv444(char *src, char *dst, int unit)
{
	return this->yuv422_444order(src, dst, YUV422_UYVY, unit);
}

int CYuv422::Vyuy2Yuv444(char *src, char *dst, int unit)
{
	return this->yuv422_444order(src, dst, YUV422_VYUY, unit);
}

int CYuv422::Yvyu2Yuv444(char *src, char *dst, int unit)
{
	return this->yuv422_444order(src, dst, YUV422_YVYU, unit);
}

int CYuv422::Yuvy2Yuv444(char *src, char *dst, int unit)
{
	return this->yuv422_444order(src, dst, YUV422_YUVY, unit);
}

int CYuv422::Yvuy2Yuv444(char *src, char *dst, int unit)
{
	return this->yuv422_444order(src, dst, YUV422_YVUY, unit);
}

int CYuv422::Uvyy2Yuv444(char *src, char *dst, int unit)
{
	return this->yuv422_444order(src, dst, YUV422_UVYY, unit);
}

int CYuv422::Vuyy2Yuv444(char *src, char *dst, int unit)
{
	return this->yuv422_444order(src, dst, YUV422_VUYY, unit);
}

int CYuv422::Yyvu2Yuv444(char *src, char *dst, int unit)
{
	return this->yuv422_444order(src, dst, YUV422_YYVU, unit);
}

//=======================================================
//
//		YUV422 image resize
//
//=======================================================
int CYuv422::YyuvResize(char *src, char *dest, int srcw, int srch, int dstw, int dsth, int unit)
{
	return this->yuv422_resizeorder(src, dest, srcw, srch, dstw, dsth, YUV422_YYUV, unit);
}

int CYuv422::YyvuResize(char *src, char *dest, int srcw, int srch, int dstw, int dsth, int unit)
{
	return this->yuv422_resizeorder(src, dest, srcw, srch, dstw, dsth, YUV422_YYVU, unit);
}

int CYuv422::YuyvResize(char *src, char *dest, int srcw, int srch, int dstw, int dsth, int unit)
{
	return this->yuv422_resizeorder(src, dest, srcw, srch, dstw, dsth, YUV422_YUYV, unit);
}

int CYuv422::YvyuResize(char *src, char *dest, int srcw, int srch, int dstw, int dsth, int unit)
{
	return this->yuv422_resizeorder(src, dest, srcw, srch, dstw, dsth, YUV422_YVYU, unit);
}

int CYuv422::UyvyResize(char *src, char *dest, int srcw, int srch, int dstw, int dsth, int unit)
{
	return this->yuv422_resizeorder(src, dest, srcw, srch, dstw, dsth, YUV422_UYVY, unit);
}

int CYuv422::VyuyResize(char *src, char *dest, int srcw, int srch, int dstw, int dsth, int unit)
{
	return this->yuv422_resizeorder(src, dest, srcw, srch, dstw, dsth, YUV422_VYUY, unit);
}

int CYuv422::UvyyResize(char *src, char *dest, int srcw, int srch, int dstw, int dsth, int unit)
{
	return this->yuv422_resizeorder(src, dest, srcw, srch, dstw, dsth, YUV422_UVYY, unit);
}

int CYuv422::VuyyResize(char *src, char *dest, int srcw, int srch, int dstw, int dsth, int unit)
{
	return this->yuv422_resizeorder(src, dest, srcw, srch, dstw, dsth, YUV422_VUYY, unit);
}

int CYuv422::YuvyResize(char *src, char *dest, int srcw, int srch, int dstw, int dsth, int unit)
{
	return this->yuv422_resizeorder(src, dest, srcw, srch, dstw, dsth, YUV422_YUVY, unit);
}

int CYuv422::YvuyResize(char *src, char *dest, int srcw, int srch, int dstw, int dsth, int unit)
{
	return this->yuv422_resizeorder(src, dest, srcw, srch, dstw, dsth, YUV422_YVUY, unit);
}

//=======================================================
//
//		YUV422 image sizer
//
//=======================================================
int CYuv422::YuyvSizer(char *src, char *dest, int srcw, int dstw, int dsth, int px, int py, int unit)
{
	return this->MYuv422Sizer(src, dest, srcw, dstw, dsth, px, py, YUV422_YUYV, unit);
}

int CYuv422::YuvySizer(char *src, char *dest, int srcw, int dstw, int dsth, int px, int py, int unit)
{
	return this->MYuv422Sizer(src, dest, srcw, dstw, dsth, px, py, YUV422_YUVY, unit);
}

int CYuv422::YvyuSizer(char *src, char *dest, int srcw, int dstw, int dsth, int px, int py, int unit)
{
	return this->MYuv422Sizer(src, dest, srcw, dstw, dsth, px, py, YUV422_YVYU, unit);
}

int CYuv422::YvuySizer(char *src, char *dest, int srcw, int dstw, int dsth, int px, int py, int unit)
{
	return this->MYuv422Sizer(src, dest, srcw, dstw, dsth, px, py, YUV422_YVUY, unit);
}

int CYuv422::YyuvSizer(char *src, char *dest, int srcw, int dstw, int dsth, int px, int py, int unit)
{
	return this->MYuv422Sizer(src, dest, srcw, dstw, dsth, px, py, YUV422_YYUV, unit);
}

int CYuv422::YyvuSizer(char *src, char *dest, int srcw, int dstw, int dsth, int px, int py, int unit)
{
	return this->MYuv422Sizer(src, dest, srcw, dstw, dsth, px, py, YUV422_YYVU, unit);
}

int CYuv422::UyvySizer(char *src, char *dest, int srcw, int dstw, int dsth, int px, int py, int unit)
{
	return this->MYuv422Sizer(src, dest, srcw, dstw, dsth, px, py, YUV422_UYVY, unit);
}

int CYuv422::VyuySizer(char *src, char *dest, int srcw, int dstw, int dsth, int px, int py, int unit)
{
	return this->MYuv422Sizer(src, dest, srcw, dstw, dsth, px, py, YUV422_VYUY, unit);
}

int CYuv422::UvyySizer(char *src, char *dest, int srcw, int dstw, int dsth, int px, int py, int unit)
{
	return this->MYuv422Sizer(src, dest, srcw, dstw, dsth, px, py, YUV422_UVYY, unit);
}

int CYuv422::VuyySizer(char *src, char *dest, int srcw, int dstw, int dsth, int px, int py, int unit)
{
	return this->MYuv422Sizer(src, dest, srcw, dstw, dsth, px, py, YUV422_VUYY, unit);
}



//=======================================================
//
//		YUV422 encode
//
//=======================================================
int CYuv422::getVwc(int width, int height)
{
	width = ((width + 15) >> 4) << 4;
	height = ((height + 7) >> 3) << 3;
	width *= height;
	width <<= 1;
	return width;
}

int CYuv422::Blk2Jpeg(char *src, char *dest, int width, int height,  int *nextq, int qf)
{
	CJpegEncoder enc;
	int len;//, q;

	enc.SetJpegQf(qf);
	enc.SetJpegSize(width, height);
	enc.SetYuvFmt(YUV422BLK);
	enc.Open();
	len = enc.getVwc();
	enc.Write(src, len);
	len = enc.Read(dest, len);
	//*nextq = enc.calcQ(len);
	*nextq = qf;
	return len;
}

int CYuv422::Blk2Jpeg1(char *src, char *dest, int width, int height, int *nextq, int qf, int tcr, PTRudebrcOption pinfo)
{
	CJpegEncoder enc;
	int len;

	enc.SetJpegQf(qf, tcr, pinfo);
	enc.SetJpegSize(width, height);
	enc.SetYuvFmt(YUV422BLK);
	enc.Open();
	len = enc.getVwc();
	if (pinfo != NULL)
        enc.WriteBrc(src, len);
	else
		enc.Write(src, len);
	len = enc.Read(dest, len);
	if (pinfo != NULL)
		*nextq = enc.RudeBrcQ(len);
	else
		*nextq = enc.calcQ(len);
	
	return len;
}
//=======================================================
//
//		YUV422 to frame
//
//=======================================================
int CYuv422::MYuv422_FrmOrder(char *src, char *dst, int width, int height, int order, int unit)
{
	int i, j, ypos, upos, vpos;
	int yuv[4];
	char *psrc = src;

	ypos = 0;
	upos = width * height * unit;
	vpos = upos + (upos >> 1);
	for(i = 0; i < height; i++)
	{
		for(j = 0; j < width; j += 2, psrc += unit << 2, ypos += unit << 1, upos++, vpos++)
		{
			CYuv2Rgb::getyuv422(psrc, yuv, order, unit);
			CBaseConvert::setvalue(dst + ypos, yuv, 1, unit);
			CBaseConvert::setvalue(dst + ypos+1, yuv+3, 1, unit);
			CBaseConvert::setvalue(dst + upos, yuv+1, 1, unit);
			CBaseConvert::setvalue(dst + vpos, yuv+2, 1, unit);
		}
	}

	return vpos;
}

int CYuv422::MFrm2Yuv422Order(char *src, char *dst, int width, int height, int order, int unit)
{
	int i, j, ypos, upos, vpos;
	int yuv[4];
	char *pdst = dst;

	ypos = 0;
	upos = width * height * unit;
	vpos = upos + (upos >> 1);
	for(i = 0; i < height; i++)
	{
		for(j = 0; j < width; j += 2, pdst += unit << 2, ypos += unit << 1, upos++, vpos++)
		{
			yuv[0] = CBaseConvert::getvalue(src + ypos, unit);
			yuv[3] = CBaseConvert::getvalue(src + ypos+1, unit);
			yuv[1] = CBaseConvert::getvalue(src + upos, unit);
			yuv[2] = CBaseConvert::getvalue(src + vpos, unit);
			CYuv2Rgb::setyuv422(pdst, yuv, order, unit);
		}
	}

	return vpos;
}
