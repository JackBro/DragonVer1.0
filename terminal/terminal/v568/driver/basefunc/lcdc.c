
#include "basefunc.h"
#ifndef	V578CMD_DISABLE

void V5B_LcdcSetDc(UINT16 x)
{
	V5_SetReg(V5_REG_LCDC_DC, x, BIT_16_LTOH);
}

UINT16 V5B_LcdcGetDc(void)
{
	return (UINT16)V5_GetReg(V5_REG_LCDC_DC, BIT_16_LTOH);
}

UINT16 V5B_LcdcEnableDither(void)
{
	UINT16	x;

	x = V5B_LcdcGetDc();
	if((x & DITHER_EN) == 0)
		V5B_LcdcSetDc((UINT16)(x | DITHER_EN));
	return x;
}

UINT16 V5B_LcdcDisableDither(void)
{
	UINT16	x;

	x = V5B_LcdcGetDc();
	V5B_LcdcSetDc((UINT16)(x & ~DITHER_EN));
	return x;
}

void V5B_LcdcSetDitherSel(UINT8 sel)
{
	UINT16	x;

	x = V5B_LcdcGetDc();
	x &= 0xfff9;
	x |= ((UINT16)sel << 1) & 0x6;
	V5B_LcdcSetDc(x);
}

UINT8 V5B_LcdcGetDitherSel(void)
{
	UINT16	x;

	x = V5B_LcdcGetDc();
	x &= 6;
	x >>= 1;
	return (UINT8)x;
}

UINT16 V5B_LcdcEnableGamma(void)
{
	UINT16	x;

	x = V5B_LcdcGetDc();
	V5B_LcdcSetDc((UINT16)(x | GAMMA_EN));
	return x;
}

UINT16 V5B_LcdcDisableGamma(void)
{
	UINT16	x;

	x = V5B_LcdcGetDc();
	V5B_LcdcSetDc((UINT16)(x & ~GAMMA_EN));
	return x;
}

void V5B_LcdcSetB0OverlayMode(UINT8 sel)
{
	UINT16	x;

	x = V5B_LcdcGetDc();
	x &= 0xffcf;
	x |= ((UINT16)sel << 4) & 0x30;
	V5B_LcdcSetDc(x);
}

void V5B_LcdcSetB1OverlayMode(UINT8 sel)
{
	UINT8	x;

	x = (UINT8)V5_GetReg(V5_REG_LCDC_UPDATE, BIT_8);
	x &= 0xcf;
	x |= (sel << 4) & 0x30;
	V5_SetReg(V5_REG_LCDC_UPDATE, x, BIT_8);
}

UINT8 V5B_LcdcGetB0OverlayMode(void)
{
	UINT16	x;

	x = V5B_LcdcGetDc();
	x &= 0x30;
	x >>= 4;
	return (UINT8)x;
}

UINT8 V5B_LcdcGetB1OverlayMode(void)
{
	UINT8	x;

	x = (UINT8)V5_GetReg(V5_REG_LCDC_UPDATE, BIT_8);
	x &= 0x30;
	x >>= 4;
	return x;
}

UINT16 V5B_LcdcEnableOverlay(void)
{
	UINT16	x;

	x = V5B_LcdcGetDc();
	if( (x & OVERMODE_EN) == 0 )
		V5B_LcdcSetDc((UINT16)(x | OVERMODE_EN));
	return x;
}

UINT16 V5B_LcdcDisableOverlay(void)
{
	UINT16	x;

	x = V5B_LcdcGetDc();
	if( (x & OVERMODE_EN) != 0 )
		V5B_LcdcSetDc((UINT16)(x & ~OVERMODE_EN));
	return x;
}

UINT16 V5B_LcdcEnableBlending(void)
{
	UINT16	x;

	x = V5B_LcdcGetDc();
	if( (x & BLEND_EN) == 0 )
		V5B_LcdcSetDc((UINT16)(x | BLEND_EN));
	return x;
}

UINT16 V5B_LcdcDisableBlending(void)
{
	UINT16 x = 0;

	x = V5B_LcdcGetDc();
	if( (x & BLEND_EN) != 0 )
		V5B_LcdcSetDc((UINT16)(x & ~BLEND_EN));
	return x;
}

UINT16 V5B_LcdcLayerB0First(void)
{
	UINT16 x = 0;

	x = V5B_LcdcGetDc();
	V5B_LcdcSetDc((UINT16)(x | B0_PRIORITY));
	return x;
}

UINT16 V5B_LcdcLayerB0FstOff(void)
{
	UINT16 x = 0;

	x = V5B_LcdcGetDc();
	V5B_LcdcSetDc((UINT16)(x & ~B0_PRIORITY));
	return x;
}

UINT16 V5B_LcdcLayerB1First(void)
{
	UINT16 x = 0;
	
	x = (UINT16)V5_GetReg(V5_REG_LCDC_UPDATE, BIT_8);
	x |= 0x40;
	V5_SetReg(V5_REG_LCDC_UPDATE, x, BIT_8);
	
	return x;
}

