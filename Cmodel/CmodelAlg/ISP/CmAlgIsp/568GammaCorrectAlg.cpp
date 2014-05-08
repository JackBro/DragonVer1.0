

#include "internal.h"

const int gDither10_8[][4] = {
	{ 1, 3, 1, 3 },
	{ 4, 2, 4, 2 },
	{ 1, 3, 1, 3 },
	{ 4, 2, 4, 2 }
};

static int rgamma[17] = {0x03, 0x13, 0x2b, 0x48, 0x62, 0x7b, 0x93, 0xa8, 0xba, 0xca, 0xd7, 0xe3, 0xec, 0xf4, 0xfa, 0xff, 0xff};
static int ggamma[17] = {0x03, 0x13, 0x2b, 0x48, 0x62, 0x7b, 0x93, 0xa8, 0xba, 0xca, 0xd7, 0xe3, 0xec, 0xf4, 0xfa, 0xff, 0xff};
static int bgamma[17] = {0x03, 0x13, 0x2b, 0x48, 0x62, 0x7b, 0x93, 0xa8, 0xba, 0xca, 0xd7, 0xe3, 0xec, 0xf4, 0xfa, 0xff, 0xff};

C568GammaCorrectAlg::C568GammaCorrectAlg()
{
	this->SetEnable(1);
	this->SetRGamma(rgamma);
	this->SetGGamma(ggamma);
	this->SetBGamma(bgamma);
	this->SetDither(0);
}

C568GammaCorrectAlg::~C568GammaCorrectAlg(void)
{
}

int C568GammaCorrectAlg::IsFrameDone(void)
{
	if(CImageAlgBase::IsFrameDone())
		return 1;
	if(this->m_outline == this->m_height)
		return 1;
	return 0;
}

void C568GammaCorrectAlg::SetInputCfg(int index, TStreamCfg *cfg)
{
	index = 0;
	this->m_width = cfg->width;
	this->m_height = cfg->height;
	this->m_fmt = cfg->fmt;
	this->m_unit = cfg->unit;
	this->m_Linebuf.SetSize(this->m_width*3, 0, this->m_unit);
}

int C568GammaCorrectAlg::GetOutputCfg(int index, TStreamCfg *cfg)
{
	index = 0;
	cfg->width = this->m_width;
	cfg->height = this->m_height;
	cfg->fmt = this->m_fmt;
	cfg->unit = 0x1;

	return 0;
}


void C568GammaCorrectAlg::ResetState(void) 
{
	CImageAlgBase::ResetState();
	this->m_Linebuf.ClearPoint();
	this->SetRGamma(rgamma);
	this->SetGGamma(ggamma);
	this->SetBGamma(bgamma);
}


void C568GammaCorrectAlg::SetEnable(int enable)
{
	this->m_enable = enable;
}

void C568GammaCorrectAlg::SetDither(int dither)
{
	this->m_dither = dither&0x1;
}

void C568GammaCorrectAlg::SetDitherValue(int val)
{
	this->m_dither_val = val;
}

void C568GammaCorrectAlg::SetRGamma(int *p)
{
	int i;

	for(i = 0; i < 17; i++)
		this->m_rgamma[i] = p[i]&0xff;
}

int* C568GammaCorrectAlg::GetRGamma(void)
{
	return this->m_rgamma;
}

void C568GammaCorrectAlg::SetBGamma(int *p)
{
	int i;

	for(i = 0; i < 17; i++)
		this->m_bgamma[i] = p[i]&0xff;
}

int* C568GammaCorrectAlg::GetBGamma(void)
{
	return this->m_bgamma;
}

void C568GammaCorrectAlg::SetGGamma(int *p)
{
	int i;

	for(i = 0; i < 17; i++)
		this->m_ggamma[i] = p[i]&0xff;
}

int* C568GammaCorrectAlg::GetGGamma(void)
{
	return this->m_ggamma;
}

int C568GammaCorrectAlg::Write(int index, char *buf, int len)
{
	index = index;

	if(len == 0)
		return 0;
	len = this->m_Linebuf.Write(buf, len);

	return len;
}

int C568GammaCorrectAlg::Read(int index, char *buf, int len)
{
	index = index;

	int i;
	if (len < 1 || !m_Linebuf.IsFull()) return 0;
	len /= (3*this->m_unit);
	len = (this->m_width > len) ? len : this->m_width;

	for(i = 0; i < len; i++)
	{
		m_r = m_Linebuf.Read(3*i);
		m_g = m_Linebuf.Read(3*i+1);
		m_b = m_Linebuf.Read(3*i+2);
		if (!this->m_enable)
		{
			if(this->m_unit == 2)
			{
				*buf++ = (char)((Dither(this->m_r)>>2)&0xff);
				*buf++ = (char)((Dither(this->m_g)>>2)&0xff);
				*buf++ = (char)((Dither(this->m_b)>>2)&0xff);
			}
			else
			{
				*buf++ = (char)(this->m_r&0xff);
				*buf++ = (char)(this->m_g&0xff);
				*buf++ = (char)(this->m_b&0xff);
			}
		}
		else
		{
			if(this->m_dither)
				this->SetDitherValue(gDither10_8[this->m_outline % 4][i % 4] - 2);

			if(this->m_unit == 2)		//10 bit data
			{
				*buf++ = (char)this->GetGammaValue(Dither(this->m_r), 0);
				*buf++ = (char)this->GetGammaValue(Dither(this->m_g), 1);
				*buf++ = (char)this->GetGammaValue(Dither(this->m_b), 2);
			}
			else
			{
				*buf++ = (char)this->GetGammaValue((this->m_r<<2), 0);
				*buf++ = (char)this->GetGammaValue((this->m_g<<2), 1);
				*buf++ = (char)this->GetGammaValue((this->m_b<<2), 2);
			}

		}
	}
	this->m_outline ++;
	this->m_Linebuf.ClearPoint();

	return len*3;
}

//before 10bits->8bits, add dither to 10bits data
int C568GammaCorrectAlg::Dither(int val)
{
	int x;
	x = (val&0x3ff) + (this->m_dither ? this->m_dither_val : 0);
	return (x > 1023) ? 1023 : ((x < 0) ? 0 : x);
}

/* 10 bit data input, if data is 8bit, left move 2bit */
int C568GammaCorrectAlg::GetGammaValue(int val, int rgb)
{
	int	x1;
	int	y1, y2;
	int	flag;
	int	result;
	flag = val >> 6;
	x1 = val&0x3f;

	if(rgb == 0)	    //r
	{
		y1 = this->m_rgamma[flag]&0xff;
		y2 = this->m_rgamma[flag+1]&0xff;
	}
	else if(rgb == 1)	//g
	{
		y1 = this->m_ggamma[flag]&0xff;
		y2 = this->m_ggamma[flag+1]&0xff;
	}
	else		         //b
	{
		y1 = this->m_bgamma[flag]&0xff;
		y2 = this->m_bgamma[flag+1]&0xff;
	}

	result = y2 - y1;
	result *= x1;
	result >>= 6;
	result += y1;

	return result > 255 ? 255 : result;
}


