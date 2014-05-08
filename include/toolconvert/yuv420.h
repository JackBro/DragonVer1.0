#ifndef _YUV420_H_
#define _YUV420_H_


#ifndef TOOLCONVERT_DLL
	#define TOOLCONVERT_DLL  __declspec(dllimport)
#endif

#include "yuv444.h"

class TOOLCONVERT_DLL CYuv420 : public CYuv444
{
public:
	int Rgb2Yyuv(char *src, char *dst, int width, int cap = 256, int unit = 1);
	int Rgb2Yuyv(char *src, char *dst, int width, int cap = 256, int unit = 1);
	int Rgb2Uyvy(char *src, char *dst, int width, int cap = 256, int unit = 1);
	int Rgb2Vyuy(char *src, char *dst, int width, int cap = 256, int unit = 1);
	int Rgb2Yvyu(char *src, char *dst, int width, int cap = 256, int unit = 1);

	int Rgb2Yuvy(char *src, char *dst, int width, int cap = 256, int unit = 1);
	int Rgb2Yvuy(char *src, char *dst, int width, int cap = 256, int unit = 1);
	int Rgb2Uvyy(char *src, char *dst, int width, int cap = 256, int unit = 1);
	int Rgb2Vuyy(char *src, char *dst, int width, int cap = 256, int unit = 1);
	int Rgb2Yyvu(char *src, char *dst, int width, int cap = 256, int unit = 1);

public:
	int Yyuv2Rgb(char *src, char *dst, int width, int cap = 256, int unit = 1);
	int Yuyv2Rgb(char *src, char *dst, int width, int cap = 256, int unit = 1);
	int Uyvy2Rgb(char *src, char *dst, int width, int cap = 256, int unit = 1);
	int Vyuy2Rgb(char *src, char *dst, int width, int cap = 256, int unit = 1);
	int Yvyu2Rgb(char *src, char *dst, int width, int cap = 256, int unit = 1);

	int Yuvy2Rgb(char *src, char *dst, int width, int cap = 256, int unit = 1);
	int Yvuy2Rgb(char *src, char *dst, int width, int cap = 256, int unit = 1);
	int Uvyy2Rgb(char *src, char *dst, int width, int cap = 256, int unit = 1);
	int Vuyy2Rgb(char *src, char *dst, int width, int cap = 256, int unit = 1);
	int Yyvu2Rgb(char *src, char *dst, int width, int cap = 256, int unit = 1);

public:
	int Yyuv2Yuv444(char *src, char *dst, int width, int unit = 1);
	int Yuyv2Yuv444(char *src, char *dst, int width, int unit = 1);
	int Uyvy2Yuv444(char *src, char *dst, int width, int unit = 1);
	int Vyuy2Yuv444(char *src, char *dst, int width, int unit = 1);
	int Yvyu2Yuv444(char *src, char *dst, int width, int unit = 1);

	int Yuvy2Yuv444(char *src, char *dst, int width, int unit = 1);
	int Yvuy2Yuv444(char *src, char *dst, int width, int unit = 1);
	int Uvyy2Yuv444(char *src, char *dst, int width, int unit = 1);
	int Vuyy2Yuv444(char *src, char *dst, int width, int unit = 1);
	int Yyvu2Yuv444(char *src, char *dst, int width, int unit = 1);

public:
	int Yuv444_Yyuv(char *src, char *dst, int width, int unit = 1);
	int Yuv444_Yuyv(char *src, char *dst, int width, int unit = 1);
	int Yuv444_Uyvy(char *src, char *dst, int width, int unit = 1);
	int Yuv444_Vyuy(char *src, char *dst, int width, int unit = 1);
	int Yuv444_Yvyu(char *src, char *dst, int width, int unit = 1);

