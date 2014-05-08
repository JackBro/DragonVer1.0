#ifndef _TOOLBUF_INTERNAL_H_
#define _TOOLBUF_INTERNAL_H_

#ifdef _WINDLL
	#define TOOLBUF_DLL  __declspec(dllexport)
#endif

#pragma comment(lib, "tools.lib")


#include "toolbuf/bufferbase.h"
#include "toolbuf/srambuf.h"
#include "toolbuf/blockbuf.h"
#include "toolbuf/channelbuf.h"
#include "toolbuf/fifobuf.h"
#include "toolbuf/filobuf.h"
#include "toolbuf/framebuf.h"
#include "toolbuf/linebuf.h"
#include "toolbuf/recordbuf.h"
#include "toolbuf/spanbuf.h"
#include "toolbuf/vlfifobuf.h"
#include "toolbuf/linefifo.h"
#include "toolbuf/replinefifo.h"
#include "toolbuf/mempool.h"
#include "toolbuf/ufifobuf.h"
#include "toolbuf/yuvline.h"

#endif
