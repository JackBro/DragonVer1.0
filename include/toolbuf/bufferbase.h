#ifndef _BUFFERBASE_H_
#define _BUFFERBASE_H_


#ifndef TOOLBUF_DLL
	#define TOOLBUF_DLL  __declspec(dllimport)
#endif


#include "tools/baseitem.h"

class TOOLBUF_DLL CMemBase : public CBaseItem
{
public:
	CMemBase(int size = 0, int unit = 1);
	virtual ~CMemBase();

public:
	int		GetSize(void);
	int		GetTotalSize(void);

public:
	virtual void	SetSize(int size = 0, int unit = 1);
	virtual void	SetSize(char *buf, int size = 0, int unit = 1);

public:
	virtual int		Read(char *buf, int size);
	virtual int		Write(char *buf, int size);

protected:
	void	release(void);

private:
	void	Clear(void);

protected:
	int		m_size;
	int		m_unit;
	int		m_totalsize;

public:
	char	*m_buf;

private:
	char	*m_MemPool;
};

//=================================================

class TOOLBUF_DLL CBufferBase : public CMemBase
{
public:
	CBufferBase(int size = BUFFER_DEFAULT_SIZE, int unit = 1);
	virtual ~CBufferBase();
};


#endif


