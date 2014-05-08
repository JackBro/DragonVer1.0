
#include "basefunc.h"
#ifndef	V578CMD_DISABLE

void V5B_GeSetBitWise(UINT8 bitwise)
{
	V5_SetReg(V5_REG_GE_BITBLT_CTL0, bitwise, BIT_8);
}

UINT8 V5B_GeGetBitWise(void)
{
	return (UINT8)V5_GetReg(V5_REG_GE_BITBLT_CTL0, BIT_8);
}
//===============================

void V5B_GeSetOpmode(UINT8 x)
{
	V5_SetReg(V5_REG_GE_BITBLT_CTL1, x, BIT_8);
}

UINT8 V5B_GeGetOpmode(void)
{
	return (UINT8)V5_GetReg(V5_REG_GE_BITBLT_CTL1, BIT_8);
}

void V5B_GeSetStartPoint(UINT8 x)
{
	UINT8 y;

	y = V5B_GeGetOpmode();
	y &= 0xfc;
	y |= x & 3;
	V5B_GeSetOpmode(y);
}

UINT8 V5B_GeGetStartPoint(void)
{
	UINT8 y;

	y = V5B_GeGetOpmode();
	return y & 3;
}

UINT8 V5B_GeMonoSrcUseSrc(void)
{
	UINT8 y;

	y = V5B_GeGetOpmode();
	V5B_GeSetOpmode((UINT8)(y | SRC_EXTEND_EN));
	return y;
}

UINT8 V5B_GeMonoSrcUsePtnSrc(void)
{
	UINT8 y;

	y = V5B_GeGetOpmode();
	V5B_GeSetOpmode((UINT8)(y & ~SRC_EXTEND_EN));
	return y;
}

UINT8 V5B_GeMonoSrc(void)
{
	UINT8 y;

	y = V5B_GeGetOpmode();
	V5B_GeSetOpmode((UINT8)(y | SRC_MONO_EN));
	return y;
}

UINT8 V5B_GeColorSrc(void)
{
	UINT8 y;

	y = V5B_GeGetOpmode();
	V5B_GeSetOpmode((UINT8)(y & ~SRC_MONO_EN));
	return y;
}

UINT8 V5B_GeEnableSrcMask(void)
{
	UINT8 y;

	y = V5B_GeGetOpmode();
	V5B_GeSetOpmode((UINT8)(y | SRC_MONO_WRMASK));
	return y;
}

UINT8 V5B_GeDisableSrcMask(void)
{
	UINT8 y;

	y = V5B_GeGetOpmode();
	V5B_GeSetOpmode((UINT8)(y & ~SRC_MONO_WRMASK));
	return y;
}

UINT8 V5B_GeEnableTransparent(void)
{
	UINT8 y;

	y = V5B_GeGetOpmode();
	V5B_GeSetOpmode((UINT8)(y | TRANSPARENT_EN));
	return y;
}

UINT8 V5B_GeDisableTransparent(void)
{
	UINT8 y;

	y = V5B_GeGetOpmode();
	V5B_GeSetOpmode((UINT8)(y & ~TRANSPARENT_EN));
	return y;
}

void V5B_GeSetTransparent(UINT8 mode)
{
	UINT8 y;

	y = V5B_GeGetOpmode();
	y &= 0x3f;
	y |= (mode & 3) << 6;
	V5B_GeSetOpmode(y);
}

UINT8 V5B_GeGetTransparent(void)
{
	UINT8 y;

	y = V5B_GeGetOpmode();
	return (y >> 6) & 0x3;
}

//============================

void V5B_GeSetOpmode1(UINT8 x)
{
	V5_SetReg(V5_REG_GE_BITBLT_CTL2, x, BIT_8);
}

UINT8 V5B_GeGetOpmode1(void)
{
	return (UINT8)V5_GetReg(V5_REG_GE_BITBLT_CTL2, BIT_8);
}

UINT8 V5B_GeEnablePtnMask(void)
{
	UINT8 y;

	y = V5B_GeGetOpmode1();
	V5B_GeSetOpmode1((UINT8)(y | PTN_MONO_WRMASK));
	return y;
}

UINT8 V5B_GeDisablePtnMask(void)
{
	UINT8 y;

	y = V5B_GeGetOpmode1();
	V5B_GeSetOpmode1((UINT8)(y & ~PTN_MONO_WRMASK));
	return y;
}

