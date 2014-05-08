#ifndef _YUV411F_H_
#define _YUV411F_H_


#ifndef TOOLCONVERT_DLL
	#define TOOLCONVERT_DLL  __declspec(dllimport)
#endif

#include "yuv411.h"

class TOOLCONVERT_DLL CYuv411F : public CYuv411
{
public:
	void Rgb2Yyyyuv(char *src, char *dest, int width, int height, int unit = 1);
	void Rgb2Yyyyvu(char *src, char *dest, int width, int height, int unit = 1);
	void Rgb2Yuyvyy(char *src, char *dest, int width, int height, int unit = 1);
	void Rgb2Yvyuyy(char *src, char *dest, int width, int height, int unit = 1);
	void Rgb2Uvyyyy(char *src, char *dest, int width, int height, int unit = 1);
	void Rgb2Vuyyyy(char *src, char *dest, int width, int height, int unit = 1);

public:
	void Yyyyuv2Rgb(char *src, char *dest, int width, int height, int unit = 1);
	void Yyyyvu2Rgb(char *src, char *dest, int width, int height, int unit = 1);
	void Yuyvyy2Rgb(char *src, char *dest, int width, int height, int unit = 1);
	void Yvyuyy2Rgb(char *src, char *dest, int width, int height, int unit = 1);
	void Uvyyyy2Rgb(char *src, char *dest, int width, int height, int unit = 1);
	void Vuyyyy2Rgb(char *src, char *dest, int width, int height, int unit = 1);

public:
	void Yuv444_Yyyyuv(char *src, char *dest, int width, int height, int unit = 1);
	void Yuv444_Yyyyvu(char *src, char *dest, int width, int height, int unit = 1);
	void Yuv444_Yuyvyy(char *src, char *dest, int width, int height, int unit = 1);
	void Yuv444_Yvyuyy(char *src, char *dest, int width, int height, int unit = 1);
	void Yuv444_Uvyyyy(char *src, char *dest, int width, int height, int unit = 1);
	void Yuv444_Vuyyyy(char *src, char *dest, int width, int height, int unit = 1);

public:
	void Yyyyuv_444(char *src, char *dest, int width, int height, int unit = 1);
	void Yyyyvu_444(char *src, char *dest, int width, int height, int unit = 1);
	void Yuyvyy_444(char *src, char *dest, int width, int height, int unit = 1);
	void Yvyuyy_444(char *src, char *dest, int width, int height, int unit = 1);
	void Uvyyyy_444(char *src, char *dest, int width, int height, int unit = 1);
	void Vuyyyy_444(char *src, char *dest, int width, int height, int unit = 1);

public:
	void Yyyyuv2Blk(char *src, char *dst, int width, int height, int unit = 1);
	void Yyyyvu2Blk(char *src, char *dst, int width, int height, int unit = 1);
	void Yuyvyy2Blk(char *src, char *dst, int width, int height, int unit = 1);
	void Yvyuyy2Blk(char *src, char *dst, int width, int height, int unit = 1);
	void Uvyyyy2Blk(char *src, char *dst, int width, int height, int unit = 1);
	void Vuyyyy2Blk(char *src, char *dst, int width, int height, int unit = 1);

public:
	void Blk2Yyyyuv(char *src, char *dst, int width, int height, int unit = 1);
	void Blk2Yyyyvu(char *src, char *dst, int width, int height, int unit = 1);
	void Blk2Yuyvyy(char *src, char *dst, int width, int height, int unit = 1);
	void Blk2Yvyuyy(char *src, char *dst, int width, int height, int unit = 1);
	void Blk2Uvyyyy(char *src, char *dst, int width, int height, int unit = 1);
	void Blk2Vuyyyy(char *src, char *dst, int width, int height, int unit = 1);

