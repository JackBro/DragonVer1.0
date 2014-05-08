
#include "internal.h"

#include "../driver/basefunc/basefunc.h"



#ifndef	V578CMD_DISABLE



TTestChipContext gTestChipContext =
{
	"/flash/15b_frame.bmp",//FrmFileName
	"/flash/dec.jpg",//DecFileName
	"/flash/decresult.bin",//DecResultFileName
	"/flash/golden.bmp",//DecCmpFileName
	"/flash/still.jpg",//StiFileName
	"/flash/thumb.bin",//ThumbFileName
	0xf8,
	{
		{2048, 1536},
		{2048, 1536},
		{1280, 960},
		{128, 160},
		{80, 60},
		{0, 0}
	}
};


PTTestChipContext TestChipGetContext(void)
{
	return &gTestChipContext;
}

void TestChipOpen(void)
{
	TSize imgSize;
	UINT8 index;

	V5_Open();
	index = V5_VispGetImageIndexByWidth(2048);
	if((index != 0xff))
	{
		V5_CameraOn(index);
		V5B_SifSetCaptureDelay(1);
		V5_VispGetSize(index, &imgSize);
		gTestChipContext.aProty.imgSize.cx = imgSize.cx;
		gTestChipContext.aProty.imgSize.cy = imgSize.cy;
		gTestChipContext.aProty.winSize.cx = imgSize.cx;
		gTestChipContext.aProty.winSize.cy = imgSize.cy;
	}
	else
		V5_CameraOn(0);
}

void TestChipCapture(void)
{
	TDataBuf dbuf;
	TJpegCapParm capparm;

	dbuf.pData = (UINT8*)gTransferBuf;
	dbuf.Pos = 0;
	dbuf.Len = MALLOC_MAX_SIZE;

	TestCap(&dbuf, &capparm);

	if(dbuf.Pos == 0)
		return;

	if(capparm.jlen != 0)
		SaveImageFile(gTestChipContext.StiFileName, (char*)(dbuf.pData+capparm.tlen), (int)capparm.jlen);
	if(capparm.tlen != 0)
		SaveImageFile(gTestChipContext.ThumbFileName, (char*)(dbuf.pData), (int)capparm.tlen);

}

void TestChipSetCapture(void)
{
	TSize capSize;

	capSize.cx = gTestChipContext.aProty.capSize.cx > gTestChipContext.aProty.imgSize.cx ? gTestChipContext.aProty.imgSize.cx : gTestChipContext.aProty.capSize.cx;
	capSize.cy = gTestChipContext.aProty.capSize.cy > gTestChipContext.aProty.imgSize.cy ? gTestChipContext.aProty.imgSize.cy : gTestChipContext.aProty.capSize.cy;

	V5_IppSetCapSizerEx2(gTestChipContext.aProty.imgSize, gTestChipContext.aProty.pt, gTestChipContext.aProty.winSize, capSize);

	V5_IppSetThumbSizerByWidth((UINT8)(gTestChipContext.aProty.thumbSize.cx));

	V5_JpegAutoSetImage();
	V5_MarbAutoSetMem();

}

void TestChipSetVGACall(void)
{
	UINT8 index, oldIndex;
	TSize imgSize;
	TSize capSize;

	oldIndex = V5_VispGetImageIndex();
	index = V5_VispGetImageIndexByWidth(640);
	if((index != 0xff) && (index != oldIndex))
	{
		V5_VispGetSize(index, &imgSize);
		capSize.cx = gTestChipContext.aProty.capSize.cx > imgSize.cx ? imgSize.cx : gTestChipContext.aProty.capSize.cx;
		capSize.cy = gTestChipContext.aProty.capSize.cy > imgSize.cy ? imgSize.cy : gTestChipContext.aProty.capSize.cy;
		V5_VispSetCapSizer(index, gTestChipContext.aProty.pt, imgSize, capSize);
		V5_VispSetDispSizer(index, gTestChipContext.aProty.pt, imgSize, gTestChipContext.aProty.dispSize);
		V5_IppSetThumbSizerByWidth((UINT8)(gTestChipContext.aProty.thumbSize.cx));
		gTestChipContext.aProty.imgSize.cx = imgSize.cx;
		gTestChipContext.aProty.imgSize.cy = imgSize.cy;
		gTestChipContext.aProty.winSize.cx = imgSize.cx;
		gTestChipContext.aProty.winSize.cy = imgSize.cy;
	}
}