UINT16 V5B_LcdcLayerB1FstOff(void)
{
	UINT16	x;

	x = (UINT8)V5_GetReg(V5_REG_LCDC_UPDATE, BIT_8);
	x &= 0xbf;
	V5_SetReg(V5_REG_LCDC_UPDATE, x, BIT_8);

	return (UINT16)x;
}

void V5B_LcdcLayerAFirst(void)
{
	UINT16 x = 0;

	x = V5B_LcdcGetDc();
	V5B_LcdcSetDc((UINT16)(x & ~B0_PRIORITY));

	x = (UINT8)V5_GetReg(V5_REG_LCDC_UPDATE, BIT_8);
	x &= 0xbf;
	V5_SetReg(V5_REG_LCDC_UPDATE, x, BIT_8);
}

UINT16 V5B_LcdcDecodeMode(void)
{
	UINT16	x;

	x = V5B_LcdcGetDc();
	V5B_LcdcSetDc((UINT16)(x | LCDCDECMODE));
	return x;
}

UINT16 V5B_LcdcNormalMode(void)
{
	UINT16	x;

	x = V5B_LcdcGetDc();
	V5B_LcdcSetDc((UINT16)(x & ~LCDCDECMODE));
	return x;
}

void V5B_LcdcSetAlayerFmt(UINT8 fmt)
{
	UINT16	x;

	x = V5B_LcdcGetDc();
	x &= 0x3fff;
	x |= (UINT16)fmt << 14;
	V5B_LcdcSetDc(x);
}

UINT8 V5B_LcdcGetAlayerFmt(void)
{
	UINT16	x;

	x = V5B_LcdcGetDc();
	x >>= 14;
	return (UINT8)x & 0x3;
}

//===================

void V5B_LcdcSetDe(UINT8 x)
{
	V5_SetReg(V5_REG_LCDC_DE, x, BIT_8);
}

UINT8 V5B_LcdcGetDe(void)
{
	return (UINT8)V5_GetReg(V5_REG_LCDC_DE, BIT_8);
}

UINT8 V5B_LcdcEnableAOperation(void)
{
	UINT8	x;

	x = V5B_LcdcGetDe();
	V5B_LcdcSetDe((UINT8)((x & ~ALAYER_EN) | AOPERATION_EN));
	return x;
}

UINT8 V5B_LcdcEnableAlayer(void)
{
	UINT8	x;

	x = V5B_LcdcGetDe();
	V5B_LcdcSetDe((UINT8)(x | ALAYER_EN | DISPLAY_EN | AOPERATION_EN));
	return x;
}

UINT8 V5B_LcdcDisableAlayer(void)
{
	UINT8	x;

	x = V5B_LcdcGetDe();
	V5B_LcdcSetDe((UINT8)(x & ~(ALAYER_EN | AOPERATION_EN)));
	return x;
}

UINT8 V5B_LcdcEnableB0(void)
{
	UINT8	x;

	x = V5B_LcdcGetDe();
	V5B_LcdcSetDe((UINT8)(x | B0LAYER_EN));
	return x;
}

UINT8 V5B_LcdcDisableB0(void)
{
	UINT8	x;

	x = V5B_LcdcGetDe();
	V5B_LcdcSetDe((UINT8)(x & ~B0LAYER_EN));
	return x;
}

UINT8 V5B_LcdcEnableBG(void)
{
	UINT8	x;

	x = V5B_LcdcGetDe();
	V5B_LcdcSetDe((UINT8)(x | BACKGROUND_EN));
	return x;
}

UINT8 V5B_LcdcDisableBG(void)
{
	UINT8	x;

	x = V5B_LcdcGetDe();
	V5B_LcdcSetDe((UINT8)(x & ~BACKGROUND_EN));
	return x;
}

UINT8 V5B_LcdcEnableB1(void)
{
	UINT8	x;

	x = V5B_LcdcGetDe();
	V5B_LcdcSetDe((UINT8)(x | B1LAYER_EN));
	return x;
}

UINT8 V5B_LcdcDisableB1(void)
{
	UINT8	x;

	x = V5B_LcdcGetDe();
	V5B_LcdcSetDe((UINT8)(x & ~B1LAYER_EN));
	return x;
}

void V5B_LcdcClearEOF(void)
{
	UINT8	x;

	x = V5B_LcdcGetDe();
	V5B_LcdcSetDe((UINT8)(x & ~AUPDATE_DONE));
}

UINT8 V5B_LcdcCheckEOF(void)
{
	UINT8	x;

	x = V5B_LcdcGetDe();
	x &= AUPDATE_DONE;

	return x;
}

//====================
void V5B_LcdcResetState(void)
{
	V5_SetReg(V5_REG_LCDC_DRST, 0x80, BIT_8);
}

