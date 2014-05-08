

#ifndef _CMODEL_GLOBAL_H_
#define _CMODEL_GLOBAL_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#ifndef SINT8
#define SINT8 char
#endif

//unsigned 8-bit integer
#ifndef UINT8
#define UINT8 unsigned char
#endif

//signed 16-bit integer
#ifndef SINT16
#define SINT16 short
#endif

//unsigned 16-bit integer
#ifndef UINT16
#define UINT16 unsigned short
#endif

//signed 32-bit integer
#ifndef SINT32
#define SINT32 long
#endif

//unsigned 32-bit integer
#ifndef UINT32
#define UINT32 unsigned long
#endif

#ifndef BITDEF
#define BITDEF
enum BITDEFINEenum
{
	BIT0	=	0x1,
	BIT1	=	0x2,
	BIT2	=	0x4,
	BIT3	=	0x8,
	BIT4	=	0x10,
	BIT5	=	0x20,
	BIT6	=	0x40,
	BIT7	=	0x80,
	BIT8	=	0x100,
	BIT9	=	0x200,
	BIT10	=	0x400,
	BIT11	=	0x800,
	BIT12	=	0x1000,
	BIT13	=	0x2000,
	BIT14	=	0x4000,
	BIT15	=	0x8000,
	BIT16	=	0x10000,
	BIT17	=	0x20000,
	BIT18	=	0x40000,
	BIT19	=	0x80000,
	BIT20	=	0x100000,
	BIT21	=	0x200000,
	BIT22	=	0x400000,
	BIT23	=	0x800000,
	BIT24	=	0x1000000,
	BIT25	=	0x2000000,
	BIT26	=	0x4000000,
	BIT27	=	0x8000000,
	BIT28	=	0x10000000,
	BIT29	=	0x20000000,
	BIT30	=	0x40000000,
	BIT31	=	0x80000000
};
#endif

#ifndef SUCCEED
#define SUCCEED	0
#endif

#ifndef FAILED
#define FAILED	0xff
#endif

#ifndef TRUE
#define TRUE	0xff
#endif

#ifndef FALSE
#define FALSE	0
#endif

#ifndef NULL
#define  NULL	0
#endif


enum FLOWenum
{
	FLOW_NORMAL,
	FLOW_FRAME,
	FLOW_ERROR	= 0xff
};

enum SIGNALenum
{
	VSTART_PERIOD,
	VSYNC_PERIOD,
	INSERT_BLANK,
	VEND_PERIOD,
	INVAILD_PERIOD
};

#define FRAME_START		1
#define FRAME_STOP		0

enum STREAMenum
{
	UYVY,			//0x0
	VYUY,			//0x1
	YUYV,			//0x2
	YVYU,			//0x3

	RAW_BG,			//0x4
	RAW_GB,			//0x5
	RAW_RG,			//0x6
	RAW_GR,			//0x7

	RAWRGB,			//0x8
	
	YUV422,			//0x9
	YUV411,			//0xa
	YUV420,			//0xb
	YUV400,			//0xc
	YUV444,			//0xd

	YUV422BLK,		//0xe
	YUV411BLK,		//0xf
	YUV420BLK,		//0x10
	YUV400BLK,		//0x11
	YUV444BLK,		//0x12

	JPEGDATA		//0x13
};

typedef struct tag_TRaw8
{
	UINT8	val;
} TRaw8, *pTRaw8;

typedef struct tag_TRaw10
{
	UINT16	val;
} TRaw10, *pTRaw10;

typedef struct tag_TRgb24
{
	UINT8	b;
	UINT8	g;
	UINT8	r;
} TRgb24, *pTRgb24;

typedef struct tag_TYuv444
{
	UINT8	u;
	UINT8	y;
	UINT8	v;
} TYuv444, *pTYuv444;

typedef struct tag_TYuv422
{
	UINT8	uv;
	UINT8	y;
} TYuv422, *pTYuv422;

typedef struct tag_TSize
{
	int	width;
	int	height;
} TSize, *pTSize;

typedef struct tag_TPoint
{
	int	x;
	int	y;
} TPoint, *pTPoint;

typedef struct tag_TStream
{
	union {
		SINT8		val8;
		UINT8		uval8;
		SINT16		val16;
		UINT16		uval16;
		SINT32		val32;
		UINT32		uval32;
		TRaw8		raw8;
		TRaw10		raw10;
		TRgb24		rgb24;
		TYuv444		yuv444;
		TYuv422		yuv422;
		TYuv422		uyvy;
		TYuv422		vyuy;
		TYuv422		yuyv;
		TYuv422		yvyu;
	};
	int flag;
} TStream, *PTStream;

enum TSTREAMTYPEenum
{
	SINT8_TYPE,
	UINT8_TYPE,
	SINT16_TYPE,
	UINT16_TYPE,
	SINT32_TYPE,
	UINT32_TYPE,
	TRAW8_TYPE,
	TRAW10_TYPE,
	TRGB24_TYPE,
	TYUV444_TYPE,
	TYUV422_TYPE,

	UYVY_TYPE,
	VYUY_TYPE,
	YUYV_TYPE,
	YVYU_TYPE,

	TOTAL_STREAM_TYPE,

	STREAM_TYPE	=	0xff,

	VSYNC_FLAG	=	BIT12,
	HSYNC_FLAG	=	BIT13,
	VREF_FLAG	=	BIT14,
	HREF_FLAG	=	BIT15
};

typedef struct tag_TYuv422Block
{
	UINT8	y0[64];
	UINT8	y1[64];
	UINT8	u[64];
	UINT8	v[64];
} TYuv422Block, *pTYuv422Block;

#endif


