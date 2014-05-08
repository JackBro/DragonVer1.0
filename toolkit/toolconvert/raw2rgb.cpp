
#include "internal.h"

//=============	class CRaw2Rgb ==============================

const int gPixelCell[] = { 2, 1, 1, 2, 0, 1, 1, 0 };

const int gDither8_4[][4] = 
{
	{ 1, 9, 3, 11 },
	{ 13, 5, 15, 7 },
	{ 4, 12, 2, 10 },
	{ 16, 8, 14, 6 }
};

const int gDither8_5[][4] = 
{
	{ 1, 5, 2, 6 },
	{ 7, 3, 8, 4 },
	{ 2, 6, 1, 5 },
	{ 8, 4, 7, 3 }
};

const int gDither8_6[][4] = 
{
	{ 1, 3, 1, 3 },
	{ 4, 2, 4, 2 },
	{ 1, 3, 1, 3 },
	{ 4, 2, 4, 2 }
};

int CRaw2Rgb::rgb2bayerorder(char *src, char *dst, int order, int width, int height, int unit)
{
	int i, pos, offset, m, step = unit * 3;
	int len = 0;
	int srcpos = 0;
	int bgline, gfirst;

	bgline = gImgStreamOrder[order][0] ? 1 : 0;
	gfirst = gImgStreamOrder[order][1] ? 1 : 0;
	while(height--)
	{
		pos = bgline << 2;
		pos += gfirst << 1;
		for(i = 0, m = 0; i < width; i++, m += step)
		{
			offset = gPixelCell[pos + (i % 2)];
			memcpy(dst+len, src + srcpos+m+offset, unit);
			len += unit;
		}
		bgline = (bgline == 0) ? 1 : 0;
		gfirst = (gfirst == 0) ? 1 : 0;
		srcpos += m;
	}
	return len;
}

int CRaw2Rgb::Rgb2GB(char *src, char *dst, int width, int height, int unit)
{
	return this->rgb2bayerorder(src, dst, RAW_GB, width, height, unit);
}

int CRaw2Rgb::Rgb2BG(char *src, char *dst, int width, int height, int unit)
{
	return this->rgb2bayerorder(src, dst, RAW_BG, width, height, unit);
}

int CRaw2Rgb::Rgb2GR(char *src, char *dst, int width, int height, int unit)
{
	return this->rgb2bayerorder(src, dst, RAW_GR, width, height, unit);
}

int CRaw2Rgb::Rgb2RG(char *src, char *dst, int width, int height, int unit)
{
	return this->rgb2bayerorder(src, dst, RAW_RG, width, height, unit);
}

void CRaw2Rgb::Rgb1_Rgb24(int bitvalue, char *dst, char *palette)
{
	RGBQUAD *pal = (RGBQUAD *)palette;

	bitvalue &= BIT0;
	dst[0] = pal[bitvalue].rgbBlue;
	dst[1] = pal[bitvalue].rgbGreen;
	dst[2] = pal[bitvalue].rgbRed;
}

void CRaw2Rgb::Rgb2_Rgb24(int bitvalue, char *dst, char *palette)
{
	RGBQUAD *pal = (RGBQUAD *)palette;

	bitvalue &= BIT2 - 1;
	dst[0] = pal[bitvalue].rgbBlue;
	dst[1] = pal[bitvalue].rgbGreen;
	dst[2] = pal[bitvalue].rgbRed;
}

void CRaw2Rgb::Rgb4_Rgb24(int bitvalue, char *dst, char *palette)
{
	RGBQUAD *pal = (RGBQUAD *)palette;

	bitvalue &= BIT4 - 1;
	dst[0] = pal[bitvalue].rgbBlue;
	dst[1] = pal[bitvalue].rgbGreen;
	dst[2] = pal[bitvalue].rgbRed;
}

void CRaw2Rgb::Rgb8_Rgb24(int bitvalue, char *dst, char *palette)
{
	RGBQUAD *pal = (RGBQUAD *)palette;

	bitvalue &= BIT8 - 1;
	dst[0] = pal[bitvalue].rgbBlue;
	dst[1] = pal[bitvalue].rgbGreen;
	dst[2] = pal[bitvalue].rgbRed;
}

