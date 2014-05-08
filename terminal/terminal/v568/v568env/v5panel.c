
#include "../driver/v5api.h"
#include "v5initenv.h"
#ifndef	V578CMD_DISABLE
#ifdef	WIN32

static TV5PanelContext	gPanelContext;

//#if V5_LCD_TYPE == V5_DEF_LCD_NULL_PANEL

const TPanelInfo gPanel0_Info =
{
	NULL,
	NULL
};

const TPanelCfg gPanel0_Cfg =
{
	NULL,
	NULL,
	NULL
};

//#elif V5_LCD_TYPE == V5_DEF_LCD_LTS180S3_HF1

const TLcdifParm gMainPanel1_LcdIF =
{
	V5B_LCDIF_CSFLAG_OFF,			//csmode
	0xffff,		//csflag
	0xfffa,		//rsflag
	3,			//head num
	{0x21, 0, 0x22},
	3, 4, 0, 0,
	0,			//config
	0,			//fmt
	0x460, 0x460,
	{0, 0},		//biasx, biasy
	0,								//process flag
	0, 0, 0, 0,						//cmd code stat/end x/y
	0, 0, 0, 0						//head repeat stat/end x/y
};

const TLcdcParm gMainPanel1_Lcdc = 
{
	{128, 160},
	PANEL_DITHER565,
	{
		0,		0,		1,		4,		9,		0x10,	0x1a,	0x27,	0x37,
		0x4a,	0x5f,	0x77,	0x91,	0xac,	0xc8,	0xe4,	0xff
	}
};

const TPanelParm gMainPanel1_Parm =
{
	MULTI_16 | REG_16 | RS_HIGH,												//type

	(PTLcdifParm)&gMainPanel1_LcdIF,
	(PTLcdcParm)&gMainPanel1_Lcdc
};

const TPanelInfo gPanel1_Info =
{
	(PTPanelParm)&gMainPanel1_Parm,
	NULL
};


const TReg gMainPanel1_wakefstd[] =
{
	{0x0,	0x01,	10},
	{0x3,	0x0,	10}
};

const TReg gMainPanel1_stdby[] =
{
	{0x7,	0x137,	0},
	{0x7,	0x136,	40},
	{0x7,	0x126,	40},
	{0x7,	0x4,	0},
	{0x3,	0x7fd,	0}
};

const TReg gMainPanel1_sleep[] =
{
	{0x7,	0x137,	0},
	{0x7,	0x136,	20},
	{0x7,	0x126,	20},
	{0x7,	0x4,	0},
	{0x3,	0x7fe,	0}
};

const TReg gMainPanel1_powoff[] =
{
	{0x7,	0x136,	20},
	{0x7,	0x126,	20},
	{0x7,	0x4,	0},
	{0x3,	0x7fc,	0}
};

const TReg gMainPanel1_powon[] =
{
	{0x7,	0x104,	0},
	{0xd,	0x100,	0},
	{0xe,	0xd18,	20},
	{0x1,	0x113,	0},
	{0x2,	0x700,	0},
	{0x5,	0x1230,	0},
	{0x6,	0x0,	0},
	{0xb,	0x4008,	0},
	{0xc,	0x0,	0},
	{0x3,	0x8,	0},
	{0x4,	0x0,	20},
	{0xe,	0x2d18,	0},
	{0xd,	0x110,	20},
	{0x21,	0x0,	0},

	{0x30,	0x700,	0},
	{0x31,	0x7,	0},
	{0x32,	0x0,	0},
	{0x33,	0x100,	0},
	{0x34,	0x707,	0},
	{0x35,	0x7,	0},
	{0x36,	0x700,	0},
	{0x37,	0x0007,	0},

	{0x0f,	0x0,	0},
	{0x11,	0x0,	0},
	{0x14,	0x5c00,	0},
	{0x15,	0x9f5d,	0},
	{0x16,	0x7f00,	0},
	{0x17,	0x9f00,	0},

	{0x3a,	0x1800,	0},
	{0x3b,	0x7,	5},

	{0x7,	0x105,	0},
	{0x7,	0x125,	0},
	{0x7,	0x127,	0},
	{0x7,	0x137,	0},

	{0x30,	0x707,	0},
	{0x31,	0x07,	0},
	{0x32,	0x00,	0},
	{0x33,	0x700,	0},
	{0x3a,	0x1f00,	0},

	{0x21,	0x4040,	0}
};

const TPanelAccess gMainPanel1_Access =
{
	{ sizeof(gMainPanel1_powon)  / sizeof(TReg), (PTReg)&gMainPanel1_powon  },	//power on
	{ sizeof(gMainPanel1_powoff) / sizeof(TReg), (PTReg)&gMainPanel1_powoff },	//power off
	{ sizeof(gMainPanel1_sleep)  / sizeof(TReg), (PTReg)&gMainPanel1_sleep  },	//sleep
	{ 0, NULL },																//stand by
	{ 0, NULL },																//wake up from stand by
	{ 0, NULL }																	//wake up from sleep
};

const TPanelCfg gPanel1_Cfg =
{
	(PTPanelAccess)&gMainPanel1_Access,
	NULL,
	NULL
};

//#elif V5_LCD_TYPE == V5_DEF_LCD_LTS180SH_H1I_X

const TLcdifParm gPanel2_LcdIF =
{
	V5B_LCDIF_CSFLAG_OFF,			//csmode
	0xffff,		//csflag
	0xffea,		//rsflag
	5,			//head num
	{0x20, 0, 0x21, 0, 0x22},
	3, 7, 0, 0,
	0,			//config
	0,			//fmt
	0x460, 0x460,
	{0x04, 0},
	0,								//process flag
	0, 0, 0, 0,						//cmd code stat/end x/y
	0, 0, 0, 0						//head repeat stat/end x/y
};

const TLcdcParm gPanel2_Lcdc = 
{
	{128, 160},
	PANEL_DITHER565,
	{
		0,		0,		1,		4,		9,		0x10,	0x1a,	0x27,	0x37,
		0x4a,	0x5f,	0x77,	0x91,	0xac,	0xc8,	0xe4,	0xff
	}
};

const TPanelParm gPanel2_Parm =
{
	MULTI_16 | REG_16 | RS_HIGH,	//type

	(PTLcdifParm)&gPanel2_LcdIF,
	(PTLcdcParm)&gPanel2_Lcdc
};

const TLcdifParm gSubPanel2_LcdIF =
{
	V5B_LCDIF_CSFLAG_OFF,			//csmode
	0xffff,		//csflag
	0xffea,		//rsflag
	5,			//head num
	{0x20, 0, 0x21, 0, 0x22},
	3, 7, 0, 0,
	0,			//config
	0,			//fmt
	0x460, 0x460,
	{0x24, 0xa0},
	0,								//process flag
	0, 0, 0, 0,						//cmd code stat/end x/y
	0, 0, 0, 0						//head repeat stat/end x/y
};

