#ifndef _CMODEL_PROP_CONTAINER_H_
#define _CMODEL_PROP_CONTAINER_H_


#ifndef CMODELCONTAINER_DLL
	#define CMODELCONTAINER_DLL  __declspec(dllimport)
#endif

#include "CmPropIsp/DigitalGainBoxPropDlg.h"

//#include "CmodelAutoFocus/autofocusboxpropdlg.h"
//#include "CmodelBlackLevel/levelboxpropdlg.h"
#include "CmPropIsp/cfaextractpropdlg.h"
#include "CmPropIsp/colorconvertpropdlg.h"
#include "CmPropIsp/colorcorrectpropdlg.h"


#include "CmPropIsp/DpdDpcBoxPropDlg.h"
#include "CmPropIsp/EdgeEnhancePropDlg.h"
#include "CmPropIsp/GammaCorrectPropDlg.h"
//#include "CmodelDpdDpc/DpdDpcBoxPropDlg.h"
#include "CmPropIsp/LensFallOffBoxPropDlg.h"
//#include "CmodelTestPattern/TestPatternPropDlg.h"

#include "CmPropGates/comgatepropdlg.h"
#include "CmPropGates/DelayGatePropDlg.h"
#include "CmPropGates/andgatepropdlg.h"
#include "CmPropGates/OrGatePropDlg.h"
#include "CmPropGates/NotGatePropDlg.h"
#include "CmPropGates/XorGatePropDlg.h"
#include "CmPropGates/AddGatePropDlg.h"
#include "CmPropGates/SubGatePropDlg.h"
#include "CmPropGates/Add2GatePropDlg.h"
#include "CmPropGates/ArithmGatePropDlg.h"


#include "CmPropIPP/HsizerPropDlg.h"
#include "CmPropIPP/VsizerPropDlg.h"

#include "CmPropStill/BmpboxPropDlg.h"
#include "CmPropStill/rgb2yuvPropDlg.h"
#include "CmPropStill/yuv2yuvPropDlg.h"
#include "CmPropStill/yuv2rgbPropDlg.h"
#include "CmPropStill/yuvdivPropDlg.h"
#include "CmPropStill/yuvmergePropDlg.h"
#include "CmPropStill/fmtconvertPropDlg.h"



class CMODELCONTAINER_DLL CPropContainer
{
public:
	CPropContainer(void);
	virtual ~CPropContainer(void);

public:
	CBaseBoxPropDlg* GetPropDlg(int sel);

private:
	CBaseBoxPropDlg* GetIspPropDlg(int sel);
	CBaseBoxPropDlg* GetLogicPropDlg(int sel);
	CBaseBoxPropDlg* GetIppPropDlg(int sel);
	CBaseBoxPropDlg* GetStillPropDlg(int sel);

private:
//	CAutoFocusBoxPropDlg	m_AutoFocusPropDlg;
	CDigitalGainBoxPropDlg	m_GainPropDlg;
//	CLevelBoxPropDlg		m_BlackLevelPropDlg;
	CCfaExtractPropDlg		m_CfaExtractPropDlg;
	CColorConvertPropDlg	m_ColorConvertPropDlg;
	CColorCorrectPropDlg	m_ColorCorrectPropDlg;
	CLensFallOffBoxPropDlg	m_LensFallOffPropDlg;
	CDpdDpcBoxPropDlg		m_DpdDpcPropDlg;
	CEdgeEnhancePropDlg		m_EdgeEnhancePropDlg;
	CGammaCorrectPropDlg	m_GammaCorrectPropDlg;
//	CTestPatternPropDlg		m_TestPatternPropDlg;

	CDelayGatePropDlg		m_DelaygatePropDlg;
	CComGatePropDlg			m_ComgatePropDlg;
	CAndGatePropDlg			m_AndgatePropDlg;
	COrGatePropDlg			m_OrgatePropDlg;
	CNotGatePropDlg			m_NotgatePropDlg;
	CXorGatePropDlg			m_XorgatePropDlg;
	CAddGatePropDlg			m_AddPropDlg;
	CSubGatePropDlg			m_SubPropDlg;
	CAdd2GatePropDlg		m_Add2PropDlg;
	CArithmGatePropDlg		m_ArithmPropDlg;

	CHsizerBoxPropDlg		m_HsizerPropDlg;
	CVsizerBoxPropDlg		m_VsizerPropDlg;

	CBmpboxPropDlg			m_BmpboxPropDlg;
	CRgb2YuvPropDlg			m_Rgb2yuvPropDlg;
	CYuv2YuvPropDlg			m_yuv2yuvPropDlg;
	CYuv2RgbPropDlg			m_yuv2rgbPropDlg;
	CYuvDivPropDlg			m_yuvdivPropDlg;
	CYuvMergePropDlg		m_yuvmergePropDlg;
	CFmtCvtPropDlg			m_fmtcvtPropDlg;
};

#endif
