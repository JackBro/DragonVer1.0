

#ifndef _TOOLS_GLOBAL_H_
#define _TOOLS_GLOBAL_H_

#ifndef TOOLS_DLL
	#define TOOLS_DLL  __declspec(dllimport)
#endif

#include <stdarg.h>

#ifdef WIN32

#include <afxpriv.h>
#include <mmreg.h>
#include <gdiplus.h>
using namespace Gdiplus;

#define		WM_COMSETTING		(WM_USER+1)
#define		WM_CHANNELRCV_DATA	(WM_USER+2)
#define		WM_BOX_DELKEY		(WM_USER+3)
#define		WM_BOX_LBDWPORT		(WM_USER+4)
#define		WM_BOX_LBUPPORT		(WM_USER+5)
#define		WM_BOX_MOVE			(WM_USER+6)
#define		WM_PROPDLG_ONINIT	(WM_USER+7)
#define		WM_PROPDLG_ONOK		(WM_USER+8)
#define		WM_SETDLG_ONOK		(WM_USER+9)
#define		WM_GAMMADLG_ONAPPLY	(WM_USER+10)
#define		WM_USBSETTING		(WM_USER+11)

#endif

#include "type.h"

#define DEFAULT_FILENAME_LEN	0x100

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

#ifndef WIN32

#pragma pack(1)
typedef struct tag_RGBQUAD 
{ 
	UINT8 rgbBlue;
	UINT8 rgbGreen;
	UINT8 rgbRed;
	UINT8 rgbReserved;
} RGBQUAD, *PRGBQUAD;

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

typedef struct tWAVEFORMATEX
{
    UINT16	wFormatTag;        /* format type */
    UINT16	nChannels;         /* number of channels (i.e. mono, stereo...) */
    UINT32	nSamplesPerSec;    /* sample rate */
    UINT32	nAvgBytesPerSec;   /* for buffer estimation */
    UINT16	nBlockAlign;       /* block size of data */
    UINT16	wBitsPerSample;    /* Number of bits per sample of mono data */
    UINT16	cbSize;            /* The count in bytes of the size of
                                    extra information (after cbSize) */
} WAVEFORMATEX;

#pragma pack()

/* WAVE form wFormatTag IDs */
#define	 WAVE_FORMAT_PCM				1

