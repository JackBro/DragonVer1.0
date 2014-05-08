#ifndef _TOOLDIR_INTERNAL_H_
#define _TOOLDIR_INTERNAL_H_

#ifdef _WINDLL
	#define TOOLDIR_DLL  __declspec(dllexport)
#endif

#pragma comment(lib,"toolchar.lib")

#include "tools/global.h"

//#include "toolchar/chartools.h"
#include "toolchar/stringtools.h"

#include "tooldir/dirtool.h"

#endif
