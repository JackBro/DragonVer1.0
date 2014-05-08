#ifndef _YUV400_H_
#define _YUV400_H_


#ifndef TOOLCONVERT_DLL
	#define TOOLCONVERT_DLL  __declspec(dllimport)
#endif

#include "yuv444.h"


class TOOLCONVERT_DLL CYuv400 : public CYuv444
{
public:
	int Rgb2Yuv400(char *src, char *dst, int cap = 256, int unit = 1);
	int Yuv400_Rgb(char *src, char *dst, int cap = 256, int unit = 1);

	int Yuv444_400(char *src, char *dst, int unit = 1);
	int Yuv400_444(char *src, char *dst, int unit = 1);

	int	Yuv2Blk(char *src, char *dst, int width, int height, int unit = 1);
	int	Blk2Yuv(char *src, char *dst, int width, int height, int unit = 1);
	int	Yuv2Lbuf(char *src, char *dst, int width, int height, int unit = 1);
	int	Lbuf2Yuv(char *src, char *dst, int width, int height, int unit = 1);

public:
	void Yuv400ZoutHorResize(char *src, char *dst, int srcw, int srch, int dstw, int unit = 1);
	void Yuv400ZinHorResize(char *src, char *dst, int srcw, int srch, int dstw, int unit = 1);
	void Yuv400ZoutVerResize(char *src, char *dst, int srcw, int srch, int dsth, int unit = 1);
	void Yuv400ZinVerResize(char *src, char *dst, int srcw, int srch, int dsth, int unit = 1);
	int Yuv400Resize(char *src, char *dst, int srcw, int srch, int dstw, int dsth, int unit = 1);

protected:
	int MRgb2Yuv400(char *src, char *dest, int width, int height, int cap = 256, int unit = 1);
	int MYuv400_Rgb(char *src, char *dest, int width, int height, int cap = 256, int unit = 1);
	int MYuv444_400(char *src, char *dest, int width, int height, int unit = 1);
	int MYuv400_444(char *src, char *dest, int width, int height, int unit = 1);

	int MYuv400Sizer(char *src, char *dest, int srcw, int dstw, int dsth, 
				int px, int py, int unit = 1);

public:
	int Blk2Rgb24(char *src, char *dest, int width, int height);
	int Rgb2Yuv400Blk(char *src, char *dest, int width, int height, int cap = 256, int unit = 1);
	int Blk2Jpeg(char *src, char *dest, int width, int height, int qf = 40);
	int Rgb2Jpeg400(char * src, char * dst, int width, int height, int unit = 1, int qf = 40);

public:
	int getVwc(int width, int height);

protected:
	int Yuv400HorExtent(char *src, char *dest, int width, int height, int count, int unit = 1);
	int Yuv400VerExtent(char *src, char *dest, int width, int height, int count, int unit = 1);
	int Yuv400HorPrefilter(char *src, char *dest, int width, int height, 
				int count, int *filter, int unit = 1);
	int Yuv400VerPrefilter(char *src, char *dest, int width, int height, 
				int count, int *filter, int unit = 1);
	int MYuv400Enlarge(char *src, char *dest, int width, int height, int count, int unit = 1);
	int MYuv400Prefilter(char *src, char *dest, int width, int height, 
					int count, int *filter, int unit = 1);
};


#endif


