#ifndef _TOOLBIT_INTERNAL_H_
#define _TOOLBIT_INTERNAL_H_

#ifdef _WINDLL
	#define TOOLBIT_DLL  __declspec(dllexport)
#endif

#pragma comment(lib, "toolbuf")


#include "toolbuf/bufferbase.h"
#include "toolbuf/srambuf.h"
#include "toolbuf/fifobuf.h"
#include "toolbit/bittools.h"
#include "toolbit/bitstream.h"
#include "toolbit/bitfifo.h"
#include "toolbit/jmbitfifo.h"

#endif
