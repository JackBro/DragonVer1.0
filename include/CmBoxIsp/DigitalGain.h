#ifndef _CMODEL_ALG_DIGITAL_GAIN_H_
#define _CMODEL_ALG_DIGITAL_GAIN_H_


#ifndef CMODELISP_DLL
	#define CMODELISP_DLL  __declspec(dllimport)
#endif

#include "CmodelBase/imageunit.h"
#include "CmAlgIsp/568DigitalGainAlg.h"


class CMODELISP_DLL CDigitalGain : public CImageUnit
{
public:
	CDigitalGain();
	virtual ~CDigitalGain();

protected:	
	virtual void UpdateConfig(void);

public:
	virtual int	 GetID(void);
	virtual void ChoiceAlg(int sel);

public:
	void SetGlobalGain(int gain);
	int  GetGlobalGain(void);
	void SetRGain(int gain);
	int  GetRGain(void);
	void SetGGain(int gain);
	int  GetGGain(void);
	void SetBGain(int gain);
	int  GetBGain(void);
	void SetGlobalEn(int val);
	void SetRgbEn(int val);
	void SetGainStep(int step);
	int  GetGainStep(void);
private:

	int m_globalEn;
	int m_rgbEn;
	int m_globalGain;
	int m_gainStep;
	int m_rGain;
	int m_gGain;
	int m_bGain;

	C568DigitalGainAlg m_568DigitalGainAlg;
};

#endif


