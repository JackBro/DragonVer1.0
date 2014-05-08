
#ifndef _CMODEL_568GAMMACORRECT_ALG_H_
#define _CMODEL_568GAMMACORRECT_ALG_H_


#ifndef CMODELISPALG_DLL
	#define CMODELISPALG_DLL  __declspec(dllimport)
#endif

#include "tools/algbase.h"
#include "toolconvert/linebayerbuf.h"


class CMODELISPALG_DLL C568GammaCorrectAlg : public CImageAlgBase
{
public:
	C568GammaCorrectAlg();
	~C568GammaCorrectAlg(void);

public:
	virtual int  IsFrameDone(void);
	virtual void ResetState(void);
	virtual void SetInputCfg(int index, TStreamCfg *cfg);
	virtual int  GetOutputCfg(int index, TStreamCfg *cfg);

public:
	virtual int Read(int index, char *buf, int len);
	virtual int Write(int index, char *buf, int len);
protected:
	int	 GetGammaValue(int val, int rgb);
	int  Dither(int val);

public:
	void	SetEnable(int enable);
	void	SetDither(int dither);
	void	SetDitherValue(int val);
	void	SetRGamma(int *p);
	int*	GetRGamma(void);
	void	SetGGamma(int *p);
	int*	GetGGamma(void);
	void	SetBGamma(int *p);
	int*	GetBGamma(void);

private:
	int m_rgamma[17];
	int m_ggamma[17];
	int m_bgamma[17];
	int m_unit;
	int m_width;
	int m_height;
	int m_fmt;
	int m_r;
	int m_g;
	int m_b;
	int m_enable;		//1: enable gamma  0:disable gamma,just cut lower 2 bit
	int m_dither;
	int m_dither_val;

	CLineBayerBuf m_Linebuf;
};

#endif