#ifndef _SIMREGSRAM_H_
#define _SIMREGSRAM_H_


#ifndef CHANNELSIMREGSRAM_DLL
	#define CHANNELSIMREGSRAM_DLL  __declspec(dllimport)
#endif

#include "tools/baseitem.h"
#include "toolbuf/fifobuf.h"

enum SIMREGSRAMSTATEenum
{
	SIMREG_INITED,
	SIMREG_NOTINITED,
	SIMREG_FAILED
};

class CHANNELSIMREGSRAM_DLL CSimRegSram : public CBaseChannel
{
public:
	CSimRegSram(void);
	virtual ~CSimRegSram(void);

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
};

extern CHANNELSIMREGSRAM_DLL CSimRegSram *gpRegSram;

#endif