const TLcdcParm gSubPanel2_Lcdc = 
{
	{96, 96},
	PANEL_DITHER565,
	{
		0,		0,		1,		4,		9,		0x10,	0x1a,	0x27,	0x37,
		0x4a,	0x5f,	0x77,	0x91,	0xac,	0xc8,	0xe4,	0xff
	}
};

const TPanelParm gSubPanel2_Parm =
{
	MULTI_16 | REG_16 | RS_HIGH,	//type

	(PTLcdifParm)&gSubPanel2_LcdIF,
	(PTLcdcParm)&gSubPanel2_Lcdc
};

const TPanelInfo gPanel2_Info =
{
	(PTPanelParm)&gPanel2_Parm,
	(PTPanelParm)&gSubPanel2_Parm
};

const TReg gPanel2_powoff[] =
{
	{0x10,	0x200,	0},
	{0x12,	0x3F09,	0},
	{0x13,	0x121B,	0}
};

const TReg gPanel2_powon[] =
{
	{0x1,	0x300,	1},
	{0x2,	0x700,	1},
	{0x3,	0x30,	1},
	{0x4,	0x0,	1},
	{0x8,	0x626,	1},
	{0x9,	0x0,	1},
	{0xB,	0x0,	1},
	{0xC,	0x1,	1},
	{0xF,	0x0,	1},
	{0x7,	0x301,	20},

	{0x10,	0x200,	1},
	{0x11,	0x7,	1},
	{0x12,	0x3F09,	1},
	{0x13,	0x121B,	5},
	{0x10,	0x270,	1},
	{0x12,	0x3F19,	20},
	{0x13,	0x321B,	20},

	{0x23,	0x0,	1},
	{0x24,	0x0,	1},

	{0x50,	0x04,	1},
	{0x51,	0x83,	1},
	{0x52,	0x0,	1},
	{0x53,	0xff,	1},

	{0x60,	0x9300,	1},
	{0x61,	0x1,	1},
	{0x68,	0x0,	1},
	{0x69,	0x9f,	1},
	{0x6A,	0x0,	1},
	{0x70,	0x8B14,	1},
	{0x71,	0x1,	1},
	{0x78,	0xa0,	1},
	{0x79,	0xFF,	1},
	{0x7A,	0x0,	1},

	{0x80,	0x0,	1},
	{0x81,	0x0,	1},
	{0x82,	0x0,	1},
	{0x83,	0x0,	1},
	{0x84,	0x0,	1},
	{0x85,	0x0,	1},
	{0x86,	0x0,	1},
	{0x87,	0x0,	1},
	{0x88,	0x0,	20},

	{0x30,	0x700,	1},
	{0x31,	0x7,	1},
	{0x32,	0x0,	1},
	{0x33,	0x100,	1},
	{0x34,	0x707,	1},
	{0x35,	0x7,	1},
	{0x36,	0x700,	1},
	{0x37,	0x7,	1},
	{0x38,	0x1000,	1},
	{0x39,	0x001f,	1},

	{0x40,	0x700,	1},
	{0x41,	0x7,	1},
	{0x42,	0x0,	1},
	{0x43,	0x100,	1},
	{0x44,	0x707,	1},
	{0x45,	0x7,	1},
	{0x46,	0x700,	1},
	{0x47,	0x7,	1},
	{0x48,	0x1000,	1},
	{0x49,	0x001f,	1},

	{0x7,	0x301,	10},
	{0x7,	0x321,	1},
	{0x7,	0x323,	20},
	{0x7,	0x333,	40},

	{0x20,	0x04,	1},
	{0x21,	0x0,	1}
};

const TPanelAccess gMainPanel2_Access =
{
	{ sizeof(gPanel2_powon) / sizeof(TReg), (PTReg)&gPanel2_powon }, 		//power on
	{ sizeof(gPanel2_powoff) / sizeof(TReg), (PTReg)&gPanel2_powoff },	//power off
	{ 0, NULL },				//sleep
	{ 0, NULL },				//stand by
	{ 0, NULL },				//wake up from stand by
	{ 0, NULL },				//wake up from sleep
};

const TPanelCfg gPanel2_Cfg =
{
	(PTPanelAccess)&gMainPanel2_Access,
	(PTPanelAccess)&gMainPanel2_Access,
	NULL
};

//#elif V5_LCD_TYPE == V5_DEF_LCD_TRULY_CSTN_BJ2432E_8

const TLcdifParm gMainPanel3_LcdIF =
{
	V5B_LCDIF_CSFLAG_OFF,				//csmode
	0xffff,			//csflag
	0xffc0,			//rsflag
	6,				//head num
	{0x0043, 0x0000, 0x0000, 0x0042, 0x0000, 0x0000},
	3, 9, 5, 11,	//stxp, styp, endxp, endyp
	0,				//config
	0x04,			//fmt : 8bit bus/RGB565
	0x880, 0x7ca0,
	{0, 0x04},
	0,								//process flag
	0, 0, 0, 0,						//cmd code stat/end x/y
	0, 0, 0, 0						//head repeat stat/end x/y
};

const TLcdcParm gMainPanel3_Lcdc = 
{
	{128, 128},
	PANEL_DITHER565,
	{
		0,		0,		1,		4,		9,		0x10,	0x1a,	0x27,	0x37,
		0x4a,	0x5f,	0x77,	0x91,	0xac,	0xc8,	0xe4,	0xff
	}
};

const TPanelParm gMainPanel3_Parm =
{
	0,

	(PTLcdifParm)&gMainPanel3_LcdIF,
	(PTLcdcParm)&gMainPanel3_Lcdc
};

const TLcdifParm gSubPanel3_LcdIF =
{
	V5B_LCDIF_CSFLAG_OFF,			//csmode
	0xffff,		//csflag
	0xffea,		//rsflag
	5,			//head num
	{0x15, 0, 0x75, 0, 0x5c},
	3, 7, 0, 0, //stxp, styp, endxp, endyp
	0,			//config
	0x09,		//fmt : 8bit bus/RGB444
	0x880, 0x460,
	{0, 0},
	0,								//process flag
	0, 0, 0, 0,						//cmd code stat/end x/y
	0, 0, 0, 0						//head repeat stat/end x/y
};

const TLcdcParm gSubPanel3_Lcdc = 
{
	{96, 64},
	PANEL_DITHER444,
	{
		0,		0,		1,		4,		9,		0x10,	0x1a,	0x27,	0x37,
		0x4a,	0x5f,	0x77,	0x91,	0xac,	0xc8,	0xe4,	0xff
	}
};

