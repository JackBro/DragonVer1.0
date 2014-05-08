#include "internal.h"


CAdd2Alg::CAdd2Alg(void)
{
	this->m_clip = 255;
	this->m_opdata = 0;
}

CAdd2Alg::~CAdd2Alg(void)
{
}

int CAdd2Alg::IsFrameDone(void)
{
	if(CImageAlgBase::IsFrameDone())
		return 1;
	if(this->m_outline == this->m_height)
		return 1;
	return 0;
}

void CAdd2Alg::SetInputCfg(int index, TStreamCfg *cfg)
{
	CTypeProp tp;

	index = 0;
	this->m_width = cfg ->width;
	this->m_height = cfg->height;
	this->m_fmt = cfg->fmt;
	this->m_InUnit = cfg->unit;
	this->m_linewidth = tp.getLinewidth(this->m_width ,this->m_fmt, 1);
	this->m_linebuf[0].SetSize(this->m_linewidth, this->m_InUnit);
	
}

int CAdd2Alg::GetOutputCfg(int index, TStreamCfg *cfg)
{
	index = 0;
	cfg->width = this->m_width;
	cfg->height = this->m_height;
	cfg->fmt = this->m_fmt;
	cfg->unit = this->m_OutUnit;
	return 0;
}

void CAdd2Alg::ResetState(void)
{
	CImageAlgBase::ResetState();
}

int CAdd2Alg::Write(int index, char *buf, int len)
{
	ASSERT((index == 0));
	return this->m_linebuf[0].Write(buf, len);
}

int CAdd2Alg::Read(int index, char *buf, int len)
{
	int i, val1;
	
	ASSERT((index == 0));
	if (len < this->m_linewidth * this->m_OutUnit)
		return 0;

	if( (this->m_linebuf[0].IsFifoValid()))
	{
		this->m_linebuf[1].SetSize(buf, this->m_linewidth, this->m_OutUnit);
		for(i = 0; i < this->m_linewidth; i++)
		{
			val1 = this->m_linebuf[0].Read();
		
			val1 += this->m_opdata;
			val1 = (val1 > this->m_clip) ? this->m_clip : val1;
			this->m_linebuf[1].Write(val1);
		}
		if(this->m_linebuf[0].IsFifoEmpty())
			this->m_outline++;

		return this->m_linewidth  * this->m_OutUnit;
	}
	
	return 0;
}

void CAdd2Alg::SetClip(int val)
{
	this->m_clip = val;
}

int	CAdd2Alg::GetClip(void)
{
	return this->m_clip;
}

void CAdd2Alg::SetInUnit(int val)
{
	this->m_InUnit = val;
}

int	CAdd2Alg::GetInUnit(void)
{
	return this->m_InUnit;
}

void CAdd2Alg::SetOutUnit(int val)
{
	this->m_OutUnit = val;
}

int	CAdd2Alg::GetOutUnit(void)
{
	return this->m_OutUnit;
}

void CAdd2Alg::SetOpdata(int val)
{
	this->m_opdata = val;
}

int	CAdd2Alg::GetOpdata(void)
{
	return this->m_opdata;
}
