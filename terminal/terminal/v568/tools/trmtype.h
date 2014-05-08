#ifndef _TERMINAL_TOOLS_TYPE_H_
#define _TERMINAL_TOOLS_TYPE_H_


#include "type.h"

	//Ipp factor
	typedef struct tag_TV5BIppFactor
	{
		UINT8	intValue; //integer part of factor
		UINT16	decValue; //decimal part of factor
	} TV5BIppFactor, *PTV5BIppFactor;

	typedef struct tag_TAeAdjust
	{
		UINT8	ymean;
		UINT8	ytarget;
		UINT8	ythd;
		UINT8	ytop;
		UINT8	gain;
		UINT8	gtarget;
		UINT8	gthd;
		UINT8	gstep;
		UINT32	et;
		UINT32	newet;
		UINT32	snrclock;
		UINT32	freq;
		UINT8	maxet;
		UINT8	minet;
		UINT8	maxgain;
		UINT8	mingain;
		UINT8	speed;
	} TAeAdjust, *PTAeAdjust;

	enum ALAYERFORMATenum {
		ALAYER_LINEBUF		=	0,
		ALAYER_2FRAMEBUF	=	2,
		ALAYER_1FRAMEBUF
	};

	//lbuf yuv mode
	enum V5B_LBUF_YUVenum
	{
		V5B_LBUF_YUV_422	= 0x0,
		V5B_LBUF_YUV_420	= 0x1,
		V5B_LBUF_YUV_411	= 0x2,
		V5B_LBUF_YUV_400	= 0x3,
		V5B_LBUF_YUV_444	= 0x4
	};

	typedef struct tag_TAeStep {
		UINT8 step;				//if et change<step,et only change 1
		UINT8 div;				//if et change > setp, et change newet-et
	} TAeStep, *PTAeStep;

	enum STARTPTSELenum {
		UPLEFT,
		UPRIGHT,
		BTMLEFT,
		BTMRIGHT
	};

	enum BLAYERFORMATenum {
		BLAYER_RGB1,
		BLAYER_RGB2,
		BLAYER_RGB4,
		BLAYER_RGB8,
		BLAYER_RGB444,
		BLAYER_RGB555,
		BLAYER_RGB565,
		BLAYER_RGB666,
		BLAYER_RGB24,
		BLAYER_RGB32
	};

#endif