const TPanelParm gSubPanel3_Parm =
{
	RS_HIGH,

	(PTLcdifParm)&gSubPanel3_LcdIF,
	(PTLcdcParm)&gSubPanel3_Lcdc
};

const TPanelInfo gPanel3_Info =
{
	(PTPanelParm)&gMainPanel3_Parm,
	(PTPanelParm)&gSubPanel3_Parm
};

const TReg gMainPanel3_powon[] =
{
	{0x2c,	0xffff, 25},	//STANDBY_MODE_OFF

	{0x02,	0x01,	25},	//DD_TURN_OSC_ON
	{0x20,	0x05,	0 },	//DD_POWER_ON_POWER_CIRCUIT
	{0x26,	0x01,	50},	//DCDC_AMP_ONOFF
	{0x26,	0x09,	25},
	{0x26,	0x0b,	25},
	{0x26,	0x0f,	25},
	{0x28,	0x00,	0 },
	{0x2e,	0xffff, 0 },
	{0x45,	0x00,	0 },
	{0x53,	0x00,	0 },
	{0x10,	0x02,	0 },
	{0x24,	0x22,	25},
	{0x30,  0x00,	25},
	{0x32,	0x0e,	0 },
	{0x40,	0x01,	0 },

	{0x42,	0x04,	0 },
	{0xffff,0x83,	0 },
	{0x43,	0x0,	0 },
	{0xffff,0x7f,	0 },
	{0x34,	0x10,	0 },
	{0x36,	0x00,	0 },
	{0x2a,	0xa0,	0 },
	{0x2b,	0x60,	0 },

	{0x22,	0x22,	0 }, 
	{0x55,	0x00,	0 },
	{0x56,	0x04,	0 },
	{0x57,	0x83,	0 },
	{0x59,	0x00,	0 },
	{0xffff,0x04,	0 },
	{0xffff,0x83,	0 },
	{0xffff,0x00,	0 },
	{0x51,	0x00,	0 }
};

const TReg gSubPanel3_powon[] =
{
	{0xd1,  0xffff, 0 },
	{0x20,  0x0b,	10},

	{0x81,  0x27,   0 },
	{0xffff,0x05,   0 },

	{0x94,	0xffff, 0 },

	{0xfb,	0x03,	0 },

	{0xf7,	0x28,	0 },
	{0xffff,0x2e,	0 },
	{0xffff,0x5,	0 },

	{0xbb,	0x01,	0 },

	{0xbc,	0x0,	0 },
	{0xffff,0x0,	0 },
	{0xffff,0x02,	0 },

	{0x15,	0x0,	0 },
	{0xffff,0x2f,	0 },

	{0x75,	0x0,	0 },
	{0xffff,0x3f,	0 },

	{0xca,	0x0,	0 },
	{0xffff,0xf,	0 },
	{0xffff,0x0,	0 },

	{0xf2,	0x02,	0 },
	{0xffff,0x06,	0 },

	{0x8e,	0x0,	0 },
	{0xffff,0x0,	0 },
	{0xffff,0x067,	0 },
	{0xffff,0x04f,	0 },

	{0xa2,	0xffff, 0 },
	{0xa7,	0xffff, 0 },
	{0xaf,	0xffff, 0 }
};

void Panel3_LightOn(void)
{
	UINT8 temp, state = 0;
	UINT16 i;

	V5B_CpmSetGpioCfg(0xff);
	V5B_CpmSetGpioMode(0xff);
	V5B_CpmSetGpioDirection(0xff);

	for(temp = 0; temp < 58; temp++)
	{
		if(state)
			V5B_CpmSetGpioData(0x00);
		else
			V5B_CpmSetGpioData(0x1f);

		state = state > 0 ? 0 : 1;
		for(i = 0; i < 0x100; i++);
	}

	V5B_CpmSetGpioData(0x1f);
}

const TPanelAccess gMainPanel3_Access =
{
	{ sizeof(gMainPanel3_powon) / sizeof(TReg), (PTReg)&gMainPanel3_powon },		//power on
	{ 0, NULL },				//power off
	{ 0, NULL },				//sleep
	{ 0, NULL },				//stand by
	{ 0, NULL },				//wake up from stand by
	{ 0, NULL },				//wake up from sleep
};

const TPanelAccess gSubPanel3_Access =
{
	{ sizeof(gSubPanel3_powon) / sizeof(TReg), (PTReg)&gSubPanel3_powon },		//power on
	{ 0, NULL },		//power off
	{ 0, NULL },		//sleep
	{ 0, NULL },		//stand by
	{ 0, NULL },		//wake up from stand by
	{ 0, NULL },		//wake up from sleep
};

const TPanelCfg gPanel3_Cfg =
{
	(PTPanelAccess)&gMainPanel3_Access,
	(PTPanelAccess)&gSubPanel3_Access,
	Panel3_LightOn
};

//#elif V5_LCD_TYPE == V5_DEF_LCD_MGG1256DTSW

const TLcdifParm gMainPanel4_LcdIF =
{
	V5B_LCDIF_CSFLAG_OFF,				//csmode
	0xffff,			//csflag
	0xffb6,			//rsflag
	7,				//head num
	{0x002a, 0x0000, 0x0000, 0x002b, 0x0000, 0x0000, 0x002c},
	3, 9, 5, 11,	//stxp, styp, endxp, endyp
	0,				//config
	0,
	0x460, 0x880,
	{0x04, 0},
	0,								//process flag
	0, 0, 0, 0,						//cmd code stat/end x/y
	0, 0, 0, 0						//head repeat stat/end x/y
};

const TLcdcParm gMainPanel4_Lcdc = 
{
	{128, 128},
	PANEL_DITHER565,
	{
		0,		0,		1,		4,		9,		0x10,	0x1a,	0x27,	0x37,
		0x4a,	0x5f,	0x77,	0x91,	0xac,	0xc8,	0xe4,	0xff
	}
};

const TPanelParm gMainPanel4_Parm =
{
	MULTI_16 | RS_HIGH,

	(PTLcdifParm)&gMainPanel4_LcdIF,
	(PTLcdcParm)&gMainPanel4_Lcdc
};

const TLcdifParm gSubPanel4_LcdIF =
{
	V5B_LCDIF_CSFLAG_OFF,							//csmode
	0xffff,						//csflag
	0xffb6,						//rsflag
	7,							//head num
	{0x002a, 0x0000, 0x0000, 0x002b, 0x0000, 0x0000, 0x002c},
	3, 9, 5, 11,				//stxp, styp, endxp, endyp
	V5B_LCDIF_CS1_SEL,			//config
	0x04,
	0x460, 0x880,
	{0x02, 0},
	0,								//process flag
	0, 0, 0, 0,						//cmd code stat/end x/y
	0, 0, 0, 0						//head repeat stat/end x/y
};