void TestChipSetSXGACall(void)
{
	UINT8 index, oldIndex;
	TSize imgSize;
	TSize capSize;

	oldIndex = V5_VispGetImageIndex();
	index = V5_VispGetImageIndexByWidth(1280);
	if((index != 0xff) && (index != oldIndex))
	{
		V5_VispGetSize(index, &imgSize);
		capSize.cx = gTestChipContext.aProty.capSize.cx > imgSize.cx ? imgSize.cx : gTestChipContext.aProty.capSize.cx;
		capSize.cy = gTestChipContext.aProty.capSize.cy > imgSize.cy ? imgSize.cy : gTestChipContext.aProty.capSize.cy;
		V5_VispSetCapSizer(index, gTestChipContext.aProty.pt, imgSize, capSize);
		V5_VispSetDispSizer(index, gTestChipContext.aProty.pt, imgSize, gTestChipContext.aProty.dispSize);
		V5_IppSetThumbSizerByWidth((UINT8)(gTestChipContext.aProty.thumbSize.cx));
		gTestChipContext.aProty.imgSize.cx = imgSize.cx;
		gTestChipContext.aProty.imgSize.cy = imgSize.cy;
		gTestChipContext.aProty.winSize.cx = imgSize.cx;
		gTestChipContext.aProty.winSize.cy = imgSize.cy;
	}
}

void TestChipSetMaxSizeCall(void)
{
	UINT8 index, oldIndex;
	TSize imgSize;
	TSize capSize;

	oldIndex = V5_VispGetImageIndex();
//	index = V5_VispGetImageIndexByWidth(640);
	index = 0;
	if((index != 0xff) && (index != oldIndex))
	{
		V5_VispGetSize(index, &imgSize);
		capSize.cx = gTestChipContext.aProty.capSize.cx > imgSize.cx ? imgSize.cx : gTestChipContext.aProty.capSize.cx;
		capSize.cy = gTestChipContext.aProty.capSize.cy > imgSize.cy ? imgSize.cy : gTestChipContext.aProty.capSize.cy;
		V5_VispSetCapSizer(index, gTestChipContext.aProty.pt, imgSize, capSize);
		V5_VispSetDispSizer(index, gTestChipContext.aProty.pt, imgSize, gTestChipContext.aProty.dispSize);
		V5_IppSetThumbSizerByWidth((UINT8)(gTestChipContext.aProty.thumbSize.cx));
		gTestChipContext.aProty.imgSize.cx = imgSize.cx;
		gTestChipContext.aProty.imgSize.cy = imgSize.cy;
		gTestChipContext.aProty.winSize.cx = imgSize.cx;
		gTestChipContext.aProty.winSize.cy = imgSize.cy;
	}
}

void TestChipDecodeFrame(void)
{
	TDataBuf src;
	TDataBuf dst;

	TALayerProty proty;

	TJpegIndex info;
	UINT32 len;

	len = LoadImageFile(gTestChipContext.DecFileName, gTransferBuf, (MALLOC_MAX_SIZE>>1));
	if(len == 0)
		return;

	JpegParse((UINT8*)gTransferBuf, len, &info);

	if(info.YUVType == JPEG_UNKNOWN)
		return;

	proty.memsize = gTestChipContext.aProty.dispSize;
	proty.pos2panel = gTestChipContext.aProty.pt;
	proty.win.ltp = gTestChipContext.aProty.pt;
	proty.win.size = gTestChipContext.aProty.dispSize;

	V5_IppSetDispSizerEx2(info.JpgSize, gTestChipContext.aProty.pt, info.ImageSize, gTestChipContext.aProty.dispSize);
	V5_DispSetAmemSize(&proty);

	V5_JpegAutoSetImage();
	V5_MarbAutoSetMem();

	src.pData = (UINT8*)gTransferBuf;
	src.Len = len;
	src.Pos = 0;

	dst.pData = (UINT8*)( gTransferBuf+len );
	dst.Pos = 0;
	dst.Len = (MALLOC_MAX_SIZE>>1);

	V5_HJpegDecFrm(&src, &dst);

	if(dst.Pos == 0)
		return;

	SaveImageFile(gTestChipContext.DecResultFileName, (char*)dst.pData, (int)dst.Pos);

}

