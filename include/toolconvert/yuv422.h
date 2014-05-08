#ifndef _YUV422_H_
#define _YUV422_H_


#ifndef TOOLCONVERT_DLL
	#define TOOLCONVERT_DLL  __declspec(dllimport)
#endif

#include "yuv444.h"
#include "tooljpg/jpegencbase.h"

class TOOLCONVERT_DLL CYuv422 : public CYuv444
{
public:
	int Rgb2Yyuv(char *src, char *dst, int cap = 256, int unit = 1);
	int Rgb2Yuyv(char *src, char *dst, int cap = 256, int unit = 1);
	int Rgb2Uyvy(char *src, char *dst, int cap = 256, int unit = 1);
	int Rgb2Vyuy(char *src, char *dst, int cap = 256, int unit = 1);
	int Rgb2Yvyu(char *src, char *dst, int cap = 256, int unit = 1);

	int Rgb2Yuvy(char *src, char *dst, int cap = 256, int unit = 1);
	int Rgb2Yvuy(char *src, char *dst, int cap = 256, int unit = 1);
	int Rgb2Uvyy(char *src, char *dst, int cap = 256, int unit = 1);
	int Rgb2Vuyy(char *src, char *dst, int cap = 256, int unit = 1);
	int Rgb2Yyvu(char *src, char *dst, int cap = 256, int unit = 1);
	
public:
	int Yyuv2Rgb(char *src, char *dst, int cap = 256, int unit = 1);
	int Yuyv2Rgb(char *src, char *dst, int cap = 256, int unit = 1);
	int Uyvy2Rgb(char *src, char *dst, int cap = 256, int unit = 1);
	int Vyuy2Rgb(char *src, char *dst, int cap = 256, int unit = 1);
	int Yvyu2Rgb(char *src, char *dst, int cap = 256, int unit = 1);

	int Yuvy2Rgb(char *src, char *dst, int cap = 256, int unit = 1);
	int Yvuy2Rgb(char *src, char *dst, int cap = 256, int unit = 1);
	int Uvyy2Rgb(char *src, char *dst, int cap = 256, int unit = 1);
	int Vuyy2Rgb(char *src, char *dst, int cap = 256, int unit = 1);
	int Yyvu2Rgb(char *src, char *dst, int cap = 256, int unit = 1);

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

public:
//	int Yyuv2Lbuf(char *src, char *dst, int width, int height, int unit = 1);

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

	int Blk2Rgb24(char *src, char * dst, int width, int height);

public:
	int Yuv444_Yyuv(char *src, char *dst, int unit = 1);
	int Yuv444_Yuyv(char *src, char *dst, int unit = 1);
	int Yuv444_Uyvy(char *src, char *dst, int unit = 1);
	int Yuv444_Vyuy(char *src, char *dst, int unit = 1);
	int Yuv444_Yvyu(char *src, char *dst, int unit = 1);

	int Yuv444_Yuvy(char *src, char *dst, int unit = 1);
	int Yuv444_Yvuy(char *src, char *dst, int unit = 1);
	int Yuv444_Uvyy(char *src, char *dst, int unit = 1);
	int Yuv444_Vuyy(char *src, char *dst, int unit = 1);
	int Yuv444_Yyvu(char *src, char *dst, int unit = 1);

public:
	int Yyuv2Yuv444(char *src, char *dst, int unit = 1);
	int Yuyv2Yuv444(char *src, char *dst, int unit = 1);
	int Uyvy2Yuv444(char *src, char *dst, int unit = 1);
	int Vyuy2Yuv444(char *src, char *dst, int unit = 1);
	int Yvyu2Yuv444(char *src, char *dst, int unit = 1);

	int Yuvy2Yuv444(char *src, char *dst, int unit = 1);
	int Yvuy2Yuv444(char *src, char *dst, int unit = 1);
	int Uvyy2Yuv444(char *src, char *dst, int unit = 1);
	int Vuyy2Yuv444(char *src, char *dst, int unit = 1);
	int Yyvu2Yuv444(char *src, char *dst, int unit = 1);

public:
	int YyuvResize(char *src, char *dest, int srcw, int srch, int dstw, int dsth, int unit = 1);
	int YuyvResize(char *src, char *dest, int srcw, int srch, int dstw, int dsth, int unit = 1);
	int UyvyResize(char *src, char *dest, int srcw, int srch, int dstw, int dsth, int unit = 1);
	int VyuyResize(char *src, char *dest, int srcw, int srch, int dstw, int dsth, int unit = 1);
	int YvyuResize(char *src, char *dest, int srcw, int srch, int dstw, int dsth, int unit = 1);
		
