
#include "internal.h"
#ifndef	V578CMD_DISABLE


/*****************************************************************************************************
		SENSOR/SIF/ISP information
*****************************************************************************************************/

#if V5_DEF_SNR_OV_3610_BAYER

void SensorSetReg_OV3610_BAYER(UINT8 uAddr, UINT16 uVal)
{
	V5B_SifI2cWriteByte((UINT8)uAddr, (UINT8)uVal);
}

void SensorGetReg_OV3610_BAYER(UINT8 uAddr, UINT16 *uVal)
{
	V5B_SifI2cReadByte((UINT8)uAddr, (UINT8*)uVal);
}

UINT8 SensorIdChk_OV3610_BAYER(void)
{
	UINT8 val = 0;

	V5B_SifI2cReadByte(0x0a, &val);
	if(val != 0x36)
		return FAILED;
	V5B_SifI2cReadByte(0x0b, &val);
	if(val == 0x10)
		return SUCCEED;
	return FAILED;
}

const TReg gSifPwrOnSeqRegVal_OV3610_BAYER[] =
{
	{0x00, 0x20,	1},
	{0x00, 0x21,	1},
	{0x00, 0x20,	1}
};

const TReg	gSifRegVal_OV3610_BAYER[] =
{
	{0x00, 0x20,	0},
	{0x08, 0x01,	0},
	{0x09, 0x0c,	0},
	{0x03, 0x0a,	0},
	{0x20, 0x00,	0},
	{0x21, 0x00,	0},
	{0x22, 0x06,	0},
	{0x23, 0x00,	0},
	{0x04, 0x05,	0},
	{0x14, 0x00,	0},
	{0x15, 0x00,	0},
	{0x16, 0x07,	0},
	{0x17, 0xff,	0},
	{0x34, 0x01,	0},
	{0x01, 0x45,	0}
};

const TReg	gIspRegVal_OV3610_BAYER[] =
{
	{0x00,	0xa1,	0}
};

// QXGA mode(2048x1536)
const TReg gSnrSizeRegValQXGA_OV3610_BAYER[] = 
{
	{0x12,	0x00,	1},
	{0x11,	0x80,	1}
};

// XGA mode(1024x768)
const TReg gSnrSizeRegValXGA_OV3610_BAYER[] = 
{
	{0x12,	0x40,	1},
	{0x11,	0x80,	1}
};

const TSnrSizeCfg gSnrSizeCfg_OV3610_BAYER[] = 
{
	{
		{sizeof(gSnrSizeRegValQXGA_OV3610_BAYER) / sizeof(TReg), (PTReg)&gSnrSizeRegValQXGA_OV3610_BAYER},
		{0, NULL},
		{2048, 1536},
		30,				// maxet
		0,
		0
	},
	{
		{sizeof(gSnrSizeRegValXGA_OV3610_BAYER) / sizeof(TReg), (PTReg)&gSnrSizeRegValXGA_OV3610_BAYER},
		{0, NULL},
		{1024,768},
		30,				// maxet
		0,
		0
	}
};

const TSnrInfo gSensorInfo_OV3610_BAYER = 
{
	"OV3610 Bayer",

	0,																										//bayer mode
	0,																										//not use PCLK of sensor
	0,																										//sensor pclk value : KHz
	0,																										//[0]: VSync delay enable; [1]: Fifo VSync delay enable; [2]: Use PCLK Negtive Edge.
	0,																										//0 -- I2C, 1 -- Serial bus;
	0x01,																									//[0]: 1 -- OV I2C bus
	100,																									//I2C Rate : 100KHz
	0x60,																									//I2C address
	0x60,																									//ISP I2C address for special sensor
	0,																										//power config
	0x01,																									//reset sensor

	32,																										//brightness 
	32,																										//contrast
	32,																										//saturation
	{
		0x00,0x04,0x16,0x30,0x4E,0x68,0x81,0x98,0xAC,0xBE,0xCD,0xDA,0xE4,0xED,0xF5,0xfB,0xff				//gamma table
	},

	{0,NULL},																								//sensor poweroff
	{0,NULL},																								//sensor standby
	{0,NULL},																								//sensor sleep
	{0, NULL},

	{sizeof(gSifPwrOnSeqRegVal_OV3610_BAYER) / sizeof(TReg), (PTReg)&gSifPwrOnSeqRegVal_OV3610_BAYER},		//Sif config sequence(Reg.800 bit0~2) when sensor power on
	{0, NULL},																								//Sif config sequence(Reg.800 bit0~2) when sensor standby
	{sizeof(gSifRegVal_OV3610_BAYER) / sizeof(TReg), (PTReg)&gSifRegVal_OV3610_BAYER},						//sif initial value
	{sizeof(gIspRegVal_OV3610_BAYER) / sizeof(TReg), (PTReg)&gIspRegVal_OV3610_BAYER},						//isp initial value
	{sizeof(gSnrSizeCfg_OV3610_BAYER) / sizeof(TSnrSizeCfg), (PTSnrSizeCfg)&gSnrSizeCfg_OV3610_BAYER},		//size configure 

	SensorSetReg_OV3610_BAYER,																				//set reg callback
	SensorGetReg_OV3610_BAYER,																				//get reg callback
	SensorIdChk_OV3610_BAYER,																				//Sensor ID Check Call
	NULL,																									//Set Sensor Mirror Flip Call
	NULL,																									//Sensor switch size callback
	NULL,																									//set et callback

	{//	ytarget	ythd ytop gaintarget gainthd,gaindelay,gain,minet,maxgain,mingain,speed
		0x78, 0x11, 0x9f, 0x60, 0x20, 1, 0x60, 0, 0xe0, 0x40, 0												//should be add to other sensor configure
	},
	{	//ythd of flash,et of flash
		0x20,			10
	},
	1																										//Histogram ratio number, 1 is recommend value
};

#endif

#if V5_DEF_SNR_OV_9640_BAYER

void SensorSetReg_OV9640_BAYER(UINT8 uAddr, UINT16 uVal)
{
	V5B_SifI2cWriteByte((UINT8)uAddr, (UINT8)uVal);
}

void SensorGetReg_OV9640_BAYER(UINT8 uAddr, UINT16 *uVal)
{
	V5B_SifI2cReadByte((UINT8)uAddr, (UINT8*)uVal);
}

UINT8 SensorIdChk_OV9640_BAYER(void)
{
	UINT8 val = 0;

	V5B_SifI2cReadByte(0x0a, &val);
	if(val != 0x96)
		return FAILED;
	V5B_SifI2cReadByte(0x0b, &val);
	if(val == 0x49)
		return SUCCEED;
	return FAILED;
}

void SensorSetMirrorFlip_OV9640_BAYER(UINT8 val)
{
	UINT8 x = 0;

	V5B_SifI2cReadByte(0x1e, &x);
	x&=0xcf;
	x |= (val&0x03)<<4;
	V5B_SifI2cWriteByte(0x1e, x);
}

/********************************************************************************
Description:	Write sensor register to change exposure time
Parameter: t -- exposure time clock num 
freqtype -- frequence type 50 or 60
*********************************************************************************/
void SensorSetExposeTime_OV9640_BAYER(UINT8 freqtype, UINT32 t, TSize size, UINT32 snrclock)
{
	UINT16 vBlank = 0, times, uTemp = 0;
	UINT8 uVal = 0;
	UINT32 maxT = 0;
	TI2cBatch AeData[3];

	if(freqtype)				//60hz
		snrclock = snrclock/120;
	else						//50hz
		snrclock = snrclock/100;
	times = (UINT16)(t/snrclock);

	switch(size.cx)
	{
	case 640:
		maxT = (UINT32)(500*800)/snrclock;
		if(times > maxT)
		{
			uTemp = 500;
			vBlank = (UINT16)(t/800-500);
		}
		else
		{
			uTemp = (UINT16)(t/800);
			vBlank = 0x00;
		} 
		break;
	case 1280:
		maxT = (UINT32)(1000*1600)/snrclock;
		if(times > maxT)		
		{
			uTemp = 1000;
			vBlank = (UINT16)(t/1600-1000);
		}
		else
		{
			uTemp = (UINT16)(t/1600);
			vBlank = 0x00;
		} 
		break;
	default:
		break;
	}

	AeData[0].RegBytes  = 2;
	AeData[0].RegAddr   = 0x2d;
	AeData[0].RegVal[0] = (UINT8)(vBlank);
	AeData[0].RegVal[1] = (UINT8)(vBlank>>8);

	V5B_SifI2cReadByte(0x04, &uVal);							//read sensor setting

	AeData[1].RegBytes  = 1;
	AeData[1].RegAddr   = 0x04;
	AeData[1].RegVal[0] = (UINT8)((uTemp&0x03)|(uVal&0xfc));	//low 2 bit

	AeData[2].RegBytes  = 1;
	AeData[2].RegAddr   = 0x10;
	AeData[2].RegVal[0] = (UINT8)(uTemp>>2);					//low 2 bit

	V5B_SifI2cAeBatch(3, AeData);
}

const TReg gSifPwrOnSeqRegVal_OV9640_BAYER[] =
{
	{0x00, 0x26,	1},
	{0x00, 0x27,	1},
	{0x00, 0x26,	1}
};

const TReg	gSifRegVal_OV9640_BAYER[] =
{
	{0x08, 0x01,	0},
	{0x09, 0x0c,	0},
	{0x03, 0x0a,	0},
	{0x04, 0x05,	0},
	{0x01, 0x45,	0}
};

const TReg	gIspRegVal_OV9640_BAYER[] =
{
	{0x01,	0x7d,	0},
	{0x27,	0x20,	0},
	{0x26,	0x60,	0},
	{0x06, 0x80,	0},
	{0x07, 0x02,	0},
	{0x08, 0xe0,	0},
	{0x09, 0x01,	0},

	{0x0a,	0x40,	0},
	{0x0b,	0x60,	0},
	{0x0c,	0x05,	0},
	{0x0d,	0x0f,	0},
	{0x0e,	0x0f,	0},
	{0x0f,	0x0f,	0},

	{0x22,	0x60,	0},
	{0x23,	0x40,	0},
	{0x24,	0x40,	0},

	{0x2c,	0x66,	0},
	{0x2d,	0xdb,	0},
	{0x2e,	0x06,	0},
	{0x2f,	0xf1,	0},
	{0x30,	0x5f,	0},
	{0x31,	0xf4,	0},
	{0x32,	0xf7,	0},
	{0x33,	0xeb,	0},
	{0x34,	0x5b,	0},

	{0x00,	0x01,	0},
	{0x8b,	0xff,	0},
	{0x8c,	0xff,	0},
	{0x8d,	0xff,	0},
	{0x81,	0x01,	0},
	//ae/awb window setting
	{0x112, 0x06,	0},
	{0x113, 0x06,	0},
	{0x114, 0x06,	0},
	{0x115, 0x06,	0},
	{0x116, 0x06,	0},
	{0x118, 0x0a,	0},
	{0x119, 0x0a,	0},
	{0x11a, 0x0a,	0},
	{0x11b, 0x0a,	0},
	{0x11c, 0x0a,	0}
};

/* Sensor size control configure info */
/* VGA mode(640*480) */
const TReg gSnrSizeRegValVGA_OV9640_BAYER[] = 
{
	{0x12,	0x80,	1},
	{0x04,	0x00,	1},
	{0x12,	0x45,	1},
	{0x3A,	0x04,	1},
	{0x11,	0x80,	1},
	{0x1e,	0x30,	1},
	{0x13,	0x08,	1},
	{0x3d,	0x00,	1},
	{0x10,	0x36,	1}
};

