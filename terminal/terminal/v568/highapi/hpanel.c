#include "internal.h"
#ifndef	V578CMD_DISABLE


#if V5_LCD_TYPE == V5_DEF_LCD_LTS180S3_HF1

const TReg gPanel_wakefstd[] =
{
	{0x0,	0x01,	10},
	{0x3,	0x0,	10}
};

const TReg gPanel_stdby[] =
{
	{0x7,	0x137,	0},
	{0x7,	0x136,	40},
	{0x7,	0x126,	40},
	{0x7,	0x4,	0},
	{0x3,	0x7fd,	0}
};

const TReg gPanel_sleep[] =
{
	{0x7,	0x137,	0},
	{0x7,	0x136,	20},
	{0x7,	0x126,	20},
	{0x7,	0x4,	0},
	{0x3,	0x7fe,	0}
};

const TReg gPanel_powoff[] =
{
	{0x7,	0x136,	20},
	{0x7,	0x126,	20},
	{0x7,	0x4,	0},
	{0x3,	0x7fc,	0}
};

const TReg gPanel_powon[] =
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

const TPanelParm gPanelParm =
{
	{ sizeof(gPanel_powon)  / sizeof(TReg), (PTReg)&gPanel_powon  },	//power on
	{ sizeof(gPanel_powoff) / sizeof(TReg), (PTReg)&gPanel_powoff },	//power off
	{ sizeof(gPanel_sleep)  / sizeof(TReg), (PTReg)&gPanel_sleep  },	//sleep
	{ 0, NULL },														//stand by
	{ 0, NULL },														//wake up from stand by
	{ 0, NULL }															//wake up from sleep
};

const TPanelInfo gPanelInfo =
{
	(PTPanelParm)&gPanelParm,
	NULL,
	NULL
};

#elif V5_LCD_TYPE == V5_DEF_LCD_LTS180SH_H1I_X

const TReg gPanel_powoff[] =
{
	{0x10,	0x200,	0},
	{0x12,	0x3F09,	0},
	{0x13,	0x121B,	0}
};

const TReg gPanel_powon[] =
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

const TPanelParm gMainPanelParm =
{
	{ sizeof(gPanel_powon)  / sizeof(TReg), (PTReg)&gPanel_powon  }, 	//power on
	{ sizeof(gPanel_powoff) / sizeof(TReg), (PTReg)&gPanel_powoff },	//power off
	{ 0, NULL },														//sleep
	{ 0, NULL },														//stand by
	{ 0, NULL },														//wake up from stand by
	{ 0, NULL }
};

const TPanelInfo gPanelInfo =
{
	(PTPanelParm)&gMainPanelParm,
	(PTPanelParm)&gMainPanelParm,
	NULL
};

#elif V5_LCD_TYPE == V5_DEF_LCD_TRULY_CSTN_BJ2432E_8

const TReg gMainPanel_powon[] =
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

const TPanelParm gMainPanelParm =
{
	{ sizeof(gMainPanel_powon) / sizeof(TReg), (PTReg)&gMainPanel_powon },		//power on
	{ 0, NULL },																//power off
	{ 0, NULL },																//sleep
	{ 0, NULL },																//stand by
	{ 0, NULL },																//wake up from stand by
	{ 0, NULL }
};

const TReg gSubPanel_powon[] =
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

const TPanelParm gSubPanelParm =
{
	{ sizeof(gSubPanel_powon) / sizeof(TReg), (PTReg)&gSubPanel_powon },		//power on
	{ 0, NULL },																//power off
	{ 0, NULL },																//sleep
	{ 0, NULL },																//stand by
	{ 0, NULL },																//wake up from stand by
	{ 0, NULL }
};

void V5_HPanelLightOn(void)
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

const TPanelInfo gPanelInfo =
{
	(PTPanelParm)&gMainPanelParm,
	(PTPanelParm)&gSubPanelParm,
	HPanelLightOn
};

#elif V5_LCD_TYPE == V5_DEF_LCD_MGG1256DTSW

const TReg gMainPanel_powon[] =
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

const TPanelParm gMainPanelParm =
{
	{ sizeof(gMainPanel_powon) / sizeof(TReg), (PTReg)&gMainPanel_powon },	//power on
	{ 0, NULL },															//power off
	{ 0, NULL },															//sleep
	{ 0, NULL },															//stand by
	{ 0, NULL },															//wake up from stand by
	{ 0, NULL }
};

const TReg gSubPanel_powon[] =
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

const TPanelParm gSubPanelParm =
{
	{ sizeof(gSubPanel_powon) / sizeof(TReg), (PTReg)&gSubPanel_powon },	//power on
	{ 0, NULL },															//power off
	{ 0, NULL },															//sleep
	{ 0, NULL },															//stand by
	{ 0, NULL },															//wake up from stand by
	{ 0, NULL }
};

const TPanelInfo gPanelInfo =
{
	(PTPanelParm)&gMainPanelParm,
	(PTPanelParm)&gSubPanelParm,
	NULL
};

#elif V5_LCD_TYPE == V5_DEF_LCD_PLT2001

const TReg gMainPanel_powon[] =
{
	{0x01, 0x02, 100},
	{0x02, 0x12, 100},
	{0x03, 0x60, 100},
	{0x05, 0x08, 100},
	{0x06, 0xc5, 100},
	{0x18, 0x00, 100},
	{0x19, 0x00, 100},
	{0x17, 0x00, 200}
};

