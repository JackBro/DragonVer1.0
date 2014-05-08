
#include "internal.h"

C568EdgeEnhanceAlg::C568EdgeEnhanceAlg()
{
}

C568EdgeEnhanceAlg::~C568EdgeEnhanceAlg(void)
{
}

int C568EdgeEnhanceAlg::IsFrameDone(void)
{
	if(CImageAlgBase::IsFrameDone())
		return 1;
	if(this->m_outline == this->m_height)
		return 1;
	return 0;
}

void C568EdgeEnhanceAlg::SetInputCfg(int index, TStreamCfg *cfg)
{
	index = 0;
	this->m_width = cfg->width;
	this->m_height = cfg->height;
	this->m_fmt = cfg->fmt;
	this->m_unit = cfg->unit;
	this->m_Linebuf.SetSize(this->m_width<<1, 0, this->m_unit);
	this->m_EdgeLinebuf.SetSize(this->m_width<<1, 0, this->m_unit);
}

int C568EdgeEnhanceAlg::GetOutputCfg(int index, TStreamCfg *cfg)
{
	index = 0;
	cfg->width = this->m_width;
	cfg->height = this->m_height;
	cfg->fmt =  YUV422_YUYV | (VIDEOCLASS << CLASSSHIFT);;
	cfg->unit = this->m_unit;

	return 0;
}


void C568EdgeEnhanceAlg::ResetState(void) 
{
	CImageAlgBase::ResetState();
	this->m_Linebuf.ClearPoint();
	this->m_EdgeLinebuf.ClearPoint();
}



int C568EdgeEnhanceAlg::Write(int index, char *buf, int len)
{
	if(len == 0)
		return 0;
	switch(index) 
	{
	case 0:
		len = this->m_Linebuf.Write(buf, len);
		break;
	case 1:
		len = this->m_EdgeLinebuf.Write(buf, len);
		break;
	default:
		len = this->m_Linebuf.Write(buf, len);
	}
	
	return len;
}

int C568EdgeEnhanceAlg::Read(int index, char *buf, int len)
{
	index = index;

	int i;
	int yValue, uvValue,edgeValue;
	int flag = 0;

	if (len < 1 || !m_Linebuf.IsFull()) return 0;
	if (len < 1 || !m_EdgeLinebuf.IsFull()) return 0;
	len /= (this->m_unit);
	len = (this->m_width > len) ? len : this->m_width;

	for(i = 0; i < len; i++)
	{
		yValue = m_Linebuf.Read(2*i)&0xff;
		uvValue = m_Linebuf.Read(2*i+1)&0xff;

		edgeValue = ((m_EdgeLinebuf.Read(2*i+1)<<8) + m_EdgeLinebuf.Read(2*i))&0x1ff;

		flag = edgeValue&0x100;
		if(flag)
			yValue -= ((~edgeValue+1)&0xff);
		else
			yValue += (edgeValue&0xff);

		*buf++ = (char)(yValue > 255 ? 255 : (yValue < 0 ? 0 : (char)yValue));
		*buf++ = (char)(uvValue);
	}
	this->m_outline ++;
	this->m_Linebuf.ClearPoint();
	this->m_EdgeLinebuf.ClearPoint();

	return len << 1;
}

