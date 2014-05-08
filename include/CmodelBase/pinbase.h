

#ifndef _TOOLCMODEL_PINBASE_H_
#define _TOOLCMODEL_PINBASE_H_


#ifndef CMODELBASE_DLL
	#define CMODELBASE_DLL  __declspec(dllimport)
#endif

#include "tools/algbase.h"
#include "toolbuf/fifobuf.h"



class CMODELBASE_DLL CPinPortBase
{
public:
	CPinPortBase(void);
	virtual ~CPinPortBase(void);

public:
	void SetCfg(TStreamCfg *cfg);
	TStreamCfg * GetCfg(void);

public:
	void SetWidth(int width);
	int  GetWidth(void);
	void SetHeight(int height);
	int  GetHeight(void);
	void SetUnit(int unit);
	int  GetUnit(void);
	void SetFormat(int fmt);
	int  GetFormat(void);

private:
	TStreamCfg m_cfg;
};

class CMODELBASE_DLL CPinBase : public CFifoBuf, public CPinPortBase
{
public:
	CPinBase(void);
	virtual ~CPinBase(void);

public:
	virtual int Write(char *buf, int len);
	virtual int Read(char *buf, int len);

public:
	int		IsFifoEmpty(void);
	void	ClearFlag(void);

private:
	int  m_flag;
};

#endif