const TLcdcParm gSubPanel4_Lcdc = 
{
	{96, 64},
	PANEL_DITHER565,
	{
		0,  0,  1,  4,  9,  0x10, 0x1a, 0x27, 0x37,
		0x4a, 0x5f, 0x77, 0x91, 0xac, 0xc8, 0xe4, 0xff
	}
};

const TPanelParm gSubPanel4_Parm =
{
	RS_HIGH,

	(PTLcdifParm)&gSubPanel4_LcdIF,
	(PTLcdcParm)&gSubPanel4_Lcdc
};

const TPanelInfo gPanel4_Info =
{
	(PTPanelParm)&gMainPanel4_Parm,
	(PTPanelParm)&gSubPanel4_Parm
};

const TReg gMainPanel4_powon[] =
{
	{0x11,	0xffff, 10},

	{0xba,	0x07,   0 },
	{0xffff,0x18,	0 },

	{0xb6,	0x1a,   0 }, 
	{0xffff,0x29,	0 },
	{0xffff,0x38,	0 },

	{0xb4,	 0x01,  0 },
	{0xffff, 0x05,  0 },
	{0xffff, 0x11,  0 },
	{0xffff, 0x11,  0 },

	{0xb5,	 0x01,  0 },
	{0xffff, 0x04,  0 },
	{0xffff, 0x06,  0 },
	{0xffff, 0x08,  0 },

	{0x2a,	 0x00,  0 },
	{0xffff, 0x7f,  0 },

	{0x29,	 0xffff,0 },
	{0x25,	 0x54,  0 }
};

const TReg gSubPanel4_powon[] =
{
	{0x01,	0xffff, 10},
	{0xee,  0xffff, 0 },
	{0xb0,	0x4e,	0 },
	{0xc7,	0x0,	10},
	{0x25,	0x26,	0 },
	{0x2a,	0x02,	0 },
	{0xffff,0x61,	10},
	{0x2b,	0x00,	0 },
	{0xffff,0x3f,	0 },
	{0xc5,	0x38,	0 },
	{0xffff,0x20,	0 },
	{0xffff,0x25,	0 },
	{0xffff,0x1a,	0 },
	{0xffff,0x23,	0 },
	{0xc3,	0x33,	0 },
	{0xc4,	0x65,	0 },
	{0xc2,	0x01,	0 },
	{0xc9,	0x08,	0 },
	{0x3a,	0x05,	0 }, 
	{0x36,	0xc8,	0 },
	{0x21,	0xffff, 0 }, 
	{0x11,	0xffff, 0 }
};

const TPanelAccess gMainPanel4_Access =
{
	{ sizeof(gMainPanel4_powon) / sizeof(TReg), (PTReg)&gMainPanel4_powon },		//power on
	{ 0, NULL },				//power off
	{ 0, NULL },				//sleep
	{ 0, NULL },				//stand by
	{ 0, NULL },				//wake up from stand by
	{ 0, NULL },				//wake up from sleep
};

const TPanelAccess gSubPanel4_Access =
{
	{ sizeof(gSubPanel4_powon) / sizeof(TReg), (PTReg)&gSubPanel4_powon },		//power on
	{ 0, NULL },		//power off
	{ 0, NULL },		//sleep
	{ 0, NULL },		//stand by
	{ 0, NULL },		//wake up from stand by
	{ 0, NULL },		//wake up from sleep
};

const TPanelCfg gPanel4_Cfg =
{
	(PTPanelAccess)&gMainPanel4_Access,
	(PTPanelAccess)&gSubPanel4_Access,
	NULL
};

//#elif V5_LCD_TYPE == V5_DEF_LCD_PLT2001

const TLcdifParm gMainPanel5_LcdIF =
{
	V5B_LCDIF_CSFLAG_OFF,				//csmode
	0xffff,								//csflag
	0xfffa,								//rsflag
	4,									//head num
	{0x0018, 0x0000, 0x0019, 0x0000},
	3, 7, 0, 0,							//stxp, styp, endxp, endyp
	0,									//config
	0x00,								//fmt 
	0x461, 0x7ca0,
	{0, 0},
	0,								//process flag
	0, 0, 0, 0,						//cmd code stat/end x/y
	0, 0, 0, 0						//head repeat stat/end x/y
};

const TLcdcParm gMainPanel5_Lcdc = 
{
	{128, 160},
	0,
	{
		0,		0,		1,		4,		9,		0x10,	0x1a,	0x27,	0x37,
		0x4a,	0x5f,	0x77,	0x91,	0xac,	0xc8,	0xe4,	0xff
	}
};

const TPanelParm gMainPanel5_Parm =
{
	MULTI_16 | RS_HIGH,

	(PTLcdifParm)&gMainPanel5_LcdIF,
	(PTLcdcParm)&gMainPanel5_Lcdc
};

const TLcdifParm gSubPanel5_LcdIF =
{
	V5B_LCDIF_CSFLAG_OFF,							//csmode
	0xffff,											//csflag
	0xffb6,											//rsflag
	7,												//head num
	{0x0015,0x0000,0x0000,0x0075,0x0000,0x0000,0x005c},
	3, 9, 5, 11,									//stxp, styp, endxp, endyp
	V5B_LCDIF_CS1_SEL,								//config
	0x0c,
	0x881, 0x7ca0,
	{0x01, 0x04},
	0,								//process flag
	0, 0, 0, 0,						//cmd code stat/end x/y
	0, 0, 0, 0						//head repeat stat/end x/y
};

const TLcdcParm gSubPanel5_Lcdc = 
{
	{96, 64},
	PANEL_DITHER444,
	{
		0,		0,		1,		4,		9,		0x10,	0x1a,	0x27,	0x37,
		0x4a, 0x5f,		0x77,	0x91,	0xac,	0xc8,	0xe4,	0xff
	}
};

const TPanelParm gSubPanel5_Parm =
{
	RS_HIGH,

	(PTLcdifParm)&gSubPanel5_LcdIF,
	(PTLcdcParm)&gSubPanel5_Lcdc
};

const TPanelInfo gPanel5_Info =
{
	(PTPanelParm)&gMainPanel5_Parm,
	(PTPanelParm)&gSubPanel5_Parm
};


const TReg gMainPanel5_powon[] =
{
	{0x01, 0x02, 10},
	{0x02, 0x12, 10},
	{0x03, 0x60, 10},
	{0x05, 0x08, 10},
	{0x06, 0xc5, 10},
	{0x18, 0x00, 10},
	{0x19, 0x00, 10},
	{0x17, 0x00, 10}
};

