#ifndef _TOOLAUDIODEV_INTERNAL_H_
#define _TOOLAUDIODEV_INTERNAL_H_

#ifdef _WINDLL
	#define TOOLAUDIODEV_DLL  __declspec(dllexport)
#endif


#pragma comment(lib, "tools.lib")
#pragma comment(lib, "toolwave.lib")
#pragma comment(lib, "toolfile.lib")
#pragma comment(lib, "toolbuf.lib")
//#include "tools/global.h"
//#include "tools/baseitem.h"


//#include "toolbuf/bufferbase.h"
//#include "toolbuf/srambuf.h"
//
//#include "toolfile/filebase.h"
//#include "toolfile/filebin.h"

#include "toolwave/wavebase.h"
//#include "toolwave/wave.h"
//#include "toolwave/wavewf.h"
//#include "toolwave/wavef.h"

#include "toolaudiodev/SoundOut.h"
#include "toolaudiodev/PlayAudio.h"

#endif
