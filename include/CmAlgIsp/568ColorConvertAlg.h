

#ifndef _CMODEL_COLORCONVERT568_ALG_H_
#define _CMODEL_COLORCONVERT568_ALG_H_


#ifndef CMODELISPALG_DLL
	#define CMODELISPALG_DLL  __declspec(dllimport)
#endif

#include "tools/algbase.h"
#include "toolconvert/linebayerbuf.h"


class CMODELISPALG_DLL C568ColorConvertAlg : public CImageAlgBase
{
public:

	C568ColorConvertAlg(void);
	~C568ColorConvertAlg(void);

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
};

#endif 
