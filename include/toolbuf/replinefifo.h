#ifndef _REPEAT_LINEFIFO_BUFFER_H_
#define _REPEAT_LINEFIFO_BUFFER_H_


#ifndef TOOLBUF_DLL
	#define TOOLBUF_DLL  __declspec(dllimport)
#endif

enum REPLINEFIFOSTATEenum
{
	REPLINE_IDLE,
	REPLINE_WRITE,
	REPLINE_READ
};

#include "linefifo.h"

class TOOLBUF_DLL CRepLineFifo : public CLineFifo
{
public:
	CRepLineFifo(int width = BUFFER_DEFAULT_SIZE, int unit = 1);
	virtual ~CRepLineFifo();

public:
	virtual int		Write(char *buf, int size);
	virtual int		Read(char *buf, int width);

public:
	void	SetSize(int width = BUFFER_DEFAULT_SIZE, int unit = 1);
	void	SetSize(char *buf, int width = BUFFER_DEFAULT_SIZE, int unit = 1);

public:
	int		GetFreeSize(void);

private:
	int		m_flag;
};

#endif


