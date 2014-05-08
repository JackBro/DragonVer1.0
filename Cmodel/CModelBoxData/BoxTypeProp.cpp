
#include "internal.h"

char *CBoxTypeProp::GetFormatDesc(int fmt)
{
	int type, index;

	type = (fmt & CLASSMASK) >> CLASSSHIFT;
	index = fmt & ((1 << CLASSSHIFT) - 1);
	if(type < g_FmtGroupTotalCount)
	{
		if(index < g_fmtGroup[type].fmtcount)
			return g_fmtGroup[type].format[index];
	}
	return NULL;
}
