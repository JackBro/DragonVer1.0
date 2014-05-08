
#ifndef _TOOLS_MPEG4BASE_H_
#define _TOOLS_MPEG4BASE_H_


#ifndef TOOLMPEG4_DLL
	#define TOOLMPEG4_DLL  __declspec(dllimport)
#endif

#include "tooljpg/jmmatrix.h"
#include "mpeg4base.h"


#define CHROMA_420		1
#define DEFAULT_MBS		1200		// 640 x 480 resolution
#define MBPRED_SIZE		15

enum MPEG4STARTCODEenum {
	VIDEO_OBJECT_MIN_START_CODE		=	0x00,
	VIDEO_OBJECT_MAX_START_CODE		=	0x1F,
	VIDEO_OBJECT_LAYER_MIN_CODE		=	0x20,
	VIDEO_OBJECT_LAYER_MAX_CODE		=	0x2F,
	VISUAL_OBJECT_SEQ_START_CODE	=	0xB0,
	VISULA_OBJECT_SEQ_END_CODE		=	0xB1,
	USER_DTAT_START_CODE			=	0xB2,
	GROUP_OF_VOP_START_CODE			=	0xB3,
	VIDEO_SESSION_ERROR_CODE		=	0xB4,
	VISUAL_OBJECT_START_CODE		=	0xB5,
	VOP_START_CODE					=	0xB6

};

enum VISUALOBJECTTYPEenum {
	VIDEO_ID			=	0x01,
	STILL_TEXTURE_ID	=	0x02,
	MESH_ID				=	0x03,
	FBA_ID				=	0x04,
	MESH_3D_ID			=	0x05
};

enum VIDOBJLAYSHAPEenum {
	SHAPE_RECT			=	0,
	SHAPE_BINARY		=	1,
	SHAPE_BINARY_ONLY	=	2,
	SHAPE_GRAYSCALE		=	3
};

enum VOPCODINGTYPEenum {
	I_VOP,
	P_VOP,
	B_VOP,
	SPRITE,
	N_VOP
};

enum MACROBLOCKMODEenum {
	MODE_INTER		=	0,
	MODE_INTER_Q	=	1,
	MODE_INTER4V	=	2,
	MODE_INTRA		=	3,
	MODE_INTRA_Q	=	4,
	MODE_STUFFING	=	7,
	MODE_NOT_CODED	=	16
};

typedef struct tag_TVector
{
	int		x;
	int		y;
} TVector, *PTVector;

typedef struct tag_TMacroBlock
{
	TVector mv[4];
	int		pred_direct[6];
	int		pred_val[6][MBPRED_SIZE];
	int		quant;
	int		mbmode;
}TMacroBlock, *PTMacroBlock;

typedef struct tag_TMpeg4Option
{
	char	shortvidhdr;
	int		srcwidth;
	int		srcheight;
	int		edgedwidth;
	int		edgedheight;
	int		mb_width;
	int		mb_height;
	int		chroma;
	int		profile;
	int		vwc;

	char	vo_verid;
	char	vol_verid;
	char	vo_prio;
	char	vo_type;
	char	videofmt;
	char	videorange;
	char	vidobjtype;
	char	lowdelay;
	char	vbvparam;
	char	vidshape;
	int		timeinc;
	int		timeinc_bits;
	char	interlaced;
	char	quant_bits;
	char	quant_type;
	char	quanterpel;
	char	scalability;
	unsigned char	intraqmat[64], interqmat[64];

	char	voptype;
	char	roundtype;
	char	intradcthr;
	char	topfield;
	char	alter_ver_scan;
	int		quant;
	char	fcode_forward;
	char	fcode_backward;

	char	notcoded;
	int		mcbpc;
	int		mbmode;
	int		cbpc;
	char	acpred;
	int		cbpy;
	int		cbp;
	char	dquant;
	char	dcttype;
	char	field_pred;
	char	field_fortop;
	char	field_forbot;
	char	field_baktop;
	char	field_bakbot;


} TMpeg4Option, *PTMpeg4Option;


class TOOLMPEG4_DLL CMpeg4Base : public CJmMatrix
{
public:
	CMpeg4Base();
	virtual ~CMpeg4Base();

public:
	virtual int		GetVwc(void);

protected:
	virtual void	Open(void);
	void			CheckOption(void);

protected:
	int				m_vinc;
	int				m_horwidth;
	TMpeg4Option	m_option;
	int				m_Ydc, m_Udc, m_Vdc;
	TMacroBlock		*m_mbs;
};


#endif