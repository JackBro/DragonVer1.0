#ifndef _CMODEL_FORMATCONVERT_BOX_H_
#define _CMODEL_FORMATCONVERT_BOX_H_



#ifndef CMODELSTBOX_DLL
#define CMODELSTBOX_DLL  __declspec(dllimport)
#endif

#include "CmodelBase/imageunit.h"
#include "CmAlgStill/fmtconvertAlg.h"

class CMODELSTBOX_DLL CFmtConvertBox :	public CImageUnit
{
public:
	CFmtConvertBox(void);
	virtual ~CFmtConvertBox(void);

public:
	virtual int	 GetID(void);
	virtual void ChoiceAlg(int sel);

protected:	
	virtual void UpdateConfig(void);

public:
	void	SetOutFmt(int format);
	int		GetOutFmt(void);

private:
	int		m_format;
	CFmtConvertAlg	m_alg1;
};

extern CMODELSTBOX_DLL const int g_FmtcvtFormats[];
extern CMODELSTBOX_DLL const int g_FmtcvtCount;

#endif
