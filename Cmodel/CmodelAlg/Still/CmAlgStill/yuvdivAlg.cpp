#include "internal.h"


CYuvdivAlg::CYuvdivAlg(void)
{
	this->m_pline = &this->m_line400;
	this->m_pu = &this->m_u444;
	this->m_pv = &this->m_u444;
	this->m_outfmt[0] = RAW_Y | (VIDEOCLASS << CLASSSHIFT);
}

CYuvdivAlg::~CYuvdivAlg(void)
{
}

int CYuvdivAlg::IsFrameDone(void)
{
	if(CImageAlgBase::IsFrameDone())
		return 1;
	if(this->m_outline == this->m_height)
		return 1;
	return 0;
}

void CYuvdivAlg::SetInputCfg(int index, TStreamCfg *cfg)
{
	index = 0;

	this->m_width = cfg->width;
	this->m_height = cfg->height;
	this->m_fmt = cfg->fmt;
	this->m_unit = cfg->unit;
	this->m_outfmt[0] = RAW_Y | (VIDEOCLASS << CLASSSHIFT);
	switch(this->m_fmt & FORMATMASK)
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
		this->m_pline = &this->m_line422;
		this->m_pu = &this->m_u422;
		this->m_pv = &this->m_v422;
		this->m_outfmt[1] = RAW_U422 | (VIDEOCLASS << CLASSSHIFT);
		this->m_outfmt[2] = RAW_V422 | (VIDEOCLASS << CLASSSHIFT);
		break;
	case YUV411_YYYYUV:
	case YUV411_YYYYVU:
	case YUV411_YUYVYY:
	case YUV411_YVYUYY:
	case YUV411_UVYYYY:
	case YUV411_VUYYYY:
		this->m_pline = &this->m_line411;
		this->m_pu = &this->m_u411;
		this->m_pv = &this->m_v411;
		this->m_outfmt[1] = RAW_U411 | (VIDEOCLASS << CLASSSHIFT);
		this->m_outfmt[2] = RAW_V411 | (VIDEOCLASS << CLASSSHIFT);
		break;
	case YUV400:
	case EDGE_MAP:
	case RAW_Y:
		this->m_pline = &this->m_line400;
		this->m_pu = &this->m_u444;
		this->m_pv = &this->m_v444;
		this->m_outfmt[1] = RAW_U444 | (VIDEOCLASS << CLASSSHIFT);
		this->m_outfmt[2] = RAW_V444 | (VIDEOCLASS << CLASSSHIFT);
		break;
	case YUV444:
		this->m_pline = &this->m_line444;
		this->m_pu = &this->m_u444;
		this->m_pv = &this->m_v444;
		this->m_outfmt[1] = RAW_U444 | (VIDEOCLASS << CLASSSHIFT);
		this->m_outfmt[2] = RAW_V444 | (VIDEOCLASS << CLASSSHIFT);
		break;
	case RAW_RGB:
		this->m_pline = &this->m_line444;
		this->m_pu = &this->m_u444;
		this->m_pv = &this->m_v444;
		this->m_outfmt[0] = RAW_R | (VIDEOCLASS << CLASSSHIFT);
		this->m_outfmt[1] = RAW_G | (VIDEOCLASS << CLASSSHIFT);
		this->m_outfmt[2] = RAW_B | (VIDEOCLASS << CLASSSHIFT);
		break;
	case RAW_RBG:
		this->m_pline = &this->m_line444;
		this->m_pu = &this->m_u444;
		this->m_pv = &this->m_v444;
		this->m_outfmt[0] = RAW_R | (VIDEOCLASS << CLASSSHIFT);
		this->m_outfmt[1] = RAW_B | (VIDEOCLASS << CLASSSHIFT);
		this->m_outfmt[2] = RAW_G | (VIDEOCLASS << CLASSSHIFT);
		break;
	case RAW_GBR:
		this->m_pline = &this->m_line444;
		this->m_pu = &this->m_u444;
		this->m_pv = &this->m_v444;
		this->m_outfmt[0] = RAW_G | (VIDEOCLASS << CLASSSHIFT);
		this->m_outfmt[1] = RAW_B | (VIDEOCLASS << CLASSSHIFT);
		this->m_outfmt[2] = RAW_R | (VIDEOCLASS << CLASSSHIFT);
		break;
	case RAW_GRB:
		this->m_pline = &this->m_line444;
		this->m_pu = &this->m_u444;
		this->m_pv = &this->m_v444;
		this->m_outfmt[0] = RAW_G | (VIDEOCLASS << CLASSSHIFT);
		this->m_outfmt[1] = RAW_R | (VIDEOCLASS << CLASSSHIFT);
		this->m_outfmt[2] = RAW_B | (VIDEOCLASS << CLASSSHIFT);
		break;
	case RAW_BGR:
		this->m_pline = &this->m_line444;
		this->m_pu = &this->m_u444;
		this->m_pv = &this->m_v444;
		this->m_outfmt[0] = RAW_B | (VIDEOCLASS << CLASSSHIFT);
		this->m_outfmt[1] = RAW_G | (VIDEOCLASS << CLASSSHIFT);
		this->m_outfmt[2] = RAW_R | (VIDEOCLASS << CLASSSHIFT);
		break;
	case RAW_BRG:
		this->m_pline = &this->m_line444;
		this->m_pu = &this->m_u444;
		this->m_pv = &this->m_v444;
		this->m_outfmt[0] = RAW_B | (VIDEOCLASS << CLASSSHIFT);
		this->m_outfmt[1] = RAW_R | (VIDEOCLASS << CLASSSHIFT);
		this->m_outfmt[2] = RAW_G | (VIDEOCLASS << CLASSSHIFT);
		break;
	default:
		break;
	}
	this->m_pline->SetSize(this->m_width, this->m_unit);
	this->m_pline->SetFormat(this->m_fmt);
}

