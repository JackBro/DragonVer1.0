
#include "internal.h"

C702HsizerAlg::C702HsizerAlg(void)
{
}

C702HsizerAlg::~C702HsizerAlg(void)
{
}

int C702HsizerAlg::IsFrameDone(void)
{
	if(CImageAlgBase::IsFrameDone())
		return 1;
	if(this->m_outline == this->m_height)
		return 1;
	return 0;
}

void C702HsizerAlg::SetInputCfg(int index, TStreamCfg *cfg)
{
	index = 0;
	this->m_width = cfg->width;
	this->m_height = cfg->height;
	this->m_unit = cfg->unit;
	this->m_linebuf.SetSize(this->m_width, 0, this->m_unit);
}

int C702HsizerAlg::GetOutputCfg(int index, TStreamCfg *cfg)
{
	index = 0;
	cfg->width = this->m_width;
	cfg->height = this->m_height;
	cfg->unit = this->m_unit;
	cfg->fmt = this->m_fmt;

	return 0;
}

void C702HsizerAlg::SetFactor(void)
{
	int i;

	for (i = 0; i < 4; i++)
		this->m_coeff9[i] = (int)((float)(1 << HOR_COEFF_FMT) * (i + 1) / 9 + 0.5);
	for (i = 0; i < 22; i++)
		this->m_coeff45[i] = (int)((float)(1 << HOR_COEFF_FMT) * (i + 1) / 45 + 0.5);

	this->m_pcoeff = NULL;
	if (this->m_width == 352)
	{
		this->m_up = 45;
		this->m_down = 22;
		this->m_pcoeff = this->m_coeff45;
	}
	else
	{
		this->m_up = 9;
		this->m_down = 8;
		this->m_pcoeff = this->m_coeff9;
	}
}

void C702HsizerAlg::ResetState(void)
{
	CImageAlgBase::ResetState();
}

int C702HsizerAlg::Write(int index, char *buf, int len)
{
	index = index;

	return this->m_linebuf.Write(buf, len);
}

int C702HsizerAlg::Read(int index, char *buf, int len)
{
	index = index;
	int i, j;
	int tmp, dat0[3], dat1[3];
	int pos = 0, ind = 0;

	if ( (len < 1) || (!this->m_linebuf.IsFull()) )
		return 0;
	this->m_phase = 0;
	this->m_linebuf.Read(pos, dat0);
	pos++;
	this->m_linebuf.Read(pos, dat1);
	pos++;
	for (i = 0; i < 720; i++, ind += 3)
	{
		while (this->m_phase >= this->m_up)
		{
			this->m_phase -= this->m_up;			
			dat0[0] = dat1[0];
			dat0[1] = dat1[1];
			dat0[2] = dat1[2];
			this->m_linebuf.Read(pos, dat1);
			pos++;
		}
		if (this->m_phase == 0)
		{
			for (j = 0; j < 3; j++)
				buf[ind + j] = (char)dat0[j];
		}
		else if ((this->m_phase << 1) < this->m_up)
		{
			for (j = 0; j < 3; j++)
			{
				tmp = this->GetValue(dat1[j], dat0[j], this->m_pcoeff[this->m_phase - 1], HOR_COEFF_FMT);
				buf[ind + j] = (char)(tmp > 255 ? 255 : (tmp < 0 ? 0 : tmp));
			}
		}
		else if ((this->m_phase << 1) == this->m_up)
		{
			for (j = 0; j < 3; j++)
			{				
				buf[ind + j] = (char)((dat0[j] + dat1[j] + SIZER_RND) >> 1);
			}
		}
		else
		{
			for (j = 0; j < 3; j++)
			{
				tmp = this->GetValue(dat0[j], dat1[j], this->m_pcoeff[this->m_up - this->m_phase - 1], HOR_COEFF_FMT);
				buf[ind + j] = (char)(tmp > 255 ? 255 : (tmp < 0 ? 0 : tmp));
			}
		}
		this->m_phase += this->m_down;
	}
	return ind;
}

// ret = val1 + ( ((val0 - val1) * coeff + 1 << (SIZER_COEFF_FMT - 1)) >> SIZER_COEFF_FMT )
int C702HsizerAlg::GetValue(int val0, int val1, int coeff, int precision)
{
	int ret;

	if (val0 > val1)
	{
		ret = val1 + ( ((val0 - val1) * coeff + (SIZER_RND << (precision - 1))) >> precision );
	}
	else
	{
		ret = val1 - ( ((val1 - val0) * coeff + (SIZER_RND << (precision - 1))) >> precision );
	}
	return ret;
}