
#include "basefunc.h"

#ifndef	V578CMD_DISABLE

void V5B_JpegSetMod(UINT8 val)
{
	V5_SetReg(V5_REG_JPEG_JPEG_MODE, val, BIT_8);
}

UINT8 V5B_JpegGetMod(void)
{
	UINT8 x;

	x = (UINT8)V5_GetReg(V5_REG_JPEG_JPEG_MODE, BIT_8);

	return x;
}

void V5B_JpegSetWorkMod(UINT8 val)
{
	UINT8 x;

	x = V5B_JpegGetMod();
	V5B_JpegSetMod((x&(~(BIT0|BIT1)))|(val&(BIT0|BIT1)));
}

UINT8 V5B_JpegGetWorkMod(void)
{
	UINT8 x;

	x = V5B_JpegGetMod();

	return (x&(BIT0|BIT1));
}

UINT8 V5B_JpegSimHeadEnable(void)
{
	UINT8 x;

	x = V5B_JpegGetMod();
	V5B_JpegSetMod((UINT8)(x|BIT2));

	return x;
}

UINT8 V5B_JpegSimHeadDisable(void)
{
	UINT8 x;

	x = V5B_JpegGetMod();
	V5B_JpegSetMod((UINT8)(x&(~BIT2)));

	return x;
}

UINT8 V5B_JpegCustQTEnable(void)
{
	UINT8 x;

	x = V5B_JpegGetMod();
	V5B_JpegSetMod((UINT8)(x|BIT3));

	return x;
}

UINT8 V5B_JpegCustQTDisable(void)
{
	UINT8 x;

	x = V5B_JpegGetMod();
	V5B_JpegSetMod((UINT8)(x&(~BIT3)));

	return x;
}

UINT8 V5B_JpegDispVdEnable(void)
{
	UINT8 x;

	x = V5B_JpegGetMod();
	V5B_JpegSetMod((UINT8)(x|BIT4));

	return x;
}

UINT8 V5B_JpegDispVdDisable(void)
{
	UINT8 x;

	x = V5B_JpegGetMod();
	V5B_JpegSetMod((UINT8)(x&(~BIT4)));

	return x;
}

UINT8 V5B_JpegCustHTEnable(void)
{
	UINT8 x;

	x = V5B_JpegGetMod();
	V5B_JpegSetMod((UINT8)(x|BIT5));

	return x;
}

UINT8 V5B_JpegCustHTDisable(void)
{
	UINT8 x;

	x = V5B_JpegGetMod();
	V5B_JpegSetMod((UINT8)(x&(~BIT5)));

	return x;
}

UINT8 V5B_JpegAviFmtEnable(void)
{
	UINT8 x;

	x = V5B_JpegGetMod();
	V5B_JpegSetMod((UINT8)(x|(BIT6)));

	return x;
}

UINT8 V5B_JpegAviFmtDisable(void)
{
	UINT8 x;

	x = V5B_JpegGetMod();
	V5B_JpegSetMod((UINT8)(x&(~BIT6)));

	return x;
}

UINT8	V5B_JpegEnhBrcEnable(void)
{
	UINT8 x;

	x = V5B_JpegGetMod();
	V5B_JpegSetMod((UINT8)(x|(BIT7)));

	return x;
}

UINT8	V5B_JpegEnhBrcDisable(void)
{
	UINT8 x;

	x = V5B_JpegGetMod();
	V5B_JpegSetMod((UINT8)(x&(~BIT7)));

	return x;
}

void V5B_JpegSetCtrl(UINT8 val)
{
	V5_SetReg(V5_REG_JPEG_JPEG_CTRL, val, BIT_8);
}

UINT8 V5B_JpegGetCtrl(void)
{
	UINT8 x;

	x = (UINT8)V5_GetReg(V5_REG_JPEG_JPEG_CTRL, BIT_8);

	return x;
}

void V5B_JpegStartEncode(void)
{
	V5B_JpegSetCtrl(BIT0);
}

void V5B_JpegStartDecode(void)
{
	V5B_JpegSetCtrl(BIT1);
}

void V5B_JpegRestartDecode(void)
{
	V5B_JpegSetCtrl(BIT2);
}

void V5B_JpegResetState(void)
{
	V5B_JpegSetCtrl(BIT3);
}

void V5B_JpegStartWriteQT(void)
{
	V5B_JpegSetCtrl(BIT4);
}

