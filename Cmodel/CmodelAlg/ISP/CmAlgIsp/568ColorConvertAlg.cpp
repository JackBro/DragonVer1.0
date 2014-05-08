#include "internal.h"


C568ColorConvertAlg::C568ColorConvertAlg(void)
{
	
}
C568ColorConvertAlg::~C568ColorConvertAlg(void)
{
}


int C568ColorConvertAlg::IsFrameDone(void)
{
	if(CImageAlgBase::IsFrameDone())
		return 1;
	if(this->m_outline == this->m_height)
		return 1;
	return 0;
}

void C568ColorConvertAlg::SetInputCfg(int index, TStreamCfg *cfg)
{
	index = 0;
	this->m_width = cfg->width;
	this->m_height = cfg->height;
	this->m_fmt = cfg->fmt;
	this->m_unit = cfg->unit;
	this->m_Linebuf.SetSize(this->m_width*3, 0, this->m_unit); //this->m_unit =1
}

int C568ColorConvertAlg::GetOutputCfg(int index, TStreamCfg *cfg)
{
	index = 0;
	cfg->width = this->m_width;
	cfg->height = this->m_height;
	cfg->unit = this->m_unit;
	cfg->fmt = YUV422_YUYV  | (VIDEOCLASS << CLASSSHIFT);

	return 0;
}


void C568ColorConvertAlg::ResetState(void) 
{
	CImageAlgBase::ResetState();
	this->m_Linebuf.ClearPoint();
}



C568ColorConvertAlg::Write(int index, char *buf, int len)
{
	index = index;

	if(len == 0)
		return 0;
	len = this->m_Linebuf.Write(buf, len);
	return len;
}

int C568ColorConvertAlg::Read(int index, char *buf, int len)
{
	index = index;

	int r1, g1, b1;
	int r2, g2, b2;
	int y1, u1, v1;
	int y2, u2, v2;
	int tmp = 0;
	int i;

	if (len < 1 || !m_Linebuf.IsFull()) return 0;
	len /= (3*this->m_unit);
	len = (this->m_width > len) ? len : this->m_width;

	for(i = 0; i < len; i+=2)
	{
		r1 = m_Linebuf.Read(3*i)&0xff;
		g1 = m_Linebuf.Read(3*i+1)&0xff;
		b1 = m_Linebuf.Read(3*i+2)&0xff;

		y1 = ((77 * r1)>>8) + ((150 * g1)>>8) + ((29 * b1)>>8);
		u1 = ((((128 * b1)>>7) - ((43 * r1)>>7) - ((85 * g1)>>7))>>1) + 128;
		v1 = ((((128 * r1)>>7) - ((107 * g1)>>7) - ((21 * b1)>>7))>>1) + 128;

		r2 = m_Linebuf.Read(3*i+3)&0xff;
		g2 = m_Linebuf.Read(3*i+4)&0xff;
		b2 = m_Linebuf.Read(3*i+5)&0xff;

		y2 = ((77 * r2)>>8) + ((150 * g2)>>8) + ((29 * b2)>>8);
		u2 = ((((128 * b2)>>7) - ((43 * r2)>>7) - ((85 * g2)>>7))>>1) + 128;
		v2 = ((((128 * r2)>>7) - ((107 * g2)>>7) - ((21 * b2)>>7))>>1) + 128;
			
		tmp = (u1+u2)>>1;
		*buf++ = (char)(y1 < 0 ? 0 : ((y1 > 255) ? 255 : y1));
		*buf++ = (char)(tmp < 0 ? 0 : ((tmp > 255) ? 255 : tmp));

		tmp = (v1+v2)>>1;
		*buf++ = (char)(y2 < 0 ? 0 : ((y2 > 255) ? 255 : y2));
		*buf++ = (char)(tmp < 0 ? 0 : ((tmp > 255) ? 255 : tmp));
	}
	this->m_outline ++;
	this->m_Linebuf.ClearPoint();
	len <<= 1;

	return len;
}

