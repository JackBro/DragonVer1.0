


#ifndef _RDK_V5TYPE_H_
#define _RDK_V5TYPE_H_


#include "../tools/trmtype.h"

//==========================
//
//		VMARB module
//
//============================

	enum CHIPWORKMODEenum {
		PREVIEW_MODE,
		PREVIEWBRC_MODE,
		PREVIEWFRAME_MODE,

		CAPTURESTILL_MODE,
		CAPTURESTILLTHUMB_MODE,

		CAPTUREVIDEO_MODE,
		CAPTUREVIDEOTHUMB_MODE,
		CAPTUREAVI_MODE,	//include header and index
		CAPTUREAVITHUMB_MODE,

		CAPTUREFRAME_MODE,

		CAPTUREFRAMEVIDEO_MODE,
		CAPTUREFRAMEAVI_MODE,	//include header and index

		MULTISHOT_MODE,
		MULTISHOTTHUMB_MODE,

		DISPLAYSTILL_MODE,

		ENCODE_MODE,

		DECODE_MODE,
		DECODEIPP_MODE,
		DECODEFRAME_MODE,

		MULTISHOTFRAME_MODE,
		CAPTURETHUMB_MODE,
		DISPLAYVIDEO_MODE,
		DISPLAYAVI_MODE,
	
		DIRECTDISPLAY_MODE,
		BYPASS_MODE,
		THROUGH_MODE,

		DECODEIPPBYPASS_MODE,
		DECODEFRAMEBYPASS_MODE,
		DISPLAYBYPASS_MODE,

		CAPTUREFRAMETHUMB_MODE,

		CAPTUREFRAMEVIDEOTHUMB_MODE,
		CAPTUREFRAMEAVITHUMB_MODE	//include header and index
	};

	enum LAYERAFMTORDERenum {	//selection for none-gbuf panel
		LINEBUF_ONLY,
		ONEFRAME_ONLY,
		TWOFRAME_ONLY
	};

	//Marb int
	#define V5_MARB_INT_NUM 8 //maybe 16 in future
	enum MARB_INT_TYPEenum
	{
		INT_JBUF_FIFOCNT		= 0,
		INT_JBUF_INTV			= 1, //ignore
		INT_JPEG_DONE			= 2,
		INT_JBUF_ERR			= 3,
		INT_TBUF_FIFOCNT		= 4,
		INT_TBUF_INTV			= 5, //ignore
		INT_THUMB_DONE			= 6,
		INT_THUMB_ERR			= 7,
		MARB_INT_ALL			= V5_MARB_INT_NUM
	};

	//Marb slave int
	#define V5_MARB_SLV_INT_NUM 5 //maybe 16 in future
	enum MARB_SLV_INT_TYPEenum
	{
		INT_MEM0_REF_ERR		= 0,
		INT_MEM1_REF_ERR		= 1, //ignore
		INT_MEM2_REF_ERR		= 2,
		INT_RECAP_ERR			= 3,
		INT_RECAP				= 4,
		MARB_SLV_INT_ALL		= V5_MARB_SLV_INT_NUM
	};

	enum MARB_POWER_SAVEenum
	{
		MARB_POWER_SAVE_IO		= 0,
		MARB_POWER_SAVE_CLK		= 1
	};

	#define DEFAULT_ALORDER		TWOFRAME_ONLY
	#define DEFAULT_INDEXSIZE	0x400
	#define	DEFAULT_FRMINTVAL	4
	#define DEFAULT_GAPRATIO	1
	#define MAX_GAPRATIO		8

	typedef struct tag_TMarbInfo {
		UINT32  IndexSize; //Avi index memory size
		UINT8	allocmode; //Memory alloc mode
		UINT8	thumbcnt; //Muti thumb count.
		UINT8	gapratio; //fifo count interrupt occur data size/ memory size
		UINT8	pwrsave; //power save mode MARB_POWER_SAVEenum
	} TMarbInfo, *PTMarbInfo;

	typedef struct tag_TMarbMap {
		UINT32	layerB0start, layerB0end;		//0, 1
		UINT32	layerB1start, layerB1end;		//2, 3
		UINT32	jbufstart, jbufend;				//4, 5
		UINT32	lbuf0start;						//6
		UINT32	lbuf1start;						//7
		UINT32  lbufend;						//8
		UINT32	thumbstart, thumbend;			//9, 10
		UINT32	indexstart, indexend;			//11, 12
		UINT32	layerA0start, layerA0end;		//13, 14
		UINT32	layerA1start, layerA1end;		//15, 16
	} TMarbMap, *PTMarbMap;

	typedef void (* PVMarbIsrCallback)(void);

	typedef struct tag_TMarbContext {
		UINT8	state;
		UINT8	mode;
		//PTMarbInfo pinputinfo;
		TMarbInfo setting;
		UINT32	jbufpos;
		UINT32	thumbpos;
		UINT32	sramstart;
		UINT32	sramend;
//		UINT32	thumbsize;
		TMarbMap map;
		PVMarbIsrCallback marbintisr[V5_MARB_INT_NUM];
		PVMarbIsrCallback marbslvintisr[V5_MARB_SLV_INT_NUM];
	} TMarbContext, *PTMarbContext;	//size = 92B


