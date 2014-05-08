#ifndef _TYPEPROP_H_
#define _TYPEPROP_H_


#ifndef TOOLS_DLL
	#define TOOLS_DLL  __declspec(dllimport)
#endif


#include "global.h"


class TOOLS_DLL CTypeProp
{
public:
	int		getVwc(int width, int height, int format, int unit = 1);
	int		getLinewidth(int width, int format, int unit = 1);
	char * GetFormatDesc(int fmt);

};

#endif


