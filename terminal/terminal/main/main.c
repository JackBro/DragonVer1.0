

#include "main.h"


/* For test OS*/
void TestOS( void );
/* Test end */
	

int Main(int argc, char *argv[])
{
	HostInit();
	GetFreeSdramInfo();
	VmallocInit(gSdramInfo.start_addr, gSdramInfo.len);
	Led_Display(3);

	NandInitEnv();
	yaffs_StartUp();
	yaffs_mount("/flash");

#ifndef V938CMD_DISABLE
	V9_InitEnv();
#endif

#ifndef V578CMD_DISABLE
	V5_InitEnv();
#endif

#ifndef USBCMD_DISABLE
	InitUsbEnv();
	OpenUsb();
#endif

#ifndef NCSC

#ifdef V568_CHIPTEST
	TestChipProcess();
//	TestChip1TStart();
	beep(1);
	Delay(300);
	beep(0);
#else
	beep(1);
	Delay(100);
	beep(0);
	Led_Display(0);
#endif

#endif

	shell();
	/* For test OS*/
    //TestOS();
	/* Test end */
	argc = argv[0][0];
	return 0;
}

