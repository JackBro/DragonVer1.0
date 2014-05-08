


#include "basefunc.h"
#ifndef	V578CMD_DISABLE


void V5B_LcdifSetHeadRsFlag(UINT16 uVal)
{
	V5_SetReg(V5_REG_LCDIF_HEAD_RS_FLAG_L, uVal, BIT_16_LTOH);
}

UINT16 V5B_LcdifGetHeadRsFlag(void)
{
	return (UINT16)V5_GetReg(V5_REG_LCDIF_HEAD_RS_FLAG_L, BIT_16_LTOH);
}
//============================

void V5B_LcdifSetHeadNum(UINT16 uVal)
{
	V5_SetReg(V5_REG_LCDIF_HEAD_NUM_L, uVal, BIT_16_LTOH);
}

UINT16 V5B_LcdifGetHeadNum(void)
{
	return (UINT16)V5_GetReg(V5_REG_LCDIF_HEAD_NUM_L, BIT_16_LTOH);
}
//============================

void V5B_LcdifSetHeadStxPos(UINT16 x)
{
	V5_SetReg(V5_REG_LCDIF_START_X_POS_L, x, BIT_16_LTOH);
}

UINT16 V5B_LcdifGetHeadStxPos(void)
{
	return (UINT16)V5_GetReg(V5_REG_LCDIF_START_X_POS_L, BIT_16_LTOH);
}

void V5B_LcdifSetHeadStyPos(UINT16 x)
{
	V5_SetReg(V5_REG_LCDIF_START_Y_POS_L, x, BIT_16_LTOH);
}

UINT16 V5B_LcdifGetHeadStyPos(void)
{
	return (UINT16)V5_GetReg(V5_REG_LCDIF_START_Y_POS_L, BIT_16_LTOH);
}

void V5B_LcdifSetHeadEdxPos(UINT16 x)
{
	V5_SetReg(V5_REG_LCDIF_END_X_POS_L, x, BIT_16_LTOH);
}

UINT16 V5B_LcdifGetHeadEdxPos(void)
{
	return (UINT16)V5_GetReg(V5_REG_LCDIF_END_X_POS_L, BIT_16_LTOH);
}

void V5B_LcdifSetHeadEdyPos(UINT16 x)
{
	V5_SetReg(V5_REG_LCDIF_END_Y_POS_L, x, BIT_16_LTOH);
}

UINT16 V5B_LcdifGetHeadEdyPos(void)
{
	return (UINT16)V5_GetReg(V5_REG_LCDIF_END_Y_POS_L, BIT_16_LTOH);
}
//============================

void V5B_LcdifSetHeadCfg(UINT16 uVal)
{
	V5_SetReg(V5_REG_LCDIF_HEAD_CONFIG_L, uVal, BIT_16_LTOH);
}

UINT16 V5B_LcdifGetHeadCfg(void)
{
	return (UINT16)V5_GetReg(V5_REG_LCDIF_HEAD_CONFIG_L, BIT_16_LTOH);
}

void V5B_LcdifEnableLineHead(void)
{
	V5_SetReg(V5_REG_LCDIF_HEAD_CONFIG_L, 0x1, BIT_8);
}

void V5B_LcdifDisableLineHead(void)
{
	V5_SetReg(V5_REG_LCDIF_HEAD_CONFIG_L, 0, BIT_8);
}

void V5B_LcdifUpdateHead(void)
{
	V5_SetReg(V5_REG_LCDIF_HEAD_CONFIG_L, 0x9, BIT_8);
}
//============================

void V5B_LcdifSetCfg(UINT8 x)
{
	V5_SetReg(V5_REG_LCDIF_CONFIG_L, x, BIT_8);
}

UINT8 V5B_LcdifGetCfg(void)
{
	return (UINT8)V5_GetReg(V5_REG_LCDIF_CONFIG_L, BIT_8);
}

UINT8 V5B_LcdifEnableCs0(void)
{
	UINT8 x = 0;

	x = V5B_BiuGetLcdIfCon();
	V5B_BiuSetLcdIfCon(x & 0xfe);
	return x;
}