int CYuvdivAlg::GetOutputCfg(int index, TStreamCfg *cfg)
{
	cfg->width = this->m_width;
	cfg->height = this->m_height;
	cfg->unit = this->m_unit;
	cfg->fmt = this->m_outfmt[index];
	this->m_yline.SetSize(this->m_width, this->m_unit);
	this->m_pu->SetSize(this->m_width, this->m_unit);
	this->m_pv->SetSize(this->m_width, this->m_unit);
	return 0;
}

void CYuvdivAlg::ResetState(void)
{
	CImageAlgBase::ResetState();
	this->m_lc[0] = this->m_lc[1] = this->m_lc[2] = 0;
}

int CYuvdivAlg::Write(int index, char *buf, int len)
{
	index = this->m_pline->Write(buf, len);
	return index;
}

int CYuvdivAlg::Read(int index, char *buf, int len)
{
	int i, yuv[12];
	CTypeProp tp;

	if(tp.getVwc(this->m_width, 1, this->m_fmt, this->m_unit) > len)
		return 0;
	if(this->m_pline->IsFifoValid())
	{
		for(i = 0; i < this->m_width; i += 4)
		{
			this->m_pline->Read(yuv);
			this->m_yline.Write(yuv);
			this->m_pu->Write(yuv);
			this->m_pv->Write(yuv);
		}
		this->m_pline->ClearPoint();
	}
	switch(index)
	{
	case 1:
		len = this->m_pu->Read(buf, len);
		if(len)
			this->m_lc[0]++;
		this->m_pu->ClearPoint();
		break;
	case 2:
		len = this->m_pv->Read(buf, len);
		if(len)
			this->m_lc[0]++;
		this->m_pv->ClearPoint();
		break;
	case 0:
	default:
		len = this->m_yline.Read(buf, len);
		if(len)
			this->m_lc[0]++;
		this->m_yline.ClearPoint();
		break;
	}
	this->m_outline = this->m_lc[0] > this->m_lc[1] ? this->m_lc[0] : this->m_lc[1];
	this->m_outline = this->m_outline > this->m_lc[2] ? this->m_outline : this->m_lc[2];

	return len;
}

