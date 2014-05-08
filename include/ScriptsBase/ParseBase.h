#ifndef __SCRIPTSBASE_PARSEBASE_H__
#define __SCRIPTSBASE_PARSEBASE_H__


#ifndef SCRIPTSBASE_DLL
	#define SCRIPTSBASE_DLL _declspec(dllimport)
#endif

#include "toolfile/filebin.h"
#include "toolchar/charfifo.h"
#include "toolchain/chaintools.h"

class SCRIPTSBASE_DLL CParseBase : public CNodeBase
{
public:
	CParseBase(void);
	virtual ~CParseBase(void);

public:
	CParseBase *GetFather(void);
	void SetFather(CParseBase *pf);
	CParseBase *GetSibling(void);
	void SetSilbing(CParseBase *ps);

private:
	CParseBase *m_pfather;
	CParseBase *m_psibing;
};

#endif
