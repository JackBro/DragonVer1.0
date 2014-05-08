
#include "internal.h"

const char *gImageStreamType[] = 
{ 
	//yuv422
	"YUV422_UYVY", "YUV422_VYUY", "YUV422_YUYV", "YUV422_YVYU",	"YUV422_YUVY",
	"YUV422_YVUY", "YUV422_UVYY", "YUV422_VUYY", "YUV422_YYVU",	"YUV422_YYUV",

	"BG", "GB", "RG", "GR",				//rawdata
	"RAW_RGB", "RAW_RBG", "RAW_GBR", "RAW_GRB", "RAW_BGR", "RAW_BRG",

	//yuv420
	"YUV420_UYVY", "YUV420_VYUY", "YUV420_YUYV", "YUV420_YVYU",	"YUV420_YUVY",
	"YUV420_YVUY", "YUV420_UVYY", "YUV420_VUYY", "YUV420_YYVU",	"YUV420_YYUV",

	"YUV420_YYU", "YUV420_YUY", "YUV420_UYY",

	"YUV411_YYYYUV", "YUV411_YYYYVU", "YUV411_YUYVYY", "YUV411_YVYUYY",
	"YUV411_UVYYYY", "YUV411_VUYYYY",

	"YUV400",	//Y Y Y Y Y ...
	"YUV444",	//YUV YUV ...

	"RGB1", "RGB2", "RGB4", "RGB8", "RGB444", "RGB555", "RGB565", "RGB666", "RGB24", "RGB32",

	"YUV422M2BLK", "YUV422BLK", "YUV420BLK", "YUV411BLK", "YUV400BLK", "YUV444BLK", "YUV444M2BLK",
	"YUV422LBUF", "YUV420LBUF", "YUV411LBUF", "YUV400LBUF", "YUV444LBUF",

	"BMP",
	"JPEG422", "JPEG420", "JPEG411", "JPEG400", "JPEG444",
	"RAW_R", "RAW_G", "RAW_B", 
	"RAW_Y",
	"RAW_U444", "RAW_U422", "RAW_U420", "RAW_U411", 
	"RAW_V444", "RAW_V422", "RAW_V420", "RAW_V411",

	"YUV422FRM", "YUV420FRM", "YUV411FRM", "YUV400FRM", "YUV444FRM",
	
	"MPEG1I_FRM",
	"MPEG2IFRM420", "MPEG2IFRM422", "MPEG2IFRM444",
	"MPEG4_FRM",

	"EDGE_MAP",
	"YIQ", "HSI", 
	"RAW_YIQ_Y", "RAW_YIQ_I", "RAW_YIQ_Q",
	"RAW_HSI_I", "RAW_HSI_H", "RAW_HSI_S",

	"GIF", "PNG", "PCX", "CCIR601_656NTSC",	"CCIR601_656PAL",

	"YCbCr422_YYCbCr", "YCbCr422_YpCbpCrp",	"YCbCr420_YYYYCbCr", "YCbCr420_YpCbpCrp",
	"RAW_RGB_RpGpBp"
};

const char gTotalImageStreamType = sizeof(gImageStreamType) / sizeof(char *);

