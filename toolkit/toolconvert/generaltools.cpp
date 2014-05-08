
#include "internal.h"

//***************************************************//

//===================================================
//
//			Rgb Convert
//
//==============================================

//***************************************************//
void CGeneralTools::Rgb24Convert(char *src, char *dest, int width, int height, int dstfmt, int dither, int qfactor)
{
	switch(dstfmt)
	{
	case YUV422_UYVY:
		CYuv422F::Rgb2Uyvy(src, dest, width, height, 1);
		break;
	case YUV422_VYUY:
		CYuv422F::Rgb2Vyuy(src, dest, width, height, 1);
		break;
	case YUV422_YUYV:
		CYuv422F::Rgb2Yuyv(src, dest, width, height, 1);
		break;
	case YUV422_YVYU:
		CYuv422F::Rgb2Yvyu(src, dest, width, height, 1);
		break;
	case YUV422_YUVY:
		CYuv422F::Rgb2Yuvy(src, dest, width, height, 1);
		break;
	case YUV422_YVUY:
		CYuv422F::Rgb2Yvuy(src, dest, width, height, 1);
		break;
	case YUV422_UVYY:
		CYuv422F::Rgb2Uvyy(src, dest, width, height, 1);
		break;
	case YUV422_VUYY:
		CYuv422F::Rgb2Vuyy(src, dest, width, height, 1);
		break;
	case YUV422_YYVU:
		CYuv422F::Rgb2Yyvu(src, dest, width, height, 1);
		break;
	case RAW_BG:
		CRaw2RgbF::Rgb2BG(src, dest, width, height, 1);
		break;
	case RAW_GB:
		CRaw2RgbF::Rgb2GB(src, dest, width, height, 1);
		break;
	case RAW_RG:
		CRaw2RgbF::Rgb2RG(src, dest, width, height, 1);
		break;
	case RAW_GR:
		CRaw2RgbF::Rgb2GR(src, dest, width, height, 1);
		break;
	
	case RGB1:
		break;
	case RGB2:
		break;
	case RGB4:
		break;
	case RGB8:
		break;
	case RGB444:
		CRaw2RgbF::Rgb24_Rgb444(src, dest, width, height, dither);
		break;
	case RGB666:
		CRaw2RgbF::Rgb24_Rgb666(src, dest, width, height, dither);
		break;
	case RGB32:
		CRaw2RgbF::Rgb24_Rgb32(src, dest, width, height);
		break;
	case RGB24:
		break;
	case RGB565:
		CRaw2RgbF::Rgb24_Rgb565(src, dest, width, height, dither);
		break;
	case RGB555:
		CRaw2RgbF::Rgb24_Rgb555(src, dest, width, height, dither);
		break;
	case YUV422_YYUV:
		CYuv422F::Rgb2Yyuv(src, dest, width, height, 1);
		break;
	case YUV411_YYYYUV:
		CYuv411F::Rgb2Yyyyuv(src, dest, width, height, 1);
		break;
	case YUV411_YYYYVU:
		CYuv411F::Rgb2Yyyyvu(src, dest, width, height, 1);
		break;
	case YUV411_YUYVYY:
		CYuv411F::Rgb2Yuyvyy(src, dest, width, height, 1);
		break;
	case YUV411_YVYUYY:
		CYuv411F::Rgb2Yvyuyy(src, dest, width, height, 1);
		break;
	case YUV411_UVYYYY:
		CYuv411F::Rgb2Uvyyyy(src, dest, width, height, 1);
		break;
	case YUV411_VUYYYY:
		CYuv411F::Rgb2Vuyyyy(src, dest, width, height, 1);
		break;
	case YUV420_UYVY:
		CYuv420F::Rgb2Uyvy(src, dest, width, height, 1);
		break;
	case YUV420_VYUY:
		CYuv420F::Rgb2Vyuy(src, dest, width, height, 1);
		break;
	case YUV420_YUYV:
		CYuv420F::Rgb2Yuyv(src, dest, width, height, 1);
		break;
	case YUV420_YVYU:
		CYuv420F::Rgb2Yvyu(src, dest, width, height, 1);
		break;
	case YUV420_YUVY:
		CYuv420F::Rgb2Yuvy(src, dest, width, height, 1);
		break;
	case YUV420_YVUY:
		CYuv420F::Rgb2Yvuy(src, dest, width, height, 1);
		break;
	case YUV420_UVYY:
		CYuv420F::Rgb2Uvyy(src, dest, width, height, 1);
		break;
	case YUV420_VUYY:
		CYuv420F::Rgb2Vuyy(src, dest, width, height, 1);
		break;
	case YUV420_YYVU:
		CYuv420F::Rgb2Yyvu(src, dest, width, height, 1);
		break;
	case YUV420_YYUV:
		CYuv420F::Rgb2Yyuv(src, dest, width, height, 1);
		break;
	case YUV400:
		CYuv400F::Rgb2Yuv400(src, dest, width, height, 1);
		break;
	case YUV444:
		CYuv444F::Rgb2Yuv444(src, dest, width, height, 1);
		break;
	case YUV422BLK:
		CYuv422F::Rgb2Yuv422Blk(src, dest, width, height, 1);
		break;
	case YUV411BLK:
		CYuv411F::Rgb2Yuv411Blk(src, dest, width, height, 1);
		break;
	case YUV420BLK:
		CYuv420F::Rgb2Yuv420Blk(src, dest, width, height, 1);
		break;
	case YUV400BLK:
		CYuv400F::Rgb2Yuv400Blk(src, dest, width, height, 1);
		break;
	case YUV444BLK:
		CYuv444F::Rgb2Yuv444Blk(src, dest, width, height, 1);
		break;
	case BMP:
		this->Rgb2BmpConvert(src, dest, width, height, RGB24);
		break;
	case YIQ:
		CRgb2yiqF::rgb2yiq(src, dest, width, height);
		break;
	case JPEG422:
		CYuv422F::Rgb2Jpeg422(src, dest, width, height, 1, qfactor);
		break;
	case JPEG420:
		CYuv420F::Rgb2Jpeg420(src, dest, width, height, 1, qfactor);
		break;
	case JPEG411:
		CYuv411F::Rgb2Jpeg411(src, dest, width, height, 1, qfactor);
		break;
	case JPEG400:
		CYuv400F::Rgb2Jpeg400(src, dest, width, height, 1, qfactor);
		break;
	case JPEG444:
		CYuv444F::Rgb2Jpeg444(src, dest, width, height, 1, qfactor);
		break;
	default:
		break;
	}
}

//===================================================
//
//			Yyuv 422 Convert
//
//==============================================
void CGeneralTools::Yyuv422Convert(char *src, char *dest, int width, int height, int dstfmt, int unit)
{
	switch(dstfmt)
	{
	case YUV422_UYVY:
	case YUV422_VYUY:
	case YUV422_YUYV:
	case YUV422_YVYU:
	case YUV422_YUVY:
	case YUV422_YVUY:
	case YUV422_UVYY:
	case YUV422_VUYY:
	case YUV422_YYVU:
		break;
	case RAW_BG:
	case RAW_GB:
	case RAW_RG:
	case RAW_GR:
		break;
	case RGB1:
	case RGB2:
	case RGB4:
	case RGB8:
	case RGB444:
	case RGB666:
	case RGB32:
		break;

	case RGB24:
		CYuv422F::Yyuv2Rgb(src, dest, width, height, unit);
		break;
	case RGB565:
	case RGB555:
		break;
	case YUV422_YYUV:
		break;
	case YUV411_YYYYUV:
	case YUV411_YYYYVU:
	case YUV411_YUYVYY:
	case YUV411_YVYUYY:
	case YUV411_UVYYYY:
	case YUV411_VUYYYY:
	case YUV420_UYVY:
	case YUV420_VYUY:
	case YUV420_YUYV:
	case YUV420_YVYU:
	case YUV420_YUVY:
	case YUV420_YVUY:
	case YUV420_UVYY:
	case YUV420_VUYY:
	case YUV420_YYVU:
		break;
	case YUV420_YYUV:
	case YUV400:
		break;
	case YUV444:
		CYuv422F::Yyuv2Yuv444(src, dest, width, height, unit);
		break;
	case YUV422BLK:
		CYuv422F::Yyuv2Blk(src, dest, width, height, unit);
		break;
	case YUV411BLK:
	case YUV420BLK:
	case YUV400BLK:
	case YUV444BLK:
		break;
	case YUV422LBUF:
		CYuv422F::Yyuv2Lbuf(src, dest, width, height, unit);
		break;
	case YUV411LBUF:
	case YUV420LBUF:
	case YUV400LBUF:
	case YUV444LBUF:
		break;
	case BMP:
	case JPEG422:
	case JPEG420:
	case JPEG411:
	case JPEG400:
	case JPEG444:
		break;
	case YUV422FRM:
		CYuv422F::Yyuv2Frame(src, dest, width, height, unit);
		break;
	default:
		break;
	}
}

//===================================================
//
//			Vyuy 422 Convert
//
//==============================================
void CGeneralTools::Vyuy422Convert(char *src, char *dest, int width, int height, int dstfmt, int unit)
{
	switch(dstfmt)
	{
	case YUV422_UYVY:
	case YUV422_VYUY:
	case YUV422_YUYV:
	case YUV422_YVYU:
	case YUV422_YUVY:
	case YUV422_YVUY:
	case YUV422_UVYY:
	case YUV422_VUYY:
	case YUV422_YYVU:
		break;
	case RAW_BG:
	case RAW_GB:
	case RAW_RG:
	case RAW_GR:
		break;
	case RGB1:
	case RGB2:
	case RGB4:
	case RGB8:
	case RGB444:
	case RGB666:
	case RGB32:
		break;

	case RGB24:
		CYuv422F::Vyuy2Rgb(src, dest, width, height, unit);
		break;
	case RGB565:
	case RGB555:
		break;
	case YUV422_YYUV:
		break;
	case YUV411_YYYYUV:
	case YUV411_YYYYVU:
	case YUV411_YUYVYY:
	case YUV411_YVYUYY:
	case YUV411_UVYYYY:
	case YUV411_VUYYYY:
	case YUV420_UYVY:
	case YUV420_VYUY:
	case YUV420_YUYV:
	case YUV420_YVYU:
	case YUV420_YUVY:
	case YUV420_YVUY:
	case YUV420_UVYY:
	case YUV420_VUYY:
	case YUV420_YYVU:
		break;
	case YUV420_YYUV:
	case YUV400:
		break;
	case YUV444:
		CYuv422F::Vyuy2Yuv444(src, dest, width, height, unit);
		break;
	case YUV422BLK:
		CYuv422F::Vyuy2Blk(src, dest, width, height, unit);
		break;
	case YUV411BLK:
	case YUV420BLK:
	case YUV400BLK:
	case YUV444BLK:
		break;
	case YUV422LBUF:
		CYuv422F::Vyuy2Lbuf(src, dest, width, height, unit);
		break;
	case YUV411LBUF:
	case YUV420LBUF:
	case YUV400LBUF:
	case YUV444LBUF:
		break;
	case BMP:
	case JPEG422:
	case JPEG420:
	case JPEG411:
	case JPEG400:
	case JPEG444:
		break;
	case YUV422FRM:
		CYuv422F::Vyuy2Frame(src, dest, width, height, unit);
		break;
	default:
		break;
	}
}

//===================================================
//
//			Yuyv 422 Convert
//
//==============================================
void CGeneralTools::Yuyv422Convert(char *src, char *dest, int width, int height, int dstfmt, int unit)
{
	switch(dstfmt)
	{
	case YUV422_UYVY:
	case YUV422_VYUY:
	case YUV422_YUYV:
	case YUV422_YVYU:
	case YUV422_YUVY:
	case YUV422_YVUY:
	case YUV422_UVYY:
	case YUV422_VUYY:
	case YUV422_YYVU:
		break;
	case RAW_BG:
	case RAW_GB:
	case RAW_RG:
	case RAW_GR:
		break;
	case RGB1:
	case RGB2:
	case RGB4:
	case RGB8:
	case RGB444:
	case RGB666:
	case RGB32:
		break;

	case RGB24:
		CYuv422F::Yuyv2Rgb(src, dest, width, height, unit);
		break;
	case RGB565:
	case RGB555:
		break;
	case YUV422_YYUV:
		break;
	case YUV411_YYYYUV:
	case YUV411_YYYYVU:
	case YUV411_YUYVYY:
	case YUV411_YVYUYY:
	case YUV411_UVYYYY:
	case YUV411_VUYYYY:
	case YUV420_UYVY:
	case YUV420_VYUY:
	case YUV420_YUYV:
	case YUV420_YVYU:
	case YUV420_YUVY:
	case YUV420_YVUY:
	case YUV420_UVYY:
	case YUV420_VUYY:
	case YUV420_YYVU:
		break;
	case YUV420_YYUV:
	case YUV400:
		break;
	case YUV444:
		CYuv422F::Yuyv2Yuv444(src, dest, width, height, unit);
		break;
	case YUV422BLK:
		CYuv422F::Yuyv2Blk(src, dest, width, height, unit);
		break;
	case YUV411BLK:
	case YUV420BLK:
	case YUV400BLK:
	case YUV444BLK:
		break;
	case YUV422LBUF:
		CYuv422F::Yuyv2Lbuf(src, dest, width, height, unit);
		break;
	case YUV411LBUF:
	case YUV420LBUF:
	case YUV400LBUF:
	case YUV444LBUF:
		break;
	case BMP:
	case JPEG422:
	case JPEG420:
	case JPEG411:
	case JPEG400:
	case JPEG444:
		break;
	case YUV422FRM:
		CYuv422F::Yuyv2Frame(src, dest, width, height, unit);
		break;


	default:
		break;
	}
}

//===================================================
//
//			Uyvy 422 Convert
//
//==============================================
void CGeneralTools::Uyvy422Convert(char *src, char *dest, int width, int height, int dstfmt, int unit)
{
	switch(dstfmt)
	{
	case YUV422_UYVY:
	case YUV422_VYUY:
	case YUV422_YUYV:
	case YUV422_YVYU:
	case YUV422_YUVY:
	case YUV422_YVUY:
	case YUV422_UVYY:
	case YUV422_VUYY:
	case YUV422_YYVU:
		break;
	case RAW_BG:
	case RAW_GB:
	case RAW_RG:
	case RAW_GR:
		break;
	case RGB1:
	case RGB2:
	case RGB4:
	case RGB8:
	case RGB444:
	case RGB666:
	case RGB32:
		break;

	case RGB24:
		CYuv422F::Uyvy2Rgb(src, dest, width, height, unit);
		break;
	case RGB565:
	case RGB555:
		break;
	case YUV422_YYUV:
		break;
	case YUV411_YYYYUV:
	case YUV411_YYYYVU:
	case YUV411_YUYVYY:
	case YUV411_YVYUYY:
	case YUV411_UVYYYY:
	case YUV411_VUYYYY:
	case YUV420_UYVY:
	case YUV420_VYUY:
	case YUV420_YUYV:
	case YUV420_YVYU:
	case YUV420_YUVY:
	case YUV420_YVUY:
	case YUV420_UVYY:
	case YUV420_VUYY:
	case YUV420_YYVU:
		break;
	case YUV420_YYUV:
	case YUV400:
		break;
	case YUV444:
		CYuv422F::Uyvy2Yuv444(src, dest, width, height, unit);
		break;
	case YUV422BLK:
		CYuv422F::Uyvy2Blk(src, dest, width, height, unit);
		break;
	case YUV411BLK:
	case YUV420BLK:
	case YUV400BLK:
	case YUV444BLK:
		break;
	case YUV422LBUF:
		CYuv422F::Uyvy2Lbuf(src, dest, width, height, unit);
		break;
	case YUV411LBUF:
	case YUV420LBUF:
	case YUV400LBUF:
	case YUV444LBUF:
		break;
	case BMP:
	case JPEG422:
	case JPEG420:
	case JPEG411:
	case JPEG400:
	case JPEG444:
		break;
	case YUV422FRM:
		CYuv422F::Uyvy2Frame(src, dest, width, height, unit);
		break;

	default:
		break;
	}
}

//===================================================
//
//			YVYU 422 Convert
//
//==============================================
void CGeneralTools::Yvyu422Convert(char *src, char *dest, int width, int height, int dstfmt, int unit)
{
	switch(dstfmt)
	{
	case YUV422_UYVY:
	case YUV422_VYUY:
	case YUV422_YUYV:
	case YUV422_YVYU:
	case YUV422_YUVY:
	case YUV422_YVUY:
	case YUV422_UVYY:
	case YUV422_VUYY:
	case YUV422_YYVU:
		break;
	case RAW_BG:
	case RAW_GB:
	case RAW_RG:
	case RAW_GR:
		break;
	case RGB1:
	case RGB2:
	case RGB4:
	case RGB8:
	case RGB444:
	case RGB666:
	case RGB32:
		break;

	case RGB24:
		CYuv422F::Yvyu2Rgb(src, dest, width, height, unit);
		break;
	case RGB565:
	case RGB555:
		break;
	case YUV422_YYUV:
		break;
	case YUV411_YYYYUV:
	case YUV411_YYYYVU:
	case YUV411_YUYVYY:
	case YUV411_YVYUYY:
	case YUV411_UVYYYY:
	case YUV411_VUYYYY:
	case YUV420_UYVY:
	case YUV420_VYUY:
	case YUV420_YUYV:
	case YUV420_YVYU:
	case YUV420_YUVY:
	case YUV420_YVUY:
	case YUV420_UVYY:
	case YUV420_VUYY:
	case YUV420_YYVU:
		break;
	case YUV420_YYUV:
	case YUV400:
		break;
	case YUV444:
		CYuv422F::Yvyu2Yuv444(src, dest, width, height, unit);
		break;
	case YUV422BLK:
		CYuv422F::Yvyu2Blk(src, dest, width, height, unit);
		break;
	case YUV411BLK:
	case YUV420BLK:
	case YUV400BLK:
	case YUV444BLK:
		break;
	case YUV422LBUF:
		CYuv422F::Yvyu2Lbuf(src, dest, width, height, unit);
		break;
	case YUV411LBUF:
	case YUV420LBUF:
	case YUV400LBUF:
	case YUV444LBUF:
		break;
	case BMP:
	case JPEG422:
	case JPEG420:
	case JPEG411:
	case JPEG400:
	case JPEG444:
		break;
	case YUV422FRM:
		CYuv422F::Yvyu2Frame(src, dest, width, height, unit);
		break;

	default:
		break;
	}
}

//===================================================
//
//			YUVY 422 Convert
//
//==============================================
void CGeneralTools::Yuvy422Convert(char *src, char *dest, int width, int height, int dstfmt, int unit)
{
	switch(dstfmt)
	{
	case YUV422_UYVY:
	case YUV422_VYUY:
	case YUV422_YUYV:
	case YUV422_YVYU:
	case YUV422_YUVY:
	case YUV422_YVUY:
	case YUV422_UVYY:
	case YUV422_VUYY:
	case YUV422_YYVU:
		break;
	case RAW_BG:
	case RAW_GB:
	case RAW_RG:
	case RAW_GR:
		break;
	case RGB1:
	case RGB2:
	case RGB4:
	case RGB8:
	case RGB444:
	case RGB666:
	case RGB32:
		break;

	case RGB24:
		CYuv422F::Yuvy2Rgb(src, dest, width, height, unit);
		break;
	case RGB565:
	case RGB555:
		break;
	case YUV422_YYUV:
		break;
	case YUV411_YYYYUV:
	case YUV411_YYYYVU:
	case YUV411_YUYVYY:
	case YUV411_YVYUYY:
	case YUV411_UVYYYY:
	case YUV411_VUYYYY:
	case YUV420_UYVY:
	case YUV420_VYUY:
	case YUV420_YUYV:
	case YUV420_YVYU:
	case YUV420_YUVY:
	case YUV420_YVUY:
	case YUV420_UVYY:
	case YUV420_VUYY:
	case YUV420_YYVU:
		break;
	case YUV420_YYUV:
	case YUV400:
		break;
	case YUV444:
		CYuv422F::Yuvy2Yuv444(src, dest, width, height, unit);
		break;
	case YUV422BLK:
		CYuv422F::Yuvy2Blk(src, dest, width, height, unit);
		break;
	case YUV411BLK:
	case YUV420BLK:
	case YUV400BLK:
	case YUV444BLK:
		break;
	case YUV422LBUF:
		CYuv422F::Yuvy2Lbuf(src, dest, width, height, unit);
		break;
	case YUV411LBUF:
	case YUV420LBUF:
	case YUV400LBUF:
	case YUV444LBUF:
		break;
	case BMP:
	case JPEG422:
	case JPEG420:
	case JPEG411:
	case JPEG400:
	case JPEG444:
		break;
	case YUV422FRM:
		CYuv422F::Yuvy2Frame(src, dest, width, height, unit);
		break;

	default:
		break;
	}
}

