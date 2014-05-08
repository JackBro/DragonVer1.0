#ifndef _TOOLPARSE_INTERNAL_H_
#define _TOOLPARSE_INTERNAL_H_

#ifdef _WINDLL
	#define TOOLPARSE_DLL  __declspec(dllexport)
#endif

#pragma comment(lib,"toolbuf.lib")
#pragma comment(lib,"toolfile.lib")
#pragma comment(lib,"toolchar.lib")
#pragma comment(lib,"tools.lib")

//#include "tools/global.h"
//#include "tools/baseitem.h"
//
//#include "toolbuf/BufferBase.h"
//
//#include "toolchar/chartools.h"
#include "toolchar/stringtools.h"
//
//#include "toolfile/filebase.h"
//#include "toolfile/filebin.h"
//#include "toolfile/dbugtools.h"

#include "toolparse/batchparse.h"

#endif
