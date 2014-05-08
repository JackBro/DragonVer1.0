#include "basefunc.h"
#ifndef	V578CMD_DISABLE

/********************************************************************************
Description:
config internal ISP base function
Parameters:
x:		setting value
Note:
<TABLE>
value		meaning
-----		-------
BIT0-3		pixel rate = (val+1)*clk
BIT4		G pixel first
BIT5		1:BG line first		0:RG line first
BIT6		1:only testbar		0:video on center window
BIT7		enable testbar function
BIT8		enable edge enhancement function
BIT9		enable lens fall off function 
BIT10		dpd/dpc and noise removal function 
BIT11		RGB gamma correction
BIT12		color matrix correction
BIT13		Y gamma correction
BIT14		digital RGB gain
BIT15		gray image
</TABLE>
Remarks:
*********************************************************************************/
void V5B_IspSetBasctl(UINT16 x)
{
	V5_SetReg(V5_REG_ISP_CTL_BAS, x, BIT_16_LTOH);
}

UINT16 V5B_IspGetBasctl(void)
{
	UINT16 x = 0;

	x = (UINT16)V5_GetReg(V5_REG_ISP_CTL_BAS, BIT_16_LTOH);
	return x;
}

void V5B_IspSetPixelRate(UINT8 rate)
{
	UINT8 x = 0;

	x = (UINT8)V5_GetReg(V5_REG_ISP_CTL_BAS, BIT_8);
	x &= 0xf0;
	x |= (rate & 0x0f);
	V5_SetReg(V5_REG_ISP_CTL_BAS, x, BIT_8);
}

UINT8 V5B_IspGetPixelRate(void)
{
	UINT8 x = 0;

	x = (UINT8)V5_GetReg(V5_REG_ISP_CTL_BAS, BIT_8);
	return (x & 0x0f);
}

void V5B_IspEnableGFirst(void)
{
	UINT8 x = 0;

	x = (UINT8)V5_GetReg(V5_REG_ISP_CTL_BAS, BIT_8);
	V5_SetReg(V5_REG_ISP_CTL_BAS, (x | 0x10), BIT_8);
}

void V5B_IspDisableGFirst(void)
{
	UINT8 x = 0;

	x = (UINT8)V5_GetReg(V5_REG_ISP_CTL_BAS, BIT_8);
	V5_SetReg(V5_REG_ISP_CTL_BAS, (x & 0xef), BIT_8);
}

void V5B_IspEnableBgFirst(void)
{
	UINT8 x = 0;

	x = (UINT8)V5_GetReg(V5_REG_ISP_CTL_BAS, BIT_8);
	V5_SetReg(V5_REG_ISP_CTL_BAS, (x | 0x20), BIT_8);
}

void V5B_IspDisableBgFirst(void)
{
	UINT8 x = 0;

	x = (UINT8)V5_GetReg(V5_REG_ISP_CTL_BAS, BIT_8);
	V5_SetReg(V5_REG_ISP_CTL_BAS, (x & 0xdf), BIT_8);
}

void V5B_IspEnableYuvFmt(void)
{
	UINT8 x = 0;

	x = (UINT8)V5_GetReg(V5_REG_ISP_CTL_BAS, BIT_8);
	V5_SetReg(V5_REG_ISP_CTL_BAS, (x | 0x40), BIT_8);
}

void V5B_IspDisableYuvFmt(void)
{
	UINT8 x = 0;

	x = (UINT8)V5_GetReg(V5_REG_ISP_CTL_BAS, BIT_8);
	V5_SetReg(V5_REG_ISP_CTL_BAS, (x & 0xbf), BIT_8);
}

void V5B_IspEnableHPF(void)
{
	UINT8 x = 0;

	x = (UINT8)V5_GetReg(V5_REG_ISP_CTL_BAS, BIT_8);
	V5_SetReg(V5_REG_ISP_CTL_BAS, (x | 0x80), BIT_8);
}

void V5B_IspEnableLPF(void)
{
	UINT8 x = 0;

	x = (UINT8)V5_GetReg(V5_REG_ISP_CTL_BAS, BIT_8);
	V5_SetReg(V5_REG_ISP_CTL_BAS, (x & 0x7f), BIT_8);
}

void V5B_IspEnableEdgeEnh(void)
{
	UINT8 x = 0;

	x = (UINT8)V5_GetReg(V5_REG_ISP_CTL_BAS_H, BIT_8);
	V5_SetReg(V5_REG_ISP_CTL_BAS_H, (x | 0x01), BIT_8);
}

void V5B_IspDisableEdgeEnh(void)
{
	UINT8 x = 0;

	x = (UINT8)V5_GetReg(V5_REG_ISP_CTL_BAS_H, BIT_8);
	V5_SetReg(V5_REG_ISP_CTL_BAS_H, (x & 0xfe), BIT_8);
}

void V5B_IspEnableLensOff(void)
{
	UINT8 x = 0;

	x = (UINT8)V5_GetReg(V5_REG_ISP_CTL_BAS_H, BIT_8);
	V5_SetReg(V5_REG_ISP_CTL_BAS_H, (x | 0x02), BIT_8);
}

void V5B_IspDisableLensOff(void)
{
	UINT8 x = 0;

	x = (UINT8)V5_GetReg(V5_REG_ISP_CTL_BAS_H, BIT_8);
	V5_SetReg(V5_REG_ISP_CTL_BAS_H, (x & 0xfd), BIT_8);
}

void V5B_IspEnableDpc(void)
{
	UINT8 x = 0;

	x = (UINT8)V5_GetReg(V5_REG_ISP_CTL_BAS_H, BIT_8);
	V5_SetReg(V5_REG_ISP_CTL_BAS_H, (x | 0x04), BIT_8);
}

void V5B_IspDisableDpc(void)
{
	UINT8 x = 0;

	x = (UINT8)V5_GetReg(V5_REG_ISP_CTL_BAS_H, BIT_8);
	V5_SetReg(V5_REG_ISP_CTL_BAS_H, (x & 0xfb), BIT_8);
}

void V5B_IspEnableRgbGamma(void)
{
	UINT8 x = 0;

	x = (UINT8)V5_GetReg(V5_REG_ISP_CTL_BAS_H, BIT_8);
	V5_SetReg(V5_REG_ISP_CTL_BAS_H, (x | 0x08), BIT_8);
}

void V5B_IspDisableRgbGamma(void)
{
	UINT8 x = 0;

	x = (UINT8)V5_GetReg(V5_REG_ISP_CTL_BAS_H, BIT_8);
	V5_SetReg(V5_REG_ISP_CTL_BAS_H, (x & 0xf7), BIT_8);
}

void V5B_IspEnableColorMatrix(void)
{
	UINT8 x = 0;

	x = (UINT8)V5_GetReg(V5_REG_ISP_CTL_BAS_H, BIT_8);
	V5_SetReg(V5_REG_ISP_CTL_BAS_H, (x | 0x10), BIT_8);
}

void V5B_IspDisableColorMatrix(void)
{
	UINT8 x = 0;

	x = (UINT8)V5_GetReg(V5_REG_ISP_CTL_BAS_H, BIT_8);
	V5_SetReg(V5_REG_ISP_CTL_BAS_H, (x & 0xef), BIT_8);
}

void V5B_IspEnableYGamma(void)
{
	UINT8 x = 0;

	x = (UINT8)V5_GetReg(V5_REG_ISP_CTL_BAS_H, BIT_8);
	V5_SetReg(V5_REG_ISP_CTL_BAS_H, (x | 0x20), BIT_8);
}

void V5B_IspDisableYGamma(void)
{
	UINT8 x = 0;

	x = (UINT8)V5_GetReg(V5_REG_ISP_CTL_BAS_H, BIT_8);
	V5_SetReg(V5_REG_ISP_CTL_BAS_H, (x & 0xdf), BIT_8);
}