const TReg gSubPanel5_powon[] =
{
	{0xd1,	0,		0},
	{0x20,  0xb,	10},

	{0x81,  0x20,	0},
	{0xffff,0x06,   0},
	{0x94,	0,		0},

	{0xfb,	0x03,	0},

	{0xf7,	0x28,	0},
	{0xffff,0x2e,	0},
	{0xffff,0x05,	0},

	{0xf2,	0x08,	0},
	{0xffff,0x06,	0},

	{0xbc,	0x3,	0},
	{0xffff,0x0,	0},
	{0xffff,0x02,	0},

	{0x15,	0x0,	0},
	{0xffff,0x2f,	0},

	{0x75,	0,		0},
	{0xffff,0x3f,	0},

	{0xbb, 0x1,		0},

	{0xca,	0,		0},
	{0xffff,0xf,	0},
	{0xffff,0,		0},

	{0xa2,	0,		0},
	{0xa7,	0,		0},
	{0xaf,	0,		0}
};

const TPanelAccess gMainPanel5_Access =
{
	{ sizeof(gMainPanel5_powon) / sizeof(TReg), (PTReg)&gMainPanel5_powon },		//power on
	{ 0, NULL },																//power off
	{ 0, NULL },																//sleep
	{ 0, NULL },																//stand by
	{ 0, NULL },																//wake up from stand by
	{ 0, NULL },																//wake up from sleep
};

const TPanelAccess gSubPanel5_Access =
{
	{ sizeof(gSubPanel5_powon) / sizeof(TReg), (PTReg)&gSubPanel5_powon },		//power on
	{ 0, NULL },																//power off
	{ 0, NULL },																//sleep
	{ 0, NULL },																//stand by
	{ 0, NULL },																//wake up from stand by
	{ 0, NULL },																//wake up from sleep
};

const TPanelCfg gPanel5_Cfg =
{
	(PTPanelAccess)&gMainPanel5_Access,
	(PTPanelAccess)&gSubPanel5_Access,
	NULL
};

//#elif V5_LCD_TYPE == V5_DEF_LCD_AMPIRE128160

const TLcdifParm gMainPanel6_LcdIF =
{
	V5B_LCDIF_CSFLAG_OFF,							//csmode
	0xffff,											//csflag
    0xfffa,		//rsflag
	3,			//head num
	{0x21, 0, 0x22},
	3, 4, 0, 0,
	0,		//config
	0,		//fmt
	0x460, 0x460,
	{2, 0},	//biasx, biasy
	0,								//process flag
	0, 0, 0, 0,						//cmd code stat/end x/y
	0, 0, 0, 0						//head repeat stat/end x/y
};

const TLcdcParm gMainPanel6_Lcdc = 
{
	{128, 160},
	PANEL_DITHER565,
	{
		0,		0,		1,		4,		9,		0x10,	0x1a,	0x27,	0x37,
		0x4a,	0x5f,	0x77,	0x91,	0xac,	0xc8,	0xe4,	0xff
	}
};

const TPanelParm gMainPanel6_Parm =
{
	MULTI_16 | RS_HIGH,

	(PTLcdifParm)&gMainPanel6_LcdIF,
	(PTLcdcParm)&gMainPanel6_Lcdc
};

const TLcdifParm gSubPanel6_LcdIF =
{
	V5B_LCDIF_CSFLAG_OFF,							//csmode
	0xffff,											//csflag
	0xffcc,  //rsflag
	6,   //head num
	{0x0,0x21, 0,0,0, 0x22},
	7, 5, 0, 0, //
	V5B_LCDIF_CS1_SEL,  //config
	0x04,  //fmt
	0x460,460,
	{0, 0}, //biasx, biasy
	0,								//process flag
	0, 0, 0, 0,						//cmd code stat/end x/y
	0, 0, 0, 0						//head repeat stat/end x/y
};

const TLcdcParm gSubPanel6_Lcdc = 
{
	{96, 64},
	PANEL_DITHER565,
	{
		0,		0,		1,		4,		9,		0x10,	0x1a,	0x27,	0x37,
		0x4a,	0x5f,	0x77,	0x91,	0xac,	0xc8,	0xe4,	0xff
	}
};

const TPanelParm gSubPanel6_Parm =
{
	RS_HIGH | REG_16,

	(PTLcdifParm)&gSubPanel6_LcdIF,
	(PTLcdcParm)&gSubPanel6_Lcdc
};

const TPanelInfo gPanel6_Info =
{
	(PTPanelParm)&gMainPanel6_Parm,
	(PTPanelParm)&gSubPanel6_Parm
};


const TReg gMainPanel6_powon[] =
{
	{0x0,	0x01,	20},
	{0xd,	0x0000,	20},
	{0xe,	0x0,		20},
	{0x7,	0x0,		20},
	{0xc,	0x2,		20},
	{0xd,	0x0202,	20},
	{0x4,	0x8000,	20},
	{0xe,	0xe1f,	20},
	
	{0x3,	0xc,		15},
	{0xe,	0x2e1f,	15},//
	{0xd,	0x0212,	15},
	{0x1,	0x0113,	15},//
	{0x2,	0x700,	15},//
	{0x5,	0x1030,	15},
	{0x6,	0x0000,	15},
	
	{0xb,	0x0000,	15},
	{0x21,	0x2,		15},
	{0x30,	0x700,	15},
	{0x31,	0x7,		15},
	{0x32,	0x0,		15},
	{0x33,	0x100,	15},
	{0x34,	0x707,	15},
	{0x35,	0x7,		15},
	{0x36,	0x700,	15},
	{0x37,	0x0001,	15},
	{0x0f,	0x2,		15},
	{0x11,	0x0,		15},
	{0x14,	0xaf00,	15},
	{0x15,	0xaf00,	15},
	{0x16,	0x8300,	15},
	{0x17,	0xaf00,	15},
	{0x3a,	0x1800,	15},
	{0x3b,	0x7,		15},
	{0x7,	0x105,	15},
	{0x7,	0x125,	15},
	{0x7,	0x127,	15},
	{0x7,	0x137,	15}
};

const TReg gSubPanel6_powon[] =
{
	{0x0,	0x01,	20},//osc start
	
	{0x03,	0x108c,	15},//power control 118c
	{0x0c,	0x0,		15},
	{0x04,	0x3ab0,	15},

	{0x01,	0x0207,	15},
	{0x02,	0,		15},
	{0x05,	0x0230,	15},
	{0x06,	0x0,		15},
	{0x0b,	0,		15},
	{0x14,	0x5300,	15},
	{0x15,	0x0000,	15}, 
	{0x16,	0x5f00,	15},
	{0x17,	0x3f00,	15},
	{0x20,	0x0000,	15},
	{0x07,	0x0002,	15},
	{0x07,	0x0003,	15},
	{0x21,	0,		15}
};

