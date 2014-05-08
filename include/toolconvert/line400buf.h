#ifndef _LINE400_BUFFER_H_
#define _LINE400_BUFFER_H_


#ifndef TOOLCONVERT_DLL
	#define TOOLCONVERT_DLL  __declspec(dllimport)
#endif

#include "toolbuf/linebuf.h"

class TOOLCONVERT_DLL CLine400Buf : public CLineBuf
{
public:
	CLine400Buf(int width = DEFAULT_LINEBUF_WIDTH, int edge = 3, int unit = 1);
	virtual ~CLine400Buf();

public:
	virtual int		Write(char *buf, int size);
	virtual void	Read(int pos, int *yuv);

public:
	virtual void operator = (CLine400Buf &a);

public:
	void	SetSize(int width = DEFAULT_LINEBUF_WIDTH, int edge = 3, int unit = 1);
	void	SetSize(char *buf, int width = DEFAULT_LINEBUF_WIDTH, int edge = 3, int unit = 1);
};

#endif