void CRaw2Rgb::Rgb8_Rgb2(int bitvalue, char *dst, int dststart)
{
	CBitTools bits;

	bits.CopyBitStream((char *)&bitvalue, dst, 2, dststart, 0);
}

void CRaw2Rgb::Rgb444_Rgb24(int bitvalue, char *dst)
{
	dst[0] = (char)((bitvalue & 0xf) << 4);
	dst[1] = (char)(bitvalue & 0xf0);
	dst[2] = (char)((bitvalue & 0xf00) >> 4);
}

void CRaw2Rgb::Rgb666_Rgb24(int bitvalue, char *dst)
{
	dst[0] = (char)((bitvalue & 0x3f) << 2);
	dst[1] = (char)((bitvalue & 0xfc0) >> 4);
	dst[2] = (char)((bitvalue & 0x3f000) >> 10);
}

void CRaw2Rgb::Rgb555_Rgb24(char *src, char *dst)
{
	dst[0] = (char)((src[0] & 0x1f) << 3);
	dst[1] = (char)( ( (src[0] & 0xe0) >> 2) | ( (src[1] & 0x3) << 6 ) );
	dst[2] = (char)((src[1] & 0x7f) << 1);
}

void CRaw2Rgb::Rgb565_Rgb24(char *src, char *dst)
{
	dst[0] = (char)((src[0] & 0x1f) << 3);
	dst[1] = (char)( ( (src[0] & 0xe0) >> 3) | ( (src[1] & 0x7) << 5 ) );
	dst[2] = (char)(src[1] & 0xf8);
}

void CRaw2Rgb::Rgb32_Rgb24(char *src, char *dst)
{
	dst[0] = src[0];
	dst[1] = src[1];
	dst[2] = src[2];
}

//general Rgb format to Rgb24:
int CRaw2Rgb::Rgb_Rgb24Convert(char *src, char *dest, int width, int height, int srcfmt)
{
	char pal1[8], pal2[16], pal4[0x40], pal8[0x400], srcbuf444[3];
	int length, i, len, val = 0;
	length = width * height;
	CBitTools bits;

	switch(srcfmt)
	{
	case RGB1:
		{
			memcpy(pal1, src, 8);
			src += 8;
			while(length)
			{
				len = (length > 8) ? 8 : length;
				memcpy((char *)&val, src, 1); //fsrc.Read((char *)&val, 1);
				src += 1; 
				for(i = 0; i < len; i++, val >>= 1)
				{
					CRaw2Rgb::Rgb1_Rgb24(val & 1, dest, pal1);
					dest += 3; //set the position to next RGB24 pixel.
				}
				length -= len;
			}
			break;
		}
	case RGB2:
		{
			memcpy(pal2, src, 16);
			src += 16;
			while(length)
			{
				len = (length > 4) ? 4 : length;
				memcpy((char *)&val, src, 1); //fsrc.Read((char *)&val, 1);
				src += 1; 
				for(i = 0; i < len; i++, val >>= 2)
				{
					CRaw2Rgb::Rgb2_Rgb24(val & 3, dest, pal2);
					dest += 3; //set the destination buffer position to next RGB24 pixel.
				}
				length -= len;
			}				
			break;
		}
	case RGB4:
		{
			memcpy(pal4, src, 0x40);
			src += 0x40;
			while(length)
			{
				len = (length > 2) ? 2 : length;
				memcpy((char *)&val, src, 1); //fsrc.Read((char *)&val, 1);
				src += 1; 
				for(i = 0; i < len; i++, val >>= 4)
				{
					CRaw2Rgb::Rgb4_Rgb24(val & 0xf, dest, pal4);
					dest += 3;
				}
				length -= len;
			}
			break;
		}
	case RGB8:
		{
			memcpy(pal8, src, 0x400);
			src += 0x400;
			while(length)
			{
				memcpy((char *)&val, src, 1); //fsrc.Read((char *)&val, 1);
				src += 1; 
				CRaw2Rgb::Rgb8_Rgb24(val, dest, pal8);
				dest += 3;
				length--;
			}
			break;
		}
	case RGB444:
		{
			length *= 12;
			while(length)
			{
				len = (length > 24) ? 24 : length;
				memcpy(srcbuf444, src, 3);
				src += 3;
				for(i = 0; i < len; i += 12)
				{
					bits.CopyBitStream(srcbuf444, (char *)&val, 12, 0, i);
					CRaw2Rgb::Rgb444_Rgb24(val, dest);
					dest += 3;
				}
				length -= len;
			}			
			break;
		}
	case RGB555:
		{
			while(length)
			{
				CRaw2Rgb::Rgb555_Rgb24(src, dest);
				src += 2;
				dest += 3;
				length --;
			}
			break;
		}
	case RGB565:
		{
			while(length)
			{
				CRaw2Rgb::Rgb565_Rgb24(src, dest);
				src += 2;
				dest += 3;
				length --;
			}			
			break;
		}
	case RGB32:
		{
			while(length)
			{
				CRaw2Rgb::Rgb32_Rgb24(src, dest);
				src += 4;
				dest += 3;
				length --;
			}			
			break;
		}
	case RGB24:
		memcpy(dest, src, length*3);
		break;
	default:
		break;
	}

	return (width*height*3); //return Rgb24 buffer length of dest.
}

