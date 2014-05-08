/*************************************************************************/
/*									 */
/* The last modification date:  02-23-2004                               */
/* REMARKS:  Created initial version 1.0                                 */
/*                                                                       */
/*                Copyright (C) 2004 Vimicro CO.,LTD     		 */
/*************************************************************************/
#include "v558def.h"
#include "v558api.h"
#include "vregdef.h"

typedef struct tag_V558_PANELWRITEINFO {
	UINT8     DataPosition;
	UINT8     RegType;
	UINT8     BusType;
} V558_PANELWRITEINFO,*pV558_PANELWRITEINFO;
const V558_PANELWRITEINFO Panel_WriteType;
const V558_PANELWRITEINFO SubPanel_WriteType;
enum {
	NoReg,		//no sub
	RegRsLow16,
	RegRsHigh16,
	RegRsLow8,
	RegRsHigh8
};
enum {			//lcd 8 bit only
	High8,
	Low8
};
enum {			//lcd 8 bit only
	Lcd_Multi16,
	Lcd_Multi8
};
/********************************************************************************

	Description:
		write value to panel's index register(e.t samsung's panel)

	Parameters:
		val:	the index value

	Remarks:
		state: invalid

*********************************************************************************/
void V558_LcdPanelWrIndex(UINT16 val,pV558_PANELWRITEINFO WriteType)
{
if (WriteType->BusType==Lcd_Multi16)
{
		PanelWrRsClr(val);
}
else
{
		if(WriteType->DataPosition==High8)		
			{
					switch(WriteType->RegType)
					{
						case RegRsHigh16:
						case RegRsLow16:
							V558_SetReg(V558_REG_LCD_EXW0+1	,  (UINT8)(val >> 8) );
							V558_SetReg(V558_REG_LCD_EXWEN, 0x1);
							V558_SetReg(V558_REG_LCD_EXW0	+1,  (UINT8)val );
							V558_SetReg(V558_REG_LCD_EXWEN, 0x1);
							break;
						case RegRsHigh8:
						case RegRsLow8:
							V558_SetReg(V558_REG_LCD_EXW0+1	,  (UINT8)val);
							V558_SetReg(V558_REG_LCD_EXWEN, 0x1);
							break;
						default:
							break;
					}
			}
		else
			{
					switch(WriteType->RegType)
					{
						case RegRsHigh16:
						case RegRsLow16:
							V558_SetReg(V558_REG_LCD_EXW0	,  (UINT8)(val >> 8) );
							V558_SetReg(V558_REG_LCD_EXWEN, 0x1);
							V558_SetReg(V558_REG_LCD_EXW0	,  (UINT8)val );
							V558_SetReg(V558_REG_LCD_EXWEN, 0x1);
							break;
						case RegRsHigh8:
						case RegRsLow8:
							V558_SetReg(V558_REG_LCD_EXW0	,  (UINT8)val  );
							V558_SetReg(V558_REG_LCD_EXWEN, 0x1);
							break;
						default:
							break;
					}

			}
}

}

/********************************************************************************

	Description:
		get panel's status

	Returns:
		return getting value

	Note:
		\1. This function must be in though mode.

		\2. The read process is different according to lcd panel

	Remarks:
		state: invalid

*********************************************************************************/
UINT16 V558_LcdPanelGetStatus(pV558_PANELWRITEINFO WriteType)
{
	UINT16	ret = 0;
	UINT8	x1=0, x2=0;
	
if (WriteType->BusType==Lcd_Multi16)
{
		ret=PanelRdRsClr();
}
else
{
	switch(WriteType->RegType)
		     	{
		     	case RegRsHigh16:
		     	case RegRsLow16:
				V558_SetReg(V558_REG_LCD_EXRS, 0x2);
				V558_GetReg(V558_REG_LCD_EXR0   ,  &x1);	//set display gram address
				V558_SetReg(V558_REG_LCD_EXRS, 0x2);
				V558_GetReg(V558_REG_LCD_EXR0	,  &x2);
				ret = ((UINT16)x2 << 8) | (UINT16)x1;
				break;
			case RegRsHigh8:
		     	case RegRsLow8:
		     		if(WriteType->DataPosition==High8)
		     			{
		     				V558_SetReg(V558_REG_LCD_EXRS, 0x2);
						V558_GetReg(V558_REG_LCD_EXR0+1   ,  &x1);	//set display gram address
						ret = (UINT16)x1;
		     			}
		     		else
		     			{
						V558_SetReg(V558_REG_LCD_EXRS, 0x2);
						V558_GetReg(V558_REG_LCD_EXR0   ,  &x1);	//set display gram address
						ret =(UINT16)x1;
		     			}
				break;
			default:
				break;
		     	}
	}
	return ret;
}
/********************************************************************************

	Description:
		get panel's reg

	Returns:
		return getting value

	Note:
		\1. This function must be in though mode.

		\2. The read process is different according to lcd panel

	Remarks:
		state: invalid

*********************************************************************************/
UINT16 V558_LcdPanelGetReg(UINT16 adr,pV558_PANELWRITEINFO WriteType)
{
	UINT16	ret = 0;
	UINT8	x1=0, x2=0;
	V558_LcdPanelWrIndex(adr,WriteType);
if (WriteType->BusType==Lcd_Multi16)
		ret=PanelRdRsSet();
else
	{	
		switch(WriteType->RegType)
		     	{
		     	case RegRsHigh16:
		     	case RegRsLow16:
				V558_SetReg(V558_REG_LCD_EXRS, 0x1);
				V558_GetReg(V558_REG_LCD_EXR0   ,  &x1);	//set display gram address
				V558_SetReg(V558_REG_LCD_EXRS, 0x1);
				V558_GetReg(V558_REG_LCD_EXR0	,  &x2);
				ret = ((UINT16)x2 << 8) | (UINT16)x1;
				break;
			case RegRsHigh8:
		     	case RegRsLow8:
		     		if(WriteType->DataPosition==High8)
		     			{
		     				V558_SetReg(V558_REG_LCD_EXRS, 0x1);
						V558_GetReg(V558_REG_LCD_EXR0+1   ,  &x1);	//set display gram address
						ret =(UINT16)x1;
		     			}
		     		else
		     			{
						V558_SetReg(V558_REG_LCD_EXRS, 0x1);
						V558_GetReg(V558_REG_LCD_EXR0   ,  &x1);	//set display gram address
						ret =  (UINT16)x1;
		     			}
				break;
			default:
				break;
		     	}
		}	
	return ret;
}

/********************************************************************************

	Description:
		writing control register

	Parameters:
		val:	the index value

	Remarks:
		state: invalid

*********************************************************************************/
void V558_LcdPanelWrReg(UINT16 val,pV558_PANELWRITEINFO WriteType)
{
	switch(WriteType->RegType)
		{
			case RegRsHigh16:
				if(WriteType->BusType==Lcd_Multi16)		
					PanelWrRsSet(val);
				else
					if(WriteType->DataPosition==High8)
					{
						V558_SetReg(V558_REG_LCD_EXW1+1	,  (UINT8)(val >> 8) );
						V558_SetReg(V558_REG_LCD_EXWEN, 0x2);
						V558_SetReg(V558_REG_LCD_EXW1+1,  (UINT8)val );
						V558_SetReg(V558_REG_LCD_EXWEN, 0x2);
					}
					else
					{
						V558_SetReg(V558_REG_LCD_EXW1	,  (UINT8)(val >> 8) );
						V558_SetReg(V558_REG_LCD_EXWEN, 0x2);
						V558_SetReg(V558_REG_LCD_EXW1,  (UINT8)val );
						V558_SetReg(V558_REG_LCD_EXWEN, 0x2);
					}
				break;
			case RegRsHigh8:
					if(WriteType->DataPosition==High8)
					{
						V558_SetReg(V558_REG_LCD_EXW1+1,  (UINT8)val );
						V558_SetReg(V558_REG_LCD_EXWEN, 0x2);
					}
					else
					{
						V558_SetReg(V558_REG_LCD_EXW1,  (UINT8)val );
						V558_SetReg(V558_REG_LCD_EXWEN, 0x2);
					}
				break;
			case RegRsLow16:
				if(WriteType->BusType==Lcd_Multi16)
				{
						PanelWrRsClr(val);
				}
				else
				{
                      		if(WriteType->DataPosition==High8)
                      		{
					V558_SetReg(V558_REG_LCD_EXW0+1	,  (UINT8)(val >> 8) );
					V558_SetReg(V558_REG_LCD_EXWEN, 0x1);
					V558_SetReg(V558_REG_LCD_EXW0	+1,  (UINT8)val );
					V558_SetReg(V558_REG_LCD_EXWEN, 0x1);
                      		}
                      		else
                      	  	{
						V558_SetReg(V558_REG_LCD_EXW0	,  (UINT8)(val >> 8) );
						V558_SetReg(V558_REG_LCD_EXWEN, 0x1);
						V558_SetReg(V558_REG_LCD_EXW0	,  (UINT8)val );
						V558_SetReg(V558_REG_LCD_EXWEN, 0x1);
                      		}
				}
				break;
			case RegRsLow8:
                      	if(WriteType->DataPosition==High8)
                      	{
					V558_SetReg(V558_REG_LCD_EXW0	+1,  (UINT8)val );
					V558_SetReg(V558_REG_LCD_EXWEN, 0x1);
                      	}
                      	else
                      	  {
					V558_SetReg(V558_REG_LCD_EXW0	,  (UINT8)val );
					V558_SetReg(V558_REG_LCD_EXWEN, 0x1);
                      	}
			default:
				break;
		}
}



