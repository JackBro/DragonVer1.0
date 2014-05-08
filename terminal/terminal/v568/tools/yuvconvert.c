#include "internal.h"

static const int gStreamOrder[][6] =
{
	{ 1, 0, 2, 3 },			//YUV422 UYVY
	{ 2, 0, 1, 3 },			//YUV422 VYUY
	{ 0, 1, 3, 2 },			//YUV422 YUYV
	{ 0, 2, 3, 1 },			//YUV422 YVYU
	{ 0, 1, 2, 3 },			//YUV422 YUVY

	{ 0, 2, 1, 3 },			//YUV422 YVUY
	{ 1, 2, 0, 3 },			//YUV422 UVYY
	{ 2, 1, 0, 3 },			//YUV422 VUYY
	{ 0, 3, 2, 1 },			//YUV422 YYVU
	{ 0, 3, 1, 2 },			//YUV422 YYUV

	{ 1, 0 },				//RAW_BG
	{ 1, 1 },				//RAW_GB
	{ 0, 0 },				//RAW_RG
	{ 0, 1 },				//RAW_GR

	{ 0, 1, 2 },			//RAW_RGB,
	{ 0, 2, 1 },			//RAW_RBG,
	{ 1, 2, 0 },			//RAW_GBR,
	{ 1, 0, 2 },			//RAW_GRB,
	{ 2, 1, 0 },			//RAW_BGR,
	{ 2, 0, 1 },			//RAW_BRG,

	{ 1, 0, 2, 3 },			//YUV420 UYVY
	{ 2, 0, 1, 3 },			//YUV420 VYUY
	{ 0, 1, 3, 2 },			//YUV420 YUYV
	{ 0, 2, 3, 1 },			//YUV420 YVYU
	{ 0, 1, 2, 3 },			//YUV420 YUVY

	{ 0, 2, 1, 3 },			//YUV420 YVUY
	{ 1, 2, 0, 3 },			//YUV420 UVYY
	{ 2, 1, 0, 3 },			//YUV420 VUYY
	{ 0, 3, 2, 1 },			//YUV420 YYVU
	{ 0, 3, 1, 2 },			//YUV420 YYUV

	{ 0, 3, 6, 9, 1, 2 },	//YUV411_YYYYUV
	{ 0, 3, 6, 9, 2, 1 },	//YUV411_YYYYVU,
	{ 0, 1, 3, 2, 6, 9 },	//YUV411_YUYVYY,
	{ 0, 2, 3, 1, 6, 9 },	//YUV411_YVYUYY,
	{ 1, 2, 0, 3, 6, 9 },	//YUV411_UVYYYY,
	{ 2, 1, 0, 3, 6, 9 }	//YUV411_VUYYYY,
};

/***************************************** Base tools **********************************************/

void CopyBitStreamLow(char *src, char *dst, int size, int dststart, int srcstart)
{
	int i, j, n, val, k;

	i = dststart >> 3;
	j = dststart % 8;
	n = srcstart % 8;
	if(j)
	{
		dst[i] &= (1 << j) - 1;
		val = *((int *)(src + (srcstart >> 3)));
		dst[i] |= (char)((val >> n) << j);
		i++;
		j = 8 - j;
		srcstart += j;
		size -= j;
		n = srcstart % 8;
	}
	while(size)
	{
		k = (size > 8) ? 8 : size;
		val = *((int *)(src + (srcstart >> 3)));
		dst[i] = (char)(val >> n);
		i++;
		srcstart += k;
		size -= k;
	}
}

void BaseSetValueOne(char *dst, int val)
{
	dst[0] = (char)val;
}

void BaseSetValue(char *dst, int *val, int count)
{
	int i;

	for(i = 0; i < count; i++, dst++)
		BaseSetValueOne(dst, val[i]);
}

int BaseGetValueOne(char *src)
{
	int val = 0;

	val = (int)src[0] & 0xff;
	return val;
}

void BaseGetValue(char *src, int *val, int count)
{
	int i;

	for(i = 0; i < count; i++, src++)
		val[i] = BaseGetValueOne(src);
}

void BasePrefilter(int *dat, int cycle, int *filter, int count)
{
	int i, k, start, end, offset;

	offset = count * cycle;
	for(i = 0; i < cycle; i++)
		dat[offset + i] *= filter[0];

	for(k = 0, start = 0, end = (count << 1) * cycle; k < count; k++, start += cycle, end -= cycle)
	{
		for(i = 0; i < cycle; i++)
		{
			dat[start + i] *= filter[count-k];
			dat[end + i] *= filter[count-k];
			dat[offset + i] += dat[start + i] + dat[end + i];
		}
	}
}

//=============	prefilter process ==============================
int HorExtent(char *src, char *dest, int width, int height, int cycle, int count)
{
	int i, j, size, step, pos;
	int buf[MAXCYCLECOUNT];
	char *psrc, *pdst;

	size = width * height * cycle;
	if(count == 1)
	{
		memcpy(dest, src, size);
		return size;
	}
	psrc = src;
	pdst = dest;
	count --;
	size = width * cycle;
	step = cycle;
	for(i = 0; i < height; i++, psrc += size)
	{
		for(j = 0, pos = count * step; j < count; j++, pdst += step, pos -= step)
		{
			BaseGetValue(psrc+pos, buf, cycle);
			BaseSetValue(pdst, buf, cycle);
		}
		memcpy(pdst, psrc, size);
		pdst += size;
		for(j = 0, pos = (width - 2) * step; j < count; j++, pdst += step, pos -= step)
		{
			BaseGetValue(psrc+pos, buf, cycle);
			BaseSetValue(pdst, buf, cycle);
		}
	}
	return (width + (count << 1)) * height * cycle;
}

int VerExtent(char *src, char *dest, int width, int height, int cycle, int count)
{
	int i, size, step;
	char *psrc, *pdst;

	size = width * height * cycle;
	if(count == 1)
	{
		memcpy(dest, src, size);
		return size;
	}
	count--;
	step = width * cycle;
	memcpy(dest + count * step, src, size);
	psrc = src + (step * count);
	pdst = dest;
	for(i = 0; i < count; i++, psrc -= step, pdst += step)
		memcpy(pdst, psrc, step);
	psrc = src + size - (step << 1);
	pdst += size;
	for(i = 0; i < count; i++, psrc -= step, pdst += step)
		memcpy(pdst, psrc, step);
	return width * (height + (count << 1)) * cycle;
}

int Enlarge(char *src, char *dest, int width, int height, int cycle, int count)
{
	HorExtent(src, dest, width, height, cycle, count);
	width += (count - 1) << 1;
	VerExtent(dest, src, width, height, cycle, count);
	height += (count - 1) << 1;
	width *= height * cycle;
	memcpy(dest, src, width);
	return width;
}

int HorPrefilter(char *src, char *dest, int width, int height, int cycle, int count, int *filter)
{
	int i, j, k, size, total, m, n, step, offset;
	char *psrc, *pdst, *psrc1;
	int *pbuf;
	int buf[BUFFER_DEFAULT_SIZE];

	if(count > (MAXFILTERCOUNT >> 1))
		return 0;
	size = width * height * cycle;
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
	size = (count * cycle) << 1;
	m = width - count;
	n = (count << 1) + 1;
	offset = count * cycle;
	step = cycle;
	for(i = 0; i < height; i++)
	{
		for(j = count; j < m; j++, pdst += step, psrc += step)
		{
			for(k = 0, psrc1 = psrc, pbuf = buf; k < n; k++, psrc1 += step, pbuf += cycle)
				BaseGetValue(psrc1, pbuf, cycle);
			BasePrefilter(buf, cycle, filter, count);
			for(k = 0; k < cycle; k++)
				buf[offset + k] /= total;
			BaseSetValue(pdst, buf + offset, cycle);
		}
		psrc += size;
	}
	width -= count << 1;
	return width * height * cycle;
}

int VerPrefilter(char *src, char *dest, int width, int height, int cycle, int count, int *filter)
{
	int i, j, k, size, total, m, n, step, offset;
	char *psrc, *pdst, *pdst1, *psrc1;
	int *pbuf;
	int buf[BUFFER_DEFAULT_SIZE];

	if(count > (MAXFILTERCOUNT >> 1))
		return 0;
	size = width * height * cycle;
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
	size = width * cycle;
	count--;
	m = height - count;
	n = (count << 1) + 1;
	step = cycle;
	offset = count * cycle;
	for(i = 0; i < width; i++, psrc += step, pdst += step)
	{
		for(j = count, pdst1 = pdst, psrc1 = psrc; j < m; j++, pdst1 += size, psrc1 += size)
		{
			for(k = 0, total = 0, pbuf = buf; k < n; k++, total += size, pbuf += cycle)
				BaseGetValue(psrc1 + total, pbuf, cycle);
			BasePrefilter(buf, cycle, filter, count);
			for(k = 0; k < cycle; k++)
				buf[offset+k] /= total;
			BaseSetValue(pdst1, buf + k, cycle);
		}
	}
	height -= count << 1;
	return width * height * cycle;
}

int Prefilter(char *src, char *dest, int width, int height, int cycle, int count, int *filter)
{
	Enlarge(src, dest, width, height, cycle, count);
	HorPrefilter(src, dest, width + ((count - 1) << 1), height + ((count - 1) << 1), 
			cycle, count, filter);
	VerPrefilter(dest, src, width, height + ((count - 1) << 1), cycle, count, filter);
	width *= height * cycle;
	memcpy(src, dest, width);
	return width;
}

void YuvAdjustToBlk(char* pbuf, unsigned long len, unsigned char val)
{
	unsigned long i;
	
	for(i = 0; i < len; i++)
	{
		*(pbuf+i) = (char)(*(pbuf+i) + val);
	}
}
/***************************************** bayer2rgb **********************************************/

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

int Rgb2Bayer(char *src, char *dst, int order, int width, int height)
{
	int i, pos, offset, m, step = 3;
	int len = 0;
	int srcpos = 0;
	int bgline, gfirst;

	bgline = gStreamOrder[order][0] ? 1 : 0;
	gfirst = gStreamOrder[order][1] ? 1 : 0;
	while(height--)
	{
		pos = bgline << 2;
		pos += gfirst << 1;
		for(i = 0, m = 0; i < width; i++, m += step)
		{
			offset = gPixelCell[pos + (i % 2)];
			memcpy(dst+len, src + srcpos+m+offset, 1);
			len++;
		}
		bgline = (bgline == 0) ? 1 : 0;
		gfirst = (gfirst == 0) ? 1 : 0;
		srcpos += m;
	}
	return len;
}

#if 0
void Rgb1_Rgb24(int bitvalue, char *dst, char *palette)
{
	RGBQUAD *pal = (RGBQUAD *)palette;

	bitvalue &= BIT0;
	dst[0] = pal[bitvalue].rgbBlue;
	dst[1] = pal[bitvalue].rgbGreen;
	dst[2] = pal[bitvalue].rgbRed;
}

void Rgb2_Rgb24(int bitvalue, char *dst, char *palette)
{
	RGBQUAD *pal = (RGBQUAD *)palette;

	bitvalue &= BIT2 - 1;
	dst[0] = pal[bitvalue].rgbBlue;
	dst[1] = pal[bitvalue].rgbGreen;
	dst[2] = pal[bitvalue].rgbRed;
}