//===================================================
//
//			YVUY 422 Convert
//
//==============================================
void CGeneralTools::Yvuy422Convert(char *src, char *dest, int width, int height, int dstfmt, int unit)
{
	switch(dstfmt)
	{
	case YUV422_UYVY:
	case YUV422_VYUY:
	case YUV422_YUYV:
	case YUV422_YVYU:
	case YUV422_YUVY:
	case YUV422_YVUY:
	case YUV422_UVYY:
	case YUV422_VUYY:
	case YUV422_YYVU:
		break;
	case RAW_BG:
	case RAW_GB:
	case RAW_RG:
	case RAW_GR:
		break;
	case RGB1:
	case RGB2:
	case RGB4:
	case RGB8:
	case RGB444:
	case RGB666:
	case RGB32:
		break;

	case RGB24:
		CYuv422F::Yvuy2Rgb(src, dest, width, height, unit);
		break;
	case RGB565:
	case RGB555:
		break;
	case YUV422_YYUV:
		break;
	case YUV411_YYYYUV:
	case YUV411_YYYYVU:
	case YUV411_YUYVYY:
	case YUV411_YVYUYY:
	case YUV411_UVYYYY:
	case YUV411_VUYYYY:
	case YUV420_UYVY:
	case YUV420_VYUY:
	case YUV420_YUYV:
	case YUV420_YVYU:
	case YUV420_YUVY:
	case YUV420_YVUY:
	case YUV420_UVYY:
	case YUV420_VUYY:
	case YUV420_YYVU:
		break;
	case YUV420_YYUV:
	case YUV400:
		break;
	case YUV444:
		CYuv422F::Yvuy2Yuv444(src, dest, width, height, unit);
		break;
	case YUV422BLK:
		CYuv422F::Yvuy2Blk(src, dest, width, height, unit);
		break;
	case YUV411BLK:
	case YUV420BLK:
	case YUV400BLK:
	case YUV444BLK:
		break;
	case YUV422LBUF:
		CYuv422F::Yvuy2Lbuf(src, dest, width, height, unit);
		break;
	case YUV411LBUF:
	case YUV420LBUF:
	case YUV400LBUF:
	case YUV444LBUF:
		break;
	case BMP:
	case JPEG422:
	case JPEG420:
	case JPEG411:
	case JPEG400:
	case JPEG444:
		break;
	case YUV422FRM:
		CYuv422F::Yvuy2Frame(src, dest, width, height, unit);
		break;

	default:
		break;
	}
}

//===================================================
//
//			UVYY 422 Convert
//
//==============================================
void CGeneralTools::Uvyy422Convert(char *src, char *dest, int width, int height, int dstfmt, int unit)
{
	switch(dstfmt)
	{
	case YUV422_UYVY:
	case YUV422_VYUY:
	case YUV422_YUYV:
	case YUV422_YVYU:
	case YUV422_YUVY:
	case YUV422_YVUY:
	case YUV422_UVYY:
	case YUV422_VUYY:
	case YUV422_YYVU:
		break;
	case RAW_BG:
	case RAW_GB:
	case RAW_RG:
	case RAW_GR:
		break;
	case RGB1:
	case RGB2:
	case RGB4:
	case RGB8:
	case RGB444:
	case RGB666:
	case RGB32:
		break;

	case RGB24:
		CYuv422F::Uvyy2Rgb(src, dest, width, height, unit);
		break;
	case RGB565:
	case RGB555:
		break;
	case YUV422_YYUV:
		break;
	case YUV411_YYYYUV:
	case YUV411_YYYYVU:
	case YUV411_YUYVYY:
	case YUV411_YVYUYY:
	case YUV411_UVYYYY:
	case YUV411_VUYYYY:
	case YUV420_UYVY:
	case YUV420_VYUY:
	case YUV420_YUYV:
	case YUV420_YVYU:
	case YUV420_YUVY:
	case YUV420_YVUY:
	case YUV420_UVYY:
	case YUV420_VUYY:
	case YUV420_YYVU:
		break;
	case YUV420_YYUV:
	case YUV400:
		break;
	case YUV444:
		CYuv422F::Uvyy2Yuv444(src, dest, width, height, unit);
		break;
	case YUV422BLK:
		CYuv422F::Uvyy2Blk(src, dest, width, height, unit);
		break;
	case YUV411BLK:
	case YUV420BLK:
	case YUV400BLK:
	case YUV444BLK:
		break;
	case YUV422LBUF:
		CYuv422F::Uvyy2Lbuf(src, dest, width, height, unit);
		break;
	case YUV411LBUF:
	case YUV420LBUF:
	case YUV400LBUF:
	case YUV444LBUF:
		break;
	case BMP:
	case JPEG422:
	case JPEG420:
	case JPEG411:
	case JPEG400:
	case JPEG444:
		break;
	case YUV422FRM:
		CYuv422F::Uvyy2Frame(src, dest, width, height, unit);
		break;

	default:
		break;
	}
}

//===================================================
//
//			VUYY 422 Convert
//
//==============================================
void CGeneralTools::Vuyy422Convert(char *src, char *dest, int width, int height, int dstfmt, int unit)
{
	switch(dstfmt)
	{
	case YUV422_UYVY:
	case YUV422_VYUY:
	case YUV422_YUYV:
	case YUV422_YVYU:
	case YUV422_YUVY:
	case YUV422_YVUY:
	case YUV422_UVYY:
	case YUV422_VUYY:
	case YUV422_YYVU:
		break;
	case RAW_BG:
	case RAW_GB:
	case RAW_RG:
	case RAW_GR:
		break;
	case RGB1:
	case RGB2:
	case RGB4:
	case RGB8:
	case RGB444:
	case RGB666:
	case RGB32:
		break;

	case RGB24:
		CYuv422F::Vuyy2Rgb(src, dest, width, height, unit);
		break;
	case RGB565:
	case RGB555:
		break;
	case YUV422_YYUV:
		break;
	case YUV411_YYYYUV:
	case YUV411_YYYYVU:
	case YUV411_YUYVYY:
	case YUV411_YVYUYY:
	case YUV411_UVYYYY:
	case YUV411_VUYYYY:
	case YUV420_UYVY:
	case YUV420_VYUY:
	case YUV420_YUYV:
	case YUV420_YVYU:
	case YUV420_YUVY:
	case YUV420_YVUY:
	case YUV420_UVYY:
	case YUV420_VUYY:
	case YUV420_YYVU:
		break;
	case YUV420_YYUV:
	case YUV400:
		break;
	case YUV444:
		CYuv422F::Vuyy2Yuv444(src, dest, width, height, unit);
		break;
	case YUV422BLK:
		CYuv422F::Vuyy2Blk(src, dest, width, height, unit);
		break;
	case YUV411BLK:
	case YUV420BLK:
	case YUV400BLK:
	case YUV444BLK:
		break;
	case YUV422LBUF:
		CYuv422F::Vuyy2Lbuf(src, dest, width, height, unit);
		break;
	case YUV411LBUF:
	case YUV420LBUF:
	case YUV400LBUF:
	case YUV444LBUF:
		break;
	case BMP:
	case JPEG422:
	case JPEG420:
	case JPEG411:
	case JPEG400:
	case JPEG444:
		break;
	case YUV422FRM:
		CYuv422F::Vuyy2Frame(src, dest, width, height, unit);
		break;

	default:
		break;
	}
}

//===================================================
//
//			YYVU 422 Convert
//
//==============================================
void CGeneralTools::Yyvu422Convert(char *src, char *dest, int width, int height, int dstfmt, int unit)
{
	switch(dstfmt)
	{
	case YUV422_UYVY:
	case YUV422_VYUY:
	case YUV422_YUYV:
	case YUV422_YVYU:
	case YUV422_YUVY:
	case YUV422_YVUY:
	case YUV422_UVYY:
	case YUV422_VUYY:
	case YUV422_YYVU:
		break;
	case RAW_BG:
	case RAW_GB:
	case RAW_RG:
	case RAW_GR:
		break;
	case RGB1:
	case RGB2:
	case RGB4:
	case RGB8:
	case RGB444:
	case RGB666:
	case RGB32:
		break;

	case RGB24:
		CYuv422F::Yyvu2Rgb(src, dest, width, height, unit);
		break;
	case RGB565:
	case RGB555:
		break;
	case YUV422_YYUV:
		break;
	case YUV411_YYYYUV:
	case YUV411_YYYYVU:
	case YUV411_YUYVYY:
	case YUV411_YVYUYY:
	case YUV411_UVYYYY:
	case YUV411_VUYYYY:
	case YUV420_UYVY:
	case YUV420_VYUY:
	case YUV420_YUYV:
	case YUV420_YVYU:
	case YUV420_YUVY:
	case YUV420_YVUY:
	case YUV420_UVYY:
	case YUV420_VUYY:
	case YUV420_YYVU:
		break;
	case YUV420_YYUV:
	case YUV400:
		break;
	case YUV444:
		CYuv422F::Yyvu2Yuv444(src, dest, width, height, unit);
		break;
	case YUV422BLK:
		CYuv422F::Yyvu2Blk(src, dest, width, height, unit);
		break;
	case YUV411BLK:
	case YUV420BLK:
	case YUV400BLK:
	case YUV444BLK:
		break;
	case YUV422LBUF:
		CYuv422F::Yyvu2Lbuf(src, dest, width, height, unit);
		break;
	case YUV411LBUF:
	case YUV420LBUF:
	case YUV400LBUF:
	case YUV444LBUF:
		break;
	case BMP:
	case JPEG422:
	case JPEG420:
	case JPEG411:
	case JPEG400:
	case JPEG444:
		break;
	case YUV422FRM:
		CYuv422F::Yyvu2Frame(src, dest, width, height, unit);
		break;

	default:
		break;
	}
}

//***************************************************//

//===================================================
//
//			Yiq Convert
//
//==============================================

//***************************************************//
void CGeneralTools::YiqConvert(char *src, char *dest, int width, int height, int dstfmt, int unit)
{
    switch(dstfmt)
	{
	    case RGB24:
			CYiq2rgbF::yiq2rgb(src, dest, width, height, unit);
			break;

		default:
			break;
	}
}

//***************************************************//

//===================================================
//
//			Yyuv 420 Convert
//
//==============================================

//***************************************************//
void CGeneralTools::Yyuv420Convert(char *src, char *dest, int width, int height, int dstfmt, int unit)
{
	switch(dstfmt)
	{
	case YUV422_UYVY:
	case YUV422_VYUY:
	case YUV422_YUYV:
	case YUV422_YVYU:
	case YUV422_YUVY:
	case YUV422_YVUY:
	case YUV422_UVYY:
	case YUV422_VUYY:
	case YUV422_YYVU:
		break;
	case RAW_BG:
	case RAW_GB:
	case RAW_RG:
	case RAW_GR:
		break;
	case RGB1:
	case RGB2:
	case RGB4:
	case RGB8:
	case RGB444:
	case RGB666:
	case RGB32:
		break;

	case RGB24:
		CYuv420F::Yyuv2Rgb(src, dest, width, height, unit);
		break;
	case RGB565:
	case RGB555:
		break;
	case YUV422_YYUV:
		break;
	case YUV411_YYYYUV:
	case YUV411_YYYYVU:
	case YUV411_YUYVYY:
	case YUV411_YVYUYY:
	case YUV411_UVYYYY:
	case YUV411_VUYYYY:
	case YUV420_UYVY:
	case YUV420_VYUY:
	case YUV420_YUYV:
	case YUV420_YVYU:
	case YUV420_YUVY:
	case YUV420_YVUY:
	case YUV420_UVYY:
	case YUV420_VUYY:
	case YUV420_YYVU:
		break;
	case YUV420_YYUV:
	case YUV400:
		break;
	case YUV444:
		CYuv420F::Yyuv2Yuv444(src, dest, width, height, unit);
		break;
	case YUV422BLK:
	case YUV411BLK:
		break;
	case YUV420BLK:
		CYuv420F::Yyuv2Blk(src, dest, width, height, unit);
		break;
	case YUV400BLK:
	case YUV444BLK:
	case YUV422LBUF:
	case YUV411LBUF:
		break;
	case YUV420LBUF:
		CYuv420F::Yyuv2Lbuf(src, dest, width, height, unit);
		break;
	case YUV400LBUF:
	case YUV444LBUF:
		break;
	case BMP:
	case JPEG422:
	case JPEG420:
	case JPEG411:
	case JPEG400:
	case JPEG444:
		break;
	case YUV420FRM:
		CYuv420F::Yyuv2Frame(src, dest, width, height, unit);
		break;
	default:
		break;
	}
}

//===================================================
//
//			Vyuy 420 Convert
//
//==============================================
void CGeneralTools::Vyuy420Convert(char *src, char *dest, int width, int height, int dstfmt, int unit)
{
	switch(dstfmt)
	{
	case YUV422_UYVY:
	case YUV422_VYUY:
	case YUV422_YUYV:
	case YUV422_YVYU:
	case YUV422_YUVY:
	case YUV422_YVUY:
	case YUV422_UVYY:
	case YUV422_VUYY:
	case YUV422_YYVU:
		break;
	case RAW_BG:
	case RAW_GB:
	case RAW_RG:
	case RAW_GR:
		break;
	case RGB1:
	case RGB2:
	case RGB4:
	case RGB8:
	case RGB444:
	case RGB666:
	case RGB32:
		break;

	case RGB24:
		CYuv420F::Vyuy2Rgb(src, dest, width, height, unit);
		break;
	case RGB565:
	case RGB555:
		break;
	case YUV422_YYUV:
		break;
	case YUV411_YYYYUV:
	case YUV411_YYYYVU:
	case YUV411_YUYVYY:
	case YUV411_YVYUYY:
	case YUV411_UVYYYY:
	case YUV411_VUYYYY:
	case YUV420_UYVY:
	case YUV420_VYUY:
	case YUV420_YUYV:
	case YUV420_YVYU:
	case YUV420_YUVY:
	case YUV420_YVUY:
	case YUV420_UVYY:
	case YUV420_VUYY:
	case YUV420_YYVU:
		break;
	case YUV420_YYUV:
	case YUV400:
		break;
	case YUV444:
		CYuv420F::Vyuy2Yuv444(src, dest, width, height, unit);
		break;
	case YUV422BLK:
	case YUV411BLK:
		break;
	case YUV420BLK:
		CYuv420F::Vyuy2Blk(src, dest, width, height, unit);
		break;
	case YUV400BLK:
	case YUV444BLK:
	case YUV422LBUF:
	case YUV411LBUF:
		break;
	case YUV420LBUF:
		CYuv420F::Vyuy2Lbuf(src, dest, width, height, unit);
		break;
	case YUV400LBUF:
	case YUV444LBUF:
		break;
	case BMP:
	case JPEG422:
	case JPEG420:
	case JPEG411:
	case JPEG400:
	case JPEG444:
		break;
	case YUV420FRM:
		CYuv420F::Vyuy2Frame(src, dest, width, height, unit);
		break;
	default:
		break;
	}
}

//===================================================
//
//			Yuyv 420 Convert
//
//==============================================
void CGeneralTools::Yuyv420Convert(char *src, char *dest, int width, int height, int dstfmt, int unit)
{
	switch(dstfmt)
	{
	case YUV422_UYVY:
	case YUV422_VYUY:
	case YUV422_YUYV:
	case YUV422_YVYU:
	case YUV422_YUVY:
	case YUV422_YVUY:
	case YUV422_UVYY:
	case YUV422_VUYY:
	case YUV422_YYVU:
		break;
	case RAW_BG:
	case RAW_GB:
	case RAW_RG:
	case RAW_GR:
		break;
	case RGB1:
	case RGB2:
	case RGB4:
	case RGB8:
	case RGB444:
	case RGB666:
	case RGB32:
		break;

	case RGB24:
		CYuv420F::Yuyv2Rgb(src, dest, width, height, unit);
		break;
	case RGB565:
	case RGB555:
		break;
	case YUV422_YYUV:
		break;
	case YUV411_YYYYUV:
	case YUV411_YYYYVU:
	case YUV411_YUYVYY:
	case YUV411_YVYUYY:
	case YUV411_UVYYYY:
	case YUV411_VUYYYY:
	case YUV420_UYVY:
	case YUV420_VYUY:
	case YUV420_YUYV:
	case YUV420_YVYU:
	case YUV420_YUVY:
	case YUV420_YVUY:
	case YUV420_UVYY:
	case YUV420_VUYY:
	case YUV420_YYVU:
		break;
	case YUV420_YYUV:
	case YUV400:
		break;
	case YUV444:
		CYuv420F::Yuyv2Yuv444(src, dest, width, height, unit);
		break;
	case YUV422BLK:
	case YUV411BLK:
		break;
	case YUV420BLK:
		CYuv420F::Yuyv2Blk(src, dest, width, height, unit);
		break;
	case YUV400BLK:
	case YUV444BLK:
	case YUV422LBUF:
	case YUV411LBUF:
		break;
	case YUV420LBUF:
		CYuv420F::Yuyv2Lbuf(src, dest, width, height, unit);
		break;
	case YUV400LBUF:
	case YUV444LBUF:
		break;
	case BMP:
	case JPEG422:
	case JPEG420:
	case JPEG411:
	case JPEG400:
	case JPEG444:
		break;
	case YUV420FRM:
		CYuv420F::Yuyv2Frame(src, dest, width, height, unit);
		break;
	default:
		break;
	}
}

//===================================================
//
//			Uyvy 420 Convert
//
//==============================================
void CGeneralTools::Uyvy420Convert(char *src, char *dest, int width, int height, int dstfmt, int unit)
{
	switch(dstfmt)
	{
	case YUV422_UYVY:
	case YUV422_VYUY:
	case YUV422_YUYV:
	case YUV422_YVYU:
	case YUV422_YUVY:
	case YUV422_YVUY:
	case YUV422_UVYY:
	case YUV422_VUYY:
	case YUV422_YYVU:
		break;
	case RAW_BG:
	case RAW_GB:
	case RAW_RG:
	case RAW_GR:
		break;
	case RGB1:
	case RGB2:
	case RGB4:
	case RGB8:
	case RGB444:
	case RGB666:
	case RGB32:
		break;

	case RGB24:
		CYuv420F::Uyvy2Rgb(src, dest, width, height, unit);
		break;
	case RGB565:
	case RGB555:
		break;
	case YUV422_YYUV:
		break;
	case YUV411_YYYYUV:
	case YUV411_YYYYVU:
	case YUV411_YUYVYY:
	case YUV411_YVYUYY:
	case YUV411_UVYYYY:
	case YUV411_VUYYYY:
	case YUV420_UYVY:
	case YUV420_VYUY:
	case YUV420_YUYV:
	case YUV420_YVYU:
	case YUV420_YUVY:
	case YUV420_YVUY:
	case YUV420_UVYY:
	case YUV420_VUYY:
	case YUV420_YYVU:
		break;
	case YUV420_YYUV:
	case YUV400:
		break;
	case YUV444:
		CYuv420F::Uyvy2Yuv444(src, dest, width, height, unit);
		break;
	case YUV422BLK:
	case YUV411BLK:
		break;
	case YUV420BLK:
		CYuv420F::Uyvy2Blk(src, dest, width, height, unit);
		break;
	case YUV400BLK:
	case YUV444BLK:
	case YUV422LBUF:
	case YUV411LBUF:
		break;
	case YUV420LBUF:
		CYuv420F::Uyvy2Lbuf(src, dest, width, height, unit);
		break;
	case YUV400LBUF:
	case YUV444LBUF:
		break;
	case BMP:
	case JPEG422:
	case JPEG420:
	case JPEG411:
	case JPEG400:
	case JPEG444:
		break;
	case YUV420FRM:
		CYuv420F::Uyvy2Frame(src, dest, width, height, unit);
		break;
	default:
		break;
	}
}

//===================================================
//
//			YVYU 420 Convert
//
//==============================================
void CGeneralTools::Yvyu420Convert(char *src, char *dest, int width, int height, int dstfmt, int unit)
{
	switch(dstfmt)
	{
	case YUV422_UYVY:
	case YUV422_VYUY:
	case YUV422_YUYV:
	case YUV422_YVYU:
	case YUV422_YUVY:
	case YUV422_YVUY:
	case YUV422_UVYY:
	case YUV422_VUYY:
	case YUV422_YYVU:
		break;
	case RAW_BG:
	case RAW_GB:
	case RAW_RG:
	case RAW_GR:
		break;
	case RGB1:
	case RGB2:
	case RGB4:
	case RGB8:
	case RGB444:
	case RGB666:
	case RGB32:
		break;

	case RGB24:
		CYuv420F::Yvyu2Rgb(src, dest, width, height, unit);
		break;
	case RGB565:
	case RGB555:
		break;
	case YUV422_YYUV:
		break;
	case YUV411_YYYYUV:
	case YUV411_YYYYVU:
	case YUV411_YUYVYY:
	case YUV411_YVYUYY:
	case YUV411_UVYYYY:
	case YUV411_VUYYYY:
	case YUV420_UYVY:
	case YUV420_VYUY:
	case YUV420_YUYV:
	case YUV420_YVYU:
	case YUV420_YUVY:
	case YUV420_YVUY:
	case YUV420_UVYY:
	case YUV420_VUYY:
	case YUV420_YYVU:
		break;
	case YUV420_YYUV:
	case YUV400:
		break;
	case YUV444:
		CYuv420F::Yvyu2Yuv444(src, dest, width, height, unit);
		break;
	case YUV422BLK:
	case YUV411BLK:
		break;
	case YUV420BLK:
		CYuv420F::Yvyu2Blk(src, dest, width, height, unit);
		break;
	case YUV400BLK:
	case YUV444BLK:
	case YUV422LBUF:
	case YUV411LBUF:
		break;
	case YUV420LBUF:
		CYuv420F::Yvyu2Lbuf(src, dest, width, height, unit);
		break;
	case YUV400LBUF:
	case YUV444LBUF:
		break;
	case BMP:
	case JPEG422:
	case JPEG420:
	case JPEG411:
	case JPEG400:
	case JPEG444:
		break;
	case YUV420FRM:
		CYuv420F::Yvyu2Frame(src, dest, width, height, unit);
		break;
	default:
		break;
	}
}

