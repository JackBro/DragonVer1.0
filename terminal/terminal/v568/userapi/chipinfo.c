
#include "internal.h"
#ifndef	V578CMD_DISABLE

#ifdef WIN32
TCtrlInfo gCtrlInfo = {
#else
const TCtrlInfo gCtrlInfo = {
#endif
	V5_BUS_MULTI16, //bustype
	0x23,        //Interrupt control: low lever, 2 clock width
	0, //clkcfgtype 0
	24000, //clkin
	48000, //mclk
	32,//Pll m(must >= 2, need substract 2 when set into register)
	2, //Pll n(must >= 2, need substract 2 when set into register)
	1, //Pll od0
	0, //Pll od1
	2, //Clk output div (must >= 2 ,mclk = clkout/2)
    0x90,//bypass register address

#if V5_BUS_MODE == V5_BUS_MODE0
	{0x10, 0x10, 0x3, 0x3}, //dlysel l_yong not config
#endif

#if V5_BUS_MODE == V5_BUS_MODE1
	{0x10, 0x10, 0x3, 0x3}, //dlysel l_yong not config
#endif

#if V5_BUS_MODE == V5_BUS_MODE2
	{0x10, 0x10, 0x3, 0x3}, //dlysel l_yong not config
#endif

#if V5_BUS_MODE == V5_BUS_MODE3
	{0x10, 0x10, 0x3, 0x3}, //dlysel l_yong not config
#endif

};

const TMarbInfo gMarbInfo =
{
	0x400, //Avi index memory size
	ONEFRAME_ONLY, //Memory alloc mode
	12, //Muti thumb count.
	1, //fifo count interrupt occur data size/ memory size
	MARB_POWER_SAVE_IO //MARB_POWER_SAVE_CLK
};

const TJpegInfo gJpegInfo = {
	BRC_EN|BRC_RUDE_UV_ONLY, //see JPEGBRCCTRLenum.
	0x78, //qualitify factor
	0x29, //target compress rate, must >= 5, act com rate = (tcr-1)/4
	{ //Rude brc
		//	CompLevel[6]
			{0x3f,	0x1f,	0x0f,	0x08,	0x04,	0x02},
		//	RudeAc
			0x00,
		//	RudeLevel
			0x00,
		//	ThdWc
			0x00
	},
	{ //Enh brc
		//	thdr1,	thdr2,	thdl1,	thdl2,	shfr1,	shfr2,	shfl1,	shfl2,
			0x00,	0x00,	0x00,	0x00,	0x00,	0x00,	0x00,	0x00
	},
	{
		0x00, 0x04
	}

};

#endif //V578CMD_DISABLE
