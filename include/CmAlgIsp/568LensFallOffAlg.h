

#ifndef _CMODEL_C568LENSFALLOFF_ALG_H_
#define _CMODEL_C568LENSFALLOFF_ALG_H_


#ifndef CMODELISPALG_DLL
	#define CMODELISPALG_DLL  __declspec(dllimport)
#endif

#include "tools/algbase.h"
#include "toolbuf/linebuf.h"
#include "toolconvert/linebayerbuf.h"


class CMODELISPALG_DLL C568LensFallOffAlg : public CImageAlgBase
{
public:
	C568LensFallOffAlg(void);
	~C568LensFallOffAlg(void);
public:
	virtual int  IsFrameDone(void);
	virtual void ResetState(void);
	virtual void SetInputCfg(int index, TStreamCfg *cfg);
	virtual int  GetOutputCfg(int index, TStreamCfg *cfg);

public:
	int  GetLensCx(void) const;
	int  GetLensCy(void) const;
	int  GetLensFocus(void) const;
	void SetLensX(int x);
	void SetLensY(int y);
	void SetLensFocus(int f);

public:
	virtual int  Write(int index, char *buf, int len);
	virtual int  Read(int index, char *buf, int len);

private:
	void Init(void);
private:
	int		m_width;
	int		m_height;
	int		m_fmt;
	int		m_unit;
	int     m_lensCx;		//0-11bit
	int     m_lensCy;		//0-11bit
	int     m_lensFocus;	//0-12bit

	CLineBayerBuf	m_Linebuf;
};


#endif