	int YuvyResize(char *src, char *dest, int srcw, int srch, int dstw, int dsth, int unit = 1);
	int YvuyResize(char *src, char *dest, int srcw, int srch, int dstw, int dsth, int unit = 1);
	int UvyyResize(char *src, char *dest, int srcw, int srch, int dstw, int dsth, int unit = 1);
	int VuyyResize(char *src, char *dest, int srcw, int srch, int dstw, int dsth, int unit = 1);
	int YyvuResize(char *src, char *dest, int srcw, int srch, int dstw, int dsth, int unit = 1);

public:
	int YyuvSizer(char *src, char *dest, int srcw, int dstw, int dsth, int px, int py, int unit = 1);
	int YuyvSizer(char *src, char *dest, int srcw, int dstw, int dsth, int px, int py, int unit = 1);
	int UyvySizer(char *src, char *dest, int srcw, int dstw, int dsth, int px, int py, int unit = 1);
	int VyuySizer(char *src, char *dest, int srcw, int dstw, int dsth, int px, int py, int unit = 1);
	int YvyuSizer(char *src, char *dest, int srcw, int dstw, int dsth, int px, int py, int unit = 1);
				
	int YuvySizer(char *src, char *dest, int srcw, int dstw, int dsth, int px, int py, int unit = 1);
	int YvuySizer(char *src, char *dest, int srcw, int dstw, int dsth, int px, int py, int unit = 1);
	int UvyySizer(char *src, char *dest, int srcw, int dstw, int dsth, int px, int py, int unit = 1);
	int VuyySizer(char *src, char *dest, int srcw, int dstw, int dsth, int px, int py, int unit = 1);
	int YyvuSizer(char *src, char *dest, int srcw, int dstw, int dsth, int px, int py, int unit = 1);

public:
	int Blk2Jpeg(char *src, char *dest, int width, int height, int *nextq, int qf = 40);
	int Blk2Jpeg1(char *src, char *dest, int width, int height, int *nextq, int qf = 40, int tcr = 41, PTRudebrcOption pinfo = NULL);

public:
	int getVwc(int width, int height);

public:
	int rgb2yuv422order(char *src, char *dst, int order, int cap = 256, int unit = 1);
	int Rgb2Yuv422Blk(char * src, char *dst, int width, int blkHeight, int unit = 1);
	int Rgb2Jpeg422(char *src, char *dst, int width, int height, int qfact = 40, int unit = 1);
	int yuv422_rgborder(char *src, char *dst, int order, int cap = 256, int unit = 1);
	int yuv422_blkorder(char *src, char *dst, int order, int width, int height, int unit = 1);
	int blk2yuv422order(char *src, char *dst, int order, int width, int height, int unit = 1);
	int yuv422_lbuforder(char *src, char *dst, int order, int width, int height, int unit = 1);
	int lbuf2yuv422order(char *src, char *dst, int order, int width, int height, int unit = 1);
	int lbuf4442yuv422order(char *src, char *dst, int order, int width, int height, int unit = 1);
	int yuv422_444order(char *src, char *dst, int order, int unit = 1);
	int yuv444_422order(char *src, char *dst, int order, int unit = 1);
	int yuv422_resizeorder(char *src, char *dest, int srcw, int srch, int dstw, int dsth, int order, int unit = 1);

	int MRgb2Yuv422order(char *src, char *dst, int width, int height, int order, int cap = 256, int unit = 1);
	int MYuv422_Rgborder(char *src, char *dst, int width, int height, int order, int cap = 256, int unit = 1);
	int MYuv444_422order(char *src, char *dst, int width, int height, int order, int unit = 1);
	int MYuv422_444order(char *src, char *dst, int width, int height, int order, int unit = 1);
	int MYuv422Sizer(char *src, char *dest, int srcw, int dstw, int dsth, 
								 int px, int py, int order, int unit = 1);
	int MYuv422_FrmOrder(char *src, char *dst, int width, int height, int order, int unit = 1);
	int MFrm2Yuv422Order(char *src, char *dst, int width, int height, int order, int unit = 1);
};


#endif


