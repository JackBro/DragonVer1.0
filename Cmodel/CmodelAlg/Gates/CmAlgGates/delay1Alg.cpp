#include "internal.h"


CDelay1Alg::CDelay1Alg(void)
{
	this->m_delay = 0;
}

CDelay1Alg::~CDelay1Alg(void)
{
}

int CDelay1Alg::IsFrameDone(void)
{
	if(CImageAlgBase::IsFrameDone())
		return 1;
	if(this->m_outline == this->m_height)
		return 1;
	return 0;
}

void CDelay1Alg::SetInputCfg(int index, TStreamCfg *cfg)
{
	index = 0;
	this->m_width = cfg->width;
	this->m_height = cfg->height;
	this->m_fmt = cfg->fmt;
	this->m_unit = cfg->unit;
}

int CDelay1Alg::GetOutputCfg(int index, TStreamCfg *cfg)
{
	int i;
	CTypeProp	tp;

	index = 0;
	this->m_linewidth = tp.getLinewidth(this->m_width, this->m_fmt, this->m_unit);
	cfg->width = this->m_width;
	cfg->height = this->m_height;
	cfg->fmt = this->m_fmt;
	cfg->unit = this->m_unit;
	this->m_line.SetUnitCount(this->m_delay+1);
	for(i = 0; i <= this->m_delay; i++)
		this->m_line[i]->SetSize(m_linewidth);
	return 0;
}

void CDelay1Alg::ResetState(void)
{
	this->m_outline = 0;
	this->m_inline = 0;
	this->m_delaycnt = this->m_delay+1;
}

int CDelay1Alg::Write(int index, char *buf, int len)
{
	if(this->m_inline == this->m_height)
		return 0;
	index = this->m_inline % (this->m_delay+1);
	len = this->m_line[index]->Write(buf, len);
	if(len && this->m_line[index]->IsFifoValid())
	{
		this->m_inline++;
		if(this->m_delaycnt)
			this->m_delaycnt--;
	}
	return len;
}

int CDelay1Alg::Read(int index, char *buf, int len)
{
	if(this->m_delaycnt)
		return 0;
	if(len < this->m_linewidth)
		return 0;
	index = this->m_outline % (this->m_delay+1);
	len = this->m_line[index]->Read(buf, len);
	if(len)
	{
		this->m_line[index]->ClearPoint();
		this->m_outline++;
	}
	return len;
}

void CDelay1Alg::SetDelay(int delay)
{
	this->m_delay = delay;
}

int	CDelay1Alg::GetDelay(void)
{
	return this->m_delay;
}

