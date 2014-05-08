
#include "internal.h"

CMpeg2IBase::CMpeg2IBase()
{
}

void CMpeg2IBase::Open(void)
{
	this->m_Ydc = this->m_Udc = this->m_Vdc = 0;
}

void CMpeg2IBase::CheckOption(void)
{
	this->m_vinc = 8;
	this->m_option.mb_width = (this->m_option.srcwidth + 15) >> 4;
	this->m_option.mb_height = (this->m_option.progseq) ? ((this->m_option.srcheight + 15) >> 4) : ((this->m_option.srcheight + 31) >> 4);
	switch(this->m_option.chroma)
	{
	case CHROMT_422:
		this->m_option.srcheight = ((this->m_option.srcheight + 7) >> 3) << 3;
		this->m_option.srcwidth = ((this->m_option.srcwidth + 15) >> 4) << 4;
		this->m_horwidth = this->m_option.srcwidth << 4;
		this->m_option.vwc = this->m_option.srcwidth * this->m_option.srcheight * 2;
		break;
	case CHROMT_444:
		this->m_option.srcheight = ((this->m_option.srcheight + 7) >> 3) << 3;
		this->m_option.srcwidth = ((this->m_option.srcwidth + 7) >> 3) << 3;
		this->m_horwidth = this->m_option.srcwidth * 24;
		this->m_option.vwc = this->m_option.srcwidth * this->m_option.srcheight * 3;
		break;
	case CHROMT_420:
		this->m_option.srcheight = ((this->m_option.srcheight + 15) >> 4) << 4;
		this->m_option.srcwidth = ((this->m_option.srcwidth + 15) >> 4) << 4;
		this->m_vinc = 16;
		this->m_horwidth = (this->m_option.srcwidth << 4) + (this->m_option.srcwidth << 3);
		this->m_option.vwc = (this->m_option.srcwidth * this->m_option.srcheight * 3) >> 1;
		break;
	case CHROMT_FMTRES:
	default:
		return;
	}
}

int	CMpeg2IBase::getVwc(void)
{
	int vwc = 0, cx, cy;

	switch(this->m_option.chroma)
	{
	case CHROMT_422:
		cx = ((this->m_option.srcwidth + 15) >> 4) << 4;
		cy = ((this->m_option.srcheight + 7) >> 3) << 3;
		vwc = (cx * cy) << 1;
		break;
	case CHROMT_420:
		cx = ((this->m_option.srcwidth + 15) >> 4) << 4;
		cy = ((this->m_option.srcheight + 15) >> 4) << 4;
		vwc = (cx * cy * 3) >> 1;
		break;
	case CHROMT_444:
		cx = ((this->m_option.srcwidth + 7) >> 3) << 3;
		cy = ((this->m_option.srcheight + 7) >> 3) << 3;
		vwc = cx * cy * 3;
		break;
	default:
		break;
	}
	return vwc;
}
