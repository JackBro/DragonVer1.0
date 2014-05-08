#ifndef _CHANNEL_BUFFER_H_
#define _CHANNEL_BUFFER_H_


#ifndef TOOLBUF_DLL
	#define TOOLBUF_DLL  __declspec(dllimport)
#endif


#define DEFAULT_CHANNEL_SIZE	0x100

#include "srambuf.h"

class TOOLBUF_DLL CChannelBuf : public CSramBuf
{
public:
	CChannelBuf(int size = DEFAULT_CHANNEL_SIZE, int unit = 1);
	virtual ~CChannelBuf();

public:
	virtual int		Write(char *buf, int size);
	virtual int		Read(char *buf, int size);

public:
	void	SetSize(int size, int unit);
	void	SetSize(char *buf, int size, int unit);
	void	ClearPoint(void);

private:
	int		m_point;
};

#endif


