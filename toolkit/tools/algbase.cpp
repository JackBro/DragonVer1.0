#include "internal.h"

CAlgBase::CAlgBase(void)
{
}

CAlgBase::~CAlgBase(void)
{
}

void CAlgBase::ResetState(void)
{
}

void CAlgBase::SetInputCfg(int index, TStreamCfg *cfg)
{
	index = index;
	cfg = cfg;
}

int CAlgBase::GetOutputCfg(int index, TStreamCfg *cfg)
{
	index = index;
	cfg = cfg;
	return 0;
}

//===============================

CImageAlgBase::CImageAlgBase(void)
{
	this->m_outline = -1;
}

CImageAlgBase::~CImageAlgBase(void)
{
}

void CImageAlgBase::ResetState(void)
{
	this->m_outline = -1;
}

int CImageAlgBase::IsFrameDone(void)
{
	if(this->m_outline < 0)
		return 1;
	return 0;
}


