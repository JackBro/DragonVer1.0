#ifndef _CMODEL_DELAYCELL1_ALG_H_
#define _CMODEL_DELAYCELL1_ALG_H_


#ifndef CMALGGATES_DLL
	#define CMALGGATES_DLL  __declspec(dllimport)
#endif

#include "tools/algbase.h"
#include "toolbuf/linefifo.h"
#include "toolchain/chaintools.h"

class CMALGGATES_DLL CDelay1Alg : public CImageAlgBase
{
public:
	CDelay1Alg(void);
	virtual ~CDelay1Alg(void);

public:
	virtual int IsFrameDone(void);
	virtual void ResetState(void);
	virtual void SetInputCfg(int index, TStreamCfg *cfg);
	virtual int  GetOutputCfg(int index, TStreamCfg *cfg);

public:
	virtual int  Write(int index, char *buf, int len);
	virtual int  Read(int index, char *buf, int len);

public:
	void	SetDelay(int delay);
	int		GetDelay(void);

private:
	int		m_width, m_linewidth;
	int		m_height;
	int		m_fmt;
	int		m_unit;
	int		m_delay, m_delaycnt;
	int		m_inline;
	CChainTool<CLineFifo>	m_line;
};

#endif