//==========================
//
//		VCTRL module
//
//============================

	//software reset
	enum SW_RESETenum
	{
		//lcdif
		SW_RESET_LCDIF			= BIT0,
		//lcdc
		SW_RESET_LCDC			= BIT1,
		//marb
		SW_RESET_MARB			= BIT2,
		//ge
		SW_RESET_GE				= BIT3,
		//jpeg
		SW_RESET_JPEG			= BIT4,
		//lbuf
		SW_RESET_LBUF			= BIT5,
		//ipp
		SW_RESET_IPP			= BIT6,
		//sensor interface
		SW_RESET_SIF			= BIT7,
		//isp
		SW_RESET_ISP			= BIT8,
		//global
		SW_RESET_GLB			= 0xffff
	};

	//module clock
	enum MOD_CLKenum
	{
		//cpm
		MOD_CLK_CPM			=	BIT0,
		//lcdif
		MOD_CLK_LCDIF		=	BIT1,
		//lcdc
		MOD_CLK_LCDC		=	BIT2,
		//marb fast clock
		MOD_CLK_MARB_FAST	=	BIT3,
		//marb
		MOD_CLK_MARB		=	BIT4,
		//ge
		MOD_CLK_GE			=	BIT5,
		//jpeg core clock
		MOD_CLK_JPEG_CORE	=	BIT6,
		//jpeg
		MOD_CLK_JPEG		=	BIT7,
		//lbuf
		MOD_CLK_LBUF		=	BIT8,
		//ipp
		MOD_CLK_IPP			=	BIT9,
		//isp
		MOD_CLK_ISP			=	BIT10,
		//sif
		MOD_CLK_SIF			=	BIT11,
		//all module clock
		MOD_CLK_ALL			=	0xffff
	};

	typedef void (*PV5IsrCallback)(UINT16 intflag);

	// Chip control information
	typedef struct tag_TCtrlInfo {
		UINT8	bustype; //Bus type
		UINT8	intprop; //Interrupt control: lever or edge, highor low, width
		UINT8	clkcfgtype; //0-(clk use vctrl api) 1-(clk use user set para)
		UINT32	clkin; //Input clock
		UINT32	mclk; //Chip work clock
		UINT8	pll_m; //Pll m(must > 2)
		UINT8	pll_n; //Pll n(must > 2)
		UINT8	pll_od0; //Pll od0
		UINT8	pll_od1; //Pll od1
		UINT8	clkoutdiv; //Clk output div
		UINT8   bypassRegAddr;//bypass register address
		UINT8	dlycell[4]; //dlycell:[0] Addr, [1]: DataOut, [2]: DataIn, [3]: DataOen	
	} TCtrlInfo, *PTCtrlInfo;

	//interrupt number
	#define V5_INT_NUM 8

	//interruput
	enum INT_TYPEenum
	{
		//SIF
		INT_SIF		= 0,
		//ISP
		INT_ISP		= 1,
		//LBUF
		INT_LBUF	= 2,
		//JPEG
		INT_JPEG	= 3,
		//GE
		INT_GE		= 4,
		//MARB0
		INT_MARB0	= 5,
		//LCDC
		INT_LCDC	= 6,
		//MARB1
		INT_MARB1	= 7,

		//all interrupt
		INT_ALL		= V5_INT_NUM
	};

	typedef struct tag_TCtrlContext {
		UINT8		               state;
		UINT8		               mode;
		UINT16		               intoccurnum;
		UINT8		               intoccurtype[V5_INT_NUM];
		UINT16		               intoccurflag[V5_INT_NUM];
		PV5IsrCallback             intisr[V5_INT_NUM];
		UINT32	                   clkin; //Input clock
		UINT32	                   mclk; //Chip work clock
		PTCtrlInfo	               pinputinfo;
	} TCtrlContext, *PTCtrlContext;