//===================================================
//
//			YUVY 420 Convert
//
//==============================================
void CGeneralTools::Yuvy420Convert(char *src, char *dest, int width, int height, int dstfmt, int unit)
{
	switch(dstfmt)
	{
	case YUV422_UYVY:
	case YUV422_VYUY:
	case YUV422_YUYV:
	case YUV422_YVYU:
	case YUV422_YUVY:
	case YUV422_YVUY:
	case YUV422_UVYY:
	case YUV422_VUYY:
	case YUV422_YYVU:
		break;
	case RAW_BG:
	case RAW_GB:
	case RAW_RG:
	case RAW_GR:
		break;
	case RGB1:
	case RGB2:
	case RGB4:
	case RGB8:
	case RGB444:
	case RGB666:
	case RGB32:
		break;

	case RGB24:
		CYuv420F::Yuvy2Rgb(src, dest, width, height, unit);
		break;
	case RGB565:
	case RGB555:
		break;
	case YUV422_YYUV:
		break;
	case YUV411_YYYYUV:
	case YUV411_YYYYVU:
	case YUV411_YUYVYY:
	case YUV411_YVYUYY:
	case YUV411_UVYYYY:
	case YUV411_VUYYYY:
	case YUV420_UYVY:
	case YUV420_VYUY:
	case YUV420_YUYV:
	case YUV420_YVYU:
	case YUV420_YUVY:
	case YUV420_YVUY:
	case YUV420_UVYY:
	case YUV420_VUYY:
	case YUV420_YYVU:
		break;
	case YUV420_YYUV:
	case YUV400:
		break;
	case YUV444:
		CYuv420F::Yuvy2Yuv444(src, dest, width, height, unit);
		break;
	case YUV422BLK:
	case YUV411BLK:
	case YUV420BLK:
		CYuv420F::Yuvy2Blk(src, dest, width, height, unit);
		break;
	case YUV400BLK:
	case YUV444BLK:
	case YUV422LBUF:
	case YUV411LBUF:
		break;
	case YUV420LBUF:
		CYuv420F::Yuvy2Lbuf(src, dest, width, height, unit);
		break;
	case YUV400LBUF:
	case YUV444LBUF:
		break;
	case BMP:
	case JPEG422:
	case JPEG420:
	case JPEG411:
	case JPEG400:
	case JPEG444:
		break;
	case YUV420FRM:
		CYuv420F::Yuvy2Frame(src, dest, width, height, unit);
		break;
	default:
		break;
	}
}

//===================================================
//
//			YVUY 420 Convert
//
//==============================================
void CGeneralTools::Yvuy420Convert(char *src, char *dest, int width, int height, int dstfmt, int unit)
{
	switch(dstfmt)
	{
	case YUV422_UYVY:
	case YUV422_VYUY:
	case YUV422_YUYV:
	case YUV422_YVYU:
	case YUV422_YUVY:
	case YUV422_YVUY:
	case YUV422_UVYY:
	case YUV422_VUYY:
	case YUV422_YYVU:
		break;
	case RAW_BG:
	case RAW_GB:
	case RAW_RG:
	case RAW_GR:
		break;
	case RGB1:
	case RGB2:
	case RGB4:
	case RGB8:
	case RGB444:
	case RGB666:
	case RGB32:
		break;

	case RGB24:
		CYuv420F::Yvuy2Rgb(src, dest, width, height, unit);
		break;
	case RGB565:
	case RGB555:
		break;
	case YUV422_YYUV:
		break;
	case YUV411_YYYYUV:
	case YUV411_YYYYVU:
	case YUV411_YUYVYY:
	case YUV411_YVYUYY:
	case YUV411_UVYYYY:
	case YUV411_VUYYYY:
	case YUV420_UYVY:
	case YUV420_VYUY:
	case YUV420_YUYV:
	case YUV420_YVYU:
	case YUV420_YUVY:
	case YUV420_YVUY:
	case YUV420_UVYY:
	case YUV420_VUYY:
	case YUV420_YYVU:
		break;
	case YUV420_YYUV:
	case YUV400:
		break;
	case YUV444:
		CYuv420F::Yvuy2Yuv444(src, dest, width, height, unit);
		break;
	case YUV422BLK:
	case YUV411BLK:
		break;
	case YUV420BLK:
		CYuv420F::Yvuy2Blk(src, dest, width, height, unit);
		break;
	case YUV400BLK:
	case YUV444BLK:
	case YUV422LBUF:
	case YUV411LBUF:
		break;
	case YUV420LBUF:
		CYuv420F::Yvuy2Lbuf(src, dest, width, height, unit);
		break;
	case YUV400LBUF:
	case YUV444LBUF:
		break;
	case BMP:
	case JPEG422:
	case JPEG420:
	case JPEG411:
	case JPEG400:
	case JPEG444:
		break;
	case YUV420FRM:
		CYuv420F::Yvuy2Frame(src, dest, width, height, unit);
		break;
	default:
		break;
	}
}

//===================================================
//
//			UVYY 420 Convert
//
//==============================================
void CGeneralTools::Uvyy420Convert(char *src, char *dest, int width, int height, int dstfmt, int unit)
{
	switch(dstfmt)
	{
	case YUV422_UYVY:
	case YUV422_VYUY:
	case YUV422_YUYV:
	case YUV422_YVYU:
	case YUV422_YUVY:
	case YUV422_YVUY:
	case YUV422_UVYY:
	case YUV422_VUYY:
	case YUV422_YYVU:
		break;
	case RAW_BG:
	case RAW_GB:
	case RAW_RG:
	case RAW_GR:
		break;
	case RGB1:
	case RGB2:
	case RGB4:
	case RGB8:
	case RGB444:
	case RGB666:
	case RGB32:
		break;

	case RGB24:
		CYuv420F::Uvyy2Rgb(src, dest, width, height, unit);
		break;
	case RGB565:
	case RGB555:
		break;
	case YUV422_YYUV:
		break;
	case YUV411_YYYYUV:
	case YUV411_YYYYVU:
	case YUV411_YUYVYY:
	case YUV411_YVYUYY:
	case YUV411_UVYYYY:
	case YUV411_VUYYYY:
	case YUV420_UYVY:
	case YUV420_VYUY:
	case YUV420_YUYV:
	case YUV420_YVYU:
	case YUV420_YUVY:
	case YUV420_YVUY:
	case YUV420_UVYY:
	case YUV420_VUYY:
	case YUV420_YYVU:
		break;
	case YUV420_YYUV:
	case YUV400:
		break;
	case YUV444:
		CYuv420F::Uvyy2Yuv444(src, dest, width, height, unit);
		break;
	case YUV422BLK:
	case YUV411BLK:
		break;
	case YUV420BLK:
		CYuv420F::Uvyy2Blk(src, dest, width, height, unit);
		break;
	case YUV400BLK:
	case YUV444BLK:
	case YUV422LBUF:
	case YUV411LBUF:
		break;
	case YUV420LBUF:
		CYuv420F::Uvyy2Lbuf(src, dest, width, height, unit);
		break;
	case YUV400LBUF:
	case YUV444LBUF:
		break;
	case BMP:
	case JPEG422:
	case JPEG420:
	case JPEG411:
	case JPEG400:
	case JPEG444:
		break;
	case YUV420FRM:
		CYuv420F::Uvyy2Frame(src, dest, width, height, unit);
		break;
	default:
		break;
	}
}

//===================================================
//
//			VUYY 420 Convert
//
//==============================================
void CGeneralTools::Vuyy420Convert(char *src, char *dest, int width, int height, int dstfmt, int unit)
{
	switch(dstfmt)
	{
	case YUV422_UYVY:
	case YUV422_VYUY:
	case YUV422_YUYV:
	case YUV422_YVYU:
	case YUV422_YUVY:
	case YUV422_YVUY:
	case YUV422_UVYY:
	case YUV422_VUYY:
	case YUV422_YYVU:
		break;
	case RAW_BG:
	case RAW_GB:
	case RAW_RG:
	case RAW_GR:
		break;
	case RGB1:
	case RGB2:
	case RGB4:
	case RGB8:
	case RGB444:
	case RGB666:
	case RGB32:
		break;

	case RGB24:
		CYuv420F::Vuyy2Rgb(src, dest, width, height, unit);
		break;
	case RGB565:
	case RGB555:
		break;
	case YUV422_YYUV:
		break;
	case YUV411_YYYYUV:
	case YUV411_YYYYVU:
	case YUV411_YUYVYY:
	case YUV411_YVYUYY:
	case YUV411_UVYYYY:
	case YUV411_VUYYYY:
	case YUV420_UYVY:
	case YUV420_VYUY:
	case YUV420_YUYV:
	case YUV420_YVYU:
	case YUV420_YUVY:
	case YUV420_YVUY:
	case YUV420_UVYY:
	case YUV420_VUYY:
	case YUV420_YYVU:
		break;
	case YUV420_YYUV:
	case YUV400:
		break;
	case YUV444:
		CYuv420F::Vuyy2Yuv444(src, dest, width, height, unit);
		break;
	case YUV422BLK:
	case YUV411BLK:
		break;
	case YUV420BLK:
		CYuv420F::Vuyy2Blk(src, dest, width, height, unit);
		break;
	case YUV400BLK:
	case YUV444BLK:
	case YUV422LBUF:
	case YUV411LBUF:
		break;
	case YUV420LBUF:
		CYuv420F::Vuyy2Lbuf(src, dest, width, height, unit);
		break;
	case YUV400LBUF:
	case YUV444LBUF:
		break;
	case BMP:
	case JPEG422:
	case JPEG420:
	case JPEG411:
	case JPEG400:
	case JPEG444:
		break;
	case YUV420FRM:
		CYuv420F::Vuyy2Frame(src, dest, width, height, unit);
		break;
	default:
		break;
	}
}

//===================================================
//
//			YYVU 420 Convert
//
//==============================================
void CGeneralTools::Yyvu420Convert(char *src, char *dest, int width, int height, int dstfmt, int unit)
{
	switch(dstfmt)
	{
	case YUV422_UYVY:
	case YUV422_VYUY:
	case YUV422_YUYV:
	case YUV422_YVYU:
	case YUV422_YUVY:
	case YUV422_YVUY:
	case YUV422_UVYY:
	case YUV422_VUYY:
	case YUV422_YYVU:
		break;
	case RAW_BG:
	case RAW_GB:
	case RAW_RG:
	case RAW_GR:
		break;
	case RGB1:
	case RGB2:
	case RGB4:
	case RGB8:
	case RGB444:
	case RGB666:
	case RGB32:
		break;

	case RGB24:
		CYuv420F::Yyvu2Rgb(src, dest, width, height, unit);
		break;
	case RGB565:
	case RGB555:
		break;
	case YUV422_YYUV:
		break;
	case YUV411_YYYYUV:
	case YUV411_YYYYVU:
	case YUV411_YUYVYY:
	case YUV411_YVYUYY:
	case YUV411_UVYYYY:
	case YUV411_VUYYYY:
	case YUV420_UYVY:
	case YUV420_VYUY:
	case YUV420_YUYV:
	case YUV420_YVYU:
	case YUV420_YUVY:
	case YUV420_YVUY:
	case YUV420_UVYY:
	case YUV420_VUYY:
	case YUV420_YYVU:
		break;
	case YUV420_YYUV:
	case YUV400:
		break;
	case YUV444:
		CYuv420F::Yyvu2Yuv444(src, dest, width, height, unit);
		break;
	case YUV422BLK:
	case YUV411BLK:
		break;
	case YUV420BLK:
		CYuv420F::Yyvu2Blk(src, dest, width, height, unit);
		break;
	case YUV400BLK:
	case YUV444BLK:
	case YUV422LBUF:
	case YUV411LBUF:
		break;
	case YUV420LBUF:
		CYuv420F::Yyvu2Lbuf(src, dest, width, height, unit);
		break;
	case YUV400LBUF:
	case YUV444LBUF:
		break;
	case BMP:
	case JPEG422:
	case JPEG420:
	case JPEG411:
	case JPEG400:
	case JPEG444:
		break;
	case YUV420FRM:
		CYuv420F::Yyvu2Frame(src, dest, width, height, unit);
		break;
	default:
		break;
	}
}


//***************************************************//

//===================================================
//
//			Yyyyuv YUV411 Convert
//
//==============================================

//***************************************************//
void CGeneralTools::Yyyyuv411Convert(char *src, char *dest, int width, int height, int dstfmt, int unit)
{
	switch(dstfmt)
	{
	case YUV422_UYVY:
	case YUV422_VYUY:
	case YUV422_YUYV:
	case YUV422_YVYU:
	case YUV422_YUVY:
	case YUV422_YVUY:
	case YUV422_UVYY:
	case YUV422_VUYY:
	case YUV422_YYVU:
		break;
	case RAW_BG:
	case RAW_GB:
	case RAW_RG:
	case RAW_GR:
		break;
	case RGB1:
	case RGB2:
	case RGB4:
	case RGB8:
	case RGB444:
	case RGB666:
	case RGB32:
		break;

	case RGB24:
		CYuv411F::Yyyyuv2Rgb(src, dest, width, height, unit);
		break;
	case RGB565:
	case RGB555:
		break;
	case YUV422_YYUV:
		break;
	case YUV411_YYYYUV:
	case YUV411_YYYYVU:
	case YUV411_YUYVYY:
	case YUV411_YVYUYY:
	case YUV411_UVYYYY:
	case YUV411_VUYYYY:
	case YUV420_UYVY:
	case YUV420_VYUY:
	case YUV420_YUYV:
	case YUV420_YVYU:
	case YUV420_YUVY:
	case YUV420_YVUY:
	case YUV420_UVYY:
	case YUV420_VUYY:
	case YUV420_YYVU:
	case YUV420_YYUV:
	case YUV400:
		break;
	case YUV444:
		CYuv411F::Yyyyuv_444(src, dest, width, height, unit);
		break;
	case YUV422BLK:
		break;
	case YUV411BLK:
		CYuv411F::Yyyyuv2Blk(src, dest, width, height, unit);
		break;
	case YUV420BLK:
	case YUV400BLK:
	case YUV444BLK:
		break;
	case YUV422LBUF:
		break;
	case YUV411LBUF:
		CYuv411F::Yyyyuv2Lbuf(src, dest, width, height, unit);
		break;
	case YUV420LBUF:
	case YUV400LBUF:
	case YUV444LBUF:
		break;
	case BMP:
	case JPEG422:
	case JPEG420:
	case JPEG411:
	case JPEG400:
	case JPEG444:
	default:
		break;
	}
}

//===================================================
//
//			Yyyyvu YUV411 Convert
//
//==============================================
void CGeneralTools::Yyyyvu411Convert(char *src, char *dest, int width, int height, int dstfmt, int unit)
{
	switch(dstfmt)
	{
	case YUV422_UYVY:
	case YUV422_VYUY:
	case YUV422_YUYV:
	case YUV422_YVYU:
	case YUV422_YUVY:
	case YUV422_YVUY:
	case YUV422_UVYY:
	case YUV422_VUYY:
	case YUV422_YYVU:
		break;
	case RAW_BG:
	case RAW_GB:
	case RAW_RG:
	case RAW_GR:
		break;
	case RGB1:
	case RGB2:
	case RGB4:
	case RGB8:
	case RGB444:
	case RGB666:
	case RGB32:
		break;

	case RGB24:
		CYuv411F::Yyyyvu2Rgb(src, dest, width, height, unit);
		break;
	case RGB565:
	case RGB555:
		break;
	case YUV422_YYUV:
		break;
	case YUV411_YYYYUV:
	case YUV411_YYYYVU:
	case YUV411_YUYVYY:
	case YUV411_YVYUYY:
	case YUV411_UVYYYY:
	case YUV411_VUYYYY:
	case YUV420_UYVY:
	case YUV420_VYUY:
	case YUV420_YUYV:
	case YUV420_YVYU:
	case YUV420_YUVY:
	case YUV420_YVUY:
	case YUV420_UVYY:
	case YUV420_VUYY:
	case YUV420_YYVU:
	case YUV420_YYUV:
	case YUV400:
		break;
	case YUV444:
		CYuv411F::Yyyyvu_444(src, dest, width, height, unit);
		break;
	case YUV422BLK:
		break;
	case YUV411BLK:
		CYuv411F::Yyyyvu2Blk(src, dest, width, height, unit);
		break;
	case YUV420BLK:
	case YUV400BLK:
	case YUV444BLK:
		break;
	case YUV422LBUF:
		break;
	case YUV411LBUF:
		CYuv411F::Yyyyvu2Lbuf(src, dest, width, height, unit);
		break;
	case YUV420LBUF:
	case YUV400LBUF:
	case YUV444LBUF:
		break;
	case BMP:
	case JPEG422:
	case JPEG420:
	case JPEG411:
	case JPEG400:
	case JPEG444:
	default:
		break;
	}
}

//===================================================
//
//			Yuyvyy YUV411 Convert
//
//==============================================
void CGeneralTools::Yuyvyy411Convert(char *src, char *dest, int width, int height, int dstfmt, int unit)
{
	switch(dstfmt)
	{
	case YUV422_UYVY:
	case YUV422_VYUY:
	case YUV422_YUYV:
	case YUV422_YVYU:
	case YUV422_YUVY:
	case YUV422_YVUY:
	case YUV422_UVYY:
	case YUV422_VUYY:
	case YUV422_YYVU:
		break;
	case RAW_BG:
	case RAW_GB:
	case RAW_RG:
	case RAW_GR:
		break;
	case RGB1:
	case RGB2:
	case RGB4:
	case RGB8:
	case RGB444:
	case RGB666:
	case RGB32:
		break;

	case RGB24:
		CYuv411F::Yuyvyy2Rgb(src, dest, width, height, unit);
		break;
	case RGB565:
	case RGB555:
		break;
	case YUV422_YYUV:
		break;
	case YUV411_YYYYUV:
	case YUV411_YYYYVU:
	case YUV411_YUYVYY:
	case YUV411_YVYUYY:
	case YUV411_UVYYYY:
	case YUV411_VUYYYY:
	case YUV420_UYVY:
	case YUV420_VYUY:
	case YUV420_YUYV:
	case YUV420_YVYU:
	case YUV420_YUVY:
	case YUV420_YVUY:
	case YUV420_UVYY:
	case YUV420_VUYY:
	case YUV420_YYVU:
	case YUV420_YYUV:
	case YUV400:
		break;
	case YUV444:
		CYuv411F::Yuyvyy_444(src, dest, width, height, unit);
		break;
	case YUV422BLK:
		break;
	case YUV411BLK:
		CYuv411F::Yuyvyy2Blk(src, dest, width, height, unit);
		break;
	case YUV420BLK:
		break;
	case YUV400BLK:
	case YUV444BLK:
		break;
	case YUV422LBUF:
		break;
	case YUV411LBUF:
		CYuv411F::Yuyvyy2Lbuf(src, dest, width, height, unit);
		break;
	case YUV420LBUF:
	case YUV400LBUF:
	case YUV444LBUF:
		break;
	case BMP:
	case JPEG422:
	case JPEG420:
	case JPEG411:
	case JPEG400:
	case JPEG444:
	default:
		break;
	}
}

//===================================================
//
//			Yvyuyy YUV411 Convert
//
//==============================================
void CGeneralTools::Yvyuyy411Convert(char *src, char *dest, int width, int height, int dstfmt, int unit)
{
	switch(dstfmt)
	{
	case YUV422_UYVY:
	case YUV422_VYUY:
	case YUV422_YUYV:
	case YUV422_YVYU:
	case YUV422_YUVY:
	case YUV422_YVUY:
	case YUV422_UVYY:
	case YUV422_VUYY:
	case YUV422_YYVU:
		break;
	case RAW_BG:
	case RAW_GB:
	case RAW_RG:
	case RAW_GR:
		break;
	case RGB1:
	case RGB2:
	case RGB4:
	case RGB8:
	case RGB444:
	case RGB666:
	case RGB32:
		break;

	case RGB24:
		CYuv411F::Yvyuyy2Rgb(src, dest, width, height, unit);
		break;
	case RGB565:
	case RGB555:
		break;
	case YUV422_YYUV:
		break;
	case YUV411_YYYYUV:
	case YUV411_YYYYVU:
	case YUV411_YUYVYY:
	case YUV411_YVYUYY:
	case YUV411_UVYYYY:
	case YUV411_VUYYYY:
	case YUV420_UYVY:
	case YUV420_VYUY:
	case YUV420_YUYV:
	case YUV420_YVYU:
	case YUV420_YUVY:
	case YUV420_YVUY:
	case YUV420_UVYY:
	case YUV420_VUYY:
	case YUV420_YYVU:
		break;
	case YUV420_YYUV:
	case YUV400:
		break;
	case YUV444:
		CYuv411F::Yvyuyy_444(src, dest, width, height, unit);
		break;
	case YUV422BLK:
		break;
	case YUV411BLK:
		CYuv411F::Yvyuyy2Blk(src, dest, width, height, unit);
		break;
	case YUV420BLK:
		break;
	case YUV400BLK:
	case YUV444BLK:
		break;
	case YUV422LBUF:
		break;
	case YUV411LBUF:
		CYuv411F::Yvyuyy2Lbuf(src, dest, width, height, unit);
		break;
	case YUV420LBUF:
	case YUV400LBUF:
	case YUV444LBUF:
		break;
	case BMP:
	case JPEG422:
	case JPEG420:
	case JPEG411:
	case JPEG400:
	case JPEG444:
	default:
		break;
	}
}

//===================================================
//
//			Uvyyyy YUV411 Convert
//
//==============================================
void CGeneralTools::Uvyyyy411Convert(char *src, char *dest, int width, int height, int dstfmt, int unit)
{
	switch(dstfmt)
	{
	case YUV422_UYVY:
	case YUV422_VYUY:
	case YUV422_YUYV:
	case YUV422_YVYU:
	case YUV422_YUVY:
	case YUV422_YVUY:
	case YUV422_UVYY:
	case YUV422_VUYY:
	case YUV422_YYVU:
		break;
	case RAW_BG:
	case RAW_GB:
	case RAW_RG:
	case RAW_GR:
		break;
	case RGB1:
	case RGB2:
	case RGB4:
	case RGB8:
	case RGB444:
	case RGB666:
	case RGB32:
		break;

	case RGB24:
		CYuv411F::Uvyyyy2Rgb(src, dest, width, height, unit);
		break;
	case RGB565:
	case RGB555:
		break;
	case YUV422_YYUV:
		break;
	case YUV411_YYYYUV:
	case YUV411_YYYYVU:
	case YUV411_YUYVYY:
	case YUV411_YVYUYY:
	case YUV411_UVYYYY:
	case YUV411_VUYYYY:
	case YUV420_UYVY:
	case YUV420_VYUY:
	case YUV420_YUYV:
	case YUV420_YVYU:
	case YUV420_YUVY:
	case YUV420_YVUY:
	case YUV420_UVYY:
	case YUV420_VUYY:
	case YUV420_YYVU:
		break;
	case YUV420_YYUV:
	case YUV400:
		break;
	case YUV444:
		CYuv411F::Uvyyyy_444(src, dest, width, height, unit);
		break;
	case YUV422BLK:
		break;
	case YUV411BLK:
		CYuv411F::Uvyyyy2Blk(src, dest, width, height, unit);
		break;
	case YUV420BLK:
		break;
	case YUV400BLK:
	case YUV444BLK:
		break;
	case YUV422LBUF:
		break;
	case YUV411LBUF:
		CYuv411F::Uvyyyy2Lbuf(src, dest, width, height, unit);
		break;
	case YUV420LBUF:
	case YUV400LBUF:
	case YUV444LBUF:
		break;
	case BMP:
	case JPEG422:
	case JPEG420:
	case JPEG411:
	case JPEG400:
	case JPEG444:
	default:
		break;
	}
}

