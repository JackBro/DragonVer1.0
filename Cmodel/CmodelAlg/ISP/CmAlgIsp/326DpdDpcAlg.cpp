

#include "internal.h"


#define InverseBit(x) (x) ? 0 : 1;

static int gTable[gTotalNoiseTable] = {
	0x10, 0x10, 0x10, 0x10, 0x10, 
	0x10, 0x10, 0x10, 0x10, 0x10,
	0x10, 0x10, 0x10, 0x10, 0x10, 
	0x10, 0x10
};

//==================== class C326DpdDpcAlg =========================

C326DpdDpcAlg::C326DpdDpcAlg()
{
	this->Init();
}

C326DpdDpcAlg::~C326DpdDpcAlg(void)
{
}

void C326DpdDpcAlg::Init(void)
{
	this->m_foreRead3line = 0;
	this->m_pos = 0;
	this->SetBGLine(0x1);
	this->SetGpixel(0x1);
	this->SetDpdThd(0xff);
	this->SetNoiseTable(gTable);
}


int C326DpdDpcAlg::IsFrameDone(void)
{
	if(CImageAlgBase::IsFrameDone())
		return 1;
	if(this->m_outline == this->m_height)
		return 1;
	return 0;
}

void C326DpdDpcAlg::SetInputCfg(int index, TStreamCfg *cfg)
{
	index = 0;
	this->m_width = cfg->width;
	this->m_height = cfg->height;
	this->m_fmt = cfg->fmt;
	this->m_unit = cfg->unit;
	this->SetSize(this->m_width, this->m_height, this->m_unit);

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

int C326DpdDpcAlg::GetOutputCfg(int index, TStreamCfg *cfg)
{
	index = 0;
	cfg->width = this->m_width;
	cfg->height = this->m_height;
	cfg->fmt = this->m_fmt;
	cfg->unit = this->m_unit;

	return 0;
}


void C326DpdDpcAlg::ResetState(void) 
{
	CImageAlgBase::ResetState();
	this->m_foreRead3line = 0;
	this->m_pos = 0;
	int i;
	for(i = 0; i < 5; i++)
	{
		this->m_pLinebuf[i] = &(this->m_Linebuf[i]);
		this->m_pLinebuf[i]->ClearPoint();
	}
}


void C326DpdDpcAlg::SetSize(int width, int height, int unit)
{
	int i;

	this->m_width = width;
	this->m_height = height;
	this->m_unit = unit;
	for(i = 0; i < 5; i++)
	{
		this->m_Linebuf[i].SetSize(width, 0, this->m_unit);
		this->m_pLinebuf[i] = &(this->m_Linebuf[i]);
	}
}

void C326DpdDpcAlg::SetNoiseTable(int *val) 
{
	int i;

	for(i = 0; i < gTotalNoiseTable; i++)
	{
		this->m_noisetable[i] = val[i]&0xff;
	}
}

int* C326DpdDpcAlg::GetNoiseTable(void)
{
	return this->m_noisetable;
}

void C326DpdDpcAlg::SetDpdThd(int thd)
{
	this->m_dpdthd = thd&0xff;
}

int C326DpdDpcAlg::GetDpdThd(void)
{
	return this->m_dpdthd;
}

void C326DpdDpcAlg::SetBGLine(int bgline)
{
	this->m_bgline = bgline&0x1;
}

void C326DpdDpcAlg::SetGpixel(int gpixel)
{
	this->m_gpixel = gpixel&0x1;
}

int C326DpdDpcAlg::GetBGLine()
{
	return this->m_bgline;
}

int C326DpdDpcAlg::GetGpixel()
{
	return this->m_gpixel;
}

int C326DpdDpcAlg::Write(int index, char *buf, int len)
{
	index = index;

	int nLength = 0;	
	if (this->m_foreRead3line < 3) 
	{
		nLength = this->m_pLinebuf[this->m_foreRead3line+2]->Write(buf, len);
	}
	else
	{
		nLength = this->m_pLinebuf[4]->Write(buf, len);
		
	}	
	return nLength;
}

int C326DpdDpcAlg::Read(int index, char *buf, int len)
{
	index = index;

	if (this->m_foreRead3line < 3 && this->m_pLinebuf[this->m_foreRead3line+2]->IsFull()) 
		m_foreRead3line++;
	if (this->m_foreRead3line < 3 && this->m_height > 3) return 0;

	if (len < 1 || !this->m_pLinebuf[2]->IsFull()) return 0;

	this->LastLinePro();
	len = this->DpdProcess(buf, len);
	if (len > 0) this->m_outline++;
	this->LoopLinebuf();
	this->m_pos %= this->m_width;
	return len; 
}

int C326DpdDpcAlg::DpdProcess(char *buf, int len)
{
	int p[5], gpixel, max = 255;

	if(this->m_unit == 2)
		max = 1024;
	len /= this->m_unit;
	len = ((this->m_width - this->m_pos) > len) ? len : (this->m_width - this->m_pos);
	if((this->m_outline < 2) || (this->m_outline > this->m_height-3))
	{
		memcpy(buf, this->m_pLinebuf[2]->m_buf, len*this->m_unit);
		this->m_pos = this->m_width;
		return len*this->m_unit;
	}

	gpixel = (this->m_gpixel + this->m_pos)%2;

	for(; this->m_pos < len; this->m_pos++)	
	{
		if((this->m_pos < 2) || (this->m_pos > this->m_width -3))
		{
			if(this->m_unit == 1)
				*((char*)buf+this->m_pos) = (char)this->m_pLinebuf[2]->Read(this->m_pos);
			else
				*((short*)buf+this->m_pos) = (short)this->m_pLinebuf[2]->Read(this->m_pos);

		}
		else
		{
			p[0] = this->m_pLinebuf[0]->Read(this->m_pos);
			p[1] = this->m_pLinebuf[2]->Read(this->m_pos-2);
			p[2] = this->m_pLinebuf[2]->Read(this->m_pos);
			p[3] = this->m_pLinebuf[2]->Read(this->m_pos+2);
			p[4] = this->m_pLinebuf[4]->Read(this->m_pos);
			if(this->m_unit == 1)
				*((char*)buf+this->m_pos) = (char)this->GetDpdNoise(p, gpixel);
			else
				*((short*)buf+this->m_pos) = (short)this->GetDpdNoise(p, gpixel);
		}
		gpixel = InverseBit(gpixel);
	}
	return this->m_pos*this->m_unit;
}


int C326DpdDpcAlg::GetDpdNoise(int *p, int gpixel)
{
	int DV, DH, thd, avg, var;
	int pixel;
	int i;

	pixel = p[2];
	if(this->m_unit == 1)
	{
		for(i = 0; i < 5; i++)
			p[i] <<= 2;
	}
	DV = abs((p[2] << 1) - p[0] - p[4]);
	DH = abs((p[2] << 1) - p[1] - p[3]);
	thd = this->m_dpdthd<<2;
	thd |= 0x3;

	if((DV > thd) && (DH > thd))
	{
		if(DV < DH)
			var = (p[0] + p[4]) >> 1;
		else
			var = (p[1] + p[3]) >> 1;
		if(this->m_unit == 1)
			return var>>2;
		else
			return var;
	}
	if (DH > DV)
	{
		avg = p[0] + p[2] + p[4];
		avg *= 21;
		avg >>= 6;

		var = abs(p[0] - avg) + abs(p[2] - avg) + abs(p[4] - avg);
		//var *= 21;
		//var >>= 6;
		//var /= 3;
		//avg = (pix0 + pix2 + pix4) / 3;
		//var = (abs(pix0 - avg) + abs(pix2 - avg) + abs(pix4 - avg)) / 3;
	}
	else
	{
		avg = p[1] + p[2] + p[3];
		avg *= 21;
		avg >>= 6;
		//avg /= 3;	

		var = abs(p[1] - avg) + abs(p[2] - avg) + abs(p[3] - avg);
		//var *= 21;
		//var >>= 6;
		//var /= 3;
		//avg = (pix0 + pix2 + pix4) / 3;
		//var = (abs(pix0 - avg) + abs(pix2 - avg) + abs(pix4 - avg)) / 3;
	}
	var >>= 2;		//noise compare is 8bit
	if(this->m_unit == 1)
		avg >>= 2;
	if(var < GetNoiseValue((p[2]>>2), gpixel))
		return avg;

	return pixel;
}




int C326DpdDpcAlg::GetNoiseValue(int p, int gpixel)
{
	int	x1;
	int	y1, y2;
	int	flag;
	int	result;

	flag = p >> 4;
	x1 = flag << 4;

	y1 = this->m_noisetable[flag];
	y2 = this->m_noisetable[flag+1];

	result = y2 - y1;
	result *= p - x1;
	result >>= 4;
	result += y1;
	result *= 3;
	
	return gpixel ? result : result >> 1;
}

void C326DpdDpcAlg::LastLinePro(void)
{
	if(this->m_pLinebuf[4]->IsFull())
		return;
	this->m_pLinebuf[4] = this->m_pLinebuf[2];
}

void C326DpdDpcAlg::LoopLinebuf(void)
{
	int i;
	CLineBayerBuf *p = this->m_pLinebuf[0];

	if(this->m_pos != this->m_width)
		return;
	for(i = 0; i < 4; i++)
		this->m_pLinebuf[i] = this->m_pLinebuf[i+1];
	this->m_pLinebuf[4] = p;
	this->m_pLinebuf[4]->ClearPoint();
	this->m_bgline = this->m_bgline ? 0 : 1;
	this->m_gpixel = this->m_gpixel ? 0 : 1;
	//this->m_outline++;
}