/* SXGA mode(1280*960) */
const TReg gSnrSizeRegValSXGA_OV9640_BAYER[] = 
{
	{0x12,	0x80,	1},
	{0x04,	0x00,	1},
	{0x12,	0x05,	1},
	{0x3A,	0x04,	1},
	{0x11,	0x80,	1},
	{0x1e,	0x30,	1},
	{0x2a,	0x10,	1},
	{0x2b,	0x40,	1},
	{0x13,	0x08,	1},
	{0x3d,	0x00,	1},
	{0x10,	0x18,	1}
};

const TSnrSizeCfg gSensorSizeCfg_OV9640_BAYER[] = 
{
	{
		{sizeof(gSnrSizeRegValVGA_OV9640_BAYER) / sizeof(TReg), (PTReg)&gSnrSizeRegValVGA_OV9640_BAYER},
		{0, NULL},
		{640, 480},
		30,				// maxet
		0,
		0
	},
	{
		{sizeof(gSnrSizeRegValSXGA_OV9640_BAYER) / sizeof(TReg), (PTReg)&gSnrSizeRegValSXGA_OV9640_BAYER},
		{0, NULL},
		{1280,960},
		30,				// maxet
		0,
		0
	}
};

const TSnrInfo gSensorInfo_OV9640_BAYER = 
{
	"OV 9640 bayer mode",

	0,																												//bayer mode
	0,																												//not use PCLK of sensor
	0,																												//sensor pclk value : KHz
	0,																												//[0]: VSync delay enable; [1]: Fifo VSync delay enable; [2]: Use PCLK Negtive Edge.
	0,																												//0 -- I2C, 1 -- Serial bus;
	0x01,																											//[0]: 1 -- OV I2C bus
	100,																											//I2C Rate : 100KHz
	0x60,																											//I2C address
	0x60,																											//ISP I2C address for special sensor
	0x06,																											//power config
	0x01,																											// reset sensor

	32,																												//brightness 
	32,																												//contrast
	32,																												//saturation
	{
		0x00,0x04,0x16,0x30,0x4E,0x68,0x81,0x98,0xAC,0xBE,0xCD,0xDA,0xE4,0xED,0xF5,0xfB,0xff						//gamma table
	},

	{0,NULL},																										//sensor poweroff
	{0,NULL},																										//sensor standby
	{0,NULL},																										//sensor sleep
	{0, NULL},

	{sizeof(gSifPwrOnSeqRegVal_OV9640_BAYER) / sizeof(TReg), (PTReg)&gSifPwrOnSeqRegVal_OV9640_BAYER},				//Sif config sequence(Reg.800 bit0~2) when sensor power on
	{0, NULL},																										//Sif config sequence(Reg.800 bit0~2) when sensor standby
	{sizeof(gSifRegVal_OV9640_BAYER) / sizeof(TReg), (PTReg)&gSifRegVal_OV9640_BAYER},								//sif initial value
	{sizeof(gIspRegVal_OV9640_BAYER) / sizeof(TReg), (PTReg)&gIspRegVal_OV9640_BAYER},								//isp initial value
	{sizeof(gSensorSizeCfg_OV9640_BAYER) / sizeof(TSnrSizeCfg), (PTSnrSizeCfg)&gSensorSizeCfg_OV9640_BAYER},		//size configure 

	SensorSetReg_OV9640_BAYER,																						//set reg callback
	SensorGetReg_OV9640_BAYER,																						//get reg callback
	SensorIdChk_OV9640_BAYER,																						//Sensor ID Check Call
	SensorSetMirrorFlip_OV9640_BAYER,																				//Set Sensor Mirror Flip Call
	NULL,																											//Sensor switch size callback
	SensorSetExposeTime_OV9640_BAYER,																				//set et callback

	{	//	ytarget	ythd ytop gaintarget gainthd,gaindelay,gain,minet,maxgain,mingain,speed
		0x78, 0x11, 0x9f, 0x60, 0x20, 1, 0x60, 0, 0xe0, 0x40, 0														//should be add to other sensor configure
	},
	{	//ythd of flash,et of flash
		0x20,	10
	},
	1																												//Histogram ratio number, 1 is recommend value
};

#endif

#if V5_DEF_SNR_OV_9640_YUV

void SensorSetReg_OV9640_YUV(UINT8 uAddr, UINT16 uVal)
{
	V5B_SifI2cWriteByte((UINT8)uAddr, (UINT8)uVal);
}

void SensorGetReg_OV9640_YUV(UINT8 uAddr, UINT16 *uVal)
{
	V5B_SifI2cReadByte((UINT8)uAddr, (UINT8*)uVal);
}

UINT8 SensorIdChk_OV9640_YUV(void)
{
	UINT8 val = 0;

	V5B_SifI2cReadByte(0x0a, &val);
	if(val != 0x96)
		return FAILED;
	V5B_SifI2cReadByte(0x0b, &val);
	if(val == 0x49)
		return SUCCEED;
	return FAILED;
}

/******************************************************************
Desc: set sensor mirror and flip callback 
Para: BIT0 -- flip
BIT1 -- mirror
*******************************************************************/
void SensorSetMirrorFlip_OV9640_YUV(UINT8 val)
{
	UINT8 x = 0;

	V5B_SifI2cReadByte(0x1e, &x);
	x&=0xcf;
	x |= (val&0x03)<<4;
	V5B_SifI2cWriteByte(0x1e, x);
}

const TReg gSifPwrOnSeqRegVal_OV9640_YUV[] =
{
	{0x00, 0x66,	1},
	{0x00, 0x67,	1},
	{0x00, 0x66,	1}
};

const TReg	gSifRegVal_OV9640_YUV[] =
{
	{0x02, 0xb0,	0},			
	{0x08, 0x01,	0},
	{0x09, 0x0c,	0},
	{0x03, 0x0a,	0},
	{0x04, 0x05,	0},
	{0x01, 0x45,	0}
};

/* Sensor size control configure info */
/* VGA mode(640*480) */
const TReg gSnrSizeRegValVGA_OV9640_YUV[] = 
{
	{0x04,	0x00,	1},
	{0x11,	0x80,	1},
	{0x12,	0x40,	1},
	{0x1e,	0x30,	1},
	{0x3A,	0x0c,	1}
};

/* SXGA mode(1280*960) */
const TReg gSnrSizeRegValSXGA_OV9640_YUV[] = 
{
	{0x04,	0x00,	1},
	{0x11,	0x80,	1},
	{0x12,	0x00,	1},
	{0x1e,	0x30,	1},
	{0x3A,	0x0c,	1}
};

const TSnrSizeCfg gSnrSizeCfg_OV9640_YUV[] = 
{
	{
		{sizeof(gSnrSizeRegValVGA_OV9640_YUV) / sizeof(TReg), (PTReg)&gSnrSizeRegValVGA_OV9640_YUV},							//snr initial value
		{0, NULL},
		{640, 480},
		30,				// maxet
		0,
		0
	},
	{
		{sizeof(gSnrSizeRegValSXGA_OV9640_YUV) / sizeof(TReg), (PTReg)&gSnrSizeRegValSXGA_OV9640_YUV},							//snr initial value
		{0, NULL},
		{1280,960},
		30,				// maxet
		0,
		0
	}
};

const TSnrInfo gSensorInfo_OV9640_YUV = 
{
	"OV9640 YUV",

	1,																										//yuv mode
	0,																										//not use PCLK of sensor
	0,																										//sensor pclk value : KHz
	0,																										//[0]: VSync delay enable; [1]: Fifo VSync delay enable; [2]: Use PCLK Negtive Edge.
	0,																										//0 -- I2C, 1 -- Serial bus;
	0x01,																									//[0]: 1 -- OV I2C bus
	100,																									//I2C Rate : 100KHz
	0x60,																									//I2C address
	0x60,																									//ISP I2C address for special sensor
	0x06,																									//power config
	0x01,																									// reset sensor

	32,																										//brightness 
	32,																										//contrast
	32,																										//saturation
	{
		0x00,0x13,0x38,0x59,0x79,0x92,0xa7,0xb9,0xc8,0xd4,0xdf,0xe7,0xee,0xf4,0xf9,0xfc,0xff				//gamma table
	},

	{0,NULL}, 																								//sensor poweroff
	{0,NULL}, 																								//sensor standby
	{0,NULL}, 																								//sensor sleep
	{0,NULL},

	{sizeof(gSifPwrOnSeqRegVal_OV9640_YUV) / sizeof(TReg), (PTReg)&gSifPwrOnSeqRegVal_OV9640_YUV},			//Sif config sequence(Reg.800 bit0~2) when sensor power on
	{0, NULL},																								//Sif config sequence(Reg.800 bit0~2) when sensor standby
	{sizeof(gSifRegVal_OV9640_YUV) / sizeof(TReg), (PTReg)&gSifRegVal_OV9640_YUV},							//sif initial value
	{0,NULL},
	{sizeof(gSnrSizeCfg_OV9640_YUV) / sizeof(TSnrSizeCfg), (PTSnrSizeCfg)&gSnrSizeCfg_OV9640_YUV},			//size configure 

	SensorSetReg_OV9640_YUV,																				//set reg callback
	SensorGetReg_OV9640_YUV,																				//set reg callback
	SensorIdChk_OV9640_YUV,																					//Sensor ID Check Call
	SensorSetMirrorFlip_OV9640_YUV,																			//Set Sensor Mirror Flip Call
	NULL,																									//Sensor switch size callback
	NULL,																									//set et callback

	{	//	ytarget	ythd ytop gaintarget gainthd,gaindelay,gain,minet,maxgain,mingain,speed
		0x78, 0x11, 0x9f, 0x60, 0x20, 1, 0x60, 0, 0xe0, 0x40, 0												//should be add to other sensor configure
	},
	{	//ythd of flash,et of flash
		0x20,			10
	},
	1																										//Histogram ratio number, 1 is recommend value
};

#endif

#if V5_DEF_SNR_MT9D111_BAYER

void SensorSetReg_MT9D111_BAYER(UINT8 uAddr, UINT16 uVal)
{
	V5B_SifI2cWriteWord(uAddr, uVal);
}

void SensorGetReg_MT9D111_BAYER(UINT8 uAddr, UINT16 *uVal)
{
	V5B_SifI2cReadWord((UINT8)uAddr, (UINT16*)uVal);
}

UINT8 SensorIdChk_MT9D111_BAYER(void)
{
	UINT16 val = 0;

	V5B_SifI2cWriteWord(0xf0, 0);
	V5B_SifI2cReadWord(0, &val);
	if(val == 0x1519)
		return SUCCEED;
	return FAILED;
}

/******************************************************************
Desc: set sensor mirror and flip callback 
Para: BIT0 -- mirror
BIT1 -- flip
*******************************************************************/
void SensorSetMirrorFlip_MT9D111_BAYER(UINT8 val)
{
	UINT16 x;

	V5B_SifI2cWriteWord(0xf0, 0);
	V5B_SifI2cReadWord(0x20, &x);
	x&=0xfffc;

	x |= val&0x03;
	V5B_SifI2cWriteWord(0x20, x);
}


/********************************************************************************
Description:	Write sensor register to change exposure time
Parameter: t -- exposure time clock num 
freqtype -- frequence type 50 or 60
*********************************************************************************/
void SensorSetExposeTime_MT9D111_BAYER(UINT8 freqtype, UINT32 t, TSize size, UINT32 snrclock)
{
#if 0
	UINT16 uTemp = 0;
	TI2cBatch AeData;
	switch(size.cx)
	{
	case 1600:
		uTemp = (UINT16)(t/1948);
		break;
	case 800:
		uTemp = (UINT16)(t/1000); //? width
		break;
	default:
		break;
	}
	
	if(uTemp<=1)  uTemp=1;
	
	V5B_SifI2cWriteWord(0xf0, 0);
	AeData.RegBytes  = 2;
	AeData.RegAddr   = 0x09;
	AeData.RegVal[0] = (UINT8)(uTemp>>8);
	AeData.RegVal[1] = (UINT8)(uTemp);

	V5B_SifI2cAeBatch(1, &AeData); 
#endif
	t = t;
	size = size;
	freqtype = freqtype;
	snrclock = snrclock;
}

