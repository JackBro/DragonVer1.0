#ifndef _CMODEL_ARITHM1_ALG_H_
#define _CMODEL_ARITHM1_ALG_H_


#ifndef CMALGGATES_DLL
	#define CMALGGATES_DLL  __declspec(dllimport)
#endif

#include "tools/algbase.h"
#include "toolbuf/linefifo.h"

enum OPERATORenum
{
	OP_ADD,
	OP_SUB,
	OP_MULTIPLY,
	OP_DIV
};

class CMALGGATES_DLL CArithm1Alg : public CImageAlgBase
{
public:
	CArithm1Alg(void);
	virtual ~CArithm1Alg(void);

public:
	virtual int IsFrameDone(void);
	virtual void ResetState(void);
	virtual void SetInputCfg(int index, TStreamCfg *cfg);
	virtual int  GetOutputCfg(int index, TStreamCfg *cfg);

public:
	virtual int  Write(int index, char *buf, int len);
	virtual int  Read(int index, char *buf, int len);

public:
	void	SetUpClip(int val);
	int		GetUpClip(void);
	void	SetLowClip(int val);
	int		GetLowClip(void);
	void	SetInUnit(int val);
	int		GetInUnit(void);
	void	SetOutUnit(int val);
	int		GetOutUnit(void);
	void	SetOpdata(int val);
	int		GetOpdata(void);
	void	SetOperator(int val);
	int		GetOperator(void);

private:
	int		m_UpClip;
	int		m_LowClip;
	int		m_width;
	int		m_height;
	int		m_fmt;
	int		m_InUnit;
	int		m_OutUnit;
	int		m_opdata;
	int		m_operator;
	int		m_linewidth; /* total line width in byte is  m_linewidth * m_InUnit(or m_OutUnit) */
	CLineFifo	m_linebuf[2];
};

#endif
