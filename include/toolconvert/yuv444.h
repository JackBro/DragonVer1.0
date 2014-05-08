#ifndef _YUV444_H_
#define _YUV444_H_


#ifndef TOOLCONVERT_DLL
	#define TOOLCONVERT_DLL  __declspec(dllimport)
#endif


#include "toolmath/yuv2rgb.h"

class TOOLCONVERT_DLL CYuv444 : public CYuv2Rgb
{
public:
	int Rgb2Yuv444(char *src, char *dst, int cap = 256, int unit = 1);
	int Yuv444_Rgb(char *src, char *dst, int cap = 256, int unit = 1);

public:
	int Yuv444_Blk(char *src, char *dst, int width, int height, int unit = 1);
	int Blk2Yuv444(char *src, char *dst, int width, int height, int unit = 1);
	int Yuv444_Lbuf(char *src, char *dst, int width, int height, int unit = 1);
	int Lbuf2Yuv444(char *src, char *dst, int width, int height, int unit = 1);

public:
	int Yuv444Resize(char *src, char *dst, int srcw, int srch, int dstw, int dsth, int unit = 1);

public:
	int Blk2Rgb24(char *src, char *dest, int width, int height);
	int Rgb2Yuv444Blk(char *src, char *dest, int width, int height, int cap = 256, int unit = 1);
	int Blk2Jpeg(char *src, char *dest, int width, int height, int qf = 40);
	int Rgb2Jpeg444(char *src, char *dest, int width, int height, int unit = 1, int qfactor = 40);	

public:
	int getVwc(int width, int height);

public:
	void Yuv444Rotate(char *src, char *dest, int width, int height, int rotate, int unit = 1);

protected:
	void Yuv444ZoutHorResize(char *src, char *dst, int srcw, int srch, int dstw, int unit = 1);
	void Yuv444ZoutVerResize(char *src, char *dst, int srcw, int srch, int dsth, int unit = 1);

	void Yuv444ZinHorResize(char *src, char *dst, int srcw, int srch, int dstw, int unit = 1);
	void Yuv444ZinVerResize(char *src, char *dst, int srcw, int srch, int dsth, int unit = 1);

protected:
	int MRgb2Yuv444(char *src, char *dst, int width, int height, int cap = 256, int unit = 1);
	int MYuv444_Rgb(char *src, char *dst, int width, int height, int cap = 256, int unit = 1);
	int MYuv444Sizer(char *src, char *dest, int srcw, int dstw, int dsth, 
				int px, int py, int unit = 1);

protected:
	int MYuv444Prefilter(char *src, char *dest, int width, int height, 
					int count, int *filter, int unit = 1);
	int MYuv444Prefilter1(char *src, char *dest, int width, int height, 
					int count, int *filter, int unit = 1);
};


#endif


