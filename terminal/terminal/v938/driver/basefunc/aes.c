#include "basefunc938.h"

#ifndef	V938CMD_DISABLE


void  V9B_AesSetCorCmd(UINT8 val)
{
	V9_SetReg(V9_REG_PAGE3_AES_CORE_CMD, val, BIT_8);
}

UINT8 V9B_AesGetCorCmd(void)
{
	UINT8 val;

	val = (UINT8)V9_GetReg(V9_REG_PAGE3_AES_CORE_CMD, BIT_8);

	return val;
}

/** 0x0: bypass mode            0x1: reserved         **/
/** 0x2: decrypt ECB mode       0x3: decrypt CBC mode **/
void V9B_AesSetCorWorkMod(UINT8 workMod)
{
    UINT8 val;

	val =  (UINT8)V9_GetReg(V9_REG_PAGE3_AES_CORE_CMD, BIT_8);
	val &= 0xfc;
	val |= workMod;
    V9_SetReg(V9_REG_PAGE3_AES_CORE_CMD, val, BIT_8);
}

UINT8 V9B_AesGetCorWorkMod(void)
{
    UINT8 val;

	val =  (UINT8)V9_GetReg(V9_REG_PAGE3_AES_CORE_CMD, BIT_8);
	val &= 0x03;

	return val;
}

void V9B_AesRstCorSta(void)
{
	UINT8 val;

	val =  (UINT8)V9_GetReg(V9_REG_PAGE3_AES_CORE_CMD, BIT_8);
	val &= 0xdf;
	val |= 0x20;
    V9_SetReg(V9_REG_PAGE3_AES_CORE_CMD, val, BIT_8);
}

UINT8 V9B_AesGetDatVld(void)
{
    UINT8 val;

	val =  (UINT8)V9_GetReg(V9_REG_PAGE3_AES_CORE_CMD, BIT_8);
	val &= 0x40;

	return val;
}

UINT8 V9B_AesGetBusySta(void)
{
    UINT8 val;

	val =  (UINT8)V9_GetReg(V9_REG_PAGE3_AES_CORE_CMD, BIT_8);
	val &= 0x80;

	return val;
}

void V9B_AesSetDecStrbDlyCnt(UINT8 dlyCnt)
{
    V9_SetReg(V9_REG_PAGE3_MP3_STRB_DLY, dlyCnt, BIT_8);
}

void V9B_AesSetKey(UINT8 *pKey)
{
	UINT8 i;

	for(i = 0; i < 16; i++)
        V9_SetReg(V9_REG_PAGE3_AES_KEY0 + i, *pKey++, BIT_8);
}

void V9B_AesGetKey(UINT8 *pKey)
{
	UINT8 i;
	UINT8 *pTmpKey = pKey;

	for(i = 0; i < 16; i++)
        *pTmpKey++ = (UINT8)V9_GetReg(V9_REG_PAGE3_AES_KEY0 + i, BIT_8);
}

void V9B_AesSetIv(UINT8 *pIv)
{
	UINT8 i;

	for(i = 0; i < 16; i++)
        V9_SetReg(V9_REG_PAGE3_AES_IV0 + i, *pIv++, BIT_8);
}

void V9B_AesGetIv(UINT8 *pIv)
{
	UINT8 i;
	UINT8 *pTmpIv = pIv;

	for(i = 0; i < 16; i++)
        *pTmpIv++ = (UINT8)V9_GetReg(V9_REG_PAGE3_AES_IV0 + i, BIT_8);
}

#endif

