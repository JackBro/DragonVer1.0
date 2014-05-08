#include "internal.h"


CYuv2yuvAlg::CYuv2yuvAlg(void)
{
	this->m_pline[0] = &this->m_line400[0];
	this->m_pline[1] = &this->m_line400[1];
}

CYuv2yuvAlg::~CYuv2yuvAlg(void)
{
}

int CYuv2yuvAlg::IsFrameDone(void)
{
	if(CImageAlgBase::IsFrameDone())
		return 1;
	if(this->m_outline == this->m_height)
		return 1;
	return 0;
}

void CYuv2yuvAlg::SetInputCfg(int index, TStreamCfg *cfg)
{
	index = 0;

	this->m_width = cfg->width;
	this->m_height = cfg->height;
	this->m_inpfmt = cfg->fmt;
	this->m_unit = cfg->unit;
	switch(this->m_inpfmt & FORMATMASK)
	{
	case YUV422_UYVY:
	case YUV422_VYUY:
	case YUV422_YUYV:
	case YUV422_YVYU:
	case YUV422_YUVY:
	case YUV422_YVUY:
	case YUV422_UVYY:
	case YUV422_VUYY:
	case YUV422_YYVU:
	case YUV422_YYUV:
		this->m_pline[0] = &this->m_line422[0];
		break;
	case YUV411_YYYYUV:
	case YUV411_YYYYVU:
	case YUV411_YUYVYY:
	case YUV411_YVYUYY:
	case YUV411_UVYYYY:
	case YUV411_VUYYYY:
		this->m_pline[0] = &this->m_line411[0];
		break;
	case YUV400:
	case EDGE_MAP:
	case RAW_Y:
		this->m_pline[0] = &this->m_line400[0];
		break;
	case YUV444:
		this->m_pline[0] = &this->m_line444[0];
		break;
	default:
		break;
	}
	this->m_pline[0]->SetSize(this->m_width, this->m_unit);
	this->m_pline[0]->SetFormat(this->m_inpfmt);
}

int CYuv2yuvAlg::GetOutputCfg(int index, TStreamCfg *cfg)
{
	index = 0;
	cfg->width = this->m_width;
	cfg->height = this->m_height;
	cfg->fmt = this->m_outfmt;
	cfg->unit = this->m_unit;
	switch(this->m_outfmt & FORMATMASK)
	{
	case YUV422_UYVY:
	case YUV422_VYUY:
	case YUV422_YUYV:
	case YUV422_YVYU:
	case YUV422_YUVY:
	case YUV422_YVUY:
	case YUV422_UVYY:
	case YUV422_VUYY:
	case YUV422_YYVU:
	case YUV422_YYUV:
		this->m_pline[1] = &this->m_line422[1];
		break;
	case YUV411_YYYYUV:
	case YUV411_YYYYVU:
	case YUV411_YUYVYY:
	case YUV411_YVYUYY:
	case YUV411_UVYYYY:
	case YUV411_VUYYYY:
		this->m_pline[1] = &this->m_line411[1];
		break;
	case YUV400:
	case EDGE_MAP:
	case RAW_Y:
		this->m_pline[1] = &this->m_line400[1];
		break;
	case YUV444:
		this->m_pline[1] = &this->m_line444[1];
		break;
	default:
		break;
	}
	this->m_pline[1]->SetFormat(this->m_outfmt);
	return 0;
}

int	CYuv2yuvAlg::GetOutFmt(void)
{
	return this->m_outfmt;
}

void CYuv2yuvAlg::SetOutFmt(int fmt)
{
	this->m_outfmt = fmt;
}

void CYuv2yuvAlg::ResetState(void)
{
	CImageAlgBase::ResetState();
}

int CYuv2yuvAlg::Write(int index, char *buf, int len)
{
	index = index;		//dummy code, in case warning
	return this->m_pline[0]->Write(buf, len);
}

int CYuv2yuvAlg::Read(int index, char *buf, int len)
{
	int i, yuv[12];
	CTypeProp tp;

	index = 0;
	if(tp.getVwc(this->m_width, 1, this->m_outfmt, this->m_unit) > len)
		return 0;
	if(this->m_pline[0]->IsFifoValid() == 0)
		return 0;
	if(this->m_inpfmt == this->m_outfmt)
		return this->m_pline[0]->Read(buf, len);
	this->m_pline[1]->SetSize(buf, this->m_width, this->m_unit);
	for(i = 0; i < this->m_width; i += 4)
	{
		this->m_pline[0]->Read(yuv);
		this->m_pline[1]->Write(yuv);
	}
	this->m_outline++;
	this->m_pline[0]->ClearPoint();
	return this->m_pline[1]->GetDataSize();
}

