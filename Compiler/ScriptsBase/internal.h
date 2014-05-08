#ifndef _SCRIPTSBASE_INTERNAL_H_
#define _SCRIPTSBASE_INTERNAL_H_

#ifdef _WINDLL
	#define SCRIPTSBASE_DLL __declspec(dllexport)
#endif

#pragma comment(lib, "toolchar.lib")
#pragma comment(lib, "toolfile.lib")
#pragma comment(lib, "toolbuf.lib")
#pragma comment(lib, "tooltree.lib")


#include "ScriptsBase/ParseBase.h"


#endif
