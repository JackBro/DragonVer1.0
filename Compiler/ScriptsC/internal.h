#ifndef _SCRIPTSC_INTERNAL_H_
#define _SCRIPTSC_INTERNAL_H_

#ifdef _WINDLL
	#define SCRIPTSC_DLL __declspec(dllexport)
#endif


#pragma comment(lib, "tools.lib")
#pragma comment(lib, "toolchar.lib")
#pragma comment(lib, "toolfile.lib")
#pragma comment(lib, "tooltree.lib")
#pragma comment(lib, "toolbuf.lib")
#pragma comment(lib, "ScriptsBase.lib")


#include "assert.h"
#include <sys\stat.h>


#include "ScriptsC/CParser.h"
#include "ScriptsC/CScanner.h"



#endif

