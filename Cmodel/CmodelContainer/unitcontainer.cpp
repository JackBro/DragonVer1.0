#include "internal.h"


CUnitContainer::CUnitContainer(void)
{
}

CUnitContainer::~CUnitContainer(void)
{
}

CBaseUnit* CUnitContainer::Add(int sel)
{
	CBaseUnit* punit = NULL;
	int type, index;

	type = (sel & CLASSMASK) >> CLASSSHIFT;
	index = sel & ((1 << CLASSSHIFT) - 1);

	switch(type) 
	{
	case ISPCLASS:
		punit = this->AddIsp(index);
		break;
	case LOGICCLASS:
		punit = this->AddLogic(index);
		break;
	case IPPCLASS:
		punit = this->AddIpp(index);
		break;
	case STILLCLASS:
		punit = this->AddStill(index);
		break;
	default:
		break;
	}
	return punit;
}

CBaseUnit * CUnitContainer::AddIsp(int sel)
{
	CBaseUnit* punit = NULL;
	switch(sel)
	{
//	case AUTOFOCUS_BOX:
//		punit = this->m_AutoFocusChain.Add();
//		break;
//	case BLACKLEVEL_BOX:
//		punit = this->m_BlackLevelChain.Add();
//		break;
	case CFAEXTRACT_BOX:
		punit = this->m_CfaExtractChain.Add();
		break;
	case COLORCORRECT_BOX:
		punit = this->m_ColorCorrectChain.Add();
		break;
	case COLORCONVERT_BOX:
		punit = this->m_ColorConvertChain.Add();
		break;
	case DIGITALGAIN_BOX:
		punit = this->m_GainChain.Add();
		break;
	case LENSFALLOFF_BOX:
		punit = this->m_LensFallOffChain.Add();
		break;
	case DPDDPC_BOX:
		punit = this->m_DpdDpcChain.Add();
		break;
//	case TESTPATTERN_BOX:
//		punit = this->m_TestPatternChain.Add();
//		break;
	case GAMMACORRECT_BOX:
		punit = this->m_GammaCorrectChain.Add();
		break;
	case EDGEENHANCE_BOX:
		punit = this->m_EdgeEnhanceChain.Add();
		break;
	default:
		break;
	}
	return punit;
}

CBaseUnit * CUnitContainer::AddLogic(int sel)
{
	CBaseUnit* punit = NULL;
	switch(sel)
	{
	case LOGICALCOMGATE_BOX:
		punit = this->m_ComgateChain.Add();
		break;
	case LOGICALORGATE_BOX:
		punit = this->m_OrgateChain.Add();
		break;
	case LOGICALANDGATE_BOX:
		punit = this->m_AndgateChain.Add();
		break;
	case LOGICALNOTGATE_BOX:
		punit = this->m_NotgateChain.Add();
		break;
	case LOGICALXORGATE_BOX:
		punit = this->m_XorgateChain.Add();
		break;
	case ADDGATE_BOX:
		punit = this->m_AddgateChain.Add();
		break;
	case ADD2GATE_BOX:
		punit = this->m_Add2gateChain.Add();
		break;
	case SUBGATE_BOX:
		punit = this->m_SubgateChain.Add();
		break;
	case DELAYCELL_BOX:
		punit = this->m_DelaygateChain.Add();
		break;
	case ARITHMGATE_BOX:
		punit = this->m_ArithmgateChain.Add();
		break;
	default:
		break;
	}
	return punit;
}

CBaseUnit * CUnitContainer::AddIpp(int sel)
{
	CBaseUnit *punit = NULL;
	switch(sel) 
	{
	case VERSIZER_BOX:
		punit = this->m_VersizerChain.Add();
		break;
	case HORSIZER_BOX:
		punit = this->m_HorsizerChain.Add();
		break;
	default:
		break;
	}
	return punit;
}

CBaseUnit * CUnitContainer::AddStill(int sel)
{
	CBaseUnit *punit = NULL;
	switch(sel) 
	{
	case BMP2RGB_BOX:
		punit = this->m_Bmp2rgbChain.Add();
		break;
	case RGB2YUV_BOX:
		punit = this->m_Rgb2yuvChain.Add();
		break;
	case YUV2YUV_BOX:
		punit = this->m_yuv2yuvChain.Add();
		break;
	case YUV2RGB_BOX:
		punit = this->m_yuv2rgbChain.Add();
		break;
	case YUVDIV_BOX:
		punit = this->m_yuvdivChain.Add();
		break;
	case YUVMERGE_BOX:
		punit = this->m_yuvmergeChain.Add();
		break;
	case FMTCONVERT_BOX:
		punit = this->m_fmtcvtChain.Add();
		break;
	default:
		break;
	}
	return punit;
}

