#include "internal.h"


CYuvMergeAlg::CYuvMergeAlg(void)
{
	this->m_pline = &this->m_line400;
	this->m_pu = &this->m_u444;
	this->m_pv = &this->m_u444;
	this->m_fmt = YUV444 | (VIDEOCLASS << CLASSSHIFT);
}

CYuvMergeAlg::~CYuvMergeAlg(void)
{
}

int CYuvMergeAlg::IsFrameDone(void)
{
	if(CImageAlgBase::IsFrameDone())
		return 1;
	if(this->m_outline == this->m_height)
		return 1;
	return 0;
}

void CYuvMergeAlg::SetInputCfg(int index, TStreamCfg *cfg)
{
	this->m_infmt[index] = cfg->fmt & FORMATMASK;
	this->m_width = cfg->width;
	this->m_height = cfg->height;
	this->m_unit = cfg->unit;
}

int CYuvMergeAlg::GetOutputCfg(int index, TStreamCfg *cfg)
{
	index = 0;
	if( ((this->m_infmt[1] == RAW_U444) && (this->m_infmt[2] != RAW_V444)) ||
		((this->m_infmt[1] == RAW_U422) && (this->m_infmt[2] != RAW_V422)) ||
		//((this->m_infmt[1] == RAW_U420) && (this->m_infmt[2] != RAW_V420)) ||
		((this->m_infmt[1] == RAW_U411) && (this->m_infmt[2] != RAW_V411)) )
		return 1;
	cfg->width = this->m_width;
	cfg->height = this->m_height;
	cfg->unit = this->m_unit;
	switch(this->m_infmt[1])
	{
	case RAW_U422:
		this->m_fmt = YUV422_UYVY;
		this->m_pline = &this->m_line422;
		this->m_pu = &this->m_u422;
		this->m_pv = &this->m_v422;
		break;

	case RAW_V411:
		this->m_fmt = YUV411_YYYYUV;
		this->m_pline = &this->m_line411;
		this->m_pu = &this->m_u411;
		this->m_pv = &this->m_v411;
		break;
	case RAW_U444:
	default:
		this->m_fmt = YUV444;
		this->m_pline = &this->m_line444;
		this->m_pu = &this->m_u444;
		this->m_pv = &this->m_v444;
		break;
	}
	cfg->fmt = this->m_fmt | (VIDEOCLASS << CLASSSHIFT);
	this->m_yline.SetSize(this->m_width, this->m_unit);
	this->m_pu->SetSize(this->m_width, this->m_unit);
	this->m_pv->SetSize(this->m_width, this->m_unit);
	return 0;
}

void CYuvMergeAlg::ResetState(void)
{
	CImageAlgBase::ResetState();
}

int CYuvMergeAlg::Write(int index, char *buf, int len)
{
	switch(index)
	{
	case 0:
		len = this->m_yline.Write(buf, len);
		break;
	case 1:
		len = this->m_pu->Write(buf, len);
		break;
	case 2:
		len = this->m_pv->Write(buf, len);
		break;
	default:
		len = 0;
		break;
	}
	return len;
}

int CYuvMergeAlg::Read(int index, char *buf, int len)
{
	CTypeProp tp;
	int i, yuv[12];

	index = 0;
	if(tp.getVwc(this->m_width, 1, this->m_fmt, this->m_unit) > len)
		return 0;
	if( (this->m_yline.IsFifoValid() == 0) ||
		(this->m_pu->IsFifoValid() == 0) ||
		(this->m_pv->IsFifoValid() == 0) )
		return 0;
	this->m_pline->SetSize(buf, this->m_width, this->m_unit);
	for(i = 0; i < this->m_width; i += 4)
	{
		this->m_yline.Read(yuv);
		this->m_pu->Read(yuv);
		this->m_pv->Read(yuv);
		this->m_pline->Write(yuv);
	}
	this->m_outline++;
	this->m_yline.ClearPoint();
	this->m_pu->ClearPoint();
	this->m_pv->ClearPoint();
	return this->m_pline->GetDataSize();
}

