#ifndef _CMODEL_INPORT_H_
#define _CMODEL_INPORT_H_


#ifndef CMODELBASE_DLL
	#define CMODELBASE_DLL  __declspec(dllimport)
#endif

#include "pinbase.h"
#include "tools/baseitem.h"
#include "toolbuf/bufferbase.h"
#include "toolchain/chaintools.h"
#include "toolfile/filebin.h"
#include "tooltree/treebase.h"

class CMODELBASE_DLL CPortBase : public CBaseObject, public CPinPortBase
{
public:
	CPortBase(void);
	virtual ~CPortBase(void);

public:
	virtual void	Close(void);

public:
	void SetFilename(char *filename);
	char *GetFilename(void);
	void SetPin(CPinBase *pin);
	CPinBase *GetPin(void);

protected:
	char	m_filename[DEFAULT_FILENAME_LEN];	
	CPinBase	*m_pin;
	CFileBin    m_fb;
};

class CMODELBASE_DLL CInportProp : public CPortBase
{
public:
	CInportProp(void);
	virtual ~CInportProp(void);

public:
	virtual int	    Read(char *buf, int len);
	virtual void	Open(void);

public:
	void SetPortFormats(int count, int *format);
	int  GetFmtCount(void);
	int  *GetFormats(void);
	void AddRpoint(int rp);
	int	 HasUnknowType(void);
	int	 IsCtrlPort(void);

public:
	void StoreParm(CKeyTree *ptree);
	void LoadParm(CKeyTree *ptree);

private:
	int		m_fmtcnt;
	int		*m_fmts;
	CBufferBase m_pool;
	CBufferBase m_fmtpool;
	int		m_len;
	int		m_pos;
};

//============================================

class CMODELBASE_DLL CInport : public CChainTool<CInportProp>
{
public:
	CInport(void);
	virtual ~CInport(void);

public:
	void StoreParm(CKeyTree *ptree);
	void LoadParm(CKeyTree *ptree);
};

#endif
