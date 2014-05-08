#ifndef _VJPEG_H_
#define _VJPEG_H_

#ifdef __cplusplus
extern "C" {
#endif


#ifndef JPEGFAILED
#define JPEGFAILED		0xff
#endif

#ifndef SUCCEED
#define SUCCEED		0
#endif


//#define DEFAULT_THUMB_SIZE		0x1FFF
//#define DEFAULT_JPEG_SIZE		0x7FFF





#define JBUF_BASEADD    0x20000
#define LBUF0BASEADR	0x8000
#define LBUF1BASEADR	0xc000

#define LBUF_UNITSIZE	0x800

#define DEFAULT_SRC_ADR			0
#define DEFAULT_DEST_ADR		0X8000
#define DEFAULT_SRC_SIZE		0x8000
#define DEFAULT_DEST_SIZE		0x8000

#define DEFAULT_THUMB_ADDR		0xe000
#define	DEFAULT_THUMB_SIZE		0x2000
#define TOTAL_JBUF_SIZE			0x10000

#define DEFAULT_TWC				0x1000

#define V558_JPEG_NOTINIT		0
#define V558_JPEG_INITED		0X1
#define V558_JPEG_ISWORKING		2





#define DEFAULT_BRCPARM		0x111

void V558_JpegSetBrcParm(UINT16 x);
UINT16 V558_JpegGetBrcParm(void);

enum
{
	V558_YUV_422 = 0,
	V558_YUV_420,
	V558_YUV_411,
	V558_YUV_400,
	V558_PYUV_422_YYUV,
	V558_PYUV_422_YUYV,
	V558_PYUV_422_UYVY,
	V558_YUV_UNKNOWN
};
enum
{
	V558_RGB_888  = 0,
	V558_RGB_565  = 1,
	V558_RGB_555  = 2,
	V558_RGB_UNKNOWN
};
enum
{
	V558_DATATYPE_YYUV    =  0,
	V558_DATATYPE_YUYV    =  1,
	V558_DATATYPE_UYVY    =  2	
};

enum
{
	V558_ROTATION_90          = 0,
	V558_ROTATION_180         = 1,
	V558_ROTATION_270         = 2
};
enum
{
	V558_PARTCOLOR_RED        = 0,
	V558_PARTCOLOR_GREEN      = 1,
	V558_PARTCOLOR_BLUE       = 2
};

#ifdef __cplusplus
}
#endif

#endif