const TReg gSifPwrOnSeqRegVal_MT9D111_BAYER[] =
{
	{0x00, 0x21,	1},
	{0x00, 0x20,	1},
	{0x00, 0x21,	1}
};

const TReg	gSifRegVal_MT9D111_BAYER[] =
{
	{0x00, 0x21,	0},
	{0x08, 0x02,	0},
	{0x09, 0x18,	0},
		
	{0x05, 0x00,	0},
	{0x06, 0x01,	0},
	
	{0x03, 0x08,	0},
	{0x14, 0x00,	0}, //Href_start
	{0x15, 0x00,	0},
	{0x16, 0x06,	0},
	{0x17, 0x3f,	0},
	
	{0x04, 0x04,	0},
	{0x20, 0x00,	0},	//Vref_start
	{0x21, 0x00,	0},
	{0x22, 0x04,	0},
	{0x23, 0xb0,	0},
	
//	{0x34, 0x02,	0},
	{0x01, 0x41,	0}
};

const TReg	gIspRegVal_MT9D111_BAYER[] =
{
	{0x00,	0x92,	0},
	{0x01,	0x7d,	0},
	{0x22,	0x60,	0},
	{0x23,	0x20,	0},

	{0x28,	0x60,	0}, //color matrix
	{0x29,	0xf9,	0},
	{0x2a,	0xe5,	0},
	{0x2b,	0xe7,	0},
	{0x2c,	0x60,	0},
	{0x2d,	0xfa,	0},
	{0x2e,	0xf3,	0},
	{0x2f,	0xee,	0},
	{0x30,	0x5e,	0},
	{0x81,	0x01,	0}, //awb
	{0x8f,	0xff,	0}, 
	{0xda,	0x00,	0}, //ae/awb window
	{0xdb,	0x14,	0},
	{0xdc,	0x14,	0},
	{0xdd,	0x14,	0},
	{0xde,	0x14,	0},
	{0xe0,	0x00,	0},
	{0xe1,	0x10,	0},
	{0xe2,	0x10,	0},
	{0xe3,	0x10,	0},
	{0xe4,	0x10,	0},
	{0x83,	0xff,	0},


};

const TReg gSnrSizeRegValUXGA_MT9D111_BAYER[] = 
{	
	{0xf0,	0x00,	1}, //sel page 0
	{0xf2,	0x0b,	1},

	{0xf0,	0x01,	1}, //sel page 1
	{0x08,	0x00,	1},
	{0x09,	0x10,	1},
	{0x37,	0x00,	1},
	{0x3b,	0x00,	1},
	{0x3c,	0x00,	1},
	{0xc6,	0xa102,	1},
	{0xc8,	0x00,	1},
	{0xc6,	0xa104,	1},
	{0xc8,	0x03,	1},
	{0xc6,	0xa122,	1},
	{0xc8,	0x00,	1},
	{0xc6,	0xa123,	1},
	{0xc8,	0x00,	1},
	{0xc6,	0xa124,	1},
	{0xc8,	0x00,	1},
	{0xc6,	0xa125,	1},
	{0xc8,	0x00,	1},
	{0xc6,	0xa126,	1},
	{0xc8,	0x00,	1},
	{0xc6,	0xa127,	1},
	{0xc8,	0x00,	1},
	{0xc6,	0xa129,	1},
	{0xc8,	0x00,	1},
	{0xc6,	0xa12a,	1},
	{0xc8,	0x00,	1},
	{0xc6,	0xa12b,	1},
	{0xc8,	0x00,	1},
	{0xc6,	0xa12c,	1},
	{0xc8,	0x00,	1},
	{0xc6,	0xa12d,	1},
	{0xc8,	0x00,	1},
	{0xc6,	0xa12e,	1},
	{0xc8,	0x00,	1},
	{0xc6,	0xa130,	1},
	{0xc8,	0x00,	1},
	{0xc6,	0xa131,	1},
	{0xc8,	0x00,	1},
	{0xc6,	0xa132,	1},
	{0xc8,	0x00,	1},
	{0xc6,	0xa133,	1},
	{0xc8,	0x00,	1},
	{0xc6,	0xa134,	1},
	{0xc8,	0x00,	1},
	{0xc6,	0xa135,	1},
	{0xc8,	0x00,	1},
	{0xc6,	0xa137,	1},
	{0xc8,	0x00,	1},
	{0xc6,	0xa138,	1},
	{0xc8,	0x00,	1},
	{0xc6,	0xa139,	1},
	{0xc8,	0x00,	1},
	{0xc6,	0xa13a,	1},
	{0xc8,	0x00,	1},
	{0xc6,	0xa13b,	1},
	{0xc8,	0x00,	1},
	{0xc6,	0xa13c,	1},
	{0xc8,	0x00,	1},
	
	{0x43,	0x00,	1},
	{0xa4,	0x00,	1},

	{0xf0,	0x00,	1}, //sel page 0
	{0x20,	0x03,	1}, //mirror col&row
	{0x03,	0x04b0,	1}, //Height
	{0x04,	0x0640,	1}, //Width
	{0x05,	0x15c,	1}, //HB
	{0x06,	0x0020,	1}, //VB
	{0x09,	0x0d7,	1}, //ae
	{0x65,	0xa000,	1}, //bypass PLL
	{0xf0,	0x00,	1}, //sel page 0

};

const TReg gSnrSizeRegValSVGA_MT9D111_BAYER[] = 
{	
	{0xf0,	0x00,	1}, //sel page 0
	{0xf2,	0x0b,	1},

	{0xf0,	0x01,	1}, //sel page 1
	{0x08,	0x00,	1},
	{0x09,	0x10,	1},
	{0x37,	0x00,	1},
	{0x3b,	0x00,	1},
	{0x3c,	0x00,	1},
	{0xc6,	0xa102,	1},
	{0xc8,	0x00,	1},
	{0xc6,	0xa104,	1},
	{0xc8,	0x03,	1},
	{0xc6,	0xa122,	1},
	{0xc8,	0x00,	1},
	{0xc6,	0xa123,	1},
	{0xc8,	0x00,	1},
	{0xc6,	0xa124,	1},
	{0xc8,	0x00,	1},
	{0xc6,	0xa125,	1},
	{0xc8,	0x00,	1},
	{0xc6,	0xa126,	1},
	{0xc8,	0x00,	1},
	{0xc6,	0xa127,	1},
	{0xc8,	0x00,	1},
	{0xc6,	0xa129,	1},
	{0xc8,	0x00,	1},
	{0xc6,	0xa12a,	1},
	{0xc8,	0x00,	1},
	{0xc6,	0xa12b,	1},
	{0xc8,	0x00,	1},
	{0xc6,	0xa12c,	1},
	{0xc8,	0x00,	1},
	{0xc6,	0xa12d,	1},
	{0xc8,	0x00,	1},
	{0xc6,	0xa12e,	1},
	{0xc8,	0x00,	1},
	{0xc6,	0xa130,	1},
	{0xc8,	0x00,	1},
	{0xc6,	0xa131,	1},
	{0xc8,	0x00,	1},
	{0xc6,	0xa132,	1},
	{0xc8,	0x00,	1},
	{0xc6,	0xa133,	1},
	{0xc8,	0x00,	1},
	{0xc6,	0xa134,	1},
	{0xc8,	0x00,	1},
	{0xc6,	0xa135,	1},
	{0xc8,	0x00,	1},
	{0xc6,	0xa137,	1},
	{0xc8,	0x00,	1},
	{0xc6,	0xa138,	1},
	{0xc8,	0x00,	1},
	{0xc6,	0xa139,	1},
	{0xc8,	0x00,	1},
	{0xc6,	0xa13a,	1},
	{0xc8,	0x00,	1},
	{0xc6,	0xa13b,	1},
	{0xc8,	0x00,	1},
	{0xc6,	0xa13c,	1},
	{0xc8,	0x00,	1},
	
	{0x43,	0x00,	1},
	{0xa4,	0x00,	1},

	{0xf0,	0x00,	1}, //sel page 0
	{0x20,	0x83,	1}, //binning & mirror col&row
	{0x03,	0x04b0,	1}, //Height
	{0x04,	0x0640,	1}, //Width
	{0x05,	0x15c,	1}, //HB
	{0x06,	0x0020,	1}, //VB
	{0x09,	0x0d7,	1}, //ae
	{0x65,	0xa000,	1}, //bypass PLL
	{0xf0,	0x00,	1}, //sel page 0

};

const TSnrSizeCfg gSnrSizeCfg_MT9D111_BAYER[] = 
{
	{
		{sizeof(gSnrSizeRegValUXGA_MT9D111_BAYER) / sizeof(TReg), (PTReg)&gSnrSizeRegValUXGA_MT9D111_BAYER},
		{0, NULL},
		{1600, 1200},
		30,				// maxet
		0,
		1
	},
	{
		{sizeof(gSnrSizeRegValSVGA_MT9D111_BAYER) / sizeof(TReg), (PTReg)&gSnrSizeRegValSVGA_MT9D111_BAYER},
		{0, NULL},
		{800, 600},
		30,				// maxet
		0,
		1
	}
};

const TSnrInfo gSensorInfo_MT9D111_BAYER = 
{
	"MT9D111 Bayer",

	0,																										//bayer mode
	0,																										//not use PCLK of sensor
	0,																										//sensor pclk value : KHz
	0,																										//[0]: VSync delay enable; [1]: Fifo VSync delay enable; [2]: Use PCLK Negtive Edge.
	0,																										//0 -- I2C, 1 -- Serial bus;
	0,																										//[0]: 1 -- OV I2C bus
	100,																									//I2C Rate : 100KHz
	0xba,																									//I2C address
	0,																										//ISP I2C address for special sensor
	0x01,																									//power config
	0x01,																									//reset sensor

	32,																										//brightness 
	32,																										//contrast
	32,																										//saturation
	{
		0x00,0x13,0x38,0x59,0x79,0x92,0xa7,0xb9,0xc8,0xd4,0xdf,0xe7,0xee,0xf4,0xf9,0xfc,0xff				//gamma table
	},

	{0,NULL},																								//sensor poweroff
	{0,NULL},																								//sensor standby
	{0,NULL},																								//sensor sleep
	{0, NULL},

	{sizeof(gSifPwrOnSeqRegVal_MT9D111_BAYER) / sizeof(TReg), (PTReg)&gSifPwrOnSeqRegVal_MT9D111_BAYER},	//Sif config sequence(Reg.800 bit0~2) when sensor power on
	{0, NULL},																								//Sif config sequence(Reg.800 bit0~2) when sensor standby
	{sizeof(gSifRegVal_MT9D111_BAYER) / sizeof(TReg), (PTReg)&gSifRegVal_MT9D111_BAYER},					//sif initial value
	{sizeof(gIspRegVal_MT9D111_BAYER) / sizeof(TReg), (PTReg)&gIspRegVal_MT9D111_BAYER},					//isp initial value
	{sizeof(gSnrSizeCfg_MT9D111_BAYER) / sizeof(TSnrSizeCfg), (PTSnrSizeCfg)&gSnrSizeCfg_MT9D111_BAYER},	//size configure 

	SensorSetReg_MT9D111_BAYER,																				//set reg callback
	SensorGetReg_MT9D111_BAYER,																				//get reg callback
	SensorIdChk_MT9D111_BAYER,																				//Sensor ID Check Call
	SensorSetMirrorFlip_MT9D111_BAYER,																		//Set Sensor Mirror Flip Call
	NULL,																									//Sensor switch size callback
	SensorSetExposeTime_MT9D111_BAYER,																		//set et callback

	{//	ytarget	ythd ytop gaintarget gainthd,gaindelay,gain,minet,maxgain,mingain,speed
		0x78, 0x11, 0x9f, 0x60, 0x20, 1, 0x60, 0, 0xe0, 0x40, 0												//should be add to other sensor configure
	},
	{	//ythd of flash,et of flash
		0x20,			10
	},
	1																										//Histogram ratio number, 1 is recommend value
};

