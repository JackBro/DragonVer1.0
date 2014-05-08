#ifndef _RDK_LCDIF_H_
#define _RDK_LCDIF_H_

#ifdef __cplusplus
extern "C" {
#endif

//lcdif head config
enum V5B_LCDIF_HEAD_CFGenum
{
	//head enable
	V5B_LCDIF_HEAD_EN						= BIT0,
	//updata the all position of x and y
	V5B_LCDIF_HEAD_UPDATE_POS				= BIT3
};


void	V5B_LcdifSetHeadRsFlag(UINT16 uVal);
UINT16	V5B_LcdifGetHeadRsFlag(void);
//============================

void	V5B_LcdifSetHeadNum(UINT16 uVal);
UINT16	V5B_LcdifGetHeadNum(void);
//============================

void	V5B_LcdifSetHeadStxPos(UINT16 x);
UINT16	V5B_LcdifGetHeadStxPos(void);
void	V5B_LcdifSetHeadStyPos(UINT16 x);
UINT16	V5B_LcdifGetHeadStyPos(void);
void	V5B_LcdifSetHeadEdxPos(UINT16 x);
UINT16	V5B_LcdifGetHeadEdxPos(void);
void	V5B_LcdifSetHeadEdyPos(UINT16 x);
UINT16	V5B_LcdifGetHeadEdyPos(void);
//============================

void	V5B_LcdifSetHeadCfg(UINT16 uVal);
UINT16	V5B_LcdifGetHeadCfg(void);
void	V5B_LcdifEnableLineHead(void);
void	V5B_LcdifDisableLineHead(void);
void	V5B_LcdifUpdateHead(void);
//============================

void	V5B_LcdifSetBiasX(UINT8 x);
UINT8	V5B_LcdifGetBiasX(void);
void	V5B_LcdifSetBiasY(UINT8 x);
UINT8	V5B_LcdifGetBiasY(void);
void	V5B_LcdifSetBiasSpt(TPoint pt);
void	V5B_LcdifGetBiasSpt(TPoint *pt);

void	V5B_LcdifDivxEnable(void);
void	V5B_LcdifDivxDisalbe(void);
void	V5B_LcdifDivyEnable(void);
void	V5B_LcdifDivyDisalbe(void);

//============================

void	V5B_LcdifSetCfg(UINT8 x);
UINT8	V5B_LcdifGetCfg(void);
UINT8	V5B_LcdifEnableCs0(void);
UINT8	V5B_LcdifEnableCs1(void);
UINT8	V5B_LcdifEnableRsHigh(void);
UINT8	V5B_LcdifDisableRsHigh(void);
UINT8	V5B_LcdifEnable68kBus(void);
UINT8	V5B_LcdifEnable80Bus(void);
//==========================

void	V5B_LcdifResetPanel(void);
//==========================

void	V5B_LcdifSetDmapfmt(UINT8 uVal);
UINT8	V5B_LcdifGetDmapfmt(void);
//=================

void	V5B_LcdifSetPadData(UINT8 uVal);
UINT8	V5B_LcdifGetPadData(void);
void	V5B_LcdifPadEnable(void);
void	V5B_LcdifPadDisable(void);
//====================

void	V5B_LcdifSetWrTiming(UINT16 uVal);
UINT16	V5B_LcdifGetWrTiming(void);
void	V5B_LcdifSetRdTiming(UINT16 uVal);
UINT16	V5B_LcdifGetRdTiming(void);
//===============

void	V5B_LcdifRsLExW(UINT16 uVal);
void	V5B_LcdifRsHExW(UINT16 uVal);
//==================

UINT16	V5B_LcdifRsLExR(void);
UINT16	V5B_LcdifRsHExR(void);
//==================

void	V5B_LcdifSetHeadData(UINT8 index, UINT16 x);
UINT16	V5B_LcdifGetHeadData(UINT8 index);
void	V5B_LcdifSetHead(UINT8 count, UINT16 *x);

void	V5B_LcdifSetCsMode(UINT16 uVal);
UINT16	V5B_LcdifGetCsMode(void);
void	V5B_LcdifSetCsFlag(UINT16 uVal);
UINT16	V5B_LcdifGetCsFlag(void);

void	V5B_LcdifSetHeadProcFlag(UINT8 val);
UINT8	V5B_LcdifGetHeadProcFlag(void);
void	V5B_LcdifCommCodeEnable(void);
void	V5B_LcdifCommCodeDisalbe(void);
void	V5B_LcdifHeadRptEnable(void);
void	V5B_LcdifHeadRptDisalbe(void);

void	V5B_LcdifSetCommStartX(UINT16 val);
UINT16	V5B_LcdifGetCommStartX(void);
void	V5B_LcdifSetCommEndX(UINT16 val);
UINT16	V5B_LcdifGetCommEndX(void);
void	V5B_LcdifSetRptStartX(UINT16 val);
UINT16	V5B_LcdifGetRptStartX(void);
void	V5B_LcdifSetRptStartY(UINT16 val);
UINT16	V5B_LcdifGetRptStartY(void);
void	V5B_LcdifSetRptEndX(UINT16 val);
UINT16	V5B_LcdifGetRptEndX(void);
void	V5B_LcdifSetRptEndY(UINT16 val);
UINT16	V5B_LcdifGetRptEndY(void);
void	V5B_LcdifSetCommStartY(UINT16 val);
UINT16	V5B_LcdifGetCommStartY(void);
void	V5B_LcdifSetCommEndY(UINT16 val);
UINT16	V5B_LcdifGetCommEndY(void);

#ifdef __cplusplus
}
#endif


#endif