//===================================================
//
//			Vuyyyy YUV411 Convert
//
//==============================================
void CGeneralTools::Vuyyyy411Convert(char *src, char *dest, int width, int height, int dstfmt, int unit)
{
	switch(dstfmt)
	{
	case YUV422_UYVY:
	case YUV422_VYUY:
	case YUV422_YUYV:
	case YUV422_YVYU:
	case YUV422_YUVY:
	case YUV422_YVUY:
	case YUV422_UVYY:
	case YUV422_VUYY:
	case YUV422_YYVU:
		break;
	case RAW_BG:
	case RAW_GB:
	case RAW_RG:
	case RAW_GR:
		break;
	case RGB1:
	case RGB2:
	case RGB4:
	case RGB8:
	case RGB444:
	case RGB666:
	case RGB32:
		break;

	case RGB24:
		CYuv411F::Vuyyyy2Rgb(src, dest, width, height, unit);
		break;
	case RGB565:
	case RGB555:
		break;
	case YUV422_YYUV:
		break;
	case YUV411_YYYYUV:
	case YUV411_YYYYVU:
	case YUV411_YUYVYY:
	case YUV411_YVYUYY:
	case YUV411_UVYYYY:
	case YUV411_VUYYYY:
	case YUV420_UYVY:
	case YUV420_VYUY:
	case YUV420_YUYV:
	case YUV420_YVYU:
	case YUV420_YUVY:
	case YUV420_YVUY:
	case YUV420_UVYY:
	case YUV420_VUYY:
	case YUV420_YYVU:
		break;
	case YUV420_YYUV:
	case YUV400:
		break;
	case YUV444:
		CYuv411F::Vuyyyy_444(src, dest, width, height, unit);
		break;
	case YUV422BLK:
		break;
	case YUV411BLK:
		CYuv411F::Vuyyyy2Blk(src, dest, width, height, unit);
		break;
	case YUV420BLK:
		break;
	case YUV400BLK:
	case YUV444BLK:
		break;
	case YUV422LBUF:
		break;
	case YUV411LBUF:
		CYuv411F::Vuyyyy2Lbuf(src, dest, width, height, unit);
		break;
	case YUV420LBUF:
	case YUV400LBUF:
	case YUV444LBUF:
		break;
	case BMP:
	case JPEG422:
	case JPEG420:
	case JPEG411:
	case JPEG400:
	case JPEG444:
	default:
		break;
	}
}



//***************************************************//

//===================================================
//
//			YUV400 Convert
//
//==============================================

//***************************************************//
void CGeneralTools::Yuv400Convert(char *src, char *dest, int width, int height, int dstfmt, int unit)
{
	switch(dstfmt)
	{
	case YUV422_UYVY:
	case YUV422_VYUY:
	case YUV422_YUYV:
	case YUV422_YVYU:
	case YUV422_YUVY:
	case YUV422_YVUY:
	case YUV422_UVYY:
	case YUV422_VUYY:
	case YUV422_YYVU:
		break;
	case RAW_BG:
	case RAW_GB:
	case RAW_RG:
	case RAW_GR:
		break;
	case RGB1:
	case RGB2:
	case RGB4:
	case RGB8:
	case RGB444:
	case RGB666:
	case RGB32:
		break;

	case RGB24:
		CYuv400F::Yuv400_Rgb(src, dest, width, height, unit);
		break;
	case RGB565:
	case RGB555:
		break;
	case YUV422_YYUV:
		break;
	case YUV411_YYYYUV:
	case YUV411_YYYYVU:
	case YUV411_YUYVYY:
	case YUV411_YVYUYY:
	case YUV411_UVYYYY:
	case YUV411_VUYYYY:
	case YUV420_UYVY:
	case YUV420_VYUY:
	case YUV420_YUYV:
	case YUV420_YVYU:
	case YUV420_YUVY:
	case YUV420_YVUY:
	case YUV420_UVYY:
	case YUV420_VUYY:
	case YUV420_YYVU:
		break;
	case YUV420_YYUV:
	case YUV400:
	case YUV444:
		CYuv400F::Yuv400_444(src, dest, width, height, unit);
		break;
	case YUV422BLK:
	case YUV411BLK:
	case YUV420BLK:
		break;
	case YUV400BLK:
		CYuv400F::Yuv2Blk(src, dest, width, height, unit);
		break;
	case YUV444BLK:
	case YUV422LBUF:
	case YUV411LBUF:
	case YUV420LBUF:
		break;
	case YUV400LBUF:
		CYuv400F::Yuv2Lbuf(src, dest, width, height, unit);
		break;
	case YUV444LBUF:
		break;
	case BMP:
	case JPEG422:
	case JPEG420:
	case JPEG411:
	case JPEG400:
	case JPEG444:
	default:
		break;
	}
}

//***************************************************//

//===================================================
//
//			Yuv444 Convert
//
//==============================================

//***************************************************//
void CGeneralTools::Yuv444Convert(char *src, char *dest, int width, int height, int dstfmt, int unit)
{
	switch(dstfmt)
	{
	case YUV422_UYVY:
		CYuv422F::Yuv444_Uyvy(src, dest, width, height, unit);
		break;
	case YUV422_VYUY:
		CYuv422F::Yuv444_Vyuy(src, dest, width, height, unit);
		break;
	case YUV422_YUYV:
		CYuv422F::Yuv444_Yuyv(src, dest, width, height, unit);
		break;
	case YUV422_YVYU:
		CYuv422F::Yuv444_Yvyu(src, dest, width, height, unit);
		break;
	case YUV422_YUVY:
		CYuv422F::Yuv444_Yuvy(src, dest, width, height, unit);
		break;
	case YUV422_YVUY:
		CYuv422F::Yuv444_Yvuy(src, dest, width, height, unit);
		break;
	case YUV422_UVYY:
		CYuv422F::Yuv444_Uvyy(src, dest, width, height, unit);
		break;
	case YUV422_VUYY:
		CYuv422F::Yuv444_Vuyy(src, dest, width, height, unit);
		break;
	case YUV422_YYVU:
		CYuv422F::Yuv444_Yyvu(src, dest, width, height, unit);
		break;
	case RAW_BG:
	case RAW_GB:
	case RAW_RG:
	case RAW_GR:
		break;
	case RGB1:
	case RGB2:
	case RGB4:
	case RGB8:
	case RGB444:
	case RGB666:
	case RGB32:
		break;

	case RGB24:
		CYuv444F::Yuv444_Rgb(src, dest, width, height, unit);
		break;
	case RGB565:
	case RGB555:
		break;
	case YUV422_YYUV:
		break;
	case YUV411_YYYYUV:
		CYuv411F::Yuv444_Yyyyuv(src, dest, width, height, unit);
		break;
	case YUV411_YYYYVU:
		CYuv411F::Yuv444_Yyyyvu(src, dest, width, height, unit);
		break;
	case YUV411_YUYVYY:
		CYuv411F::Yuv444_Yuyvyy(src, dest, width, height, unit);
		break;
	case YUV411_YVYUYY:
		CYuv411F::Yuv444_Yvyuyy(src, dest, width, height, unit);
		break;
	case YUV411_UVYYYY:
		CYuv411F::Yuv444_Uvyyyy(src, dest, width, height, unit);
		break;
	case YUV411_VUYYYY:
		CYuv411F::Yuv444_Vuyyyy(src, dest, width, height, unit);
		break;
	case YUV420_UYVY:
		CYuv420F::Yuv444_Uyvy(src, dest, width, height, unit);
		break;
	case YUV420_VYUY:
		CYuv420F::Yuv444_Vyuy(src, dest, width, height, unit);
		break;
	case YUV420_YUYV:
		CYuv420F::Yuv444_Yuyv(src, dest, width, height, unit);
		break;
	case YUV420_YVYU:
		CYuv420F::Yuv444_Yvyu(src, dest, width, height, unit);
		break;
	case YUV420_YUVY:
		CYuv420F::Yuv444_Yuvy(src, dest, width, height, unit);
		break;
	case YUV420_YVUY:
		CYuv420F::Yuv444_Yvuy(src, dest, width, height, unit);
		break;
	case YUV420_UVYY:
		CYuv420F::Yuv444_Uvyy(src, dest, width, height, unit);
		break;
	case YUV420_VUYY:
		CYuv420F::Yuv444_Vuyy(src, dest, width, height, unit);
		break;
	case YUV420_YYVU:
		CYuv420F::Yuv444_Yyvu(src, dest, width, height, unit);
		break;
	case YUV420_YYUV:
		CYuv420F::Yuv444_Yyuv(src, dest, width, height, unit);
		break;
	case YUV400:
		CYuv400F::Yuv444_400(src, dest, width, height, unit);
		break;
	case YUV444:
		break;
	case YUV422BLK:
	case YUV411BLK:
	case YUV420BLK:
	case YUV400BLK:
		break;
	case YUV444BLK:
		CYuv444F::Yuv444_Blk(src, dest, width, height, unit);
		break;
	case YUV422LBUF:
	case YUV411LBUF:
	case YUV420LBUF:
	case YUV400LBUF:
		break;
	case YUV444LBUF:
		CYuv444F::Yuv444_Lbuf(src, dest, width, height, unit);
		break;
	case BMP:
	case JPEG422:
	case JPEG420:
	case JPEG411:
	case JPEG400:
	case JPEG444:
	default:
		break;
	}
}

//***************************************************//

//===================================================
//
//			Bayer GB Convert
//
//==============================================

//***************************************************//
void CGeneralTools::BayerGBConvert(char *src, char *dest, int width, int height, int dstfmt, int unit)
{
	switch(dstfmt)
	{
	case RGB24:
		CRaw2RgbF::GB2Rgb(src, dest, width, height, unit);
		break;
	default:
		break;
	}
}

void CGeneralTools::BayerBGConvert(char *src, char *dest, int width, int height, int dstfmt, int unit)
{
	switch(dstfmt)
	{
	case RGB24:
		CRaw2RgbF::BG2Rgb(src, dest, width, height, unit);
		break;
	default:
		break;
	}
}

void CGeneralTools::BayerGRConvert(char *src, char *dest, int width, int height, int dstfmt, int unit)
{
	switch(dstfmt)
	{
	case RGB24:
		CRaw2RgbF::GR2Rgb(src, dest, width, height, unit);
		break;
	default:
		break;
	}
}

void CGeneralTools::BayerRGConvert(char *src, char *dest, int width, int height, int dstfmt, int unit)
{
	switch(dstfmt)
	{
	case RGB24:
		CRaw2RgbF::RG2Rgb(src, dest, width, height, unit);
		break;
	default:
		break;
	}
}

//***************************************************//

//===================================================
//
//			Yuv422 Frame Convert
//
//==============================================

//***************************************************//

void CGeneralTools::Yuv422FrmConvert(char *src, char *dest, int width, int height, int dstfmt, int unit)
{
	switch(dstfmt)
	{
	case YUV422_UYVY:
		CYuv422F::Frame2Uyvy(src, dest, width, height, unit);
		break;
	case YUV422_VYUY:
		CYuv422F::Frame2Vyuy(src, dest, width, height, unit);
		break;
	case YUV422_YUYV:
		CYuv422F::Frame2Yuyv(src, dest, width, height, unit);
		break;
	case YUV422_YVYU:
		CYuv422F::Frame2Yvyu(src, dest, width, height, unit);
		break;
	case YUV422_YUVY:
		CYuv422F::Frame2Yuvy(src, dest, width, height, unit);
		break;
	case YUV422_YVUY:
		CYuv422F::Frame2Yvuy(src, dest, width, height, unit);
		break;
	case YUV422_UVYY:
		CYuv422F::Frame2Uvyy(src, dest, width, height, unit);
		break;
	case YUV422_VUYY:
		CYuv422F::Frame2Vuyy(src, dest, width, height, unit);
		break;
	case YUV422_YYVU:
		CYuv422F::Frame2Yyvu(src, dest, width, height, unit);
		break;
	case YUV422_YYUV:
		CYuv422F::Frame2Yyuv(src, dest, width, height, unit);
		break;
	case RAW_BG:
	case RAW_GB:
	case RAW_RG:
	case RAW_GR:
		break;
	case RGB1:
	case RGB2:
	case RGB4:
	case RGB8:
	case RGB444:
	case RGB666:
	case RGB32:
		break;

	case RGB24:
		break;
	case RGB565:
	case RGB555:
		break;
	case YUV411_YYYYUV:
	case YUV411_YYYYVU:
	case YUV411_YUYVYY:
	case YUV411_YVYUYY:
	case YUV411_UVYYYY:
	case YUV411_VUYYYY:
	case YUV420_UYVY:
	case YUV420_VYUY:
	case YUV420_YUYV:
	case YUV420_YVYU:
	case YUV420_YUVY:
	case YUV420_YVUY:
	case YUV420_UVYY:
	case YUV420_VUYY:
	case YUV420_YYVU:
		break;

	case YUV420_YYUV:
		break;
	case YUV400:
		break;
	case YUV444:
		break;
	case YUV422BLK:
	case YUV411BLK:
	case YUV420BLK:
	case YUV400BLK:
	case YUV444BLK:
	case BMP:
		break;
	case JPEG422:
		break;
	case JPEG420:
	case JPEG411:
	case JPEG400:
	case JPEG444:
	default:
		break;
	}
}

//***************************************************//

//===================================================
//
//			Yuv422 Frame Convert
//
//==============================================

//***************************************************//

void CGeneralTools::Yuv420FrmConvert(char *src, char *dest, int width, int height, int dstfmt, int unit)
{
	switch(dstfmt)
	{
	case YUV420_UYVY:
		CYuv420F::Frame2Uyvy(src, dest, width, height, unit);
		break;
	case YUV420_VYUY:
		CYuv420F::Frame2Vyuy(src, dest, width, height, unit);
		break;
	case YUV420_YUYV:
		CYuv420F::Frame2Yuyv(src, dest, width, height, unit);
		break;
	case YUV420_YVYU:
		CYuv420F::Frame2Yvyu(src, dest, width, height, unit);
		break;
	case YUV420_YUVY:
		CYuv420F::Frame2Yuvy(src, dest, width, height, unit);
		break;
	case YUV420_YVUY:
		CYuv420F::Frame2Yvuy(src, dest, width, height, unit);
		break;
	case YUV420_UVYY:
		CYuv420F::Frame2Uvyy(src, dest, width, height, unit);
		break;
	case YUV420_VUYY:
		CYuv420F::Frame2Vuyy(src, dest, width, height, unit);
		break;
	case YUV420_YYVU:
		CYuv420F::Frame2Yyvu(src, dest, width, height, unit);
		break;
	case YUV420_YYUV:
		CYuv420F::Frame2Yyuv(src, dest, width, height, unit);
		break;
	default:
		break;
	}
}

//***************************************************//

//===================================================
//
//			Yuv422 Block Convert
//
//==============================================

//***************************************************//
void CGeneralTools::Yuv422BlkConvert(char *src, char *dest, int width, int height, int dstfmt, int unit, int qf)
{
	switch(dstfmt)
	{
	case YUV422_UYVY:
		CYuv422F::Blk2Uyvy(src, dest, width, height, unit);
		break;
	case YUV422_VYUY:
		CYuv422F::Blk2Vyuy(src, dest, width, height, unit);
		break;
	case YUV422_YUYV:
		CYuv422F::Blk2Yuyv(src, dest, width, height, unit);
		break;
	case YUV422_YVYU:
		CYuv422F::Blk2Yvyu(src, dest, width, height, unit);
		break;
	case YUV422_YUVY:
		CYuv422F::Blk2Yuvy(src, dest, width, height, unit);
		break;
	case YUV422_YVUY:
		CYuv422F::Blk2Yvuy(src, dest, width, height, unit);
		break;
	case YUV422_UVYY:
		CYuv422F::Blk2Uvyy(src, dest, width, height, unit);
		break;
	case YUV422_VUYY:
		CYuv422F::Blk2Vuyy(src, dest, width, height, unit);
		break;
	case YUV422_YYVU:
		CYuv422F::Blk2Yyvu(src, dest, width, height, unit);
		break;
	case YUV422_YYUV:
		CYuv422F::Blk2Yyuv(src, dest, width, height, unit);
		break;
	case RAW_BG:
	case RAW_GB:
	case RAW_RG:
	case RAW_GR:
		break;
	case RGB1:
	case RGB2:
	case RGB4:
	case RGB8:
	case RGB444:
	case RGB666:
	case RGB32:
		break;
	case RGB24:
		CYuv422F::Blk2Rgb24(src, dest, width, height);
		break;
	case RGB565:
	case RGB555:
		break;
	case YUV411_YYYYUV:
	case YUV411_YYYYVU:
	case YUV411_YUYVYY:
	case YUV411_YVYUYY:
	case YUV411_UVYYYY:
	case YUV411_VUYYYY:
	case YUV420_UYVY:
	case YUV420_VYUY:
	case YUV420_YUYV:
	case YUV420_YVYU:
	case YUV420_YUVY:
	case YUV420_YVUY:
	case YUV420_UVYY:
	case YUV420_VUYY:
	case YUV420_YYVU:
		break;

	case YUV420_YYUV:
		break;
	case YUV400:
		break;
	case YUV444:
		break;
	case YUV422BLK:
	case YUV411BLK:
	case YUV420BLK:
	case YUV400BLK:
	case YUV444BLK:
		break;
	case BMP:
		this->Blk2Bmp(src, dest, width, height, YUV422BLK);
		break;
	case JPEG422:
		CYuv422F::Blk2Jpeg(src, dest, width, height, qf);
		break;
	case JPEG420:
	case JPEG411:
	case JPEG400:
	case JPEG444:
	default:
		break;
	}
}

//===================================================
//
//			Yuv400 Block Convert
//
//==============================================

void CGeneralTools::Yuv400BlkConvert(char *src, char *dest, int width, int height, int dstfmt, int unit)
{
	switch(dstfmt)
	{
	case YUV422_UYVY:
	case YUV422_VYUY:
	case YUV422_YUYV:
	case YUV422_YVYU:
	case YUV422_YUVY:
	case YUV422_YVUY:
	case YUV422_UVYY:
	case YUV422_VUYY:
	case YUV422_YYVU:
	case YUV422_YYUV:
	case RAW_BG:
	case RAW_GB:
	case RAW_RG:
	case RAW_GR:
		break;
	case RGB1:
	case RGB2:
	case RGB4:
	case RGB8:
	case RGB444:
	case RGB666:
	case RGB32:
		break;
	case RGB24:
		CYuv400F::Blk2Rgb24(src, dest, width, height);
		break;
	case RGB565:
	case RGB555:
		break;
	case YUV411_YYYYUV:
	case YUV411_YYYYVU:
	case YUV411_YUYVYY:
	case YUV411_YVYUYY:
	case YUV411_UVYYYY:
	case YUV411_VUYYYY:
	case YUV420_UYVY:
	case YUV420_VYUY:
	case YUV420_YUYV:
	case YUV420_YVYU:
	case YUV420_YUVY:
	case YUV420_YVUY:
	case YUV420_UVYY:
	case YUV420_VUYY:
	case YUV420_YYVU:
		break;

	case YUV420_YYUV:
		break;
	case YUV400:
		CYuv400F::Blk2Yuv(src, dest, width, height, unit);
		break;
	case YUV444:
	case YUV422BLK:
	case YUV411BLK:
	case YUV420BLK:
	case YUV400BLK:
	case YUV444BLK:
		break;
	case BMP:
		this->Blk2Bmp(src, dest, width, height, YUV400BLK);		
		break;
	case JPEG422:
	case JPEG420:
	case JPEG411:
		break;
	case JPEG400:
		CYuv400F::Blk2Jpeg(src, dest, width, height);
		break;
	case JPEG444:
	default:
		break;
	}
}

//===================================================
//
//			Yuv411 Block Convert
//
//==============================================

void CGeneralTools::Yuv411BlkConvert(char *src, char *dest, int width, int height, int dstfmt, int unit)
{
	switch(dstfmt)
	{
	case YUV422_UYVY:
	case YUV422_VYUY:
	case YUV422_YUYV:
	case YUV422_YVYU:
	case YUV422_YUVY:
	case YUV422_YVUY:
	case YUV422_UVYY:
	case YUV422_VUYY:
	case YUV422_YYVU:
	case YUV422_YYUV:
	case RAW_BG:
	case RAW_GB:
	case RAW_RG:
	case RAW_GR:
		break;
	case RGB1:
	case RGB2:
	case RGB4:
	case RGB8:
	case RGB444:
	case RGB666:
	case RGB32:
		break;
	case RGB24:
		CYuv411F::Blk2Rgb24(src, dest, width, height);
		break;
	case RGB565:
	case RGB555:
		break;
	case YUV411_YYYYUV:
		CYuv411F::Blk2Yyyyuv(src, dest, width, height, unit);
		break;
	case YUV411_YYYYVU:
		CYuv411F::Blk2Yyyyvu(src, dest, width, height, unit);
		break;
	case YUV411_YUYVYY:
		CYuv411F::Blk2Yuyvyy(src, dest, width, height, unit);
		break;
	case YUV411_YVYUYY:
		CYuv411F::Blk2Yvyuyy(src, dest, width, height, unit);
		break;
	case YUV411_UVYYYY:
		CYuv411F::Blk2Uvyyyy(src, dest, width, height, unit);
		break;
	case YUV411_VUYYYY:
		CYuv411F::Blk2Vuyyyy(src, dest, width, height, unit);
		break;
	case YUV420_UYVY:
	case YUV420_VYUY:
	case YUV420_YUYV:
	case YUV420_YVYU:
	case YUV420_YUVY:
	case YUV420_YVUY:
	case YUV420_UVYY:
	case YUV420_VUYY:
	case YUV420_YYVU:
		break;

	case YUV420_YYUV:
	case YUV400:
	case YUV444:
	case YUV422BLK:
	case YUV411BLK:
	case YUV420BLK:
	case YUV400BLK:
	case YUV444BLK:
		break;
	case BMP:
		this->Blk2Bmp(src, dest, width, height, YUV411BLK);
		break;
	case JPEG422:
	case JPEG420:
		break;
	case JPEG411:
		CYuv411F::Blk2Jpeg(src, dest, width, height);
		break;
	case JPEG400:
	case JPEG444:
	default:
		break;
	}
}

