

#include "basefunc.h"

#ifndef	V578CMD_DISABLE

UINT8 V5B_LbufGetStatus(void)
{
	UINT32 x;

	x = V5_GetReg(V5_REG_LBUF_LBUF_STATUS, BIT_8);

	return (UINT8)x;
}

UINT8 V5B_Lbuf0Isok(void)
{
	UINT8 x;

	x = V5B_LbufGetStatus();
	return x & 1;
}

UINT8 V5B_Lbuf1Isok(void)
{
	UINT8 x;

	x = V5B_LbufGetStatus();
	return x & 2;
}

UINT8 V5B_LbufIsHblankDone(void)
{
	UINT8 x;

	x = V5B_LbufGetStatus();
	return x & 0x80;
}

UINT8 V5B_LbufGetLinenum(void)
{
	UINT8 x;

	x = V5B_LbufGetStatus();
	x >>= 2;
	return x & 0x1f;
}

void V5B_LbufSetYuvModeCtrl(UINT8 ctrl)
{
	V5_SetReg(V5_REG_LBUF_YUV_MODE, ctrl, BIT_8);
}

UINT8 V5B_LbufGetYuvModeCtrl(void)
{
	UINT32 x;

	x = V5_GetReg(V5_REG_LBUF_YUV_MODE, BIT_8);

	return (UINT8)x;
}

void V5B_LbufSetYuvMode(UINT8 mode)
{
	UINT8 x;

	x = V5B_LbufGetYuvModeCtrl();

	V5B_LbufSetYuvModeCtrl((x&(~0x7))|(mode&0x7));
}

UINT8 V5B_LbufGetYuvMode(void)
{
	UINT8 x;

	x = V5B_LbufGetYuvModeCtrl();

	return x&0x7;
}

UINT8 V5B_LbufSetDataFromLcd(void)
{
	UINT8 x;

	x = V5B_LbufGetYuvModeCtrl();
	V5B_LbufSetYuvModeCtrl((UINT8)(x|BIT3));

	return x;
}

UINT8 V5B_LbufSetDataFromIpp(void)
{
	UINT8 x;

	x = V5B_LbufGetYuvModeCtrl();
	V5B_LbufSetYuvModeCtrl((UINT8)(x&(~BIT3)));

	return x;
}

void V5B_LbufSetPixrate(UINT8 val)
{
	UINT8 x;

	x = V5B_LbufGetYuvModeCtrl();
	V5B_LbufSetYuvModeCtrl((x&0x0f)|(val<<4));
}

UINT8 V5B_LbufGetPixrate(void)
{
	UINT8 x;

	x = V5B_LbufGetYuvModeCtrl();

	return x>>4;
}

void V5B_LbufSetLbufModeCtrl(UINT8 ctrl)
{
	V5_SetReg(V5_REG_LBUF_LBUF_MODE, ctrl, BIT_8);
}

UINT8 V5B_LbufGetLbufModeCtrl(void)
{
	UINT32 x;

	x = V5_GetReg(V5_REG_LBUF_LBUF_MODE, BIT_8);

	return (UINT8)x;
}

UINT8 V5B_LbufVsyncDelayEnable(void)
{
	UINT8 x;

	x = V5B_LbufGetLbufModeCtrl();
	V5B_LbufSetLbufModeCtrl((UINT8)(x&(~BIT5)));

	return x;
}

UINT8 V5B_LbufVsyncDelayDisable(void)
{
	UINT8 x;

	x = V5B_LbufGetLbufModeCtrl();
	V5B_LbufSetLbufModeCtrl((UINT8)(x|BIT5));

	return x;
}

void V5B_LbufSetMode(UINT8 mode)
{
	UINT8 x;

	x = V5B_LbufGetLbufModeCtrl();
	V5B_LbufSetLbufModeCtrl((x&0xe0)|mode);
}

UINT8 V5B_LbufGetMode(void)
{
	UINT8 x;

	x = V5B_LbufGetLbufModeCtrl();

	return x&0x1f;
}

void V5B_LbufSetInterCtrl(UINT8 val)
{
	V5_SetReg(V5_REG_LBUF_LBUF_CNT_RESET, val, BIT_8);
}

UINT8 V5B_LbufGetInterCtrl(void)
{
	UINT32 x;

	x = V5_GetReg(V5_REG_LBUF_LBUF_CNT_RESET, BIT_8);

	return (UINT8)x;
}

UINT8 V5B_LbufCounterReset(void)
{
	UINT8 x;

	x = V5B_LbufGetInterCtrl();
	V5B_LbufSetInterCtrl((UINT8)(x|V5B_LBUF_INTERCTRL_RESET));

	return x;
}

UINT8 V5B_LbufDecBypEnable(void)
{
	UINT8 x;

	x = V5B_LbufGetInterCtrl();
	V5B_LbufSetInterCtrl((UINT8)(x|V5B_LBUF_INTERCTRL_DECBYPASS));

	return x;
}

