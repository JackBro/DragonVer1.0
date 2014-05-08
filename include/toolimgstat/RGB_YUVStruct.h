#ifndef __TOOLIMGSTAT_RGB_YUVSTRUCT_H__
#define __TOOLIMGSTAT_RGB_YUVSTRUCT_H__


enum outputYUVMSGenum
{
	MAXY, MINY, AVGY, MAXU, MINU, AVGU, MAXV, MINV, AVGV
};

enum outputRGBMSGenum
{
	MAXR, MINR, AVGR, MAXG, MING, AVGG, MAXB, MINB, AVGB
};

const int YUVDetachStream[][6] = 
{
	{ 2, 0, 3, 1 },			//YUV422 UYVY
	{ 3, 0, 2, 1 },			//YUV422 VYUY
	{ 0, 2, 1, 3 },			//YUV422 YUYV
	{ 0, 3, 1, 2 },			//YUV422 YVYU
	{ 0, 2, 3, 1 },			//YUV422 YUVY

	{ 0, 3, 2, 1 },			//YUV422 YVUY
	{ 2, 3, 0, 1 },			//YUV422 UVYY
	{ 3, 2, 0, 1 },			//YUV422 VUYY
	{ 0, 1, 3, 2 },			//YUV422 YYVU
	{ 0, 1, 2, 3 },			//YUV422 YYUV

	{ 1, 0 },				//RAW_BG
	{ 1, 1 },				//RAW_GB
	{ 0, 0 },				//RAW_RG
	{ 0, 1 },				//RAW_GR

	{ 0, 1, 2 },			//RAW_RGB,
	{ 0, 2, 1 },			//RAW_RBG,
	{ 1, 2, 0 },			//RAW_GBR,
	{ 1, 0, 2 },			//RAW_GRB,
	{ 2, 1, 0 },			//RAW_BGR,
	{ 2, 0, 1 },			//RAW_BRG,

	{ 2, 0, 3, 1 },			//YUV420 UYVY
	{ 3, 0, 2, 1 },			//YUV420 VYUY
	{ 0, 2, 1, 3 },			//YUV420 YUYV
	{ 0, 3, 1, 2 },			//YUV420 YVYU
	{ 0, 2, 3, 1 },			//YUV420 YUVY

	{ 0, 3, 2, 1 },			//YUV420 YVUY
	{ 2, 3, 0, 1 },			//YUV420 UVYY
	{ 3, 2, 0, 1 },			//YUV420 VUYY
	{ 0, 1, 3, 2 },			//YUV420 YYVU
	{ 0, 1, 2, 3 },			//YUV420 YYUV

	{ 0, 3, 6, 9, 1, 2 },	//YUV411_YYYYUV
	{ 0, 3, 6, 9, 2, 1 },	//YUV411_YYYYVU,
	{ 0, 1, 3, 2, 6, 9 },	//YUV411_YUYVYY,
	{ 0, 2, 3, 1, 6, 9 },	//YUV411_YVYUYY,
	{ 1, 2, 0, 3, 6, 9 },	//YUV411_UVYYYY,
	{ 2, 1, 0, 3, 6, 9 },	//YUV411_VUYYYY,

	{ 0 },					//YUV400
	{ 0 },					//YUV444

	{ 0 },					//RGB1
	{ 0 },					//RGB2
	{ 0 },					//RGB4
	{ 0 },					//RGB8
	{ 0 },					//RGB444
	{ 0 },					//RGB555
	{ 0 },					//RGB565
	{ 0 },					//RGB666
	{ 0 },					//RGB24
	{ 0 },					//RGB32

	{ 0 },					//YUV422M2BLK
	{ 0 },					//YUV422BLK
	{ 0 },					//YUV420BLK
	{ 0 },					//YUV411BLK
	{ 0 },					//YUV400BLK
	{ 0 },					//YUV444BLK
	{ 0 },					//YUV444M2BLK

	{ 0 },					//YUV422LBUF
	{ 0 },					//YUV420LBUF
	{ 0 },					//YUV411LBUF
	{ 0 },					//YUV400LBUF
	{ 0 },					//YUV444LBUF

	{ 0 },					//BMP
	{ 0 },					//JPEG422
	{ 0 },					//JPEG420
	{ 0 },					//JPEG411
	{ 0 },					//JPEG400
	{ 0 },					//JPEG444

	{ 0 }, 					//RAW_R,
	{ 1 },					//RAW_G,
	{ 2 },					//RAW_B,
	{ 0 },					//RAW_Y,
	{ 0 },					//RAW_U,
	{ 0 },					//RAW_V,
	{ 0 }, 					//YUV422FRM,
	{ 0 }, 					//YUV420FRM,
	{ 0 },					//YUV411FRM,
	{ 0 },					//YUV400FRM,
	{ 0 },					//YUV444FRM

	{ 0 }, 					//MPEG1I_FRM,
	{ 0 },					//MPEG2IFRM420,
	{ 0 },					//MPEG2IFRM422,
	{ 0 },					//MPEG2IFRM444,
	{ 0 },					//MPEG4I_FRM,
	{ 0 }					//EDGE_MAP
};
#endif
