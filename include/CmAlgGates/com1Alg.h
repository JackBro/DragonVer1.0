#ifndef _CMODEL_COM1_ALG_H_
#define _CMODEL_COM1_ALG_H_


#ifndef CMALGGATES_DLL
	#define CMALGGATES_DLL  __declspec(dllimport)
#endif

#include "tools/algbase.h"
#include "toolbuf/replinefifo.h"

class CMALGGATES_DLL CCom1Alg : public CImageAlgBase
{
public:
	CCom1Alg(void);
	virtual ~CCom1Alg(void);

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
	CRepLineFifo	m_linebuf;
};

#endif