void CRaw2Rgb::Rgb24_Rgb32(char *src, char *dst)
{
	dst[0] = src[0];
	dst[1] = src[1];
	dst[2] = src[2];
	dst[3] = 0;
}

int CRaw2Rgb::Rgb24_Rgb444(char *src, char *dst, int width, int height, int startbit, int dither)
{
	int i, j, r, g, b;
	int pos = 0;
	CBitTools bits;

	for(i = 0; i < height; i++)
	{
		for(j = 0; j < width; j++, pos += 3, startbit += 12)
		{
			b = ((int)src[pos] & 0xff) + (dither ? gDither8_4[i % 4][j % 4] - 8 : 0);
			g = ((int)src[pos+1] & 0xff) + (dither ? gDither8_4[i % 4][j % 4] - 8 : 0);
			r = ((int)src[pos+2] & 0xff) + (dither ? gDither8_4[i % 4][j % 4] - 8 : 0);
			r = r > 255 ? 255 : (r < 0) ? 0 : r;
			g = g > 255 ? 255 : (g < 0) ? 0 : g;
			b = b > 255 ? 255 : (b < 0) ? 0 : b;
			bits.CopyBitStream((char *)&b, dst, 4, startbit, 4);
			bits.CopyBitStream((char *)&g, dst, 4, startbit+4, 4);
			bits.CopyBitStream((char *)&r, dst, 4, startbit+8, 4);
		}
	}
	return width * height * 12;;
}

int CRaw2Rgb::Rgb24_Rgb666(char *src, char *dst, int width, int height, int startbit, int dither)
{
	int i, j, r, g, b;
	int pos = 0;
	CBitTools bits;

	for(i = 0; i < height; i++)
	{
		for(j = 0; j < width; j++, pos += 3, startbit += 18)
		{
			b = ((int)src[pos] & 0xff) + (dither ? gDither8_6[i % 4][j % 4] - 2 : 0);
			g = ((int)src[pos+1] & 0xff) + (dither ? gDither8_6[i % 4][j % 4] - 2 : 0);
			r = ((int)src[pos+2] & 0xff) + (dither ? gDither8_6[i % 4][j % 4] - 2 : 0);
			r = r > 255 ? 255 : (r < 0) ? 0 : r;
			g = g > 255 ? 255 : (g < 0) ? 0 : g;
			b = b > 255 ? 255 : (b < 0) ? 0 : b;
			bits.CopyBitStream((char *)&b, dst, 6, startbit, 2);
			bits.CopyBitStream((char *)&g, dst, 6, startbit+6, 2);
			bits.CopyBitStream((char *)&r, dst, 6, startbit+12, 2);
		}
	}
	return width * height * 18;
}

