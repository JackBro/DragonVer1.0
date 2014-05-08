#include "basefunc.h"
#ifndef	V578CMD_DISABLE

/*********************************************************************
*                         SPECIAL CONTROL                            *
*********************************************************************/

void V5B_IppSetSpeCtrl(UINT8 sel)
{
	V5_SetReg(V5_REG_IPP_SPECTRL, (UINT32)sel, BIT_8);
}

UINT8 V5B_IppGetSpeCtrl(void)
{
	UINT8 x = 0;

	x = (UINT8)V5_GetReg(V5_REG_IPP_SPECTRL, BIT_8);
	return x;
}

void V5B_IppEnableSpecial(void)
{
	UINT8 x = 0;

	x = V5B_IppGetSpeCtrl();
	V5B_IppSetSpeCtrl(x | 0x01);
}

void V5B_IppDisableSpecial(void)
{
	UINT8 x = 0;

	x = V5B_IppGetSpeCtrl();
	V5B_IppSetSpeCtrl(x & 0xfe);
}

UINT8 V5B_IppIsSpecial(void)
{
	UINT8 x = 0;

	x = (UINT8)V5_GetReg(V5_REG_IPP_SPECTRL, BIT_8);
	return (x & 0x01);
}

void V5B_IppSetWorkMode(UINT8 x)
{
	UINT8 tmp = 0;

	tmp = V5B_IppGetSpeCtrl();
	tmp = (tmp & 0x9f) | ((x & 0x03) << 5);
	V5_SetReg(V5_REG_IPP_SPECTRL, tmp, BIT_8);
}

UINT8 V5B_IppGetWorkMode(void)
{
	UINT8 x = 0;

	x = V5B_IppGetSpeCtrl();
	return ((x & 0x60) >> 5);
}

void V5B_IppSetSpecialEffect(UINT8 x)
{
	UINT8 tmp = 0;

	tmp = V5B_IppGetSpeCtrl();
	tmp = (tmp & 0xe1) | ((x & 0x0f) << 1);
	V5_SetReg(V5_REG_IPP_SPECTRL, tmp, BIT_8);
}

UINT8 V5B_IppGetSpecialEffect(void)
{
	UINT8 x = 0;

	x = V5B_IppGetSpeCtrl();
	return ((x & 0x1e) >> 1);
}

/*********************************************************************
 *                         SIZER CONTROL 0                           *
 *********************************************************************/

void V5B_IppSetSizerCtrl0(UINT8 x)
{
	V5_SetReg(V5_REG_IPP_SIZCTRL0, x, BIT_8);
}

UINT8 V5B_IppGetSizerCtrl0(void)
{
	UINT8 x = 0;

	x = (UINT8)V5_GetReg(V5_REG_IPP_SIZCTRL0, BIT_8);
	return x;
}

void V5B_IppEnableDispSizer(void)
{
	UINT8 x = 0;

	x = V5B_IppGetSizerCtrl0();
	x |= IPP_DISP_SIZER_EN;
	V5B_IppSetSizerCtrl0(x);
}

void V5B_IppDisableDispSizer(void)
{
	UINT8 x = 0;

	x = V5B_IppGetSizerCtrl0();
	x &= ~IPP_DISP_SIZER_EN;
	V5B_IppSetSizerCtrl0(x);
}

void V5B_IppEnableCapDnSizer(void)
{
	UINT8 x = 0;

	x = V5B_IppGetSizerCtrl0();
	x |= IPP_CAP_DSIZER_EN;
	V5B_IppSetSizerCtrl0(x);
}

void V5B_IppDisableCapDnSizer(void)
{
	UINT8 x = 0;

	x = V5B_IppGetSizerCtrl0();
	x &= ~IPP_CAP_DSIZER_EN;
	V5B_IppSetSizerCtrl0(x);
}

void V5B_IppEnableCapUpSizer(void)
{
	UINT8 x = 0;

	x = V5B_IppGetSizerCtrl0();
	x |= IPP_CAP_USIZER_EN;
	V5B_IppSetSizerCtrl0(x);
}

void V5B_IppDisableCapUpSizer(void)
{
	UINT8 x = 0;

	x = V5B_IppGetSizerCtrl0();
	x &= ~IPP_CAP_USIZER_EN;
	V5B_IppSetSizerCtrl0(x);
}

void V5B_IppEnableThumbSizer(void)
{
	UINT8 x = 0;

	x = V5B_IppGetSizerCtrl0();
	x |= IPP_TMB_SIZER_EN;
	V5B_IppSetSizerCtrl0(x);
}

void V5B_IppDisableThumbSizer(void)
{
	UINT8 x = 0;

	x = V5B_IppGetSizerCtrl0();
	x &= ~IPP_TMB_SIZER_EN;
	V5B_IppSetSizerCtrl0(x);
}

void V5B_IppReset(void)
{
	UINT8 x = 0;

	x = V5B_IppGetSizerCtrl0();
	x |= IPP_RESET_TRG;
	V5B_IppSetSizerCtrl0(x);
}

void V5B_IppSetDataSelect(UINT8 sel)
{
	UINT8 x = 0;

	x = V5B_IppGetSizerCtrl0();
	x = (x & 0x9f) | ((sel & 0x03) << 5);
	V5B_IppSetSizerCtrl0(x);
}

UINT8 V5B_IppGetDataSelect(void)
{
	UINT8 x = 0;

	x = V5B_IppGetSizerCtrl0();
	return ((x >> 5) & 0x03);
}