void  V558_LcdPanelSetReg(UINT32 adr, UINT16 val)
{
	 if(adr==0xffff)
		{
			V558_LcdPanelWrReg(val,(pV558_PANELWRITEINFO)&Panel_WriteType);
			return;
		}
	else if(val==0xffff)
		{
			V558_LcdPanelWrIndex(adr,(pV558_PANELWRITEINFO)&Panel_WriteType);	
		return;
		}
	V558_LcdPanelWrIndex(adr,(pV558_PANELWRITEINFO)&Panel_WriteType);
	V558_LcdPanelWrReg(val,(pV558_PANELWRITEINFO)&Panel_WriteType);
}
void  V558_LcdSubPanelSetReg(UINT32 adr, UINT16 val)
{

	 if(adr==0xffff)
		{
			V558_LcdPanelWrReg(val,(pV558_PANELWRITEINFO)&SubPanel_WriteType);
			return;
		}
	else if(val==0xffff)
		{
			V558_LcdPanelWrIndex(adr,(pV558_PANELWRITEINFO)&SubPanel_WriteType);	
		return;
		}
	V558_LcdPanelWrIndex(adr,(pV558_PANELWRITEINFO)&SubPanel_WriteType);
	V558_LcdPanelWrReg(val,(pV558_PANELWRITEINFO)&SubPanel_WriteType);
}

#if	LTS180SH_H1I_X


const V558_PANELWRITEINFO Panel_WriteType=
{
	0,
	(UINT8)RegRsHigh16,
	(UINT8)Lcd_Multi16
};

const V558_PANELREGVALUE Panel_powoff[] =
{
	{0x10,	0x200,	0},
	{0x12,	0x3F09,	0},
	{0x13,	0x121B,	0}
};

const V558_PANELREGVALUE Panel_powon[] =
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

	{0x50,	0x0,	1},
	{0x51,	0x83,	1},
	{0x52,	0x0,	1},
//	{0x53,	0x9F,	1},
	{0x53,	0xff,	1},

	{0x60,	0x9300,	1},
	{0x61,	0x1,	1},
	{0x68,	0x0,	1},
	{0x69,	0x9f,	1},
	{0x6A,	0x0,	1},
	{0x70,	0x8B14,	1},
	{0x71,	0x1,	1},
	{0x78,	0xA0,	1},
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
//	{0x37,	0x1,	1},
//	{0x38,	0x1800,	1},
//	{0x39,	0x7,	1},
	{0x40,	0x700,	1},
	{0x41,	0x7,	1},
	{0x42,	0x0,	1},
	{0x43,	0x100,	1},
	{0x44,	0x707,	1},
	{0x45,	0x7,	1},
	{0x45,	0x7,	1},
	{0x46,	0x700,	1},

	{0x47,	0x7,	1},
	{0x48,	0x1000,	1},
	{0x49,	0x001f,	1},
//	{0x47,	0x1,	1},
//	{0x48,	0x1800,	1},
//	{0x49,	0x7,	1},

	{0x7,	0x301,	10},
	{0x7,	0x321,	1},
	{0x7,	0x323,	20},
	{0x7,	0x333,	40},

	{0x20,	0x83,	1},
	{0x21,	0x0,	1}
};

V558_LCDIFPARM Panel_LcdIF =
{
	{ 128,	160	},		//size
	{4, 0},				//mp
	{32, 160},			//sp
	0x0, 0, 0X20, 0x21, 0X0, 0,	//hsa, hea, hsxa, hsya, hexa, heya
	0xfC85,				//lc
	0x100,				//dpad
	0x460, 0x460,		//wrcycle, rdcycle
	0,					//rsPolarity
	0x0,				//dmap
	1,					//index select
	0x22,				//dispadr
	LTS180SH_H1I_X_PANEL,		//paneltype
	0,					//reserve
	{
		0,		0,		1,		4,		9,		0x10,	0x1a,	0x27,	0x37,
		0x4a,	0x5f,	0x77,	0x91,	0xac,	0xc8,	0xe4,	0xff
	},					//gamma R;
	{
		0,		0,		1,		4,		9,		0x10,	0x1a,	0x27,	0x37,
		0x4a,	0x5f,	0x77,	0x91,	0xac,	0xc8,	0xe4,	0xff
	},					//gamma G;
	{
		0,		0,		1,		4,		9,		0x10,	0x1a,	0x27,	0x37,
		0x4a,	0x5f,	0x77,	0x91,	0xac,	0xc8,	0xe4,	0xff
	},					//gamma B
	{0xff, 0x0, 0x0, 0}
};

const V558_PANELINFO	Panel_Info =
{
	"Samsung LTS180S3-H1I-X",		//description
	&Panel_LcdIF,
	{sizeof(Panel_powon) / sizeof(V558_PANELREGVALUE), Panel_powon},
	{sizeof(Panel_powoff) / sizeof(V558_PANELREGVALUE), Panel_powoff},
	{0, NULL},
	{0, NULL},
	{0, NULL},
	{0, NULL},

	V558_LcdPanelSetReg,
	NULL
};
#endif



#if	LTS180S3_HF1

const V558_PANELWRITEINFO Panel_WriteType=
{
	0,
	(UINT8)RegRsHigh16,
	(UINT8)Lcd_Multi16
};

const V558_PANELREGVALUE Panel_wakefstd[] =
{
	{0x0,	0x01,	10},
	{0x3,	0x0,	10}
};

const V558_PANELREGVALUE Panel_stdby[] =
{
	{0x7,	0x137,	0},
	{0x7,	0x136,	40},
	{0x7,	0x126,	40},
	{0x7,	0x4,	0},
	{0x3,	0x7fd,	0}
};

//static V558_PANELREGVALUE *Panel_wakefs = NULL;
//{
//};

const V558_PANELREGVALUE Panel_sleep[] =
{
	{0x7,	0x137,	0},
	{0x7,	0x136,	20},
	{0x7,	0x126,	20},
	{0x7,	0x4,	0},
	{0x3,	0x7fe,	0}
};

const V558_PANELREGVALUE Panel_powoff[] =
{
	{0x7,	0x136,	20},
	{0x7,	0x126,	20},
	{0x7,	0x4,	0},
	{0x3,	0x7fc,	0}
};

const V558_PANELREGVALUE Panel_powon[] =
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
	{0x3,	0x8,	0},//
	{0x4,	0x0,	20},
	{0xe,	0x2d18,	0},//
	{0xd,	0x110,	20},//
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

	{0x33,	0x700,	0},
	{0x3a,	0x1f00,	0},
	{0x30,	0x700,	0},
	{0x31,	0x707,	0},
	{0x32,	0x700,	0},

	{0x21,	0x0,	0}
};

V558_LCDIFPARM Panel_LcdIF =
{
	{ 128,	160	},		//size
	{0, 0},				//mp
	{0, 0},				//sp
	0x21, 0, 0, 0, 0, 0,	//hsa, hea, hsxa, hsya, hexa, heya
	0xffcc,				//lc
	0x100,				//dpad
	0x460, 0x460,		//wrcycle, rdcycle
	0,					//rsPolarity
	0x0,				//dmap
	1,					//index select
	0x22,				//dispadr
	LTS180S3_HF1_PANEL,		//paneltype
	0,					//reserve
	{
		0,		0,		1,		4,		9,		0x10,	0x1a,	0x27,	0x37,
		0x4a,	0x5f,	0x77,	0x91,	0xac,	0xc8,	0xe4,	0xff
	},					//gamma R;
	{
		0,		0,		1,		4,		9,		0x10,	0x1a,	0x27,	0x37,
		0x4a,	0x5f,	0x77,	0x91,	0xac,	0xc8,	0xe4,	0xff
	},					//gamma G;
	{
		0,		0,		1,		4,		9,		0x10,	0x1a,	0x27,	0x37,
		0x4a,	0x5f,	0x77,	0x91,	0xac,	0xc8,	0xe4,	0xff
	},					//gamma B
	{0xff, 0x0, 0x0, 0}
};

const V558_PANELINFO	Panel_Info =
{
	"Samsung LTS180S3-HF1",		//description

	&Panel_LcdIF,

	{sizeof(Panel_powon) / sizeof(V558_PANELREGVALUE), Panel_powon},
	{sizeof(Panel_powoff) / sizeof(V558_PANELREGVALUE), Panel_powoff},
	{sizeof(Panel_sleep) / sizeof(V558_PANELREGVALUE), Panel_sleep},
	{0, NULL },

	{sizeof(Panel_stdby) / sizeof(V558_PANELREGVALUE), Panel_stdby},
	{sizeof(Panel_wakefstd) / sizeof(V558_PANELREGVALUE), Panel_wakefstd},
	V558_LcdPanelSetReg,
	NULL
};
#endif



#if	Truly_128160_16
const V558_PANELWRITEINFO Panel_WriteType=
{
	0,
	(UINT8)RegRsHigh16,
	(UINT8)Lcd_Multi16
};

void TrulyInit(void)
{
	UINT8 temp, State = 0;
	UINT16 i;

	V558_CtrlSetGpioCfg(0xff);
	V558_CtrlSetGpioMode(0xff);
	V558_CtrlSetGpioDirection(0x01);

	for(temp = 0; temp < 58; temp++)
	{ 
		if(State)
		{
			V558_CtrlSetGpioData(0x00);
			State = 0;
		}
		else
		{
			State = 1;
			V558_CtrlSetGpioData(0x01);
			for(i = 0; i < 50; i++);
		}
	}
	V558_CtrlSetGpioData(0x01);
}

const V558_PANELREGVALUE Panel_wakefstd[] =
{
	{0x0,	0x01,	10},
	{0x3,	0x0,	10}
};

const V558_PANELREGVALUE Panel_stdby[] =
{
	{0x7,	0x137,	0},
	{0x7,	0x136,	40},
	{0x7,	0x126,	40},
	{0x7,	0x4,	0},
	{0x3,	0x7fd,	0}
};

//static V558_PANELREGVALUE *Panel_wakefs = NULL;
//{
//};

const V558_PANELREGVALUE Panel_sleep[] =
{
	{0x7,	0x137,	0},
	{0x7,	0x136,	20},
	{0x7,	0x126,	20},
	{0x7,	0x4,	0},
	{0x3,	0x7fe,	0}
};

const V558_PANELREGVALUE Panel_powoff[] =
{
	{0x7,	0x136,	20},
	{0x7,	0x126,	20},
	{0x7,	0x4,	0},
	{0x3,	0x7fc,	0}
};