#define  WAVE_FORMAT_UNKNOWN                    0x0000 /* Microsoft Corporation */
#define  WAVE_FORMAT_ADPCM                      0x0002 /* Microsoft Corporation */
#define  WAVE_FORMAT_IEEE_FLOAT                 0x0003 /* Microsoft Corporation */
#define  WAVE_FORMAT_VSELP                      0x0004 /* Compaq Computer Corp. */
#define  WAVE_FORMAT_IBM_CVSD                   0x0005 /* IBM Corporation */
#define  WAVE_FORMAT_ALAW                       0x0006 /* Microsoft Corporation */
#define  WAVE_FORMAT_MULAW                      0x0007 /* Microsoft Corporation */
#define  WAVE_FORMAT_DTS                        0x0008 /* Microsoft Corporation */
#define  WAVE_FORMAT_DRM                        0x0009 /* Microsoft Corporation */
#define  WAVE_FORMAT_OKI_ADPCM                  0x0010 /* OKI */
#define  WAVE_FORMAT_DVI_ADPCM                  0x0011 /* Intel Corporation */
#define  WAVE_FORMAT_IMA_ADPCM                  (WAVE_FORMAT_DVI_ADPCM) /*  Intel Corporation */
#define  WAVE_FORMAT_MEDIASPACE_ADPCM           0x0012 /* Videologic */
#define  WAVE_FORMAT_SIERRA_ADPCM               0x0013 /* Sierra Semiconductor Corp */
#define  WAVE_FORMAT_G723_ADPCM                 0x0014 /* Antex Electronics Corporation */
#define  WAVE_FORMAT_DIGISTD                    0x0015 /* DSP Solutions, Inc. */
#define  WAVE_FORMAT_DIGIFIX                    0x0016 /* DSP Solutions, Inc. */
#define  WAVE_FORMAT_DIALOGIC_OKI_ADPCM         0x0017 /* Dialogic Corporation */
#define  WAVE_FORMAT_MEDIAVISION_ADPCM          0x0018 /* Media Vision, Inc. */
#define  WAVE_FORMAT_CU_CODEC                   0x0019 /* Hewlett-Packard Company */
#define  WAVE_FORMAT_YAMAHA_ADPCM               0x0020 /* Yamaha Corporation of America */
#define  WAVE_FORMAT_SONARC                     0x0021 /* Speech Compression */
#define  WAVE_FORMAT_DSPGROUP_TRUESPEECH        0x0022 /* DSP Group, Inc */
#define  WAVE_FORMAT_ECHOSC1                    0x0023 /* Echo Speech Corporation */
#define  WAVE_FORMAT_AUDIOFILE_AF36             0x0024 /* Virtual Music, Inc. */
#define  WAVE_FORMAT_APTX                       0x0025 /* Audio Processing Technology */
#define  WAVE_FORMAT_AUDIOFILE_AF10             0x0026 /* Virtual Music, Inc. */
#define  WAVE_FORMAT_PROSODY_1612               0x0027 /* Aculab plc */
#define  WAVE_FORMAT_LRC                        0x0028 /* Merging Technologies S.A. */
#define  WAVE_FORMAT_DOLBY_AC2                  0x0030 /* Dolby Laboratories */
#define  WAVE_FORMAT_GSM610                     0x0031 /* Microsoft Corporation */
#define  WAVE_FORMAT_MSNAUDIO                   0x0032 /* Microsoft Corporation */
#define  WAVE_FORMAT_ANTEX_ADPCME               0x0033 /* Antex Electronics Corporation */
#define  WAVE_FORMAT_CONTROL_RES_VQLPC          0x0034 /* Control Resources Limited */
#define  WAVE_FORMAT_DIGIREAL                   0x0035 /* DSP Solutions, Inc. */
#define  WAVE_FORMAT_DIGIADPCM                  0x0036 /* DSP Solutions, Inc. */
#define  WAVE_FORMAT_CONTROL_RES_CR10           0x0037 /* Control Resources Limited */
#define  WAVE_FORMAT_NMS_VBXADPCM               0x0038 /* Natural MicroSystems */
#define  WAVE_FORMAT_CS_IMAADPCM                0x0039 /* Crystal Semiconductor IMA ADPCM */
#define  WAVE_FORMAT_ECHOSC3                    0x003A /* Echo Speech Corporation */
#define  WAVE_FORMAT_ROCKWELL_ADPCM             0x003B /* Rockwell International */
#define  WAVE_FORMAT_ROCKWELL_DIGITALK          0x003C /* Rockwell International */
#define  WAVE_FORMAT_XEBEC                      0x003D /* Xebec Multimedia Solutions Limited */
#define  WAVE_FORMAT_G721_ADPCM                 0x0040 /* Antex Electronics Corporation */
#define  WAVE_FORMAT_G728_CELP                  0x0041 /* Antex Electronics Corporation */
#define  WAVE_FORMAT_MSG723                     0x0042 /* Microsoft Corporation */
#define  WAVE_FORMAT_MPEG                       0x0050 /* Microsoft Corporation */
#define  WAVE_FORMAT_RT24                       0x0052 /* InSoft, Inc. */
#define  WAVE_FORMAT_PAC                        0x0053 /* InSoft, Inc. */
#define  WAVE_FORMAT_MPEGLAYER3                 0x0055 /* ISO/MPEG Layer3 Format Tag */
#define  WAVE_FORMAT_LUCENT_G723                0x0059 /* Lucent Technologies */
#define  WAVE_FORMAT_CIRRUS                     0x0060 /* Cirrus Logic */
#define  WAVE_FORMAT_ESPCM                      0x0061 /* ESS Technology */
#define  WAVE_FORMAT_VOXWARE                    0x0062 /* Voxware Inc */
#define  WAVE_FORMAT_CANOPUS_ATRAC              0x0063 /* Canopus, co., Ltd. */
#define  WAVE_FORMAT_G726_ADPCM                 0x0064 /* APICOM */
#define  WAVE_FORMAT_G722_ADPCM                 0x0065 /* APICOM */
#define  WAVE_FORMAT_DSAT_DISPLAY               0x0067 /* Microsoft Corporation */
#define  WAVE_FORMAT_VOXWARE_BYTE_ALIGNED       0x0069 /* Voxware Inc */
#define  WAVE_FORMAT_VOXWARE_AC8                0x0070 /* Voxware Inc */
#define  WAVE_FORMAT_VOXWARE_AC10               0x0071 /* Voxware Inc */
#define  WAVE_FORMAT_VOXWARE_AC16               0x0072 /* Voxware Inc */
#define  WAVE_FORMAT_VOXWARE_AC20               0x0073 /* Voxware Inc */
#define  WAVE_FORMAT_VOXWARE_RT24               0x0074 /* Voxware Inc */
#define  WAVE_FORMAT_VOXWARE_RT29               0x0075 /* Voxware Inc */
#define  WAVE_FORMAT_VOXWARE_RT29HW             0x0076 /* Voxware Inc */
#define  WAVE_FORMAT_VOXWARE_VR12               0x0077 /* Voxware Inc */
#define  WAVE_FORMAT_VOXWARE_VR18               0x0078 /* Voxware Inc */
#define  WAVE_FORMAT_VOXWARE_TQ40               0x0079 /* Voxware Inc */
#define  WAVE_FORMAT_SOFTSOUND                  0x0080 /* Softsound, Ltd. */
#define  WAVE_FORMAT_VOXWARE_TQ60               0x0081 /* Voxware Inc */
#define  WAVE_FORMAT_MSRT24                     0x0082 /* Microsoft Corporation */
#define  WAVE_FORMAT_G729A                      0x0083 /* AT&T Labs, Inc. */
#define  WAVE_FORMAT_MVI_MVI2                   0x0084 /* Motion Pixels */
#define  WAVE_FORMAT_DF_G726                    0x0085 /* DataFusion Systems (Pty) (Ltd) */
#define  WAVE_FORMAT_DF_GSM610                  0x0086 /* DataFusion Systems (Pty) (Ltd) */
#define  WAVE_FORMAT_ISIAUDIO                   0x0088 /* Iterated Systems, Inc. */
#define  WAVE_FORMAT_ONLIVE                     0x0089 /* OnLive! Technologies, Inc. */
#define  WAVE_FORMAT_SBC24                      0x0091 /* Siemens Business Communications Sys */
#define  WAVE_FORMAT_DOLBY_AC3_SPDIF            0x0092 /* Sonic Foundry */
#define  WAVE_FORMAT_MEDIASONIC_G723            0x0093 /* MediaSonic */
#define  WAVE_FORMAT_PROSODY_8KBPS              0x0094 /* Aculab plc */
#define  WAVE_FORMAT_ZYXEL_ADPCM                0x0097 /* ZyXEL Communications, Inc. */
#define  WAVE_FORMAT_PHILIPS_LPCBB              0x0098 /* Philips Speech Processing */
#define  WAVE_FORMAT_PACKED                     0x0099 /* Studer Professional Audio AG */
#define  WAVE_FORMAT_MALDEN_PHONYTALK           0x00A0 /* Malden Electronics Ltd. */
#define  WAVE_FORMAT_RHETOREX_ADPCM             0x0100 /* Rhetorex Inc. */
#define  WAVE_FORMAT_IRAT                       0x0101 /* BeCubed Software Inc. */
#define  WAVE_FORMAT_VIVO_G723                  0x0111 /* Vivo Software */
#define  WAVE_FORMAT_VIVO_SIREN                 0x0112 /* Vivo Software */
#define  WAVE_FORMAT_DIGITAL_G723               0x0123 /* Digital Equipment Corporation */
#define  WAVE_FORMAT_SANYO_LD_ADPCM             0x0125 /* Sanyo Electric Co., Ltd. */
#define  WAVE_FORMAT_SIPROLAB_ACEPLNET          0x0130 /* Sipro Lab Telecom Inc. */
#define  WAVE_FORMAT_SIPROLAB_ACELP4800         0x0131 /* Sipro Lab Telecom Inc. */
#define  WAVE_FORMAT_SIPROLAB_ACELP8V3          0x0132 /* Sipro Lab Telecom Inc. */
#define  WAVE_FORMAT_SIPROLAB_G729              0x0133 /* Sipro Lab Telecom Inc. */
#define  WAVE_FORMAT_SIPROLAB_G729A             0x0134 /* Sipro Lab Telecom Inc. */
#define  WAVE_FORMAT_SIPROLAB_KELVIN            0x0135 /* Sipro Lab Telecom Inc. */
#define  WAVE_FORMAT_G726ADPCM                  0x0140 /* Dictaphone Corporation */
#define  WAVE_FORMAT_QUALCOMM_PUREVOICE         0x0150 /* Qualcomm, Inc. */
#define  WAVE_FORMAT_QUALCOMM_HALFRATE          0x0151 /* Qualcomm, Inc. */
#define  WAVE_FORMAT_TUBGSM                     0x0155 /* Ring Zero Systems, Inc. */
#define  WAVE_FORMAT_MSAUDIO1                   0x0160 /* Microsoft Corporation */
#define  WAVE_FORMAT_UNISYS_NAP_ADPCM           0x0170 /* Unisys Corp. */
#define  WAVE_FORMAT_UNISYS_NAP_ULAW            0x0171 /* Unisys Corp. */
#define  WAVE_FORMAT_UNISYS_NAP_ALAW            0x0172 /* Unisys Corp. */
#define  WAVE_FORMAT_UNISYS_NAP_16K             0x0173 /* Unisys Corp. */
#define  WAVE_FORMAT_CREATIVE_ADPCM             0x0200 /* Creative Labs, Inc */
#define  WAVE_FORMAT_CREATIVE_FASTSPEECH8       0x0202 /* Creative Labs, Inc */
#define  WAVE_FORMAT_CREATIVE_FASTSPEECH10      0x0203 /* Creative Labs, Inc */
#define  WAVE_FORMAT_UHER_ADPCM                 0x0210 /* UHER informatic GmbH */
#define  WAVE_FORMAT_QUARTERDECK                0x0220 /* Quarterdeck Corporation */
#define  WAVE_FORMAT_ILINK_VC                   0x0230 /* I-link Worldwide */
#define  WAVE_FORMAT_RAW_SPORT                  0x0240 /* Aureal Semiconductor */
#define  WAVE_FORMAT_ESST_AC3                   0x0241 /* ESS Technology, Inc. */
#define  WAVE_FORMAT_IPI_HSX                    0x0250 /* Interactive Products, Inc. */
#define  WAVE_FORMAT_IPI_RPELP                  0x0251 /* Interactive Products, Inc. */
#define  WAVE_FORMAT_CS2                        0x0260 /* Consistent Software */
#define  WAVE_FORMAT_SONY_SCX                   0x0270 /* Sony Corp. */
#define  WAVE_FORMAT_FM_TOWNS_SND               0x0300 /* Fujitsu Corp. */
#define  WAVE_FORMAT_BTV_DIGITAL                0x0400 /* Brooktree Corporation */
#define  WAVE_FORMAT_QDESIGN_MUSIC              0x0450 /* QDesign Corporation */
#define  WAVE_FORMAT_VME_VMPCM                  0x0680 /* AT&T Labs, Inc. */
#define  WAVE_FORMAT_TPC                        0x0681 /* AT&T Labs, Inc. */
#define  WAVE_FORMAT_OLIGSM                     0x1000 /* Ing C. Olivetti & C., S.p.A. */
#define  WAVE_FORMAT_OLIADPCM                   0x1001 /* Ing C. Olivetti & C., S.p.A. */
#define  WAVE_FORMAT_OLICELP                    0x1002 /* Ing C. Olivetti & C., S.p.A. */
#define  WAVE_FORMAT_OLISBC                     0x1003 /* Ing C. Olivetti & C., S.p.A. */
#define  WAVE_FORMAT_OLIOPR                     0x1004 /* Ing C. Olivetti & C., S.p.A. */
#define  WAVE_FORMAT_LH_CODEC                   0x1100 /* Lernout & Hauspie */
#define  WAVE_FORMAT_NORRIS                     0x1400 /* Norris Communications, Inc. */
#define  WAVE_FORMAT_SOUNDSPACE_MUSICOMPRESS    0x1500 /* AT&T Labs, Inc. */
#define  WAVE_FORMAT_DVM                        0x2000 /* FAST Multimedia AG */

