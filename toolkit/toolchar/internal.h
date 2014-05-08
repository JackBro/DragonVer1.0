#ifndef _TOOLCHAR_INTERNAL_H_
#define _TOOLCHAR_INTERNAL_H_

#ifdef _WINDLL
	#define TOOLCHAR_DLL  __declspec(dllexport)
#endif

#pragma comment(lib, "toolbuf.lib")
#pragma comment(lib, "tooltree.lib")

#include "toolchar/chartools.h"
#include "toolchar/stringtools.h"
#include "toolchar/charfifo.h"
#include "toolchar/charnode.h"

#endif