void V5B_IppEnableFrame(void)
{
	UINT8 x = 0;

	x = V5B_IppGetSizerCtrl0();
	x |= IPP_FRAME_EN;
	V5B_IppSetSizerCtrl0(x);
}

void V5B_IppDisableFrame(void)
{
	UINT8 x = 0;

	x = V5B_IppGetSizerCtrl0();
	x &= ~IPP_FRAME_EN;
	V5B_IppSetSizerCtrl0(x);
}

/*********************************************************************
*                            IMAGE SIZE                              *
*********************************************************************/

void V5B_IppSetImageWidth(UINT16 x)
{
	V5_SetReg(V5_REG_IPP_IMGWD, x, BIT_16_LTOH);
}

UINT16 V5B_IppGetImageWidth(void)
{
	UINT16 x = 0;

	x = (UINT16)V5_GetReg(V5_REG_IPP_IMGWD, BIT_16_LTOH);
	return x;
}

void V5B_IppSetImageHeight(UINT16 x)
{
	V5_SetReg(V5_REG_IPP_IMGHT, x, BIT_16_LTOH);
}

UINT16 V5B_IppGetImageHeight(void)
{
	UINT16 x = 0;

	x = (UINT16)V5_GetReg(V5_REG_IPP_IMGHT, BIT_16_LTOH);
	return x;
}

void V5B_IppSetImageSize(TSize size)
{
	V5B_IppSetImageWidth(size.cx);
	V5B_IppSetImageHeight(size.cy);
}

void V5B_IppGetImageSize(PTSize psize)
{
	if(psize == NULL)
		return;
	psize->cx = V5B_IppGetImageWidth();
	psize->cy = V5B_IppGetImageHeight();
}

/*********************************************************************
*                         WINDOW POS & SIZE                          *
*********************************************************************/

void V5B_IppSetWinWidth(UINT16 x)
{
	V5_SetReg(V5_REG_IPP_WINWD, x, BIT_16_LTOH);
}

UINT16 V5B_IppGetWinWidth(void)
{
	UINT16 x = 0;

	x = (UINT16)V5_GetReg(V5_REG_IPP_WINWD, BIT_16_LTOH);
	return x;
}

void V5B_IppSetWinHeight(UINT16 x)
{
	V5_SetReg(V5_REG_IPP_WINHT, x, BIT_16_LTOH);
}

UINT16 V5B_IppGetWinHeight(void)
{
	UINT16 x = 0;

	x = (UINT16)V5_GetReg(V5_REG_IPP_WINHT, BIT_16_LTOH);
	return x;
}

void V5B_IppSetWinStartX(UINT16 x)
{
	V5_SetReg(V5_REG_IPP_WINSTX, x, BIT_16_LTOH);
}

UINT16 V5B_IppGetWinStartX(void)
{
	UINT16 x = 0;

	x = (UINT16)V5_GetReg(V5_REG_IPP_WINSTX, BIT_16_LTOH);
	return x;
}

void V5B_IppSetWinStartY(UINT16 y)
{
	V5_SetReg(V5_REG_IPP_WINSTY, y, BIT_16_LTOH);
}

UINT16 V5B_IppGetWinStartY(void)
{
	UINT16 x = 0;

	x = (UINT16)V5_GetReg(V5_REG_IPP_WINSTY, BIT_16_LTOH);
	return x;
}

void V5B_IppSetWinPoint(TPoint pt)
{
	V5B_IppSetWinStartX(pt.x);
	V5B_IppSetWinStartY(pt.y);
}

void V5B_IppGetWinPoint(PTPoint ppt)
{
	if(ppt == NULL)
		return;
	ppt->x = V5B_IppGetWinStartX();
	ppt->y = V5B_IppGetWinStartY();
}

void V5B_IppSetWinSize(TSize size)
{
	V5B_IppSetWinWidth(size.cx);
	V5B_IppSetWinHeight(size.cy);
}

void V5B_IppGetWinSize(PTSize psize)
{
	if(psize == NULL)
		return;
	psize->cx = V5B_IppGetWinWidth();
	psize->cy = V5B_IppGetWinHeight();
}

/*********************************************************************
*                           DISPLAY SIZER                            *
*********************************************************************/

void V5B_IppSetDispWidth(UINT16 x)
{
	V5_SetReg(V5_REG_IPP_DISWD, x, BIT_16_LTOH);
}

UINT16 V5B_IppGetDispWidth(void)
{
	UINT16 x = 0;

	x = (UINT16)V5_GetReg(V5_REG_IPP_DISWD, BIT_16_LTOH);
	return x;
}

void V5B_IppSetDispHeight(UINT16 x)
{
	V5_SetReg(V5_REG_IPP_DISHT, x, BIT_16_LTOH);
}

UINT16 V5B_IppGetDispHeight(void)
{
	UINT16 x = 0;

	x = (UINT16)V5_GetReg(V5_REG_IPP_DISHT, BIT_16_LTOH);
	return x;
}

void V5B_IppSetDispSize(TSize size)
{
	V5B_IppSetDispWidth(size.cx);
	V5B_IppSetDispHeight(size.cy);
}

void V5B_IppGetDispSize(PTSize psize)
{
	if(psize == NULL)
		return;
	psize->cx = V5B_IppGetDispWidth();
	psize->cy = V5B_IppGetDispHeight();
}

void V5B_IppSetDispHW1(UINT8 x)
{
	x %= 0x80;
	V5_SetReg(V5_REG_IPP_DISH_W1, x, BIT_8);
}