const V558_PANELREGVALUE Panel_powon[] =
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
	{0x3,	0x8,	0},//
	{0x4,	0x0,	20},
	{0xe,	0x2d18,	0},//
	{0xd,	0x110,	20},//
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

	{0x33,	0x707,	0},
	{0x3a,	0x1f00,	0},
	{0x30,	0x700,	0},
	{0x31,	0x707,	0},
	{0x32,	0x700,	0},

	{0x21,	0x0,	0}
};

V558_LCDIFPARM Panel_LcdIF =
{
	{ 128,	160	},		//size
	{0, 0},				//mp
	{0, 0},				//sp
	0x21, 0, 0, 0, 0, 0,	//hsa, hea, hsxa, hsya, hexa, heya
	0xffcc,				//lc
	0x100,				//dpad
	0x460, 0x460,		//wrcycle, rdcycle
	0,					//rsPolarity
	0x0,				//dmap
	1,					//index select
	0x22,				//dispadr
	Truly_128160_16_PANEL,		//paneltype
	0,					//reserve
	{
		0,		0,		1,		4,		9,		0x10,	0x1a,	0x27,	0x37,
		0x4a,	0x5f,	0x77,	0x91,	0xac,	0xc8,	0xe4,	0xff
	},					//gamma R;
	{
		0,		0,		1,		4,		9,		0x10,	0x1a,	0x27,	0x37,
		0x4a,	0x5f,	0x77,	0x91,	0xac,	0xc8,	0xe4,	0xff
	},					//gamma G;
	{
		0,		0,		1,		4,		9,		0x10,	0x1a,	0x27,	0x37,
		0x4a,	0x5f,	0x77,	0x91,	0xac,	0xc8,	0xe4,	0xff
	},					//gamma B
	{0xff, 0x0, 0x0, 0}
};

const V558_PANELINFO	Panel_Info =
{
	"Truly_128160_16",		//description

	&Panel_LcdIF,

	{sizeof(Panel_powon) / sizeof(V558_PANELREGVALUE), Panel_powon},
	{sizeof(Panel_powoff) / sizeof(V558_PANELREGVALUE), Panel_powoff},
	{sizeof(Panel_sleep) / sizeof(V558_PANELREGVALUE), Panel_sleep},
	{0, NULL },
//	{sizeof(Panel_wakefs) / sizeof(V558_PANELREGVALUE), Panel_wakefs},
	{sizeof(Panel_stdby) / sizeof(V558_PANELREGVALUE), Panel_stdby},
	{sizeof(Panel_wakefstd) / sizeof(V558_PANELREGVALUE), Panel_wakefstd},
	V558_LcdPanelSetReg,
	TrulyInit
//	NULL
};
#endif



#if	BYD_128160
const V558_PANELWRITEINFO Panel_WriteType=
{
	0,
	(UINT8)RegRsHigh16,
	(UINT8)Lcd_Multi16
};
const V558_PANELREGVALUE Panel_wakefstd[] =
{
	{0x0,	0x01,	10},
	{0x3,	0x0,	10}
};

const V558_PANELREGVALUE Panel_stdby[] =
{
	{0x7,	0x137,	0},
	{0x7,	0x136,	40},
	{0x7,	0x126,	40},
	{0x7,	0x4,	0},
	{0x3,	0x7fd,	0}
};

//static V558_PANELREGVALUE *Panel_wakefs = NULL;
//{
//};

const V558_PANELREGVALUE Panel_sleep[] =
{
	{0x7,	0x137,	0},
	{0x7,	0x136,	20},
	{0x7,	0x126,	20},
	{0x7,	0x4,	0},
	{0x3,	0x7fe,	0}
};

const V558_PANELREGVALUE Panel_powoff[] =
{
	{0x7,	0x136,	20},
	{0x7,	0x126,	20},
	{0x7,	0x4,	0},
	{0x3,	0x7fc,	0}
};

const V558_PANELREGVALUE Panel_powon[] =
{
	{0x7,	0x104,    0},//0x104,	0},
	{0xd,	0x201,	0},
	{0xe,	0xd18,	20},
	{0x1,	0x113,	0},
	{0x2,	0x700,	0},
	{0x5,	0x1230,	0},
	{0x6,	0x0,	0},
	{0xb,	0x4008,	0},
	{0xc,	0x0,	0},
	{0x3,	0x508,	0},
	{0x4,	0x0,	20},
	{0xe,	0x2d18,	0},
	{0xd,	0x2111,	20},
	{0x21,	0x0,	0},
	{0x30,	0x700,	0},
	{0x31,	0x7,	0},
	{0x32,	0x0,	0},
	{0x33,	0x100,	0},
	{0x34,	0x700,	0},
	{0x35,	0x7,	0},
	{0x36,	0x700,	0},
	{0x37,	0x01,	0},
	{0x0f,	0x0,	0},
	{0x11,	0x0,	0},
	{0x14,	0x5c00,	0},
	{0x15,	0x9f00,	0},
	{0x16,	0x7f00,	0},
	{0x17,	0x9f00,	0},
	{0x3a,	0x1800,	0},
	{0x3b,	0x7,	5},
	{0x7,	0x105,	0},
	{0x7,	0x125,	0},
	{0x7,	0x127,	0},
	{0x7,	0x137,	0},

/*	{0x33,	0x700,	0},
	{0x3a,	0x1f00,	0},
	{0x30,	0x700,	0},
	{0x31,	0x707,	0},
	{0x32,	0x700,	0},*/

	{0x21,	0x0,	0}
};

V558_LCDIFPARM Panel_LcdIF =
{
	{ 128,	160	},		//size
	{0, 0},				//mp
	{0, 0},				//sp
	0x21, 0, 0, 0, 0, 0,	//hsa, hea, hsxa, hsya, hexa, heya
	0xffcc,				//lc
	0x100,				//dpad
	0x460, 0x460,		//wrcycle, rdcycle
	0,					//rsPolarity
	0x0,				//dmap
	1,					//index select
	0x22,				//dispadr
	BYD_128160_PANEL,		//paneltype
	0,					//reserve
	{
		0,		0,		1,		4,		9,		0x10,	0x1a,	0x27,	0x37,
		0x4a,	0x5f,	0x77,	0x91,	0xac,	0xc8,	0xe4,	0xff
	},					//gamma R;
	{
		0,		0,		1,		4,		9,		0x10,	0x1a,	0x27,	0x37,
		0x4a,	0x5f,	0x77,	0x91,	0xac,	0xc8,	0xe4,	0xff
	},					//gamma G;
	{
		0,		0,		1,		4,		9,		0x10,	0x1a,	0x27,	0x37,
		0x4a,	0x5f,	0x77,	0x91,	0xac,	0xc8,	0xe4,	0xff
	},					//gamma B
	{0xff, 0x0, 0x0, 0}
};

const V558_PANELINFO	Panel_Info =
{
	" BYD_128160",		//description

	&Panel_LcdIF,

	{sizeof(Panel_powon) / sizeof(V558_PANELREGVALUE), Panel_powon},
	{sizeof(Panel_powoff) / sizeof(V558_PANELREGVALUE), Panel_powoff},
	{sizeof(Panel_sleep) / sizeof(V558_PANELREGVALUE), Panel_sleep},
	{0, NULL },
//	{sizeof(Panel_wakefs) / sizeof(V558_PANELREGVALUE), Panel_wakefs},
	{sizeof(Panel_stdby) / sizeof(V558_PANELREGVALUE), Panel_stdby},
	{sizeof(Panel_wakefstd) / sizeof(V558_PANELREGVALUE), Panel_wakefstd},
	V558_LcdPanelSetReg,
	NULL
};
#endif

#if	Jemiteck_J180S
const V558_PANELWRITEINFO Panel_WriteType=
{
	0,
	(UINT8)RegRsHigh16,
	(UINT8)Lcd_Multi16
};
const V558_PANELREGVALUE Panel_wakefstd[] =
{
	{0x0,	0x01,	10},
	{0x3,	0x0,	10}
};

const V558_PANELREGVALUE Panel_stdby[] =
{
	{0x7,	0x137,	0},
	{0x7,	0x136,	40},
	{0x7,	0x126,	40},
	{0x7,	0x4,	0},
	{0x3,	0x7fd,	0}
};

//static V558_PANELREGVALUE *Panel_wakefs = NULL;
//{
//};

const V558_PANELREGVALUE Panel_sleep[] =
{
	{0x7,	0x137,	0},
	{0x7,	0x136,	20},
	{0x7,	0x126,	20},
	{0x7,	0x4,	0},
	{0x3,	0x7fe,	0}
};

const V558_PANELREGVALUE Panel_powoff[] =
{
	{0x7,	0x136,	20},
	{0x7,	0x126,	20},
	{0x7,	0x4,	0},
	{0x3,	0x7fc,	0}
};

const V558_PANELREGVALUE Panel_powon[] =
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
	{0x3,	0x8,	0},//
	{0x4,	0x0,	20},
	{0xe,	0x2d18,	0},//
	{0xd,	0x110,	20},//
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

	{0x33,	0x700,	0},
	{0x3a,	0x1f00,	0},
	{0x30,	0x700,	0},
	{0x31,	0x707,	0},
	{0x32,	0x700,	0},

	{0x21,	0x0,	0}
};

V558_LCDIFPARM Panel_LcdIF =
{
	{ 128,	160	},		//size
	{0, 0},				//mp
	{0, 0},				//sp
	0x21, 0, 0, 0, 0, 0,	//hsa, hea, hsxa, hsya, hexa, heya
	0xffcc,				//lc
	0x100,				//dpad
	0x460, 0x460,		//wrcycle, rdcycle
	0,					//rsPolarity
	0x0,				//dmap
	1,					//index select
	0x22,				//dispadr
	Jemiteck_J180S_Panel,		//paneltype
	0,					//reserve
	{
		0,		0,		1,		4,		9,		0x10,	0x1a,	0x27,	0x37,
		0x4a,	0x5f,	0x77,	0x91,	0xac,	0xc8,	0xe4,	0xff
	},					//gamma R;
	{
		0,		0,		1,		4,		9,		0x10,	0x1a,	0x27,	0x37,
		0x4a,	0x5f,	0x77,	0x91,	0xac,	0xc8,	0xe4,	0xff
	},					//gamma G;
	{
		0,		0,		1,		4,		9,		0x10,	0x1a,	0x27,	0x37,
		0x4a,	0x5f,	0x77,	0x91,	0xac,	0xc8,	0xe4,	0xff
	},					//gamma B
	{0xff, 0x0, 0x0, 0}
};

