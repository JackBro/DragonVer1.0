#include "internal.h"


CPinPortBase::CPinPortBase()
{
	this->m_cfg.fmt = gDefaultFmt;
	this->m_cfg.height = 480;
	this->m_cfg.width = 640;
	this->m_cfg.unit = 1;
}

CPinPortBase::~CPinPortBase()
{
}

void CPinPortBase::SetCfg(TStreamCfg *cfg)
{
	this->m_cfg = *cfg;
}

TStreamCfg * CPinPortBase::GetCfg(void)
{
	return &this->m_cfg;
}

void CPinPortBase::SetWidth(int width)
{
	this->m_cfg.width = width;
}

int CPinPortBase::GetWidth(void)
{
	return this->m_cfg.width;
}

void CPinPortBase::SetHeight(int height)
{
	this->m_cfg.height = height;
}

int CPinPortBase::GetHeight(void)
{
	return this->m_cfg.height;
}

void CPinPortBase::SetUnit(int unit)
{
	this->m_cfg.unit = unit;
}

int CPinPortBase::GetUnit(void)
{
	return this->m_cfg.unit;
}

void CPinPortBase::SetFormat(int fmt)
{
	this->m_cfg.fmt = fmt;
}

int CPinPortBase::GetFormat(void)
{
	return this->m_cfg.fmt;
}

//============================================

CPinBase::CPinBase()
{
	this->m_flag = 1;
}

CPinBase::~CPinBase()
{
}

int CPinBase::Write(char *buf, int len)
{
	ASSERT(len >= 0);
	if(len == 0)
	{
		if( ((this->GetFormat() & CLASSMASK) >> CLASSSHIFT) == CTRLCLASS)
			return 0;
	}
	this->m_flag = 0;
	return CFifoBuf::Write(buf, len);
}

int CPinBase::Read(char *buf, int len)
{
	ASSERT(len >= this->GetDataSize());

	len = CFifoBuf::Read(buf, len);
	if(CFifoBuf::IsFifoEmpty())
		this->m_flag = 1;
	return len;
}

int	CPinBase::IsFifoEmpty(void)
{
	return this->m_flag;
}

void CPinBase::ClearFlag(void)
{
	CFifoBuf::ClearPoint();
	this->m_flag = 1;
}

