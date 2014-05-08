#include "internal.h"


CCom1Alg::CCom1Alg(void)
{
}

CCom1Alg::~CCom1Alg(void)
{
}

int CCom1Alg::IsFrameDone(void)
{
	if(CImageAlgBase::IsFrameDone())
		return 1;
	if(this->m_outline == this->m_height)
		return 1;
	return 0;
}

void CCom1Alg::SetInputCfg(int index, TStreamCfg *cfg)
{
	index = 0;

	this->m_width = cfg->width;
	this->m_height = cfg->height;
	this->m_fmt = cfg->fmt;
	this->m_unit = cfg->unit;
}

int CCom1Alg::GetOutputCfg(int index, TStreamCfg *cfg)
{
	CTypeProp	tp;

	index = 0;

	this->m_linewidth = tp.getLinewidth(this->m_width, this->m_fmt, this->m_unit);
	this->m_linebuf.SetSize(this->m_linewidth);
	cfg->width = this->m_width;
	cfg->height = this->m_height;
	cfg->fmt = this->m_fmt;
	cfg->unit = this->m_unit;

	return 0;
}

void CCom1Alg::ResetState(void)
{
	CImageAlgBase::ResetState();
}

int CCom1Alg::Write(int index, char *buf, int len)
{
	index = index;		//dummy code, in case warning
	len = this->m_linebuf.Write(buf, len);
	if(len && this->m_linebuf.IsFifoValid())
		this->m_outline++;
	return len;
}

int CCom1Alg::Read(int index, char *buf, int len)
{
	index = index;		//dummy code, in case warning
	if(len < this->m_linewidth)
		return 0;
	return this->m_linebuf.Read(buf, len);
}

