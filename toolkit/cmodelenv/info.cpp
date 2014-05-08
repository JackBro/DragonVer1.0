#ifdef WIN32
#include <afxpriv.h>		// required for WM_SETMESSAGESTRING
#include "info.h"
#include "../toolfile/dbugtools.h"

static SYSTEMTIME gInfoTime;

void StartRecode(void)
{
	GetSystemTime(&gInfoTime);
}

void PrintRunTime(void)
{
	SYSTEMTIME time;
	int minute, sec, msec;
	CDbugTools tools;

	GetSystemTime(&time);
	minute = (time.wMinute + 60 - gInfoTime.wMinute) % 60;
	sec = (time.wSecond + 60 - gInfoTime.wSecond) % 60;
	msec = (time.wMilliseconds + 1000 - gInfoTime.wMilliseconds) % 1000;
	sec += minute;
	tools.PrintMsg("run time: %d.%d second\n", sec, msec);
}

#else
void StartRecode(void)
{
}

void PrintRunTime(void)
{
}

#endif