void V5B_JpegStartWriteHT(void)
{
	V5B_JpegSetCtrl(BIT5);
}

void V5B_JpegSelectHT(UINT8 sel)
{
	switch(sel)
	{
	case 0x0:
	case 0x1:
	case 0x2:
	case 0x3:
		V5B_JpegSetCtrl((UINT8)(sel<<6));
		break;
	default:
		break;
	}
}

UINT8 V5B_JpegGetStatus(void)
{
	UINT8 x;

	x = (UINT8)V5_GetReg(V5_REG_JPEG_JPEG_STATUS, BIT_8);
	
	return x;
}

void V5B_JpegSetBrc(UINT8 val)
{
	V5_SetReg(V5_REG_JPEG_BRC, val, BIT_8);
}

UINT8 V5B_JpegGetBrc(void)
{
	UINT8 x;

	x = (UINT8)V5_GetReg(V5_REG_JPEG_BRC, BIT_8);
	
	return x;
}

void V5B_JpegSetBrcQF(UINT8 val)
{
	UINT8 x;

	x = V5B_JpegGetBrc();
	V5B_JpegSetBrc((x&(~0x7f))|(val&0x7f));
}

UINT8 V5B_JpegGetBrcQF(void)
{
	UINT8 x;

	x = V5B_JpegGetBrc();
	
	return x&0x7f;
}

UINT8 V5B_JpegEnAbrc(void)
{
	UINT8 x;

	x = V5B_JpegGetBrc();
	V5B_JpegSetBrc(x|0x80);
	
	return x;
}

UINT8 V5B_JpegDisAbrc(void)
{
	UINT8 x;

	x = V5B_JpegGetBrc();
	V5B_JpegSetBrc(x&(~0x80));
	
	return x;
}

void V5B_JpegSetTCR(UINT8 val)
{
	V5_SetReg(V5_REG_JPEG_TCR, val, BIT_8);
}

UINT8 V5B_JpegGetTCR(void)
{
	UINT8 x;

	x = (UINT8)V5_GetReg(V5_REG_JPEG_TCR, BIT_8);
	
	return x;
}

UINT8 V5B_JpegGetQfactor(void)
{
	UINT8 x;

	x = (UINT8)V5_GetReg(V5_REG_JPEG_QF, BIT_8);
	
	return x;
}

void V5B_JpegSetHeaderStep(UINT8 val)
{
	V5_SetReg(V5_REG_JPEG_HEADER_STEP, val, BIT_8);
}

UINT8 V5B_JpegGetHeaderStep(void)
{
	UINT8 x;

	x = (UINT8)V5_GetReg(V5_REG_JPEG_HEADER_STEP, BIT_8);
	
	return x;
}

void V5B_JpegSetVWC(UINT32 val)
{
	V5_SetReg(V5_REG_JPEG_VIDEO_WC_0, val, BIT_32_HTOL);
}

UINT32 V5B_JpegGetVWC(void)
{
	return V5_GetReg(V5_REG_JPEG_VIDEO_WC_0, BIT_32_HTOL);
}

UINT32 V5B_JpegGetHuffmanWC(void)
{
	return V5_GetReg(V5_REG_JPEG_HUFF_WC_0, BIT_32_HTOL);
}

void V5B_JpegSetImgSize(TSize size)
{
	V5B_JpegSetImageWidth(size.cx);
	V5B_JpegSetImageHeight(size.cy);
}

void V5B_JpegGetImgSize(TSize* size)
{
	size->cx = V5B_JpegGetImageWidth();
	size->cy = V5B_JpegGetImageHeight();
}

void V5B_JpegSetImageWidth(UINT16 val)
{
	V5_SetReg(V5_REG_JPEG_WIDTH_H, val, BIT_16_HTOL);
}

UINT16 V5B_JpegGetImageWidth(void)
{
	return (UINT16)V5_GetReg(V5_REG_JPEG_WIDTH_H, BIT_16_HTOL);
}

void V5B_JpegSetImageHeight(UINT16 val)
{
	V5_SetReg(V5_REG_JPEG_HEIGHT_H, val, BIT_16_HTOL);
}

UINT16 V5B_JpegGetImageHeight(void)
{
	return (UINT16)V5_GetReg(V5_REG_JPEG_HEIGHT_H, BIT_16_HTOL);
}

void V5B_JpegSetBrcParm(UINT16 val)
{
	V5_SetReg(V5_REG_JPEG_B_H, val, BIT_16_HTOL);
}

