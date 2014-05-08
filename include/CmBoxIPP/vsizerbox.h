
#ifndef _CMODEL_VSIZERBOX_H_
#define _CMODEL_VSIZERBOX_H_

#ifndef CMODELIPPBOX_DLL
	#define CMODELIPPBOX_DLL  __declspec(dllimport)
#endif


#include "CmodelBase/imageunit.h"
#include "CmAlgIPP/702VsizerAlg.h"


class CMODELIPPBOX_DLL CVSizerBox : public CImageUnit
{
public:
	CVSizerBox(void);
	virtual ~CVSizerBox(void);

public:
	virtual int	 GetID(void);
	virtual void ChoiceAlg(int sel);

public:
	int		GetOutMode(void);
	void	SetOutMode(int pal);

protected:	
	virtual void UpdateConfig(void);

private:	 
	C702VsizerAlg	m_702VsizerAlg;
	int				m_pal;
};

#endif