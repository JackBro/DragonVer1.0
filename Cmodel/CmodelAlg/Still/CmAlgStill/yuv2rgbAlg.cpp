#include "internal.h"


CYuv2rgbAlg:: ~CYuv2rgbAlg(void)
{
}


CYuv2rgbAlg:: CYuv2rgbAlg(void)
{
}

int CYuv2rgbAlg::IsFrameDone(void)
{
	if(CImageAlgBase::IsFrameDone())
		return 1;
	if(this->m_outline == this->m_height)
		return 1;
	return 0;
}

void CYuv2rgbAlg::SetInputCfg(int index, TStreamCfg *cfg)
{
	index = 0;
	this->m_width = cfg->width;
	this->m_height = cfg->height;
	this->m_fmt = cfg->fmt;
	this->m_unit = cfg->unit;
	this->m_linebuf.SetSize(this->m_width * 3, this->m_unit);
}

int CYuv2rgbAlg::GetOutputCfg(int index, TStreamCfg *cfg)
{
	index = 0;
	cfg->width = this->m_width;
	cfg->height = this->m_height;
	cfg->fmt = RGB24 | (VIDEOCLASS << CLASSSHIFT);
	cfg->unit = this->m_unit;
	if( (this->m_unit != 1) || (this->m_width %2) )
		return 1;
	return 0;
}

void CYuv2rgbAlg::ResetState(void)
{
	CImageAlgBase::ResetState();
}

int CYuv2rgbAlg::Write(int index, char *buf, int len)
{
	index = index;		//dummy code, in case warning
	return this->m_linebuf.Write(buf, len);
}

int CYuv2rgbAlg::Read(int index, char *buf, int len)
{
	int i;
	int rgb[3], yuv[3];
	CYuv2Rgb tool;
	CLineFifo pl;

	if(index > 0)
		return 0;
	if(this->m_linebuf.IsFifoValid() == 0)
		return 0;
	if(len < this->m_width * this->m_unit * 3)
		return 0;
	pl.SetSize(buf, this->m_width * 3, this->m_unit);
	for(i = 0; i < this->m_width; i++)
	{
		yuv[0] = this->m_linebuf.Read();
		yuv[1] = this->m_linebuf.Read();
		yuv[2] = this->m_linebuf.Read();
		tool.yuv2rgb(yuv, rgb);
		pl.Write(rgb[0]);
		pl.Write(rgb[1]);
		pl.Write(rgb[2]);
	}
	this->m_linebuf.ClearPoint();
	this->m_outline++;
	return pl.GetDataSize();
}