#endif

#if V5_DEF_SNR_MT9D111_YUV

void SensorSetReg_MT9D111_YUV(UINT8 uAddr, UINT16 uVal)
{
	V5B_SifI2cWriteWord((UINT8)uAddr, (UINT16)uVal);
}

void SensorGetReg_MT9D111_YUV(UINT8 uAddr, UINT16 *uVal)
{
	V5B_SifI2cReadWord((UINT8)uAddr, (UINT16*)uVal);
}

UINT8 SensorIdChk_MT9D111_YUV(void)
{
	UINT16 val = 0;

	V5B_SifI2cWriteWord(0xf0, 0);
	V5B_SifI2cReadWord(0, &val);
	if(val == 0x1519)
		return SUCCEED;
	return FAILED;
}

/******************************************************************
Desc: set sensor mirror and flip callback 
Para: BIT0 -- mirror
BIT1 -- flip
*******************************************************************/
void SensorSetMirrorFlip_MT9D111_YUV(UINT8 val)
{
	UINT16 x;

	V5B_SifI2cWriteWord(0xf0, 0);
	V5B_SifI2cReadWord(0x20, &x);
	x&=0xfffc;

	x |= val&0x03;
	V5B_SifI2cWriteWord(0x20, x);
}

const TReg gSifPwrOnSeqRegVal_MT9D111_YUV[] =
{
	{0x00, 0x61,	1},
	{0x00, 0x60,	1},
	{0x00, 0x61,	1}
};

const TReg	gSifRegVal_MT9D111_YUV[] =
{
	{0x01, 0x41,	0},
	{0x02, 0xb0,	0},
	{0x04, 0x04,	0},
	{0x14, 0x00,	0}, //Href_start
	{0x15, 0x00,	0},
	{0x16, 0x06,	0},
	{0x17, 0x3f,	0},
	{0x03, 0x08,	0},
	{0x20, 0x00,	0},	//Vref_start
	{0x21, 0x00,	0},
	{0x22, 0x04,	0},
	{0x23, 0xb0,	0},
};

const TReg gSnrSizeRegValSVGA_MT9D111_YUV[] = 
{
	{0xf0,	0x00,	0}, //sel page 0
	{0x20,	0x0303,	0}, //mirror col&row changed 11/13
	{0xf0,	0x01,	0}, //sel page 1
	{0xc6,	0xa743,	0},
	{0xc8,	0x0044,	0},
	{0xc6,	0xa120,	0},
	{0xc8,	0x003a,	0},
	{0xc6,	0x270b,	0}, //YUV output
	{0xc8,	0x0030,	0},
	{0xc6,	0xa77e,	0},
	{0xc8,	0x0000,	0},
	{0xc6,	0x2774,	0},
	{0xc8,	0xe5e1,	0},
	{0xc6,	0xa776,	0},
	{0xc8,	0x00e3,	0},
	{0xc6,	0xa103,	0}, // do preview
	{0xc8,	0x0001,	0},
};

const TReg gSnrSizeRegValUXGA_MT9D111_YUV[] = 
{
	{0xf0,	0x01,	0},
	{0xc6,	0xa103,	0},
	{0xc8,	0x0002,	0},
	{0xc6,	0xa102,	0},
	{0xc8,	0x0001,	0},
/*
	{0xC6, 0xA745,	1},
	{0xC8, 0x0000,	1},
	{0xC6, 0xA746,	1},
	{0xC8, 0x0003,	1},
	{0xC6, 0xA747,	1},
	{0xC8, 0x000a,	1},
	{0xC6, 0xA748,	1},
	{0xC8, 0x0018,	1},
	{0xC6, 0xA749,	1},
	{0xC8, 0x002f,	1},
	{0xC6, 0xA74A,	1},
	{0xC8, 0x0043,	1},
	{0xC6, 0xA74B,	1},
	{0xC8, 0x0057,	1},
	{0xC6, 0xA74C,	1},
	{0xC8, 0x006a,	1},
	{0xC6, 0xA74D,	1},
	{0xC8, 0x007d,	1},
	{0xC6, 0xA74E,	1},
	{0xC8, 0x008f,	1},
	{0xC6, 0xA74F,	1},
	{0xC8, 0x00a0,	1},
	{0xC6, 0xA750,	1},
	{0xC8, 0x00af,	1},
	{0xC6, 0xA751,	1},
	{0xC8, 0x00bd,	1},
	{0xC6, 0xA752,	1},
	{0xC8, 0x00ca,	1},
	{0xC6, 0xA753,	1},
	{0xC8, 0x00d6,	1},
	{0xC6, 0xA754,	1},
	{0xC8, 0x00e1,	1},
	{0xC6, 0xA755,	1},
	{0xC8, 0x00ec,	1},
	{0xC6, 0xA756,	1},
	{0xC8, 0x00f6,	1},
	{0xC6, 0xA757,	1},
	{0xC8, 0x00ff,	1},
	{0xC6, 0xA744,	1},
	{0xC8, 0x0001,	1},*/
	{0xC6, 0xA103,	1},
	{0xC8, 0x0005,	1},
	
};

const TSnrSizeCfg gSnrSizeCfg_MT9D111_YUV[] = 
{	{
		{sizeof(gSnrSizeRegValSVGA_MT9D111_YUV) / sizeof(TReg), (PTReg)&gSnrSizeRegValSVGA_MT9D111_YUV},
		{0, NULL},
		{800, 600},
		30,
		0,
		1
	},
	
	{
		{sizeof(gSnrSizeRegValUXGA_MT9D111_YUV) / sizeof(TReg), (PTReg)&gSnrSizeRegValUXGA_MT9D111_YUV},
		{0, NULL},
		{1600, 1200},
		30,
		0,
		0
	}
	
};

const TSnrInfo gSensorInfo_MT9D111_YUV = 
{
	"MT9D111 YUV",

	1,																										//yuv mode
	0,																										//not use PCLK of sensor
	0,																										//sensor pclk value : KHz
	0,																										//[0]: VSync delay enable; [1]: Fifo VSync delay enable; [2]: Use PCLK Negtive Edge.
	0,																										//0 -- I2C, 1 -- Serial bus;
	0,																										//[0]: 1 -- OV I2C bus
	80,																										//I2C Rate : 100KHz
	0xba,																									//I2C address
	0,																										//ISP I2C address for special sensor
	0x01,																									//power config
	0,																										//reset sensor

	32,																										//brightness 
	32,																										//contrast
	32,																										//saturation
	{
		0x00,0x04,0x16,0x30,0x4E,0x68,0x81,0x98,0xAC,0xBE,0xCD,0xDA,0xE4,0xED,0xF5,0xfB,0xff				//gamma table
	},

	{0,NULL},																								//sensor poweroff
	{0,NULL},																								//sensor standby
	{0,NULL},																								//sensor sleep
	{0, NULL},

	{sizeof(gSifPwrOnSeqRegVal_MT9D111_YUV) / sizeof(TReg), (PTReg)&gSifPwrOnSeqRegVal_MT9D111_YUV},		//Sif config sequence(Reg.800 bit0~2) when sensor power on
	{0, NULL},																								//Sif config sequence(Reg.800 bit0~2) when sensor standby
	{sizeof(gSifRegVal_MT9D111_YUV) / sizeof(TReg), (PTReg)&gSifRegVal_MT9D111_YUV},						//sif initial value
	{0, NULL},																								//isp initial value
	{sizeof(gSnrSizeCfg_MT9D111_YUV) / sizeof(TSnrSizeCfg), (PTSnrSizeCfg)&gSnrSizeCfg_MT9D111_YUV},		//size configure 

	SensorSetReg_MT9D111_YUV,																				//set reg callback
	SensorGetReg_MT9D111_YUV,																				//get reg callback
	SensorIdChk_MT9D111_YUV,																				//Sensor ID Check Call
	NULL,																									//Set Sensor Mirror Flip Call
	NULL,																									//Sensor switch size callback
	NULL,																									//set et callback

	{//	ytarget	ythd ytop gaintarget gainthd,gaindelay,gain,minet,maxgain,mingain,speed
		0x78, 0x11, 0x9f, 0x60, 0x20, 1, 0x60, 0, 0xe0, 0x40, 0												//should be add to other sensor configure
	},
	{	//ythd of flash,et of flash
		0x20,			10
	},
	1																										//Histogram ratio number, 1 is recommend value
};

#endif

#if V5_DEF_SNR_OV2620_BAYER

void SensorSetReg_OV2620_BAYER(UINT8 uAddr, UINT16 uVal)
{
	V5B_SifI2cWriteByte((UINT8)uAddr, (UINT8)uVal);
}

void SensorGetReg_OV2620_BAYER(UINT8 uAddr, UINT16 *uVal)
{
	V5B_SifI2cReadByte((UINT8)uAddr, (UINT8*)uVal);
}

UINT8 SensorIdChk_OV2620_BAYER(void)
{
	UINT8 val = 0;

	V5B_SifI2cReadByte(0x0a, &val);
	if(val != 0x26)
		return FAILED;
	V5B_SifI2cReadByte(0x0b, &val);
	if(val == 0x20)
		return SUCCEED;
	return FAILED;
}

/******************************************************************
Desc: set sensor mirror and flip callback 
Para: BIT0 -- mirror
BIT1 -- flip
*******************************************************************/
void SensorSetMirrorFlip_OV2620_BAYER(UINT8 val)
{
UINT8 x;

V5B_SifI2cReadByte(0x42, &x);
x&=0xef;

x |= val&0x10;
V5B_SifI2cWriteByte(0x42, x);
}