int CRaw2Rgb::Rgb24_Rgb555(char *src, char *dst, int width, int height, int dither)
{
	int i, j, r, g, b;
	int pos = 0;
	char *pdst = dst;

	for(i = 0; i < height; i++)
	{
		for(j = 0; j < width; j++, pos += 3, pdst += 2)
		{
			b = ((int)src[pos] & 0xff) + (dither ? gDither8_5[i % 4][j % 4] - 4 : 0);
			g = ((int)src[pos+1] & 0xff) + (dither ? gDither8_5[i % 4][j % 4] - 4 : 0);
			r = ((int)src[pos+2] & 0xff) + (dither ? gDither8_5[i % 4][j % 4] - 4 : 0);
			r = r > 255 ? 255 : (r < 0) ? 0 : r;
			g = g > 255 ? 255 : (g < 0) ? 0 : g;
			b = b > 255 ? 255 : (b < 0) ? 0 : b;
			pdst[0] = (char)( (b >> 3) | ((g & 0xf8) << 2) );
			pdst[1] = (char)( (g >> 6) | ((r & 0xf8) >> 1) | 0x80 );
		}
	}
	return width * height * 2;
}
int CRaw2Rgb::Rgb24_Rgb565(char *src, char *dst, int width, int height, int dither)
{
	int i, j, r, g, b;
	int pos = 0;
	char *pdst = dst;

	for(i = 0; i < height; i++)
	{
		for(j = 0; j < width; j++, pos += 3, pdst += 2)
		{
			b = ((int)src[pos] & 0xff) + (dither ? gDither8_5[i % 4][j % 4] - 4 : 0);
			g = ((int)src[pos+1] & 0xff) + (dither ? gDither8_6[i % 4][j % 4] - 2 : 0);
			r = ((int)src[pos+2] & 0xff) + (dither ? gDither8_5[i % 4][j % 4] - 4 : 0);
			r = r > 255 ? 255 : (r < 0) ? 0 : r;
			g = g > 255 ? 255 : (g < 0) ? 0 : g;
			b = b > 255 ? 255 : (b < 0) ? 0 : b;
			pdst[0] = (char)( (b >> 3) | ((g & 0xfc) << 3) );
			pdst[1] = (char)( (g >> 5) | (r & 0xf8) );
		}
	}
	return width * height * 2;
}

void CRaw2Rgb::Rgb555_Rgb32(char *src, char *dst)
{
	dst[0] = (char)( ( (int)src[0] & 0x1f ) << 3 );
	dst[1] = (char)( ( ( (int)src[1] & 0x3 ) << 6 ) | ( ( (int)src[0] & 0xe0 ) >> 2) );
	dst[2] = (char)( ( (int)src[2] & 0x7c ) << 1 );
	dst[3] = (char)((int)src[2] & 0x80);
}

int CRaw2Rgb::Rgb32_Rgb555(char *src, char *dst, int width, int height, int dither)
{
	int i, j, r, g, b, alpha;
	int pos = 0;
	char *pdst = dst;

	for(i = 0; i < height; i++)
	{
		for(j = 0; j < width; j++, pos += 4, pdst += 2)
		{
			b = ((int)src[pos] & 0xff) + (dither ? gDither8_5[i % 4][j % 4] - 4 : 0);
			g = ((int)src[pos+1] & 0xff) + (dither ? gDither8_5[i % 4][j % 4] - 4 : 0);
			r = ((int)src[pos+2] & 0xff) + (dither ? gDither8_5[i % 4][j % 4] - 4 : 0);
			alpha = (int)src[pos+3] & 0x80;
			r = r > 255 ? 255 : (r < 0) ? 0 : r;
			g = g > 255 ? 255 : (g < 0) ? 0 : g;
			b = b > 255 ? 255 : (b < 0) ? 0 : b;
			pdst[0] = (char)( (b >> 3) | ((g & 0xf8) << 2) );
			pdst[1] = (char)( (g >> 6) | ((r & 0xf8) >> 1) | alpha );
		}
	}
	return width * height * 2;
}