//===================================================
//
//			Yuv420 Block Convert
//
//==============================================

void CGeneralTools::Yuv420BlkConvert(char *src, char *dest, int width, int height, int dstfmt, int unit)
{
	switch(dstfmt)
	{
	case YUV422_UYVY:
	case YUV422_VYUY:
	case YUV422_YUYV:
	case YUV422_YVYU:
	case YUV422_YUVY:
	case YUV422_YVUY:
	case YUV422_UVYY:
	case YUV422_VUYY:
	case YUV422_YYVU:
	case YUV422_YYUV:
	case RAW_BG:
	case RAW_GB:
	case RAW_RG:
	case RAW_GR:
		break;
	case RGB1:
	case RGB2:
	case RGB4:
	case RGB8:
	case RGB444:
	case RGB666:
	case RGB32:
		break;
	case RGB24:
		CYuv420F::Blk2Rgb24(src, dest, width, height);
		break;
	case RGB565:
	case RGB555:
		break;
	case YUV411_YYYYUV:
	case YUV411_YYYYVU:
	case YUV411_YUYVYY:
	case YUV411_YVYUYY:
	case YUV411_UVYYYY:
	case YUV411_VUYYYY:
		break;
	case YUV420_UYVY:
		CYuv420F::Blk2Uyvy(src, dest, width, height, unit);
		break;
	case YUV420_VYUY:
		CYuv420F::Blk2Vyuy(src, dest, width, height, unit);
		break;
	case YUV420_YUYV:
		CYuv420F::Blk2Yuyv(src, dest, width, height, unit);
		break;
	case YUV420_YVYU:
		CYuv420F::Blk2Yvyu(src, dest, width, height, unit);
		break;
	case YUV420_YUVY:
		CYuv420F::Blk2Yuvy(src, dest, width, height, unit);
		break;
	case YUV420_YVUY:
		CYuv420F::Blk2Yvuy(src, dest, width, height, unit);
		break;
	case YUV420_UVYY:
		CYuv420F::Blk2Uvyy(src, dest, width, height, unit);
		break;
	case YUV420_VUYY:
		CYuv420F::Blk2Vuyy(src, dest, width, height, unit);
		break;
	case YUV420_YYVU:
		CYuv420F::Blk2Yyvu(src, dest, width, height, unit);
		break;
	case YUV420_YYUV:
		CYuv420F::Blk2Yyuv(src, dest, width, height, unit);
		break;
	case YUV400:
		break;
	case YUV444:
		break;
	case YUV422BLK:
	case YUV411BLK:
	case YUV420BLK:
	case YUV400BLK:
	case YUV444BLK:
		break;
	case BMP:
		this->Blk2Bmp(src, dest, width, height, YUV420BLK);
		break;
	case JPEG422:
		break;
	case JPEG420:
		CYuv420F::Blk2Jpeg(src, dest, width, height);
		break;
	case MPEG2IFRM420:
		CYuv420F::Blk2Mpeg2I(src, dest, width, height);
		break;
	case MPEG2IFRM422:
	case MPEG2IFRM444:
	case JPEG411:
	case JPEG400:
	case JPEG444:
	default:
		break;
	}
}

//===================================================
//
//			Yuv444 Block Convert
//
//==============================================

void CGeneralTools::Yuv444BlkConvert(char *src, char *dest, int width, int height, int dstfmt, int unit)
{
	switch(dstfmt)
	{
	case YUV422_UYVY:
	case YUV422_VYUY:
	case YUV422_YUYV:
	case YUV422_YVYU:
	case YUV422_YUVY:
	case YUV422_YVUY:
	case YUV422_UVYY:
	case YUV422_VUYY:
	case YUV422_YYVU:
	case YUV422_YYUV:
	case RAW_BG:
	case RAW_GB:
	case RAW_RG:
	case RAW_GR:
		break;
	case RGB1:
	case RGB2:
	case RGB4:
	case RGB8:
	case RGB444:
	case RGB666:
	case RGB32:
		break;
	case RGB24:
		CYuv444F::Blk2Rgb24(src, dest, width, height);
		break;
	case RGB565:
	case RGB555:
		break;
	case YUV411_YYYYUV:
	case YUV411_YYYYVU:
	case YUV411_YUYVYY:
	case YUV411_YVYUYY:
	case YUV411_UVYYYY:
	case YUV411_VUYYYY:
		break;
	case YUV420_UYVY:
	case YUV420_VYUY:
	case YUV420_YUYV:
	case YUV420_YVYU:
	case YUV420_YUVY:
	case YUV420_YVUY:
	case YUV420_UVYY:
	case YUV420_VUYY:
	case YUV420_YYVU:
	case YUV420_YYUV:
	case YUV400:
		break;
	case YUV444:
		CYuv444F::Blk2Yuv444(src, dest, width, height, unit);
		break;
	case YUV422BLK:
	case YUV411BLK:
	case YUV420BLK:
	case YUV400BLK:
	case YUV444BLK:
		break;
	case BMP:
		this->Blk2Bmp(src, dest, width, height, YUV444BLK);		
		break;
	case JPEG422:
	case JPEG420:
	case JPEG411:
	case JPEG400:
		break;
	case JPEG444:
		CYuv444F::Blk2Jpeg(src, dest, width, height);
	default:
		break;
	}
}

//***************************************************//

//===================================================
//
//			Yuv422 LBUF Convert
//
//==============================================

//***************************************************//
void CGeneralTools::Yuv422LbufConvert(char *src, char *dest, int width, int height, int dstfmt, int unit)
{
	switch(dstfmt)
	{
	case YUV422_UYVY:
		CYuv422F::Lbuf2Uyvy(src, dest, width, height, unit);
		break;
	case YUV422_VYUY:
		CYuv422F::Lbuf2Vyuy(src, dest, width, height, unit);
		break;
	case YUV422_YUYV:
		CYuv422F::Lbuf2Yuyv(src, dest, width, height, unit);
		break;
	case YUV422_YVYU:
		CYuv422F::Lbuf2Yvyu(src, dest, width, height, unit);
		break;
	case YUV422_YUVY:
		CYuv422F::Lbuf2Yuvy(src, dest, width, height, unit);
		break;
	case YUV422_YVUY:
		CYuv422F::Lbuf2Yvuy(src, dest, width, height, unit);
		break;
	case YUV422_UVYY:
		CYuv422F::Lbuf2Uvyy(src, dest, width, height, unit);
		break;
	case YUV422_VUYY:
		CYuv422F::Lbuf2Vuyy(src, dest, width, height, unit);
		break;
	case YUV422_YYVU:
		CYuv422F::Lbuf2Yyvu(src, dest, width, height, unit);
		break;
	case YUV422_YYUV:
		CYuv422F::Lbuf2Yyuv(src, dest, width, height, unit);
		break;
	case RAW_BG:
	case RAW_GB:
	case RAW_RG:
	case RAW_GR:
		break;
	case RGB1:
	case RGB2:
	case RGB4:
	case RGB8:
	case RGB444:
	case RGB666:
	case RGB32:
		break;

	case RGB24:
		break;
	case RGB565:
	case RGB555:
		break;
	case YUV411_YYYYUV:
	case YUV411_YYYYVU:
	case YUV411_YUYVYY:
	case YUV411_YVYUYY:
	case YUV411_UVYYYY:
	case YUV411_VUYYYY:
	case YUV420_UYVY:
	case YUV420_VYUY:
	case YUV420_YUYV:
	case YUV420_YVYU:
	case YUV420_YUVY:
	case YUV420_YVUY:
	case YUV420_UVYY:
	case YUV420_VUYY:
	case YUV420_YYVU:
		break;

	case YUV420_YYUV:
		break;
	case YUV400:
		break;
	case YUV444:
		break;
	case YUV422BLK:
	case YUV411BLK:
	case YUV420BLK:
	case YUV400BLK:
	case YUV444BLK:
	case BMP:
	case JPEG422:
	case JPEG420:
	case JPEG411:
	case JPEG400:
	case JPEG444:
	default:
		break;
	}
}

//***************************************************//
void CGeneralTools::Yuv444422LbufConvert(char *src, char *dest, int width, int height, int dstfmt, int unit)
{
	switch(dstfmt)
	{
	case YUV422_UYVY:
		CYuv422F::Lbuf444422_2Uyvy(src, dest, width, height, unit);
		break;
	case YUV422_VYUY:
		CYuv422F::Lbuf444422_2Vyuy(src, dest, width, height, unit);
		break;
	case YUV422_YUYV:
		CYuv422F::Lbuf444422_2Yuyv(src, dest, width, height, unit);
		break;
	case YUV422_YVYU:
		CYuv422F::Lbuf444422_2Yvyu(src, dest, width, height, unit);
		break;
	case YUV422_YUVY:
		CYuv422F::Lbuf444422_2Yuvy(src, dest, width, height, unit);
		break;
	case YUV422_YVUY:
		CYuv422F::Lbuf444422_2Yvuy(src, dest, width, height, unit);
		break;
	case YUV422_UVYY:
		CYuv422F::Lbuf444422_2Uvyy(src, dest, width, height, unit);
		break;
	case YUV422_VUYY:
		CYuv422F::Lbuf444422_2Vuyy(src, dest, width, height, unit);
		break;
	case YUV422_YYVU:
		CYuv422F::Lbuf444422_2Yyvu(src, dest, width, height, unit);
		break;
	case YUV422_YYUV:
		CYuv422F::Lbuf444422_2Yyuv(src, dest, width, height, unit);
		break;
	case RAW_BG:
	case RAW_GB:
	case RAW_RG:
	case RAW_GR:
		break;
	case RGB1:
	case RGB2:
	case RGB4:
	case RGB8:
	case RGB444:
	case RGB666:
	case RGB32:
		break;

	case RGB24:
		break;
	case RGB565:
	case RGB555:
		break;
	case YUV411_YYYYUV:
	case YUV411_YYYYVU:
	case YUV411_YUYVYY:
	case YUV411_YVYUYY:
	case YUV411_UVYYYY:
	case YUV411_VUYYYY:
	case YUV420_UYVY:
	case YUV420_VYUY:
	case YUV420_YUYV:
	case YUV420_YVYU:
	case YUV420_YUVY:
	case YUV420_YVUY:
	case YUV420_UVYY:
	case YUV420_VUYY:
	case YUV420_YYVU:
		break;

	case YUV420_YYUV:
		break;
	case YUV400:
		break;
	case YUV444:
		break;
	case YUV422BLK:
	case YUV411BLK:
	case YUV420BLK:
	case YUV400BLK:
	case YUV444BLK:
	case BMP:
	case JPEG422:
	case JPEG420:
	case JPEG411:
	case JPEG400:
	case JPEG444:
	default:
		break;
	}
}

//===================================================
//
//			Yuv411 LBUF Convert
//
//==============================================
void CGeneralTools::Yuv411LbufConvert(char *src, char *dest, int width, int height, int dstfmt, int unit)
{
	switch(dstfmt)
	{
	case YUV422_UYVY:
	case YUV422_VYUY:
	case YUV422_YUYV:
	case YUV422_YVYU:
	case YUV422_YUVY:
	case YUV422_YVUY:
	case YUV422_UVYY:
	case YUV422_VUYY:
	case YUV422_YYVU:
	case YUV422_YYUV:
		break;
	case RAW_BG:
	case RAW_GB:
	case RAW_RG:
	case RAW_GR:
		break;
	case RGB1:
	case RGB2:
	case RGB4:
	case RGB8:
	case RGB444:
	case RGB666:
	case RGB32:
		break;

	case RGB24:
		break;
	case RGB565:
	case RGB555:
		break;
	case YUV411_YYYYUV:
		CYuv411F::Lbuf2Yyyyuv(src, dest, width, height, unit);
		break;
	case YUV411_YYYYVU:
		CYuv411F::Lbuf2Yyyyvu(src, dest, width, height, unit);
		break;
	case YUV411_YUYVYY:
		CYuv411F::Lbuf2Yuyvyy(src, dest, width, height, unit);
		break;
	case YUV411_YVYUYY:
		CYuv411F::Lbuf2Yvyuyy(src, dest, width, height, unit);
		break;
	case YUV411_UVYYYY:
		CYuv411F::Lbuf2Uvyyyy(src, dest, width, height, unit);
		break;
	case YUV411_VUYYYY:
		CYuv411F::Lbuf2Vuyyyy(src, dest, width, height, unit);
		break;
	case YUV420_UYVY:
	case YUV420_VYUY:
	case YUV420_YUYV:
	case YUV420_YVYU:
	case YUV420_YUVY:
	case YUV420_YVUY:
	case YUV420_UVYY:
	case YUV420_VUYY:
	case YUV420_YYVU:
		break;

	case YUV420_YYUV:
		break;
	case YUV400:
		break;
	case YUV444:
		break;
	case YUV422BLK:
	case YUV411BLK:
	case YUV420BLK:
	case YUV400BLK:
	case YUV444BLK:
	case BMP:
	case JPEG422:
	case JPEG420:
	case JPEG411:
	case JPEG400:
	case JPEG444:
	default:
		break;
	}
}

//===================================================
//
//			Yuv400 LBUF Convert
//
//==============================================
void CGeneralTools::Yuv400LbufConvert(char *src, char *dest, int width, int height, int dstfmt, int unit)
{
	switch(dstfmt)
	{
	case YUV422_UYVY:
	case YUV422_VYUY:
	case YUV422_YUYV:
	case YUV422_YVYU:
	case YUV422_YUVY:
	case YUV422_YVUY:
	case YUV422_UVYY:
	case YUV422_VUYY:
	case YUV422_YYVU:
	case YUV422_YYUV:
		break;
	case RAW_BG:
	case RAW_GB:
	case RAW_RG:
	case RAW_GR:
		break;
	case RGB1:
	case RGB2:
	case RGB4:
	case RGB8:
	case RGB444:
	case RGB666:
	case RGB32:
		break;

	case RGB24:
		break;
	case RGB565:
	case RGB555:
		break;
	case YUV411_YYYYUV:
	case YUV411_YYYYVU:
	case YUV411_YUYVYY:
	case YUV411_YVYUYY:
	case YUV411_UVYYYY:
	case YUV411_VUYYYY:
	case YUV420_UYVY:
	case YUV420_VYUY:
	case YUV420_YUYV:
	case YUV420_YVYU:
	case YUV420_YUVY:
	case YUV420_YVUY:
	case YUV420_UVYY:
	case YUV420_VUYY:
	case YUV420_YYVU:
		break;

	case YUV420_YYUV:
		break;
	case YUV400:
		CYuv400F::Lbuf2Yuv(src, dest, width, height, unit);
		break;
	case YUV444:
		break;
	case YUV422BLK:
	case YUV411BLK:
	case YUV420BLK:
	case YUV400BLK:
	case YUV444BLK:
	case BMP:
	case JPEG422:
	case JPEG420:
	case JPEG411:
	case JPEG400:
	case JPEG444:
	default:
		break;
	}
}

//===================================================
//
//			Yuv444 LBUF Convert
//
//==============================================
void CGeneralTools::Yuv444LbufConvert(char *src, char *dest, int width, int height, int dstfmt, int unit)
{
	switch(dstfmt)
	{
	case YUV422_UYVY:
	case YUV422_VYUY:
	case YUV422_YUYV:
	case YUV422_YVYU:
	case YUV422_YUVY:
	case YUV422_YVUY:
	case YUV422_UVYY:
	case YUV422_VUYY:
	case YUV422_YYVU:
	case YUV422_YYUV:
		break;
	case RAW_BG:
	case RAW_GB:
	case RAW_RG:
	case RAW_GR:
		break;
	case RGB1:
	case RGB2:
	case RGB4:
	case RGB8:
	case RGB444:
	case RGB666:
	case RGB32:
		break;

	case RGB24:
		break;
	case RGB565:
	case RGB555:
		break;
	case YUV411_YYYYUV:
	case YUV411_YYYYVU:
	case YUV411_YUYVYY:
	case YUV411_YVYUYY:
	case YUV411_UVYYYY:
	case YUV411_VUYYYY:
	case YUV420_UYVY:
	case YUV420_VYUY:
	case YUV420_YUYV:
	case YUV420_YVYU:
	case YUV420_YUVY:
	case YUV420_YVUY:
	case YUV420_UVYY:
	case YUV420_VUYY:
	case YUV420_YYVU:
		break;

	case YUV420_YYUV:
		break;
	case YUV400:
		break;
	case YUV444:
		CYuv444F::Lbuf2Yuv444(src, dest, width, height, unit);
		break;
	case YUV422BLK:
	case YUV411BLK:
	case YUV420BLK:
	case YUV400BLK:
	case YUV444BLK:
	case BMP:
	case JPEG422:
	case JPEG420:
	case JPEG411:
	case JPEG400:
	case JPEG444:
	default:
		break;
	}
}

//===================================================
//
//			Yuv420 LBUF Convert
//
//==============================================
void CGeneralTools::Yuv420LbufConvert(char *src, char *dest, int width, int height, int dstfmt, int unit)
{
	switch(dstfmt)
	{
	case YUV422_UYVY:
	case YUV422_VYUY:
	case YUV422_YUYV:
	case YUV422_YVYU:
	case YUV422_YUVY:
	case YUV422_YVUY:
	case YUV422_UVYY:
	case YUV422_VUYY:
	case YUV422_YYVU:
	case YUV422_YYUV:
		break;
	case RAW_BG:
	case RAW_GB:
	case RAW_RG:
	case RAW_GR:
		break;
	case RGB1:
	case RGB2:
	case RGB4:
	case RGB8:
	case RGB444:
	case RGB666:
	case RGB32:
		break;

	case RGB24:
		break;
	case RGB565:
	case RGB555:
		break;
	case YUV411_YYYYUV:
	case YUV411_YYYYVU:
	case YUV411_YUYVYY:
	case YUV411_YVYUYY:
	case YUV411_UVYYYY:
	case YUV411_VUYYYY:
		break;
	case YUV420_UYVY:
		CYuv420F::Lbuf2Uyvy(src, dest, width, height, unit);
		break;
	case YUV420_VYUY:
		CYuv420F::Lbuf2Vyuy(src, dest, width, height, unit);
		break;
	case YUV420_YUYV:
		CYuv420F::Lbuf2Yuyv(src, dest, width, height, unit);
		break;
	case YUV420_YVYU:
		CYuv420F::Lbuf2Yvyu(src, dest, width, height, unit);
		break;
	case YUV420_YUVY:
		CYuv420F::Lbuf2Yuvy(src, dest, width, height, unit);
		break;
	case YUV420_YVUY:
		CYuv420F::Lbuf2Yvuy(src, dest, width, height, unit);
		break;
	case YUV420_UVYY:
		CYuv420F::Lbuf2Uvyy(src, dest, width, height, unit);
		break;
	case YUV420_VUYY:
		CYuv420F::Lbuf2Vuyy(src, dest, width, height, unit);
		break;
	case YUV420_YYVU:
		CYuv420F::Lbuf2Yyvu(src, dest, width, height, unit);
		break;
	case YUV420_YYUV:
		CYuv420F::Lbuf2Yyuv(src, dest, width, height, unit);
		break;
	case YUV400:
		break;
	case YUV444:
		break;
	case YUV422BLK:
	case YUV411BLK:
	case YUV420BLK:
	case YUV400BLK:
	case YUV444BLK:
	case BMP:
	case JPEG422:
	case JPEG420:
	case JPEG411:
	case JPEG400:
	case JPEG444:
	default:
		break;
	}
}



//===================================================
//
//			Rgb1 data conver
//
//==============================================

void CGeneralTools::Rgb1Convert(char *src, char *dest, int width, int height, int dstfmt)
{
	switch(dstfmt)
	{
	case YUV422_UYVY:
	case YUV422_VYUY:
	case YUV422_YUYV:
	case YUV422_YVYU:
	case YUV422_YUVY:
	case YUV422_YVUY:
	case YUV422_UVYY:
	case YUV422_VUYY:
	case YUV422_YYVU:
	case YUV422_YYUV:
	case RAW_BG:
	case RAW_GB:
	case RAW_RG:
	case RAW_GR:
		break;
	case RGB1:
	case RGB2:
	case RGB4:
	case RGB8:
	case RGB444:
	case RGB666:
	case RGB32:
		break;

	case RGB24:
		CRaw2RgbF::Rgb1_Rgb24(src, dest, width, height);
		break;
	case RGB565:
	case RGB555:
		break;
	case YUV411_YYYYUV:
	case YUV411_YYYYVU:
	case YUV411_YUYVYY:
	case YUV411_YVYUYY:
	case YUV411_UVYYYY:
	case YUV411_VUYYYY:
		break;
	case YUV420_UYVY:
	case YUV420_VYUY:
	case YUV420_YUYV:
	case YUV420_YVYU:
	case YUV420_YUVY:
	case YUV420_YVUY:
	case YUV420_UVYY:
	case YUV420_VUYY:
	case YUV420_YYVU:
	case YUV420_YYUV:
	case YUV400:
		break;
	case YUV444:
		break;
	case YUV422BLK:
	case YUV411BLK:
	case YUV420BLK:
	case YUV400BLK:
	case YUV444BLK:
		break;
	case BMP:
		this->Rgb2BmpConvert(src, dest, width, height, RGB1);
		break;
	case JPEG422:
	case JPEG420:
	case JPEG411:
	case JPEG400:
	case JPEG444:
	default:
		break;
	}
}