//==========================
//
//		VDISP module
//
//============================

	enum PANELSELenum
	{
		PANEL_MASTER,
		PANEL_SLAVE
	};

	enum PANELTYPEenum
	{
		PANEL_DITHER444,
		PANEL_DITHER565,
		PANEL_DITHER666,
		PANEL_DITHERMASK = BIT2,
		PANEL_DITHEREN	= BIT3,
		PANEL_GAMMAEN	= BIT4
	};

	//lcdif config
	enum V5B_LCDIF_CFGenum
	{
		//select the main lcd or sub lcd
		V5B_LCDIF_CS1_SEL						= BIT4,
		//select the 68 serial bus
		V5B_LCDIF_68BUS_SEL						= BIT5
	};

	enum V5B_LCDIF_CSCFGenum
	{
		V5B_LCDIF_CSFLAG_OFF,
		V5B_LCDIF_CSFLAG_ON
	};

	enum PANELWRITEINFO
	{
		RS_HIGH		= BIT0,
		HIGHBYTE	= BIT1,
		REG_16		= BIT2,
		MULTI_16	= BIT3
	};

	enum DISPGELAYER
	{
		GE_B0,		//layer B0
		GE_B1,		//layer B1
		GE_RES1,		//reserved 1
		GE_RES2		//reserved 2
	};

	enum PANELSTATEenum
	{
		PANEL_POWEROFF,
		PANEL_POWERON,
		PANEL_STANDBY,
		PANEL_SLEEP,
		PANEL_WAKEUPFROMSTANDBY,
		PANEL_WAKEUPFSLEEP
	};

	enum DISPMODEenum
	{
		DISP_IDLE_MODE,	//LCD module is not working.
		DISP_ABL_MODE,	//Both layer A and B are enable
		DISP_DEF_MODE	//Decode frame mode
	};

	enum DISP_LCDC_INT_TYPEenum
	{
		INT_LCDC_LAYERB_DONE,
		INT_LCDC_LAYERBG_DONE,
		INT_LCDC_FIFO_WARNING,
		INT_LCDC_VB_TIMEOUT,
		INT_LCDC_GB_TIMEOUT,
		INT_LCDC_ALL
	};

	typedef void (*PPanelCallback)(void);

	typedef struct tag_TALayerProty
	{
		TSize	memsize;
		TRect   win;
		TPoint	pos2panel;
	} TALayerProty, *PTALayerProty;

	typedef struct tag_TBLayerProty
	{
		TSize	memsize;
		TRect   win;
		TPoint	pos2panel;
		TRect	refwin;
	} TBLayerProty, *PTBLayerProty;

	typedef struct tag_TLcdcParm
	{
		TSize	size;
		UINT8	prop;		//panel has gbuf, dither en, gamma en
		UINT8	gamma[17];	/* gamma table relation to panel's gamma */
	} TLcdcParm, *PTLcdcParm;

	typedef struct tag_TLcdifParm
	{
		UINT16	csmode;
		UINT16	csflag;
		UINT16	rsflag;
		UINT16	headnum;
		UINT16	head[16];
		UINT16	stxp, styp, endxp, endyp;
		UINT8	config;
		UINT8	fmt;
		UINT16	wcy, rcy;
		TPoint	biaspt;
		UINT8	proccfg;
		UINT16	cmd_sx, cmd_ex, cmd_sy, cmd_ey;
		UINT16	rpt_sx, rpt_ex, rpt_sy, rpt_ey;
	} TLcdifParm, *PTLcdifParm;

	typedef struct tag_TPanelParm
	{
		UINT8			type;
		PTLcdifParm		lcdif;
		PTLcdcParm		lcdc;
	} TPanelParm, *PTPanelParm;

	typedef struct tag_TPanelInfo
	{
		PTPanelParm	master;
		PTPanelParm	slave;
	} TPanelInfo, *PTPanelInfo;

	typedef struct tag_TPanelAccess
	{
		TRegGroup		poweron;
		TRegGroup		poweroff;
		TRegGroup		sleep;
		TRegGroup		standby;
		TRegGroup		wakeupstb;
		TRegGroup		wakeup;
	} TPanelAccess, *PTPanelAccess;

	typedef struct tag_TPanelCfg
	{
		PTPanelAccess		pMainCfg;
		PTPanelAccess		pSubCfg;
		PPanelCallback		pBacklightOnEx;		//extra backlight on function
	} TPanelCfg, *PTPanelCfg;

	typedef struct tag_TDispContext
	{
		UINT8 state;
		PTPanelInfo pinputinfo;
		PTPanelParm pcur;
		UINT8		panelsel;
		UINT8		bsel;
		UINT8		fmt[4];
		UINT16		gespan[4];
		UINT32		adr[4];
		TSize		bmem[2];
		UINT8		mode;
		UINT8		brightness;
		UINT8		contrast;
		UINT8		dstbsel;
		UINT8		srcbsel;
	} TDispContext, *PTDispContext;

	typedef struct tag_TWinPos {
		TPoint ps;
		TSize  size;
	} TWinPos, *PTWinPos;