//====================
void V5B_LcdcUpdate(UINT8 x)
{
	V5_SetReg(V5_REG_LCDC_UPDATE, x, BIT_8);
}

UINT8 V5B_LcdcGetUpdate(void)
{
	return (UINT8)V5_GetReg(V5_REG_LCDC_UPDATE, BIT_8);
}

void V5B_LcdcSetOLP(UINT8 sel)
{
	UINT8 x = 0;

	x = V5B_LcdcGetUpdate();
	x &= 0xf3;
	x |= ((sel << 2) & 0x0c);
	V5B_LcdcUpdate(x);
}

UINT8 V5B_LcdcGetOLP(void)
{
	UINT8 x = 0;

	x = V5B_LcdcGetUpdate();
	x &= 0x0c;
	x >>= 2;
	return x;
}

void V5B_LcdcB0layerUpdate(void)
{
//	UINT32 flag = 0xffff;
	UINT8 val;

	val = (UINT8)V5_GetReg(V5_REG_LCDC_UPDATE, BIT_8);
	val |= 0x01; /* bit0 for update B0 */
	V5_SetReg(V5_REG_LCDC_UPDATE, val, BIT_8);
//	while(val && flag)
//	{
//		val = (UINT8)(0x01 & (V5_GetReg(V5_REG_LCDC_UPDATE, BIT_8)));
//		flag --;
//	}
}

void V5B_LcdcB1layerUpdate(void)
{
//	UINT32 flag = 0xffff;
	UINT8 val;

	val = (UINT8)V5_GetReg(V5_REG_LCDC_UPDATE, BIT_8);
	val |= 0x80; /* bit7 for update B1 */
	V5_SetReg(V5_REG_LCDC_UPDATE, val, BIT_8);
#if 0
	// temp for B1 update bit could not return 0 by hardware now -- 2005.08.09.
	while(val && flag)
	{
		val = (UINT8)(0x80 & (V5_GetReg(V5_REG_LCDC_UPDATE, BIT_8)));
		flag --;
	}
#endif
}

void V5B_LcdcBgUpdate(void)
{
	UINT8 val;

	val = (UINT8)V5_GetReg(V5_REG_LCDC_UPDATE, BIT_8);
	val |= 0x02; /* bit1 for update Bg */
	V5_SetReg(V5_REG_LCDC_UPDATE, val, BIT_8);
}

//========================
void V5B_LcdcSetRotateMode(UINT8 x)
{
	V5_SetReg(V5_REG_LCDC_RM, x, BIT_8);
}

UINT8 V5B_LcdcGetRotateMode(void)
{
	return (UINT8)V5_GetReg(V5_REG_LCDC_RM, BIT_8);
}

void V5B_LcdcSetAlayerRM(UINT8 x)
{
	UINT8 y;

	y = V5B_LcdcGetRotateMode();
	y &= 0xf8;
	y |= x & 0x7;
	V5B_LcdcSetRotateMode(y); 
}

UINT8 V5B_LcdcGetAlayerRM(void)
{
	UINT8 x;

	x = V5B_LcdcGetRotateMode();
	x &= 0x7;
	return x;
}

void V5B_LcdcSetBlayerRM(UINT8 x)
{
	UINT8 y;

	y = V5B_LcdcGetRotateMode();
	y &= 0xc7;
	y |= (x & 0x7) << 3;
	V5B_LcdcSetRotateMode(y); 
}

UINT8 V5B_LcdcGetBlayerRM(void)
{
	UINT8 x;

	x = V5B_LcdcGetRotateMode();
	x &= 0x38;
	x >>= 3;
	return x;
}

//==========================
void V5B_LcdcSetVbufBa0(UINT32 x)
{
	V5_SetReg(V5_REG_LCDC_VBBA0_L, x, BIT_24_LTOH);
}

UINT32 V5B_LcdcGetVbufBa0(void)
{
	return V5_GetReg(V5_REG_LCDC_VBBA0_L, BIT_24_LTOH);
}

void V5B_LcdcSetVbufBa1(UINT32 x)
{
	V5_SetReg(V5_REG_LCDC_VBBA1_L, x, BIT_24_LTOH);
}

UINT32 V5B_LcdcGetVbufBa1(void)
{
	return V5_GetReg(V5_REG_LCDC_VBBA1_L, BIT_24_LTOH);
}

//===================
void V5B_LcdcSetGbufBa0(UINT32 x)
{
	V5_SetReg(V5_REG_LCDC_GBBA0_L, x, BIT_24_LTOH);
}

UINT32 V5B_LcdcGetGbufBa0(void)
{
	return V5_GetReg(V5_REG_LCDC_GBBA0_L, BIT_24_LTOH);
}

void V5B_LcdcSetGbufBa1(UINT32 x)
{
	V5_SetReg(V5_REG_LCDC_GBBA1_L, x, BIT_24_LTOH);
}

UINT32 V5B_LcdcGetGbufBa1(void)
{
	return V5_GetReg(V5_REG_LCDC_GBBA1_L, BIT_24_LTOH);
}

