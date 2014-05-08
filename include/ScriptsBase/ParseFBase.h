#ifndef __SCRIPTSBASE_PARSEFBASE_H__
#define __SCRIPTSBASE_PARSEFBASE_H__


#ifndef SCRIPTSBASE_DLL
	#define SCRIPTSBASE_DLL _declspec(dllimport)
#endif

#include "toolfile/filebin.h"
#include "toolchar/charfifo.h"

class SCRIPTSBASE_DLL CParseFBase
{
public:
	CParseFBase(void);
	virtual ~CParseFBase(void);
};

#endif