void V5B_IspEnableRgbGain(void)
{
	UINT8 x = 0;

	x = (UINT8)V5_GetReg(V5_REG_ISP_CTL_BAS_H, BIT_8);
	V5_SetReg(V5_REG_ISP_CTL_BAS_H, (x | 0x40), BIT_8);
}

void V5B_IspDisableRgbGain(void)
{
	UINT8 x = 0;

	x = (UINT8)V5_GetReg(V5_REG_ISP_CTL_BAS_H, BIT_8);
	V5_SetReg(V5_REG_ISP_CTL_BAS_H, (x & 0xbf), BIT_8);
}

void V5B_IspEnableGrayImage(void)
{
	UINT8 x = 0;

	x = (UINT8)V5_GetReg(V5_REG_ISP_CTL_BAS_H, BIT_8);
	V5_SetReg(V5_REG_ISP_CTL_BAS_H, (x | 0x80), BIT_8);
}

void V5B_IspDisableGrayImage(void)
{
	UINT8 x = 0;

	x = (UINT8)V5_GetReg(V5_REG_ISP_CTL_BAS_H, BIT_8);
	V5_SetReg(V5_REG_ISP_CTL_BAS_H, (x & 0x7f), BIT_8);
}

void V5B_IspSetTestBarWin(TSize size)
{
	V5_SetReg(V5_REG_ISP_WIN_W, size.cx, BIT_16_LTOH);
	V5_SetReg(V5_REG_ISP_WIN_H, size.cy, BIT_16_LTOH);
}

void V5B_IspGetTestBarWin(TSize *size)
{
	size->cx = (UINT16)V5_GetReg(V5_REG_ISP_WIN_W, BIT_16_LTOH);
	size->cy = (UINT16)V5_GetReg(V5_REG_ISP_WIN_H, BIT_16_LTOH);
}

void V5B_IspSetImageSize(TSize size)
{
	V5_SetReg(V5_REG_ISP_IMG_W, size.cx, BIT_16_LTOH);
	V5_SetReg(V5_REG_ISP_IMG_H, size.cy, BIT_16_LTOH);
}

void V5B_IspGetImageSize(TSize *size)
{
	size->cx = (UINT16)V5_GetReg(V5_REG_ISP_IMG_W, BIT_16_LTOH);
	size->cy = (UINT16)V5_GetReg(V5_REG_ISP_IMG_H, BIT_16_LTOH);
}

void V5B_IspSetEdgeParm(TV5IspEdgeParm edge)
{
	V5_SetReg(V5_REG_ISP_FEM_M,	  edge.femm,   BIT_8);
	V5_SetReg(V5_REG_ISP_FEM_P,	  edge.femp,   BIT_8);
	V5_SetReg(V5_REG_ISP_FEM_X1,  edge.femx1,  BIT_8);
	V5_SetReg(V5_REG_ISP_FEM_X2,  edge.femx2,  BIT_8);
	V5_SetReg(V5_REG_ISP_FEM_MIN, edge.femmin, BIT_8);
	V5_SetReg(V5_REG_ISP_FEM_MAX, edge.femmax, BIT_8);
	V5B_IspUpdateFem();
}

void V5B_IspGetEdgeParm(PTV5IspEdgeParm pedge)
{
	pedge->femm   = (UINT8)V5_GetReg(V5_REG_ISP_FEM_M,   BIT_8);
	pedge->femp   = (UINT8)V5_GetReg(V5_REG_ISP_FEM_P,   BIT_8);
	pedge->femx1  = (UINT8)V5_GetReg(V5_REG_ISP_FEM_X1,  BIT_8);
	pedge->femx2  = (UINT8)V5_GetReg(V5_REG_ISP_FEM_X2,  BIT_8);
	pedge->femmin = (UINT8)V5_GetReg(V5_REG_ISP_FEM_MIN, BIT_8);
	pedge->femmax = (UINT8)V5_GetReg(V5_REG_ISP_FEM_MAX, BIT_8);
}

void V5B_IspSetLensParm(TV5IspLensFalloff lens)
{
	V5_SetReg(V5_REG_ISP_CENTER_X,   lens.lfcenx, BIT_16_LTOH);
	V5_SetReg(V5_REG_ISP_CENTER_Y,   lens.lfceny, BIT_16_LTOH);
	V5_SetReg(V5_REG_ISP_F2_RECPROC, lens.lffrec, BIT_16_LTOH);
}

void V5B_IspGetLensParm(PTV5IspLensFalloff plens)
{
	plens->lfcenx = (UINT16)V5_GetReg(V5_REG_ISP_CENTER_X,   BIT_16_LTOH);
	plens->lfceny = (UINT16)V5_GetReg(V5_REG_ISP_CENTER_Y,   BIT_16_LTOH);
	plens->lffrec = (UINT16)V5_GetReg(V5_REG_ISP_F2_RECPROC, BIT_16_LTOH);
}

/********************************************************************************
Description:
config auto flicker control
Parameters:
ctl:	setting value
Note:
<TABLE>
value		meaning
-----		-------
BIT0-8		auto fliker line start
BIT9-14		auto fliker line count
BIT15		auto fliker line mode
</TABLE>
Remarks:
*********************************************************************************/
void V5B_IspSetAfLnCtl(UINT16 x)
{
	V5_SetReg(V5_REG_ISP_AFLNCTL, x, BIT_16_LTOH);
}

UINT16 V5B_IspGetAfLnCtl(void)
{
	UINT16 x = 0;

	x = (UINT16)V5_GetReg(V5_REG_ISP_AFLNCTL, BIT_16_LTOH);
	return x;
}

void V5B_IspSetAfLnSt(UINT16 val)
{
	UINT16 x = 0;

	x = V5B_IspGetAfLnCtl();
	x &= 0xfe00;
	x |= (val & 0x100);
	V5B_IspSetAfLnCtl(x);
}

UINT16 V5B_IspGetAfLnSt(void)
{
	UINT16 x = 0;

	x = V5B_IspGetAfLnCtl();
	return (x & 0x01ff);
}

void V5B_IspSetAfLnCnt(UINT8 cnt)
{
	UINT8 x = 0;

	x = (UINT8)V5_GetReg(V5_REG_ISP_AFLNCTL_H, BIT_8);
	x &= 0x81;
	x |= ((cnt & 0x3f) << 1);
	V5_SetReg(V5_REG_ISP_AFLNCTL_H, x, BIT_8);
}

UINT8 V5B_IspGetAfLnCnt(void)
{
	UINT8 x = 0;

	x = (UINT8)V5_GetReg(V5_REG_ISP_AFLNCTL_H, BIT_8);
	return ((x & 0x7e) >> 1);
}

void V5B_IspEnableAfLnMode(void)
{
	UINT8 x = 0;

	x = (UINT8)V5_GetReg(V5_REG_ISP_AFLNCTL_H, BIT_8);
	V5_SetReg(V5_REG_ISP_AFLNCTL_H, (x | 0x80), BIT_8);
}

void V5B_IspDisableAfLnMode(void)
{
	UINT8 x = 0;

	x = (UINT8)V5_GetReg(V5_REG_ISP_AFLNCTL_H, BIT_8);
	V5_SetReg(V5_REG_ISP_AFLNCTL_H, (x & 0x7f), BIT_8);
}

UINT32 V5B_IspGetAfSta(void)
{
	UINT32 x = 0;

	x = V5_GetReg(V5_REG_ISP_AFSTA, BIT_32_LTOH);
	return x;
}

void V5B_IspSetAfLnStep(UINT8 step)
{
	UINT8 x = 0;

	x = (UINT8)V5_GetReg(V5_REG_ISP_AFLNSTEP, BIT_8);
	x &= 0xf0;
	x |= (step & 0x0f);
	V5_SetReg(V5_REG_ISP_AFLNSTEP, x, BIT_8);
}

UINT8 V5B_IspGetAfLnStep(void)
{
	UINT8 x = 0;

	x = (UINT8)V5_GetReg(V5_REG_ISP_AFLNSTEP, BIT_8);
	return (x & 0x0f);
}

