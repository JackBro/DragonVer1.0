#ifndef _SILLGUI_INTERNAL_H_
#define _SILLGUI_INTERNAL_H_

#ifdef _WINDLL
	#define SILLGUI_DLL __declspec(dllexport)
#endif

#pragma comment (lib, "gdiplus.lib")
#pragma comment(lib, "toolbuf.lib")
#pragma comment(lib, "toolfile.lib")
#pragma comment(lib, "toolconvert.lib")


#include "stdafx.h"
#include "resource.h"

#include "toolfile/filebin.h"

#include "toolconvert/raw2rgbf.h"
#include "toolconvert/rgb24convert.h"



#include "SillGui/ImageDiff.h"
#include "SillGui/LinkMap.h"
#include "SillGui/GLinkMap.h"
#include "SillGui/ImageBox.h"



#endif

