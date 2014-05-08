#ifndef _YUV400F_H_
#define _YUV400F_H_


#ifndef TOOLCONVERT_DLL
	#define TOOLCONVERT_DLL  __declspec(dllimport)
#endif

#include "yuv400.h"

class TOOLCONVERT_DLL CYuv400F : public CYuv400
{
public:
	void Rgb2Yuv400(char *src, char *dest, int width, int height, int unit = 1);
	void Yuv400_Rgb(char *src, char *dest, int width, int height, int unit = 1);

	void Yuv444_400(char *src, char *dst, int width, int height, int unit = 1);
	void Yuv400_444(char *src, char *dst, int width, int height, int unit = 1);

	void Yuv2Blk(char *src, char *dst, int width, int height, int unit = 1);
	void Blk2Yuv(char *src, char *dst, int width, int height, int unit = 1);
	void Yuv2Blk1(char *src, char *dst, int width, int height, int unit = 1);
	void Blk2Yuv1(char *src, char *dst, int width, int height, int unit = 1);
	void Yuv2Lbuf(char *src, char *dst, int width, int height, int unit = 1);
	void Lbuf2Yuv(char *src, char *dst, int width, int height, int unit = 1);

	void Rgb2Yuv400Blk(char *src, char *dest, int width, int height, int unit = 1);
	void Blk2Rgb24(char *src, char *dest, int width, int height);
	void Blk2Jpeg(char *src, char *dest, int width, int height, int qf = 40);
	void Rgb2Jpeg400(char *src, char *dest, int width, int height, int unit = 1, int qf = 40);

public:
	void Yuv400Resize(char *src, char *dest, int srcw, int srch, int dstw, int dsth, int unit = 1);
	void Yuv400Sizer(char *src, char *dest, int srcw, int srch, int dstw, int dsth, int px, int py, int unit = 1);
	void Yuv400Prefilter(char *src, char *dest, int width, int height, 
					int count, int *filter, int unit = 1);

private:
	CDbugTools	m_dbug;
};


#endif