void CRaw2Rgb::RgbZinHorSizer(char *src, char *dst, int srcw, int srch, int dstw, int unit)
{
	int i, j, up, down, k, gap, step = unit * 3, offset = 0;
	int val0, val1, bgr;
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
		for(i = 0; i < dstw; i++, up += srcw)
		{
			if(up >= dstw + 1)
			{
				up -= dstw + 1;
				offset += step;
			}
			down = dstw + 1 - up;
			for(k = 0; k < step; k += unit, pdst += unit)
			{
				if(up == 0)
				{
					bgr = CYuv2Rgb::getvalue(psrc + offset + k, unit);
					CBaseConvert::setvalue(pdst, bgr, unit);
				}
				else
				{
					val0 = CYuv2Rgb::getvalue(psrc + offset + k, unit);
					val1 = CYuv2Rgb::getvalue(psrc + offset + k + step, unit);
					bgr = (val0 * down + val1 * up) / (dstw + 1);
					CBaseConvert::setvalue(pdst, bgr, unit);
				}
			}
		}
	}
}

void CRaw2Rgb::RgbZinVerSizer(char *src, char *dst, int srcw, int srch, int dsth, int unit)
{
	int i, j, up, down, k, gap, step = unit * 3;
	int bgr, val0, val1;
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
	for(j = 0; j < dsth; j++, up += srch)
	{
		if(up >= (dsth+1))
		{
			up -= dsth + 1;
			psrc += gap;
		}
		down = dsth + 1 - up;
		for(i = 0; i < gap; i += step)
		{
			for(k = 0; k < step; k += unit, pdst += unit)
			{
				if(up == 0)
				{
					bgr = CYuv2Rgb::getvalue(psrc + k + i, unit);
					CBaseConvert::setvalue(pdst, bgr, unit);
				}
				else
				{
					val0 = CYuv2Rgb::getvalue(psrc + k + i, unit);
					val1 = CYuv2Rgb::getvalue(psrc + gap + i + k, unit);
					bgr = (val0 * down + val1 * up) / (dsth + 1);
					CBaseConvert::setvalue(pdst, bgr, unit);
				}
			}
		}
	}
}


void CRaw2Rgb::RgbZoutHorSizer(char *src, char *dst, int srcw, int srch, int dstw, int unit)
{
	int i, j, up, down, k, step = unit * 3;
	int val0, val1, bgr;
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
					bgr = CYuv2Rgb::getvalue(psrc + k, unit);
					CBaseConvert::setvalue(pdst, bgr, unit);
				}
				else
				{
					val0 = CYuv2Rgb::getvalue(psrc + k - step, unit);
					val1 = CYuv2Rgb::getvalue(psrc + k, unit);
					bgr = (val0 * up + val1 * down) / (dstw - 1);
					CBaseConvert::setvalue(pdst, bgr, unit);
				}
			}
		}
		//memcpy(pdst, psrc, step);
		//psrc += step;
		//pdst += step;
	}
}

void CRaw2Rgb::RgbZoutVerSizer(char *src, char *dst, int srcw, int srch, int dsth, int unit)
{
	int i, j, up, down, k, gap, step = unit * 3;
	int bgr, val0, val1;
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
	for(j = 1; j < srch; j++, up += dsth - 1)
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
					bgr = CYuv2Rgb::getvalue(psrc, unit);
					CBaseConvert::setvalue(pdst, bgr, unit);
				}
				else
				{
					val0 = CYuv2Rgb::getvalue(psrc - gap, unit);
					val1 = CYuv2Rgb::getvalue(psrc, unit);
					bgr = (val0 * up + val1 * down) / (dsth - 1);
					CBaseConvert::setvalue(pdst, bgr, unit);
				}
			}
		}
	}
//	memcpy(pdst, psrc, gap);
}

