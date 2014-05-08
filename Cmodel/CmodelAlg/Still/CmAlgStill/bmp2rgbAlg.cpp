#include "internal.h"


CBmp2rgbAlg::CBmp2rgbAlg(void)
{
}

CBmp2rgbAlg::~CBmp2rgbAlg(void)
{
}

int CBmp2rgbAlg::IsFrameDone(void)
{
	if(CImageAlgBase::IsFrameDone())
		return 1;
	if(this->m_outline == this->m_height)
		return 1;
	return 0;
}

void CBmp2rgbAlg::SetInputCfg(int index, TStreamCfg *cfg)
{
	index = 0;
	this->m_width = cfg->width;
	this->m_height = cfg->height;
	this->m_fmt = cfg->fmt;
	this->m_unit = cfg->unit;
	this->m_linebuf.SetSize(this->m_width, 0);
}

int CBmp2rgbAlg::GetOutputCfg(int index, TStreamCfg *cfg)
{
	index = 0;
	cfg->width = this->m_width;
	cfg->height = this->m_height;
	cfg->fmt = this->m_fmt;
	cfg->unit = this->m_unit;

	return 0;
}

void CBmp2rgbAlg::ResetState(void)
{
	this->m_outline = 0;
}

int CBmp2rgbAlg::Write(int index, char *buf, int len)
{
	index = index;		//dummy code, in case warning
	return this->m_linebuf.Write(buf, len);
}

int CBmp2rgbAlg::Read(int index, char *buf, int len)
{
	index = index;		//dummy code, in case warning
	len = this->m_linebuf.Read(buf, len);
	if(len)
	{
		this->m_linebuf.ClearPoint();
		this->m_outline++;
	}
	return len;
}

