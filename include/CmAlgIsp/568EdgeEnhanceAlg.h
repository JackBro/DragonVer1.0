

#ifndef _CMODEL_568EDGEENHANCE_ALG_H_
#define _CMODEL_568EDGEENHANCE_ALG_H_


#ifndef CMODELISPALG_DLL
	#define CMODELISPALG_DLL  __declspec(dllimport)
#endif

#include "tools/algbase.h"
#include "toolconvert/linebayerbuf.h"


class CMODELISPALG_DLL C568EdgeEnhanceAlg : public CImageAlgBase
{
public:
	C568EdgeEnhanceAlg();
	~C568EdgeEnhanceAlg();
public:
	virtual int  IsFrameDone(void);
	virtual void ResetState(void);
	virtual void SetInputCfg(int index, TStreamCfg *cfg);
	virtual int  GetOutputCfg(int index, TStreamCfg *cfg);
public:
	virtual int Read(int index, char *buf, int len);
	virtual int Write(int index, char *buf, int len);

private:
	int m_width;
	int m_height;
	int m_unit;
	int m_fmt;
	CLineBayerBuf m_Linebuf;
	CLineBayerBuf m_EdgeLinebuf;

};

#endif