void Rgb4_Rgb24(int bitvalue, char *dst, char *palette)
{
	RGBQUAD *pal = (RGBQUAD *)palette;

	bitvalue &= BIT4 - 1;
	dst[0] = pal[bitvalue].rgbBlue;
	dst[1] = pal[bitvalue].rgbGreen;
	dst[2] = pal[bitvalue].rgbRed;
}

void Rgb8_Rgb24(int bitvalue, char *dst, char *palette)
{
	RGBQUAD *pal = (RGBQUAD *)palette;

	bitvalue &= BIT8 - 1;
	dst[0] = pal[bitvalue].rgbBlue;
	dst[1] = pal[bitvalue].rgbGreen;
	dst[2] = pal[bitvalue].rgbRed;
}
#endif

void Rgb444_Rgb24(int bitvalue, char *dst)
{
	dst[0] = (char)((bitvalue & 0xf) << 4);
	dst[1] = (char)(bitvalue & 0xf0);
	dst[2] = (char)((bitvalue & 0xf00) >> 4);
}

void Rgb666_Rgb24(int bitvalue, char *dst)
{
	dst[0] = (char)((bitvalue & 0x3f) << 2);
	dst[1] = (char)((bitvalue & 0xfc0) >> 4);
	dst[2] = (char)((bitvalue & 0x3f000) >> 10);
}

void Rgb555_Rgb24(char *src, char *dst)
{
	dst[0] = (char)((src[0] & 0x1f) << 3);
	dst[1] = (char)( ( (src[0] & 0xe0) >> 2) | ( (src[1] & 0x3) << 6 ) );
	dst[2] = (char)((src[1] & 0x7f) << 1);
}

void Rgb565_Rgb24(char *src, char *dst)
{
	dst[0] = (char)((src[0] & 0x1f) << 3);
	dst[1] = (char)( ( (src[0] & 0xe0) >> 3) | ( (src[1] & 0x7) << 5 ) );
	dst[2] = (char)(src[1] & 0xf8);
}

void Rgb32_Rgb24(char *src, char *dst)
{
	dst[0] = src[0];
	dst[1] = src[1];
	dst[2] = src[2];
}

void Rgb24_Rgb32(char *src, char *dst)
{
	dst[0] = src[0];
	dst[1] = src[1];
	dst[2] = src[2];
	dst[3] = 0;
}

int Rgb24_Rgb444(char *src, char *dst, int width, int height, int startbit, int dither)
{
	int i, j, r, g, b;
	int pos = 0;

	for(i = 0; i < height; i++)
	{
		for(j = 0; j < width; j++, pos += 3, startbit += 12)
		{
			b = ((int)src[pos] & 0xff) + (dither ? gDither8_4[i % 4][j % 4] : 0);
			g = ((int)src[pos+1] & 0xff) + (dither ? gDither8_4[i % 4][j % 4] : 0);
			r = ((int)src[pos+2] & 0xff) + (dither ? gDither8_4[i % 4][j % 4] : 0);
			
			CopyBitStreamLow((char *)&b, dst, 4, startbit, 4);
			CopyBitStreamLow((char *)&g, dst, 4, startbit+4, 4);
			CopyBitStreamLow((char *)&r, dst, 4, startbit+8, 4);
		}
	}
	return width * height * 12;;
}

int Rgb24_Rgb666(char *src, char *dst, int width, int height, int startbit, int dither)
{
	int i, j, r, g, b;
	int pos = 0;

	for(i = 0; i < height; i++)
	{
		for(j = 0; j < width; j++, pos += 3, startbit += 18)
		{
			b = ((int)src[pos] & 0xff) + (dither ? gDither8_6[i % 4][j % 4] : 0);
			g = ((int)src[pos+1] & 0xff) + (dither ? gDither8_6[i % 4][j % 4] : 0);
			r = ((int)src[pos+2] & 0xff) + (dither ? gDither8_6[i % 4][j % 4] : 0);
			CopyBitStreamLow((char *)&b, dst, 6, startbit, 2);
			CopyBitStreamLow((char *)&g, dst, 6, startbit+6, 2);
			CopyBitStreamLow((char *)&r, dst, 6, startbit+12, 2);
		}
	}
	return width * height * 18;
}

int Rgb24_Rgb555(char *src, char *dst, int width, int height, int dither)
{
	int i, j, r, g, b;
	int pos = 0;
	char *pdst = dst;

	for(i = 0; i < height; i++)
	{
		for(j = 0; j < width; j++, pos += 3, pdst += 2)
		{
			b = ((int)src[pos] & 0xff) + (dither ? gDither8_5[i % 4][j % 4] : 0);
			g = ((int)src[pos+1] & 0xff) + (dither ? gDither8_5[i % 4][j % 4] : 0);
			r = ((int)src[pos+2] & 0xff) + (dither ? gDither8_5[i % 4][j % 4] : 0);

			pdst[0] = (char)( (b >> 3) | ((g & 0xf8) << 2) );
			pdst[1] = (char)( (g >> 6) | ((r & 0xf8) >> 1) | 0x80 );
		}
	}
	return width * height * 2;
}
int Rgb24_Rgb565(char *src, char *dst, int width, int height, int dither)
{
	int i, j, r, g, b;
	int pos = 0;
	char *pdst = dst;

	for(i = 0; i < height; i++)
	{
		for(j = 0; j < width; j++, pos += 3, pdst += 2)
		{
			b = ((int)src[pos] & 0xff) + (dither ? gDither8_5[i % 4][j % 4] : 0);
			g = ((int)src[pos+1] & 0xff) + (dither ? gDither8_5[i % 4][j % 4] : 0);
			r = ((int)src[pos+2] & 0xff) + (dither ? gDither8_5[i % 4][j % 4] : 0);

			pdst[0] = (char)( (b >> 3) | ((g & 0xfc) << 3) );
			pdst[1] = (char)( (g >> 5) | (r & 0xf8) );
		}
	}
	return width * height * 2;
}

void Rgb555_Rgb32(char *src, char *dst)
{
	dst[0] = (char)( ( (int)src[0] & 0x1f ) << 3 );
	dst[1] = (char)( ( ( (int)src[1] & 0x3 ) << 6 ) | ( ( (int)src[0] & 0xe0 ) >> 2) );
	dst[2] = (char)( ( (int)src[2] & 0x7c ) << 1 );
	dst[3] = (char)((int)src[2] & 0x80);
}

int Rgb32_Rgb555(char *src, char *dst, int width, int height, int dither)
{
	int i, j, r, g, b, alpha;
	int pos = 0;
	char *pdst = dst;

	for(i = 0; i < height; i++)
	{
		for(j = 0; j < width; j++, pos += 4, pdst += 2)
		{
			b = ((int)src[pos] & 0xff) + (dither ? gDither8_5[i % 4][j % 4] : 0);
			g = ((int)src[pos+1] & 0xff) + (dither ? gDither8_5[i % 4][j % 4] : 0);
			r = ((int)src[pos+2] & 0xff) + (dither ? gDither8_5[i % 4][j % 4] : 0);
			alpha = (int)src[pos+3] & 0x80;

			pdst[0] = (char)( (b >> 3) | ((g & 0xf8) << 2) );
			pdst[1] = (char)( (g >> 6) | ((r & 0xf8) >> 1) | alpha );
		}
	}
	return width * height * 2;
}



void RgbZinHorSizer(char *src, char *dst, int srcw, int srch, int dstw)
{
	int i, j, up, down, k, gap, step = 3, offset = 0;
	int val0, val1, bgr;
	char *psrc = src;
	char *pdst = dst;

	if(srcw == dstw)
	{
		if(dst != src)
			memcpy(dst, src, srcw * srch * 3);
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
			for(k = 0; k < step; k++, pdst++)
			{
				if(up == 0)
				{
					bgr = BaseGetValueOne(psrc + offset + k);
					BaseSetValueOne(pdst, bgr);
				}
				else
				{
					val0 = BaseGetValueOne(psrc + offset + k);
					val1 = BaseGetValueOne(psrc + offset + k + step);
					bgr = (val0 * down + val1 * up) / (dstw + 1);
					BaseSetValueOne(pdst, bgr);
				}
			}
		}
	}
}

void RgbZinVerSizer(char *src, char *dst, int srcw, int srch, int dsth)
{
	int i, j, up, down, k, gap, step = 3;
	int bgr, val0, val1;
	char *psrc = src;
	char *pdst = dst;

	if(srch == dsth)
	{
		if(dst != src)
			memcpy(dst, src, srcw * srch * 3);
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
			for(k = 0; k < step; k++, pdst++)
			{
				if(up == 0)
				{
					bgr = BaseGetValueOne(psrc + k + i);
					BaseSetValueOne(pdst, bgr);
				}
				else
				{
					val0 = BaseGetValueOne(psrc + k + i);
					val1 = BaseGetValueOne(psrc + gap + i + k);
					bgr = (val0 * down + val1 * up) / (dsth + 1);
					BaseSetValueOne(pdst, bgr);
				}
			}
		}
	}
}


void RgbZoutHorSizer(char *src, char *dst, int srcw, int srch, int dstw)
{
	int i, j, up, down, k, step = 3;
	int val0, val1, bgr;
	char *psrc = src;
	char *pdst = dst;

	if(srcw == dstw)
	{
		if(dst != src)
			memcpy(dst, src, srcw * srch * 3);
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
			for(k = 0; k < step; k++, pdst++)
			{
				if(up == 0)
				{
					bgr = BaseGetValueOne(psrc + k);
					BaseSetValueOne(pdst, bgr);
				}
				else
				{
					val0 = BaseGetValueOne(psrc + k - step);
					val1 = BaseGetValueOne(psrc + k);
					bgr = (val0 * down + val1 * up) / (dstw - 1);
					BaseSetValueOne(pdst, bgr);
				}
			}
		}
		//memcpy(pdst, psrc, step);
		//psrc += step;
		//pdst += step;
	}
}

void RgbZoutVerSizer(char *src, char *dst, int srcw, int srch, int dsth)
{
	int i, j, up, down, k, gap, step = 3;
	int bgr, val0, val1;
	char *psrc = src;
	char *pdst = dst;

	if(srch == dsth)
	{
		if(dst != src)
			memcpy(dst, src, srcw * srch * 3);
		return;
	}

	gap = srcw * 3;
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
			for(k = 0; k < step; k++, psrc++, pdst++)
			{
				if(up == 0)
				{
					bgr = BaseGetValueOne(psrc);
					BaseSetValueOne(pdst, bgr);
				}
				else
				{
					val0 = BaseGetValueOne(psrc - gap);
					val1 = BaseGetValueOne(psrc);
					bgr = (val0 * down + val1 * up) / (dsth - 1);
					BaseSetValueOne(pdst, bgr);
				}
			}
		}
	}
//	memcpy(pdst, psrc, gap);
}

int RgbResize(char *src, char *dst, int srcw, int srch, int dstw, int dsth)
{
	int len = dstw * dsth * 3;

	if(dsth <= srch)
	{
		RgbZoutVerSizer(src, src, srcw, srch, dsth);
		if(dstw <= srcw)
			RgbZoutHorSizer(src, dst, srcw, dsth, dstw);
		else
			RgbZinHorSizer(src, dst, srcw, dsth, dstw);
	}
	else
	{
		RgbZinVerSizer(src, dst, srcw, srch, dsth);
		if(dstw <= srcw)
			RgbZoutHorSizer(dst, src, srcw, dsth, dstw);
		else
			RgbZinHorSizer(dst, src, srcw, dsth, dstw);
		memcpy(dst, src, len);
	}

	return len;
}