//====================
void V5B_LcdcSetAxP(UINT16 x)
{
	V5_SetReg(V5_REG_LCDC_AX_L, x, BIT_16_LTOH);
}

UINT16 V5B_LcdcGetAxP(void)
{
	return (UINT16)V5_GetReg(V5_REG_LCDC_AX_L, BIT_16_LTOH);
}

void V5B_LcdcSetAyP(UINT16 x)
{
	V5_SetReg(V5_REG_LCDC_AY_L, x, BIT_16_LTOH);
}

UINT16 V5B_LcdcGetAyP(void)
{
	return (UINT16)V5_GetReg(V5_REG_LCDC_AY_L, BIT_16_LTOH);
}

void V5B_LcdcSetASptPanel(TPoint pt)
{
	V5B_LcdcSetAxP(pt.x);
	V5B_LcdcSetAyP(pt.y);
}

void V5B_LcdcGetASptPanel(TPoint *pt)
{
	pt->x = V5B_LcdcGetAxP();
	pt->y = V5B_LcdcGetAyP();
}

//===================
void V5B_LcdcSetAw(UINT16 x)
{
	x--;
	V5_SetReg(V5_REG_LCDC_AW_L, x, BIT_16_LTOH);
}

UINT16 V5B_LcdcGetAw(void)
{
	return (UINT16)V5_GetReg(V5_REG_LCDC_AW_L, BIT_16_LTOH) + 1;
}

void V5B_LcdcSetAh(UINT16 x)
{
	x--;
	V5_SetReg(V5_REG_LCDC_AH_L, x, BIT_16_LTOH);
}

UINT16 V5B_LcdcGetAh(void)
{
	return (UINT16)V5_GetReg(V5_REG_LCDC_AH_L, BIT_16_LTOH) + 1;
}

void V5B_LcdcSetAWinSize(TSize size)
{
	V5B_LcdcSetAw(size.cx);
	V5B_LcdcSetAh(size.cy);
}

void V5B_LcdcGetAWinSize(TSize *size)
{
	size->cx = V5B_LcdcGetAw();
	size->cy = V5B_LcdcGetAh();
}

//====================
void V5B_LcdcSetAdx0(UINT16 x)
{
	V5_SetReg(V5_REG_LCDC_ADX0_L, x, BIT_16_LTOH);
}

UINT16 V5B_LcdcGetAdx0(void)
{
	return (UINT16)V5_GetReg(V5_REG_LCDC_ADX0_L, BIT_16_LTOH);
}

void V5B_LcdcSetAdy0(UINT16 x)
{
	V5_SetReg(V5_REG_LCDC_ADY0_L, x, BIT_16_LTOH);
}

UINT16 V5B_LcdcGetAdy0(void)
{
	return (UINT16)V5_GetReg(V5_REG_LCDC_ADY0_L, BIT_16_LTOH);
}

void V5B_LcdcSetASptMem(TPoint pt)
{
	V5B_LcdcSetAdx0(pt.x);
	V5B_LcdcSetAdy0(pt.y);
}

void V5B_LcdcGetASptMem(TPoint *pt)
{
	pt->x = V5B_LcdcGetAdx0();
	pt->y = V5B_LcdcGetAdy0();
}

//=================
void V5B_LcdcSetAmw(UINT16 x)
{
	V5_SetReg(V5_REG_LCDC_AMW_L, x, BIT_16_LTOH);
}

UINT16 V5B_LcdcGetAmw(void)
{
	return (UINT16)V5_GetReg(V5_REG_LCDC_AMW_L, BIT_16_LTOH);
}

void V5B_LcdcSetAmh(UINT16 x)
{
	V5_SetReg(V5_REG_LCDC_AMH_L, x, BIT_16_LTOH);
}

UINT16 V5B_LcdcGetAmh(void)
{
	return (UINT16)V5_GetReg(V5_REG_LCDC_AMH_L, BIT_16_LTOH);
}

void V5B_LcdcSetAMSize(TSize size)
{
	V5B_LcdcSetAmw(size.cx);
	V5B_LcdcSetAmh(size.cy);
}

void V5B_LcdcGetAMSize(TSize *size)
{
	size->cx = V5B_LcdcGetAmw();
	size->cy = V5B_LcdcGetAmh();
}

//======================
void V5B_LcdcSetBx0P(UINT16 x)
{
	V5_SetReg(V5_REG_LCDC_BX0_L, x, BIT_16_LTOH);
}

UINT16 V5B_LcdcGetBx0P(void)
{
	return (UINT16)V5_GetReg(V5_REG_LCDC_BX0_L, BIT_16_LTOH);
}

void V5B_LcdcSetBy0P(UINT16 x)
{
	V5_SetReg(V5_REG_LCDC_BY0_L, x, BIT_16_LTOH);
}

UINT16 V5B_LcdcGetBy0P(void)
{
	return (UINT16)V5_GetReg(V5_REG_LCDC_BY0_L, BIT_16_LTOH);
}