#endif

#define RIFF_FLAG		0x46464952		/* 'R''I''F''F' */
#define WAVE_FLAG		0x45564157		/* 'W''A''V''E' */
#define FMT_FLAG		0x20746D66		/* 'f''m''t'' ' */
#define DATA_FLAG		0x61746164		/* 'd''a''t''a' */

typedef struct tag_TBitMap
{
	int  index;
	int  width;
	int	 height;
} TBitMap, *PTBitMap; 



extern TOOLS_DLL const char *gImageStreamType[];
extern TOOLS_DLL const char gTotalImageStreamType;
extern TOOLS_DLL const int gImgStreamOrder[][6];
extern TOOLS_DLL const int gImageProp[];
extern TOOLS_DLL const char *gRotateMode[];
extern TOOLS_DLL const char gTotalRotateMode;
extern TOOLS_DLL const char *gOvlpMode[];
extern TOOLS_DLL const char gTotalOvlpMode;
extern TOOLS_DLL const char *gOverMode[];
extern TOOLS_DLL const char gTotalOverMode;
extern TOOLS_DLL const char *gJpegFmt[];
extern TOOLS_DLL const char gTotalJpegFmt;
extern TOOLS_DLL const char *gAudStreamType[];
extern TOOLS_DLL const char gTotalAudStreamType;
extern TOOLS_DLL const char *gCtrlStreamType[];
extern TOOLS_DLL const char gTotalCtrlStreamType;
extern TOOLS_DLL const char *gSystemStreamType[];
extern TOOLS_DLL const char gTotalSystemStreamType;
extern TOOLS_DLL const char *gStreamClassType[];
extern TOOLS_DLL const char gTotalStreamClassType;

extern TOOLS_DLL const int gRegTotalType;
extern TOOLS_DLL const char *gRegType[];

const int  gTotalNoiseTable = 17;

#endif