UINT8 V5B_IppGetDispHW1(void)
{
	UINT8 x = 0;

	x = (UINT8)V5_GetReg(V5_REG_IPP_DISH_W1, BIT_8);
	if(x == 0)
		return 0x80;
	return x;
}

void V5B_IppSetDispHW2(UINT8 x)
{
	V5_SetReg(V5_REG_IPP_DISH_W2, x, BIT_8);
}

UINT8 V5B_IppGetDispHW2(void)
{
	UINT8 x = 0;

	x = (UINT8)V5_GetReg(V5_REG_IPP_DISH_W2, BIT_8);
	return x;
}

void V5B_IppSetDispHS1(UINT16 x)
{
	V5_SetReg(V5_REG_IPP_DISH_S1, x, BIT_16_LTOH);
}

UINT16 V5B_IppGetDispHS1(void)
{
	UINT16 x = 0;

	x = (UINT16)V5_GetReg(V5_REG_IPP_DISH_S1, BIT_16_LTOH);
	return x;
}

void V5B_IppSetDispHS2(UINT16 x)
{
	V5_SetReg(V5_REG_IPP_DISH_S2, x, BIT_16_LTOH);
}

UINT16 V5B_IppGetDispHS2(void)
{
	UINT16 x = 0;

	x = (UINT16)V5_GetReg(V5_REG_IPP_DISH_S2, BIT_16_LTOH);
	return x;
}

void V5B_IppSetDispHC(UINT8 x)
{
	V5_SetReg(V5_REG_IPP_DISH_C, x, BIT_8);
}

UINT8 V5B_IppGetDispHC(void)
{
	UINT8 x = 0;

	x = (UINT8)V5_GetReg(V5_REG_IPP_DISH_C, BIT_8);
	return x;
}

void V5B_IppSetDispHFactor(TV5BIppSzDnFct factor)
{
	V5B_IppSetDispHW1(factor.w1);
	V5B_IppSetDispHW2(factor.w2);
	V5B_IppSetDispHS1(factor.s1);
	V5B_IppSetDispHS2(factor.s2);
	V5B_IppSetDispHC(factor.c);
}

void V5B_IppGetDispHFactor(PTV5BIppSzDnFct pfactor)
{
	if(pfactor == NULL)
		return;
	pfactor->w1 = V5B_IppGetDispHW1();
	pfactor->w2 = V5B_IppGetDispHW2();
	pfactor->s1 = V5B_IppGetDispHS1();
	pfactor->s2 = V5B_IppGetDispHS2();
	pfactor->c = V5B_IppGetDispHC();
}

void V5B_IppSetDispVH1(UINT8 x)
{
	x %= 0x80;
	V5_SetReg(V5_REG_IPP_DISV_H1, x, BIT_8);
}

UINT8 V5B_IppGetDispVH1(void)
{
	UINT8 x = 0;

	x = (UINT8)V5_GetReg(V5_REG_IPP_DISV_H1, BIT_8);
	if(x == 0)
		return 0x80;
	return x;
}

void V5B_IppSetDispVH2(UINT8 x)
{
	V5_SetReg(V5_REG_IPP_DISV_H2, x, BIT_8);
}

UINT8 V5B_IppGetDispVH2(void)
{
	UINT8 x = 0;

	x = (UINT8)V5_GetReg(V5_REG_IPP_DISV_H2, BIT_8);
	return x;
}

void V5B_IppSetDispVS1(UINT16 x)
{
	V5_SetReg(V5_REG_IPP_DISV_S1, x, BIT_16_LTOH);
}

UINT16 V5B_IppGetDispVS1(void)
{
	UINT16 x = 0;

	x = (UINT16)V5_GetReg(V5_REG_IPP_DISV_S1, BIT_16_LTOH);
	return x;
}

void V5B_IppSetDispVS2(UINT16 x)
{
	V5_SetReg(V5_REG_IPP_DISV_S2, x, BIT_16_LTOH);
}

UINT16 V5B_IppGetDispVS2(void)
{
	UINT16 x = 0;

	x = (UINT16)V5_GetReg(V5_REG_IPP_DISV_S2, BIT_16_LTOH);
	return x;
}

void V5B_IppSetDispVC(UINT8 x)
{
	V5_SetReg(V5_REG_IPP_DISV_C, x, BIT_8);
}

UINT8 V5B_IppGetDispVC(void)
{
	UINT8 x = 0;

	x = (UINT8)V5_GetReg(V5_REG_IPP_DISV_C, BIT_8);
	return x;
}

void V5B_IppSetDispVFactor(TV5BIppSzDnFct factor)
{
	V5B_IppSetDispVH1(factor.w1);
	V5B_IppSetDispVH2(factor.w2);
	V5B_IppSetDispVS1(factor.s1);
	V5B_IppSetDispVS2(factor.s2);
	V5B_IppSetDispVC(factor.c);
}

void V5B_IppGetDispVFactor(PTV5BIppSzDnFct pfactor)
{
	if(pfactor == NULL)
		return;
	pfactor->w1 = V5B_IppGetDispVH1();
	pfactor->w2 = V5B_IppGetDispVH2();
	pfactor->s1 = V5B_IppGetDispVS1();
	pfactor->s2 = V5B_IppGetDispVS2();
	pfactor->c = V5B_IppGetDispVC();
}

/*********************************************************************
*                           CAPTURE SIZER                            *
*********************************************************************/

void V5B_IppSetCapWidth(UINT16 x)
{
	V5_SetReg(V5_REG_IPP_CAPWD, x, BIT_16_LTOH);
}

