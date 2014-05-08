
#ifndef _TOOLS_MPEG4TABLE_H_
#define _TOOLS_MPEG4TABLE_H_


#ifndef TOOLMPEG4_DLL
	#define TOOLMPEG4_DLL  __declspec(dllimport)
#endif




#define ESCAPEVAL	7167

typedef struct tag_TMpeg4Event
{
	int last;
	int run;
	int level;
} TMpeg4Event, *PTMpeg4Event;

extern TOOLMPEG4_DLL const unsigned char gScanTable[3][64];
extern TOOLMPEG4_DLL const unsigned char gDefaultIntraMat[];
extern TOOLMPEG4_DLL const unsigned char gDefaultInterMat[];
extern TOOLMPEG4_DLL const unsigned char gIntraDcThresholdTab[];

extern TOOLMPEG4_DLL const TVLCtab gMCBPCIntraTab[];
extern TOOLMPEG4_DLL const TVLCtab gMCBPCInterTab[];
extern TOOLMPEG4_DLL const TVLCtab gCBPYTab[];
extern TOOLMPEG4_DLL const TVLCtab gMVTab0[];
extern TOOLMPEG4_DLL const TVLCtab gMVTab1[];
extern TOOLMPEG4_DLL const TVLCtab gMVTab2[];
extern TOOLMPEG4_DLL const TVLCtab gMVTab3[];
extern TOOLMPEG4_DLL const TVLCtab gDClumTab[8];

extern TOOLMPEG4_DLL const int gDquantTab[];
extern TOOLMPEG4_DLL const int gDefaultPredVal[];

extern TOOLMPEG4_DLL const TVLCtab gIntraACTab0[];
extern TOOLMPEG4_DLL const TVLCtab gIntraACTab1[];
extern TOOLMPEG4_DLL const TVLCtab gIntraACTab2[];

extern TOOLMPEG4_DLL const TVLCtab gInterACTab0[];
extern TOOLMPEG4_DLL const TVLCtab gInterACTab1[];
extern TOOLMPEG4_DLL const TVLCtab gInterACTab2[];

extern TOOLMPEG4_DLL const unsigned char gEscLevelMax[2][2][64];
extern TOOLMPEG4_DLL const unsigned char gEscRunMax[2][2][64];

#endif