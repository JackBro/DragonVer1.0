#ifndef _YUVLINE_BUFFER_H_
#define _YUVLINE_BUFFER_H_


#ifndef TOOLBUF_DLL
	#define TOOLBUF_DLL  __declspec(dllimport)
#endif


#include "linefifo.h"

//==================================

class TOOLBUF_DLL CYuv400Fifo : public CLineFifo
{
public:
	CYuv400Fifo(int width = BUFFER_DEFAULT_SIZE, int unit = 1);
	virtual ~CYuv400Fifo();

public:
	virtual int		Write(char *buf, int size);
	virtual int		Read(char *buf, int width);

public:
	void Write(int *yuv, int format);	//per 4 points
	void Read(int *yuv, int format);

	virtual void Write(int *yuv);	//per 4 points
	virtual void Read(int *yuv);

public:
	void	SetFormat(int format);
	int		GetFormat(void);

protected:
	int		m_format;
};

//==================================

class TOOLBUF_DLL CYuv444Fifo : public CYuv400Fifo
{
public:
	CYuv444Fifo(int width = BUFFER_DEFAULT_SIZE, int unit = 1);
	virtual ~CYuv444Fifo();

public:
	virtual int		Write(char *buf, int size);
	virtual int		Read(char *buf, int width);

public:
	void	SetSize(int width = BUFFER_DEFAULT_SIZE, int unit = 1);
	void	SetSize(char *buf, int width = BUFFER_DEFAULT_SIZE, int unit = 1);

public:
	virtual void Write(int *yuv);	//per 4 points
	virtual void Read(int *yuv);
};

//==================================

class TOOLBUF_DLL CYuv411Fifo : public CYuv400Fifo
{
public:
	CYuv411Fifo(int width = BUFFER_DEFAULT_SIZE, int unit = 1);
	virtual ~CYuv411Fifo();

public:
	virtual int		Write(char *buf, int size);
	virtual int		Read(char *buf, int width);

public:
	void	SetSize(int width = BUFFER_DEFAULT_SIZE, int unit = 1);
	void	SetSize(char *buf, int width = BUFFER_DEFAULT_SIZE, int unit = 1);

public:
	virtual void Write(int *yuv);	//per 4 points
	virtual void Read(int *yuv);
};

//==================================

class TOOLBUF_DLL CYuv422Fifo : public CYuv400Fifo
{
public:
	CYuv422Fifo(int width = BUFFER_DEFAULT_SIZE, int unit = 1);
	virtual ~CYuv422Fifo();

public:
	virtual int		Write(char *buf, int size);
	virtual int		Read(char *buf, int width);

public:
	void	SetSize(int width = BUFFER_DEFAULT_SIZE, int unit = 1);
	void	SetSize(char *buf, int width = BUFFER_DEFAULT_SIZE, int unit = 1);

public:
	virtual void Write(int *yuv);	//per 4 points
	virtual void Read(int *yuv);
};

//==================================

class TOOLBUF_DLL CYuv422_UFifo : public CYuv400Fifo
{
public:
	CYuv422_UFifo(int width = BUFFER_DEFAULT_SIZE, int unit = 1);
	virtual ~CYuv422_UFifo();

public:
	virtual int		Write(char *buf, int size);
	virtual int		Read(char *buf, int width);

public:
	void	SetSize(int width = BUFFER_DEFAULT_SIZE, int unit = 1);
	void	SetSize(char *buf, int width = BUFFER_DEFAULT_SIZE, int unit = 1);

public:
	virtual void Write(int *yuv);	//per 4 points
	virtual void Read(int *yuv);
};

//==================================

class TOOLBUF_DLL CYuv422_VFifo : public CYuv400Fifo
{
public:
	CYuv422_VFifo(int width = BUFFER_DEFAULT_SIZE, int unit = 1);
	virtual ~CYuv422_VFifo();

public:
	virtual int		Write(char *buf, int size);
	virtual int		Read(char *buf, int width);

public:
	void	SetSize(int width = BUFFER_DEFAULT_SIZE, int unit = 1);
	void	SetSize(char *buf, int width = BUFFER_DEFAULT_SIZE, int unit = 1);

public:
	virtual void Write(int *yuv);	//per 4 points
	virtual void Read(int *yuv);
};

//==================================

class TOOLBUF_DLL CYuv444_UFifo : public CYuv400Fifo
{
public:
	CYuv444_UFifo(int width = BUFFER_DEFAULT_SIZE, int unit = 1);
	virtual ~CYuv444_UFifo();

public:
	virtual int		Write(char *buf, int size);
	virtual int		Read(char *buf, int width);

public:
	virtual void Write(int *yuv);	//per 4 points
	virtual void Read(int *yuv);
};

//==================================

class TOOLBUF_DLL CYuv444_VFifo : public CYuv400Fifo
{
public:
	CYuv444_VFifo(int width = BUFFER_DEFAULT_SIZE, int unit = 1);
	virtual ~CYuv444_VFifo();

public:
	virtual int		Write(char *buf, int size);
	virtual int		Read(char *buf, int width);

public:
	virtual void Write(int *yuv);	//per 4 points
	virtual void Read(int *yuv);
};

//==================================

class TOOLBUF_DLL CYuv411_UFifo : public CYuv400Fifo
{
public:
	CYuv411_UFifo(int width = BUFFER_DEFAULT_SIZE, int unit = 1);
	virtual ~CYuv411_UFifo();

public:
	virtual int		Write(char *buf, int size);
	virtual int		Read(char *buf, int width);

public:
	void	SetSize(int width = BUFFER_DEFAULT_SIZE, int unit = 1);
	void	SetSize(char *buf, int width = BUFFER_DEFAULT_SIZE, int unit = 1);

public:
	virtual void Write(int *yuv);	//per 4 points
	virtual void Read(int *yuv);
};

//==================================

class TOOLBUF_DLL CYuv411_VFifo : public CYuv400Fifo
{
public:
	CYuv411_VFifo(int width = BUFFER_DEFAULT_SIZE, int unit = 1);
	virtual ~CYuv411_VFifo();

public:
	virtual int		Write(char *buf, int size);
	virtual int		Read(char *buf, int width);

public:
	void	SetSize(int width = BUFFER_DEFAULT_SIZE, int unit = 1);
	void	SetSize(char *buf, int width = BUFFER_DEFAULT_SIZE, int unit = 1);

public:
	virtual void Write(int *yuv);	//per 4 points
	virtual void Read(int *yuv);
};


#endif