const int gImgStreamOrder[][6] =
{
	{ 1, 0, 2, 3 },			//YUV422 UYVY
	{ 2, 0, 1, 3 },			//YUV422 VYUY
	{ 0, 1, 3, 2 },			//YUV422 YUYV
	{ 0, 2, 3, 1 },			//YUV422 YVYU
	{ 0, 1, 2, 3 },			//YUV422 YUVY

	{ 0, 2, 1, 3 },			//YUV422 YVUY
	{ 1, 2, 0, 3 },			//YUV422 UVYY
	{ 2, 1, 0, 3 },			//YUV422 VUYY
	{ 0, 3, 2, 1 },			//YUV422 YYVU
	{ 0, 3, 1, 2 },			//YUV422 YYUV

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

	{ 1, 0, 2, 3 },			//YUV420 UYVY
	{ 2, 0, 1, 3 },			//YUV420 VYUY
	{ 0, 1, 3, 2 },			//YUV420 YUYV
	{ 0, 2, 3, 1 },			//YUV420 YVYU
	{ 0, 1, 2, 3 },			//YUV420 YUVY

	{ 0, 2, 1, 3 },			//YUV420 YVUY
	{ 1, 2, 0, 3 },			//YUV420 UVYY
	{ 2, 1, 0, 3 },			//YUV420 VUYY
	{ 0, 3, 2, 1 },			//YUV420 YYVU
	{ 0, 3, 1, 2 },			//YUV420 YYUV

	{ 2, 1, 0, 3 },			//YUV420 YYU
	{ 0, 3, 2, 1 },			//YUV420 YUY
	{ 0, 3, 1, 2 },			//YUV420 UYY

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

	{ 1 },					//RAW_U444,
	{ 1 },					//RAW_U422,
	{ 1 },					//RAW_U420,
	{ 1 },					//RAW_U411,
	{ 2 },					//RAW_V444,
	{ 2 },					//RAW_V422,
	{ 2 },					//RAW_V420,
	{ 2 },					//RAW_V411,

	{ 0 }, 					//YUV422FRM,
	{ 0 }, 					//YUV420FRM,
	{ 0 },					//YUV411FRM,
	{ 0 },					//YUV400FRM,
	{ 0 },					//YUV444FRM

	{ 0 }, 					//MPEG1I_FRM,
	{ 0 },					//MPEG2IFRM420,
	{ 0 },					//MPEG2IFRM422,
	{ 0 },					//MPEG2IFRM444,
	{ 0 },					//MPEG4_FRM,
	{ 0 }					//EDGE_MAP
};

const int gImageProp[] =
{
    16,		//YUV422_UYVY,
	16,		//YUV422_VYUY,
	16,		//YUV422_YUYV,
	16,		//YUV422_YVYU,
	16,		//YUV422_YUVY,

	16,		//YUV422_YVUY,
	16,		//YUV422_UVYY,
	16,		//YUV422_VUYY,
	16,		//YUV422_YYVU,
	16,		//YUV422_YYUV,

	8,		//RAW_BG,
	8,		//RAW_GB,
	8,		//RAW_RG,
	8,		//RAW_GR,

	24,		//RAW_RGB,
	24,		//RAW_RBG,
	24,		//RAW_GBR,
	24,		//RAW_GRB,
	24,		//RAW_BGR,
	24,		//RAW_BRG,

	12,		//YUV420_UYVY,
	12,		//YUV420_VYUY,
	12,		//YUV420_YUYV,
	12,		//YUV420_YVYU,
	12,		//YUV420_YUVY,

	12,		//YUV420_YVUY,
	12,		//YUV420_UVYY,
	12,		//YUV420_VUYY,
	12,		//YUV420_YYVU,
	12,		//YUV420_YYUV,

	12,		//YUV420 YYU
	12,		//YUV420 YUY
	12,		//YUV420 UYY

	12,		//YUV411_YYYYUV,
	12,		//YUV411_YYYYVU,
	12,		//YUV411_YUYVYY,
	12,		//YUV411_YVYUYY,
	12,		//YUV411_UVYYYY,
	12,		//YUV411_VUYYYY,

	8,		//YUV400,
	24,		//YUV444,

	1,		//RGB1,
	2,		//RGB2,
	4,		//RGB4,
	8,		//RGB8,
	12,		//RGB444,
	16,		//RGB555,
	16,		//RGB565,
	18,		//RGB666,
	24,		//RGB24,
	32,		//RGB32,

	16,		//YUV422M2BLK,
	16,		//YUV422BLK,
	12,		//YUV420BLK,
	12,		//YUV411BLK,
	8,		//YUV400BLK,
	24,		//YUV444BLK,
	24,		//YUV444M2BLK,

	16,		//YUV422LBUF,
	12,		//YUV420LBUF,
	12,		//YUV411LBUF,
	8,		//YUV400LBUF,
	24,		//YUV444LBUF,

	24,		//BMP,
	16,		//JPEG422,
	12,		//JPEG420,
	12,		//JPEG411,
	8,		//JPEG400,
	24,		//JPEG444,

	8,		//RAW_R,
	8,		//RAW_G,
	8,		//RAW_B,

	8,		//RAW_Y,
	8,		//RAW_U444,
	4,		//RAW_U422,
	2,		//RAW_U420,
	2,		//RAW_U411,
	8,		//RAW_V444,
	4,		//RAW_V422,
	2,		//RAW_V420,
	2,		//RAW_V411,

	16,		//YUV422FRM,
	12,		//YUV420FRM,
	12,		//YUV411FRM,
	8,		//YUV400FRM,
	24,		//YUV444FRM,

	12,		//MPEG1I_FRM,
	12,		//MPEG2IFRM420,
	16,		//MPEG2IFRM422,
	24,		//MPEG2IFRM444,
	12,		//MPEG4_FRM,

	8,		//EDGE_MAP,

	24,		//YIQ,
	24,		//HSI,
	8,		//RAW_YIQ_Y,
	8,		//RAW_YIQ_I,
	8,		//RAW_YIQ_Q,

	8,		//RAW_HSI_I,
	8,		//RAW_HSI_H,
	8,		//RAW_HSI_S,

	8,		//GIF,
	24,		//PNG,
	24,		//PCX,
	16,		//CCIR601_656NTSC,
	16,		//CCIR601_656PAL,

	16,		//YCbCr422_YYCbCr,
	16,		//YCbCr422_YpCbpCrp,
	12,		//YCbCr420_YYYYCbCr,
	12,		//YCbCr420_YpCbpCrp,
	24		//RAW_RGB_RpGpBp
};

