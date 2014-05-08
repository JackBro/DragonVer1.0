

#ifndef _TOOLS_TYPE_H_
#define _TOOLS_TYPE_H_

#include <stdio.h>
#include <string.h>
#include <math.h>

#ifndef WIN32

#ifndef SINT32
//signed 32-bit integer
#define SINT32	long
#endif

#ifndef UINT32
//unsigned 32-bit integer
#define UINT32	unsigned long 
#endif

#ifndef SINT8
//signed 8-bit integer
#define SINT8	char
#endif

#ifndef UINT8
//unsigned 8-bit integer
#define UINT8	unsigned char
#endif

#ifndef SINT16
//signed 16-bit integer
#define SINT16	short
#endif

#ifndef UINT16
//unsigned 16-bit integer
#define UINT16	unsigned short
#endif

#else

//signed 8-bit integer
typedef char SINT8;
//unsigned 8-bit integer
typedef unsigned char UINT8;
//signed 16-bit integer
typedef short SINT16;
//unsigned 16-bit integer
typedef unsigned short UINT16;

//signed 32-bit integer
typedef int SINT32;
//unsigned 32-bit integer
typedef unsigned int UINT32;

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
	BIT15	=	0x8000
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


typedef struct tag_TSize
{	
	UINT16 cx;
	UINT16 cy;
} TSize, *PTSize;

typedef struct tag_TPoint
{	
	UINT16 x;
	UINT16 y;
} TPoint, *PTPoint;

typedef struct tag_TRect
{	
	TPoint ltp;
	TSize  size;
} TRect, *PTRect;

typedef struct tag_TClip
{	
	TPoint ltp;
	TPoint rbp;
} TClip, *PTClip;

typedef struct tag_TReg {
	UINT32 adr;					//register address
	UINT16 val;					//register setting value
	UINT16 wait;				//ms
} TReg, *PTReg;

typedef struct tag_TRegGroup {
	UINT16 len;					/* register list length */
	TReg *regval;				/* registers array point */	
} TRegGroup, *PTRegGroup;

typedef struct tag_TDataBuf
{
	UINT32		Len;			//buffer length 
	UINT32		Pos;			//current data point
	UINT8		*pData;			//data pointer 
} TDataBuf, *PTDataBuf;
/*
typedef struct tag_TJfifIndex
{
	UINT8		isThumb;		//isThumb=1:have thumbnail; isThumb=0:no thumbnail.
	UINT16		fieldLen;
	UINT16		version;
	UINT8       units;
	UINT16      xdensity;
	UINT16      ydensity;
	UINT8       xthumbnail;
	UINT8       ythumbnail;
	UINT8*		ppalette;		//position of thumbnail palette begin
	UINT8*      pthumbbuf;		//position of thumbnail data begin
	UINT16      thumblen;		//the length of thumbnail
	UINT8       thumbformat;	//0:rgb24 ; 1:rgb8 ; 2:jpeg.
	UINT8       extensioncode;
} TJfifIndex, *PTJfifIndex;

typedef struct tag_TExifHeader
{
	UINT8	byteOrder;
	UINT32	ifdOffset;
} TExifHeader, *PTExifHeader;

typedef struct tag_TExifIFD0
{
	UINT16 orien;				//pot(0,0) position
	//To be add
} TExifIFD0, *PTExifIFD0;

typedef struct tag_TExifSubIFD0
{
	UINT32 imgWidth;
	UINT32 imgHeight;
	
	//To be add
} TExifSubIFD0, *PTExifSubIFD0;
*/
typedef struct tag_TExifIFD1
{	
	UINT32		thumbWidth;					//for uncompressed thumbnail 	
	UINT32		thumbHeight;
	UINT16		compression;				//when thumbnails use JPEG compression, this tag 6 = JPEG compression 1 = uncompressed 	 	
	UINT16		PhotometricInterpretation;	//The pixel composition. 2 = RGB 6 = YCbCr	
	UINT16		YCbCrSubSampling;			//The sampling ratio of chrominance components in relation to the luminance component.
											//[2, 1] = YCbCr4:2:2 [2, 2] = YCbCr4:2:0 
	UINT32		StripOffsets;				//For each strip, the byte offset of that strip. When image format is no compression, this value shows offset to image data
											// = StripsPerImage (when PlanarConfiguration = 1)
											// = SamplesPerPixel * StripsPerImage (when PlanarConfiguration = 2)		
	UINT32		StripByteCounts;			//The total number of bytes in each strip.
											// = StripsPerImage (when PlanarConfiguration = 1)
											// = SamplesPerPixel * StripsPerImage (when PlanarConfiguration = 2)
											//These members are used for compressed thumbnail
	UINT32		JPEGInterchangeFormat;		//The offset to the start byte (SOI) of JPEG compressed thumbnail data. This is not used for primary image JPEG data		
	UINT32		JPEGInterchangeFormatLength;//The number of bytes of JPEG compressed thumbnail data. This is not used for primary image JPEG data.
	
	UINT16		PlanarConfiguration;		//Indicates whether pixel components are recorded in chunky or planar format.
											//If this field does not exist, the TIFF default of 1 (chunky) is assumed.
											//1 = chunky format 2 = planar format		

/*	UINT32		RowsPerStrip;				//The number of rows per strip. This is the number of rows in the image 
											//of one strip when an image is divided into strips.
	UINT16		SamplesPerPixel;			//The number of components per pixel. Default = 3
	
	UINT16		YCbCrPositioning;			//The position of chrominance components in relation to the luminance component. This field is designated only for
											//JPEG compressed data or uncompressed YCbCr data. The TIFF default is 1 (centered);
											//Default = 1. 1 = centered 2 = co-sited
*/	
} TExifIFD1, *PTExifIFD1;