void V5B_IspSetTailBlank(UINT32 x)
{
	V5_SetReg(V5_REG_ISP_TAIL_BLANK_L, (UINT8)(x & 0xff), BIT_8);
	V5_SetReg(V5_REG_ISP_TAIL_BLANK_M, (UINT8)((x >> 8) & 0xffff), BIT_16_LTOH);
}

UINT32 V5B_IspGetTailBlank(void)
{
	UINT32 x = 0, tmp = 0;

	tmp = (UINT32)V5_GetReg(V5_REG_ISP_TAIL_BLANK_L, BIT_8);
	x	= (UINT32)V5_GetReg(V5_REG_ISP_TAIL_BLANK_M, BIT_16_LTOH);
	x	= ((x << 8) & 0xffff00) | (tmp & 0xff);
	return x;
}

void V5B_IspSetRgain(UINT8 x)
{
	V5_SetReg(V5_REG_ISP_RGAIN, x, BIT_8);
	V5B_IspUpdateDigitalGain();
}

UINT8 V5B_IspGetRgain(void)
{
	UINT8 x = 0;

	x = (UINT8)V5_GetReg(V5_REG_ISP_RGAIN, BIT_8);
	return x;
}

void V5B_IspSetGgain(UINT8 x)
{
	V5_SetReg(V5_REG_ISP_GGAIN, x, BIT_8);
	V5B_IspUpdateDigitalGain();
}

UINT8 V5B_IspGetGgain(void)
{
	UINT8 x = 0;

	x = (UINT8)V5_GetReg(V5_REG_ISP_GGAIN, BIT_8);
	return x;
}

void V5B_IspSetBgain(UINT8 x)
{
	V5_SetReg(V5_REG_ISP_BGAIN, x, BIT_8);
	V5B_IspUpdateDigitalGain();
}

UINT8 V5B_IspGetBgain(void)
{
	UINT8 x = 0;

	x = (UINT8)V5_GetReg(V5_REG_ISP_BGAIN, BIT_8);
	return x;
}

void V5B_IspEnableVsyncReset(void)
{
	UINT8 x = 0;

	x = (UINT8)V5_GetReg(V5_REG_ISP_VS_RST_DIS, BIT_8);
	V5_SetReg(V5_REG_ISP_VS_RST_DIS, (x & 0xfe), BIT_8);
}

void V5B_IspDisableVsyncReset(void)
{
	UINT8 x = 0;

	x = (UINT8)V5_GetReg(V5_REG_ISP_VS_RST_DIS, BIT_8);
	V5_SetReg(V5_REG_ISP_VS_RST_DIS, (x | 0x01), BIT_8);
}

UINT8 V5B_IspIsVsyncResetDisable(void)
{
	UINT8 x = 0;

	x = (UINT8)V5_GetReg(V5_REG_ISP_VS_RST_DIS, BIT_8);
	return x;
}

void V5B_IspSetGlbGain(UINT8 x)
{
	V5_SetReg(V5_REG_ISP_GLBGAIN, x, BIT_8);
	V5B_IspUpdateGlbGain();
}

UINT8 V5B_IspGetGlbGain(void)
{
	UINT8 x = 0;

	x = (UINT8)V5_GetReg(V5_REG_ISP_GLBGAIN, BIT_8);
	return x;
}

void V5B_IspSetGlbGainCtrl(UINT8 x)
{
	V5_SetReg(V5_REG_ISP_GLBCTL, x, BIT_8);
	V5B_IspUpdateGlbGain();
}

UINT8 V5B_IspGetGlbGainCtrl(void)
{
	UINT8 x = 0;

	x = (UINT8)V5_GetReg(V5_REG_ISP_GLBCTL, BIT_8);
	return x;
}

void V5B_IspEnableGlbGain(void)
{
	UINT8 x = 0;

	x = V5B_IspGetGlbGainCtrl();
	V5B_IspSetGlbGainCtrl((x | 0x20));
}

void V5B_IspDisableGlbGain(void)
{
	UINT8 x = 0;

	x = V5B_IspGetGlbGainCtrl();
	V5B_IspSetGlbGainCtrl((x & 0xdf));
}

void V5B_IspSetGlbGainDelay(UINT8 val)
{
	UINT8 x = 0;

	x = V5B_IspGetGlbGainCtrl();
	x &= 0xe3;
	x |= ((val & 0x07) << 2);
	V5B_IspSetGlbGainCtrl(x);
}

UINT8 V5B_IspGetGlbGainDelay(void)
{
	UINT8 x = 0;

	x = V5B_IspGetGlbGainCtrl();
	return ((x & 0x1c) >> 2);
}

/********************************************************************************
Description:
config digital global gain effect fraction
Parameters:
val:	setting value
Note:
<TABLE>
val			step
---			----
00			0x40
01			0x20
10			0x10
11			0x8
</TABLE>
Remarks:
this is gain unit, the real digita gain = 
V5B_IspGetGlbGain() / (step value);
*********************************************************************************/
void V5B_IspSetGlbGainStep(UINT8 val)
{
	UINT8 x = 0;

	x = V5B_IspGetGlbGainCtrl();
	x &= 0xfc;
	x |= (val & 0x03);
	V5B_IspSetGlbGainCtrl(x);
}

UINT8 V5B_IspGetGlbGainStep(void)
{
	UINT8 x = 0;

	x = V5B_IspGetGlbGainCtrl();
	return (x & 0x03);
}

void V5B_IspSetColorCvt(TV5IspColorCvt val)
{
	V5_SetReg(V5_REG_ISP_CRADJ, val.cradj, BIT_8);
	V5_SetReg(V5_REG_ISP_CBADJ, val.cbadj, BIT_8);
}

void V5B_IspGetColorCvt(PTV5IspColorCvt pval)
{
	pval->cradj = (UINT8)V5_GetReg(V5_REG_ISP_CRADJ, BIT_8);
	pval->cbadj = (UINT8)V5_GetReg(V5_REG_ISP_CBADJ, BIT_8);
}

void V5B_IspSetGrayImg(TV5IspGrayImg val)
{
	V5_SetReg(V5_REG_ISP_CROFS, val.croft, BIT_8);
	V5_SetReg(V5_REG_ISP_CBOFS, val.cboft, BIT_8);
	V5B_IspUpdateGray();
}

void V5B_IspGetGrayImg(PTV5IspGrayImg pval)
{
	pval->croft = (UINT8)V5_GetReg(V5_REG_ISP_CROFS, BIT_8);
	pval->cboft = (UINT8)V5_GetReg(V5_REG_ISP_CBOFS, BIT_8);
}

/********************************************************************************
Description:
config color correction matrix
Parameters:
x:	color correction matrix
Note:
CMA11 -- 33 is between [-127, 127]
\For positive values, CMAxx =Integer (Real Coefficient Value x 64);
\For negative values, CMAxx = 2's Complement (Integer (Real Coefficient Value x 64));
\Fundamental color matrix equation
\R' = R*CMA11 + G*CMA12 + B*CMA13 + OFSR
\G' = R*CMA21 + G*CMA22 + B*CMA23 + OFSG
\B' = R*CMA31 + G*CMA32 + B*CMA33 + OFSB
Remarks:
*********************************************************************************/
void V5B_IspSetColorMatrix(TV5IspColorMatrix mtx)
{
	V5_SetReg(V5_REG_ISP_COLMAT11, mtx.cmat11, BIT_8);
	V5_SetReg(V5_REG_ISP_COLMAT12, mtx.cmat12, BIT_8);
	V5_SetReg(V5_REG_ISP_COLMAT13, mtx.cmat13, BIT_8);
	V5_SetReg(V5_REG_ISP_COLMAT21, mtx.cmat21, BIT_8);
	V5_SetReg(V5_REG_ISP_COLMAT22, mtx.cmat22, BIT_8);
	V5_SetReg(V5_REG_ISP_COLMAT23, mtx.cmat23, BIT_8);
	V5_SetReg(V5_REG_ISP_COLMAT31, mtx.cmat31, BIT_8);
	V5_SetReg(V5_REG_ISP_COLMAT32, mtx.cmat32, BIT_8);
	V5_SetReg(V5_REG_ISP_COLMAT33, mtx.cmat33, BIT_8);
	V5_SetReg(V5_REG_ISP_COLOFSR,  mtx.ofsr, BIT_16_LTOH);
	V5_SetReg(V5_REG_ISP_COLOFSG,  mtx.ofsg, BIT_16_LTOH);
	V5_SetReg(V5_REG_ISP_COLOFSB,  mtx.ofsb, BIT_16_LTOH);
	V5B_IspUpdateColMat();
}