const char *gRotateMode[] = 
{ 
	"ROT_DEGREE0", "ROT_DEGREE90", "ROT_DEGREE180", "ROT_DEGREE270",
	"ROT_MIRROR0", "ROT_MIRROR90", "ROT_MIRROR180", "ROT_MIRROR270"
};

const char gTotalRotateMode = sizeof(gRotateMode) / sizeof(char *);

const char *gOvlpMode[] =
{
	"CASE 0", "CASE 1", "CASE 2", "CASE 3"
};

const char gTotalOvlpMode = sizeof (gOvlpMode) / sizeof (char *);

const char *gOverMode[] =
{
	"OM_TRANSPARENT", "OM_AND", "OM_OR", "OM_INVERT"
};

const char gTotalOverMode = sizeof(gOverMode) / sizeof(char *);

const char *gJpegFmt[] = 
{ 
	"JPEG422", "JPEG420", "JPEG411", "JPEG400", "JPEG444", "JPEG_UNKNOWN"
};

const char gTotalJpegFmt = sizeof(gJpegFmt) / sizeof(char *);


const char *gAudStreamType[] = 
{
	"WAVE_PCM_MS",        "WAVE_MS_ADPCM",         "WAVE_VIMICRO_ADPCM",
	"WAVE_YAMAHA_ADPCM",  "AUD_PCM",               "AUD_ADPCM_MS",
	"AUD_ADPCM_VIMICRO",  "AUD_ADPCM_YAMAYA",      "AUD_PCM_3D_EXPANSION",
	"AUD_PCM_3D_POSITION","AUD_PCM_3D_SURROUNDING","AUD_PCM_AGC",
	"AUD_PCM_ADC",        "AUD_PCM_128_48KHZ",     "MIDI"
};

const char gTotalAudStreamType = sizeof(gAudStreamType) / sizeof(char *);

const char *gCtrlStreamType[] =
{
	"Ctrl signal"
};

const char gTotalCtrlStreamType = sizeof(gCtrlStreamType) / sizeof(char *);

const char *gSystemStreamType[] =
{
	"AVI",
	"Mobile 3GPP",
	"Unknown stream"
};

const char gTotalSystemStreamType = sizeof(gSystemStreamType) / sizeof(char *);

const char *gStreamClassType[] =
{
	"VIDEOCLASS",
	"AUDIOCLASS",
	"SYSTEMCLASS",
	"CTRLCLASS"
};

const char gTotalStreamClassType = sizeof(gStreamClassType) / sizeof(char *);


const char *gRegType[] = 
{
	"V568",
	"V558",
	"SENSOR",
	"PANEL",
	"PANELLINE",
	"HOST",
	"LCD_SETRSLOW",
	"LCD_SETRSHIGH",
	"LCD_SETREG",
	"LCD_GETRSLOW",
	"LCD_GETRSHIGH",
	"LCD_GETREG",
	"USB9604",
	"V938"
};

const int gRegTotalType = sizeof(gRegType) / sizeof(char *);




