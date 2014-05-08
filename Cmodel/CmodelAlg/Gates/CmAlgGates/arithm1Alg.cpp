#include "internal.h"


CArithm1Alg::CArithm1Alg(void)
{
	this->m_UpClip = 255;
	this->m_LowClip = -256;
	this->m_opdata = 0;
	this->m_operator = OP_ADD;
}

CArithm1Alg::~CArithm1Alg(void)
{
}

int CArithm1Alg::IsFrameDone(void)
{
	if(CImageAlgBase::IsFrameDone())
		return 1;
	if(this->m_outline == this->m_height)
		return 1;
	return 0;
}

void CArithm1Alg::SetInputCfg(int index, TStreamCfg *cfg)
{
	CTypeProp tp;

	index = 0;
	this->m_width = cfg ->width;
	this->m_height = cfg->height;
	this->m_fmt = cfg->fmt;
	this->m_InUnit = cfg->unit;
	this->m_linewidth = tp.getLinewidth(this->m_width ,this->m_fmt, 1);
	this->m_linebuf[0].SetSize(this->m_linewidth, this->m_InUnit);
	
}

int CArithm1Alg::GetOutputCfg(int index, TStreamCfg *cfg)
{
	index = 0;
	cfg->width = this->m_width;
	cfg->height = this->m_height;
	cfg->fmt = this->m_fmt;
	cfg->unit = this->m_OutUnit;
	return 0;
}

void CArithm1Alg::ResetState(void)
{
	CImageAlgBase::ResetState();
}

int CArithm1Alg::Write(int index, char *buf, int len)
{
	ASSERT((index == 0));
	return this->m_linebuf[0].Write(buf, len);
}

int CArithm1Alg::Read(int index, char *buf, int len)
{
	int i, val1;
	
	ASSERT((index == 0));
	if (len < this->m_linewidth * this->m_OutUnit)
		return 0;

	if( (this->m_linebuf[0].IsFifoValid()))
	{
		this->m_linebuf[1].SetSize(buf, this->m_linewidth, this->m_OutUnit);
		for(i = 0; i < this->m_linewidth; i++)
		{
			val1 = this->m_linebuf[0].Read();
		
			switch(this->m_operator)
			{
			case OP_ADD:
				val1 += this->m_opdata;
				break;
			case OP_SUB:
				val1 -= this->m_opdata;
				break;
			case OP_MULTIPLY:
				val1 *= this->m_opdata;
				break;
			case OP_DIV:
				if (this->m_opdata == 0)
					return 0;
				val1 /= this->m_opdata;
				break;
			default:
				break;
			}
			
			val1 = (val1 > this->m_UpClip) ? this->m_UpClip : val1;
			val1 = (val1 < this->m_LowClip) ? this->m_LowClip: val1;
			this->m_linebuf[1].Write(val1);
		}
		if(this->m_linebuf[0].IsFifoEmpty())
			this->m_outline++;

		return this->m_linewidth  * this->m_OutUnit;
	}
	
	return 0;
}

void CArithm1Alg::SetUpClip(int val)
{
	this->m_UpClip = val;
}

int	CArithm1Alg::GetUpClip(void)
{
	return this->m_UpClip;
}

void CArithm1Alg::SetLowClip(int val)
{
	this->m_LowClip = val;
}

int	CArithm1Alg::GetLowClip(void)
{
	return this->m_LowClip;
}

void CArithm1Alg::SetInUnit(int val)
{
	this->m_InUnit = val;
}

int	CArithm1Alg::GetInUnit(void)
{
	return this->m_InUnit;
}

void CArithm1Alg::SetOutUnit(int val)
{
	this->m_OutUnit = val;
}

int	CArithm1Alg::GetOutUnit(void)
{
	return this->m_OutUnit;
}

void CArithm1Alg::SetOpdata(int val)
{
	this->m_opdata = val;
}

int	CArithm1Alg::GetOpdata(void)
{
	return this->m_opdata;
}

void CArithm1Alg::SetOperator(int val)
{
	this->m_operator = val;
}

int CArithm1Alg::GetOperator(void)
{
	return this->m_operator;
}