const TPanelAccess gMainPanel6_Access =
{
	{ sizeof(gMainPanel6_powon) / sizeof(TReg), (PTReg)&gMainPanel6_powon },	//power on
	{ 0, NULL },																//power off
	{ 0, NULL },																//sleep
	{ 0, NULL },																//stand by
	{ 0, NULL },																//wake up from stand by
	{ 0, NULL },																//wake up from sleep
};

const TPanelAccess gSubPanel6_Access =
{
	{ sizeof(gSubPanel6_powon) / sizeof(TReg), (PTReg)&gSubPanel6_powon },		//power on
	{ 0, NULL },																//power off
	{ 0, NULL },																//sleep
	{ 0, NULL },																//stand by
	{ 0, NULL },																//wake up from stand by
	{ 0, NULL },																//wake up from sleep
};

const TPanelCfg gPanel6_Cfg =
{
	(PTPanelAccess)&gMainPanel6_Access,
	(PTPanelAccess)&gSubPanel6_Access,
	NULL
};

const TLcdifParm gMainPanel7_LcdIF =
{
	V5B_LCDIF_CSFLAG_OFF,			//csmode
	0xffff,		//csflag
	0xffea,		//rsflag
	5,			//head num
	{0x200, 0, 0x201, 0, 0x202},
	3, 0x0807, 0, 0,
	0,			//config
	0,			//fmt
	0x460, 0x460,
	{0, 0},		//biasx, biasy
	0,								//process flag
	0, 0, 0, 0,						//cmd code stat/end x/y
	0, 0, 0, 0						//head repeat stat/end x/y
};

const TLcdcParm gMainPanel7_Lcdc = 
{
	{240, 320},
	PANEL_DITHER565,
	{
		0,		0,		1,		4,		9,		0x10,	0x1a,	0x27,	0x37,
		0x4a,	0x5f,	0x77,	0x91,	0xac,	0xc8,	0xe4,	0xff
	}
};

const TPanelParm gMainPanel7_Parm =
{
	MULTI_16 | REG_16 | RS_HIGH,												//type
	(PTLcdifParm)&gMainPanel7_LcdIF,
	(PTLcdcParm)&gMainPanel7_Lcdc
};

const TPanelInfo gPanel7_Info =
{
	(PTPanelParm)&gMainPanel7_Parm,
	NULL
};


const TReg gMainPanel7_powon[] =
{
	{0x0111,	0x0474,	0},
	{0x0110,	0x0101,	20},
	{0x0111,	0x02de,	0},
	{0x0110,	0x0102,	20},
	{0x0111,	0x0c74,	0},
	{0x0110,	0x0101,	20},
	{0x0050,	0x0100,	20},
	{0x0111,	0x080c,	0},
	{0x0110,	0x0100,	20},
	{0x0111,	0x029b,	0},
	{0x0110,	0x0102,	20},
	{0x0111,	0x04d1,	0},
	{0x0110,	0x0101,	20},
	{0x0111,	0x04e4,	0},
	{0x0110,	0x0101,	20},
	{0x0111,	0x0008,	0},
	{0x0110,	0x0103,	20},
	{0x0111,	0x06c0,	0},
	{0x0110,	0x0106,	20},
	{0x0111,	0x0c0a,	0},
	{0x0110,	0x0100,	20},
	{0x100,		0x0110,	20},
	{0x0007,	0x0001,	20},
	{0x0007,	0x0012,	20},
	{0x0111,	0x0c08,	0},
	{0x0110,	0x0001,	20},
	{0x0007,	0x0013,	20},
	{0x0007,	0x0113,	20},
	{0x0001,	0x0100,	20},
	{0x0002,	0x0700,	20},
	{0x0003,	0x1130,	20},
	{0x0004,	0x0000,	20},
	{0x0005,	0x0000,	20},
	{0x0008,	0x0f03,	20},
	{0x0009,	0x142c,	20},
	{0x000B,	0x0008,	20},
	{0x000C,	0x0001,	20},
	{0x000D,	0x0010,	20},
	{0x000e,	0x0210,	20},
	{0x000f,	0x0000,	20},
	{0x0010,	0x0000,	20},
	{0x0011,	0x0000,	20},
	{0x0012,	0x0002,	20},
	{0x0013,	0x0000,	20},
	{0x0015,	0x0300,	20},
	{0x0016,	0x0440,	20},
	{0x0017,	0x0000,	20},
	{0x0018,	0x0007,	20},
	{0x0019,	0x0000,	20},
	{0x001b,	0x0300,	20},
	{0x0200,	0x0000,	20},
	{0x0201,	0x0000,	20},
	{0x0210,	0x0000,	20},
	{0x0211,	0x00ef,	20},
	{0x0212,	0x0000,	20},
	{0x0213,	0x013f,	20},
	{0x0400,	0x0027,	20},
	{0x0401,	0x0001,	20},
	{0x0402,	0x0000,	20},
	{0x0403,	0x013f,	20},
	{0x0404,	0x0000,	20},
	{0x0405,	0x0000,	20},
	{0x0406,	0x0000,	20},
	{0x0304,	0x1409,	20},
	{0x0309,	0x0602,	20},
	{0x0300,	0x0003,	20},
	{0x0301,	0x4000,	20},
	{0x0302,	0x0407,	20},
	{0x0303,	0x0305,	20},
	{0x0305,	0x0704,	20},
	{0x0306,	0x0407,	20},
	{0x0307,	0x0503,	20},
	{0x202,		0xffff,	20}
};

const TPanelAccess gMainPanel7_Access =
{
	{ sizeof(gMainPanel7_powon)  / sizeof(TReg), (PTReg)&gMainPanel7_powon  },	//power on
	{ 0, NULL },																															//power off
	{ 0, NULL },																															//sleep
	{ 0, NULL },																															//stand by
	{ 0, NULL },																															//wake up from stand by
	{ 0, NULL }																																//wake up from sleep
};

const TPanelCfg gPanel7_Cfg =
{
	(PTPanelAccess)&gMainPanel7_Access,
	NULL,
	NULL
};

const TLcdifParm gMainPanel8_LcdIF =
{
	V5B_LCDIF_CSFLAG_OFF,			//csmode
	0xffff,		//csflag
	0xfff0,		//rsflag
	4,			//head num
	{0x0600, 0, 0x0700, 0},
	3, 0x0807, 0, 0,
	0,			//config
	0,			//fmt
	0x460, 0x460,
	{0, 0},		//biasx, biasy
	0,								//process flag
	0, 0, 0, 0,						//cmd code stat/end x/y
	0, 0, 0, 0						//head repeat stat/end x/y
};

