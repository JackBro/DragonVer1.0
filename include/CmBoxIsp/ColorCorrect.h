#ifndef _CMODEL_COLORCORRECT_H_
#define _CMODEL_COLORCORRECT_H_


#ifndef CMODELISP_DLL
	#define CMODELISP_DLL  __declspec(dllimport)
#endif


#include "CmodelBase/imageunit.h"
#include "CmAlgIsp/568ColorCorrectAlg.h"

class CMODELISP_DLL CColorCorrect : public CImageUnit
{
public:
	CColorCorrect(void);
	virtual ~CColorCorrect(void);

public:
	virtual int	 GetID(void);
	virtual void ChoiceAlg(int sel);
	
	void SetColorMatrix(int *p);
	int* GetColorMatrix();
	void SetColorOffset(int *p);
	int* GetColorOffset();

protected:	
	virtual void UpdateConfig(void);
private:
	int m_colormatrix[3][3];
	int m_offset[3];

	C568ColorCorrectAlg m_568ColorCorrectAlg;
};

#endif
