#ifndef _CMODEL_YUV2YUV_ALG_H_
#define _CMODEL_YUV2YUV_ALG_H_


#ifndef CMODELSTALG_DLL
	#define CMODELSTALG_DLL  __declspec(dllimport)
#endif

#include "tools/algbase.h"
#include "toolbuf/yuvline.h"

class CMODELSTALG_DLL CYuv2yuvAlg : public CImageAlgBase
{
public:
	CYuv2yuvAlg(void);
	virtual ~CYuv2yuvAlg(void);

public:
	virtual int IsFrameDone(void);
	virtual void ResetState(void);
	virtual void SetInputCfg(int index, TStreamCfg *cfg);
	virtual int  GetOutputCfg(int index, TStreamCfg *cfg);

public:
	virtual int  Write(int index, char *buf, int len);
	virtual int  Read(int index, char *buf, int len);

public:
	int		GetOutFmt(void);
	void	SetOutFmt(int fmt);

private:
	int		m_outfmt;

private:
	int		m_width;
	int		m_height;
	int		m_inpfmt;
	int		m_unit;
	CYuv400Fifo	*m_pline[2];
	CYuv400Fifo	m_line400[2];
	CYuv422Fifo	m_line422[2];
	CYuv411Fifo	m_line411[2];
	CYuv444Fifo	m_line444[2];
};

#endif