UINT16 V5B_IppGetCapWidth(void)
{
	UINT16 x = 0;

	x = (UINT16)V5_GetReg(V5_REG_IPP_CAPWD, BIT_16_LTOH);
	return x;
}

void V5B_IppSetCapHeight(UINT16 x)
{
	V5_SetReg(V5_REG_IPP_CAPHT, x, BIT_16_LTOH);
}

UINT16 V5B_IppGetCapHeight(void)
{
	UINT16 x = 0;

	x = (UINT16)V5_GetReg(V5_REG_IPP_CAPHT, BIT_16_LTOH);
	return x;
}

void V5B_IppSetCapSize(TSize size)
{
	V5B_IppSetCapWidth(size.cx);
	V5B_IppSetCapHeight(size.cy);
}

void V5B_IppGetCapSize(PTSize psize)
{
	if(psize == NULL)
		return;
	psize->cx = V5B_IppGetCapWidth();
	psize->cy = V5B_IppGetCapHeight();
}

void V5B_IppSetCapDnHW1(UINT8 x)
{
	V5_SetReg(V5_REG_IPP_CAPDH_W1, x, BIT_8);
}

UINT8 V5B_IppGetCapDnHW1(void)
{
	UINT8 x = 0;

	x = (UINT8)V5_GetReg(V5_REG_IPP_CAPDH_W1, BIT_8);
	return x;
}

void V5B_IppSetCapDnHW2(UINT8 x)
{
	V5_SetReg(V5_REG_IPP_CAPDH_W2, x, BIT_8);
}

UINT8 V5B_IppGetCapDnHW2(void)
{
	UINT8 x = 0;

	x = (UINT8)V5_GetReg(V5_REG_IPP_CAPDH_W2, BIT_8);
	return x;
}

void V5B_IppSetCapDnHS1(UINT16 x)
{
	V5_SetReg(V5_REG_IPP_CAPDH_S1, x, BIT_16_LTOH);
}

UINT16 V5B_IppGetCapDnHS1(void)
{
	UINT16 x = 0;

	x = (UINT16)V5_GetReg(V5_REG_IPP_CAPDH_S1, BIT_16_LTOH);
	return x;
}

void V5B_IppSetCapDnHS2(UINT16 x)
{
	V5_SetReg(V5_REG_IPP_CAPDH_S2, x, BIT_16_LTOH);
}

UINT16 V5B_IppGetCapDnHS2(void)
{
	UINT16 x = 0;

	x = (UINT16)V5_GetReg(V5_REG_IPP_CAPDH_S2, BIT_16_LTOH);
	return x;
}

void V5B_IppSetCapDnHC(UINT8 x)
{
	V5_SetReg(V5_REG_IPP_CAPDH_C, x, BIT_8);
}

UINT8 V5B_IppGetCapDnHC(void)
{
	UINT8 x = 0;

	x = (UINT8)V5_GetReg(V5_REG_IPP_CAPDH_C, BIT_8);
	return x;
}

void V5B_IppSetCapDnHFactor(TV5BIppSzDnFct factor)
{
	V5B_IppSetCapDnHW1(factor.w1);
	V5B_IppSetCapDnHW2(factor.w2);
	V5B_IppSetCapDnHS1(factor.s1);
	V5B_IppSetCapDnHS2(factor.s2);
	V5B_IppSetCapDnHC(factor.c);
}

void V5B_IppGetCapDnHFactor(PTV5BIppSzDnFct pfactor)
{
	if(pfactor == NULL)
		return;
	pfactor->w1 = V5B_IppGetCapDnHW1();
	pfactor->w2 = V5B_IppGetCapDnHW2();
	pfactor->s1 = V5B_IppGetCapDnHS1();
	pfactor->s2 = V5B_IppGetCapDnHS2();
	pfactor->c = V5B_IppGetCapDnHC();
}

void V5B_IppSetCapDnVH1(UINT8 x)
{
	V5_SetReg(V5_REG_IPP_CAPDV_H1, x, BIT_8);
}

UINT8 V5B_IppGetCapDnVH1(void)
{
	UINT8 x = 0;

	x = (UINT8)V5_GetReg(V5_REG_IPP_CAPDV_H1, BIT_8);
	return x;
}

void V5B_IppSetCapDnVH2(UINT8 x)
{
	V5_SetReg(V5_REG_IPP_CAPDV_H2, x, BIT_8);
}

UINT8 V5B_IppGetCapDnVH2(void)
{
	UINT8 x = 0;

	x = (UINT8)V5_GetReg(V5_REG_IPP_CAPDV_H2, BIT_8);
	return x;
}

void V5B_IppSetCapDnVS1(UINT16 x)
{
	V5_SetReg(V5_REG_IPP_CAPDV_S1, x, BIT_16_LTOH);
}

UINT16 V5B_IppGetCapDnVS1(void)
{
	UINT16 x = 0;

	x = (UINT16)V5_GetReg(V5_REG_IPP_CAPDV_S1, BIT_16_LTOH);
	return x;
}

void V5B_IppSetCapDnVS2(UINT16 x)
{
	V5_SetReg(V5_REG_IPP_CAPDV_S2, x, BIT_16_LTOH);
}

UINT16 V5B_IppGetCapDnVS2(void)
{
	UINT16 x = 0;

	x = (UINT16)V5_GetReg(V5_REG_IPP_CAPDV_S2, BIT_16_LTOH);
	return x;
}

void V5B_IppSetCapDnVC(UINT8 x)
{
	V5_SetReg(V5_REG_IPP_CAPDV_C, x, BIT_8);
}