/********************************************************************************
Description:	Write sensor register to change exposure time
Parameter: t -- exposure time clock num 
freqtype -- frequence type 50 or 60
*********************************************************************************/
void SensorSetExposeTime_OV2620_BAYER(UINT8 freqtype, UINT32 t, TSize size, UINT32 snrclock)
{
#if 0
	UINT16 uTemp = 0;
	UINT8  val;
	uTemp = (UINT16)(t/1948);
	
	if(uTemp<=1)  uTemp=1;
	
	V5B_SifI2cWriteByte(0x10, (uTemp>>3));
	V5B_SifI2cReadByte(0x04, &val);
	V5B_SifI2cWriteByte(0x04, ((val&f8)||(uTemp&07));

#endif
	t = t;
	size = size;
	freqtype = freqtype;
	snrclock = snrclock;
}

const TReg gSifPwrOnSeqRegVal_OV2620_BAYER[] =
{
	{0x00, 0x20,	1},
	{0x00, 0x21,	1},
	{0x00, 0x20,	1}
};

const TReg	gSifRegVal_OV2620_BAYER[] =
{
	{0x00, 0x20,	0},
	{0x08, 0x02,	0},
	{0x09, 0x18,	0},
	{0x03, 0x0a,	0},
	{0x20, 0x00,	0},
	{0x21, 0x00,	0},
	{0x22, 0x04,	0},
	{0x23, 0xb0,	0},
	{0x04, 0x04,	0},
	{0x14, 0x00,	0},
	{0x15, 0x00,	0},
	{0x16, 0x06,	0},
	{0x17, 0x3f,	0},
	{0x34, 0x01,	0},
	{0x01, 0x45,	0}
};

const TReg	gIspRegVal_OV2620_BAYER[] =
{
	{0x00,	0xa1,	0},
	{0x01,	0x7c,	0},
	{0x22,	0x60,	0},
	{0x23,	0x20,	0},

	{0x28,	0x70,	0}, //color matrix
	{0x29,	0xe8,	0},
	{0x2a,	0xe8,	0},
	{0x2b,	0xe8,	0},
	{0x2c,	0x70,	0},
	{0x2d,	0xe8,	0},
	{0x2e,	0xe8,	0},
	{0x2f,	0xe8,	0},
	{0x30,	0x70,	0},
	{0x81,	0x01,	0}, //awb
	{0x8f,	0xff,	0}, 
	{0xda,	0x00,	0}, //ae/awb window
	{0xdb,	0x14,	0},
	{0xdc,	0x14,	0},
	{0xdd,	0x14,	0},
	{0xde,	0x14,	0},
	{0xe0,	0x00,	0},
	{0xe1,	0x10,	0},
	{0xe2,	0x10,	0},
	{0xe3,	0x10,	0},
	{0xe4,	0x10,	0},

	{0x83,	0xff,	0},	
};

const TReg gSnrSizeRegValUXGA_OV2620_BAYER[] = 
{
	{0x12,	0x00,	1},
	{0x11,	0x80,	1},
	{0x13,	0x08,	1},
	{0x04,	0x05,	1},
	{0x10,	0x22,	1},
	{0x01,	0x80,	1},
	{0x02,	0x80,	1},
};

const TSnrSizeCfg gSnrSizeCfg_OV2620_BAYER[] = 
{
	{
		{sizeof(gSnrSizeRegValUXGA_OV2620_BAYER) / sizeof(TReg), (PTReg)&gSnrSizeRegValUXGA_OV2620_BAYER},
		{0, NULL},
		{1600, 1200},
		30,				// maxet
		0,
		0
	}
};

const TSnrInfo gSensorInfo_OV2620_BAYER = 
{
	"OV2620 Bayer",

	0,																										//bayer mode
	0,																										//not use PCLK of sensor
	0,																										//sensor pclk value : KHz
	0,																										//[0]: VSync delay enable; [1]: Fifo VSync delay enable; [2]: Use PCLK Negtive Edge.
	0,																										//0 -- I2C, 1 -- Serial bus;
	0x01,																									//[0]: 1 -- OV I2C bus
	100,																									//I2C Rate : 100KHz
	0x60,																									//I2C address
	0x60,																									//ISP I2C address for special sensor
	0,																										//power config
	0x01,																									//reset sensor

	32,																										//brightness 
	32,																										//contrast
	32,																										//saturation
	{
	//	0x00,0x04,0x16,0x30,0x4E,0x68,0x81,0x98,0xAC,0xBE,0xCD,0xDA,0xE4,0xED,0xF5,0xfB,0xff				//gamma table
	//	0x00,0x13,0x38,0x59,0x79,0x92,0xa7,0xb9,0xc8,0xd4,0xdf,0xe7,0xee,0xf4,0xf9,0xfc,0xff
		0x00,0x08,0x23,0x41,0x61,0x7a,0x92,0xa7,0xb9,0xc8,0xd5,0xe0,0xe9,0xf0,0xf7,0xfb,0xff
	},

	{0,NULL},																								//sensor poweroff
	{0,NULL},																								//sensor standby
	{0,NULL},																								//sensor sleep
	{0, NULL},

	{sizeof(gSifPwrOnSeqRegVal_OV2620_BAYER) / sizeof(TReg), (PTReg)&gSifPwrOnSeqRegVal_OV2620_BAYER},		//Sif config sequence(Reg.800 bit0~2) when sensor power on
	{0, NULL},																								//Sif config sequence(Reg.800 bit0~2) when sensor standby
	{sizeof(gSifRegVal_OV2620_BAYER) / sizeof(TReg), (PTReg)&gSifRegVal_OV2620_BAYER},						//sif initial value
	{sizeof(gIspRegVal_OV2620_BAYER) / sizeof(TReg), (PTReg)&gIspRegVal_OV2620_BAYER},						//isp initial value
	{sizeof(gSnrSizeCfg_OV2620_BAYER) / sizeof(TSnrSizeCfg), (PTSnrSizeCfg)&gSnrSizeCfg_OV2620_BAYER},		//size configure 

	SensorSetReg_OV2620_BAYER,																				//set reg callback
	SensorGetReg_OV2620_BAYER,																				//get reg callback
	SensorIdChk_OV2620_BAYER,																				//Sensor ID Check Call
	SensorSetMirrorFlip_OV2620_BAYER,																		//Set Sensor Mirror Flip Call
	NULL,																									//Sensor switch size callback
	SensorSetExposeTime_OV2620_BAYER,																		//set et callback

	{//	ytarget	ythd ytop gaintarget gainthd,gaindelay,gain,minet,maxgain,mingain,speed
		0x60, 0x11, 0x9f, 0x60, 0x20, 1, 0x60, 0, 0xe0, 0x40, 0												//should be add to other sensor configure
	},
	{	//ythd of flash,et of flash
		0x20,			10
	},
	1																										//Histogram ratio number, 1 is recommend value
};

#endif

#if V5_DEF_SNR_KAC3100_BAYER

void SensorSetReg_KAC3100_BAYER(UINT8 uAddr, UINT16 uVal)
{
	V5B_SifI2cWriteByte((UINT8)uAddr, (UINT8)uVal);
}

void SensorGetReg_KAC3100_BAYER(UINT8 uAddr, UINT16 *uVal)
{
	V5B_SifI2cReadByte((UINT8)uAddr, (UINT8*)uVal);
}

UINT8 SensorIdChk_KAC3100_BAYER(void)
{
	UINT8 val = 0;

	V5B_SifI2cReadByte(0xfe, &val);
	if(val == 0)
		return SUCCEED;
	return FAILED;
}

/******************************************************************
Desc: set sensor mirror and flip callback 
Para: BIT0 -- mirror
BIT1 -- flip
*******************************************************************/
void SensorSetMirrorFlip_KAC3100_BAYER(UINT8 val)
{
UINT8 x;

V5B_SifI2cReadByte(0x57, &x);
x&=0xfc;

x |= val&0x03;
V5B_SifI2cWriteByte(0x57, x);
}

void SensorSetExposeTime_KAC3100_BAYER(UINT8 freqtype, UINT32 t, TSize size, UINT32 snrclock)
{
#if 0
	UINT16 uTemp = 0;
	TI2cBatch AeData;
	
	switch(size.cx)
	{
		case 1600:
			uTemp = (UINT16)(t/2436);
			break;
		case 800:
			uTemp = (UINT16)(t/1404); 
			break;
		default:
			break;
	}
	if(uTemp<=1)  uTemp=1;
	
	AeData.RegBytes  = 2;
	AeData.RegAddr   = 0x4e;
	AeData.RegVal[0] = (UINT8)(uTemp>>8);
	AeData.RegVal[1] = (UINT8)(uTemp);

	V5B_SifI2cAeBatch(1, &AeData); 

#endif
	t = t;
	size = size;
	freqtype = freqtype;
	snrclock = snrclock;
}

const TReg gSifPwrOnSeqRegVal_KAC3100_BAYER[] =
{
	{0x00, 0x21,	1},
	{0x00, 0x20,	1},
	{0x00, 0x21,	1}
};

const TReg	gSifRegVal_KAC3100_BAYER[] =
{
	{0x00, 0x21,	0},
	{0x08, 0x02,	0},
	{0x09, 0x18,	0},
	{0x03, 0x0a,	0},
	{0x20, 0x00,	0},
	{0x21, 0x00,	0},
	{0x22, 0x06,	0},
	{0x23, 0x00,	0},
	{0x04, 0x05,	0},
	{0x14, 0x00,	0},
	{0x15, 0x00,	0},
	{0x16, 0x07,	0},
	{0x17, 0xff,	0},
	{0x34, 0x01,	0},
	{0x01, 0x45,	0}
};

const TReg	gIspRegVal_KAC3100_BAYER[] =
{
	{0x00,	0xb1,	0},
	{0x01,	0x7c,	0},
	{0x22,	0x60,	0},
	{0x23,	0x20,	0},

	{0x28,	0x60,	0}, //color matrix
	{0x29,	0xf0,	0},
	{0x2a,	0xf0,	0},
	{0x2b,	0xf0,	0},
	{0x2c,	0x60,	0},
	{0x2d,	0xf0,	0},
	{0x2e,	0xf0,	0},
	{0x2f,	0xf0,	0},
	{0x30,	0x60,	0},
	{0x81,	0x01,	0}, //awb
	{0x8f,	0x80,	0}, 
	{0xda,	0x00,	0}, //ae/awb window
	{0xdb,	0x20,	0},
	{0xdc,	0x20,	0},
	{0xdd,	0x20,	0},
	{0xde,	0x20,	0},
	{0xe0,	0x00,	0},
	{0xe1,	0x18,	0},
	{0xe2,	0x18,	0},
	{0xe3,	0x18,	0},
	{0xe4,	0x18,	0},

	{0x83,	0xff,	0},	
};

const TReg gSnrSizeRegValQXGA_KAC3100_BAYER[] = 
{
	{0x40,	0x2c,	1},
	{0x55,	0x00,	1},
	{0x56,	0x07,	1},
	{0x4e,	0x01,	1},
	{0x4f,	0x10,	1},
	{0x35,	0x00,	1},
	
	//{0x50,	0x06,	1}, //VFH
	//{0x51,	0x12,	1},
	//{0x52,	0x09,	1}, //VFW
	//{0x53,	0x84,	1},
};

const TReg gSnrSizeRegValXGA_KAC3100_BAYER[] = 
{
	{0x40,	0x2c,	1},
	{0x55,	0x00,	1},
	{0x56,	0x07,	1},
	{0x28,	0x00,	1},
	{0x29,	0x00,	1},
	{0x41,	0x21,	1},
	{0x43,	0x04,	1},
	{0x4e,	0x01,	1},
	{0x4f,	0x10,	1},
	{0x35,	0x00,	1},

	//{0x50,	0x03,	1}, //VFH
	//{0x51,	0x12,	1},
	//{0x52,	0x05,	1}, //VFW
	//{0x53,	0x7c,	1},
};

const TSnrSizeCfg gSnrSizeCfg_KAC3100_BAYER[] = 
{
	/*{
		
		{sizeof(gSnrSizeRegValXGA_KAC3100_BAYER) / sizeof(TReg), (PTReg)&gSnrSizeRegValXGA_KAC3100_BAYER},
		{0, NULL},
		{1024, 768},
		0,
		0
	},*/
	{
		{sizeof(gSnrSizeRegValQXGA_KAC3100_BAYER) / sizeof(TReg), (PTReg)&gSnrSizeRegValQXGA_KAC3100_BAYER},
		{0, NULL},
		{2048, 1536},
		30,				// maxet
		0,
		0
	},
	{
		
		{sizeof(gSnrSizeRegValXGA_KAC3100_BAYER) / sizeof(TReg), (PTReg)&gSnrSizeRegValXGA_KAC3100_BAYER},
		{0, NULL},
		{1024, 768},
		30,				// maxet
		0,
		0
	}
};

const TSnrInfo gSensorInfo_KAC3100_BAYER = 
{
	"KAC3100 Bayer",

	0,																										//bayer mode
	0,																										//not use PCLK of sensor
	0,																										//sensor pclk value : KHz
	0,																										//[0]: VSync delay enable; [1]: Fifo VSync delay enable; [2]: Use PCLK Negtive Edge.
	0,																										//0 -- I2C, 1 -- Serial bus;
	0,																										//[0]: 1 -- OV I2C bus
	100,																									//I2C Rate : 100KHz
	0x66,																									//I2C address
	0,																										//ISP I2C address for special sensor
	0x01,																									//power config
	0x01,																									//reset sensor

	32,																										//brightness 
	32,																										//contrast
	40,																										//saturation
	{
	//	0x00,0x04,0x16,0x30,0x4E,0x68,0x81,0x98,0xAC,0xBE,0xCD,0xDA,0xE4,0xED,0xF5,0xfB,0xff				//gamma table
		0x00,0x13,0x38,0x59,0x79,0x92,0xa7,0xb9,0xc8,0xd4,0xdf,0xe7,0xee,0xf4,0xf9,0xfc,0xff
	//	0x00,0x08,0x23,0x41,0x61,0x7a,0x92,0xa7,0xb9,0xc8,0xd5,0xe0,0xe9,0xf0,0xf7,0xfb,0xff


	},

	{0,NULL},																								//sensor poweroff
	{0,NULL},																								//sensor standby
	{0,NULL},																								//sensor sleep
	{0, NULL},

	{sizeof(gSifPwrOnSeqRegVal_KAC3100_BAYER) / sizeof(TReg), (PTReg)&gSifPwrOnSeqRegVal_KAC3100_BAYER},	//Sif config sequence(Reg.800 bit0~2) when sensor power on
	{0, NULL},																								//Sif config sequence(Reg.800 bit0~2) when sensor standby
	{sizeof(gSifRegVal_KAC3100_BAYER) / sizeof(TReg), (PTReg)&gSifRegVal_KAC3100_BAYER},					//sif initial value
	{sizeof(gIspRegVal_KAC3100_BAYER) / sizeof(TReg), (PTReg)&gIspRegVal_KAC3100_BAYER},					//isp initial value
	{sizeof(gSnrSizeCfg_KAC3100_BAYER) / sizeof(TSnrSizeCfg), (PTSnrSizeCfg)&gSnrSizeCfg_KAC3100_BAYER},	//size configure 

	SensorSetReg_KAC3100_BAYER,																				//get reg callback
	SensorGetReg_KAC3100_BAYER,																				//set reg callback
	SensorIdChk_KAC3100_BAYER,																				//Sensor ID Check Call
	SensorSetMirrorFlip_KAC3100_BAYER,																		//Set Sensor Mirror Flip Call
	NULL,																									//Sensor switch size callback
	SensorSetExposeTime_KAC3100_BAYER,																		//set et callback

	{//	ytarget	ythd ytop gaintarget gainthd,gaindelay,gain,minet,maxgain,mingain,speed
		0x80, 0x11, 0x9f, 0x60, 0x20, 1, 0x60, 0, 0xe0, 0x40, 0												//should be add to other sensor configure
	},
	{	//ythd of flash,et of flash
		0x20,			10
	},
	1																										//Histogram ratio number, 1 is recommend value
};

#endif

#if V5_DEF_SNR_S5K3BAFX_YUV

void SensorSetReg_S5K3BAFX_YUV(UINT8 uAddr, UINT16 uVal)
{
	V5B_SifI2cWriteByte((UINT8)uAddr, (UINT8)uVal);
}

void SensorGetReg_S5K3BAFX_YUV(UINT8 uAddr, UINT16 *uVal)
{
	V5B_SifI2cReadByte((UINT8)uAddr, (UINT8*)uVal);
}

UINT8 SensorIdChk_S5K3BAFX_YUV(void)
{
	UINT8 val = 0;

	V5B_SifI2cWriteByte(0xec, 0);
	V5B_SifI2cReadByte(0xb0, &val);
	if(val == 0xab)
		return SUCCEED;
	return FAILED;
}

const TReg gSifPwrOnSeqRegVal_S5K3BAFX_YUV[] =
{
	{0x00, 0x65,	1},
	{0x00, 0x64,	1},
	{0x00, 0x65,	1}
};

const TReg	gSifRegVal_S5K3BAFX_YUV[] =
{
	{0x00, 0x65,	0},
	{0x08, 0x09,	0},
	{0x09, 0x60,	0},
	{0x02, 0xb3,	0},
	{0x03, 0x0a,	0},
	{0x20, 0x00,	0},
	{0x21, 0x00,	0},
	{0x22, 0x04,	0},
	{0x23, 0xb0,	0},
	{0x04, 0x04,	0},
	{0x14, 0x00,	0},
	{0x15, 0x00,	0},
	{0x16, 0x06,	0},
	{0x17, 0x3f,	0},
	{0x34, 0x01,	0},
	{0x01, 0x41,	0}
};

const TReg gSnrSizeRegValUXGA_S5K3BAFX_YUV[] = 
{
	{0xec,	0x00,	80},
	{0x03,	0x00,	80},
	{0x02,	0x00,	80},
	{0x71,	0x00,	80},
	{0x72,	0x1e,	80},
	{0xec,	0x01,	80},
	{0x7f,	0x06,	80}
};

const TSnrSizeCfg gSnrSizeCfg_S5K3BAFX_YUV[] = 
{
	{
		{sizeof(gSnrSizeRegValUXGA_S5K3BAFX_YUV) / sizeof(TReg), (PTReg)&gSnrSizeRegValUXGA_S5K3BAFX_YUV},
		{0, NULL},
		{1600, 1200},
		30,				// maxet
		0,
		0
	}
};

const TSnrInfo gSensorInfo_S5K3BAFX_YUV = 
{
	"S5K3BAFX YUV",

	1,																										//bayer mode
	0,																										//not use PCLK of sensor
	0,																										//sensor pclk value : KHz
	0,																										//[0]: VSync delay enable; [1]: Fifo VSync delay enable; [2]: Use PCLK Negtive Edge.
	0,																										//0 -- I2C, 1 -- Serial bus;
	0,																										//[0]: 1 -- OV I2C bus
	5,																										//I2C Rate : 100KHz
	0x5a,																									//I2C address
	0x5a,																									//ISP I2C address for special sensor
	0x05,																									//power config
	0x01,																									//reset sensor

	32,																										//brightness 
	32,																										//contrast
	32,																										//saturation
	{
		0x00,0x04,0x16,0x30,0x4E,0x68,0x81,0x98,0xAC,0xBE,0xCD,0xDA,0xE4,0xED,0xF5,0xfB,0xff				//gamma table
	},

	{0,NULL},																								//sensor poweroff
	{0,NULL},																								//sensor standby
	{0,NULL},																								//sensor sleep
	{0, NULL},

	{sizeof(gSifPwrOnSeqRegVal_S5K3BAFX_YUV) / sizeof(TReg), (PTReg)&gSifPwrOnSeqRegVal_S5K3BAFX_YUV},		//Sif config sequence(Reg.800 bit0~2) when sensor power on
	{0, NULL},																								//Sif config sequence(Reg.800 bit0~2) when sensor standby
	{sizeof(gSifRegVal_S5K3BAFX_YUV) / sizeof(TReg), (PTReg)&gSifRegVal_S5K3BAFX_YUV},						//sif initial value
	{0, NULL},																								//isp initial value
	{sizeof(gSnrSizeCfg_S5K3BAFX_YUV) / sizeof(TSnrSizeCfg), (PTSnrSizeCfg)&gSnrSizeCfg_S5K3BAFX_YUV},		//size configure 

	SensorSetReg_S5K3BAFX_YUV,																				//set reg callback
	SensorGetReg_S5K3BAFX_YUV,																				//get reg callback
	SensorIdChk_S5K3BAFX_YUV,																				//Sensor ID Check Call
	NULL,																									//Set Sensor Mirror Flip Call
	NULL,																									//Sensor switch size callback
	NULL,																									//set et callback

	{//	ytarget	ythd ytop gaintarget gainthd,gaindelay,gain,minet,maxgain,mingain,speed
		0x78, 0x11, 0x9f, 0x60, 0x20, 1, 0x60, 0, 0xe0, 0x40, 0												//should be add to other sensor configure
	},
	{	//ythd of flash,et of flash
		0x20,			10
	},
	1																										//Histogram ratio number, 1 is recommend value
};

#endif

#if V5_DEF_SNR_S5K3BAFX_CCIR656

void SensorSetReg_S5K3BAFX_C656(UINT8 uAddr, UINT16 uVal)
{
	V5B_SifI2cWriteByte((UINT8)uAddr, (UINT8)uVal);
}

void SensorGetReg_S5K3BAFX_C656(UINT8 uAddr, UINT16 *uVal)
{
	V5B_SifI2cReadByte((UINT8)uAddr, (UINT8*)uVal);
}

UINT8 SensorIdChk_S5K3BAFX_C656(void)
{
	UINT8 val = 0;

	V5B_SifI2cWriteByte(0xec, 0);
	V5B_SifI2cReadByte(0xb0, &val);
	if(val == 0xab)
		return SUCCEED;
	return FAILED;
}

const TReg gSifPwrOnSeqRegVal_S5K3BAFX_C656[] =
{
	{0x00, 0xe5,	1},
	{0x00, 0xe4,	1},
	{0x00, 0xe5,	1}
};

const TReg	gSifRegVal_S5K3BAFX_C656[] =
{
	{0x00, 0xe5,	0},
	{0x08, 0x09,	0},
	{0x09, 0x60,	0},
	{0x02, 0xb3,	0},
	{0x03, 0x0a,	0},
	{0x20, 0x00,	0},
	{0x21, 0x00,	0},
	{0x22, 0x04,	0},
	{0x23, 0xb0,	0},
	{0x04, 0x14,	0},
	{0x14, 0x00,	0},
	{0x15, 0x00,	0},
	{0x16, 0x06,	0},
	{0x17, 0x3f,	0},
	{0x34, 0x01,	0},
	{0x01, 0x41,	0}
};

const TReg gSnrSizeRegValUXGA_S5K3BAFX_C656[] = 
{
	{0xec,	0x00,	80},
	{0x03,  0x00,	80},
	{0x02,	0x00,	80},
	{0x71,	0x00,	80},
	{0x72,	0x1e,	80},
	{0xec,	0x01,	80},
	{0x7f,	0x02,	80}
};

const TSnrSizeCfg gSnrSizeCfg_S5K3BAFX_C656[] = 
{
	{
		{sizeof(gSnrSizeRegValUXGA_S5K3BAFX_C656) / sizeof(TReg), (PTReg)&gSnrSizeRegValUXGA_S5K3BAFX_C656},
		{0, NULL},
		{1600, 1200},
		30,				// maxet
		0,
		0
	}
};

const TSnrInfo gSensorInfo_S5K3BAFX_C656 = 
{
	"S5K3BAFX YUV",

	1,																										//bayer mode
	0,																										//use PCLK of sensor
	0,																										//sensor pclk value : KHz
	0x04,																									//[0]: VSync delay enable; [1]: Fifo VSync delay enable; [2]: Use PCLK Negtive Edge.
	0,																										//0 -- I2C, 1 -- Serial bus;
	0,																										//[0]: 1 -- OV I2C bus
	5,																										//I2C Rate : 100KHz
	0x5a,																									//I2C address
	0x5a,																									//ISP I2C address for special sensor
	0x05,																									//power config
	0x01,																									//reset sensor

	32,																										//brightness 
	32,																										//contrast
	32,																										//saturation
	{
		0x00,0x04,0x16,0x30,0x4E,0x68,0x81,0x98,0xAC,0xBE,0xCD,0xDA,0xE4,0xED,0xF5,0xfB,0xff				//gamma table
	},

	{0,NULL},																								//sensor poweroff
	{0,NULL},																								//sensor standby
	{0,NULL},																								//sensor sleep
	{0, NULL},

	{sizeof(gSifPwrOnSeqRegVal_S5K3BAFX_C656) / sizeof(TReg), (PTReg)&gSifPwrOnSeqRegVal_S5K3BAFX_C656},	//Sif config sequence(Reg.800 bit0~2) when sensor power on
	{0, NULL},																								//Sif config sequence(Reg.800 bit0~2) when sensor standby
	{sizeof(gSifRegVal_S5K3BAFX_C656) / sizeof(TReg), (PTReg)&gSifRegVal_S5K3BAFX_C656},					//sif initial value
	{0, NULL},																								//isp initial value
	{sizeof(gSnrSizeCfg_S5K3BAFX_C656) / sizeof(TSnrSizeCfg), (PTSnrSizeCfg)&gSnrSizeCfg_S5K3BAFX_C656},	//size configure 

	SensorSetReg_S5K3BAFX_C656,																				//set reg callback
	SensorGetReg_S5K3BAFX_C656,																				//get reg callback
	SensorIdChk_S5K3BAFX_C656,																				//Sensor ID Check Call
	NULL,																									//Set Sensor Mirror Flip Call
	NULL,																									//Sensor switch size callback
	NULL,																									//set et callback

	{//	ytarget	ythd ytop gaintarget gainthd,gaindelay,gain,minet,maxgain,mingain,speed
		0x78, 0x11, 0x9f, 0x60, 0x20, 1, 0x60, 0, 0xe0, 0x40, 0												//should be add to other sensor configure
	},
	{	//ythd of flash,et of flash
		0x20,			10
	},
	1																										//Histogram ratio number, 1 is recommend value
};

#endif

#if V5_DEF_SNR_PO1200N_BAYER

void SensorSetReg_PO1200N_BAYER(UINT8 uAddr, UINT16 uVal)
{
	V5B_SifI2cWriteByte((UINT8)uAddr, (UINT8)uVal);
}

void SensorGetReg_PO1200N_BAYER(UINT8 uAddr, UINT16 *uVal)
{
	V5B_SifI2cReadByte((UINT8)uAddr, (UINT8*)uVal);
}

UINT8 SensorIdChk_PO1200N_BAYER(void)
{
	UINT8 val = 0;

	V5B_SifI2cReadByte(0, &val);
	if(val != 0x12)
		return FAILED;
	V5B_SifI2cReadByte(0x01, &val);
	if(val == 0)
		return SUCCEED;
	return FAILED;
}

const TReg gSifPwrOnSeqRegVal_PO1200N_BAYER[] =
{
	{0x00, 0x21,	1},
	{0x00, 0x20,	1},
	{0x00, 0x21,	1}
};

const TReg	gSifRegVal_PO1200N_BAYER[] =
{
	{0x00, 0x21,	0},
	{0x5c, 0x01,	0},
	{0x2c, 0x06,	0},
	{0x2d, 0xbb,	0},
	{0x2e, 0x04,	0},
	{0x2f, 0x00,	0},
	{0x40, 0x00,	0},
	{0x41, 0x34,	0},
	{0x42, 0x04,	0},
	{0x43, 0x00,	0},
	{0x03, 0x18,	0},
	{0x20, 0x00,	0},
	{0x21, 0x00,	0},
	{0x22, 0x04,	0},
	{0x23, 0xb0,	0},
	{0x04, 0x15,	0},
	{0x14, 0x00,	0},
	{0x15, 0x00,	0},
	{0x16, 0x06,	0},
	{0x17, 0x3f,	0},
	{0x34, 0x01,	0},
	{0x01, 0x41,	0}
};

const TReg gSnrSizeRegValUXGA_PO1200N_BAYER[] = 
{
	{0x03, 0x00,    0},
	{0x12, 0x05,    0},
	{0x13, 0x02,    0},
	{0x1e, 0x06,    0},
	{0x21, 0x00,    0},
	{0x25, 0x02,    0},
	{0x3c, 0x4f,    0},
	{0x3f, 0xe0,    0},
	{0x42, 0xff,    0},
	{0x45, 0x34,    0},
	{0x55, 0xfe,    0},
	{0x58, 0x77,    0},
	{0x59, 0xd3,    0},
	{0x5e, 0x04,    0},
	{0x61, 0x00,    0},
	{0x62, 0x04,    0},
	{0xa7, 0x31,    0},
	{0xa9, 0x44,    0},
	{0xb0, 0x00,    0},
	{0xb1, 0x00,    0},
	{0xb3, 0x11,    0},
	{0xb6, 0x26,    0},
	{0xb7, 0x20,    0},
	{0xba, 0x04,    0},
	{0x88, 0x42,    0},
	{0x89, 0x9a,    0},
	{0x8a, 0x88,    0},
	{0x8b, 0x8e,    0},
	{0x8c, 0x3e,    0},
	{0x8d, 0x90,    0},
	{0x8e, 0x87,    0},
	{0x8f, 0x96,    0},
	{0x90, 0x3d,    0},
	{0x64, 0x00,    0},
	{0x65, 0x10,    0},
	{0x66, 0x20,    0},
	{0x67, 0x2b,    0},
	{0x68, 0x36,    0},
	{0x69, 0x49,    0},
	{0x6a, 0x5a,    0},
	{0x6b, 0x7f,    0},
	{0x6c, 0x9b,    0},
	{0x6d, 0xba,    0},
	{0x6e, 0xd4,    0},
	{0x6f, 0xea,    0},
	{0x70, 0x00,    0},
	{0x71, 0x10,    0},
	{0x72, 0x20,    0},
	{0x73, 0x2b,    0},
	{0x74, 0x36,    0},
	{0x75, 0x49,    0},
	{0x76, 0x5a,    0},
	{0x77, 0x7f,    0},
	{0x78, 0x9b,    0},
	{0x79, 0xba,    0},
	{0x7a, 0xd4,    0},
	{0x7b, 0xea,    0},
	{0x7c, 0x00,    0},
	{0x7d, 0x10,    0},
	{0x7e, 0x20,    0},
	{0x7f, 0x2b,    0},
	{0x80, 0x36,    0},
	{0x81, 0x49,    0},
	{0x82, 0x5a,    0},
	{0x83, 0x7f,    0},
	{0x84, 0x9b,    0},
	{0x85, 0xba,    0},
	{0x86, 0xd4,    0},
	{0x87, 0xea,    0},

	{0x56, 0x04,    0},
	{0x57, 0x2a,    0},

	{0x03, 0x01,    0},
	{0x04, 0x10,    0},
	{0x05, 0x10,    0},
	{0x06, 0x10,    0},
	{0x07, 0x10,    0},
	{0x08, 0x13,    0},
	{0x0a, 0x00,    0},
	{0x0b, 0x10,    0},
	{0x0c, 0x20,    0},
	{0x0d, 0x18,    0},
	{0x22, 0x01,    0},
	{0x23, 0x60,    0},
	{0x25, 0x08,    0},
	{0x26, 0x82,    0},
	{0x2e, 0x0f,    0},
	{0x2f, 0x1e,    0},
	{0x30, 0x2d,    0},
	{0x31, 0x3c,    0},
	{0x32, 0x4b,    0},
	{0x33, 0x5a,    0},
	{0x34, 0x69,    0},
	{0x35, 0x78,    0},
	{0x36, 0x87,    0},
	{0x37, 0x96,    0},
	{0x38, 0xa5,    0},
	{0x39, 0xb4,    0},
	{0x3a, 0xc3,    0},
	{0x3b, 0xd2,    0},
	{0x3c, 0xe1,    0},
	{0x3e, 0xff,    0},
	{0x3f, 0xff,    0},
	{0x40, 0xff,    0},
	{0x41, 0xff,    0},
	{0x42, 0xff,    0},
	{0x43, 0xff,    0},
	{0x03, 0x00,    0}
};

const TSnrSizeCfg gSnrSizeCfg_PO1200N_BAYER[] = 
{
	{
		{sizeof(gSnrSizeRegValUXGA_PO1200N_BAYER) / sizeof(TReg), (PTReg)&gSnrSizeRegValUXGA_PO1200N_BAYER},
		{0, NULL},
		{1600, 1200},
		30,				// maxet
		0,
		1
	}
};

const TSnrInfo gSensorInfo_PO1200N_BAYER = 
{
	"PO1200N BAYER",

	0,																										//bayer mode
	0,																										//not use PCLK of sensor
	0,																										//sensor pclk value : KHz
	0,																										//[0]: VSync delay enable; [1]: Fifo VSync delay enable; [2]: Use PCLK Negtive Edge.
	0,																										//0 -- I2C, 1 -- Serial bus;
	0,																										//[0]: 1 -- OV I2C bus
	100,																									//I2C Rate : 100KHz
	0xb8,																									//I2C address
	0xb8,																									//ISP I2C address for special sensor
	0x01,																									//power config
	0x01,																									//reset sensor

	32,																										//brightness 
	32,																										//contrast
	32,																										//saturation
	{
		0x00,0x04,0x16,0x30,0x4E,0x68,0x81,0x98,0xAC,0xBE,0xCD,0xDA,0xE4,0xED,0xF5,0xfB,0xff				//gamma table
	},

	{0,NULL},																								//sensor poweroff
	{0,NULL},																								//sensor standby
	{0,NULL},																								//sensor sleep
	{0, NULL},

	{sizeof(gSifPwrOnSeqRegVal_PO1200N_BAYER) / sizeof(TReg), (PTReg)&gSifPwrOnSeqRegVal_PO1200N_BAYER},	//Sif config sequence(Reg.800 bit0~2) when sensor power on
	{0, NULL},																								//Sif config sequence(Reg.800 bit0~2) when sensor standby
	{sizeof(gSifRegVal_PO1200N_BAYER) / sizeof(TReg), (PTReg)&gSifRegVal_PO1200N_BAYER},					//sif initial value
	{0, NULL},																								//isp initial value
	{sizeof(gSnrSizeCfg_PO1200N_BAYER) / sizeof(TSnrSizeCfg), (PTSnrSizeCfg)&gSnrSizeCfg_PO1200N_BAYER},	//size configure 

	SensorSetReg_PO1200N_BAYER,																				//set reg callback
	SensorGetReg_PO1200N_BAYER,																				//get reg callback
	SensorIdChk_PO1200N_BAYER,																				//Sensor ID Check Call
	NULL,																									//Set Sensor Mirror Flip Call
	NULL,																									//Sensor switch size callback
	NULL,																									//set et callback

	{//	ytarget	ythd ytop gaintarget gainthd,gaindelay,gain,minet,maxgain,mingain,speed
		0x78, 0x11, 0x9f, 0x60, 0x20, 1, 0x60, 0, 0xe0, 0x40, 0												//should be add to other sensor configure
	},
	{	//ythd of flash,et of flash
		0x20,			10
	},
	1																										//Histogram ratio number, 1 is recommend value
};

#endif

#if V5_DEF_SNR_PO1200N_YUV

void SensorSetReg_PO1200N_YUV(UINT8 uAddr, UINT16 uVal)
{
	V5B_SifI2cWriteByte((UINT8)uAddr, (UINT8)uVal);
}

void SensorGetReg_PO1200N_YUV(UINT8 uAddr, UINT16 *uVal)
{
	V5B_SifI2cReadByte((UINT8)uAddr, (UINT8*)uVal);
}

UINT8 SensorIdChk_PO1200N_YUV(void)
{
	UINT8 val = 0;

	V5B_SifI2cReadByte(0, &val);
	if(val != 0x12)
		return FAILED;
	V5B_SifI2cReadByte(0x01, &val);
	if(val == 0)
		return SUCCEED;
	return FAILED;
}

const TReg gSifPwrOnSeqRegVal_PO1200N_YUV[] =
{
	{0x00, 0x61,	1},
	{0x00, 0x60,	1},
	{0x00, 0x61,	1}
};

const TReg	gSifRegVal_PO1200N_YUV[] =
{
	{0x00, 0x61,	0},
	{0x5c, 0x01,	0},
	{0x02, 0xb0,	0},
	{0x2c, 0x06,	0},
	{0x2d, 0xbb,	0},
	{0x2e, 0x04,	0},
	{0x2f, 0x00,	0},
	{0x40, 0x00,	0},
	{0x41, 0x34,	0},
	{0x42, 0x04,	0},
	{0x43, 0x00,	0},
	{0x03, 0x18,	0},
	{0x20, 0x00,	0},
	{0x21, 0x00,	0},
	{0x22, 0x04,	0},
	{0x23, 0xb0,	0},
	{0x04, 0x15,	0},
	{0x14, 0x00,	0},
	{0x15, 0x00,	0},
	{0x16, 0x06,	0},
	{0x17, 0x3f,	0},
	{0x34, 0x01,	0},
	{0x01, 0x41,	0}
};

const TReg gSnrSizeRegValUXGA_PO1200N_YUV[] = 
{
	{0x03,	0x00,	0},
	{0x12,	0x05,	0},
	{0x13,	0x02,	0},
	{0x1e,  0x06,   0},  
	{0x21,  0x00,   0},  
	{0x25,  0x02,   0},  
	{0x3c,  0x4f,   0},  
	{0x3f,  0xe0,   0},  
	{0x42,  0xff,   0},  
	{0x45,  0x34,   0},  
	{0x55,  0xfe,   0},  
	{0x59,  0xd3,   0},  
	{0x5e,  0x04,   0},  
	{0x61,  0x6f,   0},  
	{0x62,  0x04,   0},  
	{0xa7,  0x31,   0},  
	{0xa9,  0x44,   0},  
	{0xb0,  0x00,   0},  
	{0xb1,  0x00,   0},  
	{0xb3,  0x11,   0},  
	{0xb6,  0x26,   0},  
	{0xb7,  0x20,   0},  
	{0xba,  0x04,   0},  
	{0x88,  0x42,   0},  
	{0x89,  0x9a,   0},  
	{0x8a,  0x88,   0},  
	{0x8b,  0x8e,   0},  
	{0x8c,  0x3e,   0},  
	{0x8d,  0x90,   0},  
	{0x8e,  0x87,   0},  
	{0x8f,  0x96,   0},  
	{0x90,  0x3d,   0},  
	{0x64,  0x00,   0},  
	{0x65,  0x10,   0},  
	{0x66,  0x20,   0},  
	{0x67,  0x2b,   0},  
	{0x68,  0x36,   0},  
	{0x69,  0x49,   0},  
	{0x6a,  0x5a,   0},  
	{0x6b,  0x7f,   0},  
	{0x6c,  0x9b,   0},  
	{0x6d,  0xba,   0},  
	{0x6e,  0xd4,   0},  
	{0x6f,  0xea,   0},  
	{0x70,  0x00,   0},  
	{0x71,  0x10,   0},  
	{0x72,  0x20,   0},  
	{0x73,  0x2b,   0},  
	{0x74,  0x36,   0},  
	{0x75,  0x49,   0},  
	{0x76,  0x5a,   0},  
	{0x77,  0x7f,   0},  
	{0x78,  0x9b,   0},  
	{0x79,  0xba,   0},  
	{0x7a,  0xd4,   0},  
	{0x7b,  0xea,   0},  
	{0x7c,  0x00,   0},  
	{0x7d,  0x10,   0},  
	{0x7e,  0x20,   0},  
	{0x7f,  0x2b,   0},  
	{0x80,  0x36,   0},  
	{0x81,  0x49,   0},  
	{0x82,  0x5a,   0},  
	{0x83,  0x7f,   0},  
	{0x84,  0x9b,   0},  
	{0x85,  0xba,   0},  
	{0x86,  0xd4,   0},  
	{0x87,  0xea,   0},  

	{0x57,  0x2a,   0},  

	{0x03,  0x01,   0},  
	{0x04,  0x10,   0},  
	{0x05,  0x10,   0},  
	{0x06,  0x10,   0},  
	{0x07,  0x10,   0},  
	{0x08,  0x13,   0},  
	{0x0a,  0x00,   0},  
	{0x0b,  0x10,   0},  
	{0x0c,  0x20,   0},  
	{0x0d,  0x18,   0},  
	{0x22,  0x01,   0},  
	{0x23,  0x60,   0},  
	{0x25,  0x08,   0},  
	{0x26,  0x82,   0},  
	{0x2e,  0x0f,   0},  
	{0x2f,  0x1e,   0},  
	{0x30,  0x2d,   0},  
	{0x31,  0x3c,   0},  
	{0x32,  0x4b,   0},  
	{0x33,  0x5a,   0},  
	{0x34,  0x69,   0},  
	{0x35,  0x78,   0},  
	{0x36,  0x87,   0},  
	{0x37,  0x96,   0},  
	{0x38,  0xa5,   0},  
	{0x39,  0xb4,   0},  
	{0x3a,  0xc3,   0},  
	{0x3b,  0xd2,   0},  
	{0x3c,  0xe1,   0},  
	{0x3e,  0xff,   0},  
	{0x3f,  0xff,   0},  
	{0x40,  0xff,   0},  
	{0x41,  0xff,   0},  
	{0x42,  0xff,   0},  
	{0x43,  0xff,   0},  
	{0x03,  0x00,   0}
};

const TSnrSizeCfg gSnrSizeCfg_PO1200N_YUV[] = 
{
	{
		{sizeof(gSnrSizeRegValUXGA_PO1200N_YUV) / sizeof(TReg), (PTReg)&gSnrSizeRegValUXGA_PO1200N_YUV},
		{0, NULL},
		{1600, 1200},
		30,				// maxet
		0,
		0
	}
};

const TSnrInfo gSensorInfo_PO1200N_YUV = 
{
	"PO1200N YUV",

	1,																										//bayer mode
	0,																										//not use PCLK of sensor
	0,																										//sensor pclk value : KHz
	0,																										//[0]: VSync delay enable; [1]: Fifo VSync delay enable; [2]: Use PCLK Negtive Edge.
	0,																										//0 -- I2C, 1 -- Serial bus;
	0,																										//[0]: 1 -- OV I2C bus
	100,																									//I2C Rate : 100KHz
	0xb8,																									//I2C address
	0xb8,																									//ISP I2C address for special sensor
	0x01,																									//power config
	0x01,																									//reset sensor

	32,																										//brightness 
	32,																										//contrast
	32,																										//saturation
	{
		0x00,0x04,0x16,0x30,0x4E,0x68,0x81,0x98,0xAC,0xBE,0xCD,0xDA,0xE4,0xED,0xF5,0xfB,0xff				//gamma table
	},

	{0,NULL},																								//sensor poweroff
	{0,NULL},																								//sensor standby
	{0,NULL},																								//sensor sleep
	{0, NULL},

	{sizeof(gSifPwrOnSeqRegVal_PO1200N_YUV) / sizeof(TReg), (PTReg)&gSifPwrOnSeqRegVal_PO1200N_YUV},		//Sif config sequence(Reg.800 bit0~2) when sensor power on
	{0, NULL},																								//Sif config sequence(Reg.800 bit0~2) when sensor standby
	{sizeof(gSifRegVal_PO1200N_YUV) / sizeof(TReg), (PTReg)&gSifRegVal_PO1200N_YUV},						//sif initial value
	{0, NULL},																								//isp initial value
	{sizeof(gSnrSizeCfg_PO1200N_YUV) / sizeof(TSnrSizeCfg), (PTSnrSizeCfg)&gSnrSizeCfg_PO1200N_YUV},		//size configure 

	SensorSetReg_PO1200N_YUV,																				//set reg callback
	SensorGetReg_PO1200N_YUV,																				//get reg callback
	SensorIdChk_PO1200N_YUV,																				//Sensor ID Check Call
	NULL,																									//Set Sensor Mirror Flip Call
	NULL,																									//Sensor switch size callback
	NULL,																									//set et callback

	{//	ytarget	ythd ytop gaintarget gainthd,gaindelay,gain,minet,maxgain,mingain,speed
		0x78, 0x11, 0x9f, 0x60, 0x20, 1, 0x60, 0, 0xe0, 0x40, 0												//should be add to other sensor configure
	},
	{	//ythd of flash,et of flash
		0x20,			10
	},
	1																										//Histogram ratio number, 1 is recommend value
};

#endif

const PTSnrInfo gPSensorInfo[] =
{
	// 1.3M
#if	V5_DEF_SNR_OV_9640_BAYER
	(const PTSnrInfo)&gSensorInfo_OV9640_BAYER,			//V5_DEF_SNR_OV_9640_BAYER
#endif
#if	V5_DEF_SNR_OV_9640_YUV
	(const PTSnrInfo)&gSensorInfo_OV9640_YUV,			//V5_DEF_SNR_OV_9640_YUV
#endif
	// 2M
#if	V5_DEF_SNR_MT9D111_BAYER
	(const PTSnrInfo)&gSensorInfo_MT9D111_BAYER,		//V5_DEF_SNR_MT9D111_BAYER
#endif
#if	V5_DEF_SNR_MT9D111_YUV
	(const PTSnrInfo)&gSensorInfo_MT9D111_YUV,			//V5_DEF_SNR_MT9D111_YUV
#endif
#if	V5_DEF_SNR_OV2620_BAYER
	(const PTSnrInfo)&gSensorInfo_OV2620_BAYER,			//V5_DEF_SNR_OV2620_BAYER
#endif
#if	V5_DEF_SNR_S5K3BAFX_YUV
	(const PTSnrInfo)&gSensorInfo_S5K3BAFX_YUV,			//V5_DEF_SNR_S5K3BAFX_YUV
#endif
#if	V5_DEF_SNR_S5K3BAFX_CCIR656
	(const PTSnrInfo)&gSensorInfo_S5K3BAFX_C656,		//V5_DEF_SNR_S5K3BAFX_CCIR656
#endif
#if	V5_DEF_SNR_PO1200N_BAYER
	(const PTSnrInfo)&gSensorInfo_PO1200N_BAYER,		//V5_DEF_SNR_PO1200N_BAYER
#endif
#if	V5_DEF_SNR_PO1200N_YUV
	(const PTSnrInfo)&gSensorInfo_PO1200N_YUV,			//V5_DEF_SNR_PO1200N_YUV
#endif
	// 3M
#if	V5_DEF_SNR_OV_3610_BAYER
	(const PTSnrInfo)&gSensorInfo_OV3610_BAYER,			//V5_DEF_SNR_OV_3610_BAYER
#endif
#if	V5_DEF_SNR_KAC3100_BAYER
	(const PTSnrInfo)&gSensorInfo_KAC3100_BAYER,		//V5_DEF_SNR_KAC3100_BAYER
#endif

	NULL												//Avoid no sensor define
};

const UINT16 gSensorNum = sizeof(gPSensorInfo) / sizeof(PTSnrInfo);

#endif //V578CMD_DISABLE
