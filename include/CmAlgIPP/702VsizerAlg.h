
#ifndef _CMODEL_702VSIZER_ALG_H_
#define _CMODEL_702VSIZER_ALG_H_


#ifndef CMODELIPPALG_DLL
#define CMODELIPPALG_DLL  __declspec(dllimport)
#endif

#include "tools/algbase.h"
#include "toolconvert/line422buf.h"


class CMODELIPPALG_DLL C702VsizerAlg : public CImageAlgBase
{
public:
	C702VsizerAlg(void);
	~C702VsizerAlg(void);

public:
	virtual int  IsFrameDone(void);
	virtual void ResetState(void);
	virtual void SetInputCfg(int index, TStreamCfg *cfg);
	virtual int  GetOutputCfg(int index, TStreamCfg *cfg);

public:
	virtual int  Write(int index, char *buf, int len);
	virtual int  Read(int index, char *buf, int len);

public:
	void	SetOutMode(int pal);
	int		GetOutMode(void);
	void	SetFactor(void);
	void	LoopLinebuf(void);
	int		GetValue(int val0, int val1, int coeff, int precision);

private:
	int		m_width;
	int		m_height, m_dsth;
	int		m_pal;
	int		m_fmt;
	int		m_unit;
	int		m_inline;
	int		m_coeff5[2], m_up, m_down, m_phase, *m_pcoeff;
	
	CLine422Buf		m_linebuf[2], *m_plinebuf[2];
};






#endif