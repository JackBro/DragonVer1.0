#ifndef _CMODEL_ADD2_ALG_H_
#define _CMODEL_ADD2_ALG_H_


#ifndef CMALGGATES_DLL
	#define CMALGGATES_DLL  __declspec(dllimport)
#endif

#include "tools/algbase.h"
#include "toolbuf/linefifo.h"

class CMALGGATES_DLL CAdd2Alg : public CImageAlgBase
{
public:
	CAdd2Alg(void);
	virtual ~CAdd2Alg(void);

public:
	virtual int IsFrameDone(void);
	virtual void ResetState(void);
	virtual void SetInputCfg(int index, TStreamCfg *cfg);
	virtual int  GetOutputCfg(int index, TStreamCfg *cfg);

public:
	virtual int  Write(int index, char *buf, int len);
	virtual int  Read(int index, char *buf, int len);

public:
	void	SetClip(int val);
	int		GetClip(void);
	void	SetInUnit(int val);
	int		GetInUnit(void);
	void	SetOutUnit(int val);
	int		GetOutUnit(void);
	void	SetOpdata(int val);
	int		GetOpdata(void);

private:
	int		m_clip;
	int		m_width;
	int		m_height;
	int		m_fmt;
	int		m_InUnit;
	int		m_OutUnit;
	int		m_opdata;
	int		m_linewidth; /* total line width in byte is  m_linewidth * m_InUnit(or m_OutUnit) */
	CLineFifo	m_linebuf[2];
};

#endif