void V5B_IspGetColorMatrix(PTV5IspColorMatrix pmtx)
{
	pmtx->cmat11 = (UINT8)V5_GetReg(V5_REG_ISP_COLMAT11, BIT_8);
	pmtx->cmat12 = (UINT8)V5_GetReg(V5_REG_ISP_COLMAT12, BIT_8);
	pmtx->cmat13 = (UINT8)V5_GetReg(V5_REG_ISP_COLMAT13, BIT_8);
	pmtx->cmat21 = (UINT8)V5_GetReg(V5_REG_ISP_COLMAT21, BIT_8);
	pmtx->cmat22 = (UINT8)V5_GetReg(V5_REG_ISP_COLMAT22, BIT_8);
	pmtx->cmat23 = (UINT8)V5_GetReg(V5_REG_ISP_COLMAT23, BIT_8);
	pmtx->cmat31 = (UINT8)V5_GetReg(V5_REG_ISP_COLMAT31, BIT_8);
	pmtx->cmat32 = (UINT8)V5_GetReg(V5_REG_ISP_COLMAT32, BIT_8);
	pmtx->cmat33 = (UINT8)V5_GetReg(V5_REG_ISP_COLMAT33, BIT_8);

	pmtx->ofsr = (UINT16)V5_GetReg(V5_REG_ISP_COLOFSR, BIT_16_LTOH);
	pmtx->ofsg = (UINT16)V5_GetReg(V5_REG_ISP_COLOFSG, BIT_16_LTOH);
	pmtx->ofsb = (UINT16)V5_GetReg(V5_REG_ISP_COLOFSB, BIT_16_LTOH);
}

void V5B_IspSetYGammaStart(UINT8 *x)
{
	UINT8 i = 0;

	for(i = 0; i < 9; i++)
		V5_SetReg((V5_REG_ISP_YGMAP0 + i), x[i], BIT_8);
}

void V5B_IspGetYGammaStart(UINT8 *x)
{
	UINT8 i = 0;

	for(i = 0; i < 9; i++)
		*(x + i) = (UINT8)V5_GetReg((V5_REG_ISP_YGMAP0 + i), BIT_8);
}

void V5B_IspSetYGammaSlope(UINT8 *x)
{
	UINT8 i = 0;

	for(i = 0; i < 9; i++)
		V5_SetReg((V5_REG_ISP_YGMAS0 + i), x[i], BIT_8);
}

void V5B_IspGetYGammaSlope(UINT8 *x)
{
	UINT8 i = 0;

	for(i = 0; i < 9; i++)
		*(x + i) = (UINT8)V5_GetReg((V5_REG_ISP_YGMAS0 + i), BIT_8);
}

void V5B_IspSetRGammaStart(UINT8 *x)
{
	UINT8 i = 0;

	for(i = 0; i < 17; i++)
		V5_SetReg((V5_REG_ISP_RGMAS0 + i), x[i], BIT_8);
	V5B_IspUpdateRGamma();
}

void V5B_IspGetRGammaStart(UINT8 *x)
{
	UINT8 i = 0;

	for(i = 0; i < 17; i++)
		*(x + i) = (UINT8)V5_GetReg((V5_REG_ISP_RGMAS0 + i), BIT_8);
}

void V5B_IspSetGGammaStart(UINT8 *x)
{
	UINT8 i = 0;

	for(i = 0; i < 17; i++)
		V5_SetReg((V5_REG_ISP_GGMAS0 + i), x[i], BIT_8);
	V5B_IspUpdateGGamma();
}

void V5B_IspGetGGammaStart(UINT8 *x)
{
	UINT8 i = 0;

	for(i = 0; i < 17; i++)
		*(x + i) = (UINT8)V5_GetReg((V5_REG_ISP_GGMAS0 + i), BIT_8);
}

void V5B_IspSetBGammaStart(UINT8 *x)
{
	UINT8 i = 0;

	for(i = 0; i < 17; i++)
		V5_SetReg((V5_REG_ISP_BGMAS0 + i), x[i], BIT_8);
	V5B_IspUpdateBGamma();
}

void V5B_IspGetBGammaStart(UINT8 *x)
{
	UINT8 i = 0;

	for(i = 0; i < 17; i++)
		*(x + i) = (UINT8)V5_GetReg((V5_REG_ISP_BGMAS0 + i), BIT_8);
}

/********************************************************************************
Description:
config auto function control
Parameters:
x:	auto function control setting
Note:
<TABLE>
value		meaning
-----		-------
BIT0		Auto-flicker function on	
BIT1-3		auto function interrupt frequency: 
if 0 -> 1, else pow(2, val)+1
BIT4		AE window weight mode, 1: Smaller Y range is used
BIT5		AE block weight mode, 1: large center window
BIT6-7		AE weight mode,00: Bweight(Block weight) mode only, 
01: Wweight(Windows weight) mode only, 10: BWeight + Wweight
11: weightless mode
BIT8		AWB function enable
BIT9		AWB reverse mode
BIT10		AWB windows control, 1: 1 center window used only
BIT11-15	reserve
</TABLE>
Remarks:
*********************************************************************************/
void V5B_IspSetAfCtrl(UINT16 x)
{
	V5_SetReg(V5_REG_ISP_CTL_AUTO, x, BIT_16_LTOH);
}

UINT16 V5B_IspGetAfCtrl(void)
{
	UINT16 x = 0;

	x = (UINT16)V5_GetReg(V5_REG_ISP_CTL_AUTO, BIT_16_LTOH);
	return x;
}

void V5B_IspEnableAntiFlk(void)
{
	UINT16 x = 0;

	x = V5B_IspGetAfCtrl();
	x |= 0x01;
	V5B_IspSetAfCtrl(x);
}

void V5B_IspDisableAntiFlk(void)
{
	UINT16 x = 0;

	x = V5B_IspGetAfCtrl();
	x &= 0xfffe;
	V5B_IspSetAfCtrl(x);
}

UINT8 V5B_IspIsAntiFlk(void)
{
	UINT16 x = 0;

	x = V5B_IspGetAfCtrl();
	return (UINT8)(x & 0x01);
}

/********************************************************************************
Description:
set auto function interrupt frequency
Parameters:
frq:	auto function interrupt frequency
Note:
<TABLE>
value		mode
-----		----
0			Interrupt per 1 frame
1			Interrupt per 3 frame
2			Interrupt per 5 frame
3			Interrupt per 9 frame
4			Interrupt per 17 frame
5			Interrupt per 33 frame
6			Interrupt per 65 frame
7			Interrupt per 129 frame
</TABLE>
Remarks:
*********************************************************************************/
void V5B_IspSetAfIntFrq(UINT8 frq)
{
	UINT8 x = 0;

	x = (UINT8)V5_GetReg(V5_REG_ISP_CTL_AUTO, BIT_8);
	x &= 0xf1;
	x |= ((frq & 0x07) << 1);
	V5_SetReg(V5_REG_ISP_CTL_AUTO, x, BIT_8);
}

UINT8 V5B_IspGetAfIntFrq(void)
{
	UINT8 x = 0;

	x = (UINT8)V5_GetReg(V5_REG_ISP_CTL_AUTO, BIT_8);
	return ((x >> 1) & 0x07);
}