//=============	Rgb sizer ==============================
int MRgbSizer(char *src, char *dest, int srcw, int dstw, int dsth, int px, int py)
{
	int i, size, size1, offset;
	char *psrc, *pdst;

	size = srcw * 3;
	size1 = dstw * 3;
	offset = px * 3;

	psrc = src + size * py;
	pdst = dest;
	for(i = 0; i < dsth; i++, psrc += size, pdst += size1)
		memcpy(pdst, psrc+offset, size1);

	return size1 * dsth;
}

//=============	Rgb prefilter ==============================
int MRgbPrefilter(char *src, char *dest, int width, int height, int count, int *filter)
{
	return Prefilter(src, dest, width, height, 3, count, filter);
}


/******************************************* yuv2rgb **********************************************/

const int gTableV2R[]  =
{
	0,   1,   2,   4,   5,   7,   8,   9,  11,  12,  14,  15,  16,  18,  19,  21, 
	22,  23,  25,  26,  28,  29,  30,  32,  33,  35,  36,  37,  39,  40,  42,  43, 
	44,  46,  47,  49,  50,  51,  53,  54,  56,  57,  58,  60,  61,  63,  64,  65, 
	67,  68,  70,  71,  72,  74,  75,  77,  78,  79,  81,  82,  84,  85,  86,  88, 
	89,  91,  92,  93,  95,  96,  98,  99, 100, 102, 103, 105, 106, 107, 109, 110, 
	112, 113, 114, 116, 117, 119, 120, 122, 123, 124, 126, 127, 129, 130, 131, 133, 
	134, 136, 137, 138, 140, 141, 143, 144, 145, 147, 148, 150, 151, 152, 154, 155, 
	157, 158, 159, 161, 162, 164, 165, 166, 168, 169, 171, 172, 173, 175, 176, 178, 
	179, 180, 182, 183, 185, 186, 187, 189, 190, 192, 193, 194, 196, 197, 199, 200, 
	201, 203, 204, 206, 207, 208, 210, 211, 213, 214, 215, 217, 218, 220, 221, 222, 
	224, 225, 227, 228, 229, 231, 232, 234, 235, 236, 238, 239, 241, 242, 244, 245, 
	246, 248, 249, 251, 252, 253, 255, 256, 258, 259, 260, 262, 263, 265, 266, 267,
	269, 270, 272, 273, 274, 276, 277, 279, 280, 281, 283, 284, 286, 287, 288, 290,
	291, 293, 294, 295, 297, 298, 300, 301, 302, 304, 305, 307, 308, 309, 311, 312,
	314, 315, 316, 318, 319, 321, 322, 323, 325, 326, 328, 329, 330, 332, 333, 335, 
	336, 337, 339, 340, 342, 343, 344, 346, 347, 349, 350, 351, 353, 354, 356, 357 
};

const int gTableU2G[]  =
{
	0,   0,   0,   1,   1,   1,   2,   2,   2,   3,   3,   3,   4,   4,   4,   5, 
	5,   5,   6,   6,   6,   7,   7,   7,   8,   8,   8,   9,   9,   9,  10,  10, 
	11,  11,  11,  12,  12,  12,  13,  13,  13,  14,  14,  14,  15,  15,  15,  16, 
	16,  16,  17,  17,  17,  18,  18,  18,  19,  19,  19,  20,  20,  20,  21,  21, 
	22,  22,  22,  23,  23,  23,  24,  24,  24,  25,  25,  25,  26,  26,  26,  27, 
	27,  27,  28,  28,  28,  29,  29,  29,  30,  30,  30,  31,  31,  31,  32,  32, 
	33,  33,  33,  34,  34,  34,  35,  35,  35,  36,  36,  36,  37,  37,  37,  38, 
	38,  38,  39,  39,  39,  40,  40,  40,  41,  41,  41,  42,  42,  42,  43,  43, 
	44,  44,  44,  45,  45,  45,  46,  46,  46,  47,  47,  47,  48,  48,  48,  49, 
	49,  49,  50,  50,  50,  51,  51,  51,  52,  52,  52,  53,  53,  53,  54,  54, 
	55,  55,  55,  56,  56,  56,  57,  57,  57,  58,  58,  58,  59,  59,  59,  60, 
	60,  60,  61,  61,  61,  62,  62,  62,  63,  63,  63,  64,  64,  64,  65,  65, 
	66,  66,  66,  67,  67,  67,  68,  68,  68,  69,  69,  69,  70,  70,  70,  71, 
	71,  71,  72,  72,  72,  73,  73,  73,  74,  74,  74,  75,  75,  75,  76,  76, 
	77,  77,  77,  78,  78,  78,  79,  79,  79,  80,  80,  80,  81,  81,  81,  82, 
	82,  82,  83,  83,  83,  84,  84,  84,  85,  85,  85,  86,  86,  86,  87,  87
};

const int gTableV2G[]  =
{
	0,   0,   1,   2,   2,   3,   4,   5,   5,   6,   7,   7,   8,   9,  10,  10, 
	11,  12,  12,  13,  14,  15,  15,  16,  17,  17,  18,  19,  20,  20,  21,  22, 
	22,  23,  24,  25,  25,  26,  27,  27,  28,  29,  30,  30,  31,  32,  32,  33, 
	34,  35,  35,  36,  37,  37,  38,  39,  40,  40,  41,  42,  42,  43,  44,  45, 
	45,  46,  47,  47,  48,  49,  50,  50,  51,  52,  52,  53,  54,  55,  55,  56, 
	57,  57,  58,  59,  60,  60,  61,  62,  62,  63,  64,  65,  65,  66,  67,  67, 
	68,  69,  70,  70,  71,  72,  72,  73,  74,  75,  75,  76,  77,  77,  78,  79, 
	80,  80,  81,  82,  82,  83,  84,  85,  85,  86,  87,  87,  88,  89,  90,  90, 
	91,  92,  92,  93,  94,  95,  95,  96,  97,  97,  98,  99,  100, 100, 101, 102, 
	102, 103, 104, 105, 105, 106, 107, 107, 108, 109, 110, 110, 111, 112, 112, 113, 
	114, 115, 115, 116, 117, 117, 118, 119, 120, 120, 121, 122, 122, 123, 124, 125, 
	125, 126, 127, 127, 128, 129, 130, 130, 131, 132, 132, 133, 134, 135, 135, 136, 
	137, 137, 138, 139, 140, 140, 141, 142, 142, 143, 144, 145, 145, 146, 147, 147, 
	148, 149, 150, 150, 151, 152, 152, 153, 154, 155, 155, 156, 157, 157, 158, 159, 
	160, 160, 161, 162, 162, 163, 164, 165, 165, 166, 167, 167, 168, 169, 170, 170, 
	171, 172, 172, 173, 174, 175, 175, 176, 177, 177, 178, 179, 180, 180, 181, 182
};

const int gTableU2B[]  =
{
	0,   1,   3,   5,   7,   8,  10,  12,  14,   15,  17,  19,  21,  23,  24,  26, 
	28,  30,  31,  33,  35,  37,  39,  40,  42,  44,  46,  47,  49,  51,  53,  54, 
	56,  58,  60,  62,  63,  65,  67,  69,  70,  72,  74,  76,  78,  79,  81,  83, 
	85,  86,  88,  90,  92,  93,  95,  97,  99,  101, 102, 104, 106, 108, 109, 111, 
	113, 115, 117, 118, 120, 122, 124, 125, 127, 129, 131, 133, 134, 136, 138, 140, 
	141, 143, 145, 147, 148, 150, 152, 154, 156, 157, 159, 161, 163, 164, 166, 168, 
	170, 172, 173, 175, 177, 179, 180, 182, 184, 186, 187, 189, 191, 193, 195, 196, 
	198, 200, 202, 203, 205, 207, 209, 211, 212, 214, 216, 218, 219, 221, 223, 225, 
	227, 228, 230, 232, 234, 235, 237, 239, 241, 242, 244, 246, 248, 250, 251, 253, 
	255, 257, 258, 260, 262, 264, 266, 267, 269, 271, 273, 274, 276, 278, 280, 281, 
	283, 285, 287, 289, 290, 292, 294, 296, 297, 299, 301, 303, 305, 306, 308, 310, 
	312, 313, 315, 317, 319, 320, 322, 324, 326, 328, 329, 331, 333, 335, 336, 338, 
	340, 342, 344, 345, 347, 349, 351, 352, 354, 356, 358, 360, 361, 363, 365, 367, 
	368, 370, 372, 374, 375, 377, 379, 381, 383, 384, 386, 388, 390, 391, 393, 395, 
	397, 399, 400, 402, 404, 406, 407, 409, 411, 413, 414, 416, 418, 420, 422, 423, 
	425, 427, 429, 430, 432, 434, 436, 438, 439, 441, 443, 445, 446, 448, 450, 452
};

int Rgb2Y(int *bgr)
{
	int val;

	val = (77 * bgr[2] + 150 * bgr[1] + 29 * bgr[0]) >> 8;	//y
	return val > 255 ? 255 : ( val < 0 ? 0 : val );
}

int Rgb2U(int *bgr)
{
	int val;

	val = ((128 * bgr[0] - 43 * bgr[2] - 85 * bgr[1]) >> 8) + (256 >> 1); //u
	return val > 255 ? 255 : ( val < 0 ? 0 : val );
}

int Rgb2V(int *bgr)
{
	int val;

	val = ((128 * bgr[2] - 107 * bgr[1] - 21 * bgr[0]) >> 8) + (256 >> 1); //v
	return val > 255 ? 255 : ( val < 0 ? 0 : val );
}

void Rgb2Yuv(int *bgr, int *yuv)
{
	yuv[0] = Rgb2Y(bgr);
	yuv[1] = Rgb2U(bgr);
	yuv[2] = Rgb2V(bgr);
}

int	 Yuv2R(int *yuv)
{
	int val;

//  	val = yuv[0] + ( (359 * yuv[2]) >> 8 ) - 179;		//r
	val = yuv[0] + gTableV2R[yuv[2]] - 179;

	//val = (yuv[2] * 359) >> 8;
	//val += yuv[0];
	//if(val > 0x1ff)
	//	val -= 0x1ff;
	//val -= 179;

	return val > 255 ? 255 : ( val < 0 ? 0 : val );
}

int	 Yuv2G(int *yuv)
{
	int val;

   	//val = yuv[0] +   135 - ( (88 * yuv[1] + 183 * yuv[2]) >> 8 );	//g
	val = yuv[0] + 135 - (gTableU2G[yuv[1]] + gTableV2G[yuv[2]]);
	return val > 255 ? 255 : ( val < 0 ? 0 : val );
}

int	 Yuv2B(int *yuv)
{
	int val;

   	//val = yuv[0] + ( (454 * yuv[1]) >> 8 ) - 227;	//b
	val = yuv[0] + gTableU2B[yuv[1]] - 227;
	return val > 255 ? 255 : ( val < 0 ? 0 : val );
}

