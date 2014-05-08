

#ifndef _CMODEL_SIFALG_H_
#define _CMODEL_SIFALG_H_

#include "../../../tools/global.h"
#include "algbase.h"

//
//	use "COM" conception later to modify cmodel
//


//=============== class CSifModule =================

class CSifAlg : public CAlgBase
{
public:
	CSifAlg();

	virtual void	Sizer(TStream *input, TStream *output, TSize src, TPoint start, TPoint end);
	virtual void	Convert(TStream *input, TStream *output, TSize src, TPoint start, TPoint end);

protected:
	TStream m_data[2];
};


#endif