UINT8 V5B_LbufDecBypDisable(void)
{
	UINT8 x;

	x = V5B_LbufGetInterCtrl();
	V5B_LbufSetInterCtrl((UINT8)(x&(~V5B_LBUF_INTERCTRL_DECBYPASS)));

	return x;
}

UINT8 V5B_LbufDec2Yuv422(void)
{
	UINT8 x;

	x = V5B_LbufGetInterCtrl();
	V5B_LbufSetInterCtrl((UINT8)(x&(~V5B_LBUF_INTERCTRL_RGBSEL)));

	return x;
}

UINT8 V5B_LbufDec2Rgb565(void)
{
	UINT8 x;

	x = V5B_LbufGetInterCtrl();
	V5B_LbufSetInterCtrl((UINT8)(x|V5B_LBUF_INTERCTRL_RGBSEL));

	return x;
}

UINT8 V5B_LbufDecPreEnable(void)
{
	UINT8 x;

	x = V5B_LbufGetInterCtrl();
	V5B_LbufSetInterCtrl((UINT8)(x&(~V5B_LBUF_INTERCTRL_DECPREF)));

	return x;
}

UINT8 V5B_LbufDecPreDisable(void)
{
	UINT8 x;

	x = V5B_LbufGetInterCtrl();
	V5B_LbufSetInterCtrl((UINT8)(x|V5B_LBUF_INTERCTRL_DECPREF));

	return x;
}

UINT8 V5B_LbufEncPreEnable(void)
{
	UINT8 x;

	x = V5B_LbufGetInterCtrl();
	V5B_LbufSetInterCtrl((UINT8)(x&(~V5B_LBUF_INTERCTRL_ENCPREF)));

	return x;
}

UINT8 V5B_LbufEncPreDisable(void)
{
	UINT8 x;

	x = V5B_LbufGetInterCtrl();
	V5B_LbufSetInterCtrl((UINT8)(x|V5B_LBUF_INTERCTRL_ENCPREF));

	return x;
}

void V5B_LbufSetWidth(UINT16 val)
{
	V5_SetReg(V5_REG_LBUF_WIDTH_H, val, BIT_16_HTOL);
}

UINT16 V5B_LbufGetWidth(void)
{
	UINT32 x;

	x = V5_GetReg(V5_REG_LBUF_WIDTH_H, BIT_16_HTOL);

    return (UINT16)x;
}


void V5B_LbufSetHblank(UINT16 val)
{
	V5_SetReg(V5_REG_LBUF_HBLANK_H, val, BIT_16_HTOL);
}

UINT16 V5B_LbufGetHblank(void)
{
	UINT32 x;

	x = V5_GetReg(V5_REG_LBUF_HBLANK_H, BIT_16_HTOL);

	return (UINT16)x;
}

void V5B_LbufSetAddrInMarb(UINT32 val)
{
	V5_SetReg(V5_REG_LBUF_INI_ADDR_H, val, BIT_24_HTOL);
}

UINT32 V5B_LbufGetAddrInMarb(void)
{
	UINT32 x;

	x = V5_GetReg(V5_REG_LBUF_INI_ADDR_H, BIT_24_HTOL);

	return x;
}

UINT32 V5B_LbufGetBufAddr(void)
{
	UINT32 x;

	x = V5_GetReg(V5_REG_LBUF_ADDR_BUF_H, BIT_24_HTOL);

	return x;
}

void V5B_LbufSetCapDlyCtrl(UINT16 val)
{
	V5_SetReg(V5_REG_LBUF_CAP_DLY_H, val, BIT_16_HTOL);
}

UINT16 V5B_LbufGetCapDlyCtrl(void)
{
	UINT32 x;

	x = V5_GetReg(V5_REG_LBUF_CAP_DLY_H, BIT_16_HTOL);

	return (UINT16)x;
}

UINT16 V5B_LbufCapDlyEnable(void)
{
	UINT16 x;

	x = V5B_LbufGetCapDlyCtrl();
	V5B_LbufSetCapDlyCtrl( (UINT16)( x|BIT15 ) );

	return x;
}

UINT16 V5B_LbufCapDlyDisable(void)
{
	UINT16 x;

	x = V5B_LbufGetCapDlyCtrl();
	V5B_LbufSetCapDlyCtrl( (UINT16)( x & (~BIT15) ) );

	return x;
}

void V5B_LbufSetCapDly(UINT16 val)
{
	UINT16 x;

	if(val > 0x7fff)
		val = 0x7fff;
	x = V5B_LbufGetCapDlyCtrl();
	V5B_LbufSetCapDlyCtrl( (UINT16)( (x&BIT15)|val ) );
}

UINT16 V5B_LbufGetCapDly(void)
{
	UINT16 x;

	x = V5B_LbufGetCapDlyCtrl();
	return  (UINT16)( x & (~BIT15) );
}



#endif //V578CMD_DISABLE