UINT8 V5B_IppGetCapDnVC(void)
{
	UINT8 x = 0;

	x = (UINT8)V5_GetReg(V5_REG_IPP_CAPDV_C, BIT_8);
	return x;
}

void V5B_IppSetCapDnVFactor(TV5BIppSzDnFct factor)
{
	V5B_IppSetCapDnVH1(factor.w1);
	V5B_IppSetCapDnVH2(factor.w2);
	V5B_IppSetCapDnVS1(factor.s1);
	V5B_IppSetCapDnVS2(factor.s2);
	V5B_IppSetCapDnVC(factor.c);
}

void V5B_IppGetCapDnVFactor(PTV5BIppSzDnFct pfactor)
{
	if(pfactor == NULL)
		return;
	pfactor->w1 = V5B_IppGetCapDnVH1();
	pfactor->w2 = V5B_IppGetCapDnVH2();
	pfactor->s1 = V5B_IppGetCapDnVS1();
	pfactor->s2 = V5B_IppGetCapDnVS2();
	pfactor->c = V5B_IppGetCapDnVC();
}

void V5B_IppSetCapUpHW1(UINT8 x)
{
	V5_SetReg(V5_REG_IPP_CAPUH_W1, x, BIT_8);
}

UINT8 V5B_IppGetCapUpHW1(void)
{
	UINT8 x = 0;

	x = (UINT8)V5_GetReg(V5_REG_IPP_CAPUH_W1, BIT_8);
	return x;
}

void V5B_IppSetCapUpHW2(UINT8 x)
{
	V5_SetReg(V5_REG_IPP_CAPUH_W2, x, BIT_8);
}

UINT8 V5B_IppGetCapUpHW2(void)
{
	UINT8 x = 0;

	x = (UINT8)V5_GetReg(V5_REG_IPP_CAPUH_W2, BIT_8);
	return x;
}

void V5B_IppSetCapUpHS(UINT16 x)
{
	V5_SetReg(V5_REG_IPP_CAPUH_S, x, BIT_16_LTOH);
}

UINT16 V5B_IppGetCapUpHS(void)
{
	UINT16 x = 0;

	x = (UINT16)V5_GetReg(V5_REG_IPP_CAPUH_S, BIT_16_LTOH);
	return x;
}

void V5B_IppSetCapUpHFactor(TV5BIppSzUpFct factor)
{
	V5B_IppSetCapUpHW1(factor.w1);
	V5B_IppSetCapUpHW2(factor.w2);
	V5B_IppSetCapUpHS(factor.s);
}

void V5B_IppGetCapUpHFactor(PTV5BIppSzUpFct pfactor)
{
	if(pfactor == NULL)
		return;
	pfactor->w1 = V5B_IppGetCapUpHW1();
	pfactor->w2 = V5B_IppGetCapUpHW2();
	pfactor->s = V5B_IppGetCapUpHS();
}

void V5B_IppSetCapUpVH1(UINT8 x)
{
	V5_SetReg(V5_REG_IPP_CAPUV_H1, x, BIT_8);
}

UINT8 V5B_IppGetCapUpVH1(void)
{
	UINT8 x = 0;

	x = (UINT8)V5_GetReg(V5_REG_IPP_CAPUV_H1, BIT_8);
	return x;
}

void V5B_IppSetCapUpVH2(UINT8 x)
{
	V5_SetReg(V5_REG_IPP_CAPUV_H2, x, BIT_8);
}

UINT8 V5B_IppGetCapUpVH2(void)
{
	UINT8 x = 0;

	x = (UINT8)V5_GetReg(V5_REG_IPP_CAPUV_H2, BIT_8);
	return x;
}

void V5B_IppSetCapUpVS(UINT16 x)
{
	V5_SetReg(V5_REG_IPP_CAPUV_S, x, BIT_16_LTOH);
}

UINT16 V5B_IppGetCapUPVS(void)
{
	UINT16 x = 0;

	x = (UINT16)V5_GetReg(V5_REG_IPP_CAPUV_S, BIT_16_LTOH);
	return x;
}

void V5B_IppSetCapUpVFactor(TV5BIppSzUpFct factor)
{
	V5B_IppSetCapUpVH1(factor.w1);
	V5B_IppSetCapUpVH2(factor.w2);
	V5B_IppSetCapUpVS(factor.s);
}

void V5B_IppGetCapUpVFactor(PTV5BIppSzUpFct pfactor)
{
	if(pfactor == NULL)
		return;
	pfactor->w1 = V5B_IppGetCapUpVH1();
	pfactor->w2 = V5B_IppGetCapUpVH2();
	pfactor->s = V5B_IppGetCapUPVS();
}

/*********************************************************************
*                          THUMBNAIL SIZER                           *
*********************************************************************/

void V5B_IppSetThumbWidth(UINT16 x)
{
	x %= 0x100;
	V5_SetReg(V5_REG_IPP_THUMB_WD, x, BIT_8);
}

UINT16 V5B_IppGetThumbWidth(void)
{
	UINT8 x = 0;

	x = (UINT8)V5_GetReg(V5_REG_IPP_THUMB_WD, BIT_8);
	if(x == 0)
		return 0x100;
	return x;
}

void V5B_IppSetThumbHeight(UINT16 x)
{
	x %= 0x100;
	V5_SetReg(V5_REG_IPP_THUMB_HT, x, BIT_8);
}

UINT16 V5B_IppGetThumbHeight(void)
{
	UINT8 x = 0;

	x = (UINT8)V5_GetReg(V5_REG_IPP_THUMB_HT, BIT_8);
	if(x == 0)
		return 0x100;
	return x;
}

