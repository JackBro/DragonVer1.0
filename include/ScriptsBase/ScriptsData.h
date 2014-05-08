#ifndef _SCRIPTSDATA_H_
#define _SCRIPTSDATA_H_


#ifndef SCRIPTSBASE_DLL
	#define SCRIPTSBASE_DLL _declspec(dllimport)
#endif



typedef struct tag_TScanFlag {
	char start[4];
	char stop[4];
} TScanFlag, *PTScanFlag;


extern SCRIPTSBASE_DLL const TScanFlag gC_Comments[];
extern SCRIPTSBASE_DLL const int gC_CommentsSize;
extern SCRIPTSBASE_DLL const char * gC_ReservedKeywordList[];
extern SCRIPTSBASE_DLL const int gC_ReservedKeywordListSize;


extern SCRIPTSBASE_DLL const TScanFlag gMap_Comments[];
extern SCRIPTSBASE_DLL const int gMap_CommentsSize;
extern SCRIPTSBASE_DLL const char * gMap_ReservedKeywordList[];
extern SCRIPTSBASE_DLL const int gMap_ReservedKeywordListSize;


#endif