const TLcdcParm gMainPanel8_Lcdc = 
{
	{240, 320},
	PANEL_DITHER565,
	{
		0,		0,		1,		4,		9,		0x10,	0x1a,	0x27,	0x37,
		0x4a,	0x5f,	0x77,	0x91,	0xac,	0xc8,	0xe4,	0xff
	}
};

const TPanelParm gMainPanel8_Parm =
{
	MULTI_16 | REG_16,												//type
	(PTLcdifParm)&gMainPanel8_LcdIF,
	(PTLcdcParm)&gMainPanel8_Lcdc
};

const TPanelInfo gPanel8_Info =
{
	(PTPanelParm)&gMainPanel8_Parm,
	NULL
};

const TReg gMainPanel8_powon[] =
{
	{0x0200,  0xffff, 0},
	{0x0301,  0xffff, 0},

	{0x4101,  0xffff,  5},	//250us

	{0x0040, 0xffff, 0},	//{0x0040, 0xffff, 0},

	{0x4e20, 0xffff, 200},	//10MS
	{0x4e60, 0xffff, 0},

	{0x0100, 0xffff, 1},	//51us
	{0x2201, 0xffff, 1},	//51us
	{0x2200, 0xffff, 334},	//16.7ms

	{0x0102, 0xffff, 0},
	{0x2757, 0xffff, 5},	//250us

	{0x2860, 0xffff, 5},	//250us
	{0x2967, 0xffff, 5},	//250us

	{0x2a00, 0xffff, 5},	//250us

	{0x3c00, 0xffff, 5},	//250us
	{0x3d01, 0xffff, 5},	//250us
	{0x3e59, 0xffff, 5},	//250us

	{0x3f87, 0xffff, 5},	//250us

	{0x402f, 0xffff, 5},	//250us


	{0x4b03, 0xffff, 0},
	{0x4c01, 0xffff, 0},
	{0x4f04, 0xffff, 0},
	{0x5026, 0xffff, 0},
	{0x5319, 0xffff, 0},
	{0x5422, 0xffff, 0},
	{0x550f, 0xffff, 0},
	{0x5618, 0xffff, 0},
	{0x5705,0xffff, 0},
	{0x580e, 0xffff, 0},

	{0x34b0, 0xffff, 0},
	{0x3006, 0xffff, 0},
	{0x3500, 0xffff, 0},
	{0x3170, 0xffff, 0},
	{0x3377, 0xffff, 0},
	{0x3277, 0xffff, 0},
	{0x3777, 0xffff, 0},
	{0x3604, 0xffff, 0},

	{0x0500, 0xffff, 0},	//{0x0500, 0xffff, 0 
	//r5:D7 DTX1 The data bus of the display data at the time of 16-bit parallel data transfer inputted is set.
	//DTX1 = L: 1-pixel/16-bit mode
	//DTX1 = H: 1-pixel/18-bit mode
	{0x0600, 0x0000, 0},
	{0x0700, 0x0000, 0},

	{0x4d10, 0xffff, 0},
	{0x26ef, 0xffff, 200},

	{0x4d00, 0xffff, 0},
	{0x5d18, 0xffff, 340},

	{0x0112, 0xffff, 0},	//{0x0112, 0xffff, 0}
	{0x3d05, 0xffff, 5},

	{0x4d28, 0xffff, 0},	//0x4d28
	{0x5df8, 0xffff, 1800},

	{0x3d07, 0xffff, 5},
	{0x0000, 0xffff, 0}
};

const TPanelAccess gMainPanel8_Access =
{
	{ sizeof(gMainPanel8_powon)  / sizeof(TReg), (PTReg)&gMainPanel8_powon  },	//power on
	{ 0, NULL },																															//power off
	{ 0, NULL },																															//sleep
	{ 0, NULL },																															//stand by
	{ 0, NULL },																															//wake up from stand by
	{ 0, NULL }																																//wake up from sleep
};

const TPanelCfg gPanel8_Cfg =
{
	(PTPanelAccess)&gMainPanel8_Access,
	NULL,
	NULL
};

//#elif V5_LCD_TYPE == V5_DEF_LCD_UPD161690

const TLcdifParm gMainPanel9_LcdIF =
{
	V5B_LCDIF_CSFLAG_OFF,			//csmode
	0xffff,							//csflag
	0xffea,							//rsflag
	5,								//head num
	{0x03, 0, 0x05, 0, 0x0b},
	3, 7, 0, 0,
	0,								//config
	0x08,								//fmt
	0x881, 0x460,
	{0, 0},							//biasx, biasy
	0,								//process flag
	0, 0, 0, 0,						//cmd code stat/end x/y
	0, 0, 0, 0						//head repeat stat/end x/y
};

const TLcdcParm gMainPanel9_Lcdc = 
{
	{128, 160},
	PANEL_DITHER444,
	{
		0,		0,		1,		4,		9,		0x10,	0x1a,	0x27,	0x37,
			0x4a,	0x5f,	0x77,	0x91,	0xac,	0xc8,	0xe4,	0xff
	}
};

const TPanelParm gMainPanel9Parm =
{
	RS_HIGH,												//type

	(PTLcdifParm)&gMainPanel9_LcdIF,
	(PTLcdcParm)&gMainPanel9_Lcdc
};

const TPanelInfo gPanel9_Info =
{
	(PTPanelParm)&gMainPanel9Parm,
	NULL
};

const TReg gMainPanel9_powon[] =
{
	{0x21, 0x20, 2}, 
	{0x02, 0x01, 2},
	{0x20, 0x01, 2},
	{0x20, 0x03, 2}, 
	{0x20, 0x07, 2}, 
	{0x20, 0x0f, 2},//0x0f

	{0x21, 0x21, 2}, 
	{0x21, 0x23, 2}, 
	{0x21, 0x27, 2}, 

	{0x24, 0xf7, 2}, //c1
	{0x25, 0x50, 2}, //f3
	{0x26, 0x01, 2}, //
	{0x29, 0x30, 2}, //30


	{0x00,    0x00,      20},
	{0x01,    0x20,      2},

	{0x06, 0x00, 2},//

	{0x00, 0x40, 2},
	{0x03,    0x00,  2},
	{0x05, 0x00, 2}
};

const TPanelAccess gMainPanel9Access =
{
	{ sizeof(gMainPanel9_powon)  / sizeof(TReg), (PTReg)&gMainPanel9_powon  },	//power on
	{ 0, NULL },																//power off
	{ 0, NULL  },																//sleep
	{ 0, NULL },																//stand by
	{ 0, NULL },																//wake up from stand by
	{ 0, NULL }																	//wake up from sleep
};

