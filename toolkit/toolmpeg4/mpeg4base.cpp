

#include "internal.h"

CMpeg4Base::CMpeg4Base()
{
/*	if (this->m_mbs != NULL)
	{
		delete [](this->m_mbs);
		this->m_mbs = NULL;
	}
*/
	this->m_mbs = new TMacroBlock[DEFAULT_MBS];
}

CMpeg4Base::~CMpeg4Base()
{
	if (this->m_mbs != NULL)
	{
		delete [](this->m_mbs);
		this->m_mbs = NULL;
	}
}

void CMpeg4Base::Open(void)
{
	this->m_horwidth = 0;
	this->m_Ydc = this->m_Udc = this->m_Vdc = 0;
	memset(&(this->m_option), 0, sizeof(TMpeg4Option));
}

int CMpeg4Base::GetVwc(void)
{
	int vwc = 0, cx, cy;

	if (this->m_option.chroma == CHROMA_420)
	{
		cx = ((this->m_option.srcwidth + 15) >> 4) << 4;
		cy = ((this->m_option.srcheight + 15) >> 4) << 4;
		vwc = (cx * cy * 3) >> 1;
	}
	return vwc;
}

void CMpeg4Base::CheckOption(void)
{
	if (this->m_option.chroma == CHROMA_420)
	{		
		this->m_option.srcwidth = ((this->m_option.srcwidth + 15) >> 4) << 4;
		this->m_option.srcheight = ((this->m_option.srcheight + 15) >> 4) << 4;
		this->m_option.mb_width = this->m_option.srcwidth >> 4;
		this->m_option.mb_height = this->m_option.srcheight >> 4;
		this->m_option.edgedwidth = this->m_option.srcwidth + (32 << 1);
		this->m_option.edgedheight = this->m_option.srcheight + (32 << 1);
		this->m_vinc = 16;
		this->m_horwidth = (this->m_option.srcwidth << 4) + (this->m_option.srcwidth << 3);
		this->m_option.vwc = (this->m_option.srcwidth * this->m_option.srcheight * 3) >> 1;
	}
	return;
}

