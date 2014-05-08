


#ifndef _RDK_LCDC_H_
#define _RDK_LCDC_H_

#ifdef __cplusplus
extern "C" {
#endif

	enum DITHERSELenum {
		DITHER444,
		DITHER565,
		DITHER666
	};

	enum DISPISTenum {
		IST_UPDATEB,
		IST_UPDATEBG,
		IST_FIFOWARNING,
		IST_VBTIMEOUT,
		IST_GBTIMEOUT
	};

//===================
void	V5B_LcdcSetDc(UINT16 x);
UINT16	V5B_LcdcGetDc(void);

UINT16	V5B_LcdcEnableDither(void);
UINT16	V5B_LcdcDisableDither(void);
void	V5B_LcdcSetDitherSel(UINT8 sel);
UINT8	V5B_LcdcGetDitherSel(void);
UINT16	V5B_LcdcEnableGamma(void);
UINT16	V5B_LcdcDisableGamma(void);

void	V5B_LcdcSetB0OverlayMode(UINT8 sel);
UINT8	V5B_LcdcGetB0OverlayMode(void);
void	V5B_LcdcSetB1OverlayMode(UINT8 sel);
UINT8	V5B_LcdcGetB1OverlayMode(void);

UINT16	V5B_LcdcEnableOverlay(void);
UINT16	V5B_LcdcDisableOverlay(void);
UINT16	V5B_LcdcEnableBlending(void);
UINT16	V5B_LcdcDisableBlending(void);

UINT16	V5B_LcdcLayerB0First(void);
UINT16	V5B_LcdcLayerB0FstOff(void);
UINT16	V5B_LcdcLayerB1First(void);
UINT16	V5B_LcdcLayerB1FstOff(void);
void	V5B_LcdcLayerAFirst(void);

UINT16	V5B_LcdcDecodeMode(void);
UINT16	V5B_LcdcNormalMode(void);

void	V5B_LcdcSetAlayerFmt(UINT8 fmt);
UINT8	V5B_LcdcGetAlayerFmt(void);

//===================
void	V5B_LcdcSetDe(UINT8 x);
UINT8	V5B_LcdcGetDe(void);

void	V5B_LcdcClearEOF(void);
UINT8	V5B_LcdcCheckEOF(void);
UINT8	V5B_LcdcEnableAOperation(void);
UINT8	V5B_LcdcEnableAlayer(void);
UINT8	V5B_LcdcDisableAlayer(void);
UINT8	V5B_LcdcEnableB0(void);
UINT8	V5B_LcdcDisableB0(void);
UINT8	V5B_LcdcEnableBG(void);
UINT8	V5B_LcdcDisableBG(void);
UINT8	V5B_LcdcEnableB1(void);
UINT8	V5B_LcdcDisableB1(void);

//====================
void	V5B_LcdcResetState(void);

//====================
void	V5B_LcdcUpdate(UINT8 x);
UINT8	V5B_LcdcGetUpdate(void);
void	V5B_LcdcSetOLP(UINT8 sel);
UINT8	V5B_LcdcGetOLP(void);
void	V5B_LcdcB0layerUpdate(void);
void	V5B_LcdcB1layerUpdate(void);
void	V5B_LcdcBgUpdate(void);

//========================
void	V5B_LcdcSetRotateMode(UINT8 x);
UINT8	V5B_LcdcGetRotateMode(void);
void	V5B_LcdcSetAlayerRM(UINT8 x);
UINT8	V5B_LcdcGetAlayerRM(void);
void	V5B_LcdcSetBlayerRM(UINT8 x);
UINT8	V5B_LcdcGetBlayerRM(void);

//==========================
void	V5B_LcdcSetVbufBa0(UINT32 x);
UINT32	V5B_LcdcGetVbufBa0(void);
void	V5B_LcdcSetVbufBa1(UINT32 x);
UINT32	V5B_LcdcGetVbufBa1(void);

//===================
void	V5B_LcdcSetGbufBa0(UINT32 x);
UINT32	V5B_LcdcGetGbufBa0(void);
void	V5B_LcdcSetGbufBa1(UINT32 x);
UINT32	V5B_LcdcGetGbufBa1(void);

//====================
void	V5B_LcdcSetAxP(UINT16 x);
UINT16	V5B_LcdcGetAxP(void);
void	V5B_LcdcSetAyP(UINT16 x);
UINT16	V5B_LcdcGetAyP(void);
void	V5B_LcdcSetASptPanel(TPoint pt);
void	V5B_LcdcGetASptPanel(TPoint *pt);

//===================
void	V5B_LcdcSetAw(UINT16 x);
UINT16	V5B_LcdcGetAw(void);
void	V5B_LcdcSetAh(UINT16 x);
UINT16	V5B_LcdcGetAh(void);
void	V5B_LcdcSetAWinSize(TSize size);
void	V5B_LcdcGetAWinSize(TSize *size);

//====================
void	V5B_LcdcSetAdx0(UINT16 x);
UINT16	V5B_LcdcGetAdx0(void);
void	V5B_LcdcSetAdy0(UINT16 x);
UINT16	V5B_LcdcGetAdy0(void);
void	V5B_LcdcSetASptMem(TPoint pt);
void	V5B_LcdcGetASptMem(TPoint *pt);

//=================
void	V5B_LcdcSetAmw(UINT16 x);
UINT16	V5B_LcdcGetAmw(void);
void	V5B_LcdcSetAmh(UINT16 x);
UINT16	V5B_LcdcGetAmh(void);
void	V5B_LcdcSetAMSize(TSize size);
void	V5B_LcdcGetAMSize(TSize *size);

//======================
void	V5B_LcdcSetBx0P(UINT16 x);
UINT16	V5B_LcdcGetBx0P(void);
void	V5B_LcdcSetBy0P(UINT16 x);
UINT16	V5B_LcdcGetBy0P(void);
void	V5B_LcdcSetB0SptPanel(TPoint pt);
void	V5B_LcdcGetB0SptPanel(TPoint *pt);

//======================
void	V5B_LcdcSetBx1P(UINT16 x);
UINT16	V5B_LcdcGetBx1P(void);
void	V5B_LcdcSetBy1P(UINT16 x);
UINT16	V5B_LcdcGetBy1P(void);
void	V5B_LcdcSetB1SptPanel(TPoint pt);
void	V5B_LcdcGetB1SptPanel(TPoint *pt);

//=========================
void	V5B_LcdcSetBw0(UINT16 x);
UINT16	V5B_LcdcGetBw0(void);
void	V5B_LcdcSetBh0(UINT16 x);
UINT16	V5B_LcdcGetBh0(void);
void	V5B_LcdcSetB0WinSize(TSize size);
void	V5B_LcdcGetB0WinSize(TSize *size);

//=========================
void	V5B_LcdcSetBw1(UINT16 x);
UINT16	V5B_LcdcGetBw1(void);
void	V5B_LcdcSetBh1(UINT16 x);
UINT16	V5B_LcdcGetBh1(void);
void	V5B_LcdcSetB1WinSize(TSize size);
void	V5B_LcdcGetB1WinSize(TSize *size);

//=========================
void	V5B_LcdcSetBdx0(UINT16 x);
UINT16	V5B_LcdcGetBdx0(void);
void	V5B_LcdcSetBdy0(UINT16 x);
UINT16	V5B_LcdcGetBdy0(void);
void	V5B_LcdcSetB0SptMem(TPoint pt);
void	V5B_LcdcGetB0SptMem(TPoint *pt);

//==============================
void	V5B_LcdcSetBdx1(UINT16 x);
UINT16	V5B_LcdcGetBdx1(void);
void	V5B_LcdcSetBdy1(UINT16 x);
UINT16	V5B_LcdcGetBdy1(void);
void	V5B_LcdcSetB1SptMem(TPoint pt);
void	V5B_LcdcGetB1SptMem(TPoint *pt);

//==========================
void	V5B_LcdcSetBmw0(UINT16 x);
UINT16	V5B_LcdcGetBmw0(void);
void	V5B_LcdcSetBmw1(UINT16 x);
UINT16	V5B_LcdcGetBmw1(void);

//============================
void	V5B_LcdcSetBgpx(UINT16 x);
UINT16	V5B_LcdcGetBgpx(void);
void	V5B_LcdcSetBgpy(UINT16 x);
UINT16	V5B_LcdcGetBgpy(void);
void	V5B_LcdcSetBgStartPt(TPoint pt);
void	V5B_LcdcGetBgStartPt(TPoint *pt);

//==========================
void	V5B_LcdcSetBgw(UINT16 x);
UINT16	V5B_LcdcGetBgw(void);
void	V5B_LcdcSetBgh(UINT16 x);
UINT16	V5B_LcdcGetBgh(void);
void	V5B_LcdcSetBgSize(TSize size);
void	V5B_LcdcGetBgSize(TSize *size);

//============================
void	V5B_LcdcSetBrx0(UINT16 x);
UINT16	V5B_LcdcGetBrx0(void);
void	V5B_LcdcSetBry0(UINT16 x);
UINT16	V5B_LcdcGetBry0(void);
void	V5B_LcdcSetB0rPoint(TPoint pt);
void	V5B_LcdcGetB0rPoint(TPoint *pt);

//============================
void	V5B_LcdcSetBrx1(UINT16 x);
UINT16	V5B_LcdcGetBrx1(void);
void	V5B_LcdcSetBry1(UINT16 x);
UINT16	V5B_LcdcGetBry1(void);
void	V5B_LcdcSetB1rPoint(TPoint pt);
void	V5B_LcdcGetB1rPoint(TPoint *pt);

//====================
void	V5B_LcdcSetBrw0(UINT16 x);
UINT16	V5B_LcdcGetBrw0(void);
void	V5B_LcdcSetBrh0(UINT16 x);
UINT16	V5B_LcdcGetBrh0(void);
void	V5B_LcdcSetB0rSize(TSize size);
void	V5B_LcdcGetB0rSize(TSize *size);

//====================
void	V5B_LcdcSetBrw1(UINT16 x);
UINT16	V5B_LcdcGetBrw1(void);
void	V5B_LcdcSetBrh1(UINT16 x);
UINT16	V5B_LcdcGetBrh1(void);
void	V5B_LcdcSetB1rSize(TSize size);
void	V5B_LcdcGetB1rSize(TSize *size);

//============
void	V5B_LcdcSetB0OverKeyColor(UINT32 x);
void	V5B_LcdcSetB1OverKeyColor(UINT32 x);
UINT32	V5B_LcdcGetB0OverKeyColor(void);
UINT32	V5B_LcdcGetB1OverKeyColor(void);

//============
void	V5B_LcdcSetB0OkcMode(UINT8 x);
UINT8	V5B_LcdcGetB0OkcMode(void);
void	V5B_LcdcSetB1OkcMode(UINT8 x);
UINT8	V5B_LcdcGetB1OkcMode(void);

//===========
void	V5B_LcdcSetB0BlendRatio(UINT8 x);
UINT8	V5B_LcdcGetB0BlendRatio(void);
void	V5B_LcdcSetB1BlendRatio(UINT8 x);
UINT8	V5B_LcdcGetB1BlendRatio(void);

//=======================
void	V5B_LcdcSetGbufFormat(UINT8 x);
UINT8	V5B_LcdcGetGbufFormat(void);

//======================
void	V5B_LcdcSetBgColor(UINT32 x);
UINT32	V5B_LcdcGetBgColor(void);

//======================
void	V5B_LcdcSetPaletteUnit(UINT8 index, UINT32 x);
void	V5B_LcdcSetPalette(UINT16 total, UINT32 *x);

//==========================
void	V5B_LcdcSetGammaRUnit(UINT8 index, UINT8 x);
UINT8	V5B_LcdcGetGammaRUnit(UINT8 index);
void	V5B_LcdcSetGammaR(UINT8 *x);
void	V5B_LcdcSetGammaGUnit(UINT8 index, UINT8 x);
UINT8	V5B_LcdcGetGammaGUnit(UINT8 index);
void	V5B_LcdcSetGammaG(UINT8 *x);
void	V5B_LcdcSetGammaBUnit(UINT8 index, UINT8 x);
UINT8	V5B_LcdcGetGammaBUnit(UINT8 index);
void	V5B_LcdcSetGammaB(UINT8 *x);

//===================
void	V5B_LcdcSetDEdelay(UINT8 x);
UINT8	V5B_LcdcGetDEdelay(void);

//===================
void	V5B_LcdcSetDEinteval(UINT8 x);
UINT8	V5B_LcdcGetDEinteval(void);

//===================
void	V5B_LcdcSetLineinteval(UINT8 x);
UINT8	V5B_LcdcGetLineinteval(void);

#ifdef __cplusplus
}
#endif

#endif


