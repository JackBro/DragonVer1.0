#ifndef _LINE_YUV422_BUFFER_H_
#define _LINE_YUV422_BUFFER_H_


#ifndef TOOLCONVERT_DLL
	#define TOOLCONVERT_DLL  __declspec(dllimport)
#endif

#include "toolbuf/linebuf.h"


class TOOLCONVERT_DLL CLine422Buf : public CLineBuf
{
public:
	CLine422Buf(int width = DEFAULT_LINEBUF_WIDTH, int edge = 4, int unit = 1);
	virtual ~CLine422Buf();

public:
	virtual int		Write(char *buf, int size);
	virtual void	Read(int pos, int *yuv);

public:
	virtual void operator = (CLine422Buf &a);

public:
	void	SetSize(int width = DEFAULT_LINEBUF_WIDTH, int edge = 4, int unit = 1);
	void	SetSize(char *buf, int width = DEFAULT_LINEBUF_WIDTH, int edge = 4, int unit = 1);
	void	SetFormat(int format);
	
private:
	int		m_format;
};

#endif


