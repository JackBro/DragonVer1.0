#ifndef _YUV411_H_
#define _YUV411_H_


#ifndef TOOLCONVERT_DLL
	#define TOOLCONVERT_DLL  __declspec(dllimport)
#endif

#include "yuv444.h"

class TOOLCONVERT_DLL CYuv411 : public CYuv444
{
public:
	int Rgb2Yyyyuv(char *src, char *dst, int cap = 256, int unit = 1);
	int Rgb2Yyyyvu(char *src, char *dst, int cap = 256, int unit = 1);
	int Rgb2Yuyvyy(char *src, char *dst, int cap = 256, int unit = 1);
	int Rgb2Yvyuyy(char *src, char *dst, int cap = 256, int unit = 1);
	int Rgb2Uvyyyy(char *src, char *dst, int cap = 256, int unit = 1);
	int Rgb2Vuyyyy(char *src, char *dst, int cap = 256, int unit = 1);

public:
	int Yyyyuv2Rgb(char *src, char *dst, int cap = 256, int unit = 1);
	int Yyyyvu2Rgb(char *src, char *dst, int cap = 256, int unit = 1);
	int Yuyvyy2Rgb(char *src, char *dst, int cap = 256, int unit = 1);
	int Yvyuyy2Rgb(char *src, char *dst, int cap = 256, int unit = 1);
	int Uvyyyy2Rgb(char *src, char *dst, int cap = 256, int unit = 1);
	int Vuyyyy2Rgb(char *src, char *dst, int cap = 256, int unit = 1);

public:
	int Yuv444_Yyyyuv(char *src, char *dst, int unit = 1);
	int Yuv444_Yyyyvu(char *src, char *dst, int unit = 1);
	int Yuv444_Yuyvyy(char *src, char *dst, int unit = 1);
	int Yuv444_Yvyuyy(char *src, char *dst, int unit = 1);
	int Yuv444_Uvyyyy(char *src, char *dst, int unit = 1);
	int Yuv444_Vuyyyy(char *src, char *dst, int unit = 1);

public:
	int Yyyyuv_444(char *src, char *dst, int unit = 1);
	int Yyyyvu_444(char *src, char *dst, int unit = 1);
	int Yuyvyy_444(char *src, char *dst, int unit = 1);
	int Yvyuyy_444(char *src, char *dst, int unit = 1);
	int Uvyyyy_444(char *src, char *dst, int unit = 1);
	int Vuyyyy_444(char *src, char *dst, int unit = 1);

public:
	int Blk2Yyyyuv(char *src, char *dst, int width, int height, int unit = 1);
	int Blk2Yyyyvu(char *src, char *dst, int width, int height, int unit = 1);
	int Blk2Yuyvyy(char *src, char *dst, int width, int height, int unit = 1);
	int Blk2Yvyuyy(char *src, char *dst, int width, int height, int unit = 1);
	int Blk2Uvyyyy(char *src, char *dst, int width, int height, int unit = 1);
	int Blk2Vuyyyy(char *src, char *dst, int width, int height, int unit = 1);

	int Blk2Rgb24(char * src, char * dest, int width, int height);

public:
	int Yyyyuv2Blk(char *src, char *dst, int width, int height, int unit = 1);
	int Yyyyvu2Blk(char *src, char *dst, int width, int height, int unit = 1);
	int Yuyvyy2Blk(char *src, char *dst, int width, int height, int unit = 1);
	int Yvyuyy2Blk(char *src, char *dst, int width, int height, int unit = 1);
	int Uvyyyy2Blk(char *src, char *dst, int width, int height, int unit = 1);
	int Vuyyyy2Blk(char *src, char *dst, int width, int height, int unit = 1);

	int Rgb2Yuv411Blk(char *src, char *dst, int width, int height, int unit = 1);
	int Rgb2Jpeg411(char *src, char *dst, int width, int height, int unit = 1, int qf = 40);

public:
	int VuyyyyResize(char *src, char *dest, int srcw, int srch, int dstw, int dsth, int unit = 1);
	int UvyyyyResize(char *src, char *dest, int srcw, int srch, int dstw, int dsth, int unit = 1);
	int YuyvyyResize(char *src, char *dest, int srcw, int srch, int dstw, int dsth, int unit = 1);
	int YvyuyyResize(char *src, char *dest, int srcw, int srch, int dstw, int dsth, int unit = 1);
	int YyyyuvResize(char *src, char *dest, int srcw, int srch, int dstw, int dsth, int unit = 1);
	int YyyyvuResize(char *src, char *dest, int srcw, int srch, int dstw, int dsth, int unit = 1);

public:
	int YyyyuvSizer(char *src, char *dest, int srcw, int dstw, int dsth, int px, int py, int unit = 1);
	int YyyyvuSizer(char *src, char *dest, int srcw, int dstw, int dsth, int px, int py, int unit = 1);
	int YuyvyySizer(char *src, char *dest, int srcw, int dstw, int dsth, int px, int py, int unit = 1);
	int YvyuyySizer(char *src, char *dest, int srcw, int dstw, int dsth, int px, int py, int unit = 1);
	int UvyyyySizer(char *src, char *dest, int srcw, int dstw, int dsth, int px, int py, int unit = 1);
	int VuyyyySizer(char *src, char *dest, int srcw, int dstw, int dsth, int px, int py, int unit = 1);

public:
	int Blk2Jpeg(char *src, char *dest, int width, int height, int qf = 40);

public:
	int getVwc(int width, int height);

protected:
	int rgb2yuv411order(char *src, char *dst, int order, int cap = 256, int unit = 1);
	int yuv411_rgborder(char *src, char *dst, int order, int cap = 256, int unit = 1);
	int yuv444_411order(char *src, char *dst, int order, int unit = 1);
	int yuv411_444order(char *src, char *dst, int order, int unit = 1);
	int yuv411_blkorder(char *src, char *dst, int order, int width, int height, int unit = 1);
	int blk2yuv411order(char *src, char *dst, int order, int width, int height, int unit = 1);
	int yuv411_lbuforder(char *src, char *dst, int order, int width, int height, int unit = 1);
	int lbuf2yuv411order(char *src, char *dst, int order, int width, int height, int unit = 1);
	int yuv411_resizeorder(char *src, char *dest, int srcw, int srch, int dstw, int dsth, int order, int unit = 1);

	int MRgb2Yuv411order(char *src, char *dst, int width, int height, 
								int order, int cap = 256, int unit = 1);
	int MYuv411_Rgborder(char *src, char *dst, int width, int height, 
								int order, int cap = 256, int unit = 1);
	int MYuv444_411order(char *src, char *dst, int width, int height, int order, int unit = 1);
	int MYuv411_444order(char *src, char *dst, int width, int height, int order, int unit = 1);
	int MYuv411Sizer(char *src, char *dest, int srcw, int dstw, int dsth, 
								 int px, int py, int order, int unit = 1);
};


#endif


