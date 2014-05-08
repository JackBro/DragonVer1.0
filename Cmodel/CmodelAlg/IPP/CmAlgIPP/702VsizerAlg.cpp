
#include "internal.h"

C702VsizerAlg::C702VsizerAlg(void)
{
	this->m_pal = 0;
	this->m_inline = 0;
}

C702VsizerAlg::~C702VsizerAlg(void)
{
}

int C702VsizerAlg::IsFrameDone(void)
{
	if(CImageAlgBase::IsFrameDone())
		return 1;
	if(this->m_outline == this->m_dsth)
		return 1;
	return 0;
}

void C702VsizerAlg::SetInputCfg(int index, TStreamCfg *cfg)
{
	index = 0;
	this->m_width = cfg->width;
	this->m_height = cfg->height;
	this->m_unit = cfg->unit;
	this->m_linebuf[0].SetSize(this->m_width, 0, this->m_unit);
	this->m_linebuf[1].SetSize(this->m_width, 0, this->m_unit);
	this->m_plinebuf[0] = &(this->m_linebuf[0]);
	this->m_plinebuf[1] = &(this->m_linebuf[1]);
}

int C702VsizerAlg::GetOutputCfg(int index, TStreamCfg *cfg)
{
	index = 0;
	cfg->width = this->m_width;
	cfg->height = this->m_height;
	cfg->unit = this->m_unit;
	cfg->fmt = this->m_fmt;
	return 0;
}

void C702VsizerAlg::SetOutMode(int pal)
{
	this->m_pal = pal;
}

int C702VsizerAlg::GetOutMode(void)
{
	return this->m_pal;
}

void C702VsizerAlg::SetFactor(void)
{
	int i;
	
	for (i = 0; i < 2; i++)
		this->m_coeff5[i] = (int)((float)(1 << VER_COEFF_FMT) * (i + 1) / 5 + 0.5);

	this->m_dsth = (!this->m_pal) ? 480 :((this->m_height == 480) ? 600 : 576);
	this->m_pcoeff = NULL;
	this->m_inline = 0;
	this->m_phase = 0;
	if (this->m_height == 288)
	{
		if (this->m_pal == 1)
		{
			this->m_up = 2;
			this->m_down = 1;
		}
		else
		{
			this->m_up = 5;
			this->m_down = 3;
			this->m_pcoeff = this->m_coeff5;
		}
	}
	else
	{
		if (this->m_pal == 1)
		{
			this->m_up = 5; 
			this->m_down = 4;
			this->m_pcoeff = this->m_coeff5;
		}
		else
		{
			this->m_up = 1;
			this->m_down = 1;
		}
	}
}

void C702VsizerAlg::LoopLinebuf(void)
{
	CLine422Buf *p;

	p = this->m_plinebuf[0];
	this->m_plinebuf[0] = this->m_plinebuf[1];
	this->m_plinebuf[1] = p;
}

void C702VsizerAlg::ResetState(void)
{
	CImageAlgBase::ResetState();
}

int C702VsizerAlg::Write(int index, char *buf, int len)
{
	index = index;
	int size;
	
	if (this->m_inline == this->m_height)
	{
		this->m_plinebuf[1] = this->m_plinebuf[0];
		return 0;
	}
	else
	{
		size = this->m_plinebuf[1]->Write(buf, len);
		if (this->m_plinebuf[1]->IsFull() && size)
			this->m_inline++;
		return size;
	}
}

int C702VsizerAlg::Read(int index, char *buf, int len)
{
	index = index;
	int linesize, i, pos = 0;
	int val0, val1, tmp;

	linesize = this->m_width << 1;
	if ( (this->m_outline == this->m_dsth) || (!this->m_plinebuf[1]->IsFull()) )
		return 0;	
	while (this->m_phase < this->m_up)
	{
		if (this->m_phase == 0)
		{
			if (this->m_inline == 1)
			{
				memcpy(buf, this->m_plinebuf[1]->m_buf, linesize);				
				this->m_phase += this->m_down;
				this->m_plinebuf[1]->ClearPoint();
				this->LoopLinebuf();
				this->m_outline++;
				return linesize;	
			}
			else
			{
                memcpy(buf + pos, this->m_plinebuf[0]->m_buf, linesize);
			}
		}
		else if ((this->m_phase << 1) < this->m_up)
		{
			for (i = 0; i < linesize; i++)
			{
				val0 = (int)(this->m_plinebuf[0]->m_buf[i] & 0xff);
				val1 = (int)(this->m_plinebuf[1]->m_buf[i] & 0xff);
				tmp = this->GetValue(val1, val0, this->m_pcoeff[this->m_phase - 1], VER_COEFF_FMT);
				buf[pos + i] = (char)(tmp > 255 ? 255 : (tmp < 0 ? 0 : tmp));
			}
		}
		else if ((this->m_phase << 1) == this->m_up)
		{
			for (i = 0; i < linesize; i++)
			{
				val0 = (int)(this->m_plinebuf[0]->m_buf[i] & 0xff);
				val1 = (int)(this->m_plinebuf[1]->m_buf[i] & 0xff);
				buf[pos + i] = (char)((val0 + val1 + SIZER_RND) >> 1);
			}
		}
		else
		{
			for (i = 0; i < linesize; i++)
			{
				val0 = (int)(this->m_plinebuf[0]->m_buf[i] & 0xff);
				val1 = (int)(this->m_plinebuf[1]->m_buf[i] & 0xff);
				tmp = this->GetValue(val0, val1, this->m_pcoeff[this->m_up - this->m_phase - 1], VER_COEFF_FMT);
				buf[pos + i] = (char)(tmp > 255 ? 255 : (tmp < 0 ? 0 : tmp));
			}
		}
		this->m_phase += this->m_down;
		pos += linesize;
		this->m_outline++;
	}
	this->m_phase -= this->m_up;
	this->m_plinebuf[1]->ClearPoint();
	this->LoopLinebuf();

	return pos;	
}

// ret = val1 + ( ((val0 - val1) * coeff + 1 << (SIZER_COEFF_FMT - 1)) >> SIZER_COEFF_FMT )
int C702VsizerAlg::GetValue(int val0, int val1, int coeff, int precision)
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