const V558_PANELINFO	Panel_Info =
{
	" Jemiteck_J180S",		//description

	&Panel_LcdIF,
	{sizeof(Panel_powon) / sizeof(V558_PANELREGVALUE), Panel_powon},
	{sizeof(Panel_powoff) / sizeof(V558_PANELREGVALUE), Panel_powoff},
	{sizeof(Panel_sleep) / sizeof(V558_PANELREGVALUE), Panel_sleep},
	{0, NULL },
//	{sizeof(Panel_wakefs) / sizeof(V558_PANELREGVALUE), Panel_wakefs},
	{sizeof(Panel_stdby) / sizeof(V558_PANELREGVALUE), Panel_stdby},
	{sizeof(Panel_wakefstd) / sizeof(V558_PANELREGVALUE), Panel_wakefstd},
	V558_LcdPanelSetReg,
	NULL
};
#endif


#if	UG_12D181_M16
const V558_PANELWRITEINFO Panel_WriteType=
{
	Low8,
	RegRsLow8,
	Lcd_Multi16
};


const V558_PANELREGVALUE Panel_powon[] =
{
	{0x50,	0xffff,	0},			//DD_TURN_DISPLAY_OFF
	{0x2c,	0xffff,	25},			//STANDBY_MODE_OFF
	{0x2,	0x01,	25},		//DD_TURN_OSC_ON
	
	{0x20,	0x0a,	0},		//DD_POWER_ON_POWER_CIRCUIT
	{0x22,	0x11,	0},			

	{0x26,	0x1,	50},		//DCDC_AMP_ONOFF
	{0x26,	0x9,	25},
	{0x26,	0xb,	25},
	{0x26,	0xf,	25},

	//{0x2,	0x1,	10},
	//{0x2e00,	0x0,	10},

	{0x10,	0x02,	0},

	{0x40,	0x01,	0},
	{0x34,    0x98,       0},
	{0x55,    0x0,       0},
	{0x2a,    0xd4,25},

	{0x28,	0x01,	0},
	{0x2e,0xffff,0},
	
	{0x45,	0x00,	10},
	
	{0,0,0},
	{0,0,0},
	
	{0x53,0,0},
	{0x24,0x22,25},
	{0x30,0x0d,0},
	{0x32,0x0c,0},
	
	{0x42,0x0,0},
	{0xffff,0x7f,0},
		
	{0x43,0x0,0},
	{0xffff,0x7f,0},
	
	{0x2b,0x78,0},
	
	{0x59,0x0,0},
	{0xffff,0x0,0},
	{0xffff,0x7f,0},
	{0xffff,0x0,0},

	{0x5a,0x0,0},
	{0x51,0xffff,0}
};

V558_LCDIFPARM Panel_LcdIF =
{
	{ 128,	128	},		//size
	{0x0, 0x0},				//mp
	{0x0, 0x0},				//sp
	0x43, 0x42, 0x0, 0x0, 0X0, 0x0,	//hsa, hea, hsxa, hsya, hexa, heya
	0x708a,				//lc	//old: 108a
	0x100,				//dpad
	0x0480, 0x7CA0,		//wrcycle, rdcycle
	0,					//rsPolarity
	0x0,				//dmap
	5,					//index select
	0x51,				//dispadr
	//LTS180S3_HF1_PANEL,
	UG_12D181_Panel,		//paneltype
	0,					//reserve
	{
		0,		0,		1,		4,		9,		0x10,	0x1a,	0x27,	0x37,
		0x4a,	0x5f,	0x77,	0x91,	0xac,	0xc8,	0xe4,	0xff
	},					//gamma R;
	{
		0,		0,		1,		4,		9,		0x10,	0x1a,	0x27,	0x37,
		0x4a,	0x5f,	0x77,	0x91,	0xac,	0xc8,	0xe4,	0xff
	},					//gamma G;
	{
		0,		0,		1,		4,		9,		0x10,	0x1a,	0x27,	0x37,
		0x4a,	0x5f,	0x77,	0x91,	0xac,	0xc8,	0xe4,	0xff
	},					//gamma B
	{0xff, 0x0, 0x0, 0}
};

const V558_PANELINFO	Panel_Info =
{
	"Lanser UG_12D181_Panel",		//description

	&Panel_LcdIF,

	{sizeof(Panel_powon) / sizeof(V558_PANELREGVALUE), Panel_powon},
	{0, NULL},
	{0, NULL},
	{0, NULL},
	{0, NULL},
	{0, NULL},
//	{sizeof(Panel_sleep) / sizeof(V558_PANELREGVALUE), Panel_sleep},
//	{sizeof(Panel_wakefs) / sizeof(V558_PANELREGVALUE), Panel_wakefs},
//	{sizeof(Panel_stdby) / sizeof(V558_PANELREGVALUE), Panel_stdby},
//	{sizeof(Panel_wakefstd) / sizeof(V558_PANELREGVALUE), Panel_wakefstd}
	V558_LcdPanelSetReg,
	NULL
};
#endif
#if	UG_12D181_M8
const V558_PANELWRITEINFO Panel_WriteType=
{
	Low8,
	RegRsLow8,
	Lcd_Multi16
};



const V558_PANELREGVALUE Panel_powon[] =
{
	{0x50,	0xffff,	0},			//DD_TURN_DISPLAY_OFF
	{0x2c,	0xffff,	25},			//STANDBY_MODE_OFF
	{0x2,	0x01,	25},		//DD_TURN_OSC_ON
	
	{0x20,	0x0a,	0},		//DD_POWER_ON_POWER_CIRCUIT
	{0x22,	0x11,	0},			

	{0x26,	0x1,	50},		//DCDC_AMP_ONOFF
	{0x26,	0x9,	25},
	{0x26,	0xb,	25},
	{0x26,	0xf,	25},

	//{0x2,	0x1,	10},
	//{0x2e00,	0x0,	10},

	{0x10,	0x02,	0},

	{0x40,	0x01,	0},
	{0x34,    0x98,       0},
	{0x55,    0x0,       0},
	{0x2a,    0xd4,25},

	{0x28,	0x01,	0},
	{0x2e,0xffff,0},
	
	{0x45,	0x00,	10},
	
	{0,0,0},
	{0,0,0},
	
	{0x53,0,0},
	{0x24,0x22,25},
	{0x30,0x0d,0},
	{0x32,0x0c,0},
	
	{0x42,0x0,0},
	{0xffff,0x7f,0},
		
	{0x43,0x0,0},
	{0xffff,0x7f,0},
	
	{0x2b,0x78,0},
	
	{0x59,0x0,0},
	{0xffff,0x0,0},
	{0xffff,0x7f,0},
	{0xffff,0x0,0},

	{0x5a,0x0,0},
	{0x51,0xffff,0}
};

V558_LCDIFPARM Panel_LcdIF =
{
	{ 128,	128	},		//size
	{0x0, 0x0},				//mp
	{0x0, 0x0},				//sp
	0x43, 0x42, 0x0, 0x0, 0X0, 0x0,	//hsa, hea, hsxa, hsya, hexa, heya
	0x708a,				//lc	//old: 108a
	0x100,				//dpad
	0x0460, 0x7CA0,		//wrcycle, rdcycle
	0,					//rsPolarity
	0x4,
	5,					//index select
	0x51,				//dispadr
	//LTS180S3_HF1_PANEL,
	UG_12D181_Panel,		//paneltype
	0,					//reserve
	{
		0,		0,		1,		4,		9,		0x10,	0x1a,	0x27,	0x37,
		0x4a,	0x5f,	0x77,	0x91,	0xac,	0xc8,	0xe4,	0xff
	},					//gamma R;
	{
		0,		0,		1,		4,		9,		0x10,	0x1a,	0x27,	0x37,
		0x4a,	0x5f,	0x77,	0x91,	0xac,	0xc8,	0xe4,	0xff
	},					//gamma G;
	{
		0,		0,		1,		4,		9,		0x10,	0x1a,	0x27,	0x37,
		0x4a,	0x5f,	0x77,	0x91,	0xac,	0xc8,	0xe4,	0xff
	},					//gamma B
	{0xff, 0x0, 0x0, 0}
};

const V558_PANELINFO	Panel_Info =
{
	"Lanser UG_12D181_Panel",		//description

	&Panel_LcdIF,

	{sizeof(Panel_powon) / sizeof(V558_PANELREGVALUE), Panel_powon},
	{0, NULL},
	{0, NULL},
	{0, NULL},
	{0, NULL},
	{0, NULL},
//	{sizeof(Panel_sleep) / sizeof(V558_PANELREGVALUE), Panel_sleep},
//	{sizeof(Panel_wakefs) / sizeof(V558_PANELREGVALUE), Panel_wakefs},
//	{sizeof(Panel_stdby) / sizeof(V558_PANELREGVALUE), Panel_stdby},
//	{sizeof(Panel_wakefstd) / sizeof(V558_PANELREGVALUE), Panel_wakefstd}
	V558_LcdPanelSetReg,
	NULL
};
#endif
#if	UG_120114A
const V558_PANELWRITEINFO Panel_WriteType=
{
	Low8,
	RegRsLow8,
	Lcd_Multi16
};
const V558_PANELREGVALUE Panel_powon[] =
{
	{0,0 ,0},
{0x2c,0,25},
{0x02,0x01,25},
{0x20,0x05,0},
{0x22,0x11,0},
{0x26,0x0f,50},
{0x28,0x01,0},
//{0x10,0x22,0},
{0x10,0x26,0},
{0x40,0x01,0},
{0x34,0x1f,0},
{0x30,0x1f,0},
{0x32,0x0e,0},
{0x24,0x00,0},
//{0x2a,0xa6,0},
{0x2a,0xc5,0},
{0xeb,0,0},
{0x51,0,25},
};

