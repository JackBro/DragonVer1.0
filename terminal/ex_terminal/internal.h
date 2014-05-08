#ifndef _BASECLASS_INTERNAL_H_
#define _BASECLASS_INTERNAL_H_

#ifdef _WINDLL
	#define TERMINAL_DLL  __declspec(dllexport)
#endif

#pragma comment(lib,"trmyaffs.lib")
#pragma comment(lib,"trmsetupenv.lib")
#pragma comment(lib,"trmdriver.lib")
#pragma comment(lib,"trmtools")
#pragma comment(lib,"trmuserapi")
#pragma comment(lib,"trmusb")
#pragma comment(lib,"trmv568env")
#pragma comment(lib,"trm938highapi")
#pragma comment(lib,"trm568hapi")
#pragma comment(lib,"trmtestdemo")
#pragma comment(lib,"trmvstdio")
#pragma comment(lib,"trmfat")
#pragma comment(lib,"trmnandflash")
#pragma comment(lib,"trm938driver")
#pragma comment(lib,"trm938userapi")
#pragma comment(lib,"trmv938env")
#pragma comment(lib,"trm938testdemo")
#pragma comment(lib,"trm938tools")
#pragma comment(lib,"trmOS")
#pragma comment(lib,"trmmain")
#pragma comment(lib,"highapi")
#pragma comment(lib,"dbugtools")
#pragma comment(lib,"v568simpledemo")

#include "dll_terminal.h"


#endif
