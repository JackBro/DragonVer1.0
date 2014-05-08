#ifndef _SPANBUF_H_
#define _SPANBUF_H_


#ifndef TOOLBUF_DLL
	#define TOOLBUF_DLL  __declspec(dllimport)
#endif


#include "srambuf.h"

class TOOLBUF_DLL CSpanBuf : public CSramBuf
{
public:
	CSpanBuf(int size = BUFFER_DEFAULT_SIZE, int unit = 1);
	virtual ~CSpanBuf();

public:
	virtual int		Read(char *buf, int size);
	virtual int		Write(char *buf, int size);

public:
	void	SetSize(int size = 0, int unit = 1);
	void	SetSize(char *buf, int size = 0, int unit = 1);

public:
	int		Write(char *buf, int size, int start, int span = 1, int skip = 0);
	int		Read(char *buf, int size, int start, int span = 1, int skip = 0);
	void	SetOption(int start, int span, int skip);
	int		ReadOne(int pos);
	int		WriteOne(int value, int pos);

private:
	int		m_span, m_skip, m_start;
};

#endif


