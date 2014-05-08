
#include "internal.h"
#ifndef	V578CMD_DISABLE

#ifdef _DEBUG_ISP_AE
	FILE *fp;
#endif

void TestVisp(void)
{
	V5_VispInitContext();
	V5_VispSetSensorInfo();
	V5_VispOpen();
}

/*********************************************/

void TestSetThumb(UINT16 capWidth, UINT16 capHeight, UINT8 width, UINT8 *thumbWidth, UINT8 *thumbHeight, UINT8 * thumbRatio)
{
	UINT16 tempY;
	UINT32 temp;

	if(width > capWidth)
		width = (UINT8)capWidth;
	if(width%2)
		width--;
	*thumbRatio = (UINT8)(capWidth / width);
	tempY = capHeight /(*thumbRatio);
	if(tempY%2)
		tempY--;
	
	temp = (UINT32)(width * tempY);
	if(temp > 4500 )
	{	
		temp = 4500 * width / tempY;
		width = (UINT8)vsqrt(temp);
		tempY = (UINT16)(width * 4500 / temp);
		if(width%2)
			width--;
		if(tempY%2)
			tempY--;
	}
	
	*thumbWidth = (UINT8)width;
	*thumbHeight = (UINT8)tempY;
	*thumbRatio = (UINT8)capWidth/width;
}

void TestComputerParm(UINT16 srcWidth, UINT16 srcHeight, UINT16 width, UINT16 *dstWidth, UINT16 *dstHeight, UINT8 *intNum, UINT16 *decNum)
{
	UINT16 tempY;
	UINT32 temp;

	/* if width > src.cx ,image is zoom in 
	if(width > src.cx)
		width = src.cx;
	*/
	if(width == 0)
		return;
	//temp = (srcWidth % width)<<10/width;
	temp = ((srcWidth % width)<<10)/width;
	*intNum = (UINT8)(srcWidth/width);
	*decNum = (UINT16)(temp);
    
	temp = srcHeight * width;
	tempY = (UINT16)(temp/srcWidth);
	if(width%2)
		width--;
	if(tempY%2)
		tempY--;
	*dstWidth = width;
	*dstHeight = tempY;
}

void AdjustAe(UINT32 et, UINT8 ygain, UINT8 env)
{
	TSize size;
	int i;

	size.cx = 640;
	size.cy = 480;
	et = et;
	env = env;
	ygain = ygain;

	i = TestTime(TestAeSim);
	tmPrintf("time is %ld", i);
	beep(1);
	V5_Delay(100);
	beep(0);
	
	/*if(V5_VispGetCurrentSize(&size) == FAILED)
		return;

	if(gVispContext.pinfo->snrSetETCall)
		gVispContext.pinfo->snrSetETCall(0, et, size);*/
	//SensorSetExposeTime(env, et, size, 6000000);
	//V5B_IspEnableGlbGain();
	//V5B_IspSetGlbGainDelay(2);
	//V5B_IspSetGlbGain(ygain);
}

UINT8 TestAeInterrupt(void)
{
	UINT16 flag;
	TCtrlContext *pCtrl;

	pCtrl = V5_CtrlGetContext();

	flag = V5B_CpmGetIntFlagSec(INT_ISP);
	
	pCtrl->intisr[1](flag);
	_ISR_VispIspIntHandle(flag);
	/*if(flag & (0x1 << (INT_ISP_Y_MEAN % 0x8)))
	{
		_ISR_VispAutoFunInt();
		return TRUE;
	}*/
	return TRUE;

}

void TestAutoExpose(void)
{
	int count = 10000;

	V5_VispEnableAeQuery();
	
#ifdef _DEBUG_ISP_AE
	fp = fopen("ae_debug.txt","wb");
#endif
	while(count)
	{
		TestAeInterrupt();
		count--;
		//V5_Delay(100);
	}
#ifdef _DEBUG_ISP_AE
	fclose(fp);
#endif
}


void TestAutoDetectEnv(void)
{
	int count = 20;

	V5_CtrlIntDisable(INT_ISP);
	V5_VispJudgeEnv();
	while(count)
	{
		TestAeInterrupt();
		count--;
		V5_Delay(100);
	}
}

void TestAeSim(void)
{
	PTAeAdjust ae;
	PTVispContext p;
	UINT32 et;
	UINT8 ret;
	UINT8 gstep;
	
	p = V5_VispGetContext();
	ae = (&p->ae);
	ae->snrclock = 12000;
	ae->snrclock *= 1000;
	ae->ymean = 0xee;
	ae->gtarget = p->pinfo->aeparm.gtarget;	//hx:to be define,according to the speed value?? adjust speed by speed value??
	gstep = 0;
	ae->et = 0x258;
	ae->gain = 0x4a;
	switch(gstep)
	{
		case 0:
			ae->gstep = 0x40;
			break;
		case 1:
			ae->gstep = 0x20;
			break;
		case 2:
			ae->gstep = 0x10;
			break;
		case 3:
			ae->gstep = 0x8;
			break;
		default:
			break;
	 }

	ret = VComputeEt(ae);
	
	if(ret == UN_ET_GAIN)
		return;

	if(ret == GAIN_ONLY)
	{
		ae->gain = ae->gtarget;
		return;
	}
	
	et = (UINT32)(ae->snrclock/ae->freq)*ae->newet;
	
	ae->et = ae->newet;
	ae->gain = ae->gtarget;
}

int TestTime(PTestCall pcall)
{
	int i;
	int mclk;

	mclk = (int)V5_CtrlGetMclk();
	WriteMemory(REG_WTCON, ((mclk/1000 - 1) << 8) | (2 << 3));
	WriteMemory(REG_WTDAT, 0xffff);
	WriteMemory(REG_WTCNT, 0xffff);
	WriteMemory(REG_WTCON, ((mclk/1000 - 1) << 8) | (2 << 3) | (1 << 5));

	for(i = TEST_TIME_COUNT; i > 0; i--)
        pcall();

	WriteMemory(REG_WTCON, ((mclk/1000 - 1) << 8) | (2 << 3));
	i = 0xffff - ReadMemory(REG_WTCNT);
	i *= 64;

	return i;
}

void V5_VispTestCaptureSXGA(void)
{
	TSize winSize, dispSize;
	TPoint pt;
	UINT8 index = 1;

	V5_VispDisableAe();
	V5_VispSetCurEt((UINT32)2*ET_ONE_STEP);
	//V5B_SifSetCaptureDelay(gVispContext.pinfo->aeparm.gdelay+1);
	index = V5_VispGetImageIndexByWidth(1280);
	V5_VispGetSize(index, &winSize);
	V5B_IppGetWinPoint(&pt);
	V5B_IppGetDispSize(&dispSize);
	V5_VispSetDispSizer(index, pt, winSize, dispSize);
	V5_VispSetCapSizer(index, pt, winSize, winSize);
}

void V5_VispTestCapture(void)
{
	TSize winSize, dispSize;
	TPoint pt;
	UINT8 index = 1;

	V5_VispDisableAe();
	V5_VispSetCurEt((UINT32)2*ET_ONE_STEP);
	index = V5_VispGetImageIndex();
	V5_VispGetSize(index, &winSize);
	V5B_IppGetWinPoint(&pt);
	V5B_IppGetDispSize(&dispSize);
	V5_VispSetDispSizer(index, pt, winSize, dispSize);
	V5_VispSetCapSizer(index, pt, winSize, winSize);
}

#endif //V578CMD_DISABLE
