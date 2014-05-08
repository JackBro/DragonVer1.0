#ifndef _FIFO_BUFFER_H_
#define _FIFO_BUFFER_H_


#ifndef TOOLBUF_DLL
	#define TOOLBUF_DLL  __declspec(dllimport)
#endif


#include "srambuf.h"

class TOOLBUF_DLL CFifoBuf : public CSramBuf
{
public:
	CFifoBuf(int size = BUFFER_DEFAULT_SIZE, int unit = 1);
	virtual ~CFifoBuf();

public:
	virtual int		Write(char *buf, int size);
	virtual int		Read(char *buf, int size);

public:
	void	SetSize(int size = BUFFER_DEFAULT_SIZE, int unit = 1);
	void	SetSize(char *buf, int size = BUFFER_DEFAULT_SIZE, int unit = 1);
	int		IsFifoEmpty(void);

public:
	void	ClearPoint(void);
	void	SetReadPoint(int x);
	void	SetWritePoint(int x);
	void	ValidPoint(void);
	int		GetReadPoint(void);
	int		GetWritePoint(void);
	int		GetDataSize(void);
	int		GetFreeSize(void);

public:
	void	PushState(void);
	void	PopState(void);

private:
	int		m_valid;
	int		m_rpoint;
	int		m_wpoint;

private:
	int		m_tempvalid;
	int		m_temprp;
	int		m_tempwp;
};

#endif