void V5B_IspEnableAeSmallYrange(void)
{
	UINT8 x = 0;

	x = (UINT8)V5_GetReg(V5_REG_ISP_CTL_AUTO, BIT_8);
	V5_SetReg(V5_REG_ISP_CTL_AUTO, (x | 0x10), BIT_8);
}

void V5B_IspEnableAeLargeYrange(void)
{
	UINT8 x = 0;

	x = (UINT8)V5_GetReg(V5_REG_ISP_CTL_AUTO, BIT_8);
	V5_SetReg(V5_REG_ISP_CTL_AUTO, (x & 0xef), BIT_8);
}

/********************************************************************************
Description:
set AE weight mode
Parameters:
mode:	AE weight mode setting
Note:
<TABLE>
value		mode
-----		----
0			Bweight mode
1			Wweight mode
2			Bweight+Wweight mode
3			Weightless mode
</TABLE>
Remarks:
*********************************************************************************/
void V5B_IspSetAeWeightMode(UINT8 mode)
{
	UINT8 x = 0;

	x = (UINT8)V5_GetReg(V5_REG_ISP_CTL_AUTO, BIT_8);
	x &= 0x3f;
	x |= ((mode & 0x03) << 6);
	V5_SetReg(V5_REG_ISP_CTL_AUTO, x, BIT_8);
}

UINT8 V5B_IspGetAeWeightMode(void)
{
	UINT8 x = 0;

	x = (UINT8)V5_GetReg(V5_REG_ISP_CTL_AUTO, BIT_8);
	return (x >> 6);
}

void V5B_IspEnableAwb(void)
{
	UINT8 x = 0;

	x = (UINT8)V5_GetReg(V5_REG_ISP_CTL_AUTO_H, BIT_8);
	V5_SetReg(V5_REG_ISP_CTL_AUTO_H, (x | 0x01), BIT_8);
}

void V5B_IspDisableAwb(void)
{
	UINT8 x = 0;

	x = (UINT8)V5_GetReg(V5_REG_ISP_CTL_AUTO_H, BIT_8);
	V5_SetReg(V5_REG_ISP_CTL_AUTO_H, (x & 0xfe), BIT_8);
}

UINT8 V5B_IspIsAwb(void)
{
	UINT8 x = 0;

	x = (UINT8)V5_GetReg(V5_REG_ISP_CTL_AUTO_H, BIT_8);
	return (x & 0x01);
}

void V5B_IspEnableAwbRev(void)
{
	UINT8 x = 0;

	x = (UINT8)V5_GetReg(V5_REG_ISP_CTL_AUTO_H, BIT_8);
	V5_SetReg(V5_REG_ISP_CTL_AUTO_H, (x | 0x02), BIT_8);
}

void V5B_IspDisableAwbRev(void)
{
	UINT8 x = 0;

	x = (UINT8)V5_GetReg(V5_REG_ISP_CTL_AUTO_H, BIT_8);
	V5_SetReg(V5_REG_ISP_CTL_AUTO_H, (x & 0xfd), BIT_8);
}

void V5B_IspEnableAwbOneWin(void)
{
	UINT8 x = 0;

	x = (UINT8)V5_GetReg(V5_REG_ISP_CTL_AUTO_H, BIT_8);
	V5_SetReg(V5_REG_ISP_CTL_AUTO_H, (x | 0x04), BIT_8);
}

void V5B_IspDisableAwbOneWin(void)
{
	UINT8 x = 0;

	x = (UINT8)V5_GetReg(V5_REG_ISP_CTL_AUTO_H, BIT_8);
	V5_SetReg(V5_REG_ISP_CTL_AUTO_H, (x & 0xfb), BIT_8);
}

void V5B_IspEnableNoiseRmv(void)
{
	UINT8 x = 0;

	x = (UINT8)V5_GetReg(V5_REG_ISP_CTL_AUTO_H, BIT_8);
	V5_SetReg(V5_REG_ISP_CTL_AUTO_H, (x | 0x08), BIT_8);
}

void V5B_IspDisableNoiseRmv(void)
{
	UINT8 x = 0;

	x = (UINT8)V5_GetReg(V5_REG_ISP_CTL_AUTO_H, BIT_8);
	V5_SetReg(V5_REG_ISP_CTL_AUTO_H, (x & 0xf7), BIT_8);
}

void V5B_IspEnableUvNoiseRmv(void)
{
	UINT8 x = 0;

	x = (UINT8)V5_GetReg(V5_REG_ISP_CTL_AUTO_H, BIT_8);
	V5_SetReg(V5_REG_ISP_CTL_AUTO_H, (x | 0x10), BIT_8);
}

void V5B_IspDisableUvNoiseRmv(void)
{
	UINT8 x = 0;

	x = (UINT8)V5_GetReg(V5_REG_ISP_CTL_AUTO_H, BIT_8);
	V5_SetReg(V5_REG_ISP_CTL_AUTO_H, (x & 0xef), BIT_8);
}

void V5B_IspEnableAutoFocus(void)
{
	UINT8 x = 0;

	x = (UINT8)V5_GetReg(V5_REG_ISP_CTL_AUTO_H, BIT_8);
	V5_SetReg(V5_REG_ISP_CTL_AUTO_H, (x | 0x20), BIT_8);
}

void V5B_IspDisableAutoFocus(void)
{
	UINT8 x = 0;

	x = (UINT8)V5_GetReg(V5_REG_ISP_CTL_AUTO_H, BIT_8);
	V5_SetReg(V5_REG_ISP_CTL_AUTO_H, (x & 0xdf), BIT_8);
}

void V5B_IspSetEdgeMatrix(UINT8 sel)
{
	UINT8 x = 0;

	x = (UINT8)V5_GetReg(V5_REG_ISP_CTL_AUTO_H, BIT_8);
	x &= 0xbf;
	x |= ((sel & 0x01) << 6);
	V5_SetReg(V5_REG_ISP_CTL_AUTO_H, x, BIT_8);
}

UINT8 V5B_IspGetEdgeMatrix(void)
{
	UINT8 x = 0;

	x = (UINT8)V5_GetReg(V5_REG_ISP_CTL_AUTO_H, BIT_8);
	return ((x >> 6) & 0x01);
}

void V5B_IspSetCtrl1(UINT8 x)
{
	V5_SetReg(V5_REG_ISP_CTL1, x, BIT_8);
}

UINT8 V5B_IspGetCtrl1(void)
{
	UINT8 x = 0;

	x = (UINT8)V5_GetReg(V5_REG_ISP_CTL1, BIT_8);
	return x;
}

void V5B_IspEnableHomoImg(void)
{
	UINT8 x = 0;

	x = V5B_IspGetCtrl1();
	V5B_IspSetCtrl1((x | 0x01));
}

void V5B_IspDisableHomoImg(void)
{
	UINT8 x = 0;

	x = V5B_IspGetCtrl1();
	V5B_IspSetCtrl1((x & 0xfe));
}

void V5B_IspSetFilterMode(UINT8 sel)
{
	UINT8 x = 0;

	x = V5B_IspGetCtrl1();
	x &= 0xfd;
	x |= ((sel << 1) & 0x02);
	V5B_IspSetCtrl1(x);
}

UINT8 V5B_IspGetFilterMode(void)
{
	UINT8 x = 0;

	x = V5B_IspGetCtrl1();
	return ((x >> 1) & 0x01);
}

void V5B_IspSetTestBarPtn(UINT8 sel)
{
	UINT8 x = 0;

	x = V5B_IspGetCtrl1();
	x &= 0xfb;
	x |= ((sel << 2) & 0x04);
	V5B_IspSetCtrl1(x);
}

UINT8 V5B_IspGetTestBarPtn(void)
{
	UINT8 x = 0;

	x = V5B_IspGetCtrl1();
	return ((x >> 2) & 0x01);
}

void V5B_IspEnableTestBar(void)
{
	UINT8 x = 0;

	x = V5B_IspGetCtrl1();
	V5B_IspSetCtrl1((x | 0x08));
}

