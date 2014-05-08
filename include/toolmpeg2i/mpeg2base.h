
#ifndef _TOOLS_MPEG2BASE_H_
#define _TOOLS_MPEG2BASE_H_


#ifndef TOOLMPEG2I_DLL
	#define TOOLMPEG2I_DLL  __declspec(dllimport)
#endif

#include "tooljpg/jmmatrix.h"
#include "tools/type.h"

enum MPEGSETTINGenum {
	MPEG1_STREAM		=	BIT0,	//or MPEG2
	LOAD_INTRA_QUANT	=	BIT1,
	LOAD_NONINTRA_QUANT	=	BIT2,
	FIELD_ENCODING		=	BIT3	//or frame encoding
};

#define MPEG_STARTCODE	0x100

enum MPEGSTARTCODEenum {
	MPEGPIC_START_CODE	=	0x00,
	MPEG_MINSLICE_CODE	=	0x01,
	MPEG_MAXSLICE_CODE	=	0xaf,
	MPEG_USERDATA_CODE	=	0xb2,
	MPEG_SEQHEADER_CODE	=	0xb3,
	MPEG_SEQERROR_CODE	=	0xb4,
	MPEG_EXTSTART_CODE	=	0xb5,
	MPEG_SEQEND_CODE	=	0xb7,
	MPEG_GROUPSTART_CODE=	0xb8,
	MPEG_MINSYS_CODE	=	0xb9,
	MPEG_MAXSYS_CODE	=	0xff
};

enum MPEGEXTCODEINDENTenum {
	MPEG_SEQ_EXTID		=	0x1,
	MPEG_SEQDIS_EXTID	=	0x2,
	MPEG_QMATRIX_EXTID	=	0x3,
	MPEG_COPYRIGHT_EXTID=	0x4,
	MPEG_SEQSCALA_EXTID	=	0x5,
	MPEG_PICDISP_EXTID	=	0x7,
	MPEG_PICCODE_EXTID	=	0x8,
	MPEG_PICSPA_EXTID	=	0x9,
	MPEG_PICTEM_EXTID	=	0xa
};

enum MPEGFRAMERATEenum {
	FRMRATE_FORBIDDEN	=	0,
	FRMRATE_23976		=	1,
	FRMRATE_24			=	2,
	FRMRATE_25			=	3,
	FRMRATE_2997		=	4,
	FRMRATE_30			=	5,
	FRMRATE_50			=	6,
	FRMRATE_5994		=	7,
	FRMRATE_60			=	8
};

enum CHROMAFMTenum {
	CHROMT_FMTRES	=	0,
	CHROMT_420		=	1,
	CHROMT_422		=	2,
	CHROMT_444		=	3
};

enum VIDEOFORMATenum {
	VID_COMPONENT,
	VID_PAL,
	VID_NTSC,
	VID_SECAM,
	VID_MAC,
	VID_UNSPECIFIC
};

enum PICCODINGTYPEenum {
	PIC_FORBIDDEN,
	PIC_INTRA,
	PIC_PREDICT,
	PIC_BI_PREDICT
};

enum MPEGPROFILEenum {
	PROFILE_HIGH	=	1,
	PROFILE_SPASCALA=	2,
	PROFILE_SNRSCALA=	3,
	PROFILE_MAIN	=	4,
	PROFILE_SIMPLE	=	5
};

enum MPEGLEVELenum {
	LEVEL_HIGH		=	4,
	LEVEL_HIGH1440	=	6,
	LEVEL_MAIN		=	8,
	LEVEL_LOW		=	10
};

#define DEFAULT_ASPECT_RATIO	1

enum COLOURPRIMARYenum {
	COLOUR_FORBIDDEN	=	0,
	ITU_R_BT_709		=	1,
	ITU_R_BT_470_M		=	4,
	ITU_R_BT_470_BG		=	5,
	SMPTE_170M			=	6,
	SMPTE_240M			=	7
};

enum MPEGINTRADCPRECISIONenum {
	MPEGDC_8BITS,
	MPEGDC_9BITS,
	MPEGDC_10BITS,
	MPEGDC_11BITS
};

enum MPEGPICSTRUCTenum {
	PICSTRUCT_TOPFIELD	=	1,
	PICSTRUCT_BTMFIELD	=	2,
	PICSTRUCT_FRAMEPIC	=	3
};

enum SCALABLEMODEenum {
	DATA_PARTITION	= 0,
	SPA_SCALABLE	= 1,
	SNR_SCALABLE	= 2,
	TEMP_SCALABLE	= 3
};

enum MACROBLOCKTYPEenum {
	MACROBLOCK_INTRA			=	1,
	MACROBLOCK_PATTERN			=	2,
	MACROBLOCK_MOTION_BACKWARD	=	4,
	MACROBLOCK_MOTION_FORWARD	=	8,
	MACROBLOCK_QUANT			=	16
};

enum MVFORMATenum {
	MVFORMAT_FIELD,
	MVFORMAT_FRAME
};

enum MOTIONTYPEenum {
	MOTION_FIELD	=	1,
	MOTION_FRAME	=	2,
	MOTION_16X8		=	2,
	MOTION_DMV		=	3
};

#define MPEG2_MBQUANT	16

#define VENDER_MARKER	"Produced by VIMicro!"

typedef struct tag_TMpeg2Option
{
//	char	flag;
	char	frmrate;
	char	pictype;
	char	frmfield;	
	char	dc_precision;
	char	repfield;
	char	proframe;
	int		srcwidth, srcheight;
	int		mb_width, mb_height;
	char	aspectratio;
	int		framerate;
	int		bitrate;
	int		vbvsize;
	char	intraqmat[64], nonintraqmat[64];
	char	chrointraqmat[64], chrononintraqmat[64];
	char	profile, level;
	char	progseq;
	char	chroma;
	char	vidfmt;
	char	clrdescrip;
	char	clrprim, transcharac, matcoeff;
	int		dispwidth, dispheight;
	int		vbvdelay;
	char	slbmode;
	char	fpfdct;
	char	concealmv;
	char	qscaletype;
	int		quantcode;
	char	intravlcfmt;
	char	alterscan;
	char	mbtype;
	int		mbaddrinc;
	char	motiontype;
	char	mvcnt;
	char	mvfmt;
	char	dmv;
	char	f_code[2][2];
	char	mvfldsel[2][2];
	char	mncode[2][2][2];
	char	mnresid[2][2][2];
	int		pmv[2][2][2];
	char	dmvector[2];
//	char	patcode[12];
	char	dcttype;
	int		cbp;
	int		vwc;
} TMpeg2Option, *PTMpeg2Option;

typedef struct tag_TMpeg2Table 
{
	unsigned char	*QTable;		/* Lum quantization tables */
} TMpeg2Table, *PTMpeg2Table;


class TOOLMPEG2I_DLL CMpeg2IBase : public CJmMatrix
{
public:
	CMpeg2IBase();

public:
	virtual int		getVwc(void);

protected:
	virtual void Open(void);
	void CheckOption(void);

protected:
	int		m_horwidth;
	int		m_vinc;
	TMpeg2Option	m_option;
	int		m_Ydc, m_Udc, m_Vdc;
};

#endif

