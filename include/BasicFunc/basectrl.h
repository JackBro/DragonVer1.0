#ifndef __BASICFUNC_BASECTRL_H__
#define __BASICFUNC_BASECTRL_H__



#include "toolparse/batchparse.h"

class CBaseCtrl
{
public:
	void	WriteReg(TRegUnit *unit);
	void	WriteReg(int type, int adr, int val);
	void	ReadReg(TRegUnit *unit);
	int		ReadReg(int type, int adr);
};

#endif