void Yuv2Rgb(int *yuv, int *bgr)
{
	bgr[2] = Yuv2R(yuv);
	bgr[1] = Yuv2G(yuv);
	bgr[0] = Yuv2B(yuv);
}

//
void GetBgr(char *src, int *bgr)
{
	//int i;
	//char *psrc = src;

	//for(i = 0; i < 3; i++, psrc += unit)
	//	bgr[i] = this->getvalue(psrc, unit);
	BaseGetValue(src, bgr, 3);
}

void GetYuv444(char *src, int *yuv)
{
	//int i;
	//char *psrc = src;

	//for(i = 0; i < 3; i++, psrc += unit)
	//	yuv[i] = this->getvalue(psrc, unit);
	BaseGetValue(src, yuv, 3);
}

void GetYuv422(char *src, int *yuv, int order)
{
	int i;
	char *psrc = src;

	for(i = 0; i < 4; i++, psrc++)
		yuv[gStreamOrder[order][i]] = BaseGetValueOne(psrc);
}

void GetYuvFrom422(char *src, int *yuv, int order)
{
	GetYuv422(src, yuv, order);
	yuv[4] = yuv[1];
	yuv[5] = yuv[2];
}

void GetYuv400(char *src, int *yuv)
{
	yuv[0] = BaseGetValueOne(src);
}

void GetYuvFrom400(char *src, int *yuv)
{
	yuv[0] = BaseGetValueOne(src);
	yuv[1] = 0x80;
	yuv[2] = 0x80;
}

void GetYuv411(char *src, int *yuv, int order)
{
	int i;
	char *psrc = src;

	for(i = 0; i < 6; i++, psrc++)
		yuv[gStreamOrder[order][i]] = BaseGetValueOne(psrc);
}

void GetYuvFrom411(char *src, int *yuv, int order)
{
	GetYuv411(src, yuv, order);
	yuv[4] = yuv[1];
	yuv[5] = yuv[2];
	yuv[7] = yuv[1];
	yuv[8] = yuv[2];
	yuv[10] = yuv[1];
	yuv[11] = yuv[2];
}

void GetYuv420(char *src, int *yuv, int order, int offset, int gap)
{
	int i;
	char *psrc = src + offset;

	for(i = 0; i < 4; i++, psrc++)
		yuv[gStreamOrder[order][i]] = BaseGetValueOne(psrc);
	yuv[6] = BaseGetValueOne(src+gap+(offset >> 1));
	yuv[9] = BaseGetValueOne(src+gap+(offset >> 1) + 1);
}

void SetYuv420(char *dst, int *yuv, int order, int offset, int gap)
{
	int i;
	char *pdst = dst + offset;

	for(i = 0; i < 4; i++, pdst++)
		BaseSetValueOne(pdst, yuv[gStreamOrder[order][i]]);
	BaseSetValueOne(dst+gap+(offset >> 1), yuv[6]);
	BaseSetValueOne(dst+gap+(offset >> 1) + 1, yuv[9]);
}

void GetYuvFrom420(char *src, int *yuv, int order, int offset, int gap)
{
	GetYuv420(src, yuv, order, offset, gap);
	yuv[4] = yuv[1];
	yuv[5] = yuv[2];
	yuv[7] = yuv[1];
	yuv[8] = yuv[2];
	yuv[10] = yuv[1];
	yuv[11] = yuv[2];
}

void SetBgr(char *dst, int *bgr)
{
	//int i;
	//char *pdst = dst;

	//for(i = 0; i < 3; i++, pdst += unit)
	//	CBaseTools::setvalue(pdst, bgr[i], unit);
	BaseSetValue(dst, bgr, 3);
}

void SetYuv444(char *dst, int *yuv)
{
	//int i;
	//char *pdst = dst;

	//for(i = 0; i < 3; i++, pdst += unit)
	//	CBaseTools::setvalue(pdst, yuv[i], unit);
	BaseSetValue(dst, yuv, 3);
}

void SetYuv422(char *dst, int *yuv, int order)
{
	int i;
	char *pdst = dst;

	for(i = 0; i < 4; i++, pdst++)
		BaseSetValueOne(pdst, yuv[gStreamOrder[order][i]]);
}

void SetYuv411(char *dst, int *yuv, int order)
{
	int i;
	char *pdst = dst;

	for(i = 0; i < 6; i++, pdst++)
		BaseSetValueOne(pdst, yuv[gStreamOrder[order][i]]);
}

int Yuv2RgbPrefilter(int *dat, int *filter, int count)
{
	//int k;

	//dat[count] *= filter[0];
	//for(k = 0; k < count; k++)
	//{
	//	dat[k] *= filter[count-k];
	//	dat[(count << 1) - k] *= filter[count-k];
	//	dat[count] += dat[k] + dat[(count << 1) - k];
	//}
	//return dat[count];
	BasePrefilter(dat, 1, filter, count);
	return dat[count];
}

void Yuv2RgbRgbPrefilter(int *dat, int *filter, int count)
{
	//int i, k, start, end, offset;

	//offset = count * 3;
	//for(i = 0; i < 3; i++)
	//	dat[offset + i] *= filter[0];

	//for(k = 0, start = 0, end = (count << 1) * 3; k < count; k++, start += 3, end -= 3)
	//{
	//	for(i = 0; i < 3; i++)
	//	{
	//		dat[start + i] *= filter[count-k];
	//		dat[end + i] *= filter[count-k];
	//		dat[offset + i] += dat[start + i] + dat[end + i];
	//	}
	//}
	BasePrefilter(dat, 3, filter, count);
}


/***************************************** yuv400 **********************************************/

int Yuv400HorExtent(char *src, char *dest, int width, int height, int count)
{
	int i, j, size, val, pos;
	char *psrc, *pdst;

	size = width * height;
	if(count == 1)
	{
		memcpy(dest, src, size);
		return size;
	}
	psrc = src;
	pdst = dest;
	count--;
	size = width;
	for(i = 0; i < height; i++, psrc += size)
	{
		for(j = 0, pos = count; j < count; j++, pdst++, pos--)
		{
			val = BaseGetValueOne(psrc+pos);
			BaseSetValueOne(pdst, val);
		}
		memcpy(pdst, psrc, size);
		pdst += size;
		for(j = 0, pos = (width-2); j < count; j++, pdst++, pos--)
		{
			val = BaseGetValueOne(psrc+pos);
			BaseSetValueOne(pdst, val);
		}
	}
	return (width + (count << 1)) * height;
}

int Yuv400VerExtent(char *src, char *dest, int width, int height, int count)
{
	int i, size, step;
	char *psrc, *pdst;

	size = width * height;
	if(count == 1)
	{
		memcpy(dest, src, size);
		return size;
	}
	count--;
	step = width;
	memcpy(dest + step * count, src, size);
	psrc = src + (step * count);
	pdst = dest;
	for(i = 0; i < count; i++, psrc -= step, pdst += step)
		memcpy(pdst, psrc, step);
	psrc = src + size - (step << 1);
	pdst += size;
	for(i = 0; i < count; i++, psrc -= step, pdst += step)
		memcpy(pdst, psrc, step);
	return width * (height + (count << 1));
}

int MYuv400Enlarge(char *src, char *dest, int width, int height, int count)
{
	Yuv400HorExtent(src, dest, width, height, count);
	width += (count - 1) << 1;
	Yuv400VerExtent(dest, src, width, height, count);
	height += (count - 1) << 1;
	width *= height;
	memcpy(dest, src, width);
	return width;
}

int Yuv400HorPrefilter(char *src, char *dest, int width, int height, 
								int count, int *filter)
{
	int i, j, k, size, total, val, m, n;
	char *psrc, *pdst, *psrc1;
	int dat[MAXFILTERCOUNT];

	if(count > (MAXFILTERCOUNT >> 1))
		return 0;
	size = width * height;
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
	size = (count) << 1;
	m = width - count;
	n = (count << 1) + 1;
	for(i = 0; i < height; i++)
	{
		for(j = count; j < m; j++, pdst++, psrc++)
		{
			for(k = 0, psrc1 = psrc; k < n; k++, psrc1++)
				dat[k] = BaseGetValueOne(psrc1);
			val = Yuv2RgbPrefilter(dat, filter, count);
			val /= total;
			BaseSetValueOne(pdst, val);
		}
		psrc += size;
	}
	width -= count << 1;
	return width * height;
}

int Yuv400VerPrefilter(char *src, char *dest, int width, int height, 
								int count, int *filter)
{
	int i, j, k, size, total, val, m, n;
	char *psrc, *pdst, *pdst1, *psrc1;
	int dat[MAXFILTERCOUNT];

	if(count > (MAXFILTERCOUNT >> 1))
		return 0;
	size = width * height;
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
	size = width;
	count--;
	m = height - count;
	n = (count << 1) + 1;
	for(i = 0; i < width; i++, psrc++, pdst++)
	{
		for(j = count, pdst1 = pdst, psrc1 = psrc; j < m; j++, pdst1 += size, psrc1 += size)
		{
			for(k = 0, total = 0; k < n; k++, total += size)
				dat[k] = BaseGetValueOne(psrc1+total);
			val = Yuv2RgbPrefilter(dat, filter, count);
			val /= total;
			BaseSetValueOne(pdst1, val);
		}
	}
	height -= count << 1;
	return width * height;
}

int MYuv400Prefilter(char *src, char *dest, int width, int height, 
					int count, int *filter)
{
	MYuv400Enlarge(src, dest, width, height, count);
	Yuv400HorPrefilter(src, dest, width + ((count - 1) << 1), height + ((count - 1) << 1), 
			count, filter);
	Yuv400VerPrefilter(dest, src, width, height + ((count - 1) << 1), count, filter);
	width *= height;
	memcpy(src, dest, width);
	return width;
}

int MRgb2Yuv400(char *src, char *dest, int width, int height)
{
	int i, size;

	size = 3;
	height *= width * 3;
	width = 0;
	for(i = 0; i < height; i += size)
		width += Rgb2Yuv400(src + i, dest + width);
	return width;
}

int MYuv400_Rgb(char *src, char *dest, int width, int height)
{
	int i;

	height *= width;
	width = 0;
	for(i = 0; i < height; i++)
		width += Yuv400_Rgb(src + i, dest + width);
	return width;
}

int MYuv444_400(char *src, char *dest, int width, int height)
{
	int i, size;

	size = 3;
	height *= width * 3;
	width = 0;
	for(i = 0; i < height; i += size)
		width += Yuv444_400(src + i, dest + width);
	return width;
}

int MYuv400_444(char *src, char *dest, int width, int height)
{
	int i;

	height *= width;
	width = 0;
	for(i = 0; i < height; i++)
		width += Yuv400_444(src + i, dest + width);
	return width;
}

int MYuv400Sizer(char *src, char *dest, int srcw, int dstw, int dsth, int px, int py)
{
	int i, size, size1, offset;
	char *psrc, *pdst;

	size = srcw;
	size1 = dstw;
	offset = px;

	psrc = src + size * py;
	pdst = dest;
	for(i = 0; i < dsth; i++, psrc += size, pdst += size1)
		memcpy(pdst, psrc+offset, size1);

	return size1 * dsth;
}