void V5B_IspDisableTestBar(void)
{
	UINT8 x = 0;

	x = V5B_IspGetCtrl1();
	V5B_IspSetCtrl1((x & 0xf7));
}

void V5B_IspSetCtrlUpdate(UINT8 x)
{
	V5_SetReg(V5_REG_ISP_CTL_UPDATE, x, BIT_8);
}

void V5B_IspUpdateGlbGain(void)
{
	V5B_IspSetCtrlUpdate(BIT0);
}

void V5B_IspUpdateDigitalGain(void)
{
	V5B_IspSetCtrlUpdate(BIT1);
}

void V5B_IspUpdateBGamma(void)
{
	V5B_IspSetCtrlUpdate(BIT2);
}

void V5B_IspUpdateGGamma(void)
{
	V5B_IspSetCtrlUpdate(BIT3);
}

void V5B_IspUpdateRGamma(void)
{
	V5B_IspSetCtrlUpdate(BIT4);
}

void V5B_IspUpdateColMat(void)
{
	V5B_IspSetCtrlUpdate(BIT5);
}

void V5B_IspUpdateGray(void)
{
	V5B_IspSetCtrlUpdate(BIT6);
}

void V5B_IspUpdateFem(void)
{
	V5B_IspSetCtrlUpdate(BIT7);
}

/********************************************************************************
Description:
set AWB calculating color range
Parameters:
x:	range parameters
Note:
When calculating the average value of red, green and blue, not all 
the pixels in frame can contribute to the sum.  Only those pixels 
which satisfy the following condition are added together:
Ybot < Y < Ytop, I < Itop, Q < Qtop, In which Y, I Q is the luminance, 
inphase, quadratire of a pixel in YIQ color model
Remarks:

*********************************************************************************/
void V5B_IspSetColorRange(TV5IspAwbClrRange x)
{
	V5_SetReg(V5_REG_ISP_YBOT, x.ybot, BIT_8);
	V5_SetReg(V5_REG_ISP_YTOP, x.ytop, BIT_8);
	V5_SetReg(V5_REG_ISP_QTOP, x.qtop, BIT_8);
	V5_SetReg(V5_REG_ISP_ITOP, x.itop, BIT_8);
}

void V5B_IspGetColorRange(PTV5IspAwbClrRange px)
{
	px->ybot = (UINT8)V5_GetReg(V5_REG_ISP_YBOT, BIT_8);
	px->ytop = (UINT8)V5_GetReg(V5_REG_ISP_YTOP, BIT_8);
	px->qtop = (UINT8)V5_GetReg(V5_REG_ISP_QTOP, BIT_8);
	px->itop = (UINT8)V5_GetReg(V5_REG_ISP_ITOP, BIT_8);
}

/********************************************************************************
Description:
set AWB gain limit range
Parameters:
x:	range parameters
Note:
These registers define the threshold of the gain value when auto 
white balance is enabled. The produced gain value is 
GLOW <= GAIN <= GTOP
Remarks:
*********************************************************************************/
void V5B_IspSetAwbGainLmt(TV5IspAwbGainLmt x)
{
	V5_SetReg(V5_REG_ISP_GLOW, x.glow, BIT_8);
	V5_SetReg(V5_REG_ISP_GTOP, x.gtop, BIT_8);
}

void V5B_IspGetAwbGainLmt(PTV5IspAwbGainLmt px)
{
	px->glow = (UINT8)V5_GetReg(V5_REG_ISP_GLOW, BIT_8);
	px->gtop = (UINT8)V5_GetReg(V5_REG_ISP_GTOP, BIT_8);
}

/********************************************************************************
Description:
set AWB threshold
Parameters:
x:	threshold value
Note:
\Rdiff = | Rmean - Gmean |,		Bdiff = | Bmean - Gmean |
\The special R gain adjustment step length is determined by the 
relationship among Rdiff and AWBTHD1 AWBTHD2 as below , B gain 
adjustment step length is in the same way .
<TABLE>
value		meaning
-----		-------
BIT0-5		AWBSTEP
BIT6-9		AWBTHD1
BIT10-15	AWBTHD2
</TABLE>
Remarks:
*********************************************************************************/
void V5B_IspSetAwbCtrl(UINT16 x)
{
	V5_SetReg(V5_REG_ISP_AWBCTRL, x, BIT_16_LTOH);
}

UINT16 V5B_IspGetAwbCtrl(void)
{
	UINT16 x = 0;

	x = (UINT16)V5_GetReg(V5_REG_ISP_AWBCTRL, BIT_16_LTOH);
	return x;
}

void V5B_IspSetAwbStep(UINT8 val)
{
	UINT16 x = 0;

	x = V5B_IspGetAwbCtrl();
	x &= 0xffc0;
	x |= (val & 0x3f);
	V5B_IspSetAwbCtrl(x);
}

UINT8 V5B_IspGetAwbStep(void)
{
	UINT16 x = 0;

	x = V5B_IspGetAwbCtrl();
	return (UINT8)(x & 0x3f);
}

void V5B_IspSetAwbThd1(UINT8 val)
{
	UINT16 x = 0;

	x = V5B_IspGetAwbCtrl();
	x &= 0xfc3f;
	x |= ((((UINT16)val) << 6) & 0x03c0);
	V5B_IspSetAwbCtrl(x);
}

UINT8 V5B_IspGetAwbThd1(void)
{
	UINT16 x = 0;

	x = V5B_IspGetAwbCtrl();
	return (UINT8)((x >> 6) & 0x0f);
}

void V5B_IspSetAwbThd2(UINT8 val)
{
	UINT16 x = 0;

	x = V5B_IspGetAwbCtrl();
	x &= 0x03ff;
	x |= ((((UINT16)val) << 10) & 0xfc00);
	V5B_IspSetAwbCtrl(x);
}

UINT8 V5B_IspGetAwbThd2(void)
{
	UINT16 x = 0;

	x = V5B_IspGetAwbCtrl();
	return (UINT8)((x >> 10) & 0x3f);
}

UINT8 V5B_IspGetRMean(void)
{
	UINT8 x = 0;

	x = (UINT8)V5_GetReg(V5_REG_ISP_RMEAN, BIT_8);
	return x;
}

UINT8 V5B_IspGetGMean(void)
{
	UINT8 x = 0;

	x = (UINT8)V5_GetReg(V5_REG_ISP_GMEAN, BIT_8);
	return x;
}

UINT8 V5B_IspGetBMean(void)
{
	UINT8 x = 0;

	x = (UINT8)V5_GetReg(V5_REG_ISP_BMEAN, BIT_8);
	return x;
}

UINT8 V5B_IspGetYMean(void)
{
	UINT8 x = 0;

	x = (UINT8)V5_GetReg(V5_REG_ISP_YMEAN, BIT_8);
	return x;
}

UINT8 V5B_IspGetAfStatus(void)
{
	UINT8 x = 0;

	x = (UINT8)V5_GetReg(V5_REG_ISP_AUTO_STATUS, BIT_8);
	return x;
}

void V5B_IspSetDpdThd(UINT8 x)
{
	V5_SetReg(V5_REG_ISP_DPDTHD, x, BIT_8);
}

UINT8 V5B_IspGetDpdThd(void)
{
	UINT8 x = 0;

	x = (UINT8)V5_GetReg(V5_REG_ISP_DPDTHD, BIT_8);
	return x;
}

void V5B_IspSetNoiseTable(UINT8 *x)
{
	UINT8 i = 0;
	
	for(i = 0; i < 17; i++)
		V5_SetReg((V5_REG_ISP_NT0 + i), x[i], BIT_8);
}

void V5B_IspGetNoiseTable(UINT8 *x)
{
	UINT8 i = 0;
	
	for(i = 0; i < 17; i++)
		*(x + i) = (UINT8)V5_GetReg((V5_REG_ISP_NT0 + i), BIT_8);
}

void V5B_IspSetDpdThdUV(UINT8 sel)
{
	V5_SetReg(V5_REG_ISP_DPDTHD, sel, BIT_8);
}