	void Blk2Rgb24(char * src, char * dest, int width, int height);

public:
	void Yyyyuv2Lbuf(char *src, char *dst, int width, int height, int unit = 1);
	void Yyyyvu2Lbuf(char *src, char *dst, int width, int height, int unit = 1);
	void Yuyvyy2Lbuf(char *src, char *dst, int width, int height, int unit = 1);
	void Yvyuyy2Lbuf(char *src, char *dst, int width, int height, int unit = 1);
	void Uvyyyy2Lbuf(char *src, char *dst, int width, int height, int unit = 1);
	void Vuyyyy2Lbuf(char *src, char *dst, int width, int height, int unit = 1);

public:
	void Lbuf2Yyyyuv(char *src, char *dst, int width, int height, int unit = 1);
	void Lbuf2Yyyyvu(char *src, char *dst, int width, int height, int unit = 1);
	void Lbuf2Yuyvyy(char *src, char *dst, int width, int height, int unit = 1);
	void Lbuf2Yvyuyy(char *src, char *dst, int width, int height, int unit = 1);
	void Lbuf2Uvyyyy(char *src, char *dst, int width, int height, int unit = 1);
	void Lbuf2Vuyyyy(char *src, char *dst, int width, int height, int unit = 1);

public:
	void YyyyuvResize(char *src, char *dest, int srcw, int srch, int dstw, int dsth, int unit = 1);
	void YyyyvuResize(char *src, char *dest, int srcw, int srch, int dstw, int dsth, int unit = 1);
	void YuyvyyResize(char *src, char *dest, int srcw, int srch, int dstw, int dsth, int unit = 1);
	void YvyuyyResize(char *src, char *dest, int srcw, int srch, int dstw, int dsth, int unit = 1);
	void UvyyyyResize(char *src, char *dest, int srcw, int srch, int dstw, int dsth, int unit = 1);
	void VuyyyyResize(char *src, char *dest, int srcw, int srch, int dstw, int dsth, int unit = 1);

public:
	void YyyyuvSizer(char *src, char *dest, int srcw, int srch, int dstw, int dsth, int px, int py, int unit = 1);
	void YyyyvuSizer(char *src, char *dest, int srcw, int srch, int dstw, int dsth, int px, int py, int unit = 1);
	void YuyvyySizer(char *src, char *dest, int srcw, int srch, int dstw, int dsth, int px, int py, int unit = 1);
	void YvyuyySizer(char *src, char *dest, int srcw, int srch, int dstw, int dsth, int px, int py, int unit = 1);
	void UvyyyySizer(char *src, char *dest, int srcw, int srch, int dstw, int dsth, int px, int py, int unit = 1);
	void VuyyyySizer(char *src, char *dest, int srcw, int srch, int dstw, int dsth, int px, int py, int unit = 1);

public:
	void YyyyuvPrefilter(char *src, char *dest, int width, int height, 
					int count, int *filter, int unit = 1);
	void YyyyvuPrefilter(char *src, char *dest, int width, int height, 
					int count, int *filter, int unit = 1);
	void YuyvyyPrefilter(char *src, char *dest, int width, int height, 
					int count, int *filter, int unit = 1);
	void YvyuyyPrefilter(char *src, char *dest, int width, int height, 
					int count, int *filter, int unit = 1);
	void UvyyyyPrefilter(char *src, char *dest, int width, int height, 
					int count, int *filter, int unit = 1);
	void VuyyyyPrefilter(char *src, char *dest, int width, int height, 
					int count, int *filter, int unit = 1);

protected:
	void rgb2yuv411order(char *src, char *dst, int width, int height, int order, int unit = 1);
	void yuv411_rgborder(char *src, char *dst, int width, int height, int order, int unit = 1);
	void yuv444_411order(char *src, char *dst, int width, int height, int order, int unit = 1);
	void yuv411_444order(char *src, char *dst, int width, int height, int order, int unit = 1);
	void yuv411_blkorder(char *src, char *dst, int width, int height, int order, int unit = 1);
	void blk2yuv411order(char *src, char *dst, int width, int height, int order, int unit = 1);
	void yuv411_blkorder1(char *src, char *dst, int width, int height, int order, int unit = 1);
	void blk2yuv411order1(char *src, char *dst, int width, int height, int order, int unit = 1);
	void yuv411_lbuforder(char *src, char *dst, int width, int height, int order, int unit = 1);
	void lbuf2yuv411order(char *src, char *dst, int width, int height, int order, int unit = 1);

	void	Rgb2Yuv411Blk(char *src, char *dst, int width, int height, int unit = 1);
	void Rgb2Jpeg411(char *src, char *dest, int width, int height, int unit = 1, int qfactor = 40);	

protected:
	void yuv411_resizeorder(char *src, char *dest, int srcw, int srch, int dstw, int dsth, int order, int unit = 1);
	void yuv411sizerorder(char *src, char *dest, int srcw, int srch, int dstw, int dsth, 
					int px, int py, int order, int unit = 1);
	void Yuv411Prefilterorder(char *src, char *dest, int width, int height, int order,
					int count, int *filter, int unit = 1);

public:
	void Yuv411Prefilter(char *src, char *dest, int width, int height, 
					int count, int *filter, int unit = 1);

public:
	void Blk2Jpeg(char *src, char *dest, int width, int height, int qf = 40);

private:
	CDbugTools	m_dbug;
};


#endif


