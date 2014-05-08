#ifndef _RAW2RGBF_H_
#define _RAW2RGBF_H_


#ifndef TOOLCONVERT_DLL
	#define TOOLCONVERT_DLL  __declspec(dllimport)
#endif

#include "raw2rgb.h"
#include "toolfile/filebin.h"
#include "toolfile/dbugtools.h"

class TOOLCONVERT_DLL CRaw2RgbF : public CRaw2Rgb
{
public:
	void Rgb2GB(char *src, char *dest, int width, int height, int unit = 1);
	void Rgb2GR(char *src, char *dest, int width, int height, int unit = 1);
	void Rgb2BG(char *src, char *dest, int width, int height, int unit = 1);
	void Rgb2RG(char *src, char *dest, int width, int height, int unit = 1);

public:
	void GB2Rgb(char *src, char *dest, int width, int height, int unit = 1);
	void BG2Rgb(char *src, char *dest, int width, int height, int unit = 1);
	void GR2Rgb(char *src, char *dest, int width, int height, int unit = 1);
	void RG2Rgb(char *src, char *dest, int width, int height, int unit = 1);

public:	//indirect color convert
	void Rgb1_Rgb24(char *src, char *dst, int width, int height);
	void Rgb2_Rgb24(char *src, char *dst, int width, int height);
	void Rgb4_Rgb24(char *src, char *dst, int width, int height);
	void Rgb8_Rgb24(char *src, char *dst, int width, int height);
	void Rgb8_Rgb2(char *src, char *dst, int width, int height);

public:
	void Rgb444_Rgb24(char *src, char *dst, int width, int height);
	void Rgb555_Rgb24(char *src, char *dst, int width, int height);
	void Rgb565_Rgb24(char *src, char *dst, int width, int height);
	void Rgb666_Rgb24(char *src, char *dst, int width, int height);
	void Rgb32_Rgb24(char *src, char *dst, int width, int height);
	void Rgb555_Rgb32(char *src, char *dst, int width, int height);

public:
	void Rgb24_Rgb444(char *src, char *dst, int width, int height, int dither = 1);
	void Rgb24_Rgb666(char *src, char *dst, int width, int height, int dither = 1);
	void Rgb24_Rgb555(char *src, char *dst, int width, int height, int dither = 1);
	void Rgb24_Rgb565(char *src, char *dst, int width, int height, int dither = 1);
	void Rgb24_Rgb32(char *src, char *dst, int width, int height);

public:
	void Rgb32_Rgb555(char *src, char *dst, int width, int height, int dither = 1);	

public:
	void	Rgb32_Bmp(char *src, char *dest, int width, int height);
	void	Rgb24_Bmp(char *src, char *dest, int width, int height);
	void	Rgb666_Bmp(char *src, char *dest, int width, int height);
	void	Rgb565_Bmp(char *src, char *dest, int width, int height);
	void	Rgb555_Bmp(char *src, char *dest, int width, int height);
	void	Rgb444_Bmp(char *src, char *dest, int width, int height);
	void	Rgb8_Bmp(char *src, char *dest, int width, int height);
	void	Rgb4_Bmp(char *src, char *dest, int width, int height);
	void	Rgb2_Bmp(char *src, char *dest, int width, int height);
	void	Rgb1_Bmp(char *src, char *dest, int width, int height);

public:
	void	Bmp1_Rgb(char *src, char *dest);
	void	Bmp2_Rgb(char *src, char *dest);
	void	Bmp4_Rgb(char *src, char *dest);
	void	Bmp8_Rgb(char *src, char *dest);
	void	Bmp444_Rgb(char *src, char *dest);
	void	Bmp555_Rgb(char *src, char *dest);
	void	Bmp565_Rgb(char *src, char *dest);
	void	Bmp666_Rgb(char *src, char *dest);
	void	Bmp24_Rgb(char *src, char *dest);
	void	Bmp32_Rgb(char *src, char *dest);
	
public:
	int		GetBmpFileFormat(char *src);
	int		GetBmpFileBitWidth(char * src);

public:
	void	PrintBmpFileInfo(char *src, char *dest, TBitMap *bmpinfo, int print = 0);

protected:
	void	printbmpinfo(char *dest, char *buf);
	void	rgb2bayerorder(char *src, char *dst, int width, int height, int order, int unit = 1);

protected:
	void bayer2rgborder(char *src, char *dest, int order, int width, int height, int unit = 1);

protected:
	int add1palette(char *dstbuf);
	int add2palette(char *dstbuf);
	int add4palette(char *dstbuf);
	int add8palette(char *dstbuf);
	int add565palette(char *dstbuf);
	int add555palette(char *dstbuf);		//option

	int getbmpinfo(char *srcbuf, TBitMap *pinfo = NULL);
	void bitbmp2rgb(CFileBin *fsrc, CFileBin *fdst, int width, int height, int bitwidth);
	void bitrgb2bmp(CFileBin *fsrc, CFileBin *fdst, int width, int height, int bitwidth);

public:
	int	addbmp1header(char *dest, int width, int height);
	int	addbmp2header(char *dest, int width, int height);
	int	addbmp4header(char *dest, int width, int height);
	int	addbmp8header(char *dest, int width, int height);
	int	addbmp444header(char *dest, int width, int height);

	int	addbmp555header(char *dest, int width, int height);
	int	addbmp565header(char *dest, int width, int height);
	int	addbmp666header(char *dest, int width, int height);
	int	addbmp24header(char *dest, int width, int height);
	int	addbmp32header(char *dest, int width, int height);

public:
	void RgbResize(char *src, char *dest, int srcw, int srch, int dstw, int dsth, int unit = 1);
	void RgbSizer(char *src, char *dest, int srcw, int srch, int dstw, int dsth, int px, int py, int unit = 1);
	void RgbPrefilter(char *src, char *dest, int width, int height, 
					int count, int *filter, int unit = 1);
	void Rgb24Rotate(char *src, char *dest, int width, int height, int rotate, int unit = 1);

private:
	CDbugTools	m_dbug;
};


#endif


