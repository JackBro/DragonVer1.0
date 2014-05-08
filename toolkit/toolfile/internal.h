#ifndef _TOOLFILE_INTERNAL_H_
#define _TOOLFILE_INTERNAL_H_

#ifdef _WINDLL
	#define TOOLFILE_DLL  __declspec(dllexport)
#endif

#pragma comment(lib, "tools.lib")
#pragma comment(lib, "toolbuf.lib")


#include "toolfile/filebase.h"
#include "toolfile/filebin.h"
#include "toolfile/dbugtools.h"
#include "toolfile/fileinput.h"

#endif
