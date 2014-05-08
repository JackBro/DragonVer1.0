#ifndef _CMODEL_ALG_BASE_H_
#define _CMODEL_ALG_BASE_H_


#ifndef TOOLS_DLL
	#define TOOLS_DLL  __declspec(dllimport)
#endif



typedef struct tag_TStreamCfg
{
	int fmt;
	int unit;
	int width;
	int height;
} TStreamCfg, *PTStreamCfg;

class TOOLS_DLL CAlgBase
{
public:
	CAlgBase(void);
	virtual ~CAlgBase(void);

public:
	virtual int  Write(int index, char *buf, int len) = 0;
	virtual int  Read(int index, char *buf, int len) = 0;
	virtual void ResetState(void);
	virtual void SetInputCfg(int index, TStreamCfg *cfg);
	virtual int  GetOutputCfg(int index, TStreamCfg *cfg);
};

class TOOLS_DLL CImageAlgBase : public CAlgBase
{
public:
	CImageAlgBase(void);
	virtual ~CImageAlgBase(void);

public:
	virtual void ResetState(void);
	virtual int IsFrameDone(void);

public:
	int		m_outline;
};

#endif
