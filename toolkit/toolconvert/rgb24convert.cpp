

#include "internal.h"

int CRgb24Convert::Rgb24_Img(char *src, char *dest, int width, int height, int fmt)
{
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
	case YUV422_YYUV:
		width = CYuv422::MRgb2Yuv422order(src, dest, width, height, fmt);
		break;
	case RAW_BG:
	case RAW_GB:
	case RAW_RG:
	case RAW_GR:
		width = CRaw2Rgb::rgb2bayerorder(src, dest, fmt, width, height);
		break;
	case YUV400:
		width = CYuv400::MRgb2Yuv400(src, dest, width, height);
		break;
	case YUV444:
		width = CYuv444::MRgb2Yuv444(src, dest, width, height);
		break;
	case RGB24:
		width *= height * 3;
		memcpy(dest, src, width);
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
		width = CYuv420::MRgb2Yuv420order(src, dest, width, height, fmt);
		break;

	case YUV411_YYYYUV:
	case YUV411_YYYYVU:
	case YUV411_YUYVYY:
	case YUV411_YVYUYY:
	case YUV411_UVYYYY:
	case YUV411_VUYYYY:
		width = CYuv411::MRgb2Yuv411order(src, dest, width, height, fmt);
		break;

	default:
		width = 0;
		break;
	}
	return width;
}

int CRgb24Convert::Img2Rgb24(char *src, char *dest, int width, int height, int fmt)
{
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
	case YUV422_YYUV:
		width = CYuv422::MYuv422_Rgborder(src, dest, width, height, fmt);
		break;
	case RAW_BG:
	case RAW_GB:
	case RAW_RG:
	case RAW_GR:
		width = CRaw2Rgb::bayer2rgborder(src, dest, fmt, width, height);
		break;
	case YUV400:
		width = CYuv400::MYuv400_Rgb(src, dest, width, height);
		break;
	case YUV444:
		width = CYuv444::MYuv444_Rgb(src, dest, width, height);
		break;
	case RGB24:
		width *= height * 3;
		memcpy(dest, src, width);
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
		width = CYuv420::MYuv420_Rgborder(src, dest, width, height, fmt);
		break;

	case YUV411_YYYYUV:
	case YUV411_YYYYVU:
	case YUV411_YUYVYY:
	case YUV411_YVYUYY:
	case YUV411_UVYYYY:
	case YUV411_VUYYYY:
		width = CYuv411::MYuv411_Rgborder(src, dest, width, height, fmt);
		break;

	default:
		width = 0;
		break;
	}
	return width;
}