int Rgb2Yuv400(char *src, char *dst)
{
	int y, bgr[3];

	GetBgr(src, bgr);

	y = Rgb2Y(bgr);

	BaseSetValueOne(dst, y);

	return 1;
}

int Yuv400_Rgb(char *src, char *dst)
{
	int yuv[3], bgr[3];

	GetYuvFrom400(src, yuv);

	Yuv2Rgb(yuv, bgr);

	SetBgr(dst, bgr);

	return 3;
}

int Yuv444_400(char *src, char *dst)
{
	int yuv[3];

	GetYuv444(src, yuv);

	BaseSetValueOne(dst, yuv[0]);

	return 1;
}

int Yuv400_444(char *src, char *dst)
{
	int yuv[3];

	GetYuvFrom400(src, yuv);

	SetYuv444(dst, yuv);

	return 3;
}

int	Yuv2Blk(char *src, char *dst, int width, int height)
{
	int i, j, offset;
	int y;
	int pos, blkwidth;
	char *psrc = src;

	blkwidth = (((width + 7) >> 3) << 6);
	for(i = 0; i < height; i++)
	{
		offset = ((i >> 3) * blkwidth) + ((i % 8) << 3);
		for(j = 0; j < width; j++, psrc++)
		{
			y = BaseGetValueOne(psrc);

			pos = (((j >> 3) << 6) + (j % 8));
			pos += offset;

			BaseSetValueOne(dst+pos, y);
		}
	}
	return blkwidth * ((height + 7) >> 3);
}

int	Blk2Yuv(char *src, char *dst, int width, int height)
{
	int i, j, offset;
	int y;
	int pos, blkwidth;
	char *pdst = dst;

	blkwidth = (((width + 7) >> 3) << 6);
	for(i = 0; i < height; i++)
	{
		offset = ((i >> 3) * blkwidth) + ((i % 8) << 3);
		for(j = 0; j < width; j++, pdst++)
		{
			pos = ( ((j >> 3) << 6) + (j % 8) );
			pos += offset;
			y = BaseGetValueOne(src+pos);

			BaseSetValueOne(pdst, y);
		}
	}
	return blkwidth * ((height + 7) >> 3);
}

int Yuv2Lbuf(char *src, char *dst, int width, int height)
{
	width *= height;
	memcpy(dst, src, width);
	return width;
}

int	Lbuf2Yuv(char *src, char *dst, int width, int height)
{
	width *= height;
	memcpy(dst, src, width);
	return width;
}

//======================================================
//
//			Yuv image resize
//
//=================================================
void Yuv400ZoutHorResize(char *src, char *dst, int srcw, int srch, int dstw)
{
	int i, j, up, down;
	int val0, val1, yuv;
	char *psrc = src;
	char *pdst = dst;

	if(srcw == dstw)
	{
		if(dst != src)
			memcpy(dst, src, srcw * srch);
		return;
	}
	for(j = 0; j < srch; j++)
	{
		yuv = BaseGetValueOne(psrc);
		BaseSetValueOne(pdst, yuv);
		psrc++;
		pdst++;
		up = dstw - 1;
		for(i = 1; i < srcw; i++, up += dstw - 1, psrc++)
		{
			if(up < srcw - 1)
				continue;
			up -= srcw - 1;
			down = dstw - 1 - up;
			if(up == 0)
			{
				yuv = BaseGetValueOne(psrc);
				BaseSetValueOne(pdst, yuv);
			}
			else
			{
				val0 = BaseGetValueOne(psrc - 1);
				val1 = BaseGetValueOne(psrc);
				yuv = (val0 * down + val1 * up) / (dstw - 1);
				BaseSetValueOne(pdst, yuv);
			}
			pdst++;
		}
	}
}

void Yuv400ZinHorResize(char *src, char *dst, int srcw, int srch, int dstw)
{
	int i, j, up, down, gap, offset = 0;
	int val0, val1, yuv;
	char *psrc = src;
	char *pdst = dst;

	if(srcw == dstw)
	{
		if(dst != src)
			memcpy(dst, src, srcw * srch);
		return;
	}
	gap = srcw;
	for(j = 0; j < srch; j++, psrc += gap)
	{
		up = 0;
		offset = 0;
		for(i = 1; i < dstw; i++, up += srcw, pdst++)
		{
			if(up >= dstw + 1)
			{
				up -= dstw + 1;
				offset++;
			}
			down = dstw + 1 - up;
			if(up == 0)
			{
				yuv = BaseGetValueOne(psrc + offset);
				BaseSetValueOne(pdst, yuv);
			}
			else
			{
				val0 = BaseGetValueOne(psrc + offset);
				val1 = BaseGetValueOne(psrc + offset + 1);
				yuv = (val0 * down + val1 * up) / (dstw + 1);
				BaseSetValueOne(pdst, yuv);
			}
		}
	}
}

void Yuv400ZoutVerResize(char *src, char *dst, int srcw, int srch, int dsth)
{
	int i, j, up, down, gap;
	int yuv, val0, val1;
	char *psrc = src;
	char *pdst = dst;

	if(srch == dsth)
	{
		if(dst != src)
			memcpy(dst, src, srcw * srch);
		return;
	}
	up = srch + 1;
	gap = srcw;
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
		for(i = 0; i < srcw; i++, psrc++, pdst++)
		{
			if(up == 0)
			{
				yuv = BaseGetValueOne(psrc);
				BaseSetValueOne(pdst, yuv);
			}
			else
			{
				val0 = BaseGetValueOne(psrc + gap);
				val1 = BaseGetValueOne(psrc);
				yuv = (val0 * down + val1 * up) / (dsth - 1);
				BaseSetValueOne(pdst, yuv);
			}
		}
	}
}

void Yuv400ZinVerResize(char *src, char *dst, int srcw, int srch, int dsth)
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
	gap = srcw;
	for(j = 0; j < dsth; j++, up += srch)
	{
		if(up >= (dsth+1))
		{
			up -= dsth + 1;
			psrc += gap;
		}
		down = dsth + 1 - up;
		for(i = 0; i < gap; i++, pdst++)
		{
			if(up == 0)
			{
				yuv = BaseGetValueOne(psrc + i);
				BaseSetValueOne(pdst, yuv);
			}
			else
			{
				val0 = BaseGetValueOne(psrc + i);
				val1 = BaseGetValueOne(psrc + gap + i);
				yuv = (val0 * down + val1 * up) / (dsth + 1);
				BaseSetValueOne(pdst, yuv);
			}
		}
	}
}

int Yuv400Resize(char *src, char *dst, int srcw, int srch, int dstw, int dsth)
{
	int len = dstw * dsth;

	if(dsth <= srch)
	{
		Yuv400ZoutVerResize(src, src, srcw, srch, dsth);
		if(dstw <= srcw)
			Yuv400ZoutHorResize(src, dst, srcw, dsth, dstw);
		else
			Yuv400ZinHorResize(src, dst, srcw, dsth, dstw);
	}
	else
	{
		Yuv400ZinVerResize(src, dst, srcw, srch, dsth);
		if(dstw <= srcw)
			Yuv400ZoutHorResize(dst, src, srcw, dsth, dstw);
		else
			Yuv400ZinHorResize(dst, src, srcw, dsth, dstw);
		memcpy(dst, src, len);
	}

	return len;
}


/***************************************** yuv411 **********************************************/

int Yuv411_Resizeorder(char *src, char *dst, int srcw, int srch, int dstw, 
							   int dsth, int order)
{
	MYuv411_444order(src, dst, srcw, srch, order);
	Yuv444Resize(dst, src, srcw, srch, dstw, dsth);
	return MYuv444_411order(src, dst, dstw, dsth, order);
}

int MYuv411Sizer(char *src, char *dest, int srcw, int dstw, int dsth, 
								 int px, int py, int order)
{
	int i, j, size, size1, offset, step = 6, step1 = step << 1;
	char *psrc, *pdst, *psrc1, *pdst1;
	char pool[BUFFER_DEFAULT_SIZE];

	size = (srcw * 3) >> 1;
	size1 = (dstw * 3) >> 1;
	offset = (px * 3) >> 1;
	psrc = src + size * py;
	pdst = dest;

	for(i = 0; i < dsth; i++, psrc += size)
	{
		if(px % 4)
		{
			psrc1 = psrc;
			pdst1 = pool;
			for(j = 0; j < srcw; j += 4, psrc1 += step, pdst1 += step1)
				Yuv411_444order(psrc1, pdst1, order);
			psrc1 = pool + (px * 3);
			for(j = 0; j < dstw; j += 4, psrc1 += step1, pdst += step)
				Yuv444_411order(psrc1, pdst, order);
			offset = 0;
		}
		else
			memcpy(pdst, psrc + offset, size1);
	}
	return size1 * py;
}

int MRgb2Yuv411order(char *src, char *dst, int width, int height, int order)
{
	int i, size;

	size = 3 * 4;
	height *= width * 3;
	width = 0;
	for(i = 0; i < height; i += size)
		width += Rgb2Yuv411order(src+i, dst + width, order);
	return width;
}

int MYuv411_Rgborder(char *src, char *dst, int width, int height, int order)
{
	int i, size;

	size = 6;
	height *= width * 3;
	height >>= 1;
	width = 0;
	for(i = 0; i < height; i += size)
		width += Rgb2Yuv411order(src+i, dst + width, order);
	return width;
}

int MYuv444_411order(char *src, char *dst, int width, int height, int order)
{
	int i, size;

	size = 3 * 4;
	height *= width * 3;
	width = 0;
	for(i = 0; i < height; i += size)
		width += Yuv444_411order(src+i, dst + width, order);
	return width;
}

int MYuv411_444order(char *src, char *dst, int width, int height, int order)
{
	int i, size;

	size = 6;
	height *= width * 3;
	height >>= 1;
	width = 0;
	for(i = 0; i < height; i += size)
		width += Yuv411_444order(src+i, dst + width, order);
	return width;
}

int Rgb2Yuv411order(char *src, char *dst, int order)
{
	int i, step = 3;
	int yuv[12], bgr[3];
	char *psrc = src;

	for(i = 0; i < 12; i += 3, psrc += step)
	{
		GetBgr(psrc, bgr);
		Rgb2Yuv(bgr, yuv+i);
	}

	yuv[1] += yuv[4] + yuv[7] + yuv[10];
	yuv[2] += yuv[5] + yuv[8] + yuv[11];
	yuv[1] >>= 2;
	yuv[2] >>= 2;

	SetYuv411(dst, yuv, order);

	return 6;
}

int Yuv411_Rgborder(char *src, char *dst, int order)
{
	int i, step = 3;
	char *pdst = dst;
	int yuv[12], bgr[3];

	GetYuvFrom411(src, yuv, order);

	for(i = 0; i < 12; i += 3, pdst += step)
	{
		Yuv2Rgb(yuv+i, bgr);
		SetBgr(pdst, bgr);
	}
	return 12;
}

int Yuv444_411order(char *src, char *dst, int order)
{
	int i, step = 3;
	int yuv[12];
	char *psrc = src;

	for(i = 0; i < 12; i += 3, psrc += step)
		GetYuv444(psrc, yuv+i);

	yuv[1] += yuv[4] + yuv[7] + yuv[10];
	yuv[2] += yuv[5] + yuv[8] + yuv[11];
	yuv[1] >>= 2;
	yuv[2] >>= 2;

	SetYuv411(dst, yuv, order);

	return 6;
}