void V5B_IppSetThumbSize(TSize size)
{
	V5B_IppSetThumbWidth(size.cx);
	V5B_IppSetThumbHeight(size.cy);
}

void V5B_IppGetThumbSize(PTSize psize)
{
	if(psize == NULL)
		return;
	psize->cx = V5B_IppGetThumbWidth();
	psize->cy = V5B_IppGetThumbHeight();
}

void V5B_IppSetThumbHW1(UINT8 x)
{
	V5_SetReg(V5_REG_IPP_THUMBH_W1, x, BIT_8);
}

UINT8 V5B_IppGetThumbHW1(void)
{
	UINT8 x = 0;

	x = (UINT8)V5_GetReg(V5_REG_IPP_THUMBH_W1, BIT_8);
	return x;
}

void V5B_IppSetThumbHW2(UINT8 x)
{
	V5_SetReg(V5_REG_IPP_THUMBH_W2, x, BIT_8);
}

UINT8 V5B_IppGetThumbHW2(void)
{
	UINT8 x = 0;

	x = (UINT8)V5_GetReg(V5_REG_IPP_THUMBH_W2, BIT_8);
	return x;
}

void V5B_IppSetThumbHS1(UINT16 x)
{
	V5_SetReg(V5_REG_IPP_THUMBH_S1, x, BIT_16_LTOH);
}

UINT16 V5B_IppGetThumbHS1(void)
{
	UINT16 x = 0;

	x = (UINT16)V5_GetReg(V5_REG_IPP_THUMBH_S1, BIT_16_LTOH);
	return x;
}

void V5B_IppSetThumbHS2(UINT16 x)
{
	V5_SetReg(V5_REG_IPP_THUMBH_S2, x, BIT_16_LTOH);
}

UINT16 V5B_IppGetThumbHS2(void)
{
	UINT16 x = 0;

	x = (UINT16)V5_GetReg(V5_REG_IPP_THUMBH_S2, BIT_16_LTOH);
	return x;
}

void V5B_IppSetThumbHC(UINT8 x)
{
	V5_SetReg(V5_REG_IPP_THUMBH_C, x, BIT_8);
}

UINT8 V5B_IppGetThumbHC(void)
{
	UINT8 x = 0;

	x = (UINT8)V5_GetReg(V5_REG_IPP_THUMBH_C, BIT_8);
	return x;
}

void V5B_IppSetThumbHFactor(TV5BIppSzDnFct factor)
{
	V5B_IppSetThumbHW1(factor.w1);
	V5B_IppSetThumbHW2(factor.w2);
	V5B_IppSetThumbHS1(factor.s1);
	V5B_IppSetThumbHS2(factor.s2);
	V5B_IppSetThumbHC(factor.c);
}

void V5B_IppGetThumbHFactor(PTV5BIppSzDnFct pfactor)
{
	if(pfactor == NULL)
		return;
	pfactor->w1 = V5B_IppGetThumbHW1();
	pfactor->w2 = V5B_IppGetThumbHW2();
	pfactor->s1 = V5B_IppGetThumbHS1();
	pfactor->s2 = V5B_IppGetThumbHS2();
	pfactor->c = V5B_IppGetThumbHC();
}

void V5B_IppSetThumbVH1(UINT8 x)
{
	V5_SetReg(V5_REG_IPP_THUMBV_H1, x, BIT_8);
}

UINT8 V5B_IppGetThumbVH1(void)
{
	UINT8 x = 0;

	x = (UINT8)V5_GetReg(V5_REG_IPP_THUMBV_H1, BIT_8);
	return x;
}

void V5B_IppSetThumbVH2(UINT8 x)
{
	V5_SetReg(V5_REG_IPP_THUMBV_H2, x, BIT_8);
}

UINT8 V5B_IppGetThumbVH2(void)
{
	UINT8 x = 0;

	x = (UINT8)V5_GetReg(V5_REG_IPP_THUMBV_H2, BIT_8);
	return x;
}

void V5B_IppSetThumbVS1(UINT16 x)
{
	V5_SetReg(V5_REG_IPP_THUMBV_S1, x, BIT_16_LTOH);
}

UINT16 V5B_IppGetThumbVS1(void)
{
	UINT16 x = 0;

	x = (UINT16)V5_GetReg(V5_REG_IPP_THUMBV_S1, BIT_16_LTOH);
	return x;
}

void V5B_IppSetThumbVS2(UINT16 x)
{
	V5_SetReg(V5_REG_IPP_THUMBV_S2, x, BIT_16_LTOH);
}

UINT16 V5B_IppGetThumbVS2(void)
{
	UINT16 x = 0;

	x = (UINT16)V5_GetReg(V5_REG_IPP_THUMBV_S2, BIT_16_LTOH);
	return x;
}

void V5B_IppSetThumbVC(UINT8 x)
{
	V5_SetReg(V5_REG_IPP_THUMBV_C, x, BIT_8);
}

UINT8 V5B_IppGetThumbVC(void)
{
	UINT8 x = 0;

	x = (UINT8)V5_GetReg(V5_REG_IPP_THUMBV_C, BIT_8);
	return x;
}

void V5B_IppSetThumbVFactor(TV5BIppSzDnFct factor)
{
	V5B_IppSetThumbVH1(factor.w1);
	V5B_IppSetThumbVH2(factor.w2);
	V5B_IppSetThumbVS1(factor.s1);
	V5B_IppSetThumbVS2(factor.s2);
	V5B_IppSetThumbVC(factor.c);
}

