
#ifndef _REGLIST_H_
#define	_REGLIST_H_


#ifndef REGLIST_DLL
#define REGLIST_DLL _declspec(dllimport)
#endif


#include "toolparse/batchparse.h"

enum REGGROUPenum {
	V568_REGGROUP,
	ARM7_REGGROUP,
	USB9604_REGGROUP,
	V558_REGGROUP,
	V938_REGGROUP,
	V578_REGGROUP
};

typedef struct tag_TReglistGroup {
	TRegList *reglist;
	int		listcount;
} TReglistGroup, *PTReglistGroup;

extern REGLIST_DLL const TRegList gV568Reglist[];
extern REGLIST_DLL const int gV568ReglistCount;
extern REGLIST_DLL const TRegList gArm7Reglist[];
extern REGLIST_DLL const int gArm7ReglistCount;
extern REGLIST_DLL const TRegList gUsb9604Reglist[];
extern REGLIST_DLL const int gUsb9604ReglistCount;
extern REGLIST_DLL const TRegList g558Reglist[];
extern REGLIST_DLL const int g558ReglistCount;
extern REGLIST_DLL const TRegList gV938Reglist[];
extern REGLIST_DLL const int gV938ReglistCount;
extern REGLIST_DLL const TRegList gV578Reglist[];
extern REGLIST_DLL const int gV578ReglistCount;

extern REGLIST_DLL const TReglistGroup gRegGroup[];

#endif