UINT8 V5B_LcdifEnableCs1(void)
{
	UINT8 x = 0;

	x = V5B_BiuGetLcdIfCon();
	V5B_BiuSetLcdIfCon(x | 0x01);
	return x;
}

UINT8 V5B_LcdifEnableRsHigh(void)
{
	UINT8 x;

	x = V5B_LcdifGetCfg();
	V5B_LcdifSetCfg(x | 0x8);
	return x;
}

UINT8 V5B_LcdifDisableRsHigh(void)
{
	UINT8 x;

	x = V5B_LcdifGetCfg();
	V5B_LcdifSetCfg(x & 0xf7);
	return x;
}

UINT8 V5B_LcdifEnable68kBus(void)
{
	UINT8 x;

	x = V5B_LcdifGetCfg();
	V5B_LcdifSetCfg(x | 0x20);
	return x;
}

UINT8 V5B_LcdifEnable80Bus(void)
{
	UINT8 x;

	x = V5B_LcdifGetCfg();
	V5B_LcdifSetCfg(x & 0xdf);
	return x;
}

//==========================

void V5B_LcdifResetPanel(void)
{
	V5_SetReg(V5_REG_LCDIF_LCD_RESET, 0x0, BIT_8);
#ifndef NCSC
	V5_Delay(100);
#endif
	V5_SetReg(V5_REG_LCDIF_LCD_RESET, 1, BIT_8);
#ifndef NCSC
	V5_Delay(20);	//wait signal stable to avoid writing reg fail.
#endif
}
//==========================

void V5B_LcdifSetDmapfmt(UINT8 uVal)
{
	V5_SetReg(V5_REG_LCDIF_DMAP, uVal, BIT_8);
}

UINT8 V5B_LcdifGetDmapfmt(void)
{
	return (UINT8)V5_GetReg(V5_REG_LCDIF_DMAP, BIT_8);
}
//=================

void V5B_LcdifSetPadData(UINT8 uVal)
{
	V5_SetReg(V5_REG_LCDIF_PAD_L, uVal, BIT_8);
}

UINT8 V5B_LcdifGetPadData(void)
{
	return (UINT8)V5_GetReg(V5_REG_LCDIF_PAD_L, BIT_8);
}

void V5B_LcdifPadEnable(void)
{
	V5_SetReg(V5_REG_LCDIF_PAD_H, 0x1, BIT_8);
}

void V5B_LcdifPadDisable(void)
{
	V5_SetReg(V5_REG_LCDIF_PAD_H, 0x0, BIT_8);
}
//====================

void V5B_LcdifSetWrTiming(UINT16 uVal)
{
	V5_SetReg(V5_REG_LCDIF_WCONT_L, uVal, BIT_16_LTOH);
}

UINT16 V5B_LcdifGetWrTiming(void)
{
	return (UINT16)V5_GetReg(V5_REG_LCDIF_WCONT_L, BIT_16_LTOH);
}

void V5B_LcdifSetRdTiming(UINT16 uVal)
{
	V5_SetReg(V5_REG_LCDIF_RCONT_L, uVal, BIT_16_LTOH);
}

UINT16 V5B_LcdifGetRdTiming(void)
{
	return (UINT16)V5_GetReg(V5_REG_LCDIF_RCONT_L, BIT_16_LTOH);
}
//===============

void V5B_LcdifRsLExW(UINT16 uVal)
{
	V5_SetReg(V5_REG_LCDIF_EXW0_L, uVal, BIT_16_LTOH);
	V5_SetReg(V5_REG_LCDIF_EXW_EN, 0x1, BIT_8);
}

void V5B_LcdifRsHExW(UINT16 uVal)
{
	V5_SetReg(V5_REG_LCDIF_EXW1_L, uVal, BIT_16_LTOH);
	V5_SetReg(V5_REG_LCDIF_EXW_EN, 0x2, BIT_8);
}
//==================