int CRaw2Rgb::RgbResize(char *src, char *dst, int srcw, int srch, int dstw, int dsth, int unit)
{
	int len = dstw * dsth * unit * 3;

	if(dsth <= srch)
	{
		this->RgbZoutVerSizer(src, src, srcw, srch, dsth, unit);
		if(dstw <= srcw)
			this->RgbZoutHorSizer(src, dst, srcw, dsth, dstw, unit);
		else
			this->RgbZinHorSizer(src, dst, srcw, dsth, dstw, unit);
	}
	else
	{
		this->RgbZinVerSizer(src, dst, srcw, srch, dsth, unit);
		if(dstw <= srcw)
			this->RgbZoutHorSizer(dst, src, srcw, dsth, dstw, unit);
		else
			this->RgbZinHorSizer(dst, src, srcw, dsth, dstw, unit);
		memcpy(dst, src, len);
	}

	return len;
}

//=============	Rgb sizer ==============================
int CRaw2Rgb::MRgbSizer(char *src, char *dest, int srcw, int dstw, int dsth, int px, int py, int unit)
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



//=============	Rgb prefilter ==============================
int CRaw2Rgb::MRgbPrefilter1(char *src, char *dest, int width, int height, 
					int count, int *filter, int unit)
{
	return CBaseConvert::Prefilter(src, dest, width, height, 3, count, filter, unit);
}

