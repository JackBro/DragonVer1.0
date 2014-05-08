#ifndef _HEADERPAGE_INTERNAL_H_
#define _HEADERPAGE_INTERNAL_H_

#ifdef _WINDLL
	#define HEADERPAGE_DLL  __declspec(dllexport)
#endif


#pragma comment(lib, "toolfile.lib")

#include "stdafx.h"
#include "resource.h"

#include "toolfile/filebin.h"

#include "headerpage/dibdef.h"
#include "headerpage/dibbase.h"
#include "headerpage/dib.h"
#include "headerpage/headerpage.h"
#include "headerpage/LogSplash.h"
#include "headerpage/logwin.h"


#endif