UINT8 V5B_IspGetDpdThdUV(void)
{
	UINT8 sel = 0;
	
	sel = (UINT8)V5_GetReg(V5_REG_ISP_DPDTHD, BIT_8);
	return sel;
}

void V5B_IspSetNoiseTableUV(UINT8 *x)
{
	UINT8 i = 0;
	
	for(i = 0; i < 17; i++)
		V5_SetReg((V5_REG_ISP_NT0_UV + i), x[i], BIT_8);
}

void V5B_IspGetNoiseTableUV(UINT8 *x)
{
	UINT8 i = 0;
	
	for(i = 0; i < 17; i++)
		*(x + i) = (UINT8)V5_GetReg((V5_REG_ISP_NT0_UV + i), BIT_8);
}

void V5B_IspSetNormalChromaNoiseTableUV(void)
{
	UINT8 table[17];

	memset(table, 0xff, 17);
	V5B_IspSetNoiseTableUV(table);
}

void V5B_IspSetLowChromaNoiseTableUV(void)
{
	UINT8 table[17];

	memset(table, 0x10, 17);
	V5B_IspSetNoiseTableUV(table);
}

void V5B_IspSetBLROffset(UINT16 x)
{
	V5_SetReg(V5_REG_ISP_BL_R_OFFSET, x, BIT_16_LTOH);
}

UINT16 V5B_IspGetBLROffset(void)
{
	UINT16 x = 0;

	x = (UINT16)V5_GetReg(V5_REG_ISP_BL_R_OFFSET, BIT_16_LTOH);
	return x;
}

void V5B_IspSetBLBOffset(UINT16 x)
{
	V5_SetReg(V5_REG_ISP_BL_B_OFFSET, x, BIT_16_LTOH);
}

UINT16 V5B_IspGetBLBOffset(void)
{
	UINT16 x = 0;

	x = (UINT16)V5_GetReg(V5_REG_ISP_BL_B_OFFSET, BIT_16_LTOH);
	return x;
}

void V5B_IspSetBLG1Offset(UINT16 x)
{
	V5_SetReg(V5_REG_ISP_BL_G1_OFFSET, x, BIT_16_LTOH);
}

UINT16 V5B_IspGetBLG1Offset(void)
{
	UINT16 x = 0;

	x = (UINT16)V5_GetReg(V5_REG_ISP_BL_G1_OFFSET, BIT_16_LTOH);
	return x;
}

void V5B_IspSetBLG2Offset(UINT16 x)
{
	V5_SetReg(V5_REG_ISP_BL_G2_OFFSET, x, BIT_16_LTOH);
}

UINT16 V5B_IspGetBLG2Offset(void)
{
	UINT16 x = 0;

	x = (UINT16)V5_GetReg(V5_REG_ISP_BL_G2_OFFSET, BIT_16_LTOH);
	return x;
}

/********************************************************************************
Description:
Edge sum for auto focus
Edge_sum is used to do auto focus, adjust the lens to the position where you can get the biggest edge_sum of a frame.
If a picture is in focus, the sum of it's edge-map data is bigges
Returns:
edge sum value
Note:
read only
Remarks:
*********************************************************************************/
UINT32 V5B_IspGetEdgeSum(void)
{
	UINT32 x = 0;

	x = V5_GetReg(V5_REG_ISP_EDGE_SUM_L, BIT_32_LTOH);
	return x;
}

/********************************************************************************
Description:
set AWB/AE Window Horizontal Position
Parameters:
x:	AWB/AE Window Horizontal Position
Remarks:
*********************************************************************************/
void V5B_IspSetAHStart(UINT8 *x)
{
	V5_SetReg(V5_REG_ISP_AEAWB_WIN_HPOS_0, x[0], BIT_8);
	V5_SetReg(V5_REG_ISP_AEAWB_WIN_HPOS_1, x[1], BIT_8);
	V5_SetReg(V5_REG_ISP_AEAWB_WIN_HPOS_2, x[2], BIT_8);
	V5_SetReg(V5_REG_ISP_AEAWB_WIN_HPOS_3, x[3], BIT_8);
	V5_SetReg(V5_REG_ISP_AEAWB_WIN_HPOS_4, x[4], BIT_8);
}

void V5B_IspGetAHStart(UINT8 *x)
{
	*x = (UINT8)V5_GetReg(V5_REG_ISP_AEAWB_WIN_HPOS_0, BIT_8);
	*(x + 1) = (UINT8)V5_GetReg(V5_REG_ISP_AEAWB_WIN_HPOS_1, BIT_8);
	*(x + 2) = (UINT8)V5_GetReg(V5_REG_ISP_AEAWB_WIN_HPOS_2, BIT_8);
	*(x + 3) = (UINT8)V5_GetReg(V5_REG_ISP_AEAWB_WIN_HPOS_3, BIT_8);
	*(x + 4) = (UINT8)V5_GetReg(V5_REG_ISP_AEAWB_WIN_HPOS_4, BIT_8);
}

/********************************************************************************
Description:
set AWB/AE Window Vertical  Position
Parameters:
x:	AWB/AE Window Vertical  Position
Remarks:
*********************************************************************************/
void V5B_IspSetAVStart(UINT8 *x)
{
	V5_SetReg(V5_REG_ISP_AEAWB_WIN_VPOS_0, x[0], BIT_8);
	V5_SetReg(V5_REG_ISP_AEAWB_WIN_VPOS_1, x[1], BIT_8);
	V5_SetReg(V5_REG_ISP_AEAWB_WIN_VPOS_2, x[2], BIT_8);
	V5_SetReg(V5_REG_ISP_AEAWB_WIN_VPOS_3, x[3], BIT_8);
	V5_SetReg(V5_REG_ISP_AEAWB_WIN_VPOS_4, x[4], BIT_8);
}

void V5B_IspGetAVStart(UINT8 *x)
{
	*x = (UINT8)V5_GetReg(V5_REG_ISP_AEAWB_WIN_VPOS_0, BIT_8);
	*(x + 1) = (UINT8)V5_GetReg(V5_REG_ISP_AEAWB_WIN_VPOS_1, BIT_8);
	*(x + 2) = (UINT8)V5_GetReg(V5_REG_ISP_AEAWB_WIN_VPOS_2, BIT_8);
	*(x + 3) = (UINT8)V5_GetReg(V5_REG_ISP_AEAWB_WIN_VPOS_3, BIT_8);
	*(x + 4) = (UINT8)V5_GetReg(V5_REG_ISP_AEAWB_WIN_VPOS_4, BIT_8);
}

/********************************************************************************
Description:
set histogram enable/disable
Parameters:
sel:	setting value
Note:
<TABLE>
value			description
BIT7-2			reserve
BIT1			1 : histogram statistic result is available
BIT0			1 : histogram compensation takes effect on the 
Y channel of YUV format data stream

</TABLE>
Remarks:
*********************************************************************************/
void V5B_IspSetHistCtrl(UINT8 x)
{
	V5_SetReg(V5_REG_ISP_HIS_CTRL, x, BIT_8);
}

UINT8 V5B_IspGetHistCtrl(void)
{
	UINT8 sel = 0;

	sel = (UINT8)V5_GetReg(V5_REG_ISP_HIS_CTRL, BIT_8);
	return sel;
}

void V5B_IspEnableHistEffect(void)
{
	UINT8 x = 0;

	x = V5B_IspGetHistCtrl();
	V5B_IspSetHistCtrl(x | 0x01);
}

void V5B_IspDisableHistEffect(void)
{
	UINT8 x = 0;

	x = V5B_IspGetHistCtrl();
	V5B_IspSetHistCtrl(x & 0xfe);
}

void V5B_IspEnableHistManual(void)
{
	UINT8 x = 0;

	x = V5B_IspGetHistCtrl();
	V5B_IspSetHistCtrl(x | 0x02);
}

