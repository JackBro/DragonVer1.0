#ifndef _RAW2RGB_H_
#define _RAW2RGB_H_


#ifndef TOOLCONVERT_DLL
	#define TOOLCONVERT_DLL  __declspec(dllimport)
#endif
#include "toolmath/yuv2rgb.h"

class TOOLCONVERT_DLL CRaw2Rgb : public CYuv2Rgb
{
public:
	int Rgb2GB(char *src, char *dst, int width, int height, int unit = 1);
	int Rgb2BG(char *src, char *dst, int width, int height, int unit = 1);
	int Rgb2GR(char *src, char *dst, int width, int height, int unit = 1);
	int Rgb2RG(char *src, char *dst, int width, int height, int unit = 1);

public:
	int GB2Rgb(char *src, char *dst, int width, int height, int unit = 1);
	int BG2Rgb(char *src, char *dst, int width, int height, int unit = 1);
	int GR2Rgb(char *src, char *dst, int width, int height, int unit = 1);
	int RG2Rgb(char *src, char *dst, int width, int height, int unit = 1);
	int bitbmp2rgb(char *src, char *dst, int width, int height, int bitwidth);
	int bitrgb2bmp(char *src, char *dst, int width, int height, int bitwidth);
	int addbmp24header1(char *dst, int width, int height);

//====================================
//
//	below services only support R, G, B component 8bit
//
//=======================================
public:	//indirect color convert
	void Rgb1_Rgb24(int bitvalue, char *dst, char *palette);
	void Rgb2_Rgb24(int bitvalue, char *dst, char *palette);
	void Rgb4_Rgb24(int bitvalue, char *dst, char *palette);
	void Rgb8_Rgb24(int bitvalue, char *dst, char *palette);
	void Rgb8_Rgb2(int bitvalue, char *dst, int dststart = 0);

public:	//direct color convert
	void Rgb444_Rgb24(int bitvalue, char *dst);
	void Rgb666_Rgb24(int bitvalue, char *dst);

	void Rgb555_Rgb24(char *src, char *dst);
	void Rgb565_Rgb24(char *src, char *dst);
	void Rgb32_Rgb24(char *src, char *dst);
	void Rgb24_Rgb32(char *src, char *dst);

public:
	int Rgb_Rgb24Convert(char *src, char *dest, int width, int height, int srcfmt);

public:	//none alpha bit direct color convert
	int Rgb24_Rgb444(char *src, char *dst, int width, int height, int startbit = 0, int dither = 1);
		//return bit length
	int Rgb24_Rgb666(char *src, char *dst, int width, int height, int startbit = 0, int dither = 1);
		//return bit length
	int Rgb24_Rgb555(char *src, char *dst, int width, int height, int dither = 1);	//return byte length
	int Rgb24_Rgb565(char *src, char *dst, int width, int height, int dither = 1);	//return byte length

public:	//alpha bit direct color convert
	virtual void Rgb555_Rgb32(char *src, char *dst);
	int Rgb32_Rgb555(char *src, char *dst, int width, int height, int dither = 1);

public:
	int RgbResize(char *src, char *dst, int srcw, int srch, int dstw, int dsth, int unit = 1);

public:
	int MRgbSizer(char *src, char *dest, int srcw, int dstw, int dsth, 
				int px, int py, int unit = 1);
public:
	void Rgb24Rotate(char *src, char *dest, int width, int height, int rotate, int unit = 1);

public:
	int Rgb2Rgb24(char *src, char *dest, int srcw, int srch, int unit = 1);
	int RgbGetEdge(char *src, char *dest, int srcw, int srch, int unit = 1);

protected:
	void RgbZoutHorSizer(char *src, char *dst, int srcw, int srch, int dstw, int unit = 1);
	void RgbZoutVerSizer(char *src, char *dst, int srcw, int srch, int dsth, int unit = 1);

	void RgbZinHorSizer(char *src, char *dst, int srcw, int srch, int dstw, int unit = 1);
	void RgbZinVerSizer(char *src, char *dst, int srcw, int srch, int dsth, int unit = 1);

protected:
	int rgb2bayerorder(char *src, char *dst, int order, int width, int height, int unit = 1);

protected:
	int bayer2rgborder(char *src, char *dst, int order, int width, int height, int unit = 1);

protected:
	int MRgbPrefilter(char *src, char *dest, int width, int height, 
					int count, int *filter, int unit = 1);
	int MRgbPrefilter1(char *src, char *dest, int width, int height, 
					int count, int *filter, int unit = 1);
};


#endif


