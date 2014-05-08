#ifndef _CMODEL_COLORCONVERT_H_
#define _CMODEL_COLORCONVERT_H_


#ifndef CMODELISP_DLL
	#define CMODELISP_DLL  __declspec(dllimport)
#endif

#include "CmodelBase/imageunit.h"
#include "CmAlgIsp/568ColorConvertAlg.h"

class CMODELISP_DLL CColorConvert : public CImageUnit
{
public:
	CColorConvert(void);
	virtual ~CColorConvert(void);

public:
	virtual int	 GetID(void);
	virtual void ChoiceAlg(int sel);

protected:	
	virtual void UpdateConfig(void);

private:
	C568ColorConvertAlg m_568ColorConvertAlg;
	
};

#endif