void V5B_IppGetThumbVFactor(PTV5BIppSzDnFct pfactor)
{
	if(pfactor == NULL)
		return;
	pfactor->w1 = V5B_IppGetThumbVH1();
	pfactor->w2 = V5B_IppGetThumbVH2();
	pfactor->s1 = V5B_IppGetThumbVS1();
	pfactor->s2 = V5B_IppGetThumbVS2();
	pfactor->c = V5B_IppGetThumbVC();
}

void V5B_IppSetThumbInput(UINT8 x)
{
	x &= 0x03;
	V5_SetReg(V5_REG_IPP_THUMB_INPUT, x, BIT_8);
}

UINT8 V5B_IppGetThumbInput(void)
{
	UINT8 x = 0;

	x = (UINT8)V5_GetReg(V5_REG_IPP_THUMB_INPUT, BIT_8);
	return (x & 0x03);
}

/*********************************************************************
*                         SIZER CONTROL 1                           *
*********************************************************************/

void V5B_IppSetSizerCtrl1(UINT8 x)
{
	V5_SetReg(V5_REG_IPP_SIZCTRL1, x, BIT_8);
}

UINT8 V5B_IppGetSizerCtrl1(void)
{
	UINT8 x = 0;

	x = (UINT8)V5_GetReg(V5_REG_IPP_SIZCTRL1, BIT_8);
	return x;
}

void V5B_IppEnableCPDHSBP(void)
{
	UINT8 x = 0;

	x = V5B_IppGetSizerCtrl1();
	x |= IPP_CAP_D_HSIZER_EN;
	V5B_IppSetSizerCtrl1(x);
}

void V5B_IppDisableCPDHSBP(void)
{
	UINT8 x = 0;

	x = V5B_IppGetSizerCtrl1();
	x &= ~IPP_CAP_D_HSIZER_EN;
	V5B_IppSetSizerCtrl1(x);
}

void V5B_IppEnableCPDVSBP(void)
{
	UINT8 x = 0;

	x = V5B_IppGetSizerCtrl1();
	x |= IPP_CAP_D_VSIZER_EN;
	V5B_IppSetSizerCtrl1(x);
}

void V5B_IppDisableCPDVSBP(void)
{
	UINT8 x = 0;

	x = V5B_IppGetSizerCtrl1();
	x &= ~IPP_CAP_D_VSIZER_EN;
	V5B_IppSetSizerCtrl1(x);
}

void V5B_IppEnableCPUHSBP(void)
{
	UINT8 x = 0;

	x = V5B_IppGetSizerCtrl1();
	x |= IPP_CAP_U_HSIZER_EN;
	V5B_IppSetSizerCtrl1(x);
}

void V5B_IppDisableCPUHSBP(void)
{
	UINT8 x = 0;

	x = V5B_IppGetSizerCtrl1();
	x &= ~IPP_CAP_U_HSIZER_EN;
	V5B_IppSetSizerCtrl1(x);
}

void V5B_IppEnableCPUVSBP(void)
{
	UINT8 x = 0;

	x = V5B_IppGetSizerCtrl1();
	x |= IPP_CAP_U_VSIZER_EN;
	V5B_IppSetSizerCtrl1(x);
}

void V5B_IppDisableCPUVSBP(void)
{
	UINT8 x = 0;

	x = V5B_IppGetSizerCtrl1();
	x &= ~IPP_CAP_U_VSIZER_EN;
	V5B_IppSetSizerCtrl1(x);
}

void V5B_IppEnableDPHSBP(void)
{
	UINT8 x = 0;

	x = V5B_IppGetSizerCtrl1();
	x |= IPP_DISP_HSIZER_EN;
	V5B_IppSetSizerCtrl1(x);
}

void V5B_IppDisableDPHSBP(void)
{
	UINT8 x = 0;

	x = V5B_IppGetSizerCtrl1();
	x &= ~IPP_DISP_HSIZER_EN;
	V5B_IppSetSizerCtrl1(x);
}

void V5B_IppEnableDPVSBP(void)
{
	UINT8 x = 0;

	x = V5B_IppGetSizerCtrl1();
	x |= IPP_DISP_VSIZER_EN;
	V5B_IppSetSizerCtrl1(x);
}

void V5B_IppDisableDPVSBP(void)
{
	UINT8 x = 0;

	x = V5B_IppGetSizerCtrl1();
	x &= ~IPP_DISP_VSIZER_EN;
	V5B_IppSetSizerCtrl1(x);
}

void V5B_IppEnableTMHSBP(void)
{
	UINT8 x = 0;

	x = V5B_IppGetSizerCtrl1();
	x |= IPP_TMB_HSIZER_EN;
	V5B_IppSetSizerCtrl1(x);
}

void V5B_IppDisableTMHSBP(void)
{
	UINT8 x = 0;

	x = V5B_IppGetSizerCtrl1();
	x &= ~IPP_TMB_HSIZER_EN;
	V5B_IppSetSizerCtrl1(x);
}

void V5B_IppEnableTMVSBP(void)
{
	UINT8 x = 0;

	x = V5B_IppGetSizerCtrl1();
	x |= IPP_TMB_VSIZER_EN;
	V5B_IppSetSizerCtrl1(x);
}

void V5B_IppDisableTMVSBP(void)
{
	UINT8 x = 0;

	x = V5B_IppGetSizerCtrl1();
	x &= ~IPP_TMB_VSIZER_EN;
	V5B_IppSetSizerCtrl1(x);
}

/*********************************************************************
*                       UV OFFSET & THRESHOLD                        *
*********************************************************************/