const TPanelParm gMainPanelParm =
{
	{ sizeof(gMainPanel_powon) / sizeof(TReg), (PTReg)&gMainPanel_powon },	//power on
	{ 0, NULL },															//power off
	{ 0, NULL },															//sleep
	{ 0, NULL },															//stand by
	{ 0, NULL },															//wake up from stand by
	{ 0, NULL }
};

const TReg gSubPanel_powon[] =
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

const TPanelParm gSubPanelParm =
{
	{ sizeof(gSubPanel_powon) / sizeof(TReg), (PTReg)&gSubPanel_powon },	//power on
	{ 0, NULL },															//power off
	{ 0, NULL },															//sleep
	{ 0, NULL },															//stand by
	{ 0, NULL },															//wake up from stand by
	{ 0, NULL }
};

const TPanelInfo gPanelInfo =
{
	(PTPanelParm)&gMainPanelParm,
	(PTPanelParm)&gSubPanelParm,
	NULL
};

#endif


void V5_HPanelWriteData(UINT8 type, UINT16 val)
{
	type = type & (MULTI_16 | REG_16 | HIGHBYTE | RS_HIGH);
	switch(type)
	{
	case 0x00:		//interface=8,command=8,byte=low,RS=0
		V5B_LcdifRsLExW(val & 0xff);
		break;
	case 0x01:		//interface=8,command=8,byte=low,RS=1
		V5B_LcdifRsHExW(val & 0xff);
		break;
	case 0x02:		//interface=8,command=8,byte=high,RS=0
		V5B_LcdifRsLExW((val << 8) & 0xff00);
		break;
	case 0x03:		//interface=8,command=8,byte=high,RS=1
		V5B_LcdifRsHExW((val << 8) & 0xff00);
		break;
		
	case 0x04:		//interface=8,command=16,byte=low,RS=0
		V5B_LcdifRsLExW((val >> 8) & 0xff);
		V5B_LcdifRsLExW(val & 0xff);
		break;
	case 0x05:		//interface=8,command=16,byte=low,RS=1
		V5B_LcdifRsHExW((val >> 8) & 0xff);
		V5B_LcdifRsHExW(val & 0xff);
		break;
	case 0x06:		//interface=8,command=16,byte=high,RS=0
		V5B_LcdifRsLExW(val & 0xff00);
		V5B_LcdifRsLExW((val << 8) & 0xff00);
		break;
	case 0x07:		//interface=8,command=16,byte=high,RS=1
		V5B_LcdifRsHExW(val & 0xff00);
		V5B_LcdifRsHExW((val << 8) & 0xff00);
		break;
		
	case 0x08:		//interface=16,command=8, byte=low, RS=0
	case 0x0c:		//interface=16,command=16,byte=low, RS=0
		V5B_LcdifRsLExW(val);
		break;
		
	case 0x09:		//interface=16,command=8, byte=low, RS=1
	case 0x0d:		//interface=16,command=16,byte=low, RS=1
		V5B_LcdifRsHExW(val);
		break;

	default :
		break;
	};
}

void V5_HPanelSetLcdReg(UINT8 type, UINT32 addr, UINT16 val)
{
	if(val == 0xffff)
		V5_HPanelWriteData(type & 0xfe, (UINT16)addr);
	else if(addr == 0xffff)
		V5_HPanelWriteData(type, val);
	else
	{
		V5_HPanelWriteData(type & 0xfe, (UINT16)addr);
		V5_HPanelWriteData(type, val);
	}
}

void V5_HPanelSetPanelRegList(PTRegGroup parm)
{
	UINT8 i, type;

	type = V5_DispGetPanelType();
	for(i = 0; i < parm->len; i++)
	{
		V5_HPanelSetLcdReg(type, parm->regval[i].adr, parm->regval[i].val);
#ifndef NCSC
		V5_Delay(parm->regval[i].wait);
#endif
	}
}

void V5_HPanelSetPanelState(UINT8 state, UINT8 flag)
{
	PTPanelParm	pcont = NULL;

	if(flag)
		if(gPanelInfo.pBacklightOnEx)
			gPanelInfo.pBacklightOnEx();

	pcont = gPanelInfo.master;
	if(PANEL_SLAVE == V5_DispGetPanelSel())
		pcont = gPanelInfo.slave;
	if(pcont == NULL)
		return;

	switch(state)
	{
	case PANEL_POWERON:
		V5_HPanelSetPanelRegList(&(pcont->poweron));
		break;
	case PANEL_STANDBY:
		V5_HPanelSetPanelRegList(&(pcont->standby));
		break;
	case PANEL_POWEROFF:
		V5_HPanelSetPanelRegList(&(pcont->poweroff));
		break;
	case PANEL_SLEEP:
		V5_HPanelSetPanelRegList(&(pcont->sleep));
		break;
	case PANEL_WAKEUPFROMSTANDBY:
		V5_HPanelSetPanelRegList(&(pcont->wakeupstb));
		break;
	case PANEL_WAKEUPFSLEEP:
		V5_HPanelSetPanelRegList(&(pcont->wakeup));
		break;
	default:
		return;
	}
}

#endif //V578CMD_DISABLE
