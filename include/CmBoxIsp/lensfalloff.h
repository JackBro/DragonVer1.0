#ifndef _CMODEL_LENSFALLOFF_H_
#define _CMODEL_LENSFALLOFF_H_


#ifndef CMODELISP_DLL
	#define CMODELISP_DLL  __declspec(dllimport)
#endif


#include "CmodelBase/imageunit.h"
#include "CmAlgIsp/568LensFallOffAlg.h"

class CMODELISP_DLL CLensFallOff : public CImageUnit
{
public:
	CLensFallOff(void);
	virtual ~CLensFallOff(void);

public:
	virtual int	 GetID(void);
	virtual void ChoiceAlg(int sel);

public:
	int  Get568LensCx(void) const;
	int  Get568LensCy(void) const;
	int  Get568LensFocus(void) const;
	void Set568LensCx(int lensCx);
	void Set568LensCy(int lensCy);
	void Set568LensFocus(int lensFocus);
protected:	
	virtual void UpdateConfig(void);

private:
	C568LensFallOffAlg m_568LensFallOffAlg; 
	int		    m_568lensCx;	//0-11bit
	int         m_568lensCy;	//0-11bit
	int         m_568lensFocus;	//0-12bit	
};

#endif