void V5B_LcdcSetB0SptPanel(TPoint pt)
{
	V5B_LcdcSetBx0P(pt.x);
	V5B_LcdcSetBy0P(pt.y);
}

void V5B_LcdcGetB0SptPanel(TPoint *pt)
{
	pt->x = V5B_LcdcGetBx0P();
	pt->y = V5B_LcdcGetBy0P();
}

//=========================
void V5B_LcdcSetBw0(UINT16 x)
{
	x--;
	V5_SetReg(V5_REG_LCDC_BW0_L, x, BIT_16_LTOH);
}

UINT16 V5B_LcdcGetBw0(void)
{
	return (UINT16)V5_GetReg(V5_REG_LCDC_BW0_L, BIT_16_LTOH) + 1;
}

void V5B_LcdcSetBh0(UINT16 x)
{
	x--;
	V5_SetReg(V5_REG_LCDC_BH0_L, x, BIT_16_LTOH);
}

UINT16 V5B_LcdcGetBh0(void)
{
	return (UINT16)V5_GetReg(V5_REG_LCDC_BH0_L, BIT_16_LTOH) + 1;
}

void V5B_LcdcSetB0WinSize(TSize size)
{
	V5B_LcdcSetBw0(size.cx);
	V5B_LcdcSetBh0(size.cy);
}

void V5B_LcdcGetB0WinSize(TSize *size)
{
	size->cx = V5B_LcdcGetBw0();
	size->cy = V5B_LcdcGetBh0();
}

//=========================
void V5B_LcdcSetBdx0(UINT16 x)
{
	V5_SetReg(V5_REG_LCDC_BDX0_L, x, BIT_16_LTOH);
}

UINT16 V5B_LcdcGetBdx0(void)
{
	return (UINT16)V5_GetReg(V5_REG_LCDC_BDX0_L, BIT_16_LTOH);
}

void V5B_LcdcSetBdy0(UINT16 x)
{
	V5_SetReg(V5_REG_LCDC_BDY0_L, x, BIT_16_LTOH);
}

UINT16 V5B_LcdcGetBdy0(void)
{
	return (UINT16)V5_GetReg(V5_REG_LCDC_BDY0_L, BIT_16_LTOH);
}

void V5B_LcdcSetB0SptMem(TPoint pt)
{
	V5B_LcdcSetBdx0(pt.x);
	V5B_LcdcSetBdy0(pt.y);
}

void V5B_LcdcGetB0SptMem(TPoint *pt)
{
	pt->x = V5B_LcdcGetBdx0();
	pt->y = V5B_LcdcGetBdy0();
}

void V5B_LcdcSetBdx1(UINT16 x)
{
	V5_SetReg(V5_REG_LCDC_BDX1_L, x, BIT_16_LTOH);
}

UINT16 V5B_LcdcGetBdx1(void)
{
	return (UINT16)V5_GetReg(V5_REG_LCDC_BDX1_L, BIT_16_LTOH);
}

void V5B_LcdcSetBdy1(UINT16 x)
{
	V5_SetReg(V5_REG_LCDC_BDY1_L, x, BIT_16_LTOH);
}

UINT16 V5B_LcdcGetBdy1(void)
{
	return (UINT16)V5_GetReg(V5_REG_LCDC_BDY1_L, BIT_16_LTOH);
}

void V5B_LcdcSetB1SptMem(TPoint pt)
{
	V5B_LcdcSetBdx1(pt.x);
	V5B_LcdcSetBdy1(pt.y);
}

void V5B_LcdcGetB1SptMem(TPoint *pt)
{
	pt->x = V5B_LcdcGetBdx1();
	pt->y = V5B_LcdcGetBdy1();
}

//==========================
void V5B_LcdcSetBmw0(UINT16 x)
{
	x--;
	V5_SetReg(V5_REG_LCDC_BMW0_L, x, BIT_16_LTOH);
}

UINT16 V5B_LcdcGetBmw0(void)
{
	return (UINT16)V5_GetReg(V5_REG_LCDC_BMW0_L, BIT_16_LTOH) + 1;
}

void V5B_LcdcSetBmw1(UINT16 x)
{
	x--;
	V5_SetReg(V5_REG_LCDC_BMW1_L, x, BIT_16_LTOH);
}

UINT16 V5B_LcdcGetBmw1(void)
{
	return (UINT16)V5_GetReg(V5_REG_LCDC_BMW1_L, BIT_16_LTOH) + 1;
}

//============================
void V5B_LcdcSetBgpx(UINT16 x)
{
	V5_SetReg(V5_REG_LCDC_BGX_L, x, BIT_16_LTOH);
}

UINT16 V5B_LcdcGetBgpx(void)
{
	return (UINT16)V5_GetReg(V5_REG_LCDC_BGX_L, BIT_16_LTOH);
}