//==========================
//
//		VISP module
//
//============================

	enum VISPWORKMODEenum
	{
		VISP_PREVIEWFRAME_MODE,
		VISP_PREVIEW_MODE,
		VISP_PREVIEWBRC_MODE,
		VISP_CAPTURE_MODE,
		VISP_CAPTURETHUMB_MODE,
		VISP_CAPTUREFRAME_MODE,
		VISP_DISPLAY_MODE,
		VISP_ENCODE_DECODE_MODE,
		VISP_DECODEIPP_MODE,
		VISP_DECODEFRAME_MODE,
		VISP_NOSTREAM_MODE
	};

	#define V5_VISP_INT_NUM	3
	#define V5_SIF_INT_NUM	2

	#define V5_ISP_MAX_BRIGHT_CONTRAST	64		//algorithmic require
	#define V5_ISP_MIN_BRIGHT_CONTRAST	1
	#define V5_ISP_MAX_SATURATION		64
	#define V5_ISP_MIN_SATURATION		0

	enum VISP_ISP_INT_TYPEenum
	{
		INT_ISP_Y_MEAN		= 0,
		INT_ISP_ANTIF		= 1,
		INT_ISP_AUTO_FOC	= 2,
		INT_ISP_ALL			= V5_VISP_INT_NUM
	};

	enum SENSORYUVFMTenum
	{
		SNR_UYVY,
		SNR_VYUY,
		SNR_YUYV,
		SNR_YVYU
	};

	enum SENSORSTATEenum
	{
		SENSOR_NOINIT,
		SENSOR_POWERON,
		SENSOR_POWEROFF,
		SENSOR_STANDBY,
		SENSOR_SLEEP
	};

	enum VISPI2CMODEenum
	{
		I2C_NORMAL	= 0,
		I2C_PUP		= 0x18,
		I2C_PDOWN	= 0x08
	};

	//light frequence type
	typedef enum _FREQ_TYPEenum
	{
		FREQ_50HZ =		0x0,
		FREQ_60HZ =		0x1,
		FREQ_NONE =		0x2,		//outdoor
		FREQ_NOTSURE =	0x3
	}FREQ_TYPEenum;

	typedef void	(*PSensorSetReg)(UINT8 uAddr, UINT16 uVal);
	typedef void	(*PSensorGetReg)(UINT8 uAddr, UINT16 *uVal);
	typedef UINT8	(*PSnrIdChk)(void);
	typedef void	(*PSnrMirrorFlip)(UINT8 val);
	typedef void	(*PSnrSwitchSize)(UINT8 index, UINT32 et, UINT8 gain);
	typedef void	(*PSensorSetET)(UINT8 freqtype, UINT32 t, TSize size, UINT32 snrclock);
	typedef void	(*PVispIsrCallback)(void);

	typedef struct tag_THistStat
	{
		UINT8		x_cur;
		UINT8		x_avg;
		UINT8		buf[32];
	} THistStat, *PTHistStat;

	typedef struct tag_TSnrSizeCfg
	{
		TRegGroup	snr;
		TRegGroup	visp;
		TSize		size;
		UINT8		maxet;
		UINT8		snrclkratio;		//SNR CLK = MCLK / ratio
		UINT8		snrpixelratio;		//SNR Pixel freq = SNR CLK / pixel ratio
	} TSnrSizeCfg, *PTSnrSizeCfg;

	typedef struct tag_TTSnrSizeCfgGroup
	{
		UINT8 len;
		TSnrSizeCfg *pSizeCfg;
	} TSnrSizeCfgGroup, *PTSnrSizeCfgGroup;

	typedef struct tag_TAeParm
	{
		UINT8	ytarget;
		UINT8	ythreshold;
		UINT8	ytop;
		UINT8	gtarget;						//gain target
		UINT8	gthreshold;						//gain threshold
		UINT8	gdelay;
		UINT8	gain;
		UINT8	minet;							//if minet=0,et can decrease as it shoule be.if minet=1,it means 
		UINT8	maxgain;
		UINT8	mingain;
		UINT8	speed;							//adjust ae speed 0,1,2 
	} TAeParm, *PTAeParm;

	typedef struct tag_TFlashParm
	{
		UINT8 yflashthd;
		UINT8 flashstep;
	} TFlashParm, *PTFlashParm;

	typedef struct tag_THistParm
	{
		UINT8	ratio;							//percent of whole frame pixel, 1 is recommend.
	} THistParm, *PTHistParm;

	typedef struct tag_TSensorInfo
	{
		char				desc[40];	

		UINT8				snrtype;			//sensor type(RGB or YUV)
		UINT8				pclk;				//use PCLK of sensor
		UINT16				pclkval;			//snr pixel clock value : KHz				
		UINT8				clkcfg;				//[0]: VSync delay enable; [1]: Fifo VSync delay enable; [2]: Use PCLK Negtive Edge.
		UINT8				bustype;			//[0]: 0 -- I2C, 1 -- Serial bus;
		UINT8				i2ctype;			//[0]: 1 -- OV I2C bus
		UINT16				i2crate;			//I2C rate : KHz				
		UINT8				i2caddress;			//I2C address
		UINT8				i2cispaddress;		//ISP I2C address
		UINT8				pwrcfg;				//sensor power initial configure(SIF REG801 BIT[0]~BIT[2])
		UINT8				snrrst;				//Reset sensor enable

		UINT8				brightness;			//brightness
		UINT8				contrast;			//contrast
		UINT8				saturation;			//saturation
		UINT8				gammaRGB[17];		// RGB gamma value array

		TRegGroup			poweroff;			//sensor poweroff register
		TRegGroup			standby;			//sensor standby register
		TRegGroup			sleep;				//sensor sleep register
		TRegGroup			snrisp;				//SENSOR ISP initial configure

		TRegGroup			sifpwronseq;		//Sif config sequence(Reg.800 bit0~2) when sensor power on
		TRegGroup			sifstdbyseq;		//Sif config sequence(Reg.800 bit0~2) when sensor standby
		TRegGroup			dsif;				//SIF initial configure
		TRegGroup			disp;				//ISP initial configure
		TSnrSizeCfgGroup	snrSizeCfg;			//sensor size configure information

		PSensorSetReg		snrSetRegCall;		//set reg callback
		PSensorGetReg		snrGetRegCall;
		PSnrIdChk			snrIdChkCall;
		PSnrMirrorFlip		snrMirrorFlipCall;
		PSnrSwitchSize		snrSwitchSizeCall;
		PSensorSetET		snrSetETCall;		//set et callback

		TAeParm				aeparm;
		TFlashParm			flashparm;
		THistParm			histparm;
	} TSnrInfo, *PTSnrInfo;

	typedef struct tag_TVispContext
	{
		UINT8		valid;
		UINT8		snrstate;
		UINT8		i2cmode;
		PTSnrInfo	pinfo;

		UINT8		mode;
		UINT8		index;					//current image index
		UINT8		brightness;				//brightness
		UINT8		contrast;				//contrast
		UINT8		saturation;				//saturation
	#if 0
		UINT8		saturation;
		UINT8		hue;
	#endif
		UINT8		enfliker;
		TAeAdjust	ae;	
		UINT8		enae;
		UINT8		enflash;
		TFlashParm	flashparm;
		UINT8		env;			
		PVispIsrCallback vispisr[V5_VISP_INT_NUM];
		PVispIsrCallback sifisr[V5_SIF_INT_NUM];
	} TVispContext, *PTVispContext;


