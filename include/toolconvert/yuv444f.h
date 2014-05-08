#ifndef _YUV444F_H_
#define _YUV444F_H_


#ifndef TOOLCONVERT_DLL
	#define TOOLCONVERT_DLL  __declspec(dllimport)
#endif

#include "yuv444.h"

class TOOLCONVERT_DLL CYuv444F : public CYuv444
{
public:
	void Rgb2Yuv444(char *src, char *dest, int width, int height, int unit = 1);
	void Yuv444_Rgb(char *src, char *dest, int width, int height, int unit = 1);
	void Yuv444_Blk(char *src, char *dst, int width, int height, int unit = 1);
	void Blk2Yuv444(char *src, char *dst, int width, int height, int unit = 1);
	void Yuv444_Blk1(char *src, char *dst, int width, int height, int unit = 1);
	void Blk2Yuv4441(char *src, char *dst, int width, int height, int unit = 1);
	void Yuv444_Lbuf(char *src, char *dst, int width, int height, int unit = 1);
	void Lbuf2Yuv444(char *src, char *dst, int width, int height, int unit = 1);

public:
	void Yuv444Resize(char *src, char *dest, int srcw, int srch, int dstw, int dsth, int unit = 1);
	void Yuv444Sizer(char *src, char *dest, int srcw, int srch, int dstw, int dsth, int px, int py, int unit = 1);
	void Yuv444Prefilter(char *src, char *dest, int width, int height, 
					int count, int *filter, int unit = 1);
	void Yuv444Rotate(char *src, char *dest, int width, int height, int rotate, int unit = 1);

public:
	void Rgb2Yuv444Blk(char *src, char *dest, int width, int height, int unit = 1);
	void Blk2Rgb24(char *src, char *dest, int width, int height);
	void Blk2Jpeg(char *src, char *dest, int width, int height, int qf = 40);
	void Rgb2Jpeg444(char *src, char *dest, int width, int height, int unit = 1, int qfactor = 40);

private:
	CDbugTools	m_dbug;
};


#endif


