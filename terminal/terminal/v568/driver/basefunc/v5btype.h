


#ifndef _RDK_V5BTYPE_H_
#define _RDK_V5BTYPE_H_

#include "chip.h"

//============== ipp type =======================

enum V5B_IPPMODEenum
{
	IPP_MODE_VIEW	= 0,
	IPP_MODE_CAP	= 0x01,
	IPP_MODE_DEC	= 0x02,
	IPP_MODE_DISP	= 0x03
};

enum V5B_IPPSPECIALenum
{
	IPP_SPE_SEPHIA		= 0,
	IPP_SPE_COLOR		= 0x01,
	IPP_SPE_NEGATIVE	= 0x02,
	IPP_SPE_SKETCH		= 0x03,
	IPP_SPE_MONO		= 0x04,
	IPP_SPE_BLOCK		= 0x05,
	IPP_SPE_TV			= 0x06,
	IPP_SPE_GRID		= 0x07
};

enum V5B_IPPSIZECTRL0enum
{
	IPP_TMB_SIZER_EN	= BIT0,
	IPP_CAP_USIZER_EN	= BIT1,
	IPP_CAP_DSIZER_EN	= BIT2,
	IPP_DISP_SIZER_EN	= BIT3,
	IPP_RESET_TRG		= BIT4,
	IPP_FRAME_EN		= BIT7
};

enum V5B_IPPDATASELECTenum
{
	IPP_DATA_LBUF	= 0,
	IPP_DATA_SIF	= 0x01,
	IPP_DATA_ISP	= 0x02
};

enum V5B_IPPTHUMBSRCenum
{
	IPP_TMB_SPECIAL	= 0,
	IPP_TMB_DSIZER	= 0x01,
	IPP_TMB_CSIZER	= 0x02
};

enum V5B_IPPSIZECTRL1enum
{
	IPP_TMB_VSIZER_EN	= BIT0,
	IPP_TMB_HSIZER_EN	= BIT1,
	IPP_DISP_VSIZER_EN	= BIT2,
	IPP_DISP_HSIZER_EN	= BIT3,
	IPP_CAP_U_VSIZER_EN	= BIT4,
	IPP_CAP_U_HSIZER_EN	= BIT5,
	IPP_CAP_D_VSIZER_EN	= BIT6,
	IPP_CAP_D_HSIZER_EN	= BIT7
};

typedef struct tag_TV5BIppSzDnFct
{
	UINT8	w1;
	UINT8	w2;
	UINT16	s1;
	UINT16	s2;
	UINT8	c;
} TV5BIppSzDnFct, *PTV5BIppSzDnFct;

typedef struct tag_TV5BIppSzUpFct
{
	UINT8	w1;
	UINT8	w2;
	UINT16	s;
} TV5BIppSzUpFct, *PTV5BIppSzUpFct;

//============== sif type =======================
//I2c Structure
typedef struct tag_TI2cBatch
{
	UINT8 RegBytes;
	UINT8 RegAddr;
	UINT8 RegVal[3];
} TI2cBatch, *PTI2cBatch;

//============== biu type =======================
//Define bus type
#define V5_BUS_SEPERATE		1
#define V5_BUS_MULTI8		2
#define V5_BUS_MULTI16		3

//Define bus mode
#define V5_BUS_MODE0		0
#define V5_BUS_MODE1		1
#define V5_BUS_MODE2		2
#define V5_BUS_MODE3		3

//============== Chip type=========================
#define MPWCHIPID                0x0568
#define MASSCHIPID               0x05681100

enum CHIPTYPEenum
{
	DESIGNIN_VER = 0,
	MASS_VER     = 1
};

//============== ISP type =======================

enum ISPCONFIGenum 
{
	TESTBAR_EN = BIT7,
	SHARPEDGE_EN = BIT8,
	LENFALL_EN = BIT9,
	DPD_EN = BIT10,
	RGB_GAMMA_EN = BIT11,
	COLORCORRECT_EN = BIT12,
	Y_GAMMA_EN = BIT13,
	DIG_GAIN_EN = BIT14
};

enum ISPAUTOFUNCenum
{
	AUTOFLK_EN = BIT0,
	AWB_EN = BIT8,
	Y_NOISE_RMV = BIT11,
	UV_NOISE_RMV = BIT12
};

//============== ge type =======================
	enum OPMODE0enum {
		SRC_EXTEND_EN	=	BIT2,
		SRC_MONO_EN		=	BIT3,
		SRC_MONO_WRMASK	=	BIT4,
		TRANSPARENT_EN	=	BIT5
	};

	enum OPMODE1enum {
		PTN_MONO_WRMASK	=	BIT0,
		PTN_MONO_EN		=	BIT1
	};

