
#ifndef _TOOLS_MPEG2TABLE_H_
#define _TOOLS_MPEG2TABLE_H_


#ifndef TOOLMPEG2I_DLL
	#define TOOLMPEG2I_DLL  __declspec(dllimport)
#endif

#include "tooljpg/jpegtable.h"

typedef struct tag_THuffEvent
{
	int value;
	int run;
	int level;
} THuffEvent, *PTHuffEvent;

typedef struct tag_TVLCtab
{
	int  val;
	char len;
} TVLCtab, *PTVLCtab;

extern TOOLMPEG2I_DLL const unsigned char gDefaultIntraQtable[];
extern TOOLMPEG2I_DLL const unsigned char gDefaultNonIntraQtable[];
extern TOOLMPEG2I_DLL const unsigned char gQuantiserScale[2][32];
extern TOOLMPEG2I_DLL const THuffValue gMbtypeTable[3][32];
extern TOOLMPEG2I_DLL const THuffValue gAddrIncTable[34];

extern TOOLMPEG2I_DLL const THuffValue gDClumtab[12];
extern TOOLMPEG2I_DLL const THuffValue gDCchromtab[12];
extern TOOLMPEG2I_DLL const THuffValue gIntraActbl1[2][40];
extern TOOLMPEG2I_DLL const THuffValue gIntraActbl2[30][5];
extern TOOLMPEG2I_DLL const THuffValue gIntraActbl1a[2][40];
extern TOOLMPEG2I_DLL const THuffValue gIntraActbl2a[30][5];

extern TOOLMPEG2I_DLL const int gIntraAcTblLen[];
extern TOOLMPEG2I_DLL const int gInterAcTblLen[];
extern TOOLMPEG2I_DLL const THuffEvent gIntraActbl[17][16];
extern TOOLMPEG2I_DLL const THuffEvent gInterActbl[17][16];

extern TOOLMPEG2I_DLL const TVLCtab gMVtable0[];
extern TOOLMPEG2I_DLL const TVLCtab gMVtable1[];
extern TOOLMPEG2I_DLL const TVLCtab gMVtable2[];
extern TOOLMPEG2I_DLL const TVLCtab gCBPtable0[];
extern TOOLMPEG2I_DLL const TVLCtab gCBPtable1[];
extern TOOLMPEG2I_DLL const TVLCtab gCBPtable2[];
extern TOOLMPEG2I_DLL const TVLCtab gCBPtable3[];

#endif

