#ifndef _LINE_BAYER_BUFFER_H_
#define _LINE_BAYER_BUFFER_H_


#ifndef TOOLCONVERT_DLL
	#define TOOLCONVERT_DLL  __declspec(dllimport)
#endif


#include "toolbuf/linebuf.h"

class TOOLCONVERT_DLL CLineBayerBuf : public CLineBuf
{
public:
	CLineBayerBuf(int width = DEFAULT_LINEBUF_WIDTH, int edge = 4, int unit = 1);
	virtual ~CLineBayerBuf();

public:
	int		Read(int pos);

public:
	virtual void operator = (CLineBayerBuf &a);

};

#endif