void CUnitContainer::Del(CBaseUnit *punit)
{
	int sel;
	int type, index;

	sel = punit->GetID();
	type = (sel & CLASSMASK) >> CLASSSHIFT;
	index = sel & ((1 << CLASSSHIFT) - 1);

	switch(type) 
	{
	case ISPCLASS:
		this->DelIsp(index, punit);
		break;
	case LOGICCLASS:
		this->DelLogic(index, punit);
		break;
	case IPPCLASS:
		this->DelIpp(index, punit);
		break;
	case STILLCLASS:
		this->DelStill(index, punit);
		break;
	default:
		break;
	}
}

void CUnitContainer::DelIsp(int sel, CBaseUnit *punit)
{
	switch(sel)
	{
//	case AUTOFOCUS_BOX:
//		this->m_AutoFocusChain.DeleteUnit((CAutoFocus *)punit );
//		break;
//	case BLACKLEVEL_BOX:
//		this->m_BlackLevelChain.DeleteUnit((CBlackLevel *)punit );
//		break;
	case CFAEXTRACT_BOX:
		this->m_CfaExtractChain.DeleteUnit((CCfaExtract *)punit );
		break;
	case COLORCORRECT_BOX:
		this->m_ColorCorrectChain.DeleteUnit((CColorCorrect *)punit );
		break;
	case COLORCONVERT_BOX:
		this->m_ColorConvertChain.DeleteUnit((CColorConvert *)punit );
		break;
	case DIGITALGAIN_BOX:
		this->m_GainChain.DeleteUnit((CDigitalGain *)punit );
		break;
	case LENSFALLOFF_BOX:
		this->m_LensFallOffChain.DeleteUnit((CLensFallOff *)punit );
		break;
	case DPDDPC_BOX:
		this->m_DpdDpcChain.DeleteUnit((CDpdDpc *)punit );
		break;
//	case TESTPATTERN_BOX:
//		this->m_TestPatternChain.DeleteUnit((CTestPattern *)punit );
//		break;
	case GAMMACORRECT_BOX:
		this->m_GammaCorrectChain.DeleteUnit((CGammaCorrect *)punit );
		break;
	case EDGEENHANCE_BOX:
		this->m_EdgeEnhanceChain.DeleteUnit((CEdgeEnhance *)punit );
		break;
	default:
		break;
	}
}
void CUnitContainer::DelLogic(int sel, CBaseUnit *punit)
{
	switch(sel)
	{
	case LOGICALCOMGATE_BOX:
		this->m_ComgateChain.DeleteUnit((CComGate *)punit );
		break;
	case LOGICALORGATE_BOX:
		this->m_OrgateChain.DeleteUnit((COrGate *)punit );
		break;
	case LOGICALANDGATE_BOX:
		this->m_AndgateChain.DeleteUnit((CAndGate *)punit );
		break;
	case LOGICALNOTGATE_BOX:
		this->m_NotgateChain.DeleteUnit((CNotGate *)punit );
		break;
	case LOGICALXORGATE_BOX:
		this->m_XorgateChain.DeleteUnit((CXorGate *)punit );
		break;
	case ADDGATE_BOX:
		this->m_AddgateChain.DeleteUnit((CAddGate *)punit );
		break;
	case ADD2GATE_BOX:
		this->m_Add2gateChain.DeleteUnit((CAdd2Gate *)punit );
		break;
	case SUBGATE_BOX:
		this->m_SubgateChain.DeleteUnit((CSubGate *)punit );
		break;
	case ARITHMGATE_BOX:
		this->m_ArithmgateChain.DeleteUnit((CArithmGate *)punit );
		break;
	default:
		break;
	}
}

void CUnitContainer::DelIpp(int sel, CBaseUnit *punit)
{
	switch(sel) 
	{
	case VERSIZER_BOX:
		this->m_VersizerChain.DeleteUnit((CVSizerBox *)punit);
		break;
	case HORSIZER_BOX:
		this->m_HorsizerChain.DeleteUnit((CHSizerBox *)punit);
		break;
	default:
		break;
	}
}

void CUnitContainer::DelStill(int sel, CBaseUnit *punit)
{
	switch(sel) 
	{
	case BMP2RGB_BOX:
		this->m_Bmp2rgbChain.DeleteUnit((CBmp2rgbBox *)punit);
		break;
	case RGB2YUV_BOX:
		this->m_Rgb2yuvChain.DeleteUnit((CRgb2yuvBox *)punit);
		break;
	case YUV2YUV_BOX:
		this->m_yuv2yuvChain.DeleteUnit((CYuv2yuvBox *)punit);
		break;
	case YUV2RGB_BOX:
		this->m_yuv2rgbChain.DeleteUnit((CYuv2rgbBox *)punit);
		break;
	case YUVDIV_BOX:
		this->m_yuvdivChain.DeleteUnit((CYuvdivBox *)punit);
		break;
	case YUVMERGE_BOX:
		this->m_yuvmergeChain.DeleteUnit((CYuvMergeBox *)punit);
		break;
	case FMTCONVERT_BOX:
		this->m_fmtcvtChain.DeleteUnit((CFmtConvertBox *)punit);
		break;
	case DELAYCELL_BOX:
		this->m_DelaygateChain.DeleteUnit((CDelayGate *)punit );
		break;
	default:
		break;
	}
}

