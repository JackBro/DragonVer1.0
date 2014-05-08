#include "yuv2rgb.h"
#include <string.h>

const int gYuv420_Order[] =	{ 0, 3, 1, 2 };	

int yuv420_444(char *src, char *dst, int width)
{
	int i, j, offset, gap1, gap2, k;
	int yuv[12];
	char *pdst = dst;

	gap1 = width << 1;
	gap2 = width * 3;
	for(i = 0, offset = 0; i < width; i += 2, offset += 4, pdst += 6)
	{
		for(k = 0; k < 4; k++)
			yuv[gYuv420_Order[k]] = ((int)src[offset+k]) & 0xff ;
		yuv[4] = yuv[1];
		yuv[5] = yuv[2];
		yuv[6] = src[gap1 + (offset >> 1)];
		yuv[9] = src[gap1 + (offset >> 1) + 1];
		yuv[7] = yuv[1];
		yuv[10] = yuv[1];
		yuv[8] = yuv[2];
		yuv[11] = yuv[2];

		for(j = 0; j < 6; j++)
		{
			pdst[j] = (char)yuv[j];
			pdst[j+gap2] = (char)yuv[j+6];
		}
	}
	return width * 6;
}

/**
Description:
	convert YUV420 to YUV444
Parameters:
	src:	souce buffer, contain whold image
	dst:	output buffer, will store output image
	width:
	height:	image dimension
Notes:
	system must allocate enough buffer
Returns:
	return output buffer image size
*/
int Yuv420T444(char *src, char *dst, int width, int height)
{
	int size, i, len;
	char *psrc = src;
	char *pdst = dst;

	size = width * 3;
	for(i = 0; i < height; i+=2)
	{
		len = yuv420_444(psrc, pdst, width);
		psrc += size;
		pdst += len;
	}
	return width * height * 3;
}

int yuv444_420(char *src, char *dst, int width)
{
	int i, j, k;
	int yuv[12];
	int gap1, gap2, offset;
	char *psrc = src;

	gap1 = width * 3;
	gap2 = width << 1;
	for(i = 0, offset = 0; i < width; i += 2, offset += 4, psrc += 6)
	{
		for(j = 0; j < 6; j++)
		{
			yuv[j] = (int)psrc[j] & 0xff;
			yuv[j+6] = (int)psrc[gap1+j] & 0xff;
		}

		yuv[1] += yuv[4] + yuv[7] + yuv[10];
		yuv[2] += yuv[5] + yuv[8] + yuv[11];
		yuv[1] >>= 2;
		yuv[2] >>= 2;

		for(k = 0; k < 4; k++)
			dst[offset+k] = (char)yuv[gYuv420_Order[k]];

		dst[gap2 + (offset >> 1)] = (char)yuv[6];
		dst[gap2 + (offset >> 1) + 1] = (char)yuv[9];
	}
	return width * 3;
}

/**
Description:
	convert YUV444 to YUV420
Parameters:
	src:	souce buffer, contain whold image
	dst:	output buffer, will store output image
	width:
	height:	image dimension
Notes:
	system must allocate enough buffer
Returns:
	return output buffer image size
*/
int Yuv444T420(char *src, char *dst, int width, int height)
{
	int size, i, len;
	char *psrc = src;
	char *pdst = dst;

	size = width * 6;
	for(i = 0; i < height; i+=2)
	{
		len = yuv444_420(psrc, pdst, width);
		psrc += size;
		pdst += len;
	}
	return (width * height * 3) >> 1;
}

void Yuv444HorSizer(char *src, char *dst, int srcw, int srch, int dstw)
{
	int i, j, up, left, right, k;
	int temp1[3], temp2[3], yuv[3];
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
		up = 0;
		for(i = 0; i < srcw; i++, psrc += 3)
		{
			up += dstw;
			if(up < srcw)
				continue;
			right = up - srcw;
			left = srcw - right;
			for(k = 0; k < 3; k++)
			{
				if(right == 0)
				{
					pdst[k] = psrc[k];
				}
				else
				{
					temp1[k] = (int)psrc[k] & 0xff;
					temp2[k] = (int)psrc[k+3] & 0xff;
					yuv[k] = (temp1[k] * left + temp2[k] * right) / srcw;
					pdst[k] = (char)yuv[k];
				}
			}
			pdst += 3;
			up -= srcw;
		}
	}
}

void Yuv444VerSizer(char *src, char *dst, int srcw, int srch, int dsth)
{
	int i, j, up, top, btm, k, offset, gap;
	int temp1[3], temp2[3], yuv[3];
	char *psrc = src;
	char *pdst = dst;

	if(srch == dsth)
	{
		if(dst != src)
			memcpy(dst, src, srcw * srch * 3);
		return;
	}
	up = 0;
	gap = srcw * 3;
	for(j = 0; j < srch; j++, psrc += gap)
	{
		up += dsth;
		if(up < srch)
			continue;
		btm = up - srch;
		top = srch - btm;
		for(i = 0, offset = 0; i < srcw; i++, pdst += 3)
		{
			for(k = 0; k < 3; k++, offset ++)
			{
				if(btm == 0)
				{
					pdst[k] = psrc[offset];
				}
				else
				{
					temp1[k] = (int)psrc[offset] & 0xff;
					temp2[k] = (int)psrc[gap + offset] & 0xff;
					yuv[k] = (temp1[k] * top + temp2[k] * btm) / srch;
					pdst[k] = (char)yuv[k];
				}
			}
		}
		up -= srch;
	}
}

/**
Description:
	shrink YUV444 image
Parameters:
	src:	souce buffer, contain whold image
	dst:	output buffer, will store output image
	srcw:	srouce image width
	srch:	srouce image height
	dstw:	destination image width
	dsth:	destination image height
Notes:
	system must allocate enough buffer
Returns:
	return output buffer image size
*/
int Yuv444_Sizer(char *src, char *dst, int srcw, int srch, int dstw, int dsth)
{
	if( (dstw > srcw) || (dsth > srch) )
		return 0;
	Yuv444VerSizer(src, src, srcw, srch, dsth);
	Yuv444HorSizer(src, dst, srcw, dsth, dstw);
	return dstw * dsth * 3;
}

/**
Description:
	shrink YUV420 image
Parameters:
	src:	souce buffer, contain whold image
	dst:	output buffer, will store output image
	srcw:	srouce image width
	srch:	srouce image height
	dstw:	destination image width
	dsth:	destination image height
Notes:
	system must allocate enough buffer for YUV444 source and destination image.
Returns:
	return output buffer image size
*/
int Yuv420_Sizer(char *src, char *dst, int srcw, int srch, int dstw, int dsth)
{
	if( (dstw > srcw) || (dsth > srch) )
		return 0;
	Yuv420T444(src, dst, srcw, srch);
	Yuv444_Sizer(dst, src, srcw, srch, dstw, dsth);
	Yuv444T420(src, dst, dstw, dsth);
	return (dstw * dsth * 3) >> 1;
}