//==========================
//
//		VJPEG module
//
//============================

	enum JPEGMODEenum {
		JPEG_MODE_IDLE				,	//Jpeg module doesn't work.
		JPEG_MODE_CAPTURE			,	//Jpeg capture
		JPEG_MODE_CAPTUREFRAME		,	//Jpeg capture with frame
		JPEG_MODE_ENCODE			,	//Jpeg encode
		JPEG_MODE_DISPLAY			,	//Jpeg display
		JPEG_MODE_DISPLAY_BYPASS	,	//Jpeg display bypass
		JPEG_MODE_DECODE			,	//Jpeg decode
		JPEG_MODE_DECODEIPP			,	//Jpeg decode ipp
		JPEG_MODE_DECODEFRAME		,	//Jpeg decode with frame
		JPEG_MODE_DECODEIPP_BYPASS	,	//Jpeg decode ipp bypass
		JPEG_MODE_DECODEFRAME_BYPASS	//Jpeg decode with fram bypass
	};

	//jpeg header type
	enum JPEG_HEADERenum
	{
		//FFD8
		JPEG_HEADER_FFD8					= 0x0,
		//FFFE
		JPEG_HEADER_FFFE					= 0x1
	};

	//interrupt number
	#define V5_JPEG_JPEG_INT_NUM 3 //maybe 16 in future
	#define V5_JPEG_LBUF_INT_NUM 1 //maybe 16 in future

	//JPEG jpeg int
	enum JPEG_JPEG_INT_TYPEenum
	{
		INT_JPEG_ENC_DONE		= 0,
		INT_JPEG_DEC_DONE		= 1,
		INT_JPEG_DEC_ERR		= 2,
		JPEG_JPEG_INT_ALL		= V5_JPEG_JPEG_INT_NUM
	};

	//JPEG lbuf int
	enum JPEG_LBUF_INT_TYPEenum
	{
		INT_LBUF_READ			= 0,
		JPEG_LBUF_INT_ALL		= V5_JPEG_LBUF_INT_NUM
	};


	typedef void (* PVJpegIsrCallback)(void);

	//jpeg control
	enum JPEGBRCCTRLenum {
		BRC_EN			 =	BIT0, //Brc enable
		BRC_RUDE_EN		 =	BIT1, //Rude brc enable
		BRC_RUDE_UV_ONLY =	BIT2, //Rude brc yuv only
		BRC_ENH_EN		 =	BIT3  //Enhanced brc enable
	};


	typedef struct tag_TJpegEnhBrc {
		UINT8		ThdR1;
		UINT8		ThdR2;
		UINT8		ThdL1;
		UINT8		ThdL2;
		UINT8		ShfR1;
		UINT8		ShfR2;
		UINT8		ShfL1;
		UINT8		ShfL2;
	} TJpegEnhBrc, *PTJpegEnhBrc;

	typedef struct tag_TJpegRudeBrc {
		UINT8		CompLevel[6];
		UINT8		RudeAc;
		UINT8		RudeLevel;
		UINT32		ThdWc;
	} TJpegRudeBrc, *PTJpegRudeBrc;

	typedef struct tag_TJpegCapDly {
		UINT8		en;
		UINT8		feedval;
	} TJpegCapDly, *PTJpegCapDly;

	typedef struct tag_TJpegInfo {
		UINT8			ctrl; //BIT0: brc enable,BIT1:tcr enable
		UINT8			qf; //qualitify factor
		UINT8			tcr; //target compress rate, must >= 5
		TJpegRudeBrc	rudebrc;
		TJpegEnhBrc		enhbrc;
		TJpegCapDly		capdly;
	} TJpegInfo, *PTJpegInfo;

	typedef struct tag_TJpegContext {
		UINT8		state;
		PTJpegInfo	pinputinfo;
		UINT8		mode;
		UINT8		ctrl; //BIT0: brc enable,BIT1:tcr enable
		PTJpegIndex pindex;
		TJpegCapDly	capdly;
		PVJpegIsrCallback jpegintisr[V5_JPEG_JPEG_INT_NUM];
		PVJpegIsrCallback lbufintisr[V5_JPEG_LBUF_INT_NUM];
	} TJpegContext, *PTJpegContext;


#endif