V558_LCDIFPARM Panel_LcdIF =
{
 { 128+4, 160+2 },  //size
 {0x0, 0x0},    //mp
 {0x0, 0x0},    //sp
 0x43, 0x42, 0x0, 0x0, 0X0, 0x0, //hsa, hea, hsxa, hsya, hexa, heya
 0x708a,    //lc //old: 108a
 0x100,    //dpad
 0x0460, 0x7CA0,  //wrcycle, rdcycle
 0,     //rsPolarity
 0x0,    //dmap
 5,     //index select
 0x51,    //dispadr
 UG_120114A_Panel,  //paneltype
 0,     //reserve
 {
  0,  0,  1,  4,  9,  0x10, 0x1a, 0x27, 0x37,
  0x4a, 0x5f, 0x77, 0x91, 0xac, 0xc8, 0xe4, 0xff
 },     //gamma R;
 {
  0,  0,  1,  4,  9,  0x10, 0x1a, 0x27, 0x37,
  0x4a, 0x5f, 0x77, 0x91, 0xac, 0xc8, 0xe4, 0xff
 },     //gamma G;
 {
  0,  0,  1,  4,  9,  0x10, 0x1a, 0x27, 0x37,
  0x4a, 0x5f, 0x77, 0x91, 0xac, 0xc8, 0xe4, 0xff
 },     //gamma B
	{0xff, 0x0, 0x0, 0}		//background color
};

const V558_PANELINFO	Panel_Info =
{
	"Lanser UG_12D181_Panel",		//description

	&Panel_LcdIF,
	{sizeof(Panel_powon) / sizeof(V558_PANELREGVALUE), Panel_powon},
	{0, NULL},
	{0, NULL},
	{0, NULL},
	{0, NULL},
	{0, NULL},
//	{sizeof(Panel_sleep) / sizeof(V558_PANELREGVALUE), Panel_sleep},
//	{sizeof(Panel_wakefs) / sizeof(V558_PANELREGVALUE), Panel_wakefs},
//	{sizeof(Panel_stdby) / sizeof(V558_PANELREGVALUE), Panel_stdby},
//	{sizeof(Panel_wakefstd) / sizeof(V558_PANELREGVALUE), Panel_wakefstd}
	V558_LcdPanelSetReg,
	NULL
};
const V558_PANELWRITEINFO SubPanel_WriteType=
{
	Low8,
	RegRsLow8,
	Lcd_Multi16
};
const V558_PANELREGVALUE SubPanel_powon[] =
{
	{0,0 ,0},
{0x2c,0,25},
{0x02,0x01,25},
{0x20,0x05,0},
{0x22,0x11,0},
{0x26,0x0f,50},
{0x28,0x01,0},
//{0x10,0x22,0},
{0x10,0x26,0},
{0x40,0x01,0},
{0x34,0x1f,0},
{0x30,0x1f,0},
{0x32,0x0e,0},
{0x24,0x00,0},
//{0x2a,0xa6,0},
{0x2a,0xc5,0},
{0xeb,0,0},
{0x51,0,25},
};

V558_LCDIFPARM SubPanel_LcdIF =
{
 { 128+4, 160+2 },  //size
 {0x0, 0x0},    //mp
 {0x0, 0x0},    //sp
 0x43, 0x42, 0x0, 0x0, 0X0, 0x0, //hsa, hea, hsxa, hsya, hexa, heya
 0x708a,    //lc //old: 108a
 0x100,    //dpad
 0x0460, 0x7CA0,  //wrcycle, rdcycle
 0,     //rsPolarity
 0x0,    //dmap
 5,     //index select
 0x51,    //dispadr
 UG_120114A_Panel,  //paneltype
 0,     //reserve
 {
  0,  0,  1,  4,  9,  0x10, 0x1a, 0x27, 0x37,
  0x4a, 0x5f, 0x77, 0x91, 0xac, 0xc8, 0xe4, 0xff
 },     //gamma R;
 {
  0,  0,  1,  4,  9,  0x10, 0x1a, 0x27, 0x37,
  0x4a, 0x5f, 0x77, 0x91, 0xac, 0xc8, 0xe4, 0xff
 },     //gamma G;
 {
  0,  0,  1,  4,  9,  0x10, 0x1a, 0x27, 0x37,
  0x4a, 0x5f, 0x77, 0x91, 0xac, 0xc8, 0xe4, 0xff
 },     //gamma B
	{0xff, 0x0, 0x0, 0}		//background color
};

const V558_PANELINFO	SlavePanel_Info =
{
	"Lanser UG_12D181_Panel",		//description

	&SubPanel_LcdIF,
	{sizeof(SubPanel_powon) / sizeof(V558_PANELREGVALUE), SubPanel_powon},
	{0, NULL},
	{0, NULL},
	{0, NULL},
	{0, NULL},
	{0, NULL},
//	{sizeof(Panel_sleep) / sizeof(V558_PANELREGVALUE), Panel_sleep},
//	{sizeof(Panel_wakefs) / sizeof(V558_PANELREGVALUE), Panel_wakefs},
//	{sizeof(Panel_stdby) / sizeof(V558_PANELREGVALUE), Panel_stdby},
//	{sizeof(Panel_wakefstd) / sizeof(V558_PANELREGVALUE), Panel_wakefstd}
	V558_LcdSubPanelSetReg,
	NULL
};
#endif

#if	LP_8937_A_M16
const V558_PANELWRITEINFO Panel_WriteType=
{
	Low8,
	RegRsHigh16,
	Lcd_Multi16
};


const V558_PANELREGVALUE Panel_wakefstd[] =
{
	{0x9,	0x0,	100},
	{0x8,	0x3,	0}
};

const V558_PANELREGVALUE Panel_stdby[] =
{
	{0x8,	0x0,	0},
	{0x9,	0x2,	200},
	{0x9,	0x1,	0},
};
const V558_PANELREGVALUE Panel_wakefs[] =
{
	{0x9,	0x0,	100},
	{0x8,	0x3,	0},
};
const V558_PANELREGVALUE Panel_sleep[] =
{
	{0x8,	0x0,	0},
	{0x9,	0x2,	0},
};
const V558_PANELREGVALUE Panel_powon[] =
{	
				//  WrIndex 6 times
	{0,     0,   0},
	{0,     0,   0},
	{0,     0,   0},
	{0x2,	0x0411,	0},	//  software reset
	{0x3,	0x1030,	0},
	
	{0x1,	0x0300,	0},	//use-state setting
	{0x2,	0x0011,	0},
	{0xb,	0x0008,	0},	//frame cycle control register,
	
	{0x30,	0x0300,	0},	//grayscale palette control registers30~3fh
	{0x31,	0x0906,	0},
	{0x32,	0x0f0c,	0},
	{0x33,	0x1412, 0},
	{0x34,	0x1816,	0},
	{0x35,	0x1c1a,	0},
	{0x36,	0x201e,	0},
	{0x37,	0x2422,	0},
	{0x38,	0x2625,	0},
	{0x39,	0x2827,	0},
	{0x3a,	0x2a29,	0},
	{0x3b,	0x2c2b,	0},
	{0x3c,	0x2e2d,	0},
	{0x3d,	0x302f,	0},
	{0x3e,	0x3231,	0},
	{0x3f,	0x3433,	0},
	
	{0x9,	0x0,	10},	// normal mode
	
	{0x04,	0x7f8f,	0},		// contrast		
	{0x05,	0x0030,	0},		// scan mode
	{0x11,	0,	0},
	{0x13,	0x0,	20},		// sub_duty driving position register
	{0x14,	0x7f00,	20},
	{0x07,	0x0002,	0},
	{0x0b,	0x0008,	0},
	{0x17,	0x7f00,	0},
	{0x16,	0x7f00,	0},
	//{0x08,	0x0003,	0},	
	//{0x17,	0x7f00,	0},
	//{0x16,	0x7f00,	0},
	{0x21,	0,	100},
	{0x08,	0x0003,	0},
	//{0x21,	0,	0},
};
const V558_PANELREGVALUE Panel_powoff[] =
{
	{0x8,	0x0, 	0},
	{0x9,	0x2,	200},
	{0x9,	0x1,	60},

};

V558_LCDIFPARM Panel_LcdIF =
{
	{ 128,	128	},	//size
	{0, 0},			//mp
	{0, 0},			//sp
	0x21, 0, 0, 0, 0, 0,	//hsa, hea, hsxa, hsya, hexa, heya
	0x66cc,			//lc
	0x100,			//dpad
	0x460, 0xce0,		//wrcycle, rdcycle
	0,					//rsPolarity
	0x0,				//dmap
	0x1,				//index select
	0x22,			//dispadr
	LP8937_A_PANEL,		//paneltype
	0,					//reserve
	{
		0,		0,		1,		4,		9,		0x10,	0x1a,	0x27,	0x37,
		0x4a,	0x5f,	0x77,	0x91,	0xac,	0xc8,	0xe4,	0xff
	},					//gamma R;
	{
		0,		0,		1,		4,		9,		0x10,	0x1a,	0x27,	0x37,
		0x4a,	0x5f,	0x77,	0x91,	0xac,	0xc8,	0xe4,	0xff
	},					//gamma G;
	{
		0,		0,		1,		4,		9,		0x10,	0x1a,	0x27,	0x37,
		0x4a,	0x5f,	0x77,	0x91,	0xac,	0xc8,	0xe4,	0xff
	},					//gamma B
	{0xff, 0x0, 0x0, 0}		//background color
};