void V5B_IppSetUOffset(UINT8 x)
{
	V5_SetReg(V5_REG_IPP_UOFFSET, x, BIT_8);
}

UINT8 V5B_IppGetUOffset(void)
{
	UINT8 x = 0;

	x = (UINT8)V5_GetReg(V5_REG_IPP_UOFFSET, BIT_8);
	return x;
}

void V5B_IppSetVOffset(UINT8 x)
{
	V5_SetReg(V5_REG_IPP_VOFFSET, x, BIT_8);
}

UINT8 V5B_IppGetVOffset(void)
{
	UINT8 x = 0;

	x = (UINT8)V5_GetReg(V5_REG_IPP_VOFFSET, BIT_8);
	return x;
}

void V5B_IppSetUTopThrd(UINT8 x)
{
	V5_SetReg(V5_REG_IPP_UUPTHRS, x, BIT_8);
}

UINT8 V5B_IppGetUTopThrd(void)
{
	UINT8 x = 0;

	x = (UINT8)V5_GetReg(V5_REG_IPP_UUPTHRS, BIT_8);
	return x;
}

void V5B_IppSetUBottomThrd(UINT8 x)
{
	V5_SetReg(V5_REG_IPP_UDWTHRS, x, BIT_8);
}

UINT8 V5B_IppGetUBottomThrd(void)
{
	UINT8 x = 0;

	x = (UINT8)V5_GetReg(V5_REG_IPP_UDWTHRS, BIT_8);
	return x;
}

void V5B_IppSetVTopThrd(UINT8 x)
{
	V5_SetReg(V5_REG_IPP_VUPTHRS, x, BIT_8);
}

UINT8 V5B_IppGetVTopThrd(void)
{
	UINT8 x = 0;

	x = (UINT8)V5_GetReg(V5_REG_IPP_VUPTHRS, BIT_8);
	return x;
}

void V5B_IppSetVBottomThrd(UINT8 x)
{
	V5_SetReg(V5_REG_IPP_VDWTHRS, x, BIT_8);
}

UINT8 V5B_IppGetVBottomThrd(void)
{
	UINT8 x = 0;

	x = (UINT8)V5_GetReg(V5_REG_IPP_VDWTHRS, BIT_8);
	return x;
}

/*********************************************************************
*                            FRAME DROP                              *
*********************************************************************/

void V5B_IppSetCapFrmDrop(UINT16 x)
{
	V5_SetReg(V5_REG_IPP_CAPFRMDP, x, BIT_16_LTOH);
}

UINT16 V5B_IppGetCapFrmDrop(void)
{
	UINT16 x = 0;

	x = (UINT16)V5_GetReg(V5_REG_IPP_CAPFRMDP, BIT_16_LTOH);
	return x;
}

void V5B_IppSetDispFrmDrop(UINT16 x)
{
	V5_SetReg(V5_REG_IPP_DISFRMDP, x, BIT_16_LTOH);
}

UINT16 V5B_IppGetDispFrmDrop(void)
{
	UINT16 x = 0;

	x = (UINT16)V5_GetReg(V5_REG_IPP_DISFRMDP, BIT_16_LTOH);
	return x;
}

/*********************************************************************
*                            Histogram                               *
*********************************************************************/

void V5B_IppEnableHist(void)
{
	UINT8 x = 0;

	x = (UINT8)V5_GetReg(V5_REG_IPP_HIST_EN, BIT_8);
	x |= 0x01;
	V5_SetReg(V5_REG_IPP_HIST_EN, x, BIT_8);

	while(x)
		x = (UINT8)V5_GetReg(V5_REG_IPP_HIST_EN, BIT_8);
}

void V5B_IppSetHistIn(UINT8 x)
{
	V5_SetReg(V5_REG_IPP_HIST_IN_SEL, x, BIT_8);
}

UINT8 V5B_IppGetHistIn(void)
{
	UINT8 x = 0;

	x = (UINT8)V5_GetReg(V5_REG_IPP_HIST_IN_SEL, BIT_8);
	return x;
}

UINT8 V5B_IppGetHistXCur(void)
{
	UINT8 x = 0;

	x = (UINT8)V5_GetReg(V5_REG_IPP_HIST_X_CUR, BIT_8);
	return x;
}

UINT8 V5B_IppGetHistXAvg(void)
{
	UINT8 x = 0;

	x = (UINT8)V5_GetReg(V5_REG_IPP_HIST_X_AVG, BIT_8);
	return x;
}

void V5B_IppSetHistQ(UINT32 x)
{
	V5_SetReg(V5_REG_IPP_HIST_Q_L, x, BIT_32_LTOH);
}

UINT32 V5B_IppGetHistQ(void)
{
	UINT32 x = 0;

	x = (UINT32)V5_GetReg(V5_REG_IPP_HIST_Q_L, BIT_32_LTOH);
	return (x & 0x7ffff);
}

void V5B_IppGetHistResult(UINT8 *buf)
{
	UINT8 i = 0;

	for(i = 0; i < 32; i++)
		buf[i] = (UINT8)V5_GetReg((V5_REG_IPP_HIST_RLT_00 + i), BIT_8);
}

void V5B_IppSetThumOutFmt(UINT8 x)
{
	V5_SetReg(V5_REG_IPP_TSIZER_OUT, x, BIT_8);
}

UINT8 V5B_IppGetThumOutFmt(void)
{
	UINT8 x = 0;

	x = (UINT8)V5_GetReg(V5_REG_IPP_TSIZER_OUT, BIT_8);
	return x;
}

#endif //V578CMD_DISABLE