const TPanelCfg gPanel9_Cfg =
{
	(PTPanelAccess)&gMainPanel9Access,
	NULL,
	NULL
};

//elif V5_LCD_TYPE == V5_DEF_LCD_TFT_GG3216UTSW_W

const TLcdifParm gMainPanel10_LcdIF =
{
	V5B_LCDIF_CSFLAG_OFF,			//csmode
	0xffff,		//csflag
	0xfffa,		//rsflag
	3,			//head num
	{0x21, 0, 0x22},
	3, 4, 0, 0,
	0,			//config
	0,			//fmt
	0x460, 0x460,
	{4, 0},		//biasx, biasy
	0,								//process flag
	0, 0, 0, 0,						//cmd code stat/end x/y
	0, 0, 0, 0						//head repeat stat/end x/y
};

const TLcdcParm gMainPanel10_Lcdc = 
{
	{128, 160},
	PANEL_DITHER565,
	{
		0,		0,		1,		4,		9,		0x10,	0x1a,	0x27,	0x37,
		0x4a,	0x5f,	0x77,	0x91,	0xac,	0xc8,	0xe4,	0xff
	}
};

const TPanelParm gMainPanel10_Parm =
{
	MULTI_16 | REG_16 | RS_HIGH,												//type

	(PTLcdifParm)&gMainPanel10_LcdIF,
	(PTLcdcParm)&gMainPanel10_Lcdc
};

const TPanelInfo gPanel10_Info =
{
	(PTPanelParm)&gMainPanel10_Parm,
	NULL
};

const TReg gMainPanel10_powon[] =
{
	{ 0x00, 0x0001, 20 },
	{ 0x0e, 0x0a19, 20 },
	{ 0x0c, 0x0000, 20 },
	{ 0x0d, 0x0202, 20 },
	{ 0x04, 0x0000, 20 },
	{ 0x3b, 0x0007, 20 },
	{ 0x3a, 0x1800, 20 },
	{ 0x07, 0x0104, 20 },
	{ 0x01, 0x0113, 20 },
	{ 0x02, 0x0700, 20 },
	{ 0x05, 0x1030, 20 },
	{ 0x0b, 0x4000, 20 },
	{ 0x03, 0x000c, 100 },
	{ 0x0e, 0x2c17, 2 },
	{ 0x0d, 0x0415, 100 },
	{ 0x30, 0x0700, 20 },
	{ 0x31, 0x0007, 20 },
	{ 0x32, 0x0000, 20 },
	{ 0x33, 0x0500, 20 },
	{ 0x34, 0x0707, 20 },
	{ 0x35, 0x0007, 20 },
	{ 0x36, 0x0700, 20 },
	{ 0x37, 0x0001, 20 },
	{ 0x0f, 0x0002, 20 },
	{ 0x11, 0x0000, 20 },
	{ 0x14, 0x5c00, 20 },
	{ 0x16, 0x8304, 20 },
	{ 0x17, 0x9F00, 50 },
	{ 0x07, 0x0105, 40 },
	{ 0x07, 0x0125, 20 },
	{ 0x07, 0x0127, 40 },
	{ 0x07, 0x0137, 20 },
	{ 0x21, 0x0004, 20 },
	{ 0x22, 0xffff, 20 }
};

const TPanelAccess gMainPanel10_Access =
{
	{ sizeof(gMainPanel10_powon)  / sizeof(TReg), (PTReg)&gMainPanel10_powon  },	//power on
	{ 0, NULL },																	//power off
	{ 0, NULL },																	//sleep
	{ 0, NULL },																	//stand by
	{ 0, NULL },																	//wake up from stand by
	{ 0, NULL }																		//wake up from sleep
};

const TPanelCfg gPanel10_Cfg =
{
	(PTPanelAccess)&gMainPanel10_Access,
	NULL,
	NULL
};

//#else
//
//#error Please select panel type
//
//#endif

void V5_SetCurPanel(UINT8 idx)
{
	gPanelContext.cur_panel = idx;
}

UINT8 V5_GetCurPanel(void)
{
	return gPanelContext.cur_panel;
}

// All panel info
const PTPanelInfo gW32PanelInfo[] = {
	(PTPanelInfo)&gPanel0_Info,		// V5_DEF_LCD_NULL_PANEL
	(PTPanelInfo)&gPanel1_Info,		// V5_DEF_LCD_LTS180S3_HF1
	(PTPanelInfo)&gPanel2_Info,		// V5_DEF_LCD_LTS180SH_H1I_X
	(PTPanelInfo)&gPanel3_Info,		// V5_DEF_LCD_TRULY_CSTN_BJ2432E_8
	(PTPanelInfo)&gPanel4_Info,		// V5_DEF_LCD_MGG1256DTSW
	(PTPanelInfo)&gPanel5_Info,		// V5_DEF_LCD_PLT2001
	(PTPanelInfo)&gPanel6_Info,		// V5_DEF_LCD_AMPIRE128160
	(PTPanelInfo)&gPanel7_Info,		// V5_DEF_LCD_HD66781S
	(PTPanelInfo)&gPanel8_Info,		// V5_DEF_LCD_UPD161801_24
	(PTPanelInfo)&gPanel9_Info,		// V5_DEF_LCD_UPD161690
	(PTPanelInfo)&gPanel10_Info		// V5_DEF_LCD_TFT_GG3216UTSW_W
};

// All panel config
const PTPanelCfg gW32PanelCfg[] = {
	(PTPanelCfg)&gPanel0_Cfg,		// V5_DEF_LCD_NULL_PANEL
	(PTPanelCfg)&gPanel1_Cfg,		// V5_DEF_LCD_LTS180S3_HF1
	(PTPanelCfg)&gPanel2_Cfg,		// V5_DEF_LCD_LTS180SH_H1I_X
	(PTPanelCfg)&gPanel3_Cfg,		// V5_DEF_LCD_TRULY_CSTN_BJ2432E_8
	(PTPanelCfg)&gPanel4_Cfg,		// V5_DEF_LCD_MGG1256DTSW
	(PTPanelCfg)&gPanel5_Cfg,		// V5_DEF_LCD_PLT2001
	(PTPanelCfg)&gPanel6_Cfg,		// V5_DEF_LCD_AMPIRE128160
	(PTPanelCfg)&gPanel7_Cfg,		// V5_DEF_LCD_HD66781S
	(PTPanelCfg)&gPanel8_Cfg,		// V5_DEF_LCD_UPD161801_24
	(PTPanelCfg)&gPanel9_Cfg,		// V5_DEF_LCD_UPD161690
	(PTPanelCfg)&gPanel10_Cfg		// V5_DEF_LCD_TFT_GG3216UTSW_W
};

#endif // WIN32
#endif //V578CMD_DISABLE