void V5B_IspDisableHistManual(void)
{
	UINT8 x = 0;

	x = V5B_IspGetHistCtrl();
	V5B_IspSetHistCtrl(x & 0xfd);
}

void V5B_IspSetHistGain(UINT8 x)
{
	V5_SetReg(V5_REG_ISP_HIS_MANUAL_GAIN, x, BIT_8);
}

UINT8 V5B_IspGetHistGain(void)
{
	UINT8 sel = 0;

	sel = (UINT8)V5_GetReg(V5_REG_ISP_HIS_MANUAL_GAIN, BIT_8);
	return sel;
}

void V5B_IspSetYMeanTarget(UINT8 x)
{
	V5_SetReg(V5_REG_ISP_Y_MEAN_TGT, x, BIT_8);
}

UINT8 V5B_IspGetYMeanTarget(void)
{
	UINT8 x = 0;

	x = (UINT8)V5_GetReg(V5_REG_ISP_Y_MEAN_TGT, BIT_8);
	return x;
}

void V5B_IspSetYMeanThd(UINT8 x)
{
	V5_SetReg(V5_REG_ISP_Y_MEAN_THD, x, BIT_8);
}

UINT8 V5B_IspGetYMeanThd(void)
{
	UINT8 x = 0;

	x = (UINT8)V5_GetReg(V5_REG_ISP_Y_MEAN_THD, BIT_8);
	return x;
}

UINT8 V5B_IspGetAfInt(void)
{
	UINT8 x = 0;

	x = (UINT8)V5_GetReg(V5_REG_ISP_INTERRUPT, BIT_8);
	return x;
}

void V5B_IspClrAFocusInt(void)
{
	UINT8 x = 0;

	x = V5B_IspGetAfInt();
	V5_SetReg(V5_REG_ISP_INTERRUPT, (x & 0xfe), BIT_8);
}

void V5B_IspClrAFlickerInt(void)
{
	UINT8 x = 0;

	x = V5B_IspGetAfInt();
	V5_SetReg(V5_REG_ISP_INTERRUPT, (x & 0xfd), BIT_8);
}

void V5B_IspClrYMeanInt(void)
{
	UINT8 x = 0;

	x = V5B_IspGetAfInt();
	V5_SetReg(V5_REG_ISP_INTERRUPT, (x & 0xfb), BIT_8);
}

void V5B_IspSetAeWeight(UINT8 *x)
{
	UINT8 i = 0, temp = 0;

	for(i = 0; i < 8; i++)
	{
		temp = (x[2 * i] & 0x0f) | ((x[2 * i+1] << 4) & 0xf0);
		V5_SetReg((V5_REG_ISP_AE_WT_0 + i), temp, BIT_8);
	}
}

void V5B_IspGetAeWeight(UINT8 *x)
{
	UINT8 i = 0, temp = 0;

	for(i = 0; i < 8; i++)
	{
		temp = (UINT8)V5_GetReg((V5_REG_ISP_AE_WT_0 + i), BIT_8);
		x[2 * i] = (temp & 0x0f);
		x[2 * i + 1] = ((temp >> 4) & 0x0f);
	}
}

void V5B_IspGetWinYMean(UINT8 *x)
{
	UINT8 i = 0;

	for(i = 0; i < 16; i++)
		*(x + i) = (UINT8)V5_GetReg((V5_REG_ISP_WIN_YMEAN_11 + i), BIT_8);
}

void V5B_IspEnableHue(void)
{
	UINT8 x = 0;

	x = (UINT8)V5_GetReg(V5_REG_ISP_HUE_EN, BIT_8);
	V5_SetReg(V5_REG_ISP_HUE_EN, (x | 0x01), BIT_8);
}

void V5B_IspDisableHue(void)
{
	UINT8 x = 0;

	x = (UINT8)V5_GetReg(V5_REG_ISP_HUE_EN, BIT_8);
	V5_SetReg(V5_REG_ISP_HUE_EN, (x & 0xfe), BIT_8);
}

UINT8 V5B_IspIsHueEnable(void)
{
	UINT8 x = 0;

	x = (UINT8)V5_GetReg(V5_REG_ISP_HUE_EN, BIT_8);
	return x;
}

void V5B_IspSetHueCos(int x)
{
	UINT16 val = 0;

	val = (x < 0) ? ((UINT16)((x & 0x1ff) | 0x200)) : ((UINT16)(x & 0x1ff));
	V5_SetReg(V5_REG_ISP_HUE_COS, x, BIT_16_LTOH);
}

int V5B_IspGetHueCos(void)
{
	int val = 0;
	UINT16 x = 0;

	x = (UINT16)V5_GetReg(V5_REG_ISP_HUE_COS, BIT_16_LTOH);
	val = (x & 0x200) ? (-(x & 0x1ff)) : (x & 0x1ff);
	return val;
}

void V5B_IspSetHueSin(int x)
{
	UINT16 val = 0;

	val = (x < 0) ? ((UINT16)((x & 0x1ff) | 0x200)) : ((UINT16)(x & 0x1ff));
	V5_SetReg(V5_REG_ISP_HUE_SIN, x, BIT_16_LTOH);
}

int V5B_IspGetHueSin(void)
{
	int val = 0;
	UINT16 x = 0;

	x = (UINT16)V5_GetReg(V5_REG_ISP_HUE_SIN, BIT_16_LTOH);
	val = (x & 0x200) ? (-(x & 0x1ff)) : (x & 0x1ff);
	return val;
}

void V5B_IspSetSat(UINT8 x)
{
	V5_SetReg(V5_REG_ISP_SATURATION, x, BIT_8);
}

UINT8 V5B_IspGetSat(void)
{
	UINT8 x = 0;

	x = (UINT8)V5_GetReg(V5_REG_ISP_SATURATION, BIT_8);
	return x;
}

void V5B_IspSetHueYOff(int x)
{
	UINT16 val = 0;

	val = (x < 0) ? ((UINT16)((x & 0xff) | 0x100)) : ((UINT16)(x & 0xff));
	V5_SetReg(V5_REG_ISP_HUE_YOFF, x, BIT_16_LTOH);
}

int V5B_IspGetHueYOff(void)
{
	int val = 0;
	UINT16 x = 0;

	x = (UINT16)V5_GetReg(V5_REG_ISP_HUE_YOFF, BIT_16_LTOH);
	val = (x & 0x100) ? (-(x & 0xff)) : (x & 0xff);
	return val;
}

void V5B_IspSetHueUOff(int x)
{
	UINT16 val = 0;

	val = (x < 0) ? ((UINT16)((x & 0xff) | 0x100)) : ((UINT16)(x & 0xff));
	V5_SetReg(V5_REG_ISP_HUE_UOFF, x, BIT_16_LTOH);
}

int V5B_IspGetHueUOff(void)
{
	int val = 0;
	UINT16 x = 0;

	x = (UINT16)V5_GetReg(V5_REG_ISP_HUE_UOFF, BIT_16_LTOH);
	val = (x & 0x100) ? (-(x & 0xff)) : (x & 0xff);
	return val;
}

void V5B_IspSetHueVOff(int x)
{
	UINT16 val = 0;

	val = (x < 0) ? ((UINT16)((x & 0xff) | 0x100)) : ((UINT16)(x & 0xff));
	V5_SetReg(V5_REG_ISP_HUE_VOFF, x, BIT_16_LTOH);
}

int V5B_IspGetHueVOff(void)
{
	int val = 0;
	UINT16 x = 0;

	x = (UINT16)V5_GetReg(V5_REG_ISP_HUE_VOFF, BIT_16_LTOH);
	val = (x & 0x100) ? (-(x & 0xff)) : (x & 0xff);
	return val;
}

void V5B_IspModuleReset(void)
{
	V5_SetReg(V5_REG_ISP_RESET, 0x01, BIT_8);
}

void V5B_IspRegReset(void)
{
	V5_SetReg(V5_REG_ISP_RESET, 0x02, BIT_8);
}

#endif //V578CMD_DISABLE