int CRaw2Rgb::MRgbPrefilter(char *src, char *dst, int width, int height, 
					int count, int *filter, int unit)
{
	int size = width * height * unit * 3, pos = 0, pos1 = 0, size1;
	CFilterRgb tools(width, unit);

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
//		Rgb rotation
//
//=======================================================
void CRaw2Rgb::Rgb24Rotate(char *src, char *dest, int width, int height, int rotate, int unit)
{
	CBaseConvert::MRotate(src, dest, width, height, rotate, unit * 3);
}

//=======================================================
//
//		Rgb convert to rgb24
//
//=======================================================
int CRaw2Rgb::Rgb2Rgb24(char *src, char *dest, int srcw, int srch, int unit)
{
	int size, val = 0;

	unit = (unit == 1) ? 1 : 2;
	size = srcw * srch * unit;
	if(unit == 1)
		memcpy(dest, src, size);
	else
	{
		while(size)
		{
			val = this->getvalue(src, unit);
			val >>= 2;
			val = (val > 255) ? 255 : ((val < 0) ? 0 : val);
			this->setvalue(dest, val, 1);
			src += unit;
			dest++;
			size -= unit;
		}
	}
	return srcw * srch * 3;
}

int CRaw2Rgb::RgbGetEdge(char *src, char *dest, int srcw, int srch, int unit)
{
	int i, j, val, k, step = unit * 3, pos = ((srcw << 1) + 2) * step;
	int gap[8];

	gap[0] = 0 - ((srcw << 1) + 2) * step;
	gap[1] = 0 - (srcw << 1) * step;
	gap[2] = 0 - ((srcw << 1) - 2) * step;
	gap[3] = 0 - (step << 1);
	gap[4] = step >> 1;
	gap[5] = ((srcw << 1) - 2) * step;
	gap[6] = (srcw << 1) * step;
	gap[7] = ((srcw << 1) + 2) * step;

	srch -= 2;
	srcw -= 2;
	for(i = 2; i < srch; i++)
	{
		pos += step << 1;
		for(j = 2; j < srcw-2; j++, pos, pos += unit, dest++)
		{
			pos += unit;
			val = this->getvalue(src + pos, unit);
			val <<= 3;
			for(k = 0; k < 8; k++)
				val -= this->getvalue(src + pos + gap[i], unit);
			pos += unit << 1;
			this->setvalue(dest, val);
		}
		pos += step << 2;
	}
	return (srcw - 2) * (srch - 2) * unit;
}

//=======================================================
//
//		Rgb convert to rgb24
//
//=======================================================
int CRaw2Rgb::bayer2rgborder(char *src, char *dst, int order, int width, int height, int unit)
{
	int size = width * height * unit, pos = 0, pos1 = 0, size1;
	C5x5Base tools(width, unit);

	size1 = size * 3;
	height = size1;
	tools.SetOrder(order);
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
	return height;
}

int CRaw2Rgb::GB2Rgb(char *src, char *dst, int width, int height, int unit)
{
	return this->bayer2rgborder(src, dst, RAW_GB, width, height, unit);
}

int CRaw2Rgb::BG2Rgb(char *src, char *dst, int width, int height, int unit)
{
	return this->bayer2rgborder(src, dst, RAW_BG, width, height, unit);
}

int CRaw2Rgb::GR2Rgb(char *src, char *dst, int width, int height, int unit)
{
	return this->bayer2rgborder(src, dst, RAW_GR, width, height, unit);
}

int CRaw2Rgb::RG2Rgb(char *src, char *dst, int width, int height, int unit)
{
	return this->bayer2rgborder(src, dst, RAW_RG, width, height, unit);
}

int CRaw2Rgb::bitbmp2rgb(char *src, char *dst, int width, int height, int bitwidth)
{
	CBitFifo bits;
	CBufferBase pool;
	int offset = 0, dstlen, bw = bitwidth, len = 0;

	if( (bits.GetSize() == 0) || (pool.GetSize() == 0) )
	{
		//this->m_dbug.PrintMsg("can't alloc pool\n");
		return 0;
	}
	if(bitwidth % 8)
	{
		bitwidth *= width;
		width = ((((bitwidth + 7) >> 3) + 3) >> 2) << 2;
		while(height--)
		{
			memcpy(pool.m_buf, src, width);
			src += width;
			
			if(bw < 8)
				bits.ReverseBitStream(pool.m_buf, pool.m_buf, width, bw);
			bits.Write(pool.m_buf, bitwidth);
			dstlen = ((bitwidth + offset) >> 3) << 3;
			bits.Read(pool.m_buf, dstlen);
	
			memcpy(dst, pool.m_buf, dstlen >> 3);
			dst += (dstlen >> 3);
			len += (dstlen >> 3);
			
			offset = bitwidth + offset - dstlen;
		}
		if(offset)
		{
			bits.Read(pool.m_buf, offset);

			memcpy(dst, pool.m_buf, (offset + 7) >> 3);
			dst += ((offset + 7) >> 3);
			len += ((offset + 7) >> 3);
		}
		return len;
	}
	width *= bitwidth;
	width >>= 3;
	bitwidth = ((width + 3) >> 2) << 2;
	while(height--)
	{
		memcpy(dst, src, width);
		src += bitwidth;
		dst += width,
		len += width;
	}

	return len;
}

int CRaw2Rgb::bitrgb2bmp(char *src, char *dst, int width, int height, int bitwidth)
{	
	CBitFifo bits;
	CBufferBase pool;
	int dstlen, length, bw = bitwidth, ncount = 0;
	CDbugTools dbug;

	length = this->addbmp24header1(dst, width, height);
	dst += length;
	ncount += length;

	if( (bits.GetSize() == 0) || (pool.GetSize() == 0) )
	{
		dbug.PrintMsg("can't alloc pool\n");
		return 0;
	}
	
	if(bitwidth % 8)
	{
		bitwidth *= width;
		dstlen = ((bitwidth + 7) >> 3) << 3;
		width = ((((bitwidth + 7) >> 3) + 3) >> 2) << 2;
		while(height--)
		{
			memcpy(pool.m_buf, src, dstlen >> 3);
			src += (dstlen >> 3);
			bits.Write(pool.m_buf, dstlen);
			bits.Read(pool.m_buf, bitwidth);
			if(bw < 8)
				bits.ReverseBitStream(pool.m_buf, pool.m_buf, width, bw);
			memcpy(dst, pool.m_buf, width);		
			dst += width;
			ncount += width;
		}
		return ncount;
	}
	
	width *= bitwidth;
	width >>= 3;
	bitwidth = ((width + 3) >> 2) << 2;
	while(height--)
	{
		memcpy(pool.m_buf, src, width);
		memcpy(dst, pool.m_buf, bitwidth);
		src += width;
		dst += bitwidth;
		ncount += bitwidth;
	}	

	return ncount;
}

int CRaw2Rgb::addbmp24header1(char *dstbuf, int width, int height)
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