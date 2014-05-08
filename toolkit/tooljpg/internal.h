#ifndef _TOOLJPG_INTERNAL_H_
#define _TOOLJPG_INTERNAL_H_

#ifdef _WINDLL
	#define TOOLJPG_DLL  __declspec(dllexport)
#endif

#pragma comment(lib, "tools.lib")
#pragma comment(lib, "toolmath.lib")
#pragma comment(lib, "toolbit.lib")
#pragma comment(lib, "toolfile.lib")
#pragma comment(lib, "toolbuf.lib")
#pragma comment(lib, "toolchar.lib")


//#include "tools/global.h"

#include "toolchar/stringtools.h"

#include "toolmath/dct.h"


#include "tooljpg/jpegtable.h"
#include "tooljpg/jmmatrix.h"
#include "tooljpg/jpegbase.h"
#include "tooljpg/jpegparse.h"
#include "tooljpg/jpegdecbase.h"
#include "tooljpg/jpegencbase.h"
#include "tooljpg/jpegenc.h"
#include "tooljpg/jpegdec.h"
#include "tooljpg/jpeg.h"
#include "tooljpg/jpegf.h"

#endif