UINT8 V5B_GeMonoPtn(void)
{
	UINT8 y;

	y = V5B_GeGetOpmode1();
	V5B_GeSetOpmode1((UINT8)(y | PTN_MONO_EN));
	return y;
}

UINT8 V5B_GeColorPtn(void)
{
	UINT8 y;

	y = V5B_GeGetOpmode1();
	V5B_GeSetOpmode1((UINT8)(y & ~PTN_MONO_EN));
	return y;
}

void V5B_GeSetPtnVertAlign(UINT8 line)
{
	UINT8 y;

	y = V5B_GeGetOpmode1();
	y &= 0xe3;
	y |= (line & 7) << 2;
	V5B_GeSetOpmode1(y);
}

UINT8 V5B_GeGetPtnVertAlign(void)
{
	UINT8 y;

	y = V5B_GeGetOpmode1();
	return (y >> 3) & 7;
}

void V5B_GeSetColordepth(UINT8 x)
{
	UINT8 y;

	y = V5B_GeGetOpmode1();
	y &= 0x1f;
	y |= (x & 7) << 5;
	V5B_GeSetOpmode1(y);
}

UINT8 V5B_GeGetColordepth(void)
{
	UINT8 y;

	y = V5B_GeGetOpmode1();
	return (y >> 5) & 7;
}
//==========================

void V5B_GeStartOper(UINT8 x)
{
	V5_SetReg(V5_REG_GE_BITBLT_CTL3, x, BIT_8);
}

UINT8 V5B_GeGetStatus(void)
{
	return (UINT8)V5_GetReg(V5_REG_GE_BITBLT_CTL3, BIT_8);
}

void V5B_GeStartLineDraw(void)
{
	V5_SetReg(V5_REG_GE_BITBLT_CTL3, 0x20, BIT_8);
}

void V5B_GeStartBitblt(void)
{
	V5_SetReg(V5_REG_GE_BITBLT_CTL3, 0x40, BIT_8);
}

void V5B_GeReset(void)
{
	V5_SetReg(V5_REG_GE_BITBLT_CTL3, 0x08, BIT_8);
}
//===============

void V5B_GeSetPtnDW(UINT8 adr, UINT32 val)
{
	V5_SetReg(V5_REG_GE_PATTERNPORT_0, val, BIT_32_LTOH);
	V5_SetReg(V5_REG_GE_PATTERN_WRT, 0, BIT_8);
	V5_SetReg(V5_REG_GE_PATTERN_ADR, adr, BIT_8);
}

void V5B_GeSetPattern(UINT8 cnt, UINT32 *val)
{
	UINT8 i;

	for(i = 0; i < cnt; i++)
		V5B_GeSetPtnDW(i, val[i]);
}
//=========================


void V5B_GeSetPtnBkColor(UINT32 x)
{
	V5_SetReg(V5_REG_GE_PAT_SRC_BG0, x, BIT_24_LTOH);
}

UINT32 V5B_GeGetPtnBkColor(void)
{
	return 	V5_GetReg(V5_REG_GE_PAT_SRC_BG0, BIT_24_LTOH);
}
//=======================

void V5B_GeSetPtnForeColor(UINT32 x)
{
	V5_SetReg(V5_REG_GE_PAT_SRC_FG0, x, BIT_24_LTOH);
}

UINT32 V5B_GeGetPtnForeColor(void)
{
	return 	V5_GetReg(V5_REG_GE_PAT_SRC_FG0, BIT_24_LTOH);
}
//======================

void V5B_GeSetSrcBkColor(UINT32 x)
{
	V5_SetReg(V5_REG_GE_SRC_BG0, x, BIT_24_LTOH);
}

UINT32 V5B_GeGetSrcBkColor(void)
{
	return 	V5_GetReg(V5_REG_GE_SRC_BG0, BIT_24_LTOH);
}
//==============================

void V5B_GeSetSrcForeColor(UINT32 x)
{
	V5_SetReg(V5_REG_GE_SRC_FG0, x, BIT_24_LTOH);
}

UINT32 V5B_GeGetSrcForeColor(void)
{
	return 	V5_GetReg(V5_REG_GE_SRC_FG0, BIT_24_LTOH);
}
//============================

