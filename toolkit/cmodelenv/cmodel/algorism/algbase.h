

#ifndef _CMODEL_ALGBASE_H_
#define _CMODEL_ALGBASE_H_

#include "../../../tools/global.h"

//
//	use "COM" conception later to modify cmodel
//


//=============== class CSifModule =================

class CAlgBase
{
public:
	CAlgBase();

	virtual void	AddInline(TStream *input, int width);
	virtual void	AddOutline(TStream *output, int width);
	virtual void	ResetCounter(void);

protected:
	int		m_inphor;
	int		m_inpver;
	int		m_outhor;
	int		m_outver;
};


#endif


