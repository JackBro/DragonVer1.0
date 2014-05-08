#ifndef _CMODEL_YUVMERGE_ALG_H_
#define _CMODEL_YUVMERGE_ALG_H_


#ifndef CMODELSTALG_DLL
	#define CMODELSTALG_DLL  __declspec(dllimport)
#endif

#include "tools/algbase.h"
#include "toolbuf/yuvline.h"

class CMODELSTALG_DLL CYuvMergeAlg : public CImageAlgBase
{
public:
	CYuvMergeAlg(void);
	virtual ~CYuvMergeAlg(void);

public:
	virtual int IsFrameDone(void);
	virtual void ResetState(void);
	virtual void SetInputCfg(int index, TStreamCfg *cfg);
	virtual int  GetOutputCfg(int index, TStreamCfg *cfg);

public:
	virtual int  Write(int index, char *buf, int len);
	virtual int  Read(int index, char *buf, int len);

private:
	int			m_width;
	int			m_height;
	int			m_unit;
	int			m_infmt[3];
	int			m_fmt;

	CYuv400Fifo	*m_pline;
	CYuv400Fifo	m_line400;
	CYuv422Fifo	m_line422;
	CYuv411Fifo	m_line411;
	CYuv444Fifo	m_line444;

	CYuv444_UFifo	m_u444;
	CYuv422_UFifo	m_u422;
	CYuv411_UFifo	m_u411;

	CYuv444_VFifo	m_v444;
	CYuv422_VFifo	m_v422;
	CYuv411_VFifo	m_v411;

	CYuv400Fifo		m_yline, *m_pu, *m_pv;
};

#endif
