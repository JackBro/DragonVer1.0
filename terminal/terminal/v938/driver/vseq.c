 
#include "driver938.h"

#ifndef	V938CMD_DISABLE

/* temperorily located here */
TConvInfo   gConvInfo = 
{
	0x0,     //water mark of Buffer 1
	0x0200,  //SEQ buffer 1 start address
	0x1000,  //SEQ buffer 2 start address
	0x0,     //command buffer 2 start address
	0x10000, //command buffer 2 start address
};

void V9_VseqOpen(void)
{
    /* Set convert and 8051 */
	V9B_ConvEnSelConv();
	V9B_ConvSetBuf1WMark(gConvInfo.buf1WMark);
	V9B_ConvSetBuf1Addr(gConvInfo.buf1Addr);
	V9B_ConvSetBuf2Addr(gConvInfo.buf2Addr);
	V9B_ConvSetBuf2CmdAddr(gConvInfo.buf2CmdAddr);
	V9B_ConvSetV8051Addr(gConvInfo.v8051Addr);

	/* Set play mode */
	V9B_ChanSetOpMod(OPMODE_HOST_WAVE_MIDI);

	V9B_CpmStartSeq();
}

void V9_VseqClose(void)
{
	UINT8 stopVal[8] = {0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff};

	V9B_ChanSetChnStop(stopVal);
 	V9B_CpmStopSeq();
}


#endif //V938CMD_DISABLE