void V5B_SetDstWidth(UINT16 x)
{
	V5_SetReg(V5_REG_GE_DES_WIDTH_L, x, BIT_16_LTOH);
}

UINT16 V5B_GetDstWidth(void)
{
	return 	(UINT16)V5_GetReg(V5_REG_GE_DES_WIDTH_L, BIT_16_LTOH);
}

void V5B_SetDstHeight(UINT16 x)
{
	V5_SetReg(V5_REG_GE_DES_HEIGHT_L, x, BIT_16_LTOH);
}

UINT16 V5B_GetDstHeight(void)
{
	return 	(UINT16)V5_GetReg(V5_REG_GE_DES_HEIGHT_L, BIT_16_LTOH);
}

void V5B_SetDstSize(TSize size)
{
	V5B_SetDstWidth(size.cx);
	V5B_SetDstHeight(size.cy);
}

void V5B_GetDstSize(TSize *size)
{
	size->cx = V5B_GetDstWidth();
	size->cy = V5B_GetDstHeight();
}
//==============================

void V5B_GeSetSrcStartAddr(UINT32 x)
{
	V5_SetReg(V5_REG_GE_SRC_ADDR0, x, BIT_24_LTOH);
}

UINT32 V5B_GeGetSrcStartAddr(void)
{
	return 	V5_GetReg(V5_REG_GE_SRC_ADDR0, BIT_24_LTOH);
}
//================================

void V5B_GeSetSrcLineColor(UINT32 x)
{
	V5_SetReg(V5_REG_GE_LINECOLOR0, x, BIT_24_LTOH);
}

UINT32 V5B_GeGetSrcLineColor(void)
{
	return 	V5_GetReg(V5_REG_GE_LINECOLOR0, BIT_24_LTOH);
}
//===========================

void V5B_GeSetDstStartAddr(UINT32 x)
{
	V5_SetReg(V5_REG_GE_DES_ADDR0, x, BIT_24_LTOH);
}

UINT32 V5B_GeGetDstStartAddr(void)
{
	return 	V5_GetReg(V5_REG_GE_DES_ADDR0, BIT_24_LTOH);
}
//========================

void V5B_GeSetSrcSpan(UINT16 x)
{
	V5_SetReg(V5_REG_GE_SRC_SPAN0, x, BIT_16_LTOH);
}

UINT16 V5B_GeGetSrcSpan(void)
{
	return 	(UINT16)V5_GetReg(V5_REG_GE_SRC_SPAN0, BIT_16_LTOH);
}
//==============================

void V5B_GeSetDstSpan(UINT16 x)
{
	V5_SetReg(V5_REG_GE_DST_SPAN0, x, BIT_16_LTOH);
}

UINT16 V5B_GeGetDstSpan(void)
{
	return 	(UINT16)V5_GetReg(V5_REG_GE_DST_SPAN0, BIT_16_LTOH);
}
//===============================

void V5B_GeSetClipStartpx(UINT16 x)
{
	V5_SetReg(V5_REG_GE_CLIPRECT_LTX_L, x, BIT_16_LTOH);
}

UINT16 V5B_GeGetClipStartpx(void)
{
	return 	(UINT16)V5_GetReg(V5_REG_GE_CLIPRECT_LTX_L, BIT_16_LTOH);
}

void V5B_GeSetClipStartpy(UINT16 x)
{
	V5_SetReg(V5_REG_GE_CLIPRECT_LTY_L, x, BIT_16_LTOH);
}

UINT16 V5B_GeGetClipStartpy(void)
{
	return 	(UINT16)V5_GetReg(V5_REG_GE_CLIPRECT_LTY_L, BIT_16_LTOH);
}

void V5B_GeSetClipStartPt(TPoint pt)
{
	V5B_GeSetClipStartpx(pt.x);
	V5B_GeSetClipStartpy(pt.y);
}

void V5B_GeGetClipStartPt(TPoint *pt)
{
	pt->x = V5B_GeGetClipStartpx();
	pt->y = V5B_GeGetClipStartpy();
}

void V5B_GeSetClipEndpx(UINT16 x)
{
	V5_SetReg(V5_REG_GE_CLIPRECT_RBX_L, x, BIT_16_LTOH);
}

UINT16 V5B_GeGetClipEndpx(void)
{
	return 	(UINT16)V5_GetReg(V5_REG_GE_CLIPRECT_RBX_L, BIT_16_LTOH);
}

