#ifndef _CMODEL_AND1_ALG_H_
#define _CMODEL_AND1_ALG_H_


#ifndef CMALGGATES_DLL
	#define CMALGGATES_DLL  __declspec(dllimport)
#endif

#include "tools/algbase.h"
#include "toolbuf/linefifo.h"

class CMALGGATES_DLL CAnd1Alg : public CImageAlgBase
{
public:
	CAnd1Alg(void);
	virtual ~CAnd1Alg(void);

public:
	virtual int IsFrameDone(void);
	virtual void ResetState(void);
	virtual void SetInputCfg(int index, TStreamCfg *cfg);
	virtual int  GetOutputCfg(int index, TStreamCfg *cfg);

public:
	virtual int  Write(int index, char *buf, int len);
	virtual int  Read(int index, char *buf, int len);

private:
	int		m_width, m_linewidth;
	int		m_height;
	int		m_fmt;
	int		m_unit;
	CLineFifo	m_linebuf[3];
};

#endif
