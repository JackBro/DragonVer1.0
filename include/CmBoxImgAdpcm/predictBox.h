#ifndef _CMODEL_BOX_IMGADPCM_PREDICT_H_
#define _CMODEL_BOX_IMGADPCM_PREDICT_H_


#ifndef CMBOXIMGADPCM_DLL
	#define CMBOXIMGADPCM_DLL  __declspec(dllimport)
#endif

#include "CmodelBase/imageunit.h"
#include "CmAlgGates/add1Alg.h"

class CMBOXIMGADPCM_DLL CPredictBox : public CImageUnit
{
public:
	CPredictBox(void);
	virtual ~CPredictBox(void);

protected:	
	virtual void UpdateConfig(void);

public:
	virtual int	 GetID(void);
	virtual void ChoiceAlg(int sel);

public:
	void	SetClip(int val);
	int		GetClip(void);
	void	SetUnit(int val);
	int		GetUnit(void);

private:
	int		m_clip;
	int		m_unit;

private:
	CAdd1Alg	m_alg1;
};

#endif

