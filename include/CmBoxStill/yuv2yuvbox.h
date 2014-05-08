#ifndef _CMODEL_YUV2YUV_BOX_H_
#define _CMODEL_YUV2YUV_BOX_H_



#ifndef CMODELSTBOX_DLL
#define CMODELSTBOX_DLL  __declspec(dllimport)
#endif

#include "CmodelBase/imageunit.h"
#include "CmAlgStill/yuv2yuvAlg.h"

class CMODELSTBOX_DLL CYuv2yuvBox :	public CImageUnit
{
public:
	CYuv2yuvBox(void);
	virtual ~CYuv2yuvBox(void);

public:
	virtual int	 GetID(void);
	virtual void ChoiceAlg(int sel);

protected:	
	virtual void UpdateConfig(void);

public:
	int		GetOutFmt(void);
	void	SetOutFmt(int fmt);

private:
	int		m_outfmt;

private:
	CYuv2yuvAlg	m_alg1;
};

extern CMODELSTBOX_DLL const int g_Yuv2YuvFormats[];
extern CMODELSTBOX_DLL const int g_Yuv2YuvFmtCount;

#endif