UINT16 V5B_LcdifRsLExR(void)
{
	V5_SetReg(V5_REG_LCDIF_EXRS, 0x0, BIT_8);
	return (UINT16)V5_GetReg(V5_REG_LCDIF_EXR0_L, BIT_16_LTOH);
}

UINT16 V5B_LcdifRsHExR(void)
{
	V5_SetReg(V5_REG_LCDIF_EXRS, 0x1, BIT_8);
	return (UINT16)V5_GetReg(V5_REG_LCDIF_EXR0_L, BIT_16_LTOH);
}
//==================

void V5B_LcdifSetHeadData(UINT8 index, UINT16 x)
{
	V5_SetReg(V5_REG_LCDIF_HEAD_DATA0 + (index << 1), x, BIT_16_LTOH);
}

UINT16 V5B_LcdifGetHeadData(UINT8 index)
{
	return (UINT16)V5_GetReg(V5_REG_LCDIF_HEAD_DATA0 + (index << 1), BIT_16_LTOH);
}

void V5B_LcdifSetHead(UINT8 count, UINT16 *x)
{
	UINT8 i;

	count = (count > 16) ? 16 : count;
	for(i = 0; i < count; i++)
		V5B_LcdifSetHeadData(i, x[i]);
}

void V5B_LcdifSetBiasX(UINT8 x)
{
	V5_SetReg(V5_REG_LCDIF_BIAS_X, x, BIT_8);
}

UINT8 V5B_LcdifGetBiasX(void)
{
	return (UINT8)V5_GetReg(V5_REG_LCDIF_BIAS_X, BIT_8);
}

void V5B_LcdifSetBiasY(UINT8 x)
{
	V5_SetReg(V5_REG_LCDIF_BIAS_Y, x, BIT_8);
}

UINT8 V5B_LcdifGetBiasY(void)
{
	return (UINT8)V5_GetReg(V5_REG_LCDIF_BIAS_Y, BIT_8);
}

void V5B_LcdifSetBiasSpt(TPoint pt)
{
	V5B_LcdifSetBiasX((UINT8)pt.x);
	V5B_LcdifSetBiasY((UINT8)pt.y);
}

void V5B_LcdifGetBiasSpt(TPoint *pt)
{
	pt->x = V5B_LcdifGetBiasX();
	pt->y = V5B_LcdifGetBiasY();
}

void V5B_LcdifDivxEnable(void)
{
	V5_SetReg(V5_REG_LCDIF_BIAS_X_H, 0x01, BIT_8);
}

void V5B_LcdifDivxDisalbe(void)
{
	V5_SetReg(V5_REG_LCDIF_BIAS_X_H, 0, BIT_8);
}

void V5B_LcdifDivyEnable(void)
{
	V5_SetReg(V5_REG_LCDIF_BIAS_Y_H, 0x01, BIT_8);
}

void V5B_LcdifDivyDisalbe(void)
{
	V5_SetReg(V5_REG_LCDIF_BIAS_Y_H, 0, BIT_8);
}

//	Head Cs mode
void V5B_LcdifSetCsMode(UINT16 uVal)
{
	V5_SetReg(V5_REG_LCDIF_HEAD_CS_MODE_L, uVal, BIT_16_LTOH);
}

UINT16 V5B_LcdifGetCsMode(void)
{
	return (UINT16)V5_GetReg(V5_REG_LCDIF_HEAD_CS_MODE_L, BIT_16_LTOH);
}

//	Head Cs Flag
void V5B_LcdifSetCsFlag(UINT16 uVal)
{
	V5_SetReg(V5_REG_LCDIF_HEAD_CS_FLAG_L, uVal, BIT_16_LTOH);
}

UINT16 V5B_LcdifGetCsFlag(void)
{
	return (UINT16)V5_GetReg(V5_REG_LCDIF_HEAD_CS_FLAG_L, BIT_16_LTOH);
}

//	Head Process Flag
void V5B_LcdifSetHeadProcFlag(UINT8 val)
{
	V5_SetReg(V5_REG_LCDIF_HEAD_PROC_FLG, val, BIT_8);
}