//===================================================
//
//			Rgb2 data conver
//
//==============================================

void CGeneralTools::Rgb2Convert(char *src, char *dest, int width, int height, int dstfmt)
{
	switch(dstfmt)
	{
	case YUV422_UYVY:
	case YUV422_VYUY:
	case YUV422_YUYV:
	case YUV422_YVYU:
	case YUV422_YUVY:
	case YUV422_YVUY:
	case YUV422_UVYY:
	case YUV422_VUYY:
	case YUV422_YYVU:
	case YUV422_YYUV:
	case RAW_BG:
	case RAW_GB:
	case RAW_RG:
	case RAW_GR:
		break;
	case RGB1:
	case RGB2:
	case RGB4:
	case RGB8:
	case RGB444:
	case RGB666:
	case RGB32:
		break;

	case RGB24:
		CRaw2RgbF::Rgb2_Rgb24(src, dest, width, height);
		break;
	case RGB565:
	case RGB555:
		break;
	case YUV411_YYYYUV:
	case YUV411_YYYYVU:
	case YUV411_YUYVYY:
	case YUV411_YVYUYY:
	case YUV411_UVYYYY:
	case YUV411_VUYYYY:
		break;
	case YUV420_UYVY:
	case YUV420_VYUY:
	case YUV420_YUYV:
	case YUV420_YVYU:
	case YUV420_YUVY:
	case YUV420_YVUY:
	case YUV420_UVYY:
	case YUV420_VUYY:
	case YUV420_YYVU:
	case YUV420_YYUV:
	case YUV400:
		break;
	case YUV444:
		break;
	case YUV422BLK:
	case YUV411BLK:
	case YUV420BLK:
	case YUV400BLK:
	case YUV444BLK:
		break;
	case BMP:
		this->Rgb2BmpConvert(src, dest, width, height, RGB2);
		break;
	case JPEG422:
	case JPEG420:
	case JPEG411:
	case JPEG400:
	case JPEG444:
	default:
		break;
	}
}

//===================================================
//
//			Rgb4 data conver
//
//==============================================

void CGeneralTools::Rgb4Convert(char *src, char *dest, int width, int height, int dstfmt)
{
	switch(dstfmt)
	{
	case YUV422_UYVY:
	case YUV422_VYUY:
	case YUV422_YUYV:
	case YUV422_YVYU:
	case YUV422_YUVY:
	case YUV422_YVUY:
	case YUV422_UVYY:
	case YUV422_VUYY:
	case YUV422_YYVU:
	case YUV422_YYUV:
	case RAW_BG:
	case RAW_GB:
	case RAW_RG:
	case RAW_GR:
		break;
	case RGB1:
	case RGB2:
	case RGB4:
	case RGB8:
	case RGB444:
	case RGB666:
	case RGB32:
		break;

	case RGB24:
		CRaw2RgbF::Rgb4_Rgb24(src, dest, width, height);
		break;
	case RGB565:
	case RGB555:
		break;
	case YUV411_YYYYUV:
	case YUV411_YYYYVU:
	case YUV411_YUYVYY:
	case YUV411_YVYUYY:
	case YUV411_UVYYYY:
	case YUV411_VUYYYY:
		break;
	case YUV420_UYVY:
	case YUV420_VYUY:
	case YUV420_YUYV:
	case YUV420_YVYU:
	case YUV420_YUVY:
	case YUV420_YVUY:
	case YUV420_UVYY:
	case YUV420_VUYY:
	case YUV420_YYVU:
	case YUV420_YYUV:
	case YUV400:
		break;
	case YUV444:
		break;
	case YUV422BLK:
	case YUV411BLK:
	case YUV420BLK:
	case YUV400BLK:
	case YUV444BLK:
		break;
	case BMP:
		this->Rgb2BmpConvert(src, dest, width, height, RGB4);
		break;
	case JPEG422:
	case JPEG420:
	case JPEG411:
	case JPEG400:
	case JPEG444:
	default:
		break;
	}
}

//===================================================
//
//			Rgb8 data conver
//
//==============================================

void CGeneralTools::Rgb8Convert(char *src, char *dest, int width, int height, int dstfmt)
{
	switch(dstfmt)
	{
	case YUV422_UYVY:
	case YUV422_VYUY:
	case YUV422_YUYV:
	case YUV422_YVYU:
	case YUV422_YUVY:
	case YUV422_YVUY:
	case YUV422_UVYY:
	case YUV422_VUYY:
	case YUV422_YYVU:
	case YUV422_YYUV:
	case RAW_BG:
	case RAW_GB:
	case RAW_RG:
	case RAW_GR:
		break;
	case RGB1:
		break;
	case RGB2:
		CRaw2RgbF::Rgb8_Rgb2(src, dest, width, height);
		break;
	case RGB4:
	case RGB8:
	case RGB444:
	case RGB666:
	case RGB32:
		break;

	case RGB24:
		CRaw2RgbF::Rgb8_Rgb24(src, dest, width, height);
		break;
	case RGB565:
	case RGB555:
		break;
	case YUV411_YYYYUV:
	case YUV411_YYYYVU:
	case YUV411_YUYVYY:
	case YUV411_YVYUYY:
	case YUV411_UVYYYY:
	case YUV411_VUYYYY:
		break;
	case YUV420_UYVY:
	case YUV420_VYUY:
	case YUV420_YUYV:
	case YUV420_YVYU:
	case YUV420_YUVY:
	case YUV420_YVUY:
	case YUV420_UVYY:
	case YUV420_VUYY:
	case YUV420_YYVU:
	case YUV420_YYUV:
	case YUV400:
		break;
	case YUV444:
		break;
	case YUV422BLK:
	case YUV411BLK:
	case YUV420BLK:
	case YUV400BLK:
	case YUV444BLK:
		break;
	case BMP:
		this->Rgb2BmpConvert(src, dest, width, height, RGB8);
		break;
	case JPEG422:
	case JPEG420:
	case JPEG411:
	case JPEG400:
	case JPEG444:
	default:
		break;
	}
}

//===================================================
//
//			Rgb444 data conver
//
//==============================================

void CGeneralTools::Rgb444Convert(char *src, char *dest, int width, int height, int dstfmt, int qf, int unit)
{
	switch(dstfmt)
	{
	case YUV422_UYVY:
	case YUV422_VYUY:
	case YUV422_YUYV:
	case YUV422_YVYU:
	case YUV422_YUVY:
	case YUV422_YVUY:
	case YUV422_UVYY:
	case YUV422_VUYY:
	case YUV422_YYVU:
	case YUV422_YYUV:
	case RAW_BG:
	case RAW_GB:
	case RAW_RG:
	case RAW_GR:
		break;
	case RGB1:
	case RGB2:
	case RGB4:
	case RGB8:
	case RGB444:
	case RGB666:
	case RGB32:
		break;

	case RGB24:
		CRaw2RgbF::Rgb444_Rgb24(src, dest, width, height);
		break;
	case RGB565:
	case RGB555:
		break;
	case YUV411_YYYYUV:
	case YUV411_YYYYVU:
	case YUV411_YUYVYY:
	case YUV411_YVYUYY:
	case YUV411_UVYYYY:
	case YUV411_VUYYYY:
		break;
	case YUV420_UYVY:
	case YUV420_VYUY:
	case YUV420_YUYV:
	case YUV420_YVYU:
	case YUV420_YUVY:
	case YUV420_YVUY:
	case YUV420_UVYY:
	case YUV420_VUYY:
	case YUV420_YYVU:
	case YUV420_YYUV:
	case YUV400:
		break;
	case YUV444:
		break;
	case YUV422BLK:
	case YUV411BLK:
	case YUV420BLK:
	case YUV400BLK:
	case YUV444BLK:
		this->Rgb2YuvBlkConvert(src, dest, width, height, RGB444, dstfmt, unit);
		break;
	case BMP:
		this->Rgb2BmpConvert(src, dest, width, height, RGB444);
		break;
	case JPEG422:
	case JPEG420:
	case JPEG411:
	case JPEG400:
	case JPEG444:
		this->Rgb2JpegConvert(src, dest, width, height, RGB444, dstfmt, qf, unit);
		break;
	default:
		break;
	}
}

//===================================================
//
//			Rgb555 data conver
//
//==============================================

void CGeneralTools::Rgb555Convert(char *src, char *dest, int width, int height, int dstfmt)
{
	switch(dstfmt)
	{
	case YUV422_UYVY:
	case YUV422_VYUY:
	case YUV422_YUYV:
	case YUV422_YVYU:
	case YUV422_YUVY:
	case YUV422_YVUY:
	case YUV422_UVYY:
	case YUV422_VUYY:
	case YUV422_YYVU:
	case YUV422_YYUV:
	case RAW_BG:
	case RAW_GB:
	case RAW_RG:
	case RAW_GR:
		break;
	case RGB1:
	case RGB2:
	case RGB4:
	case RGB8:
	case RGB444:
	case RGB666:
	case RGB32:
		break;

	case RGB24:
		CRaw2RgbF::Rgb555_Rgb24(src, dest, width, height);
		break;
	case RGB565:
	case RGB555:
		break;
	case YUV411_YYYYUV:
	case YUV411_YYYYVU:
	case YUV411_YUYVYY:
	case YUV411_YVYUYY:
	case YUV411_UVYYYY:
	case YUV411_VUYYYY:
		break;
	case YUV420_UYVY:
	case YUV420_VYUY:
	case YUV420_YUYV:
	case YUV420_YVYU:
	case YUV420_YUVY:
	case YUV420_YVUY:
	case YUV420_UVYY:
	case YUV420_VUYY:
	case YUV420_YYVU:
	case YUV420_YYUV:
	case YUV400:
		break;
	case YUV444:
		break;
	case YUV422BLK:
	case YUV411BLK:
	case YUV420BLK:
	case YUV400BLK:
	case YUV444BLK:
		break;
	case BMP:
		this->Rgb2BmpConvert(src, dest, width, height, RGB555);
		break;
	case JPEG422:
	case JPEG420:
	case JPEG411:
	case JPEG400:
	case JPEG444:
	default:
		break;
	}
}

//===================================================
//
//			Rgb565 data conver
//
//==============================================

void CGeneralTools::Rgb565Convert(char *src, char *dest, int width, int height, int dstfmt)
{
	switch(dstfmt)
	{
	case YUV422_UYVY:
	case YUV422_VYUY:
	case YUV422_YUYV:
	case YUV422_YVYU:
	case YUV422_YUVY:
	case YUV422_YVUY:
	case YUV422_UVYY:
	case YUV422_VUYY:
	case YUV422_YYVU:
	case YUV422_YYUV:
	case RAW_BG:
	case RAW_GB:
	case RAW_RG:
	case RAW_GR:
		break;
	case RGB1:
	case RGB2:
	case RGB4:
	case RGB8:
	case RGB444:
	case RGB666:
	case RGB32:
		break;

	case RGB24:
		CRaw2RgbF::Rgb565_Rgb24(src, dest, width, height);
		break;
	case RGB565:
	case RGB555:
		break;
	case YUV411_YYYYUV:
	case YUV411_YYYYVU:
	case YUV411_YUYVYY:
	case YUV411_YVYUYY:
	case YUV411_UVYYYY:
	case YUV411_VUYYYY:
		break;
	case YUV420_UYVY:
	case YUV420_VYUY:
	case YUV420_YUYV:
	case YUV420_YVYU:
	case YUV420_YUVY:
	case YUV420_YVUY:
	case YUV420_UVYY:
	case YUV420_VUYY:
	case YUV420_YYVU:
	case YUV420_YYUV:
	case YUV400:
		break;
	case YUV444:
		break;
	case YUV422BLK:
	case YUV411BLK:
	case YUV420BLK:
	case YUV400BLK:
	case YUV444BLK:
		break;
	case BMP:
		this->Rgb2BmpConvert(src, dest, width, height, RGB565);
		break;
	case JPEG422:
	case JPEG420:
	case JPEG411:
	case JPEG400:
	case JPEG444:
	default:
		break;
	}
}

//===================================================
//
//			Rgb666 data conver
//
//==============================================

void CGeneralTools::Rgb666Convert(char *src, char *dest, int width, int height, int dstfmt)
{
	switch(dstfmt)
	{
	case YUV422_UYVY:
	case YUV422_VYUY:
	case YUV422_YUYV:
	case YUV422_YVYU:
	case YUV422_YUVY:
	case YUV422_YVUY:
	case YUV422_UVYY:
	case YUV422_VUYY:
	case YUV422_YYVU:
	case YUV422_YYUV:
	case RAW_BG:
	case RAW_GB:
	case RAW_RG:
	case RAW_GR:
		break;
	case RGB1:
	case RGB2:
	case RGB4:
	case RGB8:
	case RGB444:
	case RGB666:
	case RGB32:
		break;

	case RGB24:
		CRaw2RgbF::Rgb666_Rgb24(src, dest, width, height);
		break;
	case RGB565:
	case RGB555:
		break;
	case YUV411_YYYYUV:
	case YUV411_YYYYVU:
	case YUV411_YUYVYY:
	case YUV411_YVYUYY:
	case YUV411_UVYYYY:
	case YUV411_VUYYYY:
		break;
	case YUV420_UYVY:
	case YUV420_VYUY:
	case YUV420_YUYV:
	case YUV420_YVYU:
	case YUV420_YUVY:
	case YUV420_YVUY:
	case YUV420_UVYY:
	case YUV420_VUYY:
	case YUV420_YYVU:
	case YUV420_YYUV:
	case YUV400:
		break;
	case YUV444:
		break;
	case YUV422BLK:
	case YUV411BLK:
	case YUV420BLK:
	case YUV400BLK:
	case YUV444BLK:
		break;
	case BMP:
		this->Rgb2BmpConvert(src, dest, width, height, RGB666);
		break;
	case JPEG422:
	case JPEG420:
	case JPEG411:
	case JPEG400:
	case JPEG444:
	default:
		break;
	}
}

//===================================================
//
//			Rgb32 data conver
//
//==============================================

void CGeneralTools::Rgb32Convert(char *src, char *dest, int width, int height, int dstfmt)
{
	switch(dstfmt)
	{
	case YUV422_UYVY:
	case YUV422_VYUY:
	case YUV422_YUYV:
	case YUV422_YVYU:
	case YUV422_YUVY:
	case YUV422_YVUY:
	case YUV422_UVYY:
	case YUV422_VUYY:
	case YUV422_YYVU:
	case YUV422_YYUV:
	case RAW_BG:
	case RAW_GB:
	case RAW_RG:
	case RAW_GR:
		break;
	case RGB1:
	case RGB2:
	case RGB4:
	case RGB8:
	case RGB444:
	case RGB666:
	case RGB32:
		break;

	case RGB24:
		CRaw2RgbF::Rgb32_Rgb24(src, dest, width, height);
		break;
	case RGB565:
	case RGB555:
		break;
	case YUV411_YYYYUV:
	case YUV411_YYYYVU:
	case YUV411_YUYVYY:
	case YUV411_YVYUYY:
	case YUV411_UVYYYY:
	case YUV411_VUYYYY:
		break;
	case YUV420_UYVY:
	case YUV420_VYUY:
	case YUV420_YUYV:
	case YUV420_YVYU:
	case YUV420_YUVY:
	case YUV420_YVUY:
	case YUV420_UVYY:
	case YUV420_VUYY:
	case YUV420_YYVU:
	case YUV420_YYUV:
	case YUV400:
		break;
	case YUV444:
		break;
	case YUV422BLK:
	case YUV411BLK:
	case YUV420BLK:
	case YUV400BLK:
	case YUV444BLK:
		break;
	case BMP:
		this->Rgb2BmpConvert(src, dest, width, height, RGB32);
		break;
	case JPEG422:
	case JPEG420:
	case JPEG411:
	case JPEG400:
	case JPEG444:
	default:
		break;
	}
}

void CGeneralTools::Rgb2YuvBlkConvert(char *src, char *dest, int width, int height, int srcfmt, int dstfmt, int unit)
{
	CFileBin fsrc, fdst;
	int length = height*width, size;
	CBufferBase srcbuf, dstbuf;

	unit = 1; //In the future, if rawRgb were introduced, this parameter may be useful.

	if( (width == 0) || (height == 0) )
	{
		this->m_dbug.PrintMsg("error: src unit = 0 or dst unit = 0\n");
		return;
	}

	fsrc.Open(src);
	fdst.Open(dest, "wb");

	size = length*3;
	dstbuf.SetSize(size);
	
	switch(srcfmt)
	{
	case RGB1:
		size = ((length+7) >> 3) + 8/*palette length*/;
		break;
	case RGB2:
		size = ((length + 3) >> 2) + 16;
		break;
	case RGB4:
		size = ((length + 1) >> 1) + 0x40;
		break;
	case RGB8:
		size = length + 0x400;
		break;
	case RGB444:
		size = (length*3) >> 1;
		break;
	case RGB555:
		size = length << 1;
		break;
	case RGB565:
		size = length << 1;
		break;
	case RGB32:
		size = length << 2;
		break;
	case RGB24:
		size = length * 3;
		break;
	default:
		break;
	}

	srcbuf.SetSize(size);
	fsrc.Read(srcbuf.m_buf, size);
	size = CRaw2Rgb::Rgb_Rgb24Convert(srcbuf.m_buf, dstbuf.m_buf, width, height, srcfmt);

	/*Now, srcbuf.m_buf is to be used as buffer for YuvBlk data this time. Since this size may be 
	used for YuvBlk444/YuvBlk411 size (including head & tail), so is greater than "width*height*unit*3".*/
	srcbuf.SetSize(length << 2);  
	size = this->Rgb24_YuvBlkBuffer(dstbuf.m_buf, srcbuf.m_buf, width, height, dstfmt);
	fdst.Write(srcbuf.m_buf, size);
}

int CGeneralTools::Rgb24_YuvBlkBuffer(char *src, char *dest, int width, int height, int dstfmt)
{
	int j, len, size, step, cap = 256, dstbuflen = 0; 
	int size_blk = (width * 3) << 3; //8 lines RGB24 data
				
	switch(dstfmt)
	{
	case YUV422BLK:
		{
			if(width % 2)
			{
				this->m_dbug.PrintMsg("Image width must be even\n");
				return 0;
			}
			for(j = 0; j < height; j += 8)
			{
				step = (8 < (height - j))? 8 : (height-j);
				len = CYuv422::Rgb2Yuv422Blk(src, dest, width, step, 1);
				src += size_blk;
				dest += len;
				dstbuflen += len;
			}
			break;
		}
	case YUV411BLK:
		{
			if(width % 4)
			{
				this->m_dbug.PrintMsg("Image width must be times of 4.\n");
				return 0;
			}
				
			for(j = 0; j < height; j += 8)
			{
				step = (8 < (height - j))? 8 : (height-j);
				len = CYuv411::Rgb2Yuv411Blk(src, dest, width, step, 1);
				//size_blk = (8 < (height - j))? size_blk : (height-j)*width*3;				
				src += size_blk;
				dest += len;
				dstbuflen += len;
			}		
			break;
		}
	case YUV400BLK:
		{
			if( (width % 2) || (height % 2) )
			{
				this->m_dbug.PrintMsg("Image width & height must be even\n");
				return 0;
			}

			dstbuflen = CYuv400::Rgb2Yuv400Blk(src, dest, width, height, cap, 1);
			break;
		}
	case YUV420BLK:
		{
			if( (width % 2) || (height % 2) )
			{
				this->m_dbug.PrintMsg("Image width & height must be even\n");
				return 0;
			}

			size = (width * 3) << 4;
			for(j = 0; j < height; j += 16)
			{
				step = (16 < (height - j))? 16 : (height-j);
				len = CYuv420::Rgb2Yuv420Blk(src, dest, width, step, 1);
				//size = (16 < (height - j))? size : (height-j)*width*3;				
				src += size;
				dest += len;
				dstbuflen += len;
			}			
			break;
		}
	case YUV444BLK:
		{
			if( (width % 2) || (height % 2) )
			{
				this->m_dbug.PrintMsg("Image width & height must be even\n");
				return 0;
			}
			
			dstbuflen = CYuv444::Rgb2Yuv444Blk(src, dest, width, height, cap, 1);
			break;
		}
	default:
		break;		
	}

	return dstbuflen;
}

void CGeneralTools::Rgb2JpegConvert(char *src, char *dest, int width, int height, int srcfmt, int dstfmt, int qf, int unit)
{
	CFileBin fsrc, fdst;
	int length = height*width, size;
	CBufferBase srcbuf, dstbuf;

	unit = 1; //In the future, if rawRgb were introduced, this parameter may be useful.

	if( (width == 0) || (height == 0) )
	{
		this->m_dbug.PrintMsg("error: src unit = 0 or dst unit = 0\n");
		return;
	}

	fsrc.Open(src);
	fdst.Open(dest, "wb");

	size = length*3;
	dstbuf.SetSize(size);
	
	switch(srcfmt)
	{
	case RGB1:
		size = ((length+7) >> 3) + 8/*palette length*/;
		break;
	case RGB2:
		size = ((length + 3) >> 2) + 16;
		break;
	case RGB4:
		size = ((length + 1) >> 1) + 0x40;
		break;
	case RGB8:
		size = length + 0x400;
		break;
	case RGB444:
		size = (length*3) >> 1;
		break;
	case RGB555:
		size = length << 1;
		break;
	case RGB565:
		size = length << 1;
		break;
	case RGB32:
		size = length << 2;
		break;
	case RGB24:
		size = length * 3;
		break;
	default:
		break;
	}

	srcbuf.SetSize(size);
	fsrc.Read(srcbuf.m_buf, size);
	size = CRaw2Rgb::Rgb_Rgb24Convert(srcbuf.m_buf, dstbuf.m_buf, width, height, srcfmt);

	srcbuf.SetSize(length); //Now, srcbuf.m_buf is to be used as buffer for Jpeg data, "length" is enough.
	size = this->Rgb24_JpegBuffer(dstbuf.m_buf, srcbuf.m_buf, width, height, dstfmt, qf);
	fdst.Write(srcbuf.m_buf, size);
}

