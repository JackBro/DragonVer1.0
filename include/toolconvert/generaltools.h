#ifndef _GENERALTOOLS_H_
#define _GENERALTOOLS_H_


#ifndef TOOLCONVERT_DLL
	#define TOOLCONVERT_DLL  __declspec(dllimport)
#endif


#include "normaltools.h"
#include "raw2rgbf.h"
#include "yuv422f.h"
#include "yuv420f.h"
#include "yuv444f.h"
#include "yuv411f.h"
#include "yuv400f.h"
#include "rgb2yiqf.h"
#include "yiq2rgbf.h"

#include "tooljpg/jpegf.h"
#include "toolmpeg2i/mpeg2if.h"
#include "toolmpeg4/mpeg4f.h"

#define ERROR_VALUE		0xffffffff

class TOOLCONVERT_DLL CGeneralTools : public CNormalTools, public CRaw2RgbF,
					public CYuv422F, public CYuv420F, public CYuv444F, 
					public CYuv411F, public CYuv400F, public CJpegF, public CMpeg2IF,
					public CRgb2yiqF, public CYiq2rgbF, public CMpeg4F
{
public:
	void	DataConvert(char *src, char *dest, int width, int height, 
								int srcfmt, int dstfmt, int unit = 1, int dither = 0, int qfactor = 40);
public:
	void	Rgb24Convert(char *src, char *dest, int width, int height, 
		int dstfmt, int dither = 1, int qfactor = 40);
	int	Rgb24_YuvBlkBuffer(char *src, char *dest, int width, int height, int dstfmt);
	int	Rgb24_JpegBuffer(char *src, char *dest, int width, int height, int dstfmt, int qf = 40);

public:
	void	Rgb1Convert(char *src, char *dest, int width, int height, int dstfmt);
	void	Rgb2Convert(char *src, char *dest, int width, int height, int dstfmt);
	void	Rgb4Convert(char *src, char *dest, int width, int height, int dstfmt);
	void	Rgb8Convert(char *src, char *dest, int width, int height, int dstfmt);
	void	Rgb444Convert(char *src, char *dest, int width, int height, int dstfmt, int qf = 40, int unit = 1);

	void	Rgb555Convert(char *src, char *dest, int width, int height, int dstfmt);
	void	Rgb565Convert(char *src, char *dest, int width, int height, int dstfmt);
	void	Rgb666Convert(char *src, char *dest, int width, int height, int dstfmt);
	void	Rgb32Convert(char *src, char *dest, int width, int height, int dstfmt);

	void	Rgb2YuvBlkConvert(char *src, char *dest, int width, int height, int srcfmt, int dstfmt, int unit = 1);
	void	Rgb2JpegConvert(char *src, char *dest, int width, int height, int srcfmt, int dstfmt, int qf, int unit = 1);

public:
	void	YiqConvert(char *src, char *dest, int width, int height, int dstfmt, int unit = 1);

public:
	void	Yyuv420Convert(char *src, char *dest, int width, int height, int dstfmt, int unit = 1);
	void	Vyuy420Convert(char *src, char *dest, int width, int height, int dstfmt, int unit = 1);
	void	Yuyv420Convert(char *src, char *dest, int width, int height, int dstfmt, int unit = 1);
	void	Uyvy420Convert(char *src, char *dest, int width, int height, int dstfmt, int unit = 1);
	void	Yvyu420Convert(char *src, char *dest, int width, int height, int dstfmt, int unit = 1);

	void	Yuvy420Convert(char *src, char *dest, int width, int height, int dstfmt, int unit = 1);
	void	Yvuy420Convert(char *src, char *dest, int width, int height, int dstfmt, int unit = 1);
	void	Uvyy420Convert(char *src, char *dest, int width, int height, int dstfmt, int unit = 1);
	void	Vuyy420Convert(char *src, char *dest, int width, int height, int dstfmt, int unit = 1);
	void	Yyvu420Convert(char *src, char *dest, int width, int height, int dstfmt, int unit = 1);

public:
	void	Yyuv422Convert(char *src, char *dest, int width, int height, int dstfmt, int unit = 1);
	void	Vyuy422Convert(char *src, char *dest, int width, int height, int dstfmt, int unit = 1);
	void	Yuyv422Convert(char *src, char *dest, int width, int height, int dstfmt, int unit = 1);
	void	Uyvy422Convert(char *src, char *dest, int width, int height, int dstfmt, int unit = 1);
	void	Yvyu422Convert(char *src, char *dest, int width, int height, int dstfmt, int unit = 1);

	void	Yuvy422Convert(char *src, char *dest, int width, int height, int dstfmt, int unit = 1);
	void	Yvuy422Convert(char *src, char *dest, int width, int height, int dstfmt, int unit = 1);
	void	Uvyy422Convert(char *src, char *dest, int width, int height, int dstfmt, int unit = 1);
	void	Vuyy422Convert(char *src, char *dest, int width, int height, int dstfmt, int unit = 1);
	void	Yyvu422Convert(char *src, char *dest, int width, int height, int dstfmt, int unit = 1);

public:
	void	Yyyyuv411Convert(char *src, char *dest, int width, int height, int dstfmt, int unit = 1);
	void	Yyyyvu411Convert(char *src, char *dest, int width, int height, int dstfmt, int unit = 1);
	void	Yuyvyy411Convert(char *src, char *dest, int width, int height, int dstfmt, int unit = 1);
	void	Yvyuyy411Convert(char *src, char *dest, int width, int height, int dstfmt, int unit = 1);
	void	Uvyyyy411Convert(char *src, char *dest, int width, int height, int dstfmt, int unit = 1);
	void	Vuyyyy411Convert(char *src, char *dest, int width, int height, int dstfmt, int unit = 1);

public:
	void	Yuv400BlkConvert(char *src, char *dest, int width, int height, int dstfmt, int unit = 1);
	void	Yuv411BlkConvert(char *src, char *dest, int width, int height, int dstfmt, int unit = 1);
	void	Yuv422BlkConvert(char *src, char *dest, int width, int height, int dstfmt, int unit = 1, int qf = 40);
	void	Yuv420BlkConvert(char *src, char *dest, int width, int height, int dstfmt, int unit = 1);
	void	Yuv444BlkConvert(char *src, char *dest, int width, int height, int dstfmt, int unit = 1);

public:
	void	Yuv422LbufConvert(char *src, char *dest, int width, int height, int dstfmt, int unit = 1);
	void	Yuv444422LbufConvert(char *src, char *dest, int width, int height, int dstfmt, int unit = 1);
	void	Yuv411LbufConvert(char *src, char *dest, int width, int height, int dstfmt, int unit = 1);
	void	Yuv400LbufConvert(char *src, char *dest, int width, int height, int dstfmt, int unit = 1);
	void	Yuv444LbufConvert(char *src, char *dest, int width, int height, int dstfmt, int unit = 1);
	void	Yuv420LbufConvert(char *src, char *dest, int width, int height, int dstfmt, int unit = 1);

public:
	void	Yuv400Convert(char *src, char *dest, int width, int height, int dstfmt, int unit = 1);

public:
	void	Yuv444Convert(char *src, char *dest, int width, int height, int dstfmt, int unit = 1);

public:
	void	BayerGBConvert(char *src, char *dest, int width, int height, int dstfmt, int unit = 1);
	void	BayerGRConvert(char *src, char *dest, int width, int height, int dstfmt, int unit = 1);
	void	BayerBGConvert(char *src, char *dest, int width, int height, int dstfmt, int unit = 1);
	void	BayerRGConvert(char *src, char *dest, int width, int height, int dstfmt, int unit = 1);

public:
	void	Yuv422FrmConvert(char *src, char *dest, int width, int height, int dstfmt, int unit = 1);
	void	Yuv420FrmConvert(char *src, char *dest, int width, int height, int dstfmt, int unit = 1);

public:
	void	Rgb2BmpConvert(char *src, char *dest, int width, int height, int srcfmt);
	void	BmpConvert(char *src, char *dest, int dstfmt, int qfactor = 40);
	void	Bmp2YuvBlkConvert(char *src,  char *dest, int dstfmt);
	void	Bmp2JpegConvert(char *src,  char *dest, int dstfmt, int qfactor = 40);
	void	Blk2Bmp(char *src, char *dest, int width, int height, int srcfmt);
	void	Jpeg2Rgb24(char *src, char *dest);
	void	Jpeg2Bmp(char *src, char *dest);

public:
	void	ImageResize(char *src, char *dest, int srcw, int srch, 
						int dstw, int dsth, int fmt, int unit = 1);

public:
	void	ImageSizer(char *src, char *dest, int srcw, int srch, 
						int dstw, int dsth, int px, int py, int fmt, int unit = 1);

public:
	void	ImagePrefilter(char *src, char *dest, int width, int height,
						int count, int *filter, int fmt, int unit = 1);

public:
	int UnitConvert(int val, int srcfmt, int dstfmt);

public:
	void	JpegConvert(char *src, char *dest, int dstfmt);

public:
	void	Mpeg2Convert(char *src, char *dest);

public:
	void	Mpeg4Convert(char *src, char *dest);

public:
	void	ImageRotate(char *src, char *dest, int width, int height, int fmt, int rotate, int unit = 1);

private:
	CDbugTools	m_dbug;
};


#endif


