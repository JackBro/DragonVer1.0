#ifndef _RGB2YIQF_H_
#define _RGB2YIQF_H_


#ifndef TOOLCONVERT_DLL
	#define TOOLCONVERT_DLL  __declspec(dllimport)
#endif

#include "rgb2yiq.h"

class TOOLCONVERT_DLL CRgb2yiqF : public CRgb2yiq
{
public:
	CRgb2yiqF();
	virtual ~CRgb2yiqF();

public:
	virtual void rgb2yiq(char *src, char *dst, int width, int height, int unit = 1);

private:
	CDbugTools	m_dbug;
};



#endif