int CGeneralTools::Rgb24_JpegBuffer(char *src, char *dest, int width, int height, int dstfmt, int qf)
{
	int len = 0; 
				
	switch(dstfmt)
	{
	case JPEG422:
		if(width % 2)
		{
			this->m_dbug.PrintMsg("Image width must be even\n");
			return 0;
		}		
		len = CYuv422::Rgb2Jpeg422(src, dest, width, height, qf, 1);
		break;
	case JPEG411:
		if(width % 4)
		{
			this->m_dbug.PrintMsg("Image width must be times of 4.\n");
			return 0;
		}
		len = CYuv411::Rgb2Jpeg411(src, dest, width, height, 1, qf);
		break;
	case JPEG400:
		if( (width % 2) || (height % 2) )
		{
			this->m_dbug.PrintMsg("Image width & height must be even\n");
			return 0;
		}
		len = CYuv400::Rgb2Jpeg400(src, dest, width, height, 1, qf);
		break;
	case JPEG420:
		if( (width % 2) || (height % 2) )
		{
			this->m_dbug.PrintMsg("Image width & height must be even\n");
			return 0;
		}
		len = CYuv420::Rgb2Jpeg420(src, dest, width, height, 1, qf);
		break;
	case JPEG444:
		if( (width % 2) || (height % 2) )
		{
			this->m_dbug.PrintMsg("Image width & height must be even\n");
			return 0;
		}
		len = CYuv444::Rgb2Jpeg444(src, dest, width, height, 1, qf);
		break;
	default:
		break;		
	}

	return len;
}

//***************************************************//

//===================================================
//
//			All data Convert
//
//==============================================

//***************************************************//
void CGeneralTools::DataConvert(char *src, char *dest, int width, int height, 
								int srcfmt, int dstfmt, int unit, int dither, int qfactor)
{
	if(unit == 0)
	{
		this->m_dbug.PrintMsg("error: unit = 0\n");
		return;
	}
	if(srcfmt == dstfmt)
	{
		CNormalTools::DropHighByte(src, dest, ADD_OPCODE, 0, 1, 1);
		return;
	}

	switch(srcfmt)
	{
	case YUV422_UYVY:
		this->Uyvy422Convert(src, dest, width, height, dstfmt, unit);
		break;
	case YUV422_VYUY:
		this->Vyuy422Convert(src, dest, width, height, dstfmt, unit);
		break;
	case YUV422_YUYV:
		this->Yuyv422Convert(src, dest, width, height, dstfmt, unit);
		break;
	case YUV422_YVYU:
		this->Yvyu422Convert(src, dest, width, height, dstfmt, unit);
		break;
	case YUV422_YUVY:
		this->Yuvy422Convert(src, dest, width, height, dstfmt, unit);
		break;
	case YUV422_YVUY:
		this->Yvuy422Convert(src, dest, width, height, dstfmt, unit);
		break;
	case YUV422_UVYY:
		this->Uvyy422Convert(src, dest, width, height, dstfmt, unit);
		break;
	case YUV422_VUYY:
		this->Vuyy422Convert(src, dest, width, height, dstfmt, unit);
		break;
	case YUV422_YYVU:
		this->Yyvu422Convert(src, dest, width, height, dstfmt, unit);
		break;
	case RAW_BG:
		this->BayerBGConvert(src, dest, width, height, dstfmt, unit);
		break;
	case RAW_GB:
		this->BayerGBConvert(src, dest, width, height, dstfmt, unit);
		break;
	case RAW_RG:
		this->BayerRGConvert(src, dest, width, height, dstfmt, unit);
		break;
	case RAW_GR:
		this->BayerGRConvert(src, dest, width, height, dstfmt, unit);
		break;
	case RAW_RGB:
		
		break;
	case RGB1:
		this->Rgb1Convert(src, dest, width, height, dstfmt);
		break;
	case RGB2:
		this->Rgb2Convert(src, dest, width, height, dstfmt);
		break;
	case RGB4:
		this->Rgb4Convert(src, dest, width, height, dstfmt);
		break;
	case RGB8:
		this->Rgb8Convert(src, dest, width, height, dstfmt);
		break;
	case RGB444:
		this->Rgb444Convert(src, dest, width, height, dstfmt, qfactor, unit);
		break;
	case RGB666:
		this->Rgb666Convert(src, dest, width, height, dstfmt);
		break;
	case RGB32:
		this->Rgb32Convert(src, dest, width, height, dstfmt);
		break;

	case RGB24:
		this->Rgb24Convert(src, dest, width, height, dstfmt, dither, qfactor);
		break;
	case RGB565:
		this->Rgb565Convert(src, dest, width, height, dstfmt);
		break;
	case RGB555:
		this->Rgb555Convert(src, dest, width, height, dstfmt);
		break;
	case YUV422_YYUV:
		this->Yyuv422Convert(src, dest, width, height, dstfmt, unit);
		break;
	case YUV411_YYYYUV:
		this->Yyyyuv411Convert(src, dest, width, height, dstfmt, unit);
		break;
	case YUV411_YYYYVU:
		this->Yyyyvu411Convert(src, dest, width, height, dstfmt, unit);
		break;
	case YUV411_YUYVYY:
		this->Yuyvyy411Convert(src, dest, width, height, dstfmt, unit);
		break;
	case YUV411_YVYUYY:
		this->Yvyuyy411Convert(src, dest, width, height, dstfmt, unit);
		break;
	case YUV411_UVYYYY:
		this->Uvyyyy411Convert(src, dest, width, height, dstfmt, unit);
		break;
	case YUV411_VUYYYY:
		this->Vuyyyy411Convert(src, dest, width, height, dstfmt, unit);
		break;
	case YUV420_YYUV:
		this->Yyuv420Convert(src, dest, width, height, dstfmt, unit);
		break;
	case YUV420_UYVY:
		this->Uyvy420Convert(src, dest, width, height, dstfmt, unit);
		break;
	case YUV420_VYUY:
		this->Vyuy420Convert(src, dest, width, height, dstfmt, unit);
		break;
	case YUV420_YUYV:
		this->Yuyv420Convert(src, dest, width, height, dstfmt, unit);
		break;
	case YUV420_YVYU:
		this->Yvyu420Convert(src, dest, width, height, dstfmt, unit);
		break;
	case YUV420_YUVY:
		this->Yuvy420Convert(src, dest, width, height, dstfmt, unit);
		break;
	case YUV420_YVUY:
		this->Yvuy420Convert(src, dest, width, height, dstfmt, unit);
		break;
	case YUV420_UVYY:
		this->Uvyy420Convert(src, dest, width, height, dstfmt, unit);
		break;
	case YUV420_VUYY:
		this->Vuyy420Convert(src, dest, width, height, dstfmt, unit);
		break;
	case YUV420_YYVU:
		this->Yyvu420Convert(src, dest, width, height, dstfmt, unit);
		break;
	case YUV400:
		this->Yuv400Convert(src, dest, width, height, dstfmt, unit);
		break;
	case YUV444:
		this->Yuv444Convert(src, dest, width, height, dstfmt, unit);
		break;
	case YUV422BLK:
		this->Yuv422BlkConvert(src, dest, width, height, dstfmt, unit, qfactor);
		break;
	case YUV411BLK:
		this->Yuv411BlkConvert(src, dest, width, height, dstfmt, unit);
		break;
	case YUV420BLK:
		this->Yuv420BlkConvert(src, dest, width, height, dstfmt, unit);
		break;
	case YUV400BLK:
		this->Yuv400BlkConvert(src, dest, width, height, dstfmt, unit);
		break;
	case YUV444BLK:
		this->Yuv444BlkConvert(src, dest, width, height, dstfmt, unit);
		break;
	case YUV422LBUF:
		this->Yuv422LbufConvert(src, dest, width, height, dstfmt, unit);
		break;
	case YUV444422LBUF:
		this->Yuv444422LbufConvert(src, dest, width, height, dstfmt, unit);
		break;
	case YUV411LBUF:
		this->Yuv411LbufConvert(src, dest, width, height, dstfmt, unit);
		break;
	case YUV420LBUF:
		this->Yuv420LbufConvert(src, dest, width, height, dstfmt, unit);
		break;
	case YUV400LBUF:
		this->Yuv400LbufConvert(src, dest, width, height, dstfmt, unit);
		break;
	case YUV444LBUF:
		this->Yuv444LbufConvert(src, dest, width, height, dstfmt, unit);
		break;
	case BMP:
		this->BmpConvert(src, dest, dstfmt, qfactor);
		break;
	case JPEG422:
	case JPEG420:
	case JPEG411:
	case JPEG400:
	case JPEG444:
		this->JpegConvert(src, dest, dstfmt);
		break;
	case YUV422FRM:
		this->Yuv422FrmConvert(src, dest, width, height, dstfmt, unit);
		break;
	case YUV420FRM:
		this->Yuv420FrmConvert(src, dest, width, height, dstfmt, unit);
		break;
	case MPEG2IFRM420:
	case MPEG2IFRM422:
	case MPEG2IFRM444:
		this->Mpeg2Convert(src, dest);
		break;
	case MPEG4_FRM:
		this->Mpeg4Convert(src, dest);
		break;

	default:
		break;
	}
}

//***************************************************//

//===============================================
//
//		Add Bmp Header
//
//================================================

//***************************************************//
void CGeneralTools::Rgb2BmpConvert(char *src, char *dest, int width, int height, int srcfmt)
{
	switch(srcfmt)
	{
	case RGB1:
		CRaw2RgbF::Rgb1_Bmp(src, dest, width, height);
		break;
	case RGB2:
		CRaw2RgbF::Rgb2_Bmp(src, dest, width, height);
		break;
	case RGB4:
		CRaw2RgbF::Rgb4_Bmp(src, dest, width, height);
		break;
	case RGB8:
		CRaw2RgbF::Rgb8_Bmp(src, dest, width, height);
		break;
	case RGB444:
		CRaw2RgbF::Rgb444_Bmp(src, dest, width, height);
		break;
	case RGB666:
		CRaw2RgbF::Rgb666_Bmp(src, dest, width, height);
		break;
	case RGB32:
		CRaw2RgbF::Rgb32_Bmp(src, dest, width, height);
		break;
	case RGB24:
		CRaw2RgbF::Rgb24_Bmp(src, dest, width, height);
		break;
	case RGB565:
		CRaw2RgbF::Rgb565_Bmp(src, dest, width, height);
		break;
	case RGB555:
		CRaw2RgbF::Rgb555_Bmp(src, dest, width, height);
		break;
	default:
		break;
	}
}

//===============================================
//
//		Delete Bmp Header
//
//================================================
void CGeneralTools::BmpConvert(char *src, char *dest, int dstfmt, int qf)
{
	int srcfmt = this->GetBmpFileFormat(src);

	if( (srcfmt != dstfmt) && (dstfmt <= RGB32) && (dstfmt >= RGB1) )
	{
		this->m_dbug.PrintMsg("Source bmp file format not same as choosed format\n");
		return;
	}
	switch(dstfmt)
	{
	case RGB1:
		CRaw2RgbF::Bmp1_Rgb(src, dest);
		break;
	case RGB2:
		CRaw2RgbF::Bmp2_Rgb(src, dest);
		break;
	case RGB4:
		CRaw2RgbF::Bmp4_Rgb(src, dest);
		break;
	case RGB8:
		CRaw2RgbF::Bmp8_Rgb(src, dest);
		break;
	case RGB444:
		CRaw2RgbF::Bmp444_Rgb(src, dest);
		break;
	case RGB666:
		CRaw2RgbF::Bmp666_Rgb(src, dest);
		break;
	case RGB32:
		CRaw2RgbF::Bmp32_Rgb(src, dest);
		break;
	case RGB24:
		CRaw2RgbF::Bmp24_Rgb(src, dest);
		break;
	case RGB565:
		CRaw2RgbF::Bmp565_Rgb(src, dest);
		break;
	case RGB555:
		CRaw2RgbF::Bmp555_Rgb(src, dest);
		break;
	case YUV422BLK:
	case YUV411BLK:
	case YUV420BLK:
	case YUV400BLK:
	case YUV444BLK:
		this->Bmp2YuvBlkConvert(src, dest, dstfmt);
		break;
	case JPEG422:
	case JPEG420:
	case JPEG411:
	case JPEG400:
	case JPEG444:
		this->Bmp2JpegConvert(src, dest, dstfmt, qf);
		break;
	default:
		break;
	}
}

void	CGeneralTools::Bmp2YuvBlkConvert(char *src,  char *dest, int dstfmt)
{
	CFileBin fsrc, fdst;
	CBufferBase pool, tmpbuf;
	BITMAPINFOHEADER *info = (BITMAPINFOHEADER *)(pool.m_buf+sizeof(BITMAPFILEHEADER));
	int msize, width, height;
	int len, bitwidth = this->GetBmpFileBitWidth(src);
	int srcfmt =this->GetBmpFileFormat(src);

	if(pool.GetSize() == 0)
	{
		this->m_dbug.PrintMsg("can't alloc pool\n");
		return;
	}	
	
	fsrc.Open(src);
	fdst.Open(dest, "wb");
	fsrc.Read(pool.m_buf, sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER));

	width = info->biWidth;
	height = info->biHeight;
	height = (height > 0) ? height : -height; //height may be a negative number.
	
	msize =  height;
	msize *= ((((width * bitwidth + 7) >> 3) + 3) >> 2) <<2;

	pool.SetSize(msize);
	tmpbuf.SetSize(msize);
	fsrc.Read(pool.m_buf, msize);

	//Store bmp image data in RGB fomat file.
	CRaw2Rgb::bitbmp2rgb(pool.m_buf, tmpbuf.m_buf, width, height, bitwidth);

	//Now, pool.m_buf is to be used as buffer for Rgb24 data. 
	pool.SetSize(width * height * 3); 

	//convert "general RGB" to RGB24
	len = CRaw2Rgb::Rgb_Rgb24Convert(tmpbuf.m_buf, pool.m_buf, width, height, srcfmt);
	
	//convert RGB fomat file into YuvBlk fomat.
	/*The following variant "tmpbuf.m_buf" is to be used as buffer for YuvBlk data this time. Since this size may be 
	used for YuvBlk444/YuvBlk411 size (including head & tail), so is greater than "width*height*unit*3".*/
	tmpbuf.SetSize(width * height << 2); 

	len = this->Rgb24_YuvBlkBuffer(pool.m_buf, tmpbuf.m_buf, width, height, dstfmt); //dstfmt = YUV422BLK, etc
	fdst.Write(tmpbuf.m_buf, len);			
}

void	CGeneralTools::Bmp2JpegConvert(char *src,  char *dest, int dstfmt, int qf)
{
	CFileBin fsrc, fdst;
	CBufferBase pool, tmpbuf;
	BITMAPINFOHEADER *info = (BITMAPINFOHEADER *)(pool.m_buf+sizeof(BITMAPFILEHEADER));
	int msize, width, height;
	int len, bitwidth = this->GetBmpFileBitWidth(src);
	int srcfmt =this->GetBmpFileFormat(src);

	if(pool.GetSize() == 0)
	{
		this->m_dbug.PrintMsg("can't alloc pool\n");
		return;
	}	
	
	fsrc.Open(src);
	fdst.Open(dest, "wb");
	fsrc.Read(pool.m_buf, sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER));

	width = info->biWidth;
	height = info->biHeight;
	height = (height > 0) ? height : -height; //height may be a negative number.
	
	msize =  height;
	msize *= ((((width * bitwidth + 7) >> 3) + 3) >> 2) <<2;

	pool.SetSize(msize);
	tmpbuf.SetSize(msize);
	fsrc.Read(pool.m_buf, msize);

	//Store bmp image data in RGB fomat file.
	CRaw2Rgb::bitbmp2rgb(pool.m_buf, tmpbuf.m_buf, width, height, bitwidth);

	//Now, pool.m_buf is to be used as buffer for Rgb24 data. 
	pool.SetSize(width * height * 3); 

	//convert "general RGB" to RGB24
	len = CRaw2Rgb::Rgb_Rgb24Convert(tmpbuf.m_buf, pool.m_buf, width, height, srcfmt);
	
	//convert RGB fomat file into Jpeg fomat.
	tmpbuf.SetSize(width * height); //tmpbuf.m_buf is to be used as buffer for Jpeg data this time. "width*height" is enough.
	
	len = this->Rgb24_JpegBuffer(pool.m_buf, tmpbuf.m_buf, width, height, dstfmt, qf); //e.g. dstfmt = JPEG422
	fdst.Write(tmpbuf.m_buf, len);			
}

void CGeneralTools::Blk2Bmp(char *src, char *dest, int width, int height, int srcfmt)
{
	int size, msize, ncount = 0;
	CFileBin fsrc, fdst;
	CBufferBase srcbuf, dstbuf;

	fsrc.Open(src);
	fdst.Open(dest, "wb");
	msize = (height * width) * 3;
	srcbuf.SetSize(msize);
	dstbuf.SetSize(msize);
	size = fsrc.Read(srcbuf.m_buf, msize);
	if (size == 0)
		return;

	//Blk to Grb24:
	switch(srcfmt)
	{
	case YUV422BLK:
		if( (width % 2) || (height % 2) )
		{
			this->m_dbug.PrintMsg("Image width & height must be even\n");
			return;
		}		
		CYuv422::Blk2Rgb24(srcbuf.m_buf, dstbuf.m_buf, width, height);
		break;
	case YUV420BLK:
		if( (width % 2) || (height % 2) )
		{
			this->m_dbug.PrintMsg("Image width & height must be even\n");
			return;
		}			
		CYuv420::Blk2Rgb24(srcbuf.m_buf, dstbuf.m_buf, width, height);
		break;
	case YUV411BLK:
		if(width % 4)
		{
			this->m_dbug.PrintMsg("Image width must be divided by 4;\n height must be even\n");
			return;
		}		
		CYuv411::Blk2Rgb24(srcbuf.m_buf, dstbuf.m_buf, width, height);
		break;
	case YUV400BLK:
		if( (width % 2) || (height % 2) )
		{
			this->m_dbug.PrintMsg("Image width & height must be even\n");
			return;
		}		
		CYuv400::Blk2Rgb24(srcbuf.m_buf, dstbuf.m_buf, width, height);
		break;
	case YUV444BLK:
		if( (width % 2) || (height % 2) )
		{
			this->m_dbug.PrintMsg("Image width & height must be even\n");
			return;
		}		
		CYuv444::Blk2Rgb24(srcbuf.m_buf, dstbuf.m_buf, width, height);
		break;
	default:
		return;
		break;
	}

	msize += sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER);
	srcbuf.SetSize(msize); //resize to hold the bmp head content.

	//Rgb24 to Bmp:
 	ncount = CRaw2Rgb::bitrgb2bmp( dstbuf.m_buf, srcbuf.m_buf, width, height, 24);
	fdst.Write(srcbuf.m_buf, ncount);	
}


void CGeneralTools::Jpeg2Rgb24(char *src, char *dest)
{
	CGeneralTools tools;
	TJpegIndex info;
	CFileBin fsrc, fdst;
	CBufferBase srcbuf, dstbuf;
	int size, ncount = 0, width, height, dstfmt;
	
	//parse Jpeg file to get some information:
	tools.JpegParse(src, &info);
	if(info.YUVType == JPEG_UNKNOWN)
		return;	
	width = info.JpgSize.cx;
	height = info.JpgSize.cy;
	dstfmt = YUV422BLK + info.YUVType;

	//Jpeg to Blk:
	fsrc.Open(src);
	fdst.Open(dest, "wb");
	size = fsrc.GetFileLength();
	srcbuf.SetSize(size);
	dstbuf.SetSize(info.vwc << 2);
	if( (srcbuf.GetTotalSize() == 0) && (dstbuf.GetTotalSize() == 0) )
	{
		this->m_dbug.PrintMsg("Can not allocate so big buffer\n");
		return;
	}
	fsrc.Read(srcbuf.m_buf, size);
	size = CJpeg::Jpeg2Blk(srcbuf.m_buf, size, dstbuf.m_buf);
	
	//Blk to Rgb24
	srcbuf.SetSize(height * width * 3); //resize to hold Rgb24 data.
	switch (dstfmt)
	{
	case YUV422BLK:
		ncount = CYuv422::Blk2Rgb24(dstbuf.m_buf, srcbuf.m_buf, width, height);
		break;
	case YUV420BLK:
		ncount = CYuv420::Blk2Rgb24(dstbuf.m_buf, srcbuf.m_buf, width, height);
		break;	
	case YUV411BLK:
		ncount = CYuv411::Blk2Rgb24(dstbuf.m_buf, srcbuf.m_buf, width, height);		
		break;
	case YUV400BLK:
		ncount = CYuv400::Blk2Rgb24(dstbuf.m_buf, srcbuf.m_buf, width, height);		
		break;
	case YUV444BLK:
		ncount = CYuv444::Blk2Rgb24(dstbuf.m_buf, srcbuf.m_buf, width, height);
		break;		
	default:
		break;
	}

	fdst.Write(srcbuf.m_buf, ncount);
}

