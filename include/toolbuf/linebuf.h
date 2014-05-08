#ifndef _LINE_BUFFER_H_
#define _LINE_BUFFER_H_


#ifndef TOOLBUF_DLL
	#define TOOLBUF_DLL  __declspec(dllimport)
#endif

#include "srambuf.h"

#define DEFAULT_LINEBUF_WIDTH	0x1000

class TOOLBUF_DLL CLineBuf : public CSramBuf
{
public:
	CLineBuf(int width = DEFAULT_LINEBUF_WIDTH, int edge = 4, int unit = 1);
	virtual ~CLineBuf();

public:
	virtual int		Write(char *buf, int size);
	virtual int		Read(char *buf, int size);

public:
	int		Read(int pos);
	int		Write(int value);

public:
	virtual void operator = (CLineBuf &a);

public:
	void	ClearPoint(void);
	int		IsFull(void);
	void	SetSize(int width = DEFAULT_LINEBUF_WIDTH, int edge = 4, int unit = 1);
	void	SetSize(char *buf, int width = DEFAULT_LINEBUF_WIDTH, int edge = 4, int unit = 1);
	
protected:
	int		m_wpoint;
	int		m_edge;
	int		m_width;
};

#endif