	int Yuv444_Yuvy(char *src, char *dst, int width, int unit = 1);
	int Yuv444_Yvuy(char *src, char *dst, int width, int unit = 1);
	int Yuv444_Uvyy(char *src, char *dst, int width, int unit = 1);
	int Yuv444_Vuyy(char *src, char *dst, int width, int unit = 1);
	int Yuv444_Yyvu(char *src, char *dst, int width, int unit = 1);

public:
	int Yyuv2Blk(char *src, char *dst, int width, int height, int unit = 1);
	int Yuyv2Blk(char *src, char *dst, int width, int height, int unit = 1);
	int Uyvy2Blk(char *src, char *dst, int width, int height, int unit = 1);
	int Vyuy2Blk(char *src, char *dst, int width, int height, int unit = 1);
	int Yvyu2Blk(char *src, char *dst, int width, int height, int unit = 1);

	int Yuvy2Blk(char *src, char *dst, int width, int height, int unit = 1);
	int Yvuy2Blk(char *src, char *dst, int width, int height, int unit = 1);
	int Uvyy2Blk(char *src, char *dst, int width, int height, int unit = 1);
	int Vuyy2Blk(char *src, char *dst, int width, int height, int unit = 1);
	int Yyvu2Blk(char *src, char *dst, int width, int height, int unit = 1);
	int Rgb2Yuv420Blk(char *src, char *dst, int width, int height, int unit = 1);

public:
	int Blk2Yyuv(char *src, char *dst, int width, int height, int unit = 1);
	int Blk2Yuyv(char *src, char *dst, int width, int height, int unit = 1);
	int Blk2Uyvy(char *src, char *dst, int width, int height, int unit = 1);
	int Blk2Vyuy(char *src, char *dst, int width, int height, int unit = 1);
	int Blk2Yvyu(char *src, char *dst, int width, int height, int unit = 1);

	int Blk2Yuvy(char *src, char *dst, int width, int height, int unit = 1);
	int Blk2Yvuy(char *src, char *dst, int width, int height, int unit = 1);
	int Blk2Uvyy(char *src, char *dst, int width, int height, int unit = 1);
	int Blk2Vuyy(char *src, char *dst, int width, int height, int unit = 1);
	int Blk2Yyvu(char *src, char *dst, int width, int height, int unit = 1);

public:
	int Blk2Rgb24(char *src, char *dest, int width, int height);
	int Blk2Jpeg(char *src, char *dest, int width, int height, int qf = 40);
	int Rgb2Jpeg420(char *src, char *dest, int width, int height, int unit = 1, int qf = 40);
	int Blk2Mpeg2I(char *src, char *dest, int width, int height);

public:
	int getVwc(int width, int height);

//protected:
public:
	int rgb2yuv420order(char *src, char *dst, int width, int order, int cap = 256, int unit = 1);
	int yuv420_rgborder(char *src, char *dst, int width, int order, int cap = 256, int unit = 1);
	int yuv444_420order(char *src, char *dst, int width, int order, int unit = 1);
	int yuv420_444order(char *src, char *dst, int width, int order, int unit = 1);
	int yuv420_blkorder(char *src, char *dst, int order, int width, int height, int unit = 1);
	int blk2yuv420order(char *src, char *dst, int order, int width, int height, int unit = 1);
	int yuv420_lbuforder(char *src, char *dst, int order, int width, int height, int unit = 1);
	int lbuf2yuv420order(char *src, char *dst, int order, int width, int height, int unit = 1);
	int yuv420_resizeorder(char *src, char *dest, int srcw, int srch, int dstw, int dsth, int order, int unit = 1);
	
	int MRgb2Yuv420order(char *src, char *dst, int width, int height, int order, int cap = 256, int unit = 1);
	int MYuv420_Rgborder(char *src, char *dst, int width, int height, int order, int cap = 256, int unit = 1);
	int MYuv444_420order(char *src, char *dst, int width, int height, int order, int unit = 1);
	int MYuv420_444order(char *src, char *dst, int width, int height, int order, int unit = 1);
	//int MYuv420Sizer(char *src, char *dest, int srcw, int dstw, int dsth, 
	//							 int px, int py, int order, int unit = 1);
	int MYuv420_FrmOrder(char *src, char *dst, int width, int height, int order, int unit = 1);
	int MFrm2Yuv420Order(char *src, char *dst, int width, int height, int order, int unit = 1);

};

//typedef int (CYuv420::*Rgb2YuvCall)(char *src, char *dst, int width, int cap, int unit);

#endif