const V558_PANELINFO	Panel_Info =
{
	"PHILIPS LP8937_A",		//description

	&Panel_LcdIF,
	{sizeof(Panel_powon) / sizeof(V558_PANELREGVALUE), Panel_powon},
	{sizeof(Panel_powoff) / sizeof(V558_PANELREGVALUE), Panel_powoff},
	{sizeof(Panel_sleep) / sizeof(V558_PANELREGVALUE), Panel_sleep},
	//{0, NULL },
	{sizeof(Panel_wakefs) / sizeof(V558_PANELREGVALUE), Panel_wakefs},
	{sizeof(Panel_stdby) / sizeof(V558_PANELREGVALUE), Panel_stdby},
	{sizeof(Panel_wakefstd) / sizeof(V558_PANELREGVALUE), Panel_wakefstd},
	V558_LcdPanelSetReg,
	NULL
};
#endif
#if	LP_8937_A_M8
const V558_PANELWRITEINFO Panel_WriteType=
{
	Low8,
	RegRsHigh16,
	Lcd_Multi8
};


const V558_PANELREGVALUE Panel_wakefstd[] =
{
	{0x9,	0x0,	100},
	{0x8,	0x3,	0}
};

const V558_PANELREGVALUE Panel_stdby[] =
{
	{0x8,	0x0,	0},
	{0x9,	0x2,	200},
	{0x9,	0x1,	0},
};
const V558_PANELREGVALUE Panel_wakefs[] =
{
	{0x9,	0x0,	100},
	{0x8,	0x3,	0},
};
const V558_PANELREGVALUE Panel_sleep[] =
{
	{0x8,	0x0,	0},
	{0x9,	0x2,	0},
};
const V558_PANELREGVALUE Panel_powon[] =
{	
				//  WrIndex 6 times
	{0,     0,   0},
	{0,     0,   0},
	{0,     0,   0},
	{0x2,	0x0411,	0},	//  software reset
	{0x3,	0x1030,	0},
	
	{0x1,	0x0300,	0},	//use-state setting
	{0x2,	0x0011,	0},
	{0xb,	0x0008,	0},	//frame cycle control register,
	
	{0x30,	0x0300,	0},	//grayscale palette control registers30~3fh
	{0x31,	0x0906,	0},
	{0x32,	0x0f0c,	0},
	{0x33,	0x1412, 0},
	{0x34,	0x1816,	0},
	{0x35,	0x1c1a,	0},
	{0x36,	0x201e,	0},
	{0x37,	0x2422,	0},
	{0x38,	0x2625,	0},
	{0x39,	0x2827,	0},
	{0x3a,	0x2a29,	0},
	{0x3b,	0x2c2b,	0},
	{0x3c,	0x2e2d,	0},
	{0x3d,	0x302f,	0},
	{0x3e,	0x3231,	0},
	{0x3f,	0x3433,	0},
	
	{0x9,	0x0,	10},	// normal mode
	
	{0x04,	0x7f8f,	0},		// contrast		
	{0x05,	0x0030,	0},		// scan mode
	{0x11,	0,	0},
	{0x13,	0x0,	20},		// sub_duty driving position register
	{0x14,	0x7f00,	20},
	{0x07,	0x0002,	0},
	{0x0b,	0x0008,	0},
	{0x17,	0x7f00,	0},
	{0x16,	0x7f00,	0},
	//{0x08,	0x0003,	0},	
	//{0x17,	0x7f00,	0},
	//{0x16,	0x7f00,	0},
	{0x21,	0,	100},
	{0x08,	0x0003,	0},
	//{0x21,	0,	0},
};
const V558_PANELREGVALUE Panel_powoff[] =
{
	{0x8,	0x0, 	0},
	{0x9,	0x2,	200},
	{0x9,	0x1,	60},

};

V558_LCDIFPARM Panel_LcdIF =
{
	{ 128,	128	},	//size
	{0, 0},			//mp
	{0, 0},			//sp
	0x21, 0, 0, 0, 0, 0,	//hsa, hea, hsxa, hsya, hexa, heya
	0x66cc,			//lc
	0x100,			//dpad
	0x460, 0xce0,		//wrcycle, rdcycle
	0,					//rsPolarity
	 0x4,
	 0x3,
	0x22,			//dispadr
	LP8937_A_PANEL,		//paneltype
	0,					//reserve
	{
		0,		0,		1,		4,		9,		0x10,	0x1a,	0x27,	0x37,
		0x4a,	0x5f,	0x77,	0x91,	0xac,	0xc8,	0xe4,	0xff
	},					//gamma R;
	{
		0,		0,		1,		4,		9,		0x10,	0x1a,	0x27,	0x37,
		0x4a,	0x5f,	0x77,	0x91,	0xac,	0xc8,	0xe4,	0xff
	},					//gamma G;
	{
		0,		0,		1,		4,		9,		0x10,	0x1a,	0x27,	0x37,
		0x4a,	0x5f,	0x77,	0x91,	0xac,	0xc8,	0xe4,	0xff
	},					//gamma B
	{0xff, 0x0, 0x0, 0}		//background color
};

const V558_PANELINFO	Panel_Info =
{
	"PHILIPS LP8937_A",		//description

	&Panel_LcdIF,
	{sizeof(Panel_powon) / sizeof(V558_PANELREGVALUE), Panel_powon},
	{sizeof(Panel_powoff) / sizeof(V558_PANELREGVALUE), Panel_powoff},
	{sizeof(Panel_sleep) / sizeof(V558_PANELREGVALUE), Panel_sleep},
	//{0, NULL },
	{sizeof(Panel_wakefs) / sizeof(V558_PANELREGVALUE), Panel_wakefs},
	{sizeof(Panel_stdby) / sizeof(V558_PANELREGVALUE), Panel_stdby},
	{sizeof(Panel_wakefstd) / sizeof(V558_PANELREGVALUE), Panel_wakefstd},
	V558_LcdPanelSetReg,
	NULL
};
#endif
#if	CV90_K2817_SSD1781
const V558_PANELWRITEINFO Panel_WriteType=
{
	Low8,
	RegRsHigh8,
	Lcd_Multi8
};

const V558_PANELREGVALUE Panel_powon[] =
{	
				//  WrIndex 6 times
	{0xd1,  0,   	0},
	{0x20,  0xf,	0},
	
	{0x81,  	0x23,   	0},
	{0xffff,	5,    	0},
	
	{0xfb,	0x05,	0},
	
	{0xf2,	0x06,	0},
	{0xffff,	0x06,	0},
	
	{0xbc,	0x0,	0},
	{0xffff,	0x0,	0},
	{0xffff,	0,	0},
	
	{0x15,	0x0,	0},
	{0xffff,	0x7f,	0},
	
	{0x75,	0,	0},
	{0xffff,	0x83,	0},

	{0xbb,	1,	0},
	
	{0xca,	0,	0},
	{0xffff,	0x20,	0},
	{0xffff,	0,	0},

	{0x94,	0,	0},
	
	{0xa7,	0,	0},
	{0x5c,	0xff,	0},
	
	{0xaf,	0,	0}	

};

V558_LCDIFPARM Panel_LcdIF =
{
	{ 128,	128	},	//size
	{0, 0},			//mp
	{0, 0},			//sp
	0x15, 0x75, 0, 0, 0, 0,	//hsa, hea, hsxa, hsya, hexa, heya
	0xd0cc,			//lc
	0x100,			//dpad
	0x460, 0x460,		//wrcycle, rdcycle
	0,					//rsPolarity
	0x4,
	0x1,			//index select
	0x5c,			//dispadr
	CV90_K2817_SSD1781_PANEL,		//paneltype
	0,			//reserve
	{
		0,		0,		1,		4,		9,		0x10,	0x1a,	0x27,	0x37,
		0x4a,	0x5f,	0x77,	0x91,	0xac,	0xc8,	0xe4,	0xff
	},					//gamma R;
	{
		0,		0,		1,		4,		9,		0x10,	0x1a,	0x27,	0x37,
		0x4a,	0x5f,	0x77,	0x91,	0xac,	0xc8,	0xe4,	0xff
	},					//gamma G;
	{
		0,		0,		1,		4,		9,		0x10,	0x1a,	0x27,	0x37,
		0x4a,	0x5f,	0x77,	0x91,	0xac,	0xc8,	0xe4,	0xff
	},					//gamma B
	{0xff, 0x0, 0x0, 0}		//background color
};

const V558_PANELINFO	Panel_Info =
{
	"CV90_K2817_SSD1781",		//description

	&Panel_LcdIF,
	{sizeof(Panel_powon) / sizeof(V558_PANELREGVALUE), Panel_powon},
	{0, NULL},
	{0, NULL},
	{0, NULL},
	{0, NULL},
	{0, NULL},

	V558_LcdPanelSetReg,
	NULL
};

#endif
#if(WB_F1216V2_6FLWB)

const V558_PANELWRITEINFO Panel_WriteType=
{
	High8,
	RegRsHigh16,
	Lcd_Multi16
};
const V558_PANELREGVALUE Panel_wakefstd[] =
{
	{0x0,	0x01,	10},//osc start
	{0x3,	0x0488,	10},//stb=0;	
	
	{0x7,	0x01,	40},//display on
	{0x7,	0x21,	0},
	{0x7,	0x23,	40},
	{0x7,	0x33,	0},

};

const V558_PANELREGVALUE Panel_stdby[] =
{
	{0xb,	0x4000,	0},
	{0x7,	0x0032,	40},	//display off
	{0x7,	0x0022,	40},
	{0x7,	0x0,	0},

	{0x3,	0x0480,	0}	//ap2~0=000
};

static V558_PANELREGVALUE Panel_wakefs[] =
{
	{0x3,	0x0488,	20}

};

const V558_PANELREGVALUE Panel_sleep[] =
{
	{0xb,	0x4000,	0},
	{0x7,	0x0032,	0},	//display off
	{0x7,	0x0022,	40},
	{0x7,	0x0,	40},
	{0x3,	0x048a,	0}	//slp=1
};

const V558_PANELREGVALUE Panel_powoff[] =
{
	{0xb,	0x4000,	0},
	{0x7,	0x0032,	20},
	{0x7,	0x0022,	20},
	{0x7,	0x0,	0},

	{0x3,	0x0480,	0}
};