/*
typedef struct tag_TExifIndex
{
	TExifHeader header;
	
	TExifIFD0 ifd0;

	TExifSubIFD0 subifd0;

	TExifIFD1 ifd1;

} TExifIndex, *PTExifIndex;
*/
enum JPEGTYPEenum	//this enum must same as lbuf format order
{
	JPEG_422 = 0,
	JPEG_420,
	JPEG_411,
	JPEG_400,
	JPEG_444,
	JPEG_UNKNOWN
};

/*Register width and order type*/
enum REGWTYPEenum
{
	BIT_8                  = 0x0,
	BIT_16_LTOH            = 0x1,
	BIT_16_HTOL            = 0x2,
	BIT_24_LTOH            = 0x3,
	BIT_24_HTOL            = 0x4,
	BIT_32_LTOH            = 0x5,
	BIT_32_HTOL            = 0x6
};

enum THUMBTYPEenum
{
	NO_THUMBNAIL	=	0,
	THUMBNAIL_JFIF	=	1,
	THUMBNAIL_JFXX	=	2,
	THUMBNAIL_EXIF	=	3
};


typedef struct tag_TThumbNail
{
	UINT8		thumbtype;
	UINT8		thumbfmt;
	UINT32		thumbWidth;
	UINT32		thumbHeight;
	UINT32		thumblen;
	UINT8*		pthumbbuf;
	UINT8*		ppalette;

	UINT8		byteOrder;

} TThumbNail, *PTThumbNail;


