#include "internal.h"


CAdd1Alg::CAdd1Alg(void)
{
	this->m_clip = 255;
	this->m_unit = 1;
}

CAdd1Alg::~CAdd1Alg(void)
{
}

int CAdd1Alg::IsFrameDone(void)
{
	if(CImageAlgBase::IsFrameDone())
		return 1;
	if(this->m_outline == this->m_height)
		return 1;
	return 0;
}

void CAdd1Alg::SetInputCfg(int index, TStreamCfg *cfg)
{
	CTypeProp tp;

	this->m_width = cfg->width;
	this->m_height = cfg->height;
	this->m_fmt = cfg->fmt;
	index = tp.getVwc(this->m_width, 1, this->m_fmt);
	this->m_linebuf[0].SetSize(index, cfg->unit);
	this->m_linebuf[1].SetSize(index, cfg->unit);
}

int CAdd1Alg::GetOutputCfg(int index, TStreamCfg *cfg)
{
	CTypeProp tp;

	index = 0;
	cfg->width = this->m_width;
	cfg->height = this->m_height;
	cfg->fmt = this->m_fmt;
	cfg->unit = this->m_unit;
	this->m_linewidth = tp.getLinewidth(this->m_width, this->m_fmt, this->m_unit);
	return 0;
}

void CAdd1Alg::ResetState(void)
{
	CImageAlgBase::ResetState();
}

int CAdd1Alg::Write(int index, char *buf, int len)
{
	ASSERT((index == 0) || (index == 1));
	return this->m_linebuf[index].Write(buf, len);
}

int CAdd1Alg::Read(int index, char *buf, int len)
{
	int i, val1, val2;

	if(len < this->m_linewidth)
		return 0;
	if( (this->m_linebuf[0].IsFifoValid() == 0) ||
		(this->m_linebuf[1].IsFifoValid() == 0) )
		return 0;

	index = this->m_linewidth / this->m_unit;
	this->m_linebuf[2].SetSize(buf, index, this->m_unit);
	for(i = 0; i < index; i++)
	{
		val1 = this->m_linebuf[0].Read();
		val2 = this->m_linebuf[1].Read();
		val1 += val2;
		val1 = (val1 > this->m_clip) ? this->m_clip : val1;
		this->m_linebuf[2].Write(val1);
	}
	if(this->m_linebuf[0].IsFifoEmpty())
		this->m_outline++;
	return this->m_linewidth;
}

void CAdd1Alg::SetClip(int val)
{
	this->m_clip = val;
}

int	CAdd1Alg::GetClip(void)
{
	return this->m_clip;
}

void CAdd1Alg::SetUnit(int val)
{
	this->m_unit = val;
}

int	CAdd1Alg::GetUnit(void)
{
	return this->m_unit;
}

