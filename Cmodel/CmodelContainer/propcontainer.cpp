#include "internal.h"


CPropContainer::CPropContainer(void)
{
}

CPropContainer::~CPropContainer(void)
{
}

CBaseBoxPropDlg* CPropContainer::GetPropDlg(int sel)
{
	CBaseBoxPropDlg *pdlg = NULL;
	int type, index;

	type = (sel & CLASSMASK) >> CLASSSHIFT;
	index = sel & ((1 << CLASSSHIFT) - 1);
	switch(type) 
	{
	case ISPCLASS:
		pdlg = this->GetIspPropDlg(index);
		break;
	case LOGICCLASS:
		pdlg = this->GetLogicPropDlg(index);
		break;
	case IPPCLASS:
		pdlg = this->GetIppPropDlg(index);
		break;
	case STILLCLASS:
		pdlg = this->GetStillPropDlg(index);
		break;
	default:
		break;
	}
	return pdlg;
}


CBaseBoxPropDlg* CPropContainer::GetIspPropDlg(int sel)
{
	CBaseBoxPropDlg *pdlg = NULL;
	switch(sel)
	{
//	case AUTOFOCUS_BOX:
//		pdlg = &this->m_AutoFocusPropDlg;
//		break;
//	case BLACKLEVEL_BOX:
//		pdlg = &this->m_BlackLevelPropDlg;
//		break;
	case DIGITALGAIN_BOX:
		pdlg = &this->m_GainPropDlg;
		break;
	case COLORCORRECT_BOX:
		pdlg = &this->m_ColorCorrectPropDlg;
		break;
	case COLORCONVERT_BOX:
		pdlg = &this->m_ColorConvertPropDlg;
		break;
	case CFAEXTRACT_BOX:
		pdlg = &this->m_CfaExtractPropDlg;
		break;
	case GAMMACORRECT_BOX:
		pdlg = &this->m_GammaCorrectPropDlg;
		break;
	case LENSFALLOFF_BOX:
		pdlg = &this->m_LensFallOffPropDlg;
		break;
	case DPDDPC_BOX:
		pdlg = &this->m_DpdDpcPropDlg;
		break;
//	case TESTPATTERN_BOX:
//		pdlg = &this->m_TestPatternPropDlg;
//		break;
	case EDGEENHANCE_BOX:
		pdlg = &this->m_EdgeEnhancePropDlg;
		break;
	default:
		break;
	}
	return pdlg;

}

CBaseBoxPropDlg* CPropContainer::GetLogicPropDlg(int sel)
{
	CBaseBoxPropDlg *pdlg = NULL;
	switch(sel)
	{
	case LOGICALNOTGATE_BOX:
		pdlg = &this->m_NotgatePropDlg;
		break;
	case LOGICALXORGATE_BOX:
		pdlg = &this->m_XorgatePropDlg;
		break;
	case LOGICALCOMGATE_BOX:
		pdlg = &this->m_ComgatePropDlg;
		break;
	case LOGICALORGATE_BOX:
		pdlg = &this->m_OrgatePropDlg;
		break;
	case LOGICALANDGATE_BOX:
		pdlg = &this->m_AndgatePropDlg;
		break;
	case ADDGATE_BOX:
		pdlg = &this->m_AddPropDlg;
		break;
	case ADD2GATE_BOX:
		pdlg = &this->m_Add2PropDlg;
		break;
	case SUBGATE_BOX:
		pdlg = &this->m_SubPropDlg;
		break;
	case DELAYCELL_BOX:
		pdlg = &this->m_DelaygatePropDlg;
		break;
	case ARITHMGATE_BOX:
		pdlg = &this->m_ArithmPropDlg;
		break;
	default:
		break;
	}
	return pdlg;
}

CBaseBoxPropDlg* CPropContainer::GetIppPropDlg(int sel)
{
	CBaseBoxPropDlg *pdlg = NULL;
	switch(sel) 
	{
	case VERSIZER_BOX:
		pdlg = &this->m_VsizerPropDlg;
		break;
	case HORSIZER_BOX:
		pdlg = &this->m_HsizerPropDlg;
		break;
	default:
		break;
	}
	return pdlg;
}

CBaseBoxPropDlg* CPropContainer::GetStillPropDlg(int sel)
{
	CBaseBoxPropDlg *pdlg = NULL;
	switch(sel) 
	{
	case BMP2RGB_BOX:
		pdlg = &this->m_BmpboxPropDlg;
		break;
	case RGB2YUV_BOX:
		pdlg = &this->m_Rgb2yuvPropDlg;
		break;
	case YUV2YUV_BOX:
		pdlg = &this->m_yuv2yuvPropDlg;
		break;
	case YUV2RGB_BOX:
		pdlg = &this->m_yuv2rgbPropDlg;
		break;
	case YUVDIV_BOX:
		pdlg = &this->m_yuvdivPropDlg;
		break;
	case YUVMERGE_BOX:
		pdlg = &this->m_yuvmergePropDlg;
		break;
	case FMTCONVERT_BOX:
		pdlg = &this->m_fmtcvtPropDlg;
		break;
	default:
		break;
	}
	return pdlg;
}
