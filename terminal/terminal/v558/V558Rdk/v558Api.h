/*
*****************************************************************
*			DDL558					*
*	DRIVER DEVELOPMENT LIBRARY OF VC0558  			*
*       COPYRIGHT (C) 2003, VIMICRO CORPORATION ("VIMICRO")	*
*	LICENSED MATERIAL  -  PROGRAM PROPERTY OF VIMICRO	*
*****************************************************************  
 
DDL558 source code has been made available to you by VIMICRO on an 
AS-IS basis. Anyone receiving this source is licensed under VIMICRO
copyrights to use it in any way he or she deems fit, including
copying it, modifying it, compiling it, and redistributing it either
with or without modifications.

Any person who transfers this source code or any derivative work
must include the VIMICRO copyright notice, this paragraph, and the
preceding two paragraphs in the transferred software.

------------------------------------------------------------------
File        : v558api.H
Purpose     : DDL558 Application Programming Interface
------------------------------------------------------------------

Revision:
                                 
      Boat Zhou    	initial  						2003-11-03   	  
      Boat Zhou    	video display,msg define, callback function   		2003-11-06   	  

*/

#ifndef _V558API_H_
#define _V558API_H_

#ifdef __cplusplus
extern "C" {
#endif


//====================================================================================
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//
//  Part 1: global definitions, structs and functions of whole chip
//
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//====================================================================================

//DOM-IGNORE-BEG
/********************************************************/
//   Structure and definition                           //
/********************************************************/
//DOM-IGNORE-END

//signed 8-bit integer
#ifndef SINT8
#define SINT8 signed char
#endif





//unsigned 8-bit integer
#ifndef UINT8
#define UINT8 unsigned char
#endif

//signed 16-bit integer
#ifndef SINT16
#define SINT16 signed short
#endif

//unsigned 16-bit integer
#ifndef UINT16
#define UINT16 unsigned short
#endif

//signed 32-bit integer
#ifndef SINT32
#define SINT32 signed long
#endif

//unsigned 32-bit integer
#ifndef UINT32
#define UINT32 unsigned long
#endif

//null pointer
#ifndef NULL
#define  NULL	((void *)0)
#endif

//void pointer
#ifndef PVOID
#define  PVOID void *
#endif

//true
#ifndef TRUE
#define TRUE	1
#endif

//false
#ifndef FALSE
#define FALSE	0
#endif

#ifndef __size_t
#define __size_t 1
typedef unsigned int size_t;
#endif


//V558 color format
typedef struct _V558_COLOR 
{
	UINT8 r;
	UINT8 g;
	UINT8 b;
	UINT8 reserve;
} V558_COLOR, *PV558_COLOR;

//V558 bitmap format
typedef struct tag_V558_BITMAP
{
	UINT16	headSize;
	UINT16	begin;
	UINT16	dataSize;
	UINT16	width, height;
	UINT8	colordep;
	UINT8	reserver;
} V558_BITMAP,*PV558_BITMAP;

//V558 rect format
typedef struct _V558_RECT
{	
	UINT16 left;
	UINT16 top;
	UINT16 width;
	UINT16 height;
} V558_RECT, *PV558_RECT;

//V558 size format
typedef struct _V558_SIZE
{	
	UINT16 cx;
	UINT16 cy;
} V558_SIZE, *PV558_SIZE;

//V558 point format
typedef struct _V558_POINT
{	
	UINT16 x;
	UINT16 y;
} V558_POINT, *PV558_POINT;

//BMP file header
typedef struct tag_V558_BITMAPFILEHEADER 
{
	UINT8	dat[14];
} V558_BITMAPFILEHEADER, *PV558_BITMAPFILEHEADER;

//BMP info header
typedef struct tag_V558_BITMAPINFOHEADER
{
	UINT8	biSize[4];
	UINT8	biWidth[4];
	UINT8	biHeight[4];
	UINT8	biDat0[2];
	UINT8	biBitCount[2];
	UINT8	biDat1[4];
	UINT8	biSizeImage[4];
	UINT8	biDat2[16];
} V558_BITMAPINFOHEADER, *PV558_BITMAPINFOHEADER; 

//DOM-IGNORE-BEG
/********************************************************/
//   Functions                                          //
/********************************************************/
//DOM-IGNORE-END


/* Read chip register, implemented by OS*/
void V558_GetReg(UINT32 uAddr, UINT8 *puVal);
/* Write chip register, implemented by OS*/
void V558_SetReg(UINT32 uAddr, UINT8 uVal);
/* Read chip sram, implemented by OS*/
void V558_ReadSram(UINT32 uStartAddr, UINT8* pData, UINT32 uSize);
/* Write chip sram, implemented by OS*/
void V558_WriteSram(UINT32 uStartAddr, UINT8* pData, UINT32 uSize);
/* Delay function, implemented by OS*/
void V558_Delay(UINT32 uTime);
/* Memory copy function, implemented by OS*/
extern void *memcpy(void * /*s1*/, const void * /*s2*/, size_t /*n*/);

void V558_Open(void);

//====================================================================================
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//
//		Part 2: base structs and definitions of different modules
//
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//====================================================================================

//DOM-IGNORE-BEG
/********************************************************/
//   Base structure and definition of CTRL module     //
/********************************************************/
//DOM-IGNORE-END

#define V558_INT_MAX_NUMBER 64
#define CTRLDESCLENGTH 40

//558 chip interrupt config
typedef struct tag_V558_CTRL_INT {
	UINT8 LeverOrEdge;
	UINT8 HighOrLow;
	UINT8 Width;
} V558_CTRL_INT, *PV558_CTRL_INT;

//558 chip clock config
typedef struct tag_V558_CTRL_CLK {
	UINT32 clkin; //clock input(KHZ)
	UINT32 clkout;//chip mclk(KHZ)
	UINT8 factorh;
	UINT8 factorl;
} V558_CTRL_CLK, *PV558_CTRL_CLK;

//DOM-IGNORE-BEG
/********************************************************/
//   Base structure and definition of LCD/GE module   //
/********************************************************/
//DOM-IGNORE-END

#define PANELDESCLENGTH	0x40

typedef void (* PPanelSetRegFunc)(UINT16 adr, UINT16 val);
typedef void (* PPanelInitFunc)(void);

//LCD PANEL register R/W format
typedef struct tag_V558_PANELREGVALUE {
	UINT16 adr;			/* panel internal register address */
	UINT16 val;			/* panel internal register setting value */
	UINT16 wait;		/* ms */
} V558_PANELREGVALUE;

//LCD PANEL information struct
typedef struct tag_V558_PANELPARM {
	UINT16 len;			/* register list length */
	const V558_PANELREGVALUE *regval;	/* registers array point */	
} V558_PANELPARM;

//lcd module: layer property
typedef struct tag_V558_LAYER_PROPERTY {
	V558_SIZE Size;
	V558_RECT DisRect;
	V558_POINT DisPoint;	
	UINT8 Format;	
} V558_LAYER_PROPERTY,*PV558_LAYER_PROPERTY;

#define V558_LCDPANEL_NOT_INIT		0
#define V558_LCDPANEL_POWOFF		1
#define V558_LCDPANEL_SLEEP			2
#define V558_LCDPANEL_STANDBY		3
#define V558_LCDPANEL_WAKEUP		4
#define V558_LCDPANEL_WAKEFSTY		5
#define V558_LCDPANEL_POWON			6

enum
{
	V558_MAIN_LCDPANEL,
	V558_SLAVE_LCDPANEL
};

#define V558_GE_BITBLT_OP		0x8


//DOM-IGNORE-BEG
/********************************************************/
//   Base structure and definition of SIF/ISP module  //
/********************************************************/
//DOM-IGNORE-END

#define SENSORDESCLENGTH	40

//Sensor register r/w struct
typedef struct tag_V558_SENSORREGVALUE {
	UINT16 adr;
	UINT16 val;
	UINT16 wait;		/* ms */
} V558_SENSORREGVALUE, *PV558_SENSORREGVALUE;

//sensor information 
typedef struct tag_V558_SENSORPARM {
	UINT8 chipadr;	//i2c or ovI2c chip address
	UINT8 IicWordOrByte;
			//0-byte,1-word
	UINT8 len;
	const V558_SENSORREGVALUE *regval;
} V558_SENSORPARM, *PV558_SENSORPARM;

//isp ae parameter
typedef struct tag_V558_AE_PARM{
	UINT8 target;	//y mean target
	UINT8 yfieldstep;	//define a field1, when //ymean<yfieldstep
			//or mean>0xff-yfieldstep, don't change gain ,
			//only change time for one step.
	UINT8 lock;	//y mean lock
	UINT8 unlock;	//y mean ulock
	UINT8 gain;	//default gain value
	UINT8 gaindelay;	//when change gain and exposure time, gain need delay
			//0,1¡­frame, so that can synchronize time and gain.
	UINT8 gainmaxmaxtime;
			//when exposure is max, can change gain to get lighter
	UINT8 gainrange[4];
			//define 4 gain range when time = 100,200,300,400,
	UINT8 yfieldmulstep;	
			//just like yfieldstep, define a field2, when 
			//ymean<yfieldmulstep or mean>0xff-yfieldstep,
			//don't change gain ,only change time for multi step.
	UINT8 stepgreattime;	//if time is great; min time adjust sep = stepgreattime
	UINT8 inclockout;	//if time < 100(light is too bright), increase y lock/unlock
			//range by inclockout
	UINT8 gainminin;	//in indoor state ,when time = 100, gain can reduce to
			//gainminin
	UINT8 etdemout;	//if time < 100(light is too bright), change time by a float
	UINT8 etnumout;	//factor: the denominator is etdemout and the numerator is
			//etnumout
	UINT8 reserved1;
	UINT8 reserved2;
	UINT8 reserved3;
}V558_AE_PARM, *PV558_AE_PARM;

//Isp auto flicker detection parameter
typedef struct tag_V558_AFLK_PARM{
	UINT8 flkcnt;	//auto flicker line count
	UINT8 flklinemode;//auto flicker line mode
	UINT8 flkstep;	//auto flicker line step
	UINT16 flklst;	//auto flicker line start
	UINT8 outthd;	//when time > 100, if the quotient of thd50 and thd60 > outthd,
			//the light type is indoor else outdoor
	UINT8 lighttype;	// light type 0--indoor, 1--out door
	UINT8 indoorfreqtype;
			//freq:0--50hz, 1--60hz
	UINT16 timeinout;	// when time < 100; set time as timeinout to detect in/out
			//lighttype
	UINT8 reserved1;
	UINT8 reserved2;
}V558_AFLK_PARM, *PV558_AFLK_PARM;

//Isp auto function info
typedef struct tag_V558_ISP_AUTOCTRL{
	V558_AE_PARM y;	//auto exposure parameter
	V558_AFLK_PARM aflk;
			//Auto flicker parameter
	UINT8 enae;	//auto exposure enable/disable flag
	UINT8 enflk;	//auto flicker enable/disable flag
	UINT8 intfrq;	//auto function interrupt occur freq, default'0' 
	UINT16 maxexp;	//max exposure time
	UINT16 defexp;	//default exposure time
	UINT16 minexp;	//min exposure time
	UINT16 exppoint;	//exposure time point
	UINT16 dpcThd;	//do dpc, exposure time threshold changing noises table value
	UINT8 enlight;	//light indoor outdoor detection enable/disable
	UINT8 reserved1;
	UINT8 reserved2;
} V558_ISP_AUTOCTRL, *PV558_ISP_AUTOCTRL;

//ISP property
typedef struct tag_V558_ISP_PROPERTY{
	UINT8 brightness;	//brightness
	UINT8 contrast;	//contrast
}V558_ISP_PROPERTY, *PV558_ISP_PROPERTY;

//ISP parameter struct
typedef struct tag_V558_SNRISPPARM {
	UINT8 chipadr;	//i2c or ovI2c chip address
	UINT8 len;	//len-register num
	const V558_SENSORREGVALUE *regval;
			//register value array
	V558_ISP_AUTOCTRL ctrl;
			//ctrl-auto exposure, auto flicker configuration
	V558_ISP_PROPERTY prp;
			// image quality property{brightness; contrast}
	UINT8 gammaRGB[17];
			// RGB gamma value array
} V558_SNRISPPARM, *PSNRV558_ISPPARM;

//SIF parameter struct
typedef struct tag_V558_SIFPARM {
	UINT16	len;
	const V558_SENSORREGVALUE	*regval;
} V558_SIFPARM, *PV558_SIFPARM;		//include internal ISP register ??

//I2C batch struct
typedef struct tag_V558_I2C_BATCH
{
	UINT8 RegBytes;
	UINT8 RegAddr;
	UINT8 RegVal[3];
} V558_I2C_BATCH, *PV558_I2C_BATCH;

//AE batch parameter
typedef struct tag_V558_AEPARM
{
	UINT8	chipadr;
	UINT8	len;
	V558_I2C_BATCH *pbatch;
} V558_AEPARM, *PV558_AEPARM;

//ISP edge enhancement parameter
typedef struct tag_V558_EDGEPARM {
	UINT8	femm;
	UINT8	femp;
	UINT8	femx1;
	UINT8	femx2;
	UINT8	femmin;
	UINT8	femmax;
} V558_EDGEPARM, *PV558_EDGEPARM;

//ISP lens fall off parameter
typedef struct tag_V558_LENSFALLOFF {
	UINT16	lfcenx;
	UINT16	lfceny;
	UINT16	lffrec;
} V558_LENSFALLOFF, *PV558_LENSFALLOFF;

//ISP color correction struct
typedef struct tag_V558_COLORCVT {
	UINT8	cradj;
	UINT8	cbadj;
	UINT8	croft;
	UINT8	cboft;
} V558_COLORCVT, *PV558_COLORCVT;

//ISP AWB color range struct
typedef struct tag_V558_AWBCLRRANGE {
	UINT8	ybot;
	UINT8	ytop;
	UINT8	qtop;
	UINT8	itop;
} V558_AWBCLRRANGE, *PV558_AWBCLRRANGE;

//ISP AWB gain limit struct
typedef struct tag_V558_AWBGAINLMT {
	UINT8	glow;
	UINT8	gtop;
} V558_AWBGAINLMT, *PV558_AWBGAINLMT;

//ISP color correction matrix
typedef struct tag_V558_COLORMATRIX {
	UINT8	cmat11, cmat12, cmat13;
	UINT8	cmat21, cmat22, cmat23;
	UINT8	cmat31, cmat32, cmat33;
} V558_COLORMATRIX, *PV558_COLORMATRIX;

//ISP color correction offset
typedef struct tag_V558_COLORMATRIXOFF {
	UINT8	ofsr;
	UINT8	ofsg;
	UINT8	ofsb;
} V558_COLORMATRIXOFF, *PV558_COLORMATRIXOFF;

//sensor communication type
enum V558_SNRCOMTYPE
{
	V558_SNRC_I2C,
	V558_SNRC_OVI2C,
	V558_SNRC_SERIAL
};

//pixel rate
typedef enum _V558_PIXEL_RATE
{
	V558_PIXEL_RATE2	= 0x1,
	V558_PIXEL_RATE4	= 0x3,
	V558_PIXEL_RATE6	= 0x5,
	V558_PIXEL_RATE8	= 0x7,
	V558_PIXEL_RATE12	= 0xB,
	V558_PIXEL_RATE16	= 0xF
}V558_PIXEL_RATE;

//I2C batch struct
typedef struct tag_V558_PWR_CFG
{
	UINT8 PwrOn;	//sensor power on signal :0 or 1
	UINT8 SnrEn;	//sensor enable signal   :0 or 1
	UINT8 PltEn;	//sensor polarity enable :0 or 1
	UINT8 PltMd;	//sensor polarity mode   :0 or 1
	UINT8 SnrRe;	//sensor reset  signal   :0 or 1
} V558_PWR_CFG, *PV558_PWR_CFG;


//DOM-IGNORE-BEG
/********************************************************/
//   Base structure and definition of IPP module      //
/********************************************************/
//DOM-IGNORE-END

//filter param, divide size to diffrent field, in each set different coef 
typedef struct tag_V558_FILTERPARM
{
	UINT8	max;  //field max val, max 128
	UINT8	min;  //field min val, min 0
	UINT8	coef[4]; //set different coef val,
} V558_FILTERPARM, *PV558_FILTERPARM;


//DOM-IGNORE-BEG
/********************************************************/
//   Base structure and definition of JPEG module       //
/********************************************************/
//DOM-IGNORE-END


//DOM-IGNORE-BEG
/********************************************************/
//   Base structure and definition of LBUF module       //
/********************************************************/
//DOM-IGNORE-END


//DOM-IGNORE-BEG
/********************************************************/
//   Base structure and definition of JBUF module       //
/********************************************************/

//DOM-IGNORE-END


//DOM-IGNORE-BEG
/********************************************************/
//   Base structure and definition of SWGP module       //
/********************************************************/
//DOM-IGNORE-END


//====================================================================================
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//
//		Part 3: global structs and definitions of different modules
//
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//====================================================================================

//DOM-IGNORE-BEG
/********************************************************/
//   Global structure and definition of CTRL module     //
/********************************************************/
//DOM-IGNORE-END

//558 chip power state
typedef enum _V558_CTRL_STATE
{
	V558_CTRL_NOT_INIT	= 0X0,
	V558_CTRL_POWOFF		,
	V558_CTRL_SLEEP		,
	V558_CTRL_STANDBY		,
	V558_CTRL_POWON
}V558_CTRL_STATE;

//558 workmode
typedef enum _V558_WORKMODE 
{ 
	V558_MODE_VIEW					= 0x00,
	V558_MODE_CAP_VIDEO				= 0x01, 
	V558_MODE_DISPLAY				= 0x02,
	V558_MODE_ENCODER				= 0x03,
	V558_MODE_DECODER				= 0x04,
	V558_MODE_DECODER_FRAME			= 0x24,
	V558_MODE_CAP_VIDEO_FRAM		= 0x21,
	V558_MODE_DIRECTDISPLAY         = 0x05,
	V558_MODE_THROUGH				= 0x06,
	V558_MODE_CAP_STILL				= 0x81,
	V558_MODE_CAP_STILL_FRAM		= 0xa1,
	V558_MODE_CAP_STILL_THUMB		= 0xc1,
	V558_MODE_CAP_STILL_THUMB_FRAM	= 0xe1,
	V558_MODE_BYPASS				= 0xff 
} V558_WORKMODE; 

//V558 reset module selection
typedef enum _V558_SW_RESET
{
	V558_SW_RESET_SIF		= 0x1,
	V558_SW_RESET_ISP		= 0x2,
	V558_SW_RESET_IPP		= 0x4,
	V558_SW_RESET_LB		= 0x10,
	V558_SW_RESET_JPG		= 0x20,
	V558_SW_RESET_GE		= 0x40,
	V558_SW_RESET_LCD		= 0x80,
	V558_SW_RESET_GBL		= 0x8000
} V558_SW_RESET;

//558 clk control mode type
typedef enum _V558_MOD_CLK
{
	V558_MOD_CLK_SIF		= 0x1,
	V558_MOD_CLK_ISP		= 0x2,
	V558_MOD_CLK_IPP		= 0x4,
	V558_MOD_CLK_LB		= 0x10,
	V558_MOD_CLK_JPG		= 0x20,
	V558_MOD_CLK_GE		= 0x40,
	V558_MOD_CLK_LCD		= 0x80,
	V558_MOD_CLK_ALL		= 0xff
} V558_MOD_CLK;


//558 interrupt type
typedef enum _V558_INT_TYPE
{
	V558_INT_AUTO_FUNC				= 0,	//ISP
	V558_INT_JPG_ERROR				= 8,	//JPGERROR
	V558_INT_JPG_DONE				,		//JPGDONE
	V558_INT_LCD_UNDEFINE0			= 16,	//LCD
	V558_INT_LCD_UNDEFINE1			,
	V558_INT_LCD_VIDEO_DISP_FRM_STOP,
	V558_INT_LCD_DISP_FIFO_FULL		,
	V558_INT_LCD_UPDATE_A_LAYER		,
	V558_INT_LCD_UPDATE_B_LAYER		,
	V558_INT_LCD_UPDATE_BACK_LAYER	,
	V558_INT_8BUF_FULL				=24,	//LBUF
	V558_INT_CAP_DONE				=32,	//64KBUF
	V558_INT_CAP_FIFO_FULL			,
	V558_INT_JPEG_BUF_ERROR			,
	V558_INT_ALL					= 0xffff
}V558_INT_TYPE;

//558 CTRL module info
typedef struct tag_V558_CTRL_INFO {
	UINT8 Desc[CTRLDESCLENGTH];
	UINT8 BusType;	//1-seperate,2-multi8,3-multi16
	V558_CTRL_INT Int;
	V558_CTRL_CLK Clk;
} V558_CTRL_INFO, *PV558_CTRL_INFO;

//558 CTRL module context
typedef struct tag_V558_CTRL_CONTEXT {
	PV558_CTRL_INFO	pInfo;
	UINT8		Valid;
	UINT8		State;
} V558_CTRL_CONTEXT, *PV558_CTRL_CONTEXT;


//DOM-IGNORE-BEG
/********************************************************/
//   Global structure and definition of LCD/GE module   //
/********************************************************/
//DOM-IGNORE-END

typedef struct tag_V558_LCDIFPARM {
	V558_SIZE size;		/* panel size */
	V558_POINT	mp, sp;	/* main panel start point & sub-panel start point */

	UINT16	hsa;		/* v558 hsa register setting value */
	UINT16	hea;		/* v558 hea register setting value */
	UINT16	hsxa;		/* v558 hsxa register setting value */
	UINT16	hsya;		/* v558 hsya register setting value */
	UINT16	hexa;		/* v558 hexa register setting value */
	UINT16	heya;		/* v558 heya register setting value */
	UINT16	lc;			/* v558 lc register setting value */
	UINT16	dpad;		/* v558 dpd register setting value */
	UINT16	wrcycle;	/* v558 wcy register setting value */
	UINT16	rdcycle;	/* v558 rcy register setting value */
	UINT8	rsPolarity;	/* v558 rsp register setting value */
	UINT8	dmap;		/* v558 dmap register setting value */
	UINT8	idsel;		/* v558 idsel register setting value */
	UINT8	dispadr;	/* v558 panel display address*/
	UINT8	paneltype;	/* panel type */
	UINT8	panelState;	/* panel state: power on, sleep... */
	UINT8	gammar[17];	/* gamma table relation to panel's gamma */
	UINT8	gammag[17];
	UINT8	gammab[17];
	V558_COLOR bkcolor;	/* background color */
} V558_LCDIFPARM;


typedef struct tag_V558_PANELINFO 
{
	UINT8	desc[PANELDESCLENGTH];	/* brief description of this panel */
	
	V558_LCDIFPARM *lcdif;			/* lcd I/F setting list */

	V558_PANELPARM powon;			/* panel enter power on state register setting */
	V558_PANELPARM powoff;	/* panel enter power off state register setting */
	V558_PANELPARM sleep;	/* panel enter sleep state register setting */
	V558_PANELPARM wakefs;	/* panel enter wake from sleep state register setting */
	V558_PANELPARM stdby;	/* panel enter standby state register setting */
	V558_PANELPARM wakefstd;/* panel enter wake from standby state register setting */

	PPanelSetRegFunc pPanelSetReg;	/* panel setting register function point */
	PPanelInitFunc	pPanelInit;		/* panel init function */
} V558_PANELINFO, *PV558_PANELINFO;

enum {
	LTS180SH_H1I_X_PANEL,
	LTS180S3_HF1_PANEL,
	TOSHIBA_LTM018A_PANEL,
	Truly_128160_16_PANEL,
	BYD_128160_PANEL,
	Jemiteck_J180S_Panel,
	UG_12D181_Panel,
	UG_120114A_Panel,
	LP8937_A_PANEL,
	CV90_K2817_SSD1781_PANEL
};

typedef struct tag_V558_LCD_CONTEXT 
{
	V558_PANELINFO	*panelInfo;
	V558_PANELINFO	*slavepanelInfo;
	V558_PANELINFO	*panelcur;
	PPanelSetRegFunc pPanelSetReg;
	UINT16		lc;
	UINT8		panelValid;
	UINT8		panelState;
	UINT8		SlavepanelState;
	UINT8		blDepth;
	V558_SIZE	blSize;
	UINT16		blSpan;
} V558_LCD_CONTEXT, *PV558_LCD_CONTEXT;


//DOM-IGNORE-BEG
/********************************************************/
//   Global structure and definition of SIF/ISP module  //
/********************************************************/
//DOM-IGNORE-END

#define MAX_BRIGHT_CONTRAST 64
#define MIN_BRIGHT_CONTRAST 1

//sensor type
enum V558_SENSORTYPE
{
	HYNIX7131GP_BAYER	,
	HYNIX7131GP_YUV		,
	FUJITSU_MBS03		,
	FUJITSU_MBS04_BAYER	,
	OV7649_BAYER		,
	HYNIX7131R_BAYER	,
	MI360_BAYER			,
	MI360_YUV			,
	OV7649_YUV			,
	TCM8210MD_YUV		,
	PIXEL2030K_BAYER	,
	PIXEL2030NC_BAYER	,
	PIXART302BCW_BAYER	,
	SAMSUNG437_BAYER	,
	SAMSUNG532_YUV		,
	STS502_BAYER		,
	SAMSUNG5K532_BAYER
};

//sensor module state
typedef enum _V558_SNR_STATE
{
	V558_SNR_NOT_INIT	= 0X0,
	V558_SNR_POWOFF		,
	V558_SNR_SLEEP		,
	V558_SNR_STANDBY	,
	V558_SNR_POWON
}V558_SNR_STATE;

//light frequence type
typedef enum _V558_FREQ_TYPE
{
	V558_FREQ_50HZ = 0x0,
	V558_FREQ_60HZ = 0x1
}V558_FREQ_TYPE;

//sensor format
typedef enum _V558_SNR_FORMAT
{
	V558_SNR_RGBFORMAT = 0x0,
	V558_SNR_YUVFORMAT = 0x1
}V558_SNR_FORMAT;

//sensor informat
typedef struct tag_V558_SENSORINFO 
{
	UINT8 desc[SENSORDESCLENGTH];
			//information description
	V558_SIZE size;	//sensor source image size
	UINT8 mode;	//sensor type(RGB or YUV)
	UINT8 pixRate;	//sensor pixel rate
	UINT8 clkcnt;	//count of master clock output to sensor
	UINT16 snrclk;	//sif clock kHz
	UINT8 comtype;	//sensor bus access type(IIC or 3wire)
	UINT16 type;	//sensor defined type(1,2,3,4¡­¡­)
	V558_PWR_CFG pwrcfg;
			//sensor power initial configure.
	V558_SIFPARM dsif;//SIF initial configure
	V558_SENSORPARM dsnr;
			//SENSOR initial configure
	V558_SNRISPPARM disp;
			//SENSOR ISP initial configure
} V558_SENSORINFO, *PV558_SENSORINFO;

//SIF/ISP module context
typedef struct tag_V558_SNR_CONTEXT {

	UINT8		valid;
	UINT8		snrstate;
	PV558_SENSORINFO	pInfo;
	UINT8		flkflag;
	UINT16		eptimes;
	UINT8		ygainmin;
	UINT8		ygainmax;
	UINT8		ygaintop;
	UINT8		ygainbottom;
	UINT8		ygaindelay;
	UINT8		lock;
} V558_SNR_CONTEXT, *PV558_SNR_CONTEXT;

//DOM-IGNORE-BEG
/********************************************************/
//   Global structure and definition of IPP module      //
/********************************************************/
//DOM-IGNORE-END

//Ipp info
typedef struct tag_V558_IPPINFO 
{
	UINT8	desc[40]; //description

	UINT8	enfilter; //filter enable/disbale
	UINT8	yoffset; //y offset
	UINT8	uoffset; //u offset
	UINT8	voffset; //v offset
	UINT8	utop, ubottom; //u top/bottom
	UINT8	vtop, vbottom; //v top/bottom
	UINT16	framedrop; //frame drop
	UINT8	filtercount; //filter param number
	V558_FILTERPARM *pfilter; //filter param.

	V558_POINT SizPoint; //Resizing window start point
	V558_SIZE SizSize; //Resizing window size
	UINT16 CapWidth; //Capture size
	UINT16 DisWidth; //Display size

} V558_IPPINFO, *PV558_IPPINFO;

//Ipp factor
typedef struct tag_V558_IPP_FACTOR {
		UINT16	src; //sorce
		UINT16	dst; //destinate
} V558_IPP_FACTOR, *PV558_IPP_FACTOR;

//Ipp context
typedef struct tag_V558_IPP_CONTEXT 
{
	PV558_IPPINFO	pIppInfo;

	UINT8	special;
	UINT8	enfilter;
	UINT8	yoffset;
	UINT8	uoffset;
	UINT8	voffset;
	UINT8	utop, ubottom;
	UINT8	vtop, vbottom;
	UINT16	framedrop;

	V558_SIZE ImgSize; //Source image size
	UINT8 PixRate; //Sensor pixel rate
	UINT8 SnrType; //Sensor type
	V558_POINT SizPoint; //Resizing window start point
	V558_SIZE SizSize; //Resizing window size
	UINT16 CapWidth; //Capture size
	UINT16 DisWidth; //Display size

	UINT8		Valid;
	UINT8		State;
} V558_IPP_CONTEXT, *PV558_IPP_CONTEXT;


//DOM-IGNORE-BEG
/********************************************************/
//   Global structure and definition of JPEG module     //
/********************************************************/
//DOM-IGNORE-END

//jpeg work mode
enum {
//	V558_CAPTURE_STILL,
//	V558_CAPTURE_STILL_THUMBNAIL,
	V558_JPEG_IDEL,
	V558_CAPTURE_STILLBRC,
	V558_CAPTURE_STILLBRC_THUMBNAIL,
	V558_CAPTURE_FRAMEVIDEO,
	V558_CAPTURE_FRAME,
	V558_CAPTURE_VIDEO,
	V558_DISPLAY_STILL,
	V558_DISPLAY_THUMBNAIL,
	V558_DISPLAY_VIDEO,
	V558_DECODE_ONLY,
	V558_DECODE_IPP,
	V558_DECODE_LCDIF,
	V558_ENCODE_JPG
};

typedef struct tag_V558_JPEGBUF
{
	UINT32		Len;		//buffer length 
	UINT32		ActLen;		//actual data length 
	UINT8		*pData;		//data pointer 
} V558_JPEGBUF, *PV558_JPEGBUF;

#define JPEGDESCLENGTH	40

typedef struct tag_V558_JPEGINFO
{
	UINT8	desc[JPEGDESCLENGTH];   //description

	UINT8	qf;                     //Q factor
	UINT8	tcr;                    //target compression ratio
	UINT8	entcr;                  //target compression ratio enable
	UINT8	res;                    //reservation
	UINT32	twc;                    //target word count
	UINT16	thumbwidth;             //the width of thumbnail
} V558_JPEGINFO, *PV558_JPEGINFO;

enum
{
	MSG_CAPTURE_DONE,
	MSG_JBUF_FULL,
	MSG_JPEG_DONE,
	MSG_READ_YUV,
	MSG_READ_YUV_DONE,
	MSG_WRITE_JPGDATA,
	MSG_JPEG_ERROR,
	MSG_JPEG_HEADER
};


typedef struct tag_V558_EXIFIFDCONTEXT
{
	UINT32          offset;       //
	UINT32          interno;
	UINT32          Valueoffset;
	UINT32          IFDLen;
	
}V558_EXIFIFDCONTEXT,*PV558_EXIFIFDCONTEXT;

typedef struct tag_V558_EXIFINFO
{
	UINT8           IsThumb;        //if IsThumb=0,haven't thumbnail;else if IsThumb=1,have thumbnail.
	UINT16          FieldLen;       //the length of app0 and app1 segment.
	UINT8           IsBigEndian;    //if IsBigEndian=0,little endian; else if IsBigEndian!=0,big endian.
	UINT32          IFD0Offset;     //0th IFD offset. If the TIFF header is followed immediately by the 
				     //0th IFD, it is written as 00000008.H.
	UINT32          thumblen;       //the length of thumbnail.
	UINT32          thumbstartpos;  //the start position of thumbnail.
	UINT32          thumbwidth;     //the width of thumbnail.
	UINT32          thumbheight;    //the height of thumbnail.
}V558_EXIFINFO,*PV558_EXIFINFO;
typedef struct tag_V558_JFIFINFO
{
	UINT8		isThumb;		//isThumb=1:have thumbnail; isThumb=0:no thumbnail.
	UINT16		fieldLen;
	UINT16		version;
	UINT8             units;
	UINT16            xdensity;
	UINT16            ydensity;
	UINT8             xthumbnail;
	UINT8             ythumbnail;
	UINT8*            pthumbbuf;	//position of thumbnail data begin
	UINT16            thumblen;	//the length of thumbnail
	UINT8             thumbformat;	//0:rgb24 ; 1:rgb8 ; 2:jpeg.
	UINT8             extensioncode;
}V558_JFIFINFO,*PV558_JFIFINFO;
typedef struct  tag_V558_JFIFTEMPLE
{
	UINT16            app0Marker;
	UINT16            lenght;
	UINT32            identifier;
	UINT16             mark;
	UINT16            version;
	UINT16            xdensity;
	UINT16            ydensity;
	UINT16            xythumbnail;
}V558_JFIFTEMPLE,*PV558_JFIFTEMPLE;

typedef struct tag_V558_JPEG_INDEX
{
	V558_SIZE	ImageSize;      //real image size
	V558_SIZE	JpgSize;        //image size after compensation,if the image format is YUV422,
                                        //then,JpgSize.cx must be multiple of 16,and JpgSize.cy must be
                                        //multiple of 8.
	V558_SIZE	LbufSize;       //set the image size of lbuf unit.

	UINT8		YUVType;        //the format of jpeg file,such as YUV422\YUV420\YUV411\YUV400
	UINT8	    HTCount;        //the number of Huffman table
	UINT8       QTCount;        //the number of Quantization table
	UINT8		CompCount;      //the number of component
	UINT8		blkcount;       //the number of block
	UINT8		restartflag;    //the flag of restart
	UINT32          flagadd[10];
	UINT32          flagindex;
	UINT8		*HT[4];         //the point of Huffman table
	UINT8	    HTLen[4];       //the length of every Huffman table

	UINT8		*QT[4];         //the point of Quantization table
	UINT8       QTPrecision[4]; //the precision of quantization table 
	UINT16		Comp[4];        //the component 
	
	
	UINT32		startpos;       //the position of jpeg file begin(begin with FFD8)
	UINT32	        offset;         //the offset from jpeg file begin to 
                                        //the postion of image data begin
	UINT32		endpos;         //the position of jpeg file end(end with FFD9)
	UINT8           isThumb;        //if isthumb=0,haven't thumbnail,else if isthumb=1,have thumbnail.
	UINT16          thumblen;       //the length of thumbnail
	UINT32          thumbstartpos;  //the start position of thumbnail.        
	UINT32          thumbwidth;     //the width of thumbnail.
	UINT32          thumbheight;    //the height of thumbnail.

	UINT32		vwc;            //video word count
	V558_JFIFINFO    jfifinfo;
} V558_JPEG_INDEX, *PV558_JPEG_INDEX;

typedef void (* PV558_JPEGCALLBACK)(UINT8 msg, PV558_JPEGBUF pdata);
typedef void (* PV558_JPEGDELAY)(void);

typedef struct tag_V558_JPEG_CONTEXT 
{
	PV558_JPEGINFO	JpegInfo;       //jpeg infortmation
	UINT8		Valid;          //jpeg module valid
	UINT8		State;          //jpeg module's state
	UINT8		mode;           //jpeg mode
	UINT8 		decdatatype;    //output data type after decode.

	UINT8		tcr;            //target compression ratio
	UINT8		entcr;          //tcr enable
	UINT8		qf;             //quantization factor
	UINT8		clkflag;        //the flag of clock
	UINT32           chipoutclk;     //the output clock of chip
	UINT8		fbdiv, plldivh, plldivl; //these is the parameter
						 //to adjust the clock.
	UINT16		wcy;                     //lcd panel write control

	UINT16		thumbwidth;              //the width of thumbnail
	UINT16		decodewidth;             //the output data width of decode
	UINT16		lcdctrl;                 //the value of lcd ctrl register
	UINT16      decline;                 //the number of decode output line from lbuf.
	UINT16      decMaxline;              //the max number of decode output line from.
	UINT16      declinesum;              //the number of decode output line to the buffer.
	UINT16      decMaxlinesum;           //the max number of decode output line to the buffer.
	
	UINT32		twc;                     //target word count
	V558_SIZE	thumb;                   //the size of thumbnail

	V558_JPEGBUF 	buf;                     //jpeg buffer
	V558_JPEGBUF 	dst;                     //destination buffer
	PV558_JPEGCALLBACK pcall;                //the pointer of callback

	UINT32		jbufadr;                 //jbuf start address
	UINT32		jbufsize;                //the size of jbuf
	UINT32		thumbadr;                // thubnail buffer start address
	UINT32		thumbsize;               //the size of thumbnail
	V558_JPEG_INDEX header;
} V558_JPEG_CONTEXT, *PV558_JPEG_CONTEXT;

//V558 yuv type
typedef struct _YUVType{
	UINT16 y;                  //y compenent in yuv model
	UINT16 u;                  //u compenent in yuv model
	UINT16 v;                  //v compenent in yuv model
}YUVType;

//V558 rgb type
typedef struct _RGBType{
	UINT16 r;                 //red color
	UINT16 g;                 //green color
	UINT16 b;                 //blue color
}RGBType;

//V558 raw type
typedef struct _RawType{
	UINT8  high;      //high 8 bit of raw data 
	UINT8  low;		  //low  8 bit of raw data 
}RawType;


//DOM-IGNORE-BEG
/********************************************************/
//   Global structure and definition of LBUF module     //
/********************************************************/
//DOM-IGNORE-END


//DOM-IGNORE-BEG
/********************************************************/
//   Global structure and definition of JBUF module     //
/********************************************************/
//DOM-IGNORE-END


//DOM-IGNORE-BEG
/********************************************************/
//   Global structure and definition of SWGP module     //
/********************************************************/
//DOM-IGNORE-END


//====================================================================================
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//
//		Part 4: global functions of different modules
//
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//====================================================================================

//DOM-IGNORE-BEG
/********************************************************/
//   Global functions of CTRL module                    //
/********************************************************/
//DOM-IGNORE-END

void V558_CtrlInitContext(void);
UINT8 V558_CtrlSetInfo(V558_CTRL_INFO *info);
UINT8 V558_CtrlOpen(void);
UINT8 V558_CtrlGetBusType(void);
void V558_CtrlIntInit(UINT8 LeverOrEdge,UINT8 HighOrLow, UINT8 Width);
void V558_CtrlIntAdd(UINT8 uIntType, void (*func)());
void V558_CtrlIntRemove(UINT8 uIntType);
void V558_CtrlIntEnable(UINT16 uIntType,UINT8 uEnable);
void V558_CtrlIntHandle(void);
UINT16 V558_CtrlSetChipClk(UINT32 clkin, UINT32 clkout, UINT8 factorh, UINT8 factorl);
void V558_CtrlGetChipClk(UINT32* clkin, UINT32* clkout, UINT8* factorh, UINT8* factorl);
UINT16 V558_CtrlChangeChipClk(UINT32 clkout);
void V558_CtrlSetChipClkOn(void);
void V558_CtrlSetChipClkOff(void);


//DOM-IGNORE-BEG
/********************************************************/
//   Global functions of LCD/GE module                  //
/********************************************************/
//DOM-IGNORE-END

void	V558_LcdInitContext(void);
UINT16	V558_LcdSetPanelInfo(PV558_PANELINFO info);	
UINT16	V558_LcdSetSlavePanelInfo(PV558_PANELINFO info);
UINT16	V558_LcdOpen(void);
void	V558_LcdClose(void);
V558_LCD_CONTEXT * V558_LcdGetContext(void);
void	V558_LcdSwitchPanel(UINT8 sel);

void	V558_LcdSetLcdIFParm(V558_LCDIFPARM *pPif);
void	V558_SetPanelRegList(V558_PANELPARM parm);
void	V558_LcdSetRWpanelFunc(PPanelSetRegFunc func);
UINT16	V558_SelectPanel(UINT8 sel);

void	V558_LcdOpenModule(void);
UINT16	V558_LcdSetPanelState(UINT8 state);	
UINT8	V558_LcdGetPanelState(void);	

void	V558_LcdWriteBL(UINT8 *pData, UINT16 length, 
						UINT16 beginp, UINT16 span, UINT16 skip);

void	V558_LCDDrawBL(UINT16 startx, UINT16 starty, UINT16 endx, UINT16 endy, UINT8 *uspImage);
void	V558_LcdDrawBmp(V558_POINT point, UINT8 *pData, UINT8 Converse);

void	V558_GeDrawLine(V558_POINT ps, V558_POINT pe, V558_COLOR color);
void	V558_GeBitblt(V558_POINT src, V558_POINT ptn, V558_RECT dst, UINT8 bitwise);


//DOM-IGNORE-BEG
/********************************************************/
//   Global functions of SIF/ISP module                 //
/********************************************************/
//DOM-IGNORE-END

void V558_SifInitContext(void);
UINT8 V558_SifSetSensorInfo(V558_SENSORINFO *info);
UINT8 V558_SifOpen(void);
UINT8 V558_SifGetSnrInfoType(UINT8* type);
UINT8 V558_SifGetSnrInfoPixRate(UINT8* rate);
UINT8 V558_SifGetSnrInfoImgSize(V558_SIZE* size);
UINT16 V558_SifSetSnrState(UINT8 state);	
UINT8 V558_SifGetSnrState(void);
void V558_SifSetSnrTime(UINT8 freqtype, UINT32 t);
void V558_SifSetSnrGain(UINT8 gain);
void V558_SifSetFlkParam(UINT8 freqflag);
UINT8 V558_IspSetExposureTime(UINT16 time);
UINT8 V558_IspGetExposureTime(UINT16* time);
UINT8 V558_IspSetGain(UINT8 gain);
UINT8 V558_IspGetGain(UINT8* gain);
UINT8 V558_IspGetLightGuide(UINT8* lightguide);
UINT8 V558_IspSetAeFlag(UINT8 uAeFlag);
UINT8 V558_IspGetAeFlag(UINT8* uAeFlag);
UINT8 V558_IspSetAFlkFlag(UINT8 uAFlkFlag);
UINT8 V558_IspGetAFlkFlag(UINT8* uAFlkFlag);
UINT8 V558_IspSetALightFlag(UINT8 uALightFlag);
UINT8 V558_IspGetALightFlag(UINT8* uALightFlag);
UINT8 V558_IspSetFreq(UINT8 freqType);
UINT8 V558_IspGetFreq(UINT8* freqType);
UINT8 V558_IspSetLight(UINT8 lightType);
UINT8 V558_IspGetLight(UINT8* lightType);
void V558_IspGetBrightness(UINT8 *lValue);
void V558_IspSetBrightness(UINT8 lValue);
void V558_IspGetContrast(UINT8 *lValue);
void V558_IspSetContrast(UINT8 lValue);

//DOM-IGNORE-BEG
/********************************************************/
//   Global functions of IPP module                     //
/********************************************************/
//DOM-IGNORE-END

void V558_IppInitContext(void);
UINT16 V558_IppSetInfo(PV558_IPPINFO info);
UINT8 V558_IppOpen(void);
void V558_IppSetCapture(UINT16 width);
void V558_IppSetDisplay(UINT16 width);
void V558_IppSetThumb(UINT16 width);
void V558_IppSetSizerCapture(V558_POINT pt, V558_SIZE size, UINT16 width);
void V558_IppSetSizerDisplay(V558_POINT pt, V558_SIZE size, UINT16 width);
void V558_IppSetZoomDisplay(V558_POINT pt, V558_SIZE size, UINT16 width);
void V558_IppDisplay(V558_SIZE src, V558_POINT pt, V558_SIZE size, UINT16 width);
void V558_IppCapture(V558_SIZE src, V558_POINT pt, V558_SIZE size, UINT16 width);
void V558_IppSetFilter(UINT16 x, UINT16 y);
void V558_IppSelectPrefilter(int sel);
void V558_IppSpeSephia(UINT8 byU, UINT8 byV);
void V558_IppSpeColorRange(UINT8 uTop, UINT8 uBottom, UINT8 vTop, UINT8 vBottom);
void V558_IppSpeNegative(void);
void V558_IppSpeRelief(void);
UINT8 V558_IppDisableSpecial(void);
void V558_GetDecodeIppSize(UINT16 srcwidth,UINT16 srcheight,UINT16 dstwidth, UINT16* ptempwidth,UINT16* ptempheight);

//DOM-IGNORE-BEG
/********************************************************/
//   Global functions of JPEG module                    //
/********************************************************/
//DOM-IGNORE-END

void V558_CaptureStill(PV558_JPEGBUF pbuf, PV558_JPEGCALLBACK pcall);
void V558_StartCapVideo(PV558_JPEGBUF pbuf, PV558_JPEGCALLBACK pcall);
void V558_CaptureStillThumb(PV558_JPEGBUF pbuf, PV558_JPEGCALLBACK pcall);
void V558_CaptureFrame(PV558_JPEGBUF pbuf, PV558_JPEGCALLBACK pcall);
void V558_CaptureFrameVideo(PV558_JPEGBUF pbuf, PV558_JPEGCALLBACK pcall);
void V558_StopCapVideo(void);


UINT8 V558_JpegParse(PV558_JPEGBUF pbuf, PV558_JPEG_INDEX pinfo);
UINT8 V558_JfifParse(PV558_JPEGBUF pbuf, PV558_JFIFINFO pinfo);
UINT16 V558_ConvertThumbRGB8ToRaw(UINT8* dst, UINT8* src, UINT8 xthumb, UINT8 ythumb,UINT8 rawformat);

void V558_DisplayStill(PV558_JPEGBUF pbuf, PV558_JPEG_INDEX pinfo, 
					  PV558_LAYER_PROPERTY proty);

UINT8 V558_HostDecode(PV558_JPEGBUF psrc, PV558_JPEGBUF pdst,UINT8 decdatatype, 
						PV558_JPEG_INDEX pinfo, PV558_JPEGCALLBACK pcall);
UINT16 V558_GetDecodeIppMinWidth(PV558_JPEG_INDEX pinfo);					
UINT8 V558_HostDecodeIpp(PV558_JPEGBUF psrc, PV558_JPEGBUF pdst, UINT8 decdatatype, PV558_JPEG_INDEX pinfo, 
						 PV558_JPEGCALLBACK pcall, UINT16 width);
UINT8 V558_HostDecodeIppSize(PV558_JPEGBUF psrc, PV558_JPEGBUF pdst, UINT8 decdatatype, PV558_JPEG_INDEX pinfo, 
						PV558_JPEGCALLBACK pcall, PV558_SIZE psize, UINT16 width);
UINT8 V558_HostDecodeLcdIF(PV558_JPEGBUF psrc, PV558_JPEGBUF pdst, UINT8 decdatatype, PV558_JPEG_INDEX pinfo, 
						PV558_JPEGCALLBACK pcall, PV558_LAYER_PROPERTY proty);
UINT8 V558_HostDecodeLcdIFSize(PV558_JPEGBUF psrc, PV558_JPEGBUF pdst, UINT8 decdatatype, PV558_JPEG_INDEX pinfo, 
						PV558_JPEGCALLBACK pcall, PV558_SIZE psize, PV558_LAYER_PROPERTY proty);
void V558_JbufReadThumb(PV558_JPEGBUF pbuf, V558_SIZE *size);
UINT8 V558_YUVPointToBlock(PV558_JPEGBUF psrc, PV558_JPEGBUF pdst,PV558_SIZE psize,PV558_SIZE pnewsize, UINT8 YUVType);
UINT8 V558_YUVTypePointToBlock(PV558_JPEGBUF psrc, PV558_JPEGBUF pdst,PV558_SIZE psize,PV558_SIZE pnewsize, UINT8 YUVType,UINT8 encdatatype);
//UINT8 V558_HostEncode(PV558_JPEGBUF psrc, PV558_JPEGBUF pdst, PV558_JPEGCALLBACK pcall,
//					  V558_SIZE size,V558_SIZE NewSize, UINT8 yuvmode);
UINT8 V558_HostEncode(PV558_JPEGBUF psrc, PV558_JPEGBUF pdst, PV558_JPEGCALLBACK pcall,
                      V558_SIZE size, UINT8 yuvmode);



void V558_AdjustCaptureSize(V558_SIZE size);
void V558_SetThumbWidth(UINT16 width);
UINT16 V558_GetThumbWidth(void);

UINT8 V558_LbufOpen(void);

void V558_JbufGeInit(void);
UINT8 V558_JbufOpen(void);

void V558_JpegInitContext(void);
UINT8 V558_JpegOpen(void);
UINT16 V558_JpegSetInfo(PV558_JPEGINFO info);

///data transform
void YUV2RGB(RGBType* rgb,YUVType* jpeg);
void RGB2RAW(RawType* raw,RGBType* rgb,UINT8 rawFormat);
UINT8 V558_RGB2YUV(PV558_JPEGBUF pOut , PV558_JPEGBUF pIn , UINT8 pOutDataType , UINT8 pInDataType);


//DOM-IGNORE-BEG
/********************************************************/
//   Global functions of LBUF module                    //
/********************************************************/
//DOM-IGNORE-END


//DOM-IGNORE-BEG
/********************************************************/
//   Global functions of JBUF module                    //
/********************************************************/
//DOM-IGNORE-END


//DOM-IGNORE-BEG
/********************************************************/
//   Global functions of SWGP module                    //
/********************************************************/
//DOM-IGNORE-END


//====================================================================================
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//
//		Part 5: base functions of different modules
//
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//====================================================================================

//DOM-IGNORE-BEG
/********************************************************/
//   Base functions of CTRL module                      //
/********************************************************/
//DOM-IGNORE-END

void V558_CtrlSetWorkMode(UINT8 uMode);
UINT8 V558_CtrlGetWorkMode(void);
void V558_CtrlEnableBRC(void);
void V558_CtrlDisableBRC(void); 
void V558_CtrlSetBitRateCtrl(UINT8 enable);
void V558_CtrlJbutPortEnable(void);
void V558_CtrlJbutPortDisable(void);
void V558_CtrlMultiClrAutoEnable(void);
void V558_CtrlMultiClrAutoDisable(void);
void V558_CtrlSwReset(UINT16 uType);
UINT8 V558_CtrlSetModClkOn(UINT8 uType);
UINT8 V558_CtrlSetModClkOff(UINT8 uType);
void V558_CtrlGetChipVision(UINT16* uType);
void V558_CtrlSetChipClkIDiv(UINT8 idiv);
void V558_CtrlChipClkIDivEnable(void);
void V558_CtrlChipClkIDivDisable(void);
void V558_CtrlSetChipClkPlldiv(UINT8 fbdiv, UINT8 plldivh, UINT8 plldivl);
void V558_CtrlGetChipClkPlldiv(UINT8* fbdiv, UINT8* plldivh, UINT8* plldivl);
void V558_CtrlChangeChipClkPlldiv(UINT8 fbdiv, UINT8 plldivh, UINT8 plldivl);
void V558_CtrlChipClkPlldivEnable(void);
void V558_CtrlChipClkPlldivDisable(void);
void V558_CtrlSetGpioCfg(UINT8 cfg);
UINT8 V558_CtrlGetGpioCfg(void);
void V558_CtrlSetGpioMode(UINT8 mode);
UINT8 V558_CtrlGetGpioMode(void);
void V558_CtrlSetGpioDirection(UINT8 dir);
UINT8 V558_CtrlGetGpioDirection(void);
void V558_CtrlSetGpioData(UINT8 data);
UINT8 V558_CtrlGetGpioData(void);
void V558_CtrlSetIntCtrl(UINT8 LeverOrEdge,UINT8 HighOrLow, UINT8 Width);
void V558_CtrlSetIntEnFirst(UINT8 uVal);
UINT8 V558_CtrlGetIntEnFirst(void);
void V558_CtrlSetIntServe(UINT8 uVal);
UINT8 V558_CtrlGetIntServe(void);
UINT8 V558_CtrlGetIntFlagFirst(void);
void V558_CtrlSetIntEnSecend(UINT8 nFirstType, UINT8 uVal);
UINT8 V558_CtrlGetIntEnSecend(UINT8 nFirstType);
UINT8 V558_CtrlGetIntFlagSecend(UINT8 nFirstType);


//DOM-IGNORE-BEG
/********************************************************/
//   Base functions of LCD/GE module                  //
/********************************************************/
//DOM-IGNORE-END

void	V558_PanelSetSize(V558_SIZE Size);
void	V558_PanelGetSize(V558_SIZE *Size);

void	PanelWrRsClr(UINT16 val);
void	PanelWrRsSet(UINT16 val);
UINT16	PanelRdRsClr(void);
UINT16	PanelRdRsSet(void);

void	V558_LcdPanelStart(UINT8 adr);
void	V558_LcdPanelSetGram(V558_POINT pt, UINT16 color);
UINT16	V558_LcdPanelGetGram(V558_POINT pt);
void	V558_LcdSetDisPeriod(V558_SIZE size);
void	V558_LcdGetDisPeriod(V558_SIZE *size);
void	V558_LcdPanelReset(void);


void	V558_LcdSetBLSize(V558_SIZE Size);
void	V558_LcdGetBLSize(V558_SIZE *Size);
void	V558_LcdSetBLDisRect(V558_RECT Rect);
void	V558_LcdGetBLDisRect(V558_RECT *Rect);
void	V558_LcdSetBLDisPoint(V558_POINT DisPoint);
void	V558_LcdGetBLDisPoint(V558_POINT *DisPoint);
void	V558_LcdSetBLFormat(UINT8 format);
UINT8	V558_LcdGetBLFormat(void);
void	V558_LcdSetBLProperty(PV558_LAYER_PROPERTY proty);
void	V558_LcdGetBLProperty(V558_LAYER_PROPERTY *proty);
void	V558_LcdSetBLPaletteEntry(V558_COLOR palette, UINT8 index);
void	V558_LcdGetBLPaletteEntry(V558_COLOR *palette, UINT8 index);
void	V558_LcdSetBLPalette(PV558_COLOR palette, UINT16 total);
void	V558_LcdSetBLOverlayCtrl(UINT8 x);
UINT8	V558_LcdGetBLOverlayCtrl(void);
UINT8	V558_LcdEnableBLOverlay(void);
UINT8	V558_LcdDisableBLOverlay(void);
void	V558_LcdSetBLKeyColor(V558_COLOR keycolor);
void	V558_LcdGetBLKeyColor(V558_COLOR *keycolor);
UINT8	V558_LcdBLEnable(void);
UINT8	V558_LcdBLDisable(void);

void	V558_LcdSetBGColor(V558_COLOR color);
void	V558_LcdGetBGColor(V558_COLOR *color);
UINT8	V558_LcdBgEnable(void);
UINT8	V558_LcdBgDisable(void);

void	V558_LcdSetALSize(V558_SIZE Size);
void	V558_LcdGetALSize(V558_SIZE *Size);
void	V558_LcdSetALDisRect(V558_RECT Rect);
void	V558_LcdGetALDisRect(V558_RECT *Rect);
void	V558_LcdSetALDisPoint(V558_POINT DisPoint);
void	V558_LcdGetALDisPoint(V558_POINT *DisPoint);
void	V558_LcdSetALProperty(PV558_LAYER_PROPERTY proty);
void	V558_LcdGetALProperty(V558_LAYER_PROPERTY *proty);
void	V558_SubLcdSetALProperty(PV558_LAYER_PROPERTY proty);
void	V558_SubLcdGetALProperty(V558_LAYER_PROPERTY *proty);
void	V558_SubLcdSetALDisPoint(V558_POINT DisPoint);
void	V558_SubLcdGetALDisPoint(V558_POINT *DisPoint);
void	V558_LcdSetALOverlayCtrl(UINT8 x);
UINT8	V558_LcdGetALOverlayCtrl(void);
UINT8	V558_LcdEnableALOverlay(void);
UINT8	V558_LcdDisableALOverlay(void);
void	V558_LcdSetALKeyColor(V558_COLOR keycolor);
void	V558_LcdGetALKeyColor(V558_COLOR *keycolor);
void	V558_LcdSetALPriority(UINT8 flag);
UINT8	V558_LcdGetALPriority(void);
UINT8	V558_LcdALEnable(void);
UINT8	V558_LcdALDisable(void);

void	V558_LcdSetDispEn(UINT8 x);
UINT8	V558_LcdGetDispEn(void);
UINT8	V558_LcdDispEnable(void);
UINT8	V558_LcdDispDisable(void);
void	V558_LcdSetDispMode(UINT8 mode);
UINT8	V558_LcdGetDispMode(void);
void	V558_LcdSetOverlayMode(UINT8 mode);
UINT8	V558_LcdGetOverlayMode(void);
void	V558_LcdSetBlendMode(UINT8	mode);
UINT8	V558_LcdGetBlendMode(void);
UINT8	V558_LcdEnableBlend(void);
UINT8	V558_LcdDisableBlend(void);
void	V558_LcdSetBlendRatio(UINT8 ratio);
UINT8	V558_LcdGetBlendRatio(void);
UINT8	V558_LcdUpBlendRatio(UINT8 step);
UINT8	V558_LcdDownBlendRatio(UINT8 step);
UINT8	V558_LcdGetInterrupt(void);
void	V558_LcdUpdate(UINT8 flag);

void	V558_LcdSetIFControl(UINT16 x);

UINT16	V558_LcdGetIFControl(void);
UINT16	V558_LcdEnableGamma(void);
UINT16	V558_LcdDisableGamma(void);
UINT16	V558_LcdEnableDither(void);
UINT16	V558_LcdDisableDither(void);
void	V558_LcdSetDitherMode(UINT8 sel);
UINT8	V558_LcdGetDitherMode(void);
void	V558_LcdSetGammaR(UINT8 *x);
void	V558_LcdSetGammaG(UINT8 *x);
void	V558_LcdSetGammaB(UINT8 *x);
void	V558_LcdSetGammaTable(UINT8 *r, UINT8 *g, UINT8 *b);

void	V558_LcdGetGammaR(UINT8 *x);
void	V558_LcdGetGammaG(UINT8 *x);
void	V558_LcdGetGammaB(UINT8 *x);
void	V558_LcdGetGammaTable(UINT8 *r, UINT8 *g, UINT8 *b);

UINT16	V558_GetPanelWcy(void);
void	V558_SetPanelWcy(UINT16 x);
void    V558_LcdInitGammaTable(void);

UINT8	V558_GeGetStatus(void);
void	V558_GeStartup(void);
void	V558_GeSetOpmode(UINT8 sel);
UINT8	V558_GeGetOpmode(void);
void	V558_GeSetBitwise(UINT8 bitwise);
UINT8	V558_GeGetBitwise(void);
void	V558_GeSetColorDepth(UINT8 colordep);
UINT8	V558_GeGetColorDepth(void);

void	V558_GeSetPatternAlign(UINT8 align);
UINT8	V558_GeGetPatternAlign(void);
void	V558_GeSetPatternAddr(UINT16 addr);
UINT16	V558_GeGetPatternAddr(void);
void	V558_GeSetHostPattern(UINT8 bsel);
void	V558_GeSetMonoPattern(UINT8 bsel);
void	V558_GeSetPatternBgcolor(V558_COLOR color);
void	V558_GeGetPatternBgcolor(V558_COLOR *color);
void	V558_GeSetPatternFgcolor(V558_COLOR color);
void	V558_GeGetPatternFgcolor(V558_COLOR *color);

void	V558_GeSetSourceAddr(UINT16 adr);
UINT16	V558_GeGetSourceAddr(void);
void	V558_GeSetSourceSpan(UINT16 span);
UINT16	V558_GeGetSourceSpan(void);
void	V558_GeSetHostSource(UINT8 bsel);
void	V558_GeSetMonoSource(UINT8 bsel);
void	V558_GeSetSourceBgcolor(V558_COLOR color);
void	V558_GeGetSourceBgcolor(V558_COLOR *color);
void	V558_GeSetSourceFgcolor(V558_COLOR color);
void	V558_GeGetSourceFgcolor(V558_COLOR *color);

void	V558_GeSetDestAddr(UINT16 adr);
UINT16	V558_GeGetDestAddr(void);
void	V558_GeSetDestSpan(UINT16 span);
UINT16	V558_GeGetDestSpan(void);
void	V558_GeSetBitbltSize(V558_SIZE size);
void	V558_GeGetBitbltSize(V558_SIZE *size);

void	V558_GeSetLineStartp(V558_POINT ps);
void	V558_GeGetLineStartp(V558_POINT *ps);
void	V558_GeSetLineEndp(V558_POINT pe);
void	V558_GeGetLineEndp(V558_POINT *pe);
void	V558_GeSetLineColor(V558_COLOR color);
void	V558_GeGetLineColor(V558_COLOR *color);
void	V558_GeSetLineProty(V558_POINT ps, V558_POINT pe, V558_COLOR color);
void	V558_GeGetLineProty(V558_POINT *ps, V558_POINT *pe, V558_COLOR *color);

void	V558_GeSetDefaultJbuf(void);


//DOM-IGNORE-BEG
/********************************************************/
//   Base functions of SIF/ISP module                 //
/********************************************************/
//DOM-IGNORE-END

void V558_SifSetSensorMode(UINT8 sel);
UINT8 V558_SifGetSensorMode(void);
UINT8 V558_SifEnableSnrSyncGen(void); //return sensor mode
UINT8 V558_SifDisableSnrSyncGen(void); //return sensor mode
UINT8 V558_SifSetDataFormat(UINT8 uFormat);
void	V558_SifSetSensorOpmode(UINT8 sel);
UINT8	V558_SifGetSensorOpmode(void);
UINT8 V558_SifEnableSyncGen(void); //return op mode
UINT8 V558_SifDisableSyncGen(void); //return op mode
void V558_SifSetYuvFormat(UINT8 sel);
UINT8 V558_SifGetYuvFormat(void);
void V558_SifSetVsyncCtrl(UINT8 sel);
UINT8 V558_SifGetVsyncCtrl(void);
void V558_SifSetHsyncCtrl(UINT8 sel);
UINT8 V558_SifGetHsyncCtrl(void);
void V558_SifSetSnrclk(UINT8 sel);
UINT8 V558_SifGetSnrclk(void);
void V558_SifSetPixelRate(UINT8 sel);
UINT8 V558_SifGetPixelRate(void);
void V558_SifSetSerialclk(UINT8 sel);
UINT8 V558_SifGetSerialclk(void);
void V558_SifSetI2cclk(UINT16 sel);
UINT16 V558_SifGetI2cclk(void);
void	V558_SifSetHrefStart(UINT16 startx);
UINT16 V558_SifGetHrefStart(void);
void V558_SifSetHrefLength(UINT16 length);
UINT16 V558_SifGetHrefLength(void);
void	V558_SifSetVrefStart(UINT16 startx);
UINT16 V558_SifGetVrefStart(void);
void V558_SifSetVrefStop(UINT16 stop);
UINT16 V558_SifGetVrefStop(void);
void V558_SifSetI2cDevAddr(UINT8 adr);
UINT8 V558_SifGetI2cDevAddr(void);
void V558_SifSetAeWinStart(UINT16 start);
UINT16 V558_SifGetAeWinStart(void);
void V558_SifSetAeWinStop(UINT16 x);
UINT16 V558_SifGetAeWinStop(void);
void V558_SifI2cAeBatch(UINT8 uNum, V558_I2C_BATCH *AeData);
UINT8 V558_SifI2cWriteByte(UINT8 uAddr, UINT8 uVal);
UINT8 V558_SifI2cWriteWord(UINT8 uAddr, UINT16 uVal);
UINT8 V558_SifI2cWriteTriWord(UINT8 uAddr, UINT32 uVal);
UINT8 V558_SifI2cReadByte(UINT8 uAddr, UINT8 *uVal);
UINT8 V558_SifI2cReadWord(UINT8 uAddr, UINT16 *uVal);
UINT8 V558_SifI2cReadTriByte(UINT8 uAddr, UINT32 *uVal);

void V558_IspSetBasctl(UINT16 x);
UINT16 V558_IspGetBasctl(void);
void V558_IspEnableRgbGain(void);
void V558_IspDisableRgbGain(void);
void V558_IspEnableYGamma(void);
void V558_IspDisableYGamma(void);
void V558_IspEnableColorMatrix(void);
void V558_IspDisableColorMatrix(void);
void V558_IspEnableRgbGamma(void);
void V558_IspDisableRgbGamma(void);
void V558_IspEnableDpc(void);
void V558_IspDisableDpc(void);
void V558_IspEnableLensoff(void);
void V558_IspDisableLensoff(void);
void V558_IspEnableEdgeenh(void);
void V558_IspDisableEdgeenh(void);
void V558_IspEnableTestbar(void);
void V558_IspDisableTestbar(void);
void V558_IspEnableBarVideo(void);
void V558_IspDisableBarVideo(void);
void V558_IspSetPixelRate(UINT8 sel);
void V558_IspSetTestBarWin(V558_SIZE size);
void V558_IspGetTestBarWin(V558_SIZE *size);
void V558_IspSetImageSize(V558_SIZE size);
void V558_IspGetImageSize(V558_SIZE *size);
void V558_IspSetEdgeParm(PV558_EDGEPARM edge);
void V558_IspGetEdgeParm(V558_EDGEPARM *edge);
void V558_IspSetLensParm(PV558_LENSFALLOFF lens);
void V558_IspGetLensParm(V558_LENSFALLOFF *lens);
void V558_IspSetAflnctl(UINT16 ctl);
UINT16 V558_IspGetAflnctl(void);
void V558_IspSetAflst(UINT16 aflst);
UINT16 V558_IspGetAflst(void);
void V558_IspSetAflcnt(UINT8 cnt);
UINT8 V558_IspGetAflcnt(void);
void V558_IspEnableAflmode(void);
void V558_IspDisableAflmode(void);
void V558_IspSetAflstep(UINT8 step);
UINT8 V558_IspGetAflstep(void);
UINT32 V558_IspGetAfThreshold(void);
void V558_IspSetRgain(UINT8 x);
UINT8 V558_IspGetRgain(void);
void V558_IspSetGgain(UINT8 x);
UINT8 V558_IspGetGgain(void);
void V558_IspSetBgain(UINT8 x);
UINT8 V558_IspGetBgain(void);
void V558_IspSetGlbgain(UINT8 x);
UINT8 V558_IspGetGlbgain(void);
void V558_IspEnableGlbgain(void);
void V558_IspDisableGlbgain(void);
void V558_IspSetGlbgainDelay(UINT8 val);
UINT8 V558_IspGetGlbgainDelay(void);
void V558_IspSetGlbgainStep(UINT8 val);
UINT8 V558_IspGetGlbgainStep(void);
void V558_IspSetColorCvt(PV558_COLORCVT val);
void V558_IspGetColorCvt(V558_COLORCVT *val);
void V558_IspSetColorMatrix(PV558_COLORMATRIX x);
void V558_IspGetColorMatrix(V558_COLORMATRIX *x);
void V558_IspSetColorMatrixOff(PV558_COLORMATRIXOFF x);
void V558_IspGetColorMatrixOff(V558_COLORMATRIXOFF *x);
void V558_IspSetYGammaStart(UINT8 *x);
void V558_IspGetYGammaStart(UINT8 *x);
void V558_IspSetYGammaSlope(UINT8 *x);
void V558_IspGetYGammaSlope(UINT8 *x);
void V558_IspSetRGammaStart(UINT8 *x);
void V558_IspGetRGammaStart(UINT8 *x);
void V558_IspSetGGammaStart(UINT8 *x);
void V558_IspGetGGammaStart(UINT8 *x);
void V558_IspSetBGammaStart(UINT8 *x);
void V558_IspGetBGammaStart(UINT8 *x);
void V558_IspSetAfctrl(UINT16 x);
UINT16 V558_IspGetAfctrl(void);
void V558_IspEnableAwbOnewin(void);
void V558_IspDisableAwbOnewin(void);
void V558_IspEnableAwbReverse(void);
void V558_IspDisableAwbReverse(void);
void V558_IspEnableAwb(void);
void V558_IspDisableAwb(void);
void V558_IspSetAewmode(UINT8 mode);
UINT8 V558_IspGetAewmode(void);
void V558_IspEnableAeLargeWin(void);
void V558_IspDisableAeLargeWin(void);
void V558_IspEnableAeSmallYrange(void);
void V558_IspDisableAeSmallYrange(void);
void V558_IspSetAfIntfrq(UINT8 frq);
UINT8 V558_IspGetAfIntfrq(void);
void V558_IspEnableAntiFlk(void);
void V558_IspDisableAntiFlk(void);
void V558_IspSetAfWin(V558_SIZE size);
void V558_IspSetAeWeight(UINT16	x);
void V558_IspSetColorRange(PV558_AWBCLRRANGE x);
void V558_IspGetColorRange(V558_AWBCLRRANGE *x);
void V558_IspSetAwbGainLmt(V558_AWBGAINLMT x);
void V558_IspGetAwbGainLmt(V558_AWBGAINLMT *x);
void V558_IspSetAwbThreshold(UINT16 x);
UINT16 V558_IspGetAwbThreshold(void);
UINT8 V558_IspGetRMean(void);
UINT8 V558_IspGetGMean(void);
UINT8 V558_IspGetBMean(void);
UINT8 V558_IspGetYMean(void);
UINT8 V558_IspGetAfStatus(void);
void V558_IspSetDpdThreshold(UINT8 x);
UINT8 V558_IspGetDpdThreshold(void);
void V558_IspSetNoiseTable(UINT8 *x);
void V558_IspGetNoiseTable(UINT8 *x);


//DOM-IGNORE-BEG
/********************************************************/
//   Base functions of IPP module                       //
/********************************************************/
//DOM-IGNORE-END

void V558_IppSetSpeCtrl(UINT8 sel);
UINT8 V558_IppGetSpeCtrl(void);
UINT8 V558_IppEnableSpecial(void);
void V558_IppSetPixelRate(UINT8 x);
UINT8 V558_IppGetPixelRate(void);
void V558_IppSetSizerCtrl(UINT8 x);
UINT8 V558_IppGetSizerCtrl(void);
UINT8 V558_IppEnableDispSizer(void);
UINT8 V558_IppDisableDispSizer(void);
UINT8 V558_IppEnableCapSizer(void);
UINT8 V558_IppDisableCapSizer(void);
UINT8 V558_IppEnablePrefilter(void);
UINT8 V558_IppDisablePrefilter(void);
void V558_IppSetImageSize(V558_SIZE size);
void V558_IppGetImageSize(V558_SIZE *size);
void V558_IppSetImageWidth(UINT16 x);
UINT16 V558_IppGetImageWidth(void);
void V558_IppSetImageHeight(UINT16 x);
UINT16 V558_IppGetImageHeight(void);
void V558_IppSetSizerSize(V558_SIZE size);
void V558_IppGetSizerSize(V558_SIZE *size);
void V558_IppSetSizerWidth(UINT16 x);
UINT16 V558_IppGetSizerWidth(void);
void V558_IppSetSizerHeight(UINT16 x);
UINT16 V558_IppGetSizerHeight(void);
void V558_IppSetSizerStartp(V558_POINT p);
void V558_IppGetSizerStartp(V558_POINT *p);
void V558_IppSetSizerStartx(UINT16 x);
UINT16 V558_IppGetSizerStartx(void);
void V558_IppSetSizerStarty(UINT16 y);
UINT16 V558_IppGetSizerStarty(void);
void V558_IppSetFirCoef(UINT8 *x);
void V558_IppGetFirCoef(UINT8 *x);
void V558_IppSetCapFactor(UINT8 x, UINT8 y);
void V558_IppGetCapFactor(UINT8 *x, UINT8 *y);
void V558_IppSetDispFactor(UINT8 src, UINT8 dst);
void V558_IppGetDispFactor(UINT8 *src, UINT8 *dst);
void V558_IppSetCapSize(V558_SIZE size);
void V558_IppGetCapSize(V558_SIZE *size);
void V558_IppSetCapWidth(UINT16 x);
UINT16 V558_IppGetCapWidth(void);
void V558_IppSetCapHeight(UINT16 x);
UINT16 V558_IppGetCapHeight(void);
void V558_IppSetDispSize(V558_SIZE size);
void V558_IppGetDispSize(V558_SIZE *size);
void V558_IppSetDispWidth(UINT16 x);
UINT16 V558_IppGetDispWidth(void);
void V558_IppSetDispHeight(UINT16 x);
UINT16 V558_IppGetDispHeight(void);
void V558_IppSetYoffset(UINT8	x);
UINT8 V558_IppGetYoffset(void);
void V558_IppSetUoffset(UINT8	x);
UINT8 V558_IppGetUoffset(void);
void V558_IppSetVoffset(UINT8	x);
UINT8 V558_IppGetVoffset(void);
void V558_IppSetUTopThrd(UINT8	x);
UINT8 V558_IppGetUTopThrd(void);
void V558_IppSetUBottomThrd(UINT8	x);
UINT8 V558_IppGetUBottomThrd(void);
void V558_IppSetVTopThrd(UINT8	x);
UINT8 V558_IppGetVTopThrd(void);
void V558_IppSetVBottomThrd(UINT8	x);
UINT8 V558_IppGetVBottomThrd(void);
void V558_IppSetFrameDrop(UINT16	x);
UINT16 V558_IppGetFrameDrop(void);


//DOM-IGNORE-BEG
/********************************************************/
//   Base functions of JPEG module                      //
/********************************************************/
//DOM-IGNORE-END

void V558_JpegSetWorkMode(UINT8 x);
UINT8 V558_JpegGetWorkMode(void);
UINT8 V558_JpegSimpHeadEnable(void);
UINT8 V558_JpegSimpHeadDisable(void);
UINT8 V558_JpegCustQuantTableEnable(void);
UINT8 V558_JpegCustQuantTableDisable(void);
UINT8 V558_JpegSameQuantTableEnable(void);
UINT8 V558_JpegSameQuantTableDisable(void);
void V558_JpegSetControl(UINT8 x);
UINT8 V558_JpegGetControl(void);

#if 0
UINT8 V558_JpegStartEncoder(void)	;
UINT8 V558_JpegStartDecoder(void)	;
UINT8 V558_JpegRestartDecoder(void)	;
UINT8 V558_JpegEnterIdle(void);
UINT8 V558_JpegStartWrQTable(void);
UINT8 V558_JpegStartWrHTable(void);
void V558_JpegSelectHTable(UINT8 x);
#else
void V558_JpegStartEncoder(void)	;
void V558_JpegStartDecoder(void)	;
void V558_JpegRestartDecoder(void)	;
void V558_JpegEnterIdle(void);
void V558_JpegStartWrQTable(void);
void V558_JpegStartWrHTable(void);
void V558_JpegSelectHTable(UINT8 x);
#endif

UINT8 V558_JpegGetStatus(void);
void V558_JpegSetBRC(UINT8	x);
UINT8 V558_JpegGetBRC(void);
UINT8	V558_JpegEnableAutoBRC(void);
UINT8	V558_JpegDisableAutoBRC(void);
void V558_JpegSetTCR(UINT8	x);
UINT8 V558_JpegGetTCR(void);
UINT8 V558_JpegGetQfactor(void);
void V558_JpegSetTWC(UINT32	x);
UINT32 V558_JpegGetTWC(void);
void V558_JpegSetVWC(UINT32	x);
UINT32 V558_JpegGetVWC(void);
UINT32 V558_JpegGetHuffmanWC(void);
void V558_JpegSetImageWidth(UINT16	x);
UINT16 V558_JpegGetImageWidth(void);
void V558_JpegSetImageHeight(UINT16	x);
UINT16 V558_JpegGetImageHeight(void);
void V558_JpegSetImageSize(V558_SIZE size);
void V558_JpegGetImageSize(V558_SIZE *size);
UINT16 V558_JpegGetFrameCnt(void);
void V558_JpegWriteDctData(UINT8 x);
void V558_JpegWriteQtable(UINT8 x);
void V558_JpegWriteHtable(UINT8 x);
void V558_JpegSetComponentNum(UINT8 x);
UINT8 V558_JpegGetComponentNum(void);
void V558_JpegSetComponentParm(UINT8 sel, UINT16 x);
UINT16 V558_JpegGetComponentParm(UINT8 sel);
void V558_JpegSetBlockNum(UINT8 x);
UINT8 V558_JpegGetBlockNum(void);
void V558_JpegSetImageParm(UINT8 count, UINT8 *x);


//DOM-IGNORE-BEG
/********************************************************/
//   Base functions of LBUF module                      //
/********************************************************/
//DOM-IGNORE-END

UINT8 V558_LbufGetStatus(void);
void V558_LbufSetYuvMode(UINT8 x);
UINT8 V558_LbufGetYuvMode(void);
void V558_LbufSetWorkmode(UINT8 x);
UINT8 V558_LbufGetWorkmode(void);
void V558_LbufSetImageWidth(UINT16 x);
UINT16 V558_LbufGetImageWidth(void);
void V558_LbufSetImageHeight(UINT16 x);
UINT16 V558_LbufGetImageHeight(void);
void V558_LbufSetImageSize(V558_SIZE size);
void V558_LbufGetImageSize(V558_SIZE *size);


//DOM-IGNORE-BEG
/********************************************************/
//   Base functions of JBUF module                      //
/********************************************************/
//DOM-IGNORE-END

void V558_JbufSetSourceAddr(UINT16 x);
UINT16 V558_JbufGetSourceAddr(void);
void V558_JbufSetSourceSize(UINT16 x);
UINT16 V558_JbufGetSourceSize(void);
void V558_JbufSetThumbAddr(UINT16 x);
UINT16 V558_JbufGetThumbAddr(void);
void V558_JbufSetThumbSize(UINT16 x);
UINT16 V558_JbufGetThumbSize(void);
void V558_JbufSetDestAddr(UINT16 x);
UINT16 V558_JbufGetDestAddr(void);
void V558_JbufSetDestSize(UINT16 x);
UINT16 V558_JbufGetDestSize(void);
void V558_JbufSetJpegAddr(UINT16 x);
UINT16 V558_JbufGetJpegAddr(void);
void V558_JbufSetJpegSize(UINT16 x);
UINT16 V558_JbufGetJpegSize(void);
UINT16 V558_JbufGetBiuAccLength(void);
void V558_JbufSetCapViewConf(UINT8 x);
UINT16 V558_JbufGetWritePointer(void);
UINT16 V558_JbufGetReadPointer(void);


//DOM-IGNORE-BEG
/********************************************************/
//   Base functions of SWGP module                    //
/********************************************************/
//DOM-IGNORE-END

void V558_GpDoHMirror(UINT8* pOut, UINT8* pIn, PV558_SIZE pinsize);
void V558_GpDoVMirror(UINT8* pOut, UINT8* pIn, PV558_SIZE pinsize);
void V558_GpAdjustLum(UINT8* pOut, UINT8* pIn, PV558_SIZE pinsize, SINT8 step);
void V558_GpSetGray(UINT8* pOut, UINT8* pIn, PV558_SIZE pinsize);
void V558_GpDoNegative(UINT8* pOut, UINT8* pIn, PV558_SIZE pinsize);
UINT8 V558_GpDoZoom(UINT8* pOut, UINT8* pIn, PV558_SIZE poutsize, PV558_SIZE pInSize);
UINT8 V558_GpDoRotation(UINT8* pOut, UINT8* pIn, PV558_SIZE poutsize, PV558_SIZE pinsize, UINT8 degree);
UINT8 V558_GpAdjustContrast(UINT8* pOut, UINT8* pIn, PV558_SIZE pinsize, SINT8 grad);
UINT8 V558_GpSetColorRange(UINT8* pOut, UINT8* pIn, PV558_SIZE pinsize, UINT8 utop, UINT8 ubottom, UINT8 vtop, UINT8 vbottom);
UINT8 V558_GpSetSepia(UINT8* pOut, UINT8* pIn, PV558_SIZE pinsize, UINT8 r, UINT8 g, UINT8 b);
UINT8 V558_GpSetFrame(UINT8* pOut, UINT8* pIn, UINT8* pInFrame , PV558_SIZE pinsize , UINT8 keycolor);

/************************************************************/
UINT8 V558_IsThumb(PV558_JPEGBUF pbuf, PV558_JPEG_INDEX pinfo);
UINT32  V558_CreateJFIF(UINT8* pOut, UINT8* pjpegin, UINT32 jpeglen, UINT8* pthumbin, UINT32 yuvthumblen);
#ifdef __cplusplus
}
#endif



#endif // _V558API_H_

