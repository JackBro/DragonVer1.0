
#include "internal938.h"
#ifndef	V938CMD_DISABLE

TV9Context	gV9Context;

#define MAX_TRANSBUF938_SIZE  0x200000     /* 2M */
char                     gTransBuf938[MAX_TRANSBUF938_SIZE];
TV9TransBufContext       gV9TransBufContext;

void V9_InitContext(void)
{
	memset(&gV9Context, 0, sizeof(TV9Context));
	memset(&gV9TransBufContext, 0, sizeof(TV9TransBufContext));
	V9_CtrlInitContext();

	V9_CtrlSetInfo((PTCtrlInfo938)&gCtrlInfo938);

	gV9TransBufContext.bufSize    = 0x400000;
	gV9TransBufContext.pBuf       = (UINT8 *)gTransBuf938;
	gV9TransBufContext.curRdPos   = 0x0;
	gV9TransBufContext.realEndPos = 0x0;

	gV9Context.mode = 0xff;
}

void V9_Open(void)
{
	V9_CtrlStartClk(CTRL_PLL_WORK_MODE);
	V9_InitContext();
	V9_CtrlOpen();
	V9_VseqOpen();
	V9_VwsOpen();	
}

void V9_Close(void)
{
	V9_VwsClose();
	V9_VseqClose();
	V9_CtrlClose();
	
	gV9Context.mode = 0xff;
}

#endif //V938CMD_DISABLE

