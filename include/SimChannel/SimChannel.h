#ifndef _SIMCHANNEL_H_
#define _SIMCHANNEL_H_

#pragma once

#include "../../toolkit/toolbuf/recordbuf.h"

enum SIMSTATEenum
{
	SIMCMD_INITED,
	SIMCMD_NOTINITED,
	SIMCMD_FAILED
};

class CSimChannel : public CBaseChannel
{
public:
	CSimChannel(void);
	virtual ~CSimChannel(void);

public:
	virtual void	Open(void);
	virtual int		Write(char *buf, int len);
	virtual int		Read(char *buf, int len);
	virtual int		IsOpened(void);
	virtual void	Close(void);

public:
	int		hostWriteData(char *buf, int len);
	int		hostReadData(char *buf, int len);

protected:

private:
	int		m_status;
	CFifoBuf	m_pc;
	CFifoBuf	m_host;

	HANDLE  m_hCmodel;
};

extern CSimChannel *gpChannel;

#endif

