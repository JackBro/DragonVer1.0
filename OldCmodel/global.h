

#ifndef _CMODEL_GLOBAL_H_
#define _CMODEL_GLOBAL_H_
#include "ex_OldCmodel.h"


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#ifndef SINT8
#define SINT8 signed char
#endif

//unsigned 8-bit integer
#ifndef UINT8
#define UINT8 unsigned char
#endif

//signed 16-bit integer
#ifndef SINT16
#define SINT16 signed short
#endif

//unsigned 16-bit integer
#ifndef UINT16
#define UINT16 unsigned short
#endif

//signed 32-bit integer
#ifndef SINT32
#define SINT32 signed long
#endif

//unsigned 32-bit integer
#ifndef UINT32
#define UINT32 unsigned long
#endif


#define BIT0		0x1
#define BIT1		0x2
#define BIT2		0x4
#define BIT3		0x8
#define BIT4		0x10
#define BIT5		0x20
#define BIT6		0x40
#define BIT7		0x80
#define BIT8		0x100
#define BIT9		0x200
#define BIT10		0x400
#define BIT11		0x800
#define BIT12		0x1000
#define BIT13		0x2000
#define BIT14		0x4000
#define BIT15		0x8000

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

enum {
	FLOW_NORMAL,
//	FLOW_DONONE,
	FLOW_FRAME,
	FLOW_ERROR	= 0xff
};

enum {
	VSTART_PERIOD,
	VSYNC_PERIOD,
	INSERT_BLANK,
	VEND_PERIOD,
	INVAILD_PERIOD
};

#define FRAME_START		1
#define FRAME_STOP		0

enum {
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

typedef struct tag_RawRgbType
{
	SINT16	b;
	SINT16	g;
	SINT16	r;
} RawRgbType, *pRawRgbType;

typedef struct tag_Yuv444Type
{
	UINT8	u;
	UINT8	y;
	UINT8	v;
	UINT8	res;
} Yuv444Type, *pYuv444Type;

typedef struct tag_Yuv422Type
{
	UINT8	uv;
	UINT8	y;
//	UINT8	v;
//	UINT8	y2;
} Yuv422Type, *pYuv422Type;

typedef struct tag_Yuv422BlockType
{
	UINT8	y0[64];
	UINT8	y1[64];
	UINT8	u[64];
	UINT8	v[64];
} Yuv422BlockType, *pYuv422BlockType;

typedef struct tag_SimSize
{
	UINT16	width;
	UINT16	height;
} SimSize, *pSimSize;

typedef struct tag_SimPoint
{
	UINT16	x;
	UINT16	y;
} SimPoint, *pSimPoint;


//===============================================
#ifndef _BMPDEFINED_

#pragma pack(1)
typedef struct tag_RGBQUAD 
{ 
	UINT8 rgbBlue;
	UINT8 rgbGreen;
	UINT8 rgbRed;
	UINT8 rgbReserved;
} RGBQUAD, *PRGBQUAD;

typedef struct tag_MV_BITMAPHEADER
{
	UINT16	headSize;
	UINT16	begin;
	UINT16	dataSize;
	UINT16	width, height;
	UINT8	bitspixel;
	UINT8	reserver;
} MV_BITMAPHEADER, *PMV_BITMAPHEADER;

typedef struct tag_BITMAPFILEHEADER 
{
	UINT16	bfType; 
	UINT32	bfSize; 
	UINT16	bfReserved1; 
	UINT16	bfReserved2; 
	UINT32	bfOffBits; 
} BITMAPFILEHEADER, *PBITMAPFILEHEADER;

#define BI_RGB        0L
#define BI_RLE8       1L
#define BI_RLE4       2L
#define BI_BITFIELDS  3L

typedef struct tag_BITMAPINFOHEADER
{ 
	UINT32	biSize; 
	SINT32	biWidth; 
	SINT32	biHeight; 
	UINT16	biPlanes; 
	UINT16	biBitCount; 
	UINT32	biCompression; 
	UINT32	biSizeImage; 
	SINT32	biXPelsPerMeter; 
	SINT32	biYPelsPerMeter; 
	UINT32	biClrUsed; 
	UINT32	biClrImportant; 
} BITMAPINFOHEADER, *PBITMAPINFOHEADER; 

#pragma pack()

#endif
//=================================================



#define MAX_IMAGEWIDTH		6000

//=============== class ImageBase =================

class OLDCMODEL_DLL CImageBase
{
public:
	CImageBase(int size = MAX_IMAGEWIDTH);
	virtual ~CImageBase(void);
	int		IsValid(void);
	void	Clear(void);

public:
	union
	{
		UINT8		*buf8;
		SINT16		*buf16;
		RawRgbType	*rgbBuf;
		Yuv422Type	*yuvBuf;
		Yuv444Type	*yuv444;
		Yuv422BlockType	*yuv422B;
	};

	int		width;					//image total width
	int		height;
	int		vs;						//vs = 1 means vsnyc is occured this line
	int		format;

protected:
	int		InitStream(int size);
	void	ReleaseStream(void);
	void	ClearInfo(void);

private:
	int		gState;
	int		gSize;
};


//=============== class ImageStream =================

class OLDCMODEL_DLL ImageStream : public CImageBase
{
public:
	ImageStream(int size = MAX_IMAGEWIDTH);
};


//=============== class ImageBlock =================

class OLDCMODEL_DLL ImageBlock : public CImageBase
{
public:
	ImageBlock(int size = MAX_IMAGEWIDTH);
};




//=============== class CImageTools =================

class OLDCMODEL_DLL CImageTools
{
public:
	void	yuv422Torgb(pYuv422Type yuv, pRawRgbType rgb);
	void	rgbToyuv422(pRawRgbType rgb, pYuv422Type yuv);
	int		MaxDivisor(int a, int b);
	void	AdjustFactor(int *up, int *down);
	int		RfromYUV(int y, int u, int v);
	int		GfromYUV(int y, int u, int v);
	int		BfromYUV(int y, int u, int v);
};



//========= class CmodelTools ==============

#define TOOLS_BUFSIZE		0x30000	/* must be 3 times */

class OLDCMODEL_DLL CmodelTools : public CImageTools
{
public:
	CmodelTools(void);
	virtual ~CmodelTools(void);

	void	hex2bin(char *src, char *dest);
	void	binadd(char *src, char *dest, int val);
	void	bin2Tohex(char *src, char *dest);
	void	bin1Tohex(char *src, char *dest);
	int		bincompare(char *src, char *dest);
	void	binsizer(char *src, char *dest, int srcwidth, int hstart, int vstart, int width, int height);
	void	bin1bytediv(char *src, char *dest, char div);
	void	bin1byte(char *src, char *dest);
	void	addbmpheader(char *src, char *dest, int width, int height);
	void	yuv422tobmp(char *src, char *dest);
	void	block422tobmp(char *src, char *dest, int width);
	void	bmp2rawdata(char *src, char *dest, int width);
	void	yuv422rotation(char *src, char *dest, int width, int height);

protected:
	void	InitTools(void);
	void	ReleaseTools(void);
	void	rotate90(Yuv422Type *src, Yuv422Type *dest, int width, int height);

private:
	short int *gSrc, *gDest;
	int		gState;
};


#endif


