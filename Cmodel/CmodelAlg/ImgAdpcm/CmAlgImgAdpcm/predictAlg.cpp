#include "internal.h"


CPredictAlg::CPredictAlg(void)
{
	this->m_clip = 255;
}

CPredictAlg::~CPredictAlg(void)
{
}

int CPredictAlg::IsFrameDone(void)
{
	if(CImageAlgBase::IsFrameDone())
		return 1;
	if(this->m_outline == this->m_height)
		return 1;
	return 0;
}

void CPredictAlg::SetInputCfg(int index, TStreamCfg *cfg)
{
	CTypeProp tp;

	index = 0;
	this->m_width = tp.getVwc(cfg->width, 1, cfg->fmt, cfg->unit);
	this->m_height = cfg->height;
	this->m_fmt = cfg->fmt;
	this->m_linebuf[0].SetSize(this->m_width, cfg->unit);
	this->m_linebuf[1].SetSize(this->m_width, cfg->unit);
}

int CPredictAlg::GetOutputCfg(int index, TStreamCfg *cfg)
{
	index = 0;
	cfg->width = this->m_width;
	cfg->height = this->m_height;
	cfg->fmt = this->m_fmt;
	cfg->unit = this->m_unit;
	return 0;
}

void CPredictAlg::ResetState(void)
{
	CImageAlgBase::ResetState();
}

int CPredictAlg::Write(int index, char *buf, int len)
{
	ASSERT((index == 0) || (index == 1));
	return this->m_linebuf[index].Write(buf, len);
}

int CPredictAlg::Read(int index, char *buf, int len)
{
	int i, val1, val2;

	index = len / this->m_unit;
	index = index > this->m_width ? this->m_width : index;
	if( (this->m_linebuf[0].IsFifoValid()) && (this->m_linebuf[1].IsFifoValid()) )
	{
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
		return index * this->m_unit;
	}
	
	return 0;
}

void CPredictAlg::SetClip(int val)
{
	this->m_clip = val;
}

int	CPredictAlg::GetClip(void)
{
	return this->m_clip;
}

void CPredictAlg::SetUnit(int val)
{
	this->m_unit = val;
}

int	CPredictAlg::GetUnit(void)
{
	return this->m_unit;
}

