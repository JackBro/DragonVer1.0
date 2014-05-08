#ifndef _CMODEL_UNIT_CONTAINER_H_
#define _CMODEL_UNIT_CONTAINER_H_


#ifndef CMODELCONTAINER_DLL
	#define CMODELCONTAINER_DLL  __declspec(dllimport)
#endif

#include "toolchain/chaintools.h"

#include "CmBoxIsp/DigitalGain.h"

//#include "CModelIsp/autofocus.h"
//#include "CmodelBlackLevel/blacklevel.h"
#include "CmBoxIsp/cfaextract.h"
#include "CmBoxIsp/colorconvert.h"
#include "CmBoxIsp/colorcorrect.h"
#include "CmBoxIsp/dpddpc.h"
#include "CmBoxIsp/edgeenhance.h"
#include "CmBoxIsp/gammacorrect.h"
#include "CmBoxIsp/lensfalloff.h"

#include "CmBoxGates/andgate.h"
#include "CmBoxGates/comgate.h"
#include "CmBoxGates/delaygate.h"
#include "CmBoxGates/orgate.h"
#include "CmBoxGates/notgate.h"
#include "CmBoxGates/xorgate.h"
#include "CmBoxGates/addgate.h"
#include "CmBoxGates/subgate.h"
#include "CmBoxGates/addgate2.h"
#include "CmBoxGates/arithmgate.h"


#include "CmBoxIPP/hsizerbox.h"
#include "CmBoxIPP/vsizerbox.h"

#include "CmBoxStill/bmp2rgbbox.h"
#include "CmBoxStill/rgb2yuvbox.h"
#include "CmBoxStill/yuv2yuvbox.h"
#include "CmBoxStill/yuv2rgbbox.h"
#include "CmBoxStill/yuvdivbox.h"
#include "CmBoxStill/yuvmergebox.h"
#include "CmBoxStill/fmtconvertbox.h"



class CMODELCONTAINER_DLL CUnitContainer
{
public:
	CUnitContainer(void);
	virtual ~CUnitContainer(void);

public:
	CBaseUnit * Add(int sel);
	void Del(CBaseUnit *punit);

private:
	CBaseUnit * AddIsp(int sel);
	CBaseUnit * AddLogic(int sel);
	CBaseUnit * AddIpp(int sel);
	CBaseUnit * AddStill(int sel);

	void DelIsp(int sel, CBaseUnit *punit);
	void DelLogic(int sel, CBaseUnit *punit);
	void DelIpp(int sel, CBaseUnit *punit);
	void DelStill(int sel, CBaseUnit *punit);

private:
//	CChainTool<CAutoFocus>		m_AutoFocusChain;
	CChainTool<CDigitalGain>		m_GainChain;
//	CChainTool<CBlackLevel>		m_BlackLevelChain;
	CChainTool<CCfaExtract>		m_CfaExtractChain;
	CChainTool<CColorConvert>	m_ColorConvertChain;
	CChainTool<CColorCorrect>	m_ColorCorrectChain;
	CChainTool<CLensFallOff>	m_LensFallOffChain;
	CChainTool<CDpdDpc>			m_DpdDpcChain;
	CChainTool<CEdgeEnhance>	m_EdgeEnhanceChain;
	CChainTool<CGammaCorrect>	m_GammaCorrectChain;
//	CChainTool<CTestPattern>	m_TestPatternChain;

	CChainTool<CDelayGate>		m_DelaygateChain;
	CChainTool<CComGate>		m_ComgateChain;
	CChainTool<CAndGate>		m_AndgateChain;
	CChainTool<COrGate>			m_OrgateChain;
	CChainTool<CNotGate>		m_NotgateChain;
	CChainTool<CXorGate>		m_XorgateChain;
	CChainTool<CAddGate>		m_AddgateChain;
	CChainTool<CSubGate>		m_SubgateChain;
	CChainTool<CAdd2Gate>		m_Add2gateChain;
	CChainTool<CArithmGate>		m_ArithmgateChain;

	CChainTool<CHSizerBox>		m_HorsizerChain;
	CChainTool<CVSizerBox>		m_VersizerChain;

	CChainTool<CBmp2rgbBox>		m_Bmp2rgbChain;
	CChainTool<CRgb2yuvBox>		m_Rgb2yuvChain;
	CChainTool<CYuv2yuvBox>		m_yuv2yuvChain;
	CChainTool<CYuv2rgbBox>		m_yuv2rgbChain;
	CChainTool<CYuvdivBox>		m_yuvdivChain;
	CChainTool<CYuvMergeBox>	m_yuvmergeChain;
	CChainTool<CFmtConvertBox>	m_fmtcvtChain;
	
};

#endif
