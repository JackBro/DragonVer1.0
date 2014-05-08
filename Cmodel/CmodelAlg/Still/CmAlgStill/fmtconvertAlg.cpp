#include "internal.h"


CFmtConvertAlg:: ~CFmtConvertAlg(void)
{
	this->m_fmt = 0;
}


CFmtConvertAlg:: CFmtConvertAlg(void)
{
}

int CFmtConvertAlg::IsFrameDone(void)
{
	if(CImageAlgBase::IsFrameDone())
		return 1;
	if(this->m_outline == this->m_height)
		return 1;
	return 0;
}

void CFmtConvertAlg::SetInputCfg(int index, TStreamCfg *cfg)
{
	CTypeProp	tp;

	index = 0;
	this->m_linewidth = tp.getVwc(cfg->width, 1, cfg->fmt);
	this->m_height = cfg->height;
	this->m_unit = cfg->unit;
	this->m_linebuf.SetSize(this->m_linewidth, this->m_unit);
}

int CFmtConvertAlg::GetOutputCfg(int index, TStreamCfg *cfg)
{
	CTypeProp	tp;

	index = 0;
	cfg->width = (this->m_linewidth / tp.getVwc(8, 1, this->m_fmt)) << 3;
	cfg->height = this->m_height;
	cfg->fmt = this->m_fmt | (VIDEOCLASS << CLASSSHIFT);
	cfg->unit = this->m_unit;

	return 0;
}

void CFmtConvertAlg::ResetState(void)
{
	CImageAlgBase::ResetState();
}

int CFmtConvertAlg::Write(int index, char *buf, int len)
{
	index = index;		//dummy code, in case warning
	return this->m_linebuf.Write(buf, len);
}

int CFmtConvertAlg::Read(int index, char *buf, int len)
{
	if(index > 0)
		return 0;
	if(this->m_linebuf.IsFifoValid() == 0)
		return 0;
	if(len < this->m_linewidth)
		return 0;
	len = this->m_linebuf.Read(buf, len);
	this->m_linebuf.ClearPoint();
	this->m_outline++;
	return len;
}

void CFmtConvertAlg::SetOutFmt(int format)
{
	this->m_fmt = format;
}

int	CFmtConvertAlg::GetOutFmt(void)
{
	return this->m_fmt;
}