void V5B_LcdcSetBgpy(UINT16 x)
{
	V5_SetReg(V5_REG_LCDC_BGY_L, x, BIT_16_LTOH);
}

UINT16 V5B_LcdcGetBgpy(void)
{
	return (UINT16)V5_GetReg(V5_REG_LCDC_BGY_L, BIT_16_LTOH);
}

void V5B_LcdcSetBgStartPt(TPoint pt)
{
	V5B_LcdcSetBgpx(pt.x);
	V5B_LcdcSetBgpy(pt.y);
}

void V5B_LcdcGetBgStartPt(TPoint *pt)
{
	pt->x = V5B_LcdcGetBgpx();
	pt->y = V5B_LcdcGetBgpy();
}

//==========================
void V5B_LcdcSetBgw(UINT16 x)
{
	x--;
	V5_SetReg(V5_REG_LCDC_BGW_L, x, BIT_16_LTOH);
}

UINT16 V5B_LcdcGetBgw(void)
{
	return (UINT16)V5_GetReg(V5_REG_LCDC_BGW_L, BIT_16_LTOH) + 1;
}

void V5B_LcdcSetBgh(UINT16 x)
{
	x--;
	V5_SetReg(V5_REG_LCDC_BGH_L, x, BIT_16_LTOH);
}

UINT16 V5B_LcdcGetBgh(void)
{
	return (UINT16)V5_GetReg(V5_REG_LCDC_BGH_L, BIT_16_LTOH) + 1;
}

void V5B_LcdcSetBgSize(TSize size)
{
	V5B_LcdcSetBgw(size.cx);
	V5B_LcdcSetBgh(size.cy);
}

void V5B_LcdcGetBgSize(TSize *size)
{
	size->cx = V5B_LcdcGetBgw();
	size->cy = V5B_LcdcGetBgh();
}
//============================

void V5B_LcdcSetBrx0(UINT16 x)
{
	V5_SetReg(V5_REG_LCDC_BRX0_L, x, BIT_16_LTOH);
}

UINT16 V5B_LcdcGetBrx0(void)
{
	return (UINT16)V5_GetReg(V5_REG_LCDC_BRX0_L, BIT_16_LTOH);
}

void V5B_LcdcSetBry0(UINT16 x)
{
	V5_SetReg(V5_REG_LCDC_BRY0_L, x, BIT_16_LTOH);
}

UINT16 V5B_LcdcGetBry0(void)
{
	return (UINT16)V5_GetReg(V5_REG_LCDC_BRY0_L, BIT_16_LTOH);
}

void V5B_LcdcSetB0rPoint(TPoint pt)
{
	V5B_LcdcSetBrx0(pt.x);
	V5B_LcdcSetBry0(pt.y);
}

void V5B_LcdcGetB0rPoint(TPoint *pt)
{
	pt->x = V5B_LcdcGetBrx0();
	pt->y = V5B_LcdcGetBry0();
}

//====================
void V5B_LcdcSetBrw0(UINT16 x)
{
	x--;
	V5_SetReg(V5_REG_LCDC_BRW0_L, x, BIT_16_LTOH);
}

UINT16 V5B_LcdcGetBrw0(void)
{
	return (UINT16)V5_GetReg(V5_REG_LCDC_BRW0_L, BIT_16_LTOH) + 1;
}

void V5B_LcdcSetBrh0(UINT16 x)
{
	x--;
	V5_SetReg(V5_REG_LCDC_BRH0_L, x, BIT_16_LTOH);
}

UINT16 V5B_LcdcGetBrh0(void)
{
	return (UINT16)V5_GetReg(V5_REG_LCDC_BRH0_L, BIT_16_LTOH) + 1;
}

void V5B_LcdcSetB0rSize(TSize size)
{
	V5B_LcdcSetBrw0(size.cx);
	V5B_LcdcSetBrh0(size.cy);
}

void V5B_LcdcGetB0rSize(TSize *size)
{
	size->cx = V5B_LcdcGetBrw0();
	size->cy = V5B_LcdcGetBrh0();
}
//============

void V5B_LcdcSetB0OverKeyColor(UINT32 x)
{
	V5_SetReg(V5_REG_LCDC_OKC0_L, x, BIT_24_LTOH);
}

UINT32 V5B_LcdcGetB0OverKeyColor(void)
{
	return V5_GetReg(V5_REG_LCDC_OKC0_L, BIT_24_LTOH);
}
//===========

void V5B_LcdcSetB0OkcMode(UINT8 x)
{
	V5_SetReg(V5_REG_LCDC_OKCM0, x, BIT_8);
}

UINT8 V5B_LcdcGetB0OkcMode(void)
{
	return (UINT8)V5_GetReg(V5_REG_LCDC_OKCM0, BIT_8);
}

//=======================

void V5B_LcdcSetB0BlendRatio(UINT8 x)
{
	V5_SetReg(V5_REG_LCDC_BR0_L, x, BIT_8);
}