int Yuv411_444order(char *src, char *dst, int order)
{
	int i, step = 3;
	char *pdst = dst;
	int yuv[12];

	GetYuvFrom411(src, yuv, order);

	for(i = 0; i < 12; i += 3, pdst += step)
		SetYuv444(pdst, yuv+i);
	return 12;
}

int Yuv411_Blkorder(char *src, char *dst, int order, int width, int height)
{
	int i, j, offset, step = 6;
	int yuv[12];
	int pos, blkwidth;
	char *psrc = src;

	blkwidth = ( (((width + 31) >> 5) * 3) << 7 );
	for(i = 0; i < height; i++)
	{
		offset = ((i >> 3) * blkwidth) + (((i % 8) << 3));
		for(j = 0; j < width; j += 4, psrc += step)
		{
			GetYuv411(psrc, yuv, order);

			pos = (((j >> 5) * 384) + (((j % 32) >> 3) << 6) + (j % 8));
			pos += offset;
			BaseSetValueOne(dst+pos, yuv[0]);
			BaseSetValueOne(dst+pos+1, yuv[3]);
			BaseSetValueOne(dst+pos+2, yuv[6]);
			BaseSetValueOne(dst+pos+3, yuv[9]);

			pos = (((j >> 5) * 384) + 256 + ((j % 32) >> 2));
			pos += offset;
			BaseSetValueOne(dst+pos, yuv[1]);
			BaseSetValueOne(dst+pos+64, yuv[2]);
		}
	}
	return blkwidth * ((height + 7) >> 3);
}

int Blk2Yuv411order(char *src, char *dst, int order, int width, int height)
{
	int i, j, offset, step = 6;
	int yuv[12];
	int pos, blkwidth;
	char *pdst = dst;

	blkwidth = ( (((width + 31) >> 5) * 3) << 7 );
	for(i = 0; i < height; i++)
	{
		offset = ((i >> 3) * blkwidth) + (((i % 8) << 3));
		for(j = 0; j < width; j += 4, pdst += step)
		{
			pos = (((j >> 5) * 384) + (((j % 32) >> 3) << 6) + (j % 8));
			pos += offset;
			yuv[0] = BaseGetValueOne(src+pos);
			yuv[3] = BaseGetValueOne(src+pos+1);
			yuv[6] = BaseGetValueOne(src+pos+2);
			yuv[9] = BaseGetValueOne(src+pos+3);

			pos = (((j >> 5) * 384) + 256 + ((j % 32) >> 2));
			pos += offset;
			yuv[1] = BaseGetValueOne(src+pos);
			yuv[2] = BaseGetValueOne(src+pos+64);

			SetYuv411(pdst, yuv, order);
		}
	}
	return blkwidth * ((height + 7) >> 3);
}

int Yuv411_Lbuforder(char *src, char *dst, int order, int width, int height)
{
	int i, j, k, ypos, upos, vpos, step = 6;
	int yuv[12];
	int blkwidth;
	char *psrc = src;

	blkwidth = ( (((width + 31) >> 5) * 3) << 7 );
	for(i = 0; i < height; i += 8)
	{
		ypos = (i >> 3) * blkwidth;
		upos = ypos + ((blkwidth << 1) / 3);
		vpos = upos + (blkwidth / 6);
		k = ((height - i) < 8) ? (height - i) : 8;
		for(; k; k--)
		{
			for(j = 0; j < width; j += 4, psrc += step , ypos += 4, upos++, vpos++)
			{
				GetYuv411(psrc, yuv, order);
				BaseSetValueOne(dst + ypos, yuv[0]);
				BaseSetValueOne(dst + ypos + 1, yuv[3]);
				BaseSetValueOne(dst + ypos + 2, yuv[6]);
				BaseSetValueOne(dst + ypos + (step >> 1), yuv[9]);
				BaseSetValueOne(dst + upos, yuv[1]);
				BaseSetValueOne(dst + vpos, yuv[2]);
			}
		}
	}
	return blkwidth * ((height + 7) >> 3);
}

int Lbuf2Yuv411order(char *src, char *dst, int order, int width, int height)
{
	int i, j, k, ypos, upos, vpos, step = 6;
	int yuv[12];
	int blkwidth;
	char *pdst = dst;

	blkwidth = ( (((width + 31) >> 5) * 3) << 7 );
	for(i = 0; i < height; i += 8)
	{
		ypos = (i >> 3) * blkwidth;
		upos = ypos + ((blkwidth << 1) / 3);
		vpos = upos + (blkwidth / 6);
		k = ((height - i) < 8) ? (height - i) : 8;
		for(; k; k--)
		{
			for(j = 0; j < width; j += 4, pdst += step, ypos += 4, upos++, vpos++)
			{
				yuv[0] = BaseGetValueOne(src + ypos);
				yuv[3] = BaseGetValueOne(src + ypos + 1);
				yuv[6] = BaseGetValueOne(src + ypos + 2);
				yuv[9] = BaseGetValueOne(src + ypos + (step >> 1));
				yuv[1] = BaseGetValueOne(src + upos);
				yuv[2] = BaseGetValueOne(src + vpos);
				SetYuv411(pdst, yuv, order);
			}
		}
	}
	return blkwidth * ((height + 7) >> 3);
}


/***************************************** yuv420 **********************************************/

int Yuv420_Resizeorder(char *src, char *dst, int srcw, int srch, int dstw, int dsth, int order)
{
	MYuv420_444order(src, dst, srcw, srch, order);
	Yuv444Resize(dst, src, srcw, srch, dstw, dsth);
	return MYuv444_420order(src, dst, dstw, dsth, order);
}

int MRgb2Yuv420order(char *src, char *dest, int width, int height, int order)
{
	int i, size, len = 0;

	size = width * 6;
	height *= width * 3;
	for(i = 0; i < height; i += size)
		len += Rgb2Yuv420order(src+i, dest+len, width, order);
	return len;
}

int MYuv420_Rgborder(char *src, char *dest, int width, int height, int order)
{
	int i, size, len = 0;

	size = width * 3;
	height *= width * 3;
	height >>= 1;
	for(i = 0; i < height; i += size)
		len += Yuv420_Rgborder(src+i, dest+len, width, order);
	return len;
}

int MYuv444_420order(char *src, char *dest, int width, int height, int order)
{
	int i, size, len = 0;

	size = width * 6;
	height *= width * 3;
	for(i = 0; i < height; i += size)
		len += Yuv444_420order(src+i, dest+len, width, order);
	return len;
}

int MYuv420_444order(char *src, char *dest, int width, int height, int order)
{
	int i, size, len = 0;

	size = width * 3;
	height *= width * 3;
	height >>= 1;
	for(i = 0; i < height; i += size)
		len += Yuv420_444order(src+i, dest+len, width, order);
	return len;
}

int Rgb2Yuv420order(char *src, char *dst, int width, int order)
{
	int i, j, step = 3;
	int yuv[12], bgr[3];
	int gap1, gap2, offset;
	char *pbuf, *psrc = src;

	gap1 = width * 3;
	gap2 = (width << 1);
	for(i = 0, offset = 0; i < width; i += 2, psrc += step << 1, offset += 4)
	{
		for(j = 0, pbuf = psrc; j < 6; j += 3, pbuf += step)
		{
			GetBgr(pbuf, bgr);
			Rgb2Yuv(bgr, yuv+j);
		}
		for(j = 6, pbuf = psrc + gap1; j < 12; j += 3, pbuf += step)
		{
			GetBgr(pbuf, bgr);
			Rgb2Yuv(bgr, yuv+j);
		}

		yuv[1] += yuv[4] + yuv[7] + yuv[10];
		yuv[2] += yuv[5] + yuv[8] + yuv[11];
		yuv[1] >>= 2;
		yuv[2] >>= 2;

		SetYuv420(dst, yuv, order, offset, gap2);
	}
	return width * 3;
}

int Yuv420_Rgborder(char *src, char *dst, int width, int order)
{
	int i, j, offset, gap1, gap2, step = 3;
	int yuv[12], bgr[12];
	char *pdst = dst;

	gap1 = (width << 1);
	gap2 = (width * 3);
	for(i = 0, offset = 0; i < width; i += 2, offset += 4)
	{
		GetYuvFrom420(src, yuv, order, offset, gap1);

		for(j = 0; j < 12; j += 3)
			Yuv2Rgb(yuv+j, bgr+j);

		for(j = 0; j < 6; j += 3, pdst += step)
		{
			SetBgr(pdst, bgr+j);
			SetBgr(pdst + gap2, bgr+6+j);
		}
	}
	return width * 6;
}

int Yuv420_444order(char *src, char *dst, int width, int order)
{
	int i, j, offset, gap1, gap2, step = 3;
	int yuv[12];
	char *pdst = dst;

	gap1 = (width << 1);
	gap2 = (width * 3);
	for(i = 0, offset = 0; i < width; i += 2, offset += 4)
	{
		GetYuvFrom420(src, yuv, order, offset, gap1);

		for(j = 0; j < 6; j += 3, pdst += step)
		{
			SetYuv444(pdst, yuv+j);
			SetYuv444(pdst + gap2, yuv+6+j);
		}
	}
	return width * 6;
}

int Yuv444_420order(char *src, char *dst, int width, int order)
{
	int i, j, step = 3;
	int yuv[12];
	int gap1, gap2, offset;
	char *psrc = src;

	gap1 = width * 3;
	gap2 = (width << 1);
	for(i = 0, offset = 0; i < width; i += 2, offset += 4)
	{
		for(j = 0; j < 6; j += 3, psrc += step)
		{
			GetYuv444(psrc, yuv+j);
			GetYuv444(psrc + gap1, yuv+6+j);
		}

		yuv[1] += yuv[4] + yuv[7] + yuv[10];
		yuv[2] += yuv[5] + yuv[8] + yuv[11];
		yuv[1] >>= 2;
		yuv[2] >>= 2;

		SetYuv420(dst, yuv, order, offset, gap2);
	}
	return width * 3;
}

int Yuv420_Blkorder(char *src, char *dst, int order, int width, int height)
{
	int i, j, yoffset, uoffset, pos1, gap1, gap2;
	int yuv[12];
	int pos, blkwidth;
	char *psrc = src;

	gap1 = width * 3;
	gap2 = width << 1;
	blkwidth = ((((width + 15) >> 4) * 3) << 7);
	for(i = 0; i < height; i += 2, psrc += gap1)
	{
		yoffset = ((i >> 4) * blkwidth) + (((i % 16) >> 3) << 7) + ((i % 8) << 3);
		uoffset = ((i >> 4) * blkwidth) + (((i >> 1) % 8) << 3);
		pos1 = 0;
		for(j = 0; j < width; j += 2, pos1 += 4)
		{
			GetYuv420(psrc, yuv, order, pos1, gap2);

			pos = (((j >> 4) * 384) + (((j % 16) >> 3) << 6) + (j % 8));
			pos += yoffset;
			BaseSetValueOne(dst+pos, yuv[0]);
			BaseSetValueOne(dst+pos+1, yuv[3]);
			BaseSetValueOne(dst+pos+8, yuv[6]);
			BaseSetValueOne(dst+pos+9, yuv[9]);

			pos = (((j >> 4) * 384) + 256 + ((j >> 1) % 8));
			pos += uoffset;

			BaseSetValueOne(dst+pos, yuv[1]);
			BaseSetValueOne(dst+pos+64, yuv[2]);
		}
	}
	return blkwidth * ((height + 15) >> 4);
}