void CGeneralTools::Jpeg2Bmp(char *src, char *dest)
{
	CGeneralTools tools;
	TJpegIndex info;
	CFileBin fsrc, fdst;
	CBufferBase srcbuf, dstbuf;
	int size, ncount = 0, width, height, dstfmt;
	
	//parse Jpeg file to get some information:
	tools.JpegParse(src, &info);
	if(info.YUVType == JPEG_UNKNOWN)
		return;	
	width = info.JpgSize.cx;
	height = info.JpgSize.cy;
	dstfmt = YUV422BLK + info.YUVType;

	//Jpeg to Blk:
	fsrc.Open(src);
	fdst.Open(dest, "wb");
	size = fsrc.GetFileLength();
	srcbuf.SetSize(size);
	dstbuf.SetSize(info.vwc << 2);
	if( (srcbuf.GetTotalSize() == 0) && (dstbuf.GetTotalSize() == 0) )
	{
		this->m_dbug.PrintMsg("Can not allocate so big buffer\n");
		return;
	}
	fsrc.Read(srcbuf.m_buf, size);
	size = CJpeg::Jpeg2Blk(srcbuf.m_buf, size, dstbuf.m_buf);
	
	//Blk to Rgb24
	srcbuf.SetSize(height * width * 3); //resize to hold Rgb24 data.
	switch (dstfmt)
	{
	case YUV422BLK:
		ncount = CYuv422::Blk2Rgb24(dstbuf.m_buf, srcbuf.m_buf, width, height);
		break;
	case YUV420BLK:
		ncount = CYuv420::Blk2Rgb24(dstbuf.m_buf, srcbuf.m_buf, width, height);
		break;	
	case YUV411BLK:
		ncount = CYuv411::Blk2Rgb24(dstbuf.m_buf, srcbuf.m_buf, width, height);		
		break;
	case YUV400BLK:
		ncount = CYuv400::Blk2Rgb24(dstbuf.m_buf, srcbuf.m_buf, width, height);		
		break;
	case YUV444BLK:
		ncount = CYuv444::Blk2Rgb24(dstbuf.m_buf, srcbuf.m_buf, width, height);
		break;		
	default:
		break;
	}

	//Rgb24 to Bmp
	size = height * width * 3 + sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER);
	dstbuf.SetSize(size); //resize to hold the bmp head content.

 	ncount = CRaw2Rgb::bitrgb2bmp( srcbuf.m_buf, dstbuf.m_buf, width, height, 24);
	fdst.Write(dstbuf.m_buf, ncount);	
}

//***************************************************//

//===============================================
//
//		Image resize
//
//================================================

//***************************************************//
void CGeneralTools::ImageResize(char *src, char *dest, int srcw, int srch, int dstw, 
								int dsth, int fmt, int unit)
{
	if(unit == 0)
	{
		this->m_dbug.PrintMsg("error: unit = 0\n");
		return;
	}
	switch(fmt)
	{
	case YUV422_UYVY:
		CYuv422F::UyvyResize(src, dest, srcw, srch, dstw, dsth, unit);
		break;
	case YUV422_VYUY:
		CYuv422F::VyuyResize(src, dest, srcw, srch, dstw, dsth, unit);
		break;
	case YUV422_YUYV:
		CYuv422F::YuyvResize(src, dest, srcw, srch, dstw, dsth, unit);
		break;
	case YUV422_YVYU:
		CYuv422F::YvyuResize(src, dest, srcw, srch, dstw, dsth, unit);
		break;
	case YUV422_YUVY:
		CYuv422F::YuvyResize(src, dest, srcw, srch, dstw, dsth, unit);
		break;
	case YUV422_YVUY:
		CYuv422F::YvuyResize(src, dest, srcw, srch, dstw, dsth, unit);
		break;
	case YUV422_UVYY:
		CYuv422F::UvyyResize(src, dest, srcw, srch, dstw, dsth, unit);
		break;
	case YUV422_VUYY:
		CYuv422F::VuyyResize(src, dest, srcw, srch, dstw, dsth, unit);
		break;
	case YUV422_YYVU:
		CYuv422F::YyvuResize(src, dest, srcw, srch, dstw, dsth, unit);
		break;
	case RAW_BG:
	case RAW_GB:
	case RAW_RG:
	case RAW_GR:
		break;
	case RGB1:
	case RGB2:
	case RGB4:
	case RGB8:
	case RGB444:
	case RGB666:
	case RGB32:
		break;

	case RGB24:
		CRaw2RgbF::RgbResize(src, dest, srcw, srch, dstw, dsth, unit);
		break;
	case RGB565:
	case RGB555:
		break;
	case YUV422_YYUV:
		CYuv422F::YyuvResize(src, dest, srcw, srch, dstw, dsth, unit);
		break;
	case YUV411_YYYYUV:
		CYuv411F::YyyyuvResize(src, dest, srcw, srch, dstw, dsth, unit);
		break;
	case YUV411_YYYYVU:
		CYuv411F::YyyyvuResize(src, dest, srcw, srch, dstw, dsth, unit);
		break;
	case YUV411_YUYVYY:
		CYuv411F::YuyvyyResize(src, dest, srcw, srch, dstw, dsth, unit);
		break;
	case YUV411_YVYUYY:
		CYuv411F::YvyuyyResize(src, dest, srcw, srch, dstw, dsth, unit);
		break;
	case YUV411_UVYYYY:
		CYuv411F::UvyyyyResize(src, dest, srcw, srch, dstw, dsth, unit);
		break;
	case YUV411_VUYYYY:
		CYuv411F::VuyyyyResize(src, dest, srcw, srch, dstw, dsth, unit);
		break;
	case YUV420_UYVY:
		CYuv420F::UyvyResize(src, dest, srcw, srch, dstw, dsth, unit);
		break;
	case YUV420_VYUY:
		CYuv420F::VyuyResize(src, dest, srcw, srch, dstw, dsth, unit);
		break;
	case YUV420_YUYV:
		CYuv420F::YuyvResize(src, dest, srcw, srch, dstw, dsth, unit);
		break;
	case YUV420_YVYU:
		CYuv420F::YvyuResize(src, dest, srcw, srch, dstw, dsth, unit);
		break;
	case YUV420_YUVY:
		CYuv420F::YuyvResize(src, dest, srcw, srch, dstw, dsth, unit);
		break;
	case YUV420_YVUY:
		CYuv420F::YvuyResize(src, dest, srcw, srch, dstw, dsth, unit);
		break;
	case YUV420_UVYY:
		CYuv420F::UvyyResize(src, dest, srcw, srch, dstw, dsth, unit);
		break;
	case YUV420_VUYY:
		CYuv420F::VuyyResize(src, dest, srcw, srch, dstw, dsth, unit);
		break;
	case YUV420_YYVU:
		CYuv420F::YyvuResize(src, dest, srcw, srch, dstw, dsth, unit);
		break;
	case YUV420_YYUV:
		CYuv420F::YyuvResize(src, dest, srcw, srch, dstw, dsth, unit);
		break;
	case YUV400:
		CYuv400F::Yuv400Resize(src, dest, srcw, srch, dstw, dsth, unit);
		break;
	case YUV444:
		CYuv444F::Yuv444Resize(src, dest, srcw, srch, dstw, dsth, unit);
		break;
	case YUV422BLK:
	case YUV411BLK:
	case YUV420BLK:
	case YUV400BLK:
	case YUV444BLK:
	case JPEG422:
	case JPEG420:
	case JPEG411:
	case JPEG400:
	case JPEG444:
	default:
		break;
	}
}


//***************************************************//

//===============================================
//
//		Image Sizer
//
//================================================

//***************************************************//
void CGeneralTools::ImageSizer(char *src, char *dest, int srcw, int srch, int dstw, 
								int dsth, int px, int py, int fmt, int unit)
{
	if(unit == 0)
	{
		this->m_dbug.PrintMsg("error: unit = 0\n");
		return;
	}
	switch(fmt)
	{
	case YUV422_UYVY:
		CYuv422F::UyvySizer(src, dest, srcw, srch, dstw, dsth, px, py, unit);
		break;
	case YUV422_VYUY:
		CYuv422F::VyuySizer(src, dest, srcw, srch, dstw, dsth, px, py, unit);
		break;
	case YUV422_YUYV:
		CYuv422F::YuyvSizer(src, dest, srcw, srch, dstw, dsth, px, py, unit);
		break;
	case YUV422_YVYU:
		CYuv422F::YvyuSizer(src, dest, srcw, srch, dstw, dsth, px, py, unit);
		break;
	case YUV422_YUVY:
		CYuv422F::YuvySizer(src, dest, srcw, srch, dstw, dsth, px, py, unit);
		break;
	case YUV422_YVUY:
		CYuv422F::YvuySizer(src, dest, srcw, srch, dstw, dsth, px, py, unit);
		break;
	case YUV422_UVYY:
		CYuv422F::UvyySizer(src, dest, srcw, srch, dstw, dsth, px, py, unit);
		break;
	case YUV422_VUYY:
		CYuv422F::VuyySizer(src, dest, srcw, srch, dstw, dsth, px, py, unit);
		break;
	case YUV422_YYVU:
		CYuv422F::YyvuSizer(src, dest, srcw, srch, dstw, dsth, px, py, unit);
		break;
	case RAW_BG:
	case RAW_GB:
	case RAW_RG:
	case RAW_GR:
		break;
	case RGB1:
	case RGB2:
	case RGB4:
	case RGB8:
	case RGB444:
	case RGB666:
	case RGB32:
		break;

	case RGB24:
		CRaw2RgbF::RgbSizer(src, dest, srcw, srch, dstw, dsth, px, py, unit);
		break;
	case RGB565:
	case RGB555:
		break;
	case YUV422_YYUV:
		CYuv422F::YyuvSizer(src, dest, srcw, srch, dstw, dsth, px, py, unit);
		break;
	case YUV411_YYYYUV:
		CYuv411F::YyyyuvSizer(src, dest, srcw, srch, dstw, dsth, px, py, unit);
		break;
	case YUV411_YYYYVU:
		CYuv411F::YyyyvuSizer(src, dest, srcw, srch, dstw, dsth, px, py, unit);
		break;
	case YUV411_YUYVYY:
		CYuv411F::YuyvyySizer(src, dest, srcw, srch, dstw, dsth, px, py, unit);
		break;
	case YUV411_YVYUYY:
		CYuv411F::YvyuyySizer(src, dest, srcw, srch, dstw, dsth, px, py, unit);
		break;
	case YUV411_UVYYYY:
		CYuv411F::UvyyyySizer(src, dest, srcw, srch, dstw, dsth, px, py, unit);
		break;
	case YUV411_VUYYYY:
		CYuv411F::VuyyyySizer(src, dest, srcw, srch, dstw, dsth, px, py, unit);
		break;
	case YUV420_UYVY:
//		CYuv420F::UyvySizer(src, dest, srcw, srch, dstw, dsth, px, py, unit);
		break;
	case YUV420_VYUY:
//		CYuv420F::VyuySizer(src, dest, srcw, srch, dstw, dsth, px, py, unit);
		break;
	case YUV420_YUYV:
//		CYuv420F::YuyvSizer(src, dest, srcw, srch, dstw, dsth, px, py, unit);
		break;
	case YUV420_YVYU:
//		CYuv420F::YvyuSizer(src, dest, srcw, srch, dstw, dsth, px, py, unit);
		break;
	case YUV420_YUVY:
//		CYuv420F::YuyvSizer(src, dest, srcw, srch, dstw, dsth, px, py, unit);
		break;
	case YUV420_YVUY:
//		CYuv420F::YvuySizer(src, dest, srcw, srch, dstw, dsth, px, py, unit);
		break;
	case YUV420_UVYY:
//		CYuv420F::UvyySizer(src, dest, srcw, srch, dstw, dsth, px, py, unit);
		break;
	case YUV420_VUYY:
//		CYuv420F::VuyySizer(src, dest, srcw, srch, dstw, dsth, px, py, unit);
		break;
	case YUV420_YYVU:
//		CYuv420F::YyvuSizer(src, dest, srcw, srch, dstw, dsth, px, py, unit);
		break;
	case YUV420_YYUV:
//		CYuv420F::YyuvSizer(src, dest, srcw, srch, dstw, dsth, px, py, unit);
		break;
	case YUV400:
		CYuv400F::Yuv400Sizer(src, dest, srcw, srch, dstw, dsth, px, py, unit);
		break;
	case YUV444:
		CYuv444F::Yuv444Sizer(src, dest, srcw, srch, dstw, dsth, px, py, unit);
		break;
	case YUV422BLK:
	case YUV411BLK:
	case YUV420BLK:
	case YUV400BLK:
	case YUV444BLK:
	case JPEG422:
	case JPEG420:
	case JPEG411:
	case JPEG400:
	case JPEG444:
	default:
		break;
	}
}

void CGeneralTools::ImagePrefilter(char *src, char *dest, int width, int height, int count, 
								int *filter, int fmt, int unit)
{
	if(unit == 0)
	{
		this->m_dbug.PrintMsg("error: unit = 0\n");
		return;
	}
	switch(fmt)
	{
	case YUV422_UYVY:
		CYuv422F::UyvyPrefilter(src, dest, width, height, count, filter, unit);
		break;
	case YUV422_VYUY:
		CYuv422F::VyuyPrefilter(src, dest, width, height, count, filter, unit);
		break;
	case YUV422_YUYV:
		CYuv422F::YuyvPrefilter(src, dest, width, height, count, filter, unit);
		break;
	case YUV422_YVYU:
		CYuv422F::YvyuPrefilter(src, dest, width, height, count, filter, unit);
		break;
	case YUV422_YUVY:
		CYuv422F::YuvyPrefilter(src, dest, width, height, count, filter, unit);
		break;
	case YUV422_YVUY:
		CYuv422F::YvuyPrefilter(src, dest, width, height, count, filter, unit);
		break;
	case YUV422_UVYY:
		CYuv422F::UvyyPrefilter(src, dest, width, height, count, filter, unit);
		break;
	case YUV422_VUYY:
		CYuv422F::VuyyPrefilter(src, dest, width, height, count, filter, unit);
		break;
	case YUV422_YYVU:
		CYuv422F::YyvuPrefilter(src, dest, width, height, count, filter, unit);
		break;
	case RAW_BG:
	case RAW_GB:
	case RAW_RG:
	case RAW_GR:
		break;
	case RGB1:
	case RGB2:
	case RGB4:
	case RGB8:
	case RGB444:
	case RGB666:
	case RGB32:
		break;

	case RGB24:
		CRaw2RgbF::RgbPrefilter(src, dest, width, height, count, filter, unit);
		break;
	case RGB565:
	case RGB555:
		break;
	case YUV422_YYUV:
		CYuv422F::YyuvPrefilter(src, dest, width, height, count, filter, unit);
		break;
	case YUV411_YYYYUV:
		CYuv411F::YyyyuvPrefilter(src, dest, width, height, count, filter, unit);
		break;
	case YUV411_YYYYVU:
		CYuv411F::YyyyvuPrefilter(src, dest, width, height, count, filter, unit);
		break;
	case YUV411_YUYVYY:
		CYuv411F::YuyvyyPrefilter(src, dest, width, height, count, filter, unit);
		break;
	case YUV411_YVYUYY:
		CYuv411F::YvyuyyPrefilter(src, dest, width, height, count, filter, unit);
		break;
	case YUV411_UVYYYY:
		CYuv411F::UvyyyyPrefilter(src, dest, width, height, count, filter, unit);
		break;
	case YUV411_VUYYYY:
		CYuv411F::VuyyyyPrefilter(src, dest, width, height, count, filter, unit);
		break;
	case YUV420_UYVY:
		break;
	case YUV420_VYUY:
		break;
	case YUV420_YUYV:
		break;
	case YUV420_YVYU:
		break;
	case YUV420_YUVY:
		break;
	case YUV420_YVUY:
		break;
	case YUV420_UVYY:
		break;
	case YUV420_VUYY:
		break;
	case YUV420_YYVU:
		break;
	case YUV420_YYUV:
		break;
	case YUV400:
		CYuv400F::Yuv400Prefilter(src, dest, width, height, count, filter, unit);
		break;
	case YUV444:
		CYuv444F::Yuv444Prefilter(src, dest, width, height, count, filter, unit);
		break;
	case YUV422BLK:
	case YUV411BLK:
	case YUV420BLK:
	case YUV400BLK:
	case YUV444BLK:
	case JPEG422:
	case JPEG420:
	case JPEG411:
	case JPEG400:
	case JPEG444:
	default:
		break;
	}
}

int CGeneralTools::UnitConvert(int val, int srcfmt, int dstfmt)
{
	int val1 = ERROR_VALUE;

	if(srcfmt == dstfmt)
		return val;

	switch(srcfmt)
	{
	case RGB444:
		if(dstfmt == RGB24)
			CRaw2Rgb::Rgb444_Rgb24(val, (char *)&val1);
		break;
	case RGB666:
		if(dstfmt == RGB24)
			CRaw2Rgb::Rgb666_Rgb24(val, (char *)&val1);
		break;
	case RGB32:
		if(dstfmt == RGB24)
			CRaw2Rgb::Rgb32_Rgb24((char *)&val, (char *)&val1);
		break;
	case RGB565:
		if(dstfmt == RGB24)
			CRaw2Rgb::Rgb565_Rgb24((char *)&val, (char *)&val1);
		break;
	case RGB555:
		if(dstfmt == RGB24)
			CRaw2Rgb::Rgb555_Rgb24((char *)&val, (char *)&val1);
		break;
	case RGB24:
		switch(dstfmt)
		{
		case RGB32:
			CRaw2Rgb::Rgb24_Rgb32((char *)&val, (char *)&val1);
			break;
		case YUV444:
			val1 = CYuv2Rgb::bgr2yuv(val);
			break;
		default:
			break;
		}
		break;
	case YUV444:
		if(dstfmt == RGB24)
			val1 = CYuv2Rgb::yuv2bgr(val);
		break;
	default:
		break;
	}
	return val1;
}

//***************************************************//

//===============================================
//
//		Jpeg convert
//
//================================================

//***************************************************//
void CGeneralTools::JpegConvert(char *src, char *dest, int dstfmt)
{
	//CJpegF::Jpeg2Blk(src, dest);

	switch(dstfmt)
	{
	case YUV422_UYVY:
		break;
	case YUV422_VYUY:
		break;
	case YUV422_YUYV:
		break;
	case YUV422_YVYU:
		break;
	case YUV422_YUVY:
		break;
	case YUV422_YVUY:
		break;
	case YUV422_UVYY:
		break;
	case YUV422_VUYY:
		break;
	case YUV422_YYVU:
		break;
	case YUV422_YYUV:
		break;
	case RAW_BG:
	case RAW_GB:
	case RAW_RG:
	case RAW_GR:
		break;
	case RGB1:
	case RGB2:
	case RGB4:
	case RGB8:
	case RGB444:
	case RGB666:
	case RGB32:
		break;
	case RGB24:
		this->Jpeg2Rgb24(src, dest);
		break;
	case RGB565:
	case RGB555:
		break;
	case YUV411_YYYYUV:
	case YUV411_YYYYVU:
	case YUV411_YUYVYY:
	case YUV411_YVYUYY:
	case YUV411_UVYYYY:
	case YUV411_VUYYYY:
	case YUV420_UYVY:
	case YUV420_VYUY:
	case YUV420_YUYV:
	case YUV420_YVYU:
	case YUV420_YUVY:
	case YUV420_YVUY:
	case YUV420_UVYY:
	case YUV420_VUYY:
	case YUV420_YYVU:
		break;

	case YUV420_YYUV:
		break;
	case YUV400:
		break;
	case YUV444:
		break;
	case YUV422BLK:
	case YUV411BLK:
	case YUV420BLK:
	case YUV400BLK:
	case YUV444BLK:
		CJpegF::Jpeg2Blk(src, dest);
		break;
	case BMP:
		this->Jpeg2Bmp(src, dest);
		break;
	case JPEG422:
		break;
	case JPEG420:
	case JPEG411:
	case JPEG400:
	case JPEG444:
	default:
		break;
	}	
}

//***************************************************//

//===============================================
//
//		Mpeg2 I frame convert
//
//================================================

//***************************************************//
void CGeneralTools::Mpeg2Convert(char *src, char *dest)
{
	CMpeg2IF::Mpeg2Blk(src, dest);
}


/***************************************************/

//===============================================
//
//		Mpeg4 frame convert
//
//================================================

//***************************************************//
void CGeneralTools::Mpeg4Convert(char *src, char *dest)
{
	CMpeg4F::Mpeg4Blk(src, dest);
}


/***************************************************/

//===============================================
//
//		YUV to CCIR601/656 convert
//
//================================================



//***************************************************//

//===============================================
//
//		Image Rotation
//
//================================================

//***************************************************//
void CGeneralTools::ImageRotate(char *src, char *dest, int width, int height, int fmt, int rotate, int unit)
{
	if(unit == 0)
	{
		this->m_dbug.PrintMsg("error: unit = 0\n");
		return;
	}
	switch(fmt)
	{
	case YUV422_UYVY:
	case YUV422_VYUY:
	case YUV422_YUYV:
	case YUV422_YVYU:
	case YUV422_YUVY:
	case YUV422_YVUY:
	case YUV422_UVYY:
	case YUV422_VUYY:
	case YUV422_YYVU:
		break;
	case RAW_BG:
	case RAW_GB:
	case RAW_RG:
	case RAW_GR:
		break;
	case RGB1:
	case RGB2:
	case RGB4:
	case RGB8:
	case RGB444:
	case RGB666:
	case RGB32:
		break;

	case RGB24:
		//this->Rgb24Rotate(src, dest, width, height, rotate, unit);
		CRaw2RgbF::Rgb24Rotate(src, dest, width, height, rotate, unit);
		break;
	case RGB565:
	case RGB555:
		break;
	case YUV422_YYUV:
	case YUV411_YYYYUV:
	case YUV411_YYYYVU:
	case YUV411_YUYVYY:
	case YUV411_YVYUYY:
	case YUV411_UVYYYY:
	case YUV411_VUYYYY:
	case YUV420_UYVY:
	case YUV420_VYUY:
	case YUV420_YUYV:
	case YUV420_YVYU:
	case YUV420_YUVY:
	case YUV420_YVUY:
	case YUV420_UVYY:
	case YUV420_VUYY:
	case YUV420_YYVU:
	case YUV420_YYUV:
		break;
	case YUV400:
		break;
	case YUV444:
		CYuv444F::Yuv444Rotate(src, dest, width, height, rotate, unit);
		break;
	case YUV422BLK:
	case YUV411BLK:
	case YUV420BLK:
	case YUV400BLK:
	case YUV444BLK:
	case JPEG422:
	case JPEG420:
	case JPEG411:
	case JPEG400:
	case JPEG444:
	default:
		break;
	}
}


