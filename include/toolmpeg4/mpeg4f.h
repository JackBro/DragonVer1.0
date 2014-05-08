
#ifndef _TOOLS_MPEG4F_H_
#define _TOOLS_MPEG4F_H_



#ifndef TOOLMPEG4_DLL
	#define TOOLMPEG4_DLL  __declspec(dllimport)
#endif

#include "mpeg4.h"
#include "toolfile/dbugtools.h"
#include "toolmpeg4/mpeg4base.h"

class TOOLMPEG4_DLL CMpeg4F : public CMpeg4
{
public:
	virtual void Mpeg4Parse(char *src, PTMpeg4Option pinfo);

public:
	void	Mpeg4Blk(char *src, char *dest);

private:
	CDbugTools	m_dbug;
};

#endif
