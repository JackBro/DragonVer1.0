#ifndef _TRM_OPTION_H_
#define _TRM_OPTION_H_

#ifdef __cplusplus
extern "C" {
#endif


/*********************************************************************
*                             Bus Config                             *
*********************************************************************/

//Define bus type
#define V5_BUS_TYPE V5_BUS_MULTI8
//Define bus mode
#define V5_BUS_MODE V5_BUS_MODE0


/*********************************************************************
*                           Sensor Config                            *
*********************************************************************/

// 1.3M
#define	V5_DEF_SNR_OV_9640_BAYER			1
#define V5_DEF_SNR_OV_9640_YUV				1
// 2M
#define V5_DEF_SNR_MT9D111_BAYER			1
#define V5_DEF_SNR_MT9D111_YUV				1
#define V5_DEF_SNR_OV2620_BAYER				1
#define V5_DEF_SNR_S5K3BAFX_YUV				1
#define V5_DEF_SNR_S5K3BAFX_CCIR656			1
#define V5_DEF_SNR_PO1200N_BAYER			1
#define V5_DEF_SNR_PO1200N_YUV				1
// 3M
#define V5_DEF_SNR_OV_3610_BAYER			1
#define V5_DEF_SNR_KAC3100_BAYER			1


/*********************************************************************
*                         Lcd Panel Config                           *
*********************************************************************/

#define V5_DEF_LCD_NULL_PANEL				0
#define V5_DEF_LCD_LTS180S3_HF1				1 
#define V5_DEF_LCD_LTS180SH_H1I_X			2
#define	V5_DEF_LCD_TRULY_CSTN_BJ2432E_8		3
#define	V5_DEF_LCD_MGG1256DTSW				4
#define	V5_DEF_LCD_PLT2001					5
#define	V5_DEF_LCD_AMPIRE128160				6
#define	V5_DEF_LCD_HD66781S					7
#define	V5_DEF_LCD_UPD161801_24				8
#define	V5_DEF_LCD_UPD161690				9
#define V5_DEF_LCD_TFT_GG3216UTSW_W			10

#define V5_LCD_TYPE							V5_DEF_LCD_TFT_GG3216UTSW_W


/*********************************************************************
*                            Global Info                             *
*********************************************************************/

#ifdef WIN32
extern TCtrlInfo gCtrlInfo;
#else
extern const TCtrlInfo	gCtrlInfo;
#endif
extern const TPanelInfo gPanelInfo;
extern const TPanelCfg  gPanelCfg;
extern const PTSnrInfo	gPSensorInfo[];
extern const UINT16		gSensorNum;
extern const TMarbInfo	gMarbInfo;
extern const TJpegInfo	gJpegInfo;

#ifdef __cplusplus
}
#endif

#endif

