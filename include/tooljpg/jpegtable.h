
#ifndef _TOOLS_JPEGTABLE_H_
#define _TOOLS_JPEGTABLE_H_


#ifndef TOOLJPG_DLL
	#define TOOLJPG_DLL  __declspec(dllimport)
#endif

#include "tools/global.h"


typedef struct tag_THuffValue
{
	int		value;
	int		length;
} THuffValue, *PTHuffValue;

extern TOOLJPG_DLL const UINT8 gJpegDefaultLumTbl[];
extern TOOLJPG_DLL const UINT8 gDefaultLumTbl[];
extern TOOLJPG_DLL const UINT8 gJpegDefaultChrTbl[];
extern TOOLJPG_DLL const UINT8 gDefaultChrTbl[];
extern TOOLJPG_DLL const UINT8 gZigZag[];
extern  TOOLJPG_DLL const unsigned char gAlterZigZag[2][64];
extern TOOLJPG_DLL const UINT8 gDefaultHuffmanTbl[];
extern TOOLJPG_DLL const UINT8 gJpegDefaultImgpara[]; 
extern TOOLJPG_DLL const int gDctMatrix[];
extern TOOLJPG_DLL const int gInvTable[];

extern TOOLJPG_DLL const int gDefHuffTblSize;
extern TOOLJPG_DLL const int gJpegImgparaSize;

#endif