int Blk2Yuv420order(char *src, char *dst, int order, int width, int height)
{
	int i, j, yoffset, uoffset, pos1, gap1, gap2;
	int yuv[12];
	int pos, blkwidth;
	char *pdst = dst;

	gap1 = width * 3;
	gap2 = width << 1;
	blkwidth = ((((width + 15) >> 4) * 3) << 7);
	for(i = 0; i < height; i += 2, pdst += gap1)
	{
		yoffset = ((i >> 4) * blkwidth) + (((i % 16) >> 3) << 7) + ((i % 8) << 3);
		uoffset = ((i >> 4) * blkwidth) + (((i >> 1) % 8) << 3);
		pos1 = 0;
		for(j = 0; j < width; j += 2, pos1 += 4)
		{
			pos = (((j >> 4) * 384) + (((j % 16) >> 3) << 6) + (j % 8));
			pos += yoffset;
			yuv[0] = BaseGetValueOne(src+pos);
			yuv[3] = BaseGetValueOne(src+pos+1);
			yuv[6] = BaseGetValueOne(src+pos+8);
			yuv[9] = BaseGetValueOne(src+pos+9);

			pos = (((j >> 4) * 384) + 256 + ((j >> 1) % 8));
			pos += uoffset;
			yuv[1] = BaseGetValueOne(src+pos);
			yuv[2] = BaseGetValueOne(src+pos+64);
			SetYuv420(pdst, yuv, order, pos1, gap2);
		}
	}
	return blkwidth * ((height + 15) >> 4);
}

int Yuv420_Lbuforder(char *src, char *dst, int order, int width, int height)
{
	int i, j, k, ypos, upos, vpos;
	int yuv[12];
	int blkwidth, gap1, gap2, pos1;
	char *psrc = src;

	blkwidth = ((((width + 15) >> 4) * 3) << 7);
	gap1 = width * 3;
	gap2 = width << 1;
	for(i = 0; i < height; i += 16)
	{
		ypos = (i >> 4) * blkwidth;
		upos = ypos + ((blkwidth << 1) / 3);
		vpos = upos + (blkwidth / 6);
		k = ((height - i) < 16) ? (height - i) : 16;
		for(; k; k -= 2, psrc += gap1)
		{
			for(j = 0, pos1 = 0; j < width; j += 2, pos1 += 4, ypos += 2, upos++, vpos++)
			{
				GetYuv420(psrc, yuv, order, pos1, gap2);
				BaseSetValueOne(dst + ypos, yuv[0]);
				BaseSetValueOne(dst + ypos + 1, yuv[3]);
				BaseSetValueOne(dst + ypos + (gap2 >> 1), yuv[6]);
				BaseSetValueOne(dst + ypos + (gap2 >> 1) + 1, yuv[9]);
				BaseSetValueOne(dst + upos, yuv[1]);
				BaseSetValueOne(dst + vpos, yuv[2]);
			}
			ypos += gap2 >> 1;
		}
	}
	return blkwidth * ((height + 15) >> 4);
}

int Lbuf2Yuv420order(char *src, char *dst, int order, int width, int height)
{
	int i, j, k, ypos, upos, vpos;
	int yuv[12];
	int blkwidth, gap1, gap2, pos1;
	char *pdst = dst;

	blkwidth = ((((width + 15) >> 4) * 3) << 7);
	gap1 = width * 3;
	gap2 = width << 1;
	for(i = 0; i < height; i += 16)
	{
		ypos = (i >> 4) * blkwidth;
		upos = ypos + ((blkwidth << 1) / 3);
		vpos = upos + (blkwidth / 6);
		k = ((height - i) < 16) ? (height - i) : 16;
		for(; k; k -= 2, pdst += gap1)
		{
			for(j = 0, pos1 = 0; j < width; j += 2, pos1 += 4, ypos += 2, upos++, vpos++)
			{
				yuv[0] = BaseGetValueOne(src + ypos);
				yuv[3] = BaseGetValueOne(src + ypos + 1);
				yuv[6] = BaseGetValueOne(src + ypos + (gap2 >> 1));
				yuv[9] = BaseGetValueOne(src + ypos + (gap2 >> 1) + 1);
				yuv[1] = BaseGetValueOne(src + upos);
				yuv[2] = BaseGetValueOne(src + vpos);
				SetYuv420(pdst, yuv, order, pos1, gap2);
			}
			ypos += gap2 >> 1;
		}
	}
	return blkwidth * ((height + 15) >> 4);
}


/***************************************** yuv422 **********************************************/

int Yuv422_Resizeorder(char *src, char *dst, int srcw, int srch, int dstw, int dsth, int order)
{
	MYuv422_444order(src, dst, srcw, srch, order);
	Yuv444Resize(dst, src, srcw, srch, dstw, dsth);
	return MYuv444_422order(src, dst, dstw, dsth, order);
}

int MYuv422Sizer(char *src, char *dest, int srcw, int dstw, int dsth, 
								 int px, int py, int order)
{
	int i, j, size, size1, offset, step = 4, step1 = 6;
	char *psrc, *pdst, *psrc1, *pdst1;
	char pool[BUFFER_DEFAULT_SIZE];

	size = srcw << 1;
	size1 = dstw << 1;
	offset = px << 1;
	psrc = src + size * py;
	pdst = dest;

	for(i = 0; i < dsth; i++, psrc += size)
	{
		if(px % 2)
		{
			psrc1 = psrc;
			pdst1 = pool;
			for(j = 0; j < srcw; j += 2, psrc1 += step, pdst1 += step1)
				Yuv422_444order(psrc1, pdst1, order);
			psrc1 = pdst + (px * 3);
			for(j = 0; j < dstw; j += 2, psrc1 += step1, pdst += step)
				Yuv444_422order(psrc1, pdst, order);
		}
		else
			memcpy(pdst, psrc + offset, size1);
	}
	return size1 * py;
}

int MRgb2Yuv422order(char *src, char *dest, int width, int height, int order)
{
	int i, size;

	size = 3 * 2;
	height *= width * 3;
	width = 0;
	for(i = 0; i < height; i += size)
		width += Rgb2Yuv422order(src + i, dest + width, order);
	return width;
}

int MYuv422_Rgborder(char *src, char *dest, int width, int height, int order)
{
	int i, size;

	size = 4;
	height *= width << 1;
	width = 0;
	for(i = 0; i < height; i += size)
		width += Yuv422_Rgborder(src + i, dest + width, order);
	return width;
}

int MYuv444_422order(char *src, char *dest, int width, int height, int order)
{
	int i, size;

	size = 3 * 2;
	height *= width * 3;
	width = 0;
	for(i = 0; i < height; i += size)
		width += Yuv444_422order(src + i, dest + width, order);
	return width;
}

int MYuv422_444order(char *src, char *dest, int width, int height, int order)
{
	int i, size;

	size = 4;
	height *= width << 1;
	width = 0;
	for(i = 0; i < height; i += size)
		width += Yuv422_444order(src + i, dest + width, order);
	return width;
}

int Rgb2Yuv422order(char *src, char *dst, int order)
{
	int i, step = 3;
	int bgr[3], yuv[6];
	char *psrc = src;

	for(i = 0; i < 6; i += 3, psrc += step)
	{
		GetBgr(psrc, bgr);
		Rgb2Yuv(bgr, yuv+i);
	}
	yuv[1] = (yuv[1] + yuv[4]) >> 1;
	yuv[2] = (yuv[2] + yuv[5]) >> 1;

	SetYuv422(dst, yuv, order);

	return 4;
}

int Yuv422_Rgborder(char *src, char *dst, int order)
{
	int i, step = 3;
	char *pdst = dst;
	int yuv[6], bgr[3];

	GetYuvFrom422(src, yuv, order);

	for(i = 0; i < 6; i += 3, pdst += step)
	{
		Yuv2Rgb(yuv + i, bgr);
		SetBgr(pdst, bgr);;
	}
	return 6;
}

int Yuv444_422order(char *src, char *dst, int order)
{
	int i, step = 3;
	int yuv[6];
	char *psrc = src;

	for(i = 0; i < 6; i += 3, psrc += step)
	{
		GetYuv444(psrc, yuv+i);
	}
	yuv[1] = (yuv[1] + yuv[4]) >> 1;
	yuv[2] = (yuv[2] + yuv[5]) >> 1;

	SetYuv422(dst, yuv, order);

	return 4;
}

int Yuv422_444order(char *src, char *dst, int order)
{
	int i, step = 3;
	char *pdst = dst;
	int yuv[6];

	GetYuvFrom422(src, yuv, order);

	for(i = 0; i < 6; i += 3, pdst += step)
	{
		SetYuv444(pdst, yuv+i);
	}
	return 6;
}

int Yuv422_Blkorder(char *src, char *dst, int order, int width, int height)
{
	int i, j, offset;
	int yuv[4];
	int pos, blkwidth;
	char *psrc = src;

	blkwidth = (((width + 15) >> 4) << 8);
	for(i = 0; i < height; i++)
	{
		offset = ((i >> 3) * blkwidth) + ((i % 8) << 3);
		for(j = 0; j < width; j += 2, psrc += 4)
		{
			GetYuv422(psrc, yuv, order);

			pos = (((j >> 4) << 8) + (((j % 16) >> 3) << 6) + (j % 8));
			pos += offset;
			BaseSetValueOne(dst+pos, yuv[0]);
			BaseSetValueOne(dst+pos+1, yuv[3]);

			pos = (((j >> 4) << 8) + 128 + ((j % 16) >> 1));
			pos += offset;

			BaseSetValueOne(dst+pos, yuv[1]);
			BaseSetValueOne(dst+pos+64, yuv[2]);
		}
	}
	return blkwidth * ((height + 7) >> 3);
}

int Blk2Yuv422order(char *src, char *dst, int order, int width, int height)
{
	int i, j, offset;
	int yuv[4];
	int pos, blkwidth;
	char *pdst = dst;

	blkwidth = (((width + 15) >> 4) << 8);
	for(i = 0; i < height; i++)
	{
		offset = ((i >> 3) * blkwidth) + ((i % 8) << 3);
		for(j = 0; j < width; j += 2, pdst += 4)
		{
			pos = (((j >> 4) << 8) + (((j % 16) >> 3) << 6) + (j % 8));
			pos += offset;
			yuv[0] = BaseGetValueOne(src+pos);
			yuv[3] = BaseGetValueOne(src+pos+1);
			pos = (((j >> 4) << 8) + 128 + ((j % 16) >> 1));
			pos += offset;
			yuv[1] = BaseGetValueOne(src+pos);
			yuv[2] = BaseGetValueOne(src+pos+64);

			SetYuv422(pdst, yuv, order);
		}
	}
	return blkwidth * ((height + 7) >> 3);
}

int Yuv422_Lbuforder(char *src, char *dst, int order, int width, int height)
{
	int i, j, k, ypos, upos, vpos;
	int yuv[4];
	int blkwidth;
	char *psrc = src;

	blkwidth = (((width + 15) >> 4) << 8);
	for(i = 0; i < height; i += 8)
	{
		ypos = (i >> 3) * blkwidth;
		upos = ypos + (blkwidth >> 1);
		vpos = upos + (blkwidth >> 2);
		k = ((height - i) < 8) ? (height - i) : 8;
		for(; k; k--)
		{
			for(j = 0; j < width; j += 2, psrc += 4, ypos += 2, upos++, vpos++)
			{
				GetYuv422(psrc, yuv, order);
				BaseSetValueOne(dst+ypos, yuv[0]);
				BaseSetValueOne(dst+ypos+1, yuv[3]);
				BaseSetValueOne(dst+upos, yuv[1]);
				BaseSetValueOne(dst+vpos, yuv[2]);
			}
		}
	}
	return blkwidth * ((height + 7) >> 3);
}

int Lbuf2Yuv422order(char *src, char *dst, int order, int width, int height)
{
	int i, j, k, ypos, upos, vpos;
	int yuv[4];
	int blkwidth;
	char *pdst = dst;

	blkwidth = (((width + 15) >> 4) << 8);
	for(i = 0; i < height; i += 8)
	{
		ypos = (i >> 3) * blkwidth;
		upos = ypos + (blkwidth >> 1);
		vpos = upos + (blkwidth >> 2);
		k = ((height - i) < 8) ? (height - i) : 8;
		for(; k; k--)
		{
			for(j = 0; j < width; j += 2, pdst += 4, ypos += 2, upos++, vpos++)
			{
				yuv[0] = BaseGetValueOne(src+ypos);
				yuv[3] = BaseGetValueOne(src+ypos+1);
				yuv[1] = BaseGetValueOne(src+upos);
				yuv[2] = BaseGetValueOne(src+vpos);
				SetYuv422(pdst, yuv, order);
			}
		}
	}
	return blkwidth * ((height + 7) >> 3);
}


/***************************************** yuv444 **********************************************/

int MYuv444Sizer(char *src, char *dest, int srcw, int dstw, int dsth, int px, int py)
{
	int i, size, size1, offset;
	char *psrc, *pdst;

	size = srcw * 3;
	size1 = dstw * 3;
	offset = px * 3;

	psrc = src + size * py;
	pdst = dest;
	for(i = 0; i < dsth; i++, psrc += size, pdst += size1)
		memcpy(pdst, psrc+offset, size1);

	return size1 * dsth;
}

int MRgb2Yuv444(char *src, char *dest, int width, int height)
{
	int i, size;

	size = 3;
	height *= width * 3;
	width = 0;
	for(i = 0; i < height; i += size)
		width += Rgb2Yuv444(src + i, dest + width);
	return width;
}

int MYuv444_Rgb(char *src, char *dest, int width, int height)
{
	int i, size;

	size = 3;
	height *= width * 3;
	width = 0;
	for(i = 0; i < height; i += size)
		width += Yuv444_Rgb(src + i, dest + width);
	return width;
}

int Rgb2Yuv444(char *src, char *dst)
{
	int yuv[3], bgr[3];

	GetBgr(src, bgr);

	Rgb2Yuv(bgr, yuv);

	SetYuv444(dst, yuv);

	return 3;
}

int Yuv444_Rgb(char *src, char *dst)
{
	int yuv[3], bgr[3];

	GetYuv444(src, yuv);

	Yuv2Rgb(yuv, bgr);

	SetBgr(dst, bgr);

	return 3;
}

int Yuv444_Blk(char *src, char *dst, int width, int height)
{
	int i, j, offset, step = 3;
	int yuv[3];
	int pos, blkwidth;
	char *psrc = src;

	blkwidth = (((width + 7) >> 3) * 192);
	for(i = 0; i < height; i++)
	{
		offset = ((i >> 3) * blkwidth) + ((i % 8) << 3);
		for(j = 0; j < width; j++, psrc += step)
		{
			GetYuv444(psrc, yuv);

			pos = (((j >> 3) * 192) + (j % 8));
			pos += offset;
			BaseSetValueOne(dst+pos, yuv[0]);
			BaseSetValueOne(dst+pos+64, yuv[1]);
			BaseSetValueOne(dst+pos+128, yuv[2]);
		}
	}
	return blkwidth * ((height + 7) >> 3);
}

int Blk2Yuv444(char *src, char *dst, int width, int height)
{
	int i, j, offset, step = 3;
	int yuv[3];
	int pos, blkwidth;
	char *pdst = dst;

	blkwidth = (((width + 7) >> 3) * 192);
	for(i = 0; i < height; i++)
	{
		offset = ((i >> 3) * blkwidth) + ((i % 8) << 3);
		for(j = 0; j < width; j++, pdst += step)
		{
			pos = (((j >> 3) * 192) + (j % 8));
			pos += offset;
			yuv[0] = BaseGetValueOne(src+pos);
			yuv[1] = BaseGetValueOne(src+pos+64);
			yuv[2] = BaseGetValueOne(src+pos+128);
			SetYuv444(pdst, yuv);
		}
	}
	return blkwidth * ((height + 7) >> 3);
}

int Yuv444_Lbuf(char *src, char *dst, int width, int height)
{
	int i, j, k, ypos, upos, vpos, step = 3;
	int yuv[3];
	int blkwidth;
	char *psrc = src;

	blkwidth = (((width + 7) >> 3) * 192);
	for(i = 0; i < height; i += 8)
	{
		ypos = (i >> 3) * blkwidth;
		upos = ypos + (blkwidth / 3);
		vpos = upos + (blkwidth / 3);
		k = ((height - i) < 8) ? (height - i) : 8;
		for(; k; k--)
		{
			for(j = 0; j < width; j++, psrc += step, ypos++, upos++, vpos++)
			{
				GetYuv444(psrc, yuv);
				BaseSetValueOne(dst+ypos, yuv[0]);
				BaseSetValueOne(dst+upos, yuv[1]);
				BaseSetValueOne(dst+vpos, yuv[2]);
			}
		}
	}
	return blkwidth * ((height + 7) >> 3);
}

int Lbuf2Yuv444(char *src, char *dst, int width, int height)
{
	int i, j, k, ypos, upos, vpos, step = 3;
	int yuv[3];
	int blkwidth;
	char *pdst = dst;

	blkwidth = (((width + 7) >> 3) * 192);
	for(i = 0; i < height; i += 8)
	{
		ypos = (i >> 3) * blkwidth;
		upos = ypos + (blkwidth / 3);
		vpos = upos + (blkwidth / 3);
		k = ((height - i) < 8) ? (height - i) : 8;
		for(; k; k--)
		{
			for(j = 0; j < width; j++, pdst += step, ypos++, upos++, vpos++)
			{
				yuv[0] = BaseGetValueOne(src+ypos);
				yuv[1] = BaseGetValueOne(src+upos);
				yuv[2] = BaseGetValueOne(src+vpos);
				SetYuv444(pdst, yuv);
			}
		}
	}
	return blkwidth * ((height + 7) >> 3);
}

void Yuv444ZoutHorResize(char *src, char *dst, int srcw, int srch, int dstw)
{
	int i, j, up, down, k, step = 3;
	int val0, val1, yuv;
	char *psrc = src;
	char *pdst = dst;

	if(srcw == dstw)
	{
		if(dst != src)
			memcpy(dst, src, srcw * srch * 3);
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
			for(k = 0; k < step; k++, pdst++)
			{
				if(up == 0)
				{
					yuv = BaseGetValueOne(psrc + k);
					BaseSetValueOne(pdst, yuv);
				}
				else
				{
					val0 = BaseGetValueOne(psrc + k - step);
					val1 = BaseGetValueOne(psrc + k);
					yuv = (val0 * down + val1 * up) / (dstw - 1);
					BaseSetValueOne(pdst, yuv);
				}
			}
		}
		//memcpy(pdst, psrc, step);
		//psrc += step;
		//pdst += step;
	}
}

void Yuv444ZinHorResize(char *src, char *dst, int srcw, int srch, int dstw)
{
	int i, j, up, down, k, gap, step = 3, offset = 0;
	int val0, val1, yuv;
	char *psrc = src;
	char *pdst = dst;

	if(srcw == dstw)
	{
		if(dst != src)
			memcpy(dst, src, srcw * srch * 3);
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
			for(k = 0; k < step; k++, pdst++)
			{
				if(up == 0)
				{
					yuv = BaseGetValueOne(psrc + offset + k);
					BaseSetValueOne(pdst, yuv);
				}
				else
				{
					val0 = BaseGetValueOne(psrc + offset + k);
					val1 = BaseGetValueOne(psrc + offset + k + step);
					yuv = (val0 * down + val1 * up) / (dstw + 1);
					BaseSetValueOne(pdst, yuv);
				}
			}
		}
	}
}

void Yuv444ZoutVerResize(char *src, char *dst, int srcw, int srch, int dsth)
{
	int i, j, up, down, k, gap, step = 3;
	int yuv, val0, val1;
	char *psrc = src;
	char *pdst = dst;

	if(srch == dsth)
	{
		if(dst != src)
			memcpy(dst, src, srcw * srch * 3);
		return;
	}
	gap = srcw * 3;
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
			for(k = 0; k < step; k++, psrc++, pdst++)
			{
				if(up == 0)
				{
					yuv = BaseGetValueOne(psrc);
					BaseSetValueOne(pdst, yuv);
				}
				else
				{
					val0 = BaseGetValueOne(psrc - gap);
					val1 = BaseGetValueOne(psrc);
					yuv = (val0 * down + val1 * up) / (dsth - 1);
					BaseSetValueOne(pdst, yuv);
				}
			}
		}
	}
	memcpy(pdst, psrc, gap);
}

void Yuv444ZinVerResize(char *src, char *dst, int srcw, int srch, int dsth)
{
	int i, j, up, down, k, gap, step = 3;
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
			for(k = 0; k < step; k++, pdst++)
			{
				if(up == 0)
				{
					yuv = BaseGetValueOne(psrc + k + i);
					BaseSetValueOne(pdst, yuv);
				}
				else
				{
					val0 = BaseGetValueOne(psrc + k + i);
					val1 = BaseGetValueOne(psrc + gap + i + k);
					yuv = (val0 * down + val1 * up) / (dsth + 1);
					BaseSetValueOne(pdst, yuv);
				}
			}
		}
	}
}

int Yuv444Resize(char *src, char *dst, int srcw, int srch, int dstw, int dsth)
{
	int len = dstw * dsth * 3;

	if(dsth <= srch)
	{
		Yuv444ZoutVerResize(src, src, srcw, srch, dsth);
		if(dstw <= srcw)
			Yuv444ZoutHorResize(src, dst, srcw, dsth, dstw);
		else
			Yuv444ZinHorResize(src, dst, srcw, dsth, dstw);
	}
	else
	{
		Yuv444ZinVerResize(src, dst, srcw, srch, dsth);
		if(dstw <= srcw)
			Yuv444ZoutHorResize(dst, src, srcw, dsth, dstw);
		else
			Yuv444ZinHorResize(dst, src, srcw, dsth, dstw);
		memcpy(dst, src, len);
	}

	return len;
}


//=============	yuv444 prefilter ==============================
int MYuv444Prefilter(char *src, char *dest, int width, int height, 
					int count, int *filter)
{
	return Prefilter(src, dest, width, height, 3, count, filter);
}