UINT16 V5B_JpegGetBrcParm(void)
{
	return (UINT16)V5_GetReg(V5_REG_JPEG_B_H, BIT_16_HTOL);
}

UINT16 V5B_JpegGetFrameCnt(void)
{
	return (UINT16)V5_GetReg(V5_REG_JPEG_FRAME_CNT_0, BIT_16_LTOH);
}

void V5B_JpegSetDctData(UINT8 val)
{
	V5_SetReg(V5_REG_JPEG_DCT_DATA, val, BIT_8);
}

void V5B_JpegSetQtable(UINT8 val)
{
	V5_SetReg(V5_REG_JPEG_QUAN_T, val, BIT_8);
}

void V5B_JpegSetHtable(UINT8 val)
{
	V5_SetReg(V5_REG_JPEG_HUFF_T, val, BIT_8);
}

void V5B_JpegSetComponentNum(UINT8 val)
{
	V5_SetReg(V5_REG_JPEG_NUM_COMP_QUANT, val, BIT_8);
}

UINT8 V5B_JpegGetComponentNum(void)
{
	UINT8 x;

	x = (UINT8)V5_GetReg(V5_REG_JPEG_NUM_COMP_QUANT, BIT_8);

	return x;
}

void V5B_JpegSetComponentParm(UINT8 sel, UINT16 val)
{
	switch(sel)
	{
	case 0:
		V5_SetReg(V5_REG_JPEG_COMP0_P0, val, BIT_16_HTOL);
		break;
	case 1:
		V5_SetReg(V5_REG_JPEG_COMP1_P0, val, BIT_16_HTOL);
		break;
	case 2:
		V5_SetReg(V5_REG_JPEG_COMP2_P0, val, BIT_16_HTOL);
		break;
	case 3:
		V5_SetReg(V5_REG_JPEG_COMP3_P0, val, BIT_16_HTOL);
		break;
	default:
		break;
	}
}

UINT16 V5B_JpegGetComponentParm(UINT8 sel)
{
	switch(sel)
	{
	case 0:
		return (UINT16)V5_GetReg(V5_REG_JPEG_COMP0_P0, BIT_16_HTOL);
	case 1:
		return (UINT16)V5_GetReg(V5_REG_JPEG_COMP1_P0, BIT_16_HTOL);
	case 2:
		return (UINT16)V5_GetReg(V5_REG_JPEG_COMP2_P0, BIT_16_HTOL);
	case 3:
		return (UINT16)V5_GetReg(V5_REG_JPEG_COMP3_P0, BIT_16_HTOL);
	default:
		return 0;
	}
}

void V5B_JpegSetBlockNum(UINT16 val)
{
	V5_SetReg(V5_REG_JPEG_NUM_BLK_H, val, BIT_16_HTOL);
}

UINT16 V5B_JpegGetBlockNum(void)
{
	return (UINT16)V5_GetReg(V5_REG_JPEG_NUM_BLK_H, BIT_16_HTOL);
}

void V5B_JpegSetImageParm(UINT8 count, UINT8 *val)
{
	UINT8 i;

	if(count > 15)
		count = 15;

	for(i = 0; i < count; i++)
	{
		V5_SetReg((V5_REG_JPEG_IMGPARA_0+i), val[i], BIT_8);
	}
}

void V5B_JpegGetImageParm(UINT8 count, UINT8 *val)
{
	UINT8 i;

	if(count > 15)
		count = 15;

	for(i = 0; i < count; i++)
	{
		*(val+i) = (UINT8)V5_GetReg(V5_REG_JPEG_IMGPARA_0+i, BIT_8);
	}
}

void V5B_JpegSetHeaderType(UINT8 val)
{
	V5_SetReg(V5_REG_JPEG_HEAD_SEL, val, BIT_8);
}

UINT8 V5B_JpegGetHeaderType(void)
{
	UINT8 x;

	x = (UINT8)V5_GetReg(V5_REG_JPEG_HEAD_SEL, BIT_8);

	return x;
}

UINT32 V5B_JpegGetJWC(void)
{
	return V5_GetReg(V5_REG_JPEG_JPEG_WC_0, BIT_32_HTOL);
}

UINT8 V5B_JpegGetPixelCount(void)
{
	return (UINT8)V5_GetReg(V5_REG_JPEG_PIXEL_COUNT, BIT_8);
}