typedef struct tag_TJpegIndex
{
	UINT8		YUVType;        //the format of jpeg file,such as YUV422\YUV420\YUV411\YUV400
	UINT8	    HTCount;        //the number of Huffman table
	UINT8       QTCount;        //the number of Quantization table
	UINT8		data_precision;	//Specifies the precision in bits for the samples of the components in the frame.
	UINT8		CompCount;      //the number of component
	UINT8		qf;				//Jpeg Q factor

	UINT8	    HTLen[4];       //the length of every Huffman table
	UINT8       QTPrecision[4]; //the precision of quantization table 

	UINT16		frmcnt;			//frame count
	UINT16		blkcount;       //the number of block
	UINT16		Comp[4];        //the component 
	
	UINT32	    offset;         //the offset from jpeg file begin to 
								//the postion of image data begin
	UINT32		vwc;            //video word count
	UINT32		sop;			//the position of jpeg file begin(begin with FFD8)
	UINT32		eop;			//the position of jpeg file end(end with FFD9)
	TSize		ImageSize;      //real image size
	TSize		JpgSize;        //image size after compensation,if the image format is YUV422,
								//then,JpgSize.cx must be multiple of 16,and JpgSize.cy must be
								//multiple of 8.

	UINT8		*HT[4];         //the point of Huffman table
	UINT8		*QT[4];         //the point of Quantization table

	TThumbNail	thumbinfo;
	
} TJpegIndex, *PTJpegIndex;

typedef struct tag_TYuv422Block
{
	UINT8	y0[64];
	UINT8	y1[64];
	UINT8	u[64];
	UINT8	v[64];
} TYuv422Block, *pTYuv422Block;

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

typedef struct tag_TRgbQuad
{
	UINT8 b;
	UINT8 g;
	UINT8 r;
	UINT8 reserve;
} TRgbQuad, *PTRgbQuad;

typedef struct tag_TYuv444
{
	UINT8	y;
	UINT8	u;
	UINT8	v;
} TYuv444, *pTYuv444;

typedef struct tag_TYuv422
{
	UINT8	uv;
	UINT8	y;
} TYuv422, *pTYuv422;

typedef struct tag_TColorMatrix {
	UINT8	cmat11, cmat12, cmat13, offsetr;
	UINT8	cmat21, cmat22, cmat23, offsetg;
	UINT8	cmat31, cmat32, cmat33, offsetb;
} TColorMatrix, *PTColorMatrix;


//BMP info header in embeded system, in case big endian, little endian
typedef struct tag_TBitmapFileHeader 
{
	UINT8	dat[14];
} TBitmapFileHeader, *PTBitmapFileHeader;

typedef struct tag_TBitmapInfoHeader
{
	UINT8	biSize[4];
	UINT8	biWidth[4];
	UINT8	biHeight[4];
	UINT8	biDat0[2];
	UINT8	biBitCount[2];
	UINT8	biDat1[4];
	UINT8	biSizeImage[4];
	UINT8	biDat2[16];
} TBitmapInfoHeader, *PTBitmapInfoHeader; 




//=========================================
//
//
//
//================================
enum CTRLSTREAMenum
{
	CTRLSIG
};

enum AUDIOSTREAMenum
{
	WAVE_PCM_MS,
	WAVE_ADPCM_MS,
	WAVE_ADPCM_VIMICRO,
	WAVE_ADPCM_YAMAYA,
	AUD_PCM,
	AUD_ADPCM_MS,
	AUD_ADPCM_VIMICRO,
	AUD_ADPCM_YAMAYA,
	AUD_PCM_3D_EXPANSION,
	AUD_PCM_3D_POSITION,
	AUD_PCM_3D_SURROUNDING,
	AUD_PCM_AGC,
	AUD_PCM_ADC,
	AUD_PCM_128_48KHZ,
	MIDI
};

enum IMAGESTREAMenum
{
	YUV422_UYVY,
	YUV422_VYUY,
	YUV422_YUYV,
	YUV422_YVYU,
	YUV422_YUVY,

	YUV422_YVUY,
	YUV422_UVYY,
	YUV422_VUYY,
	YUV422_YYVU,
	YUV422_YYUV,

	RAW_BG,
	RAW_GB,
	RAW_RG,
	RAW_GR,

	RAW_RGB,
	RAW_RBG,
	RAW_GBR,
	RAW_GRB,
	RAW_BGR,
	RAW_BRG,

	YUV420_UYVY,
	YUV420_VYUY,
	YUV420_YUYV,
	YUV420_YVYU,
	YUV420_YUVY,