UINT8 V5B_LcdcGetB0BlendRatio(void)
{
	return (UINT8)V5_GetReg(V5_REG_LCDC_BR0_L, BIT_8);
}
//=======================

void V5B_LcdcSetGbufFormat(UINT8 x)
{
	V5_SetReg(V5_REG_LCDC_GBUFFMT, x, BIT_8);
}

UINT8 V5B_LcdcGetGbufFormat(void)
{
	return (UINT8)V5_GetReg(V5_REG_LCDC_GBUFFMT, BIT_8);
}

//======================
void V5B_LcdcSetBgColor(UINT32 x)
{
	V5_SetReg(V5_REG_LCDC_BGD_L, x, BIT_24_LTOH);
}

UINT32 V5B_LcdcGetBgColor(void)
{
	return V5_GetReg(V5_REG_LCDC_BGD_L, BIT_24_LTOH);
}
//======================
void V5B_LcdcSetPaletteUnit(UINT8 index, UINT32 x)
{
	V5_SetReg(V5_REG_LCDC_PD_L, x, BIT_24_LTOH);
	V5_SetReg(V5_REG_LCDC_PA, index, BIT_8);
}

void V5B_LcdcSetPalette(UINT16 total, UINT32 *x)
{
	UINT16 i;

	for(i = 0; i < total; i++)
		V5B_LcdcSetPaletteUnit((UINT8)i, x[i]);
}

//==========================
void V5B_LcdcSetGammaRUnit(UINT8 index, UINT8 x)
{
	V5_SetReg(V5_REG_LCDC_GLUT_R0 + index, x, BIT_8);
}

UINT8 V5B_LcdcGetGammaRUnit(UINT8 index)
{
	return (UINT8)V5_GetReg(V5_REG_LCDC_GLUT_R0 + index, BIT_8);
}

void V5B_LcdcSetGammaR(UINT8 *x)
{
	UINT8 i;

	for(i = 0; i < 17; i++)
		V5_SetReg(V5_REG_LCDC_GLUT_R0 + i, x[i], BIT_8);
}

void V5B_LcdcSetGammaGUnit(UINT8 index, UINT8 x)
{
	V5_SetReg(V5_REG_LCDC_GLUT_G0 + index, x, BIT_8);
}

UINT8 V5B_LcdcGetGammaGUnit(UINT8 index)
{
	return (UINT8)V5_GetReg(V5_REG_LCDC_GLUT_G0 + index, BIT_8);
}

void V5B_LcdcSetGammaG(UINT8 *x)
{
	UINT8 i;

	for(i = 0; i < 17; i++)
		V5_SetReg(V5_REG_LCDC_GLUT_G0 + i, x[i], BIT_8);
}

void V5B_LcdcSetGammaBUnit(UINT8 index, UINT8 x)
{
	V5_SetReg(V5_REG_LCDC_GLUT_B0 + index, x, BIT_8);
}

UINT8 V5B_LcdcGetGammaBUnit(UINT8 index)
{
	return (UINT8)V5_GetReg(V5_REG_LCDC_GLUT_B0 + index, BIT_8);
}

void V5B_LcdcSetGammaB(UINT8 *x)
{
	UINT8 i;

	for(i = 0; i < 17; i++)
		V5_SetReg(V5_REG_LCDC_GLUT_B0 + i, x[i], BIT_8);
}

//===================
void V5B_LcdcSetDEdelay(UINT8 x)
{
	V5_SetReg(V5_REG_LCDC_DEDLY, x, BIT_8);
}

UINT8 V5B_LcdcGetDEdelay(void)
{
	return (UINT8)V5_GetReg(V5_REG_LCDC_DEDLY, BIT_8);
}

//===================
void V5B_LcdcSetDEinteval(UINT8 x)
{
	V5_SetReg(V5_REG_LCDC_DEINTV, x, BIT_8);
}

UINT8 V5B_LcdcGetDEinteval(void)
{
	return (UINT8)V5_GetReg(V5_REG_LCDC_DEINTV, BIT_8);
}

//===================
void V5B_LcdcSetLineinteval(UINT8 x)
{
	V5_SetReg(V5_REG_LCDC_LINEINTV, x, BIT_8);
}

UINT8 V5B_LcdcGetLineinteval(void)
{
	return (UINT8)V5_GetReg(V5_REG_LCDC_LINEINTV, BIT_8);
}

//===================
void V5B_LcdcSetBx1P(UINT16 x)
{
	V5_SetReg(V5_REG_LCDC_BX1_L, x, BIT_16_LTOH);
}

UINT16 V5B_LcdcGetBx1P(void)
{
	return (UINT16)V5_GetReg(V5_REG_LCDC_BX1_L, BIT_16_LTOH);
}

void V5B_LcdcSetBy1P(UINT16 x)
{
	V5_SetReg(V5_REG_LCDC_BY1_L, x, BIT_16_LTOH);
}