//============== disp type =======================

	enum DISPCONFIGenum {
		DITHER_EN	= BIT0,
		GAMMA_EN	= BIT3,
		OVERMODE_EN	= BIT6,
		BLEND_EN	= BIT7,
		B0_PRIORITY	= BIT8,
		LCDCDECMODE	= BIT12,
		BLAYER1_SEL	= BIT13
	};

	enum DISPENABLEenum {
		DISPLAY_EN		=	BIT0,
		ALAYER_EN		=	BIT1,
		B0LAYER_EN		=	BIT2,
		BACKGROUND_EN	=	BIT3,
		AOPERATION_EN	=	BIT4,
		B1LAYER_EN		=	BIT5,
		AUPDATE_DONE	=	BIT7
	};

	//============== marb type =======================

	//marb control
	enum V5B_MARB_CTRLenum
	{
		//start capture / pc camera
		V5B_MARB_CTRL_START_CAP_CAM			= BIT1,
		//stop capture avi / pc camera
		V5B_MARB_CTRL_STOP_CAP				= BIT2,
		//start decoder
		V5B_MARB_CTRL_START_DECODE			= BIT3,
		//update priority
		V5B_MARB_CTRL_SET_PRIORITY			= BIT4,
		//start timer
		V5B_MARB_CTRL_START_TIMER			= BIT5,
		//stop timer
		V5B_MARB_CTRL_STOP_TIMER			= BIT6,
		//lock timer
		V5B_MARB_CTRL_LOCK_TIMER			= BIT7
	};

	//marb sub modules reset
	enum V5B_MARB_SUB_RESETenum
	{
		//reset marb_ipp module
		V5B_MARB_SUB_RESET_IPP					= BIT0,
		//reset marb_jpeg module
		V5B_MARB_SUB_RESET_JPEG					= BIT1,
		//reset 1T-SRAM 0
		V5B_MARB_SUB_RESET_1TSRAM0				= BIT2,
		//reset 1T-SRAM 1
		V5B_MARB_SUB_RESET_1TSRAM1				= BIT3,
		//reset 1T-SRAM 2
		V5B_MARB_SUB_RESET_1TSRAM2				= BIT4
	};

	//marb mode control
	enum V5B_MARB_MODECTRL0enum
	{
		//capture mode, capture still image
		V5B_MARB_MODECTRL_CAP_STILL				= 0x0,
		//capture mode, capture multi-shot
		V5B_MARB_MODECTRL_CAP_MULTI_SHOT		= 0x1,
		//capture mode, capture avi
		V5B_MARB_MODECTRL_CAP_AVI				= 0x2,
		//encoder or decoder, 0: encoder, 1: decoder
		V5B_MARB_MODECTRL_DECODE				= BIT2,
		//with thumbnail or without thumbnail, 0: with thumbnail, 1: without thumbnail
		V5B_MARB_MODECTRL_WITHOUT_THUMB			= BIT3,
		//write video header to memory, 0: write to memory, 1: not write
		V5B_MARB_MODECTRL_NOT_WRITE_VIDEO_HEAD	= BIT4,
		//write avi index, 0: write, 1: not write
		V5B_MARB_MODECTRL_NOT_WRITE_AVI_IDX		= BIT5,
		//with JPEG or without JPEG when capturing, 0: with JPEG, 1: without JPEG
		V5B_MARB_MODECTRL_CAP_WITHOUT_JPEG		= BIT6,
		//jpeg interval interrupt type, 0: use jbuf_intv, 1: use frame_interv
		V5B_MARB_MODECTRL_JPEG_USE_FRM_INTV		= BIT7
	};

	enum V5B_MARB_MODECTRL1enum
	{
		//JPEG buffer type, 0: FIFO, care host read/write point,1: buffer, doesn¡¯t care host read/write point
		V5B_MARB_MODECTRL_JBUF_TYPE_BUFFER		= BIT0,
		//thumbnail buffer type, 0: FIFO, care host read/write point, 1: buffer, doesn¡¯t care host read/write point
		V5B_MARB_MODECTRL_TBUF_TYPE_BUFFER		= BIT1,
		//capture video stop type 0 - host, 1 - timer
		V5B_MARB_CAP_VID_AUTO_STOP				= BIT2,
		//tiemr mode, 0 - restart by host, 1 - restart auto
		V5B_MARB_TIMER_MODE						= BIT3,
		V5B_MARB_RECAP_ERR_OP					= BIT4
	};

	enum V5B_MARB_MEM_CTRLenum{
		MMC_POWER0 = BIT0,
		MMC_POWER1 = BIT1,
		MMC_POWER2 = BIT2,
		MMC_CLK0 = BIT4,
		MMC_CLK1 = BIT5,
		MMC_CLK2 = BIT6
	};

	//marb status
	enum V5B_MARB_STATUSenum
	{
		//JPEG FIFO count more than threshold
		V5B_MARB_STATUS_JPEG_G_THD			= BIT0,
		//JPEG buffer read/write data count reaches JPEG buffer interval, read clear
		V5B_MARB_STATUS_JPEG_RH_INTV		= BIT1,
		//jpeg frame save done
		V5B_MARB_STATUS_JPEG_FRM_DONE		= BIT2,
		//JPEG FIFO overflow
		V5B_MARB_STATUS_JPEG_OVERFLOW		= BIT3,
		//thumbnail FIFO count more than threshold
		V5B_MARB_STATUS_THUMB_G_THD			= BIT4,
		//thumbnail buffer read/write data count reaches JPEG buffer interval, read clear
		V5B_MARB_STATUS_THUMB_RH_INTV		= BIT5,
		//thumbnail frame save done
		V5B_MARB_STATUS_THUMB_FRM_DONE		= BIT6,
		//IPP FIFO overflow
		V5B_MARB_STATUS_IPP_OVERFLOW		= BIT7
	};

	enum MARB_1TCTRLenum{
		MARB_1TCTRL_ZZB0 = BIT0,
		MARB_1TCTRL_ZZB1 = BIT1,
		MARB_1TCTRL_ZZB2 = BIT2,
		MARB_1TCTRL_REV  = BIT3,
		MARB_1TCTRL_ECC0 = BIT4,
		MARB_1TCTRL_ECC1 = BIT5,
		MARB_1TCTRL_ECC2 = BIT6
	};

#endif