	YUV420_YVUY,
	YUV420_UVYY,
	YUV420_VUYY,
	YUV420_YYVU,
	YUV420_YYUV,

	YUV420_YYU,
	YUV420_YUY,
	YUV420_UYY,

	YUV411_YYYYUV,
	YUV411_YYYYVU,
	YUV411_YUYVYY,
	YUV411_YVYUYY,
	YUV411_UVYYYY,
	YUV411_VUYYYY,

	YUV400,
	YUV444,

	RGB1,
	RGB2,
	RGB4,
	RGB8,
	RGB444,
	RGB555,
	RGB565,
	RGB666,
	RGB24,
	RGB32,

	YUV422M2BLK,
	YUV422BLK,
	YUV420BLK,
	YUV411BLK,
	YUV400BLK,
	YUV444BLK,
	YUV444M2BLK,

	YUV422LBUF,
	YUV420LBUF,
	YUV411LBUF,
	YUV400LBUF,
	YUV444LBUF,

	BMP,
	JPEG422,
	JPEG420,
	JPEG411,
	JPEG400,
	JPEG444,

	RAW_R,
	RAW_G,
	RAW_B,

	RAW_Y,
	RAW_U444,
	RAW_U422,
	RAW_U420,
	RAW_U411,
	RAW_V444,
	RAW_V422,
	RAW_V420,
	RAW_V411,

	YUV422FRM,
	YUV420FRM,
	YUV411FRM,
	YUV400FRM,
	YUV444FRM,

	MPEG1I_FRM,
	MPEG2IFRM420,
	MPEG2IFRM422,
	MPEG2IFRM444,
	MPEG4_FRM,

	EDGE_MAP,

	YIQ,
	HSI,
	RAW_YIQ_Y,
	RAW_YIQ_I,
	RAW_YIQ_Q,

	RAW_HSI_I,
	RAW_HSI_H,
	RAW_HSI_S,

	GIF,
	PNG,
	PCX,
	CCIR601_656NTSC,
	CCIR601_656PAL,

	YCbCr422_YYCbCr,
	YCbCr422_YpCbpCrp,
	YCbCr420_YYYYCbCr,
	YCbCr420_YpCbpCrp,
	RAW_RGB_RpGpBp,

	//ly add for suport decode 444
	YUV444422LBUF

};

enum SYSTEMSTREAMenum
{
	AVI,
	MOBILE3GPP,
	UNKNOWNSTREAM
};

enum STREAMCLASSenum
{
	VIDEOCLASS,
	AUDIOCLASS,
	SYSTEMCLASS,
	CTRLCLASS
};

#define CLASSSHIFT		13
#define CLASSMASK		(7 << CLASSSHIFT)
#define FORMATMASK		((1 << CLASSSHIFT) - 1)

enum ROTATEMODEenum {
	ROT_DEGREE0,
	ROT_MIRROR270,
	ROT_MIRROR180,
	ROT_DEGREE90,
	ROT_MIRROR0,
	ROT_DEGREE270,
	ROT_DEGREE180,
	ROT_MIRROR90
};

enum OVERLAYMODEenum {
	OM_TRANSPARENT,
	OM_AND,
	OM_OR,
	OM_INVERT
};

enum TRANSFEROPCODEenum
{
	ADD_OPCODE,
	SUB_OPCODE,
	MUL_OPCODE,
	DIV_OPCODE,
	DIVBY_OPCODE,
	MOD_OPCODE,

	AND_OPCODE,
	OR_OPCODE,
	XOR_OPCODE,
	NOT_OPCODE,

	EQUAL_OPCODE
};

enum NTSCPALSIZEenum {
	NTSC_WIDTH		=	720,
	NTSC_HEIGHT		=	480,
	PAL_WIDTH		=	720,
	PAL_HEIGHT		=	576,
	PAL_HEIGHT_EXT	=	600
};

#define BUFFER_DEFAULT_SIZE		0x1000

#endif