void TestChipDisplay(void)
{
	TDataBuf src;
	TDataBuf dst;

	TALayerProty proty;

	TJpegIndex info;
	UINT32 len;

	len = LoadImageFile(gTestChipContext.DecFileName, gTransferBuf, (MALLOC_MAX_SIZE>>1));
	if(len == 0)
		return;

	JpegParse((UINT8*)gTransferBuf, len, &info);

	if(info.YUVType == JPEG_UNKNOWN)
		return;

	proty.memsize = gTestChipContext.aProty.dispSize;
	proty.pos2panel = gTestChipContext.aProty.pt;
	proty.win.ltp = gTestChipContext.aProty.pt;
	proty.win.size = gTestChipContext.aProty.dispSize;

	V5_IppSetDispSizerEx2(info.JpgSize, gTestChipContext.aProty.pt, info.ImageSize, gTestChipContext.aProty.dispSize);
	V5_DispSetAmemSize(&proty);

	V5_JpegAutoSetImage();
	V5_MarbAutoSetMem();

	src.pData = (UINT8*)gTransferBuf;
	src.Len = len;
	src.Pos = 0;

	dst.pData = (UINT8*)( gTransferBuf+len );
	dst.Pos = 0;
	dst.Len = (MALLOC_MAX_SIZE>>1);

	V5_HJpegDispFrm(&src);

	//V5_HJpegDecodeFrame(&src, &dst);

	//if(dst.Pos == 0)
	//	return;

	//SaveImageFile(gTestChipContext.DecResultFileName, (char*)dst.pData, (int)dst.Pos);

}

void TestChipSetMode(UINT8 mode)
{
//	V5B_SifDisableSyncGen();
	V5_SetMode(mode);
	V5_JpegAutoSetImage();
	V5_MarbAutoSetMem();
//	V5B_SifEnableSyncGen();
}

void TestChipOverlay(void)
{
	V5B_LcdcSetB0OverKeyColor(gTestChipContext.KeyColor);
	V5B_LcdcLayerB0First();
	V5B_LcdcEnableOverlay();
	V5_DispSetB0format(BLAYER_RGB565);
	TestB0Frame();
	V5_DispUpdateB0Win(NULL);
	//V5B_LcdcDisableOverlay();
}

void TestChip1TStart(void)
{
	const UINT32 adr = 0x202c, val = 0x01;

	SetMemory(REG_INTMSK, BIT_EINT1, OR_OPCODE);
	WriteMemory(REG_I_ISPC, BIT_EINT1);
	SetMemory(REG_PCONG,  0xfff3,  AND_OPCODE);

	V5B_SetBusType(V5_BUS_SEPERATE);
	V5_SetReg(adr, val, BIT_8);
}

UINT8 TestChip1TChk(void)
{
	UINT32 adr = 0, val = 0;

	for(adr = 0x202c; adr <= 0x203d; adr++)
	{
		val = V5_GetReg(adr, BIT_8);
		if(((val != 0x01) && (adr == 0x202c)) || ((val != 0) && (adr > 0x202c)))
			return 0;
	}
	return 1;
}

void TestChipProcess(void)
{

	TestChipOpen();
	V5_Delay(500);
	TestChipSetMode(CAPTURESTILLTHUMB_MODE);
	V5_JpegDisAbrc();
	TestChipSetCapture();
	TestChipCapture();
	//V5_Delay(1000);
	TestChipSetMode(DECODEFRAME_MODE);
	TestChipOverlay();
	TestChipDecodeFrame();
	V5_Delay(1000);
	TestChipSetMode(PREVIEW_MODE);
	TestChipSetMaxSizeCall();
}

void TestChipPower(void)
{
	V5B_CpmModGlbReset();
	V5_SetReg(V5_REG_BIU_PLL_BP,0x01, BIT_8);
//	V5B_BiuPllBypassEnable();
	V5_SetReg(V5_REG_BIU_PLL_PD, 0x01, BIT_8);
//	V5B_BiuPllPowerDownEnable();
	V5B_MarbSetMemCtrl(0x70);	//power down 1-T
	V5B_CpmSetSenDataBusDir(0xff);
	V5_SetReg(V5_REG_CPM_CS_PC_H, 0x07, BIT_8);
	V5B_CpmSetGpioRenSwitch(0xff);
	V5_SetReg(0x1890, 0x01, BIT_8);	//bypass sel
	V5B_CpmSetModClkVal(0xffff);	//clock off
}

UINT8 TestChipRW(void)
{
	UINT32 val = 0xff, val1;

	V5_SetReg(V5_REG_BIU_BYPASS_ADDR_L, val, BIT_8);
	val1 = V5_GetReg(V5_REG_BIU_BYPASS_ADDR_L, BIT_8);
	if(val1 != val)
		return 0;
	V5_SetReg(V5_REG_IPP_UOFFSET, val, BIT_8);
	val1 = V5_GetReg(V5_REG_IPP_UOFFSET, BIT_8);
	if(val1 != val)
		return 0;

	return 1;
}

#endif //V578CMD_DISABLE