UINT16	V5B_JpegGetBlockCount(void)
{
	return (UINT16)V5_GetReg(V5_REG_JPEG_BLOCKCOUNT_H, BIT_16_HTOL);
}

void V5B_JpegSetDecByp(UINT8 val)
{
	V5_SetReg(V5_REG_JPEG_DEC_BYPASS, val, BIT_8);
}

UINT8 V5B_JpegGetDecByp(void)
{
	UINT8 x;

	x = (UINT8)V5_GetReg(V5_REG_JPEG_DEC_BYPASS, BIT_8);

	return x;
}

UINT8 V5B_JpegDecBypEnable(void)
{
	UINT8 x;

	x = V5B_JpegGetDecByp();

	V5B_JpegSetDecByp((UINT8)(x|BIT4));

	return x;
}

UINT8 V5B_JpegDecBypDisable(void)
{
	UINT8 x;

	x = V5B_JpegGetDecByp();

	V5B_JpegSetDecByp((UINT8)(x&(~BIT4)));

	return x;
}

void V5B_JpegSetDecBypStep(UINT8 val)
{
	UINT8 x;

	x = V5B_JpegGetDecByp();

	V5B_JpegSetDecByp((x&(~0xf))|(val&0xf));
}

UINT8 V5B_JpegGetDecBypSetp(void)
{
	UINT8 x;

	x = V5B_JpegGetDecByp();

	return (x&0xf);
}

void V5B_JpegEncErrEnable(void)
{
	V5_SetReg(V5_REG_JPEG_ENC_ERR_EN, 0x1, BIT_8);
}

void V5B_JpegEncErrDisable(void)
{
	V5_SetReg(V5_REG_JPEG_ENC_ERR_EN, 0x0, BIT_8);
}

void V5B_JpegSetHblank(UINT16 val)
{
	V5_SetReg(V5_REG_JPEG_HBLANK_H, val, BIT_16_HTOL);
}

UINT16 V5B_JpegGetHblank(void)
{
	return (UINT16)V5_GetReg(V5_REG_JPEG_HBLANK_H, BIT_16_HTOL);
}

void V5B_JpegSetEnhThdR1(UINT8 val)
{
	V5_SetReg(V5_REG_JPEG_ENH_THD_R1, val, BIT_8);
}

UINT8 V5B_JpegGetEnhThdR1(void)
{
	UINT8 x;
	
	x = (UINT8)V5_GetReg(V5_REG_JPEG_ENH_THD_R1, BIT_8);

	return x;
}

void V5B_JpegSetEnhThdR2(UINT8 val)
{
	V5_SetReg(V5_REG_JPEG_ENH_THD_R2, val, BIT_8);
}

UINT8 V5B_JpegGetEnhThdR2(void)
{
	UINT8 x;
	
	x = (UINT8)V5_GetReg(V5_REG_JPEG_ENH_THD_R2, BIT_8);

	return x;
}

void V5B_JpegSetEnhThdL1(UINT8 val)
{
	V5_SetReg(V5_REG_JPEG_ENH_THD_L1, val, BIT_8);
}

UINT8 V5B_JpegGetEnhThdL1(void)
{
	UINT8 x;
	
	x = (UINT8)V5_GetReg(V5_REG_JPEG_ENH_THD_L1, BIT_8);

	return x;
}

void V5B_JpegSetEnhThdL2(UINT8 val)
{
	V5_SetReg(V5_REG_JPEG_ENH_THD_L2, val, BIT_8);
}

UINT8 V5B_JpegGetEnhThdL2(void)
{
	UINT8 x;
	
	x = (UINT8)V5_GetReg(V5_REG_JPEG_ENH_THD_L2, BIT_8);

	return x;
}

void V5B_JpegSetEnhShfR1(UINT8 val)
{
	V5_SetReg(V5_REG_JPEG_ENH_SHF_NBIT_R1, val, BIT_8);
}

UINT8 V5B_JpegGetEnhShfR1(void)
{
	UINT8 x;
	
	x = (UINT8)V5_GetReg(V5_REG_JPEG_ENH_SHF_NBIT_R1, BIT_8);

	return x;
}

void V5B_JpegSetEnhShfR2(UINT8 val)
{
	V5_SetReg(V5_REG_JPEG_ENH_SHF_NBIT_R2, val, BIT_8);
}

