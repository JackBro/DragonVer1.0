#ifndef _EMUHARDDISK_INTERNAL_H_
#define _EMUHARDDISK_INTERNAL_H_

#ifdef _WINDLL
	#define EMUHARDDISK_DLL  __declspec(dllexport)
#endif


#pragma comment(lib,"toolbuf.lib")
#pragma comment(lib,"toolfile.lib")

#include "toolfile/filebin.h"

#include "EmuHardDisk/EmuHardDisk.h"
#include "EmuHardDisk/EmuHardDiskF.h"


#endif
