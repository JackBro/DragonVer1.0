#ifndef _CMODEL_RGB2YUV_ALG_H_
#define _CMODEL_RGB2YUV_ALG_H_


#ifndef CMODELSTALG_DLL
	#define CMODELSTALG_DLL  __declspec(dllimport)
#endif

#include "tools/algbase.h"
#include "toolbuf/linefifo.h"

class CMODELSTALG_DLL CRgb2yuvAlg : public CImageAlgBase
{
public:
	CRgb2yuvAlg(void);
	virtual ~CRgb2yuvAlg(void);

public:
	virtual int IsFrameDone(void);
	virtual void ResetState(void);
	virtual void SetInputCfg(int index, TStreamCfg *cfg);
	virtual int  GetOutputCfg(int index, TStreamCfg *cfg);

public:
	virtual int  Write(int index, char *buf, int len);
	virtual int  Read(int index, char *buf, int len);

private:
	int		m_width;
	int		m_height;
	int		m_fmt;
	int		m_unit;
	CLineFifo	m_linebuf;
};

#endif
