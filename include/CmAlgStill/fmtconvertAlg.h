#ifndef _CMODEL_FORMATCONVERT_ALG_H_
#define _CMODEL_FORMATCONVERT_ALG_H_

#ifndef CMODELSTALG_DLL
	#define CMODELSTALG_DLL  __declspec(dllimport)
#endif

#include "tools/algbase.h"
#include "toolbuf/linefifo.h"

class CMODELSTALG_DLL CFmtConvertAlg : public CImageAlgBase
{
public:
	CFmtConvertAlg();
	virtual ~CFmtConvertAlg();

public:
	virtual int IsFrameDone(void);
	virtual void ResetState(void);
	virtual void SetInputCfg(int index, TStreamCfg *cfg);
	virtual int  GetOutputCfg(int index, TStreamCfg *cfg);

public:
	virtual int  Write(int index, char *buf, int len);
	virtual int  Read(int index, char *buf, int len);

public:
	void	SetOutFmt(int format);
	int		GetOutFmt(void);

private:
	int		m_linewidth;
	int		m_width;
	int		m_height;
	int		m_fmt;
	int		m_unit;
	CLineFifo	m_linebuf;
};

#endif
