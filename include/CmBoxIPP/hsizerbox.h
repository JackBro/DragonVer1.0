
#ifndef _CMODEL_HSIZERBOX_H_
#define _CMODEL_HSIZERBOX_H_

#ifndef CMODELIPPBOX_DLL
	#define CMODELIPPBOX_DLL  __declspec(dllimport)
#endif


#include "CmodelBase/imageunit.h"
#include "CmAlgIPP/702HsizerAlg.h"


class CMODELIPPBOX_DLL CHSizerBox : public CImageUnit
{
public:
	CHSizerBox(void);
	virtual ~CHSizerBox(void);

public:
	virtual int	 GetID(void);
	virtual void ChoiceAlg(int sel);

public:
	
protected:	
	virtual void UpdateConfig(void);

private:
	 
	C702HsizerAlg m_702HsizerAlg;
};

#endif