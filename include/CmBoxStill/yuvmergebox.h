#ifndef _CMODEL_YUVMERGE_BOX_H_
#define _CMODEL_YUVMERGE_BOX_H_



#ifndef CMODELSTBOX_DLL
#define CMODELSTBOX_DLL  __declspec(dllimport)
#endif

#include "CmodelBase/imageunit.h"
#include "CmAlgStill/yuvmergeAlg.h"

class CMODELSTBOX_DLL CYuvMergeBox :	public CImageUnit
{
public:
	CYuvMergeBox(void);
	virtual ~CYuvMergeBox(void);

public:
	virtual int	 GetID(void);
	virtual void ChoiceAlg(int sel);

protected:	
	virtual void UpdateConfig(void);

private:
	CYuvMergeAlg	m_alg1;
};

//extern CMODELSTBOX_DLL const int g_YuvDivFormats[];
//extern CMODELSTBOX_DLL const int g_YuvDivFmtCount;

#endif
