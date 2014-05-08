

#include "internal.h"



#define InverseBit(x) (x) ? 0 : 1;

C568DigitalGainAlg::C568DigitalGainAlg()
{
	this->Init();
}

C568DigitalGainAlg::~C568DigitalGainAlg()
{

}

void C568DigitalGainAlg::Init(void)
{
	this->SetBGLine(0x1);
	this->SetGpixel(0x1);
	this->SetRGain(0x40);
	this->SetBGain(0x40);
	this->SetGGain(0x40);
	this->SetGlobalGain(0x40);
	this->SetGlobalEn(1);
	this->SetRgbEn(1);
	this->SetGainStep(0);
}

int C568DigitalGainAlg::IsFrameDone(void)
{
	if(CImageAlgBase::IsFrameDone())
		return 1;
	if(this->m_outline == this->m_height)
		return 1;
	return 0;
}

void C568DigitalGainAlg::SetInputCfg(int index, TStreamCfg *cfg)
{
	index = 0;
	this->m_width = cfg->width;
	this->m_height = cfg->height;
	this->m_fmt = cfg->fmt;
	this->m_unit = cfg->unit;
	this->m_Linebuf.SetSize(this->m_width, 0, this->m_unit);

	if (this->m_fmt == RAW_GB) 
	{
		this->SetBGLine(0x1);
		this->SetGpixel(0x1);
	}
	else
	{
		this->SetBGLine(0);
		this->SetGpixel(0);
	}
}

int C568DigitalGainAlg::GetOutputCfg(int index, TStreamCfg *cfg)
{
	index = 0;

	cfg->width = this->m_width;
	cfg->height = this->m_height;
	cfg->fmt = this->m_fmt;
	cfg->unit = this->m_unit;
	return 0;
}


void C568DigitalGainAlg::ResetState(void) 
{
	CImageAlgBase::ResetState();
	this->m_Linebuf.ClearPoint();
}


void C568DigitalGainAlg::SetBGLine(int bgline)
{
	this->m_bgline = bgline&0x1;
}

void C568DigitalGainAlg::SetGpixel(int gpixel)
{
	this->m_gpixel = gpixel&0x1;
}

int C568DigitalGainAlg::GetBGLine()
{
	return this->m_bgline;
}

int C568DigitalGainAlg::GetGpixel()
{
	return this->m_gpixel;
}

void C568DigitalGainAlg::SetRGain(int gain)
{
	this->m_rGain = gain&0xff;
}

int C568DigitalGainAlg::GetRGain(void)
{
	return this->m_rGain;
}

void C568DigitalGainAlg::SetBGain(int gain)
{
	this->m_bGain = gain&0xff;
}

int C568DigitalGainAlg::GetBGain(void)
{
	return this->m_bGain;
}

void C568DigitalGainAlg::SetGGain(int gain)
{
	this->m_gGain = gain&0xff;
}

int C568DigitalGainAlg::GetGGain(void)
{
	return this->m_gGain;
}

void C568DigitalGainAlg::SetGlobalGain(int gain)
{
	this->m_globalGain = gain&0xff;
}

int C568DigitalGainAlg::GetGlobalGain(void)
{
	return this->m_globalGain;
}

void C568DigitalGainAlg::SetGlobalEn(int val)
{
	this->m_globalEn = val&0x1;
}

int C568DigitalGainAlg::GetGlobalEn()
{
	return this->m_globalEn;
}

void C568DigitalGainAlg::SetRgbEn(int val)
{
	this->m_rgbEn = val&0x1;
}

int C568DigitalGainAlg::GetRgbEn()
{
	return this->m_rgbEn;
}

void C568DigitalGainAlg::SetGainStep(int step)
{
	this->m_gainStep = step&0xff;
}

int C568DigitalGainAlg::GetGainStep(void)
{
	return this->m_gainStep;
}

int C568DigitalGainAlg::Write(int index, char *buf, int len)
{
	index = index;

	if(len == 0)
		return 0;
	len = this->m_Linebuf.Write(buf, len);
	return len;
}

int C568DigitalGainAlg::Read(int index, char *buf, int len)
{
	index = index;

	int nIndex;
	int	temp, max = 255;
	int GlobalEn = 0, RgbEn = 0;
	int Gpixel = this->m_gpixel;
	int GlobalGain, rGain, gGain1, gGain2, bGain, rgbGain;

	if (len < 1 || !this->m_Linebuf.IsFull())  return 0;

	len /= this->m_unit;
	len = (this->m_width > len) ? len : this->m_width;

	if(this->m_unit == 2)
		max = 1023;
	GlobalEn = this->m_globalEn;
	RgbEn = this->m_rgbEn;
	GlobalGain = this->m_globalGain;
	switch(this->m_gainStep)
	{
	case 1:
		GlobalGain <<= 1;
		break;
	case 2:
		GlobalGain <<= 2;
		break;
	case 3:
		GlobalGain <<= 3;
		break;
	case 0:
	default:
		break;
	}
	rGain = this->m_rGain;
	gGain1 = this->m_gGain;
	gGain2 = this->m_gGain;
	bGain = this->m_bGain;

	for(nIndex = 0; nIndex < len; nIndex++)
	{
		temp = this->m_Linebuf.Read(nIndex);
		if(GlobalEn)
		{
			temp *= GlobalGain;
			temp >>= 6;
			if(temp > max)
				temp = max;
		}
		if(RgbEn)
		{
			if(Gpixel)
				rgbGain = this->m_bgline ? gGain1 : gGain2;
			else if(this->m_bgline)
				rgbGain = bGain;
			else
				rgbGain = rGain;
			temp *= rgbGain;
			temp >>= 6;
			if(temp > max)
				temp = max;
		}
		if(this->m_unit == 1)
			*((char*)buf+nIndex) = (char)temp;
		else
			*((short*)buf+nIndex) = (short)temp;
		Gpixel = InverseBit(Gpixel);
	}
	this->m_bgline = InverseBit(this->m_bgline);
	this->m_gpixel = InverseBit(this->m_gpixel);

	if (len > 0) 
	{
		this->m_Linebuf.ClearPoint();
		this->m_outline ++;
	}

	return len*this->m_unit;
}




