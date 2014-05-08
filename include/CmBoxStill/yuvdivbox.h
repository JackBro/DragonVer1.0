#ifndef _CMODEL_YUVDIV_BOX_H_
#define _CMODEL_YUVDIV_BOX_H_



#ifndef CMODELSTBOX_DLL
#define CMODELSTBOX_DLL  __declspec(dllimport)
#endif

#include "CmodelBase/imageunit.h"
#include "CmAlgStill/yuvdivAlg.h"

class CMODELSTBOX_DLL CYuvdivBox :	public CImageUnit
{
public:
	CYuvdivBox(void);
	virtual ~CYuvdivBox(void);

public:
	virtual int	 GetID(void);
	virtual void ChoiceAlg(int sel);

protected:	
	virtual void UpdateConfig(void);

private:
	CYuvdivAlg	m_alg1;
};

//extern CMODELSTBOX_DLL const int g_YuvDivFormats[];
//extern CMODELSTBOX_DLL const int g_YuvDivFmtCount;

#endif