UINT16 V5B_LcdcGetBy1P(void)
{
	return (UINT16)V5_GetReg(V5_REG_LCDC_BY1_L, BIT_16_LTOH);
}

void V5B_LcdcSetB1SptPanel(TPoint pt)
{
	V5B_LcdcSetBx1P(pt.x);
	V5B_LcdcSetBy1P(pt.y);
}

void V5B_LcdcGetB1SptPanel(TPoint *pt)
{
	pt->x = V5B_LcdcGetBx1P();
	pt->y = V5B_LcdcGetBy1P();
}

//=========================
void V5B_LcdcSetBw1(UINT16 x)
{
	x--;
	V5_SetReg(V5_REG_LCDC_BW1_L, x, BIT_16_LTOH);
}

UINT16 V5B_LcdcGetBw1(void)
{
	return (UINT16)V5_GetReg(V5_REG_LCDC_BW1_L, BIT_16_LTOH) + 1;
}

void V5B_LcdcSetBh1(UINT16 x)
{
	x--;
	V5_SetReg(V5_REG_LCDC_BH1_L, x, BIT_16_LTOH);
}

UINT16 V5B_LcdcGetBh1(void)
{
	return (UINT16)V5_GetReg(V5_REG_LCDC_BH1_L, BIT_16_LTOH) + 1;
}

void V5B_LcdcSetB1WinSize(TSize size)
{
	V5B_LcdcSetBw1(size.cx);
	V5B_LcdcSetBh1(size.cy);
}

void V5B_LcdcGetB1WinSize(TSize *size)
{
	size->cx = V5B_LcdcGetBw1();
	size->cy = V5B_LcdcGetBh1();
}

//=========================
void V5B_LcdcSetBrx1(UINT16 x)
{
	V5_SetReg(V5_REG_LCDC_BRX1_L, x, BIT_16_LTOH);
}

UINT16 V5B_LcdcGetBrx1(void)
{
	return (UINT16)V5_GetReg(V5_REG_LCDC_BRX1_L, BIT_16_LTOH);
}

void V5B_LcdcSetBry1(UINT16 x)
{
	V5_SetReg(V5_REG_LCDC_BRY1_L, x, BIT_16_LTOH);
}

UINT16 V5B_LcdcGetBry1(void)
{
	return (UINT16)V5_GetReg(V5_REG_LCDC_BRY1_L, BIT_16_LTOH);
}

void V5B_LcdcSetB1rPoint(TPoint pt)
{
	V5B_LcdcSetBrx1(pt.x);
	V5B_LcdcSetBry1(pt.y);
}

void V5B_LcdcGetB1rPoint(TPoint *pt)
{
	pt->x = V5B_LcdcGetBrx1();
	pt->y = V5B_LcdcGetBry1();
}

//====================
void V5B_LcdcSetBrw1(UINT16 x)
{
	x--;
	V5_SetReg(V5_REG_LCDC_BRW1_L, x, BIT_16_LTOH);
}

UINT16 V5B_LcdcGetBrw1(void)
{
	return (UINT16)V5_GetReg(V5_REG_LCDC_BRW1_L, BIT_16_LTOH) + 1;
}

void V5B_LcdcSetBrh1(UINT16 x)
{
	x--;
	V5_SetReg(V5_REG_LCDC_BRH1_L, x, BIT_16_LTOH);
}

UINT16 V5B_LcdcGetBrh1(void)
{
	return (UINT16)V5_GetReg(V5_REG_LCDC_BRH1_L, BIT_16_LTOH) + 1;
}

void V5B_LcdcSetB1rSize(TSize size)
{
	V5B_LcdcSetBrw1(size.cx);
	V5B_LcdcSetBrh1(size.cy);
}

void V5B_LcdcGetB1rSize(TSize *size)
{
	size->cx = V5B_LcdcGetBrw1();
	size->cy = V5B_LcdcGetBrh1();
}

//=======================
void V5B_LcdcSetB1OverKeyColor(UINT32 x)
{
	V5_SetReg(V5_REG_LCDC_OKC1_L, x, BIT_24_LTOH);
}

UINT32 V5B_LcdcGetB1OverKeyColor(void)
{
	return V5_GetReg(V5_REG_LCDC_OKC1_L, BIT_24_LTOH);
}

//===========
void V5B_LcdcSetB1OkcMode(UINT8 x)
{
	V5_SetReg(V5_REG_LCDC_OKCM1, x, BIT_8);
}

UINT8 V5B_LcdcGetB1OkcMode(void)
{
	return (UINT8)V5_GetReg(V5_REG_LCDC_OKCM1, BIT_8);
}

//===========
void V5B_LcdcSetB1BlendRatio(UINT8 x)
{
	V5_SetReg(V5_REG_LCDC_BR1_L, x, BIT_8);
}

UINT8 V5B_LcdcGetB1BlendRatio(void)
{
	return (UINT8)V5_GetReg(V5_REG_LCDC_BR1_L, BIT_8);
}

#endif //V578CMD_DISABLE
