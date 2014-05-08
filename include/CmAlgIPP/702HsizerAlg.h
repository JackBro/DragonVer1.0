
#ifndef _CMODEL_702HSIZER_ALG_H_
#define _CMODEL_702HSIZER_ALG_H_


#ifndef CMODELIPPALG_DLL
#define CMODELIPPALG_DLL  __declspec(dllimport)
#endif

#include "tools/algbase.h"
#include "toolconvert/line422buf.h"

class CMODELIPPALG_DLL C702HsizerAlg : public CImageAlgBase
{
public:
	C702HsizerAlg(void);
	~C702HsizerAlg(void);

public:
	virtual int  IsFrameDone(void);
	virtual void ResetState(void);
	virtual void SetInputCfg(int index, TStreamCfg *cfg);
	virtual int  GetOutputCfg(int index, TStreamCfg *cfg);

public:
	virtual int  Write(int index, char *buf, int len);
	virtual int  Read(int index, char *buf, int len);

public:
	void	SetFactor(void);
	int		GetValue(int val0, int val1, int coeff, int precision);

private:
	int		m_width;
	int		m_height;
	int		m_fmt;
	int		m_unit;
	int		m_coeff9[4], m_coeff45[22], m_up, m_down, m_phase, *m_pcoeff;
	
	CLine422Buf		m_linebuf;
};






#endif