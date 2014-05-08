#ifndef _SRAMBUFFER_H_
#define _SRAMBUFFER_H_


#ifndef TOOLBUF_DLL
	#define TOOLBUF_DLL  __declspec(dllimport)
#endif

#include "bufferbase.h"

class TOOLBUF_DLL CSramBuf : public CBufferBase
{
public:
	CSramBuf(int size = BUFFER_DEFAULT_SIZE, int unit = 1);

public:
	virtual int		Read(char *buf, int size);
	virtual int		Write(char *buf, int size);

public:
	void	SetSize(int size = 0, int unit = 1);
	void	SetSize(char *buf, int size = 0, int unit = 1);

public:
	int		Write(char *buf, int size, int start, int end = -1);
	int		Read(char *buf, int size, int start, int end = -1);
	void	SetOption(int start, int end = 0);

private:
	int m_start;
	int m_end;
};

#endif