const V558_PANELREGVALUE Panel_powon[] =
{

	{0x7,	0x004,	0},
	{0xd,	0x203,	0},
	{0xe,	0x1115,	0},
	{0x1,	0x113,	0},
	{0x2,	0x0700,	0},
	{0x5,	0x1230,	0},
	{0x6,	0x0,	0},
	{0xb,	0x4000,	0},
	{0xc,	0x3,	0},
	{0x3,	0x488,	0},
	{0x4,	0x0,	0},
	{0xe,	0x3115,	0},
	{0xd,	0x213,	0},
	{0x21,	0x0,	0},
	{0x30,	0x0,	0},
	{0x31,	0x205,	0},
	{0x32,	0x202,	0},
	{0x33,	0x0,	0},
	{0x34,	0x504,	0},
	{0x35,	0x202,	0},
	{0x36,	0x707,	0},
	{0x37,	0,	0},
	{0xf,	0,	0},
	{0x11,	0,	0},
	{0x14,	0x9f00,	0},
	{0x15,	0x00,	0},
	{0x16,	0x7f00,	0},
	{0x17,	0x9f00,	0},
	{0x3a,   0x1000,	0},
	{0x3b,	0xb,	5},
	
	{0x7,	0x05,	40},//	display on
	{0x7,	0x25,	0},
	{0x7,	0x27,	40},
	{0x7,	0x37,	0},

	{0x21,	0,	0}	
	
};

V558_LCDIFPARM Panel_LcdIF =
{
	{ 128,	160},			//size
	{0, 0},				//mp
	{0, 0},				//sp
	0x21, 0, 0, 0, 0, 0,	//hsa, hea, hsxa, hsya, hexa, heya
	0xffcc,				//lc
	0x100,				//dpad
	0x460, 0x460,		//wrcycle, rdcycle
	0,					//rsPolarity
	0x0,				//dmap
	1,					//index select
	0x22,				//dispadr
	0,		//paneltype		
	0,					//reserve
	{
		0,		0,		1,		4,		9,		0x10,	0x1a,	0x27,	0x37,
		0x4a,	0x5f,	0x77,	0x91,	0xac,	0xc8,	0xe4,	0xff
	},					//gamma R;
	{
		0,		0,		1,		4,		9,		0x10,	0x1a,	0x27,	0x37,
		0x4a,	0x5f,	0x77,	0x91,	0xac,	0xc8,	0xe4,	0xff
	},					//gamma G;
	{
		0,		0,		1,		4,		9,		0x10,	0x1a,	0x27,	0x37,
		0x4a,	0x5f,	0x77,	0x91,	0xac,	0xc8,	0xe4,	0xff
	},					//gamma B
	{0xff, 0x0, 0x0, 0}		//background color
};
const V558_PANELINFO	Panel_Info =
{
	"WB_F1216V2_6FLWB",		//description

	&Panel_LcdIF,
	{sizeof(Panel_powon) / sizeof(V558_PANELREGVALUE), Panel_powon},
	{sizeof(Panel_powoff) / sizeof(V558_PANELREGVALUE), Panel_powoff},
	{sizeof(Panel_sleep) / sizeof(V558_PANELREGVALUE), Panel_sleep},
	{sizeof(Panel_wakefs) / sizeof(V558_PANELREGVALUE), Panel_wakefs},
	{sizeof(Panel_stdby) / sizeof(V558_PANELREGVALUE), Panel_stdby},
	{sizeof(Panel_wakefstd) / sizeof(V558_PANELREGVALUE), Panel_wakefstd},

	V558_LcdPanelSetReg,
	NULL
};

const V558_PANELWRITEINFO SubPanel_WriteType=
{
	Low8,
	RegRsHigh8,
	Lcd_Multi8
};

const V558_PANELREGVALUE SubPanel_powon[] =
{	
	
	{0xd1,  0,   	0},
	{0x20,  0xf,	0},
	
	{0x81,  	0x29,   	0},
	{0xffff,	5,    	0},
	
	{0xfb,	0x03,	0},
	
	{0xf2,	0x06,	0},
	{0xffff,	0x06,	0},
	
	{0xbc,	0x4,	0},
	{0xffff,	0x0,	0},
	{0xffff,	0x02,	0},
	
	{0x15,	0x04,	0},
	{0xffff,	0x33,	0},
	
	{0x75,	0,	0},
	{0xffff,	0x3f,	0},

	{0xbb,	1,	0},
	
	{0xca,	0,	0},
	{0xffff,	0xf,	0},
	{0xffff,	0,	0},

	{0x94,	0,	0},
	
	{0xae,	0,	0},
	{0x5c,	0xaa,	0},
	
	{0xa7,	0,	0},
	{0xaf,	0,	0}	

	
};
V558_LCDIFPARM SubPanel_LcdIF =
{
	{ 96,	64},	//size
	{0x0, 0x00},			//mp
	{0, 0},			//sp
	0x15, 0x75, 0, 0, 0, 0,	//hsa, hea, hsxa, hsya, hexa, heya
	0xd0cc,			//lc
	0x100,			//dpad
	0x460, 0x460,		//wrcycle, rdcycle
	0,					//rsPolarity
	0x0c,				//dmap
			
	0x1,			//index select
	0x5c,			//dispadr
	0,	//paneltype
	0,			//reserve
	{
		0,		0,		1,		4,		9,		0x10,	0x1a,	0x27,	0x37,
		0x4a,	0x5f,	0x77,	0x91,	0xac,	0xc8,	0xe4,	0xff
	},					//gamma R;
	{
		0,		0,		1,		4,		9,		0x10,	0x1a,	0x27,	0x37,
		0x4a,	0x5f,	0x77,	0x91,	0xac,	0xc8,	0xe4,	0xff
	},					//gamma G;
	{
		0,		0,		1,		4,		9,		0x10,	0x1a,	0x27,	0x37,
		0x4a,	0x5f,	0x77,	0x91,	0xac,	0xc8,	0xe4,	0xff
	},					//gamma B
	{0xff, 0x0, 0x0, 0}		//background color
};
const V558_PANELINFO	SlavePanel_Info=
{
	"WB_F1216V2_6FLWB",		//description

	&SubPanel_LcdIF,
	{sizeof(SubPanel_powon) / sizeof(V558_PANELREGVALUE),SubPanel_powon},
	{0, NULL},
	{0, NULL},
	{0, NULL},
	{0, NULL},
	{0, NULL},

	V558_LcdSubPanelSetReg,
	NULL

};
#endif
#if(MGG1256DTSW_ZXRT)
const V558_PANELWRITEINFO Panel_WriteType=
{
	Low8,
	RegRsHigh8,
	Lcd_Multi16
};

const V558_PANELREGVALUE Panel_powon[] =
{

	{0x1,	0xffff,	10},//reset
	{0xee,	0xffff,	0},
	{0x11,	0xffff,	0},
	{0x11,	0xffff,	0},
	{0x29,	0xffff,	0}, //display on
	{0x3a,	0x5,	0}, //display data formating:565,16bit/pixel 
	{0xc2,	0x03,	0}, //STCTR ADDCTR=11
	{0xc7,	0x1,	0}, //?
	{0xb0,	0xba,	0},
	{0x25,	0x3f,	0}, //contrast
	{0x20, 0xffff,	0},
	{0x2a,	0x04,	0}, //set xstart xend
	{0xffff,	0x83,	10},
	{0x2b,	0x00,	0}, //set ystart yend
	{0xffff,	0x7f,	0},
	{0x36,	0xc8,	0},
	{0xc5,	0x02,	0}, //frame
	{0xffff,	0x02,	0},
	{0xffff,	0x02,	0},
	{0xffff,	0x2,	0},
	{0xffff,	2,	0},
	{0xb3,	7,	0},
	{0xc3,	0x74,	0},
	{0xc4,	0x4,	0},
	
	{0xb9,	0xffff,	0},
	{0xc9,	0x1a,	0},
	{0xb4,	0xffff,	0},
	{0xb6,	0xffff,	0},
	{0x13,	0xffff,	0},
	{0x35,	0,	0},
	{0xf2,	0x0,	0},
	{0xf3,	0,	0},
	{0xf8,	8,	0},

	
};

V558_LCDIFPARM Panel_LcdIF =
{
	{ 128,	128},			//size
	{4, 0},				//mp
	{0, 0},				//sp
	0x2a, 0x2b, 0, 0, 0, 0,	//hsa, hea, hsxa, hsya, hexa, heya
	0xd08c,				//lc
	0x100,				//dpad
	0x460, 0x880,		//wrcycle, rdcycle
	0,			//rsPolarity
	0x0,			//dmap
	0x1,			//index select
	0x2c,			//dispadr
	0,		//paneltype		
	0,					//reserve
	{
		0,		0,		1,		4,		9,		0x10,	0x1a,	0x27,	0x37,
		0x4a,	0x5f,	0x77,	0x91,	0xac,	0xc8,	0xe4,	0xff
	},					//gamma R;
	{
		0,		0,		1,		4,		9,		0x10,	0x1a,	0x27,	0x37,
		0x4a,	0x5f,	0x77,	0x91,	0xac,	0xc8,	0xe4,	0xff
	},					//gamma G;
	{
		0,		0,		1,		4,		9,		0x10,	0x1a,	0x27,	0x37,
		0x4a,	0x5f,	0x77,	0x91,	0xac,	0xc8,	0xe4,	0xff
	},					//gamma B
	{0xff, 0x0, 0x0, 0}		//background color
};

const V558_PANELINFO	Panel_Info =
{
	"Truly_MGG1256DTSW_ZXRT",		//description

	&Panel_LcdIF,
	{sizeof(Panel_powon) / sizeof(V558_PANELREGVALUE), Panel_powon},
	
	{0, NULL},
	{0, NULL},
	{0, NULL},
	{0, NULL},
	{0, NULL},
	V558_LcdPanelSetReg,
	NULL
};

const V558_PANELWRITEINFO SubPanel_WriteType=
{
	Low8,
	RegRsHigh8,
	Lcd_Multi8
};