UINT8 V5B_LcdifGetHeadProcFlag(void)
{
	return (UINT8)V5_GetReg(V5_REG_LCDIF_HEAD_PROC_FLG, BIT_8);
}

void V5B_LcdifCommCodeEnable(void)
{
	UINT8 x = 0;

	x = V5B_LcdifGetHeadProcFlag();
	V5B_LcdifSetHeadProcFlag((x | 0x01));
}

void V5B_LcdifCommCodeDisalbe(void)
{
	UINT8 x = 0;

	x = V5B_LcdifGetHeadProcFlag();
	V5B_LcdifSetHeadProcFlag((x & 0xfe));
}

void V5B_LcdifHeadRptEnable(void)
{
	UINT8 x = 0;

	x = V5B_LcdifGetHeadProcFlag();
	V5B_LcdifSetHeadProcFlag((x | 0x02));
}

void V5B_LcdifHeadRptDisalbe(void)
{
	UINT8 x = 0;

	x = V5B_LcdifGetHeadProcFlag();
	V5B_LcdifSetHeadProcFlag((x & 0xfd));
}

void V5B_LcdifSetCommStartX(UINT16 val)
{
	V5_SetReg(V5_REG_LCDIF_COMM_START_X, val, BIT_16_LTOH);
}

UINT16 V5B_LcdifGetCommStartX(void)
{
	return (UINT16)V5_GetReg(V5_REG_LCDIF_COMM_START_X, BIT_16_LTOH);
}

void V5B_LcdifSetCommEndX(UINT16 val)
{
	V5_SetReg(V5_REG_LCDIF_COMM_END_X, val, BIT_16_LTOH);
}

UINT16 V5B_LcdifGetCommEndX(void)
{
	return (UINT16)V5_GetReg(V5_REG_LCDIF_COMM_END_X, BIT_16_LTOH);
}

void V5B_LcdifSetRptStartX(UINT16 val)
{
	V5_SetReg(V5_REG_LCDIF_RPT_START_X, val, BIT_16_LTOH);
}

UINT16 V5B_LcdifGetRptStartX(void)
{
	return (UINT16)V5_GetReg(V5_REG_LCDIF_RPT_START_X, BIT_16_LTOH);
}

void V5B_LcdifSetRptStartY(UINT16 val)
{
	V5_SetReg(V5_REG_LCDIF_RPT_START_Y, val, BIT_16_LTOH);
}

UINT16 V5B_LcdifGetRptStartY(void)
{
	return (UINT16)V5_GetReg(V5_REG_LCDIF_RPT_START_Y, BIT_16_LTOH);
}

void V5B_LcdifSetRptEndX(UINT16 val)
{
	V5_SetReg(V5_REG_LCDIF_RPT_END_X, val, BIT_16_LTOH);
}

UINT16 V5B_LcdifGetRptEndX(void)
{
	return (UINT16)V5_GetReg(V5_REG_LCDIF_RPT_END_X, BIT_16_LTOH);
}

void V5B_LcdifSetRptEndY(UINT16 val)
{
	V5_SetReg(V5_REG_LCDIF_RPT_END_Y, val, BIT_16_LTOH);
}

UINT16 V5B_LcdifGetRptEndY(void)
{
	return (UINT16)V5_GetReg(V5_REG_LCDIF_RPT_END_Y, BIT_16_LTOH);
}

void V5B_LcdifSetCommStartY(UINT16 val)
{
	V5_SetReg(V5_REG_LCDIF_COMM_START_Y, val, BIT_16_LTOH);
}

UINT16 V5B_LcdifGetCommStartY(void)
{
	return (UINT16)V5_GetReg(V5_REG_LCDIF_COMM_START_Y, BIT_16_LTOH);
}

void V5B_LcdifSetCommEndY(UINT16 val)
{
	V5_SetReg(V5_REG_LCDIF_COMM_END_Y, val, BIT_16_LTOH);
}

UINT16 V5B_LcdifGetCommEndY(void)
{
	return (UINT16)V5_GetReg(V5_REG_LCDIF_COMM_END_Y, BIT_16_LTOH);
}

#endif //V578CMD_DISABLE
