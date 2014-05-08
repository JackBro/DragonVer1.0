#include "internal.h"


CRgb2yuvAlg::CRgb2yuvAlg(void)
{
}

CRgb2yuvAlg::~CRgb2yuvAlg(void)
{
}

int CRgb2yuvAlg::IsFrameDone(void)
{
	if(CImageAlgBase::IsFrameDone())
		return 1;
	if(this->m_outline == this->m_height)
		return 1;
	return 0;
}

void CRgb2yuvAlg::SetInputCfg(int index, TStreamCfg *cfg)
{
	index = 0;
	this->m_width = cfg->width;
	this->m_height = cfg->height;
	this->m_fmt = cfg->fmt;
	this->m_unit = cfg->unit;
	this->m_linebuf.SetSize(this->m_width * 3, this->m_unit);
}

int CRgb2yuvAlg::GetOutputCfg(int index, TStreamCfg *cfg)
{
	index = 0;
	cfg->width = this->m_width;
	cfg->height = this->m_height;
	cfg->fmt = YUV444 | (VIDEOCLASS << CLASSSHIFT);
	cfg->unit = this->m_unit;
	if( (this->m_unit != 1) || (this->m_width %2) )
		return 1;
	return 0;
}

void CRgb2yuvAlg::ResetState(void)
{
	CImageAlgBase::ResetState();
}

int CRgb2yuvAlg::Write(int index, char *buf, int len)
{
	index = index;		//dummy code, in case warning
	return this->m_linebuf.Write(buf, len);
}

int CRgb2yuvAlg::Read(int index, char *buf, int len)
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
		rgb[0] = this->m_linebuf.Read();
		rgb[1] = this->m_linebuf.Read();
		rgb[2] = this->m_linebuf.Read();
		tool.rgb2yuv(rgb, yuv);
		pl.Write(yuv[0]);
		pl.Write(yuv[1]);
		pl.Write(yuv[2]);
	}
	this->m_linebuf.ClearPoint();
	this->m_outline++;
	return pl.GetDataSize();
}

