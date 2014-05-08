
#include "internal.h"

C568LensFallOffAlg::C568LensFallOffAlg(void)
{
	this->Init();
}

C568LensFallOffAlg::~C568LensFallOffAlg(void)
{
}

void C568LensFallOffAlg::Init(void)
{
	this->m_lensCx = 0x140;
	this->m_lensCy = 0xf0;
	this->m_lensFocus = 0x43;
}


int C568LensFallOffAlg::IsFrameDone(void)
{
	if(CImageAlgBase::IsFrameDone())
		return 1;
	if(this->m_outline == this->m_height)
		return 1;
	return 0;
}

void C568LensFallOffAlg::SetInputCfg(int index, TStreamCfg *cfg)
{
	index = 0;
	this->m_width = cfg->width;
	this->m_height = cfg->height;
	this->m_fmt = cfg->fmt;
	this->m_unit = cfg->unit;
	this->m_Linebuf.SetSize(this->m_width, 0, this->m_unit);
}

int C568LensFallOffAlg::GetOutputCfg(int index, TStreamCfg *cfg)
{
	index = 0;
	cfg->width = this->m_width;
	cfg->height = this->m_height;
	cfg->fmt = this->m_fmt;
	cfg->unit = this->m_unit;

	return 0;
}


void C568LensFallOffAlg::ResetState(void) 
{
	CImageAlgBase::ResetState();
	this->m_Linebuf.ClearPoint();
}


int C568LensFallOffAlg::GetLensCx(void) const
{
	return this->m_lensCx;
}


int C568LensFallOffAlg::GetLensCy(void) const
{
	return this->m_lensCy;
}

int C568LensFallOffAlg::GetLensFocus(void) const
{
	return this->m_lensFocus;
}


void C568LensFallOffAlg::SetLensX(int x)
{
	this->m_lensCx = x&0xfff;

}
void C568LensFallOffAlg::SetLensY(int y)
{
	this->m_lensCy = y&0xfff;
}

void C568LensFallOffAlg::SetLensFocus(int f)
{
	this->m_lensFocus = f&0x1fff;
}

int C568LensFallOffAlg::Write(int index, char *buf, int len)
{
	index = index;		//dummy code, in case warning
	if(len == 0)
		return 0;
	len = this->m_Linebuf.Write(buf, len);
	return len;
}

int C568LensFallOffAlg::Read(int index, char *buf, int len)
{
	index = index;		//dummy code, in case warning

	int	val1, val2;
	int	nIndex;
	val2 = this->m_outline+1; //val2 = this->m_line+1;
	val2 -= this->m_lensCy;
	val2 *= val2;

	if (len < 1 || !m_Linebuf.IsFull()) return 0;

	len /= this->m_unit;
	len = (this->m_width > len) ? len : this->m_width;
	for(nIndex = 0; nIndex < len; nIndex++)
	{
		//fomula:
		//	(1+(x^2+y^2)/f^2) ^ 2
		val1 = nIndex+1;				//In hw, nIndex start from 1
		val1 -= this->m_lensCx;
		val1 *= val1;

		val1 += val2;
		val1 *= this->m_lensFocus;
		val1 >>= 18;	//because "1/f2<<24" change to "1/f2<<26", so ">>16" => ">>18"
		val1 += 256;
		val1 *= val1;
		val1 *= this->m_Linebuf.Read(nIndex);
		val1 >>= 16;
		if(this->m_unit == 1)
			*((char*)buf+nIndex) = (char)(val1 > 255 ? 255 : val1 > 0 ? val1 : 0);
		else
			*((short*)buf+nIndex) = (short)(val1 > 1023 ? 1023 : val1 > 0 ? val1 : 0);
	}
	if (len > 0) 
	{
		this->m_outline++;
		this->m_Linebuf.ClearPoint();
	}

	return len*this->m_unit;
}