const V558_PANELREGVALUE SubPanel_powon[] =
{	
	
	{0x1,	0xffff,	10},//reset
	{0xee,	0xffff,	0},
	{0xb0,	0x4e,	0},
	{0xc7,	0x0,	10}, //?
	{0x25,	0x26,	0}, //contrast
	{0x2a,	0x02,	0}, //set xstart xend
	{0xffff,	0x61,	10},
	{0x2b,	0x00,	0}, //set ystart yend
	{0xffff,	0x3f,	0},
	{0xc5,	0x38,	0}, //frame
	{0xffff,	0x20,	0},
	{0xffff,	0x25,	0},
	{0xffff,	0x1a,	0},
	{0xffff,	0x23,	0},
	{0xc3,	0x33,	0},
	{0xc4,	0x65,	0},
	{0xc2,	0x01,	0},
	{0xc9,	0x08,	0},
	{0x3a,	0x05,	0},	
	{0x36,	0xc8,	0},
	{0x21,	0xffff,	0}, 
	{0x11,	0xffff,	0}

	
};
V558_LCDIFPARM SubPanel_LcdIF =
{
	{ 96,	64},	//size
	{2, 0},	//mp
	{0, 0},			//sp
	0x2a, 0x2b, 0, 0, 0, 0,	//hsa, hea, hsxa, hsya, hexa, heya
	0xd09c,			//lc
	0x100,			//dpad
	0x460, 0x880,		//wrcycle, rdcycle
	0,				//rsPolarity
	4,				//dmap		
	0x1,			//index select
	0x2c,			//dispadr
	0,			//paneltype
	0,			//reserve
	{
		0,		0,		1,		4,		9,		0x10,	0x1a,	0x27,	0x37,
		0x4a,	0x5f,	0x77,	0x91,	0xac,	0xc8,	0xe4,	0xff
	},					//gamma R;
	{
		0,		0,		1,		4,		9,		0x10,	0x1a,	0x27,	0x37,
		0x4a,	0x5f,	0x77,	0x91,	0xac,	0xc8,	0xe4,	0xff
	},					//gamma G;
	{
		0,		0,		1,		4,		9,		0x10,	0x1a,	0x27,	0x37,
		0x4a,	0x5f,	0x77,	0x91,	0xac,	0xc8,	0xe4,	0xff
	},					//gamma B
	{0xff, 0x0, 0x0, 0}		//background color
};
const V558_PANELINFO	SlavePanel_Info=
{
	"Truly_MGG1256DTSW_ZXRT",		//description

	&SubPanel_LcdIF,
	{sizeof(SubPanel_powon) / sizeof(V558_PANELREGVALUE),SubPanel_powon},
	{0, NULL},
	{0, NULL},
	{0, NULL},
	{0, NULL},
	{0, NULL},

	V558_LcdSubPanelSetReg,
	NULL
};
#endif

#if  LMG_128FE9
const V558_PANELWRITEINFO Panel_WriteType=
{
	Low8,
	RegRsHigh16,
	Lcd_Multi8
};
const V558_PANELREGVALUE Panel_wakefstd[] =
{
	{0x9,	0x0,	100},
	{0x8,	0x3,	0}
};

const V558_PANELREGVALUE Panel_stdby[] =
{
	{0x8,	0x0,	0},
	{0x9,	0x2,	200},
	{0x9,	0x1,	0},
};
const V558_PANELREGVALUE Panel_wakefs[] =
{
	{0x9,	0x0,	100},
	{0x8,	0x3,	0},
};
const V558_PANELREGVALUE Panel_sleep[] =
{
	{0x8,	0x0,	0},
	{0x9,	0x2,	0},
};
const V558_PANELREGVALUE Panel_powon[] =
{	
				//  WrIndex 6 times
	{0,     0,   0},
	{0,     0,   0},
	{0,     0,   0},
	{0x2,	0x0,	0},	//  software reset
	{0x3,	0x0,	0},
	
	{0x1,	0x0300,	0},	//use-state setting
	{0x0e,	0x03,	0},
	{0x03,	0,	0},
	//{0x2,	0x0000,	0},
			
	{0x30,	0x0300,	0},	//grayscale palette control registers30~3fh
	{0x31,	0x0906,	0},
	{0x32,	0x0f0c,	0},
	{0x33,	0x1412, 0},
	{0x34,	0x1816,	0},
	{0x35,	0x1c1a,	0},
	{0x36,	0x201e,	0},
	{0x37,	0x2422,	0},
	{0x38,	0x2625,	0},
	{0x39,	0x2827,	0},
	{0x3a,	0x2a29,	0},
	{0x3b,	0x2c2b,	0},
	{0x3c,	0x2e2d,	0},
	{0x3d,	0x302f,	0},
	{0x3e,	0x3231,	0},
	{0x3f,	0x3433,	0},

	{0x9,	0x0,	10},	// normal mode


	{0x04,	0x00e0,	0},		// contrast		
	{0x05,	0x0030,	0},		// scan mode
	{0x11,	0,	0},
	{0x13,	0x0,	20},		// sub_duty driving position register
	{0x14,	0xa000,	20},
	{0x07,	0x0000,	0},
	{0x9,	0x0,	0},
	{0xa,	0x03,	0},
	{0x0b,	0x0008,	0},
	{0x17,	0x9f00,	0},
	{0x16,	0x7f00,	0},
	{0x21,	0,	100},
	{0x08,	0x0003,	0},
};
const V558_PANELREGVALUE Panel_powoff[] =
{
	{0x8,	0x0, 	0},
	{0x9,	0x2,	200},
	{0x9,	0x1,	60},

};
V558_LCDIFPARM Panel_LcdIF =
{
	{ 128,	160	},	//size
	{0, 0},			//mp
	{0, 0},			//sp
	0x21, 0, 0, 0, 0, 0,	//hsa, hea, hsxa, hsya, hexa, heya
	0x668c,			//lc
	0x100,			//dpad
	0x460, 0xce0,		//wrcycle, rdcycle
	0,					//rsPolarity	
	0x4,			//dmap
	0x3,			//index select
	0x22,			//dispadr
	0,		//paneltype		
	0,					//reserve
	{
		0,		0,		1,		4,		9,		0x10,	0x1a,	0x27,	0x37,
		0x4a,	0x5f,	0x77,	0x91,	0xac,	0xc8,	0xe4,	0xff
	},					//gamma R;
	{
		0,		0,		1,		4,		9,		0x10,	0x1a,	0x27,	0x37,
		0x4a,	0x5f,	0x77,	0x91,	0xac,	0xc8,	0xe4,	0xff
	},					//gamma G;
	{
		0,		0,		1,		4,		9,		0x10,	0x1a,	0x27,	0x37,
		0x4a,	0x5f,	0x77,	0x91,	0xac,	0xc8,	0xe4,	0xff
	},					//gamma B
	{0xff, 0x0, 0x0, 0}		//background color
};

const V558_PANELINFO	Panel_Info =
{
	"Laser_lmg_128FE9",	//description

	&Panel_LcdIF,
	{sizeof(Panel_powon) / sizeof(V558_PANELREGVALUE), Panel_powon},
	{sizeof(Panel_powoff) / sizeof(V558_PANELREGVALUE), Panel_powoff},
	{sizeof(Panel_sleep) / sizeof(V558_PANELREGVALUE), Panel_sleep},
	//{0, NULL },
	{sizeof(Panel_wakefs) / sizeof(V558_PANELREGVALUE), Panel_wakefs},
	{sizeof(Panel_stdby) / sizeof(V558_PANELREGVALUE), Panel_stdby},
	{sizeof(Panel_wakefstd) / sizeof(V558_PANELREGVALUE), Panel_wakefstd},
	V558_LcdPanelSetReg,
	NULL

};
#endif

void V558_GetCmd_By(UINT8* uVal)
{
	*uVal=(UINT8)*(volatile UINT16*)(BYPASS_ADDR_LCDADDR);
}
void V558_GetVal_By(UINT8 * uVal)
{
	*uVal=(UINT8)*(volatile UINT16*)(BYPASS_ADDR_LCDVAL);
}
void V558_SetCmd_By(UINT16 uVal)
{
if(Panel_WriteType.BusType==Lcd_Multi16)
	*(volatile UINT16*)(BYPASS_ADDR_LCDADDR) = uVal;
else
	*(volatile UINT8*)(BYPASS_ADDR_LCDADDR) = uVal;
}
void V558_SetVal_By(UINT16 uVal)
{
	
if(Panel_WriteType.BusType==Lcd_Multi16)
	*(volatile UINT16*)(BYPASS_ADDR_LCDVAL) = uVal;
else
	*(volatile UINT8*)(BYPASS_ADDR_LCDVAL) = uVal;
}

void V558_SetReg_By(UINT16 uCmd, UINT16 uVal)
{
	*(volatile UINT16*)(BYPASS_ADDR_LCDADDR) = uCmd;
	*(volatile UINT16*)(BYPASS_ADDR_LCDVAL) = uVal;
}
void V558_SetData_By(UINT16 uCmd, UINT8* pData, UINT32 uSize)
{
	//UINT16 i ;
	//UINT8 uTemp;
	UINT8 uLeftByte = 0;
	
	if(Panel_WriteType.BusType==Lcd_Multi8)
		*(volatile UINT8*)(BYPASS_ADDR_LCDADDR) = uCmd;
	else
		*(volatile UINT16*)(BYPASS_ADDR_LCDADDR) = uCmd;


	if((uSize % 2)==1)
	{
		uLeftByte = 1;
	}
	uSize /= 2;

	while(uSize--)
	{
		if(Panel_WriteType.BusType==Lcd_Multi16)
		{
		*(volatile UINT16*)(BYPASS_ADDR_LCDVAL) = *((UINT16*)pData);
		pData+=2;
		}
		else
		{
			*(volatile UINT8*)(BYPASS_ADDR_LCDVAL) = *(pData+1);
			*(volatile UINT8*)(BYPASS_ADDR_LCDVAL) = *pData;
			pData+=2;
		}
		
	}

	if(uLeftByte)
	{
		if(Panel_WriteType.BusType==Lcd_Multi8)
			*(volatile UINT8*)(BYPASS_ADDR_LCDVAL) = *(pData);
		else
			*(volatile UINT16*)(BYPASS_ADDR_LCDVAL) = *(pData);
	}

}