void V5B_GeSetClipEndpy(UINT16 x)
{
	V5_SetReg(V5_REG_GE_CLIPRECT_RBY_L, x, BIT_16_LTOH);
}

UINT16 V5B_GeGetClipEndpy(void)
{
	return 	(UINT16)V5_GetReg(V5_REG_GE_CLIPRECT_RBY_L, BIT_16_LTOH);
}

void V5B_GeSetClipEndPt(TPoint pt)
{
	V5B_GeSetClipEndpx(pt.x);
	V5B_GeSetClipEndpy(pt.y);
}

void V5B_GeGetClipEndPt(TPoint *pt)
{
	pt->x = V5B_GeGetClipEndpx();
	pt->y = V5B_GeGetClipEndpy();
}

void V5B_GeSetClipping(TClip clip)
{
	V5B_GeSetClipStartPt(clip.ltp);
	V5B_GeSetClipEndPt(clip.rbp);
}

void V5B_GeGetClipping(TClip *clip)
{
	V5B_GeGetClipStartPt(&(clip->ltp));
	V5B_GeGetClipEndPt(&(clip->rbp));
}

//========================
void V5B_GeSetLineStartpx(UINT16 x)
{
	V5_SetReg(V5_REG_GE_LINE_STARTPX_L, x, BIT_16_LTOH);
}

UINT16 V5B_GeGetLineStartpx(void)
{
	return 	(UINT16)V5_GetReg(V5_REG_GE_LINE_STARTPX_L, BIT_16_LTOH);
}

void V5B_GeSetLineStartpy(UINT16 x)
{
	V5_SetReg(V5_REG_GE_LINE_STARTPY_L, x, BIT_16_LTOH);
}

UINT16 V5B_GeGetLineStartpy(void)
{
	return 	(UINT16)V5_GetReg(V5_REG_GE_LINE_STARTPY_L, BIT_16_LTOH);
}

void V5B_GeSetLineStartPt(TPoint pt)
{
	V5B_GeSetLineStartpx(pt.x);
	V5B_GeSetLineStartpy(pt.y);
}

void V5B_GeGetLineStartPt(TPoint *pt)
{
	pt->x = V5B_GeGetLineStartpx();
	pt->y = V5B_GeGetLineStartpy();
}

void V5B_GeSetLineEndpx(UINT16 x)
{
	V5_SetReg(V5_REG_GE_LINE_ENDPX_L, x, BIT_16_LTOH);
}

UINT16 V5B_GeGetLineEndpx(void)
{
	return 	(UINT16)V5_GetReg(V5_REG_GE_LINE_ENDPX_L, BIT_16_LTOH);
}

void V5B_GeSetLineEndpy(UINT16 x)
{
	V5_SetReg(V5_REG_GE_LINE_ENDPY_L, x, BIT_16_LTOH);
}

UINT16 V5B_GeGetLineEndpy(void)
{
	return 	(UINT16)V5_GetReg(V5_REG_GE_LINE_ENDPY_L, BIT_16_LTOH);
}

void V5B_GeSetLineEndPt(TPoint pt)
{
	V5B_GeSetLineEndpx(pt.x);
	V5B_GeSetLineEndpy(pt.y);
}

void V5B_GeGetLineEndPt(TPoint *pt)
{
	pt->x = V5B_GeGetLineEndpx();
	pt->y = V5B_GeGetLineEndpy();
}


//======================
//
//==================
void V5B_GeDrawLine(TPoint ps, TPoint pe, UINT32 color)
{
	UINT16 flag = 0x7fff;

	V5B_GeSetLineStartPt(ps);
	V5B_GeSetLineEndPt(pe);
	V5B_GeSetSrcLineColor(color);
	V5B_GeStartLineDraw();
	while(V5B_GeGetStatus() && flag)
		flag--;
}

void V5B_GeBitblt(UINT32 srcstart, UINT32 dststart, TSize size, UINT8 bitwise)
{
	UINT16 flag = 0x7fff;

	V5B_GeSetSrcStartAddr(srcstart);
	V5B_GeSetDstStartAddr(dststart);
	V5B_SetDstSize(size);
	V5B_GeSetBitWise(bitwise);
	V5B_GeStartBitblt();
	while(V5B_GeGetStatus() && flag)
		flag--;
}

#endif //V578CMD_DISABLE
