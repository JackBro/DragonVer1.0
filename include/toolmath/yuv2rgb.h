#ifndef _YUV2RGB_H_
#define _YUV2RGB_H_


#ifndef TOOLMATH_DLL
	#define TOOLMATH_DLL  __declspec(dllimport)
#endif

#include "baseconvert.h"

class TOOLMATH_DLL CYuv2Rgb : public CBaseConvert
{
public:
	int  rgb2y(int *bgr, int cap = 256);
	int  rgb2u(int *bgr, int cap = 256);
	int  rgb2v(int *bgr, int cap = 256);
	int	 bgr2yuv(int bgr);
	void rgb2yuv(int *bgr, int *yuv, int cap = 256);

	int	 yuv2r(int *yuv, int cap = 256);
	int  yuv2g(int *yuv, int cap = 256);
	int  yuv2b(int *yuv, int cap = 256);
	int  yuv2bgr(int yuv);
	void yuv2rgb(int *yuv, int *bgr, int cap = 256);

	void getbgr(char *src, int *bgr, int unit = 1);

	void getyuv444(char *src, int *yuv, int unit = 1);

	void getyuv400(char *src, int *yuv, int unit = 1);
	void getyuvfrom400(char *src, int *yuv, int unit = 1);

	void getyuv422(char *src, int *yuv, int order, int unit = 1);
	void getyuvfrom422(char *src, int *yuv, int order, int unit = 1);

	void getyuv420(char *src, int *yuv, int order, int offset, int gap, int unit = 1);
	void getyuvfrom420(char *src, int *yuv, int order, int offset, int gap, int unit = 1);
	
	void getyuv411(char *src, int *yuv, int order, int unit = 1);
	void getyuvfrom411(char *src, int *yuv, int order, int unit = 1);

	void setbgr(char *dst, int *bgr, int unit = 1);

	void setyuv444(char *dst, int *yuv, int unit = 1);
	void setyuv422(char *dst, int *yuv, int order, int unit = 1);
	void setyuv411(char *dst, int *yuv, int order, int unit = 1);
	void setyuv420(char *dst, int *yuv, int order, int offset, int gap, int unit = 1);

	int prefilter(int *dat, int *filter, int count);
	void rgbprefilter(int *dat, int *filter, int count);
};


#endif


