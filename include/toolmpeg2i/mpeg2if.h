
#ifndef _TOOLS_MPEG2IF_H_
#define _TOOLS_MPEG2IF_H_


#ifndef TOOLMPEG2I_DLL
	#define TOOLMPEG2I_DLL  __declspec(dllimport)
#endif

#include "mpeg2i.h"
#include "toolfile/dbugtools.h"
#include "toolmpeg2i/mpeg2base.h"


class TOOLMPEG2I_DLL CMpeg2IF : public CMpeg2I
{
public:
	virtual void Mpeg2Parse(char *src, PTMpeg2Option pinfo);

public:
	void	Mpeg2Blk(char *src, char *dest);

private:
	CDbugTools	m_dbug;
};

#endif