UINT8 V5B_JpegGetEnhShfR2(void)
{
	UINT8 x;
	
	x = (UINT8)V5_GetReg(V5_REG_JPEG_ENH_SHF_NBIT_R2, BIT_8);

	return x;
}

void V5B_JpegSetEnhShfL1(UINT8 val)
{
	V5_SetReg(V5_REG_JPEG_ENH_SHF_NBIT_L1, val, BIT_8);
}

UINT8 V5B_JpegGetEnhShfL1(void)
{
	UINT8 x;
	
	x = (UINT8)V5_GetReg(V5_REG_JPEG_ENH_SHF_NBIT_L1, BIT_8);

	return x;
}

void V5B_JpegSetEnhShfL2(UINT8 val)
{
	V5_SetReg(V5_REG_JPEG_ENH_SHF_NBIT_L2, val, BIT_8);
}

UINT8 V5B_JpegGetEnhShfL2(void)
{
	UINT8 x;
	
	x = (UINT8)V5_GetReg(V5_REG_JPEG_ENH_SHF_NBIT_L2, BIT_8);

	return x;
}

void V5B_JpegSetRudeCompLevel(UINT8 index, UINT8 val)
{
	V5_SetReg(V5_REG_JPEG_RUDE_QF_LEVEL0_COMP + index, val, BIT_8);
}

UINT8 V5B_JpegGetRudeCompLevel(UINT8 index)
{
	UINT8 x;
	
	x = (UINT8)V5_GetReg(V5_REG_JPEG_RUDE_QF_LEVEL0_COMP + index, BIT_8);

	return x;
}

void V5B_JpegSetRudeBrc(UINT8 val)
{
	V5_SetReg(V5_REG_JPEG_RUDE_BRC, val, BIT_8);
}

UINT8 V5B_JpegGetRudeBrc(void)
{
	UINT8 x;
	
	x = (UINT8)V5_GetReg(V5_REG_JPEG_RUDE_BRC, BIT_8);

	return x;
}

void V5B_JpegRudeBrcEnable(void)
{
	UINT8 x;
	
	x = V5B_JpegGetRudeBrc();
	V5B_JpegSetRudeBrc((UINT8)(x | BIT7));
}

void V5B_JpegRudeBrcDisable(void)
{
	UINT8 x;
	
	x = V5B_JpegGetRudeBrc();
	V5B_JpegSetRudeBrc((UINT8)(x & (~BIT7)));
}

void V5B_JpegRudeBrcUvOnlyEnable(void)
{
	UINT8 x;
	
	x = V5B_JpegGetRudeBrc();
	V5B_JpegSetRudeBrc((UINT8)(x | BIT6));
}

void V5B_JpegRudeBrcUvOnlyDisable(void)
{
	UINT8 x;
	
	x = V5B_JpegGetRudeBrc();
	V5B_JpegSetRudeBrc((UINT8)(x & (~BIT6)));
}

void V5B_JpegSetRudeAc(UINT8 val)
{
	UINT8 x;
	
	x = V5B_JpegGetRudeBrc();
	x &= 0xC0;
	V5B_JpegSetRudeBrc(x | (val&0x3F));
}

UINT8 V5B_JpegGetRudeAc(void)
{
	UINT8 x;
	
	x = V5B_JpegGetRudeBrc();

	return x&0x3F;
}

void V5B_JpegSetRudeLevel(UINT8 val)
{
	V5_SetReg(V5_REG_JPEG_RUDE_LEVEL, val&0x3, BIT_8);
}

UINT8 V5B_JpegGetRudeLevel(void)
{
	UINT8 x;
	
	x = (UINT8)V5_GetReg(V5_REG_JPEG_RUDE_LEVEL, BIT_8);

	return x&0x3;
}

void V5B_JpegSetRudeThdWc(UINT32 val)
{
	V5_SetReg(V5_REG_JPEG_RUDE_THD_WC_0, val, BIT_32_HTOL);
}

UINT32 V5B_JpegGetRudeThdWc(void)
{
	return V5_GetReg(V5_REG_JPEG_RUDE_THD_WC_0, BIT_32_HTOL);
}

void V5B_JpegSetTimer(UINT32 val)
{
	V5_SetReg(V5_REG_JPEG_TIMER_VALUE_0, val, BIT_32_HTOL);
}

UINT32 V5B_JpegGetTimer(void)
{
	return V5_GetReg(V5_REG_JPEG_TIMER_VALUE_0, BIT_32_HTOL);
}



#endif //V578CMD_DISABLE
