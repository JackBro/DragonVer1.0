

#include "internal.h"

//==================== class CIspModule ===================

CIspModule::CIspModule(int width, int height, int unit)
{
	this->SetIspCtrl(0xffff);
	this->SetBGLine(0x1);
	this->SetGpixel(0x1);
	this->m_width = width;
	this->m_height = height;
	this->m_unit = unit;
}

void CIspModule::SetIspCtrl(int ctrl)
{
	this->ispctrl = ctrl;
}

int CIspModule::GetIspCtrl(void)
{
	return this->ispctrl;
}

void CIspModule::SetBGLine(int bgline)
{
	this->bgline = bgline&0x1;
}

int CIspModule::GetBGLine(void)
{
	return this->bgline&0x1;
}

void CIspModule::SetGpixel(int gpixel)
{
	this->gpixel = gpixel&0x1;
}

int CIspModule::GetGpixel(void)
{
	return this->gpixel&0x1;
}

void CIspModule::SetIspModuleEn(int flag, int en)
{
	int ispctrl;

	ispctrl = this->GetIspCtrl();
	if(en)
		this->SetIspCtrl(ispctrl|flag);
	else
		this->SetIspCtrl(ispctrl&(~(flag)));
}

int CIspModule::Process(char *src, char *dst)
{
	int ispCtrl;
	int size, flag;
	CBufferBase edgeBuf;

	ispCtrl = this->GetIspCtrl();
	size = this->m_width* this->m_height*this->m_unit;
	memcpy(dst, src, size);
	//blacklevel->dpd->lensfalloff->digitalgain->cfa->colorcorrect->gamma correct->hsv->histo->rgb2yuv->edgeenhance
	if(ispCtrl&(BLACKLEVEL))	//enable blacklevel
	{
		memcpy(src, dst, size);
		CIspBlackLevel ispBlack(this->m_width, this->m_height, this->m_unit);
		//ispBlack.SetBOffset(0x10);
		//ispBlack.SetG1Offset(0x10);
		//ispBlack.SetG2Offset(0x10);
		//ispBlack.SetROffset(0x10);
		ispBlack.Process(src, dst);
	}
	if(ispCtrl&(DPDDPC))	//enable dpd
	{
		memcpy(src, dst, size);
		CIspDpdDpc ispDpd(this->m_width, this->m_height, this->m_unit);
		ispDpd.Process(src, dst);
	}
	if(ispCtrl&(LENSFALLOFF))	//enable LENSFALLOFF
	{
		memcpy(src, dst, size);
		CIspLensFallOff ispLens(this->m_width, this->m_height, this->m_unit);
		ispLens.Process(src, dst);
	}
	if(ispCtrl&(DIGITALGAIN))	//enable DIGITALGAIN
	{
		memcpy(src, dst, size);
		CIspDigitalGain ispGain(this->m_width, this->m_height, this->m_unit);
		ispGain.SetGlobalEn(1);
		ispGain.SetRgbEn(1);
		ispGain.Process(src, dst);
	}
	memcpy(src, dst, size);
	CIspCfaExtract ispCfa(this->m_width, this->m_height, 0, this->m_unit);
	ispCfa.Process(src, dst);
	if(ispCtrl&(EDGEENHANCE))	//enable EDGEENHANCE
	{
		edgeBuf.SetSize(size/this->m_unit, 1);
		CIspCfaExtract ispEdge(this->m_width, this->m_height, 1, this->m_unit);
		ispEdge.Process(src, edgeBuf.m_buf);
	}
	if(ispCtrl&(COLORCORRECT))	//enable COLORCORRECT
	{
		memcpy(src, dst, size*3);
		CIspColorCorrect ispColorCorrect(this->m_width, this->m_height, this->m_unit);
		ispColorCorrect.Process(src, dst);
	}
	if(ispCtrl&(GAMMACORRECT))	//enable GAMMACORRECT
		flag = 1;
	else
		flag = 0;
	memcpy(src, dst, size*3);
	CIspGammaCorrect ispGammaCorrect(this->m_width, this->m_height, this->m_unit, flag);
	ispGammaCorrect.Process(src, dst);
	
	memcpy(src, dst, size*3/this->m_unit);
	CIspColorConvert ispColorConvert(this->m_width, this->m_height);
	ispColorConvert.Process(src, dst);
	if(ispCtrl&(EDGEENHANCE))	//enable EDGEENHANCE
	{
		memcpy(src, dst, size*2/this->m_unit);
		CIspEdgeEnhance ispEdgeEnhance(this->m_width, this->m_height);
		ispEdgeEnhance.Process(src, edgeBuf.m_buf, dst);
	}
	//return 3*size/this->m_unit;
	return size*2/this->m_unit;
}

//==================== class CIspTestBar =====================

int CIspTestBar::Write(char *buf, int len)
{
	buf = buf;
	len = len;
	
	return 0;
}
int CIspTestBar::Read(char *buf, int len)
{
	buf = buf;
	len = len;

	return 0;
}

//==================== class CIspBlackLevel =========================

CIspBlackLevel::CIspBlackLevel(int width, int height, int unit)
{
	this->m_rOffset = 0;
	this->m_g1Offset = 0;
	this->m_g2Offset = 0;
	this->m_bOffset = 0;
	this->m_bgline = 1;
	this->m_gpixel = 1;
	this->m_width = width;
	this->m_height = height;
	this->m_unit = unit;
	this->m_Linebuf.SetSize(width, 0, unit);
}

void CIspBlackLevel::SetBGLine(int bgline)
{
	this->m_bgline = bgline&0x1;
}

int CIspBlackLevel::GetBGLine(void)
{
	return this->m_bgline&0x1;
}

void CIspBlackLevel::SetGpixel(int gpixel)
{
	this->m_gpixel = gpixel&0x1;
}

int CIspBlackLevel::GetGpixel(void)
{
	return this->m_gpixel&0x1;
}

void CIspBlackLevel::SetROffset(int roffset)
{
	this->m_rOffset = roffset&0x3ff;
}

void CIspBlackLevel::SetG1Offset(int g1offset)
{
	this->m_g1Offset = g1offset&0x3ff;
}

void CIspBlackLevel::SetG2Offset(int g2offset)
{
	this->m_g2Offset = g2offset&0x3ff;
}

void CIspBlackLevel::SetBOffset(int boffset)
{
	this->m_bOffset = boffset&0x3ff;
}

int CIspBlackLevel::GetROffset(void)
{
	return this->m_rOffset;
}

int CIspBlackLevel::GetG1Offset(void)
{
	return this->m_g1Offset;
}

int CIspBlackLevel::GetG2Offset(void)
{
	return this->m_g2Offset;
}

int CIspBlackLevel::GetBOffset(void)
{
	return this->m_bOffset;
}

int CIspBlackLevel::Write(char *buf, int len)
{
	this->m_Linebuf.ClearPoint();
	len = this->m_Linebuf.Write(buf, len);
	
	return len;
}

int CIspBlackLevel::Read(char *buf, int len)
{
	int	minVal = 0, val;
	int	k;
	int	temp;

	temp = this->m_gpixel;
	for(k = 0; k < len; k++)
	{
		if(this->m_gpixel)
			minVal = this->m_bgline ? this->m_g1Offset : this->m_g2Offset;
		else if(this->m_bgline)
			minVal = this->m_bOffset;
		else
			minVal = this->m_rOffset;
		if(minVal&0x200)
			minVal = -(minVal&0x1ff);
		val = this->m_Linebuf.Read(k);
		if(this->m_unit == 2)
			*((short*)buf+k) = (short)((val > minVal) ? ((val-minVal) > 1023 ? 1023 : val-minVal) : 0);
		else
			*((char*)buf+k) = (char)((val > minVal) ? ((val-minVal) > 255 ? 255 : val-minVal) : 0);
		this->m_gpixel = InverseBit(this->m_gpixel);
	}
	this->m_bgline = InverseBit(this->m_bgline);
	this->m_gpixel = InverseBit(temp);

	return k;
}

int CIspBlackLevel::Process(char *src, char *dst)
{
	int i;
	int len;

	for(i = 0; i < this->m_height; i++)
	{
		len = this->Write(src+i*this->m_width*this->m_unit, this->m_width);
		len = this->Read(dst+i*this->m_width*this->m_unit, this->m_width);
	}
	return i;
}

//==================== class CIspLensFallOff =========================

CIspLensFallOff::CIspLensFallOff(int width, int height, int unit)
{
	this->m_lensCx = 0x140;
	this->m_lensCy = 0xf0;
	this->m_lensFocus = 0x43;
	this->m_line = 0;
	this->m_unit = unit;
	this->m_width = width;
	this->m_height = height;
	this->m_Linebuf.SetSize(width, 0, unit);
}

int CIspLensFallOff::GetLensFocus(void)
{
	return this->m_lensFocus;
}

int CIspLensFallOff::GetLensX(void)
{
	return this->m_lensCx;
}

int CIspLensFallOff::GetLensY(void)
{
	return this->m_lensCy;
}

void CIspLensFallOff::SetLensFocus(int f)
{
	this->m_lensFocus = f&0x1fff;
}

void CIspLensFallOff::SetLensX(int x)
{
	this->m_lensCx = x&0xfff;
}

void CIspLensFallOff::SetLensY(int y)
{
	this->m_lensCy = y&0xfff;
}

int CIspLensFallOff::Write(char *buf, int len)
{
	this->m_Linebuf.ClearPoint();
	len = this->m_Linebuf.Write(buf, len);

	return len;
}

int CIspLensFallOff::Read(char *buf, int len)
{
	int	val1, val2;
	int	k;
	
	val2 = this->m_line;
	val2 -= this->m_lensCy;
	val2 *= val2;

	for(k = 0; k < len; k++)
	{
		//fomula:
		//	(1+(x^2+y^2)/f^2) ^ 2
		val1 = k;
		val1 -= this->m_lensCx;
		val1 *= val1;

		val1 += val2;
		val1 *= this->m_lensFocus;
		val1 >>= 16;
		val1 += 256;
		val1 *= val1;
		val1 *= this->m_Linebuf.Read(k);
		val1 >>= 16;
		if(this->m_unit == 1)
			*((char*)buf+k) = (char)(val1 > 255 ? 255 : val1 > 0 ? val1 : 0);
		else
			*((short*)buf+k) = (short)(val1 > 1023 ? 1023 : val1 > 0 ? val1 : 0);
	}

	return k;
}

int CIspLensFallOff::Process(char *src, char *dst)
{
	int i;

	for(i = 0; i < this->m_height; i++)
	{
		this->Write(src+i*this->m_width*this->m_unit, this->m_width);
		this->Read(dst+i*this->m_width*this->m_unit, this->m_width);
	}
	return i;
}

//==================== class CIspDpdDpc =========================

CIspDpdDpc::CIspDpdDpc(int width, int height, int unit)
{
	int i;
//	int table[17] = {0x20, 0x20, 0x30, 0x38, 0x40, 0x50, 0x60, 0x70, 0x80, 0x90, 0xa0, 0xb0, 0xc0, 0xd0, 0xe0, 0xf0, 0xff};
	int table[17] = {0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10};

	this->m_width = width;
	this->m_height = height;
	this->m_line = 0;
	this->m_unit = unit;
	this->m_pos = 0;
	this->SetBGLine(0x1);
	this->SetGpixel(0x0);
	this->SetDpdThd(0x20);
	this->SetNoiseTable(table);
	for(i = 0; i < 5; i++)
	{
		this->m_Linebuf[i].SetSize(width, 2, this->m_unit);
		this->m_pLinebuf[i] = &(this->m_Linebuf[i]);
	}
}

void CIspDpdDpc::SetNoiseTable(int *val)
{
	int i;
	
	for(i = 0; i < 17; i++)
	{
		this->m_noisetable[i] = val[i]&0xff;
	}
}

int* CIspDpdDpc::GetNoiseTable(void)
{
	return this->m_noisetable;
}

void CIspDpdDpc::SetDpdThd(int thd)
{
	this->m_dpdthd = thd&0xff;
}

int CIspDpdDpc::GetDpdThd(void)
{
	return this->m_dpdthd;
}

void CIspDpdDpc::SetBGLine(int bgline)
{
	this->m_bgline = bgline&0x1;
}

void CIspDpdDpc::SetGpixel(int gpixel)
{
	this->m_gpixel = gpixel&0x1;
}

int CIspDpdDpc::GetBGLine()
{
	return this->m_bgline;
}

int CIspDpdDpc::GetGpixel()
{
	return this->m_gpixel;
}

int CIspDpdDpc::Write(char *buf, int len)
{
	int pos = 0, pos1;

	if(len == 0)
		return 0;
	if(this->m_line & LAST3X3LINES)
		return 0;
	if(this->m_line == 3)
		return this->m_pLinebuf[4]->Write(buf, len);
	while(this->m_line < 3)
	{
		pos1 = this->m_pLinebuf[2+this->m_line]->Write(buf+pos, len);
		len -= pos1;
		pos += pos1;
		if(pos1 == 0)
			this->m_line++;
		if(len == 0)
			break;
	}
	if(this->m_line == 3)
	{
		this->m_Linebuf[0] = this->m_Linebuf[4];
		this->m_Linebuf[1] = this->m_Linebuf[3];
	}
	return pos;
}

int CIspDpdDpc::Read(char *buf, int len)
{
	if( ((this->m_line & 0xf) == 0) && (this->m_pLinebuf[4]->IsFull() == 0) )
		return 0;
	this->LastLinePro();
	len = this->DpdProcess(buf, len);
	this->LoopLinebuf();
	this->m_pos %= this->m_width;
	return len; 
}

int CIspDpdDpc::DpdProcess(char *buf, int len)
{
	int p[5], gpixel, max = 255;

	if(this->m_unit == 2)
		max = 1024;
	len /= this->m_unit;
	len = ((this->m_width - this->m_pos) > len) ? len : (this->m_width - this->m_pos);
	gpixel = (this->m_gpixel + this->m_pos)%2;
	
	for(; this->m_pos < len; this->m_pos++)	
	{
		if((this->m_pos < 2) || (this->m_pos > this->m_width -3))
			if(this->m_unit == 1)
				*((char*)buf+this->m_pos) = (char)this->m_pLinebuf[2]->Read(this->m_pos+2);
			else
				*((short*)buf+this->m_pos) = (short)this->m_pLinebuf[2]->Read(this->m_pos+2);
		else
		{
			p[0] = this->m_pLinebuf[0]->Read(this->m_pos+2);;
			p[1] = this->m_pLinebuf[2]->Read(this->m_pos-2+2);;
			p[2] = this->m_pLinebuf[2]->Read(this->m_pos+2);;
			p[3] = this->m_pLinebuf[2]->Read(this->m_pos+2+2);
			p[4] = this->m_pLinebuf[4]->Read(this->m_pos+2);
			if(this->m_unit == 1)
				*((char*)buf+this->m_pos) = (char)this->GetDpdNoise(p, gpixel);
			else
				*((short*)buf+this->m_pos) = (short)this->GetDpdNoise(p, gpixel);
		}
		gpixel = InverseBit(gpixel);
	}
	//this->m_gpixel = InverseBit(this->m_gpixel);
	//this->m_bgline = InverseBit(this->m_bgline);
	return this->m_pos*this->m_unit;
}

int CIspDpdDpc::GetDpdNoise(int *p, int gpixel)
{
	int DV, DH, thd, avg, var;
	int pixel;

	pixel = p[2];
	DV = abs((p[2] << 1) - p[0] - p[4]);
	DH = abs((p[2] << 1) - p[1] - p[3]);
	thd = this->m_dpdthd;

	if((DV > thd) && (DH > thd))
	{
		if(DV < DH)
			var = (p[0] + p[4]) >> 1;
		else
			var = (p[1] + p[3]) >> 1;
		return var;
	}
	if (DH > DV)
	{
		avg = p[0] + p[2] + p[4];
		avg *= 21;
		avg >>= 6;
		//avg /= 3;

		var = abs(p[0] - avg) + abs(p[2] - avg) + abs(p[4] - avg);
		var *= 21;
		var >>= 6;
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
		var *= 21;
		var >>= 6;
		//var /= 3;
		//avg = (pix0 + pix2 + pix4) / 3;
		//var = (abs(pix0 - avg) + abs(pix2 - avg) + abs(pix4 - avg)) / 3;
	}

	if(var < GetNoiseValue(pixel, gpixel))
		return avg;

	return p[2];
}

int CIspDpdDpc::GetNoiseValue(int p, int gpixel)
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
	return gpixel ? result : result >> 1;
}

void CIspDpdDpc::LastLinePro(void)
{
	if(this->m_pLinebuf[4]->IsFull())
		return;
	this->m_line |= LAST3X3LINES;
	this->m_line--;
	this->m_pLinebuf[4] = this->m_pLinebuf[2];
}

void CIspDpdDpc::LoopLinebuf(void)
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
}

int CIspDpdDpc::Process(char *src, char *dst)
{
	int size = this->m_width*this->m_height*this->m_unit, pos = 0, pos1 = 0, size1;
	int temp, width = this->m_width;

	size1 = size;
	temp = size1;
	while(size)
	{
		width = this->Write(src+pos, size);
		pos += width;
		size -= width;
		width = this->Read(dst+pos1, size1);
		pos1 += width;
		size1 -= width;
	}
	while(width)
	{
		width = this->Read(dst+pos1, size1);
		pos1 += width;
		size1 -= width;
	}
	return temp;
}

//============== class CIspDigitalGain ========================

CIspDigitalGain::CIspDigitalGain(int width, int height, int unit)
{
	this->m_width = width;
	this->m_height = height;
	this->m_line = 0;
	this->m_unit = unit;
	this->SetBGLine(0x1);
	this->SetGpixel(0x1);
	this->SetRGain(0x40);
	this->SetBGain(0x40);
	this->SetGGain(0x40);
	this->SetGlobalGain(0x80);
	this->SetGlobalEn(1);
	this->SetRgbEn(1);
	this->SetGainStep(0);
	this->m_Linebuf.SetSize(width, 0, unit);
}

void CIspDigitalGain::SetBGLine(int bgline)
{
	this->m_bgline = bgline&0x1;
}

void CIspDigitalGain::SetGpixel(int gpixel)
{
	this->m_gpixel = gpixel&0x1;
}

int CIspDigitalGain::GetBGLine()
{
	return this->m_bgline;
}

int CIspDigitalGain::GetGpixel()
{
	return this->m_gpixel;
}

void CIspDigitalGain::SetRGain(int gain)
{
	this->m_rGain = gain&0xff;
}

int CIspDigitalGain::GetRGain(void)
{
	return this->m_rGain;
}

void CIspDigitalGain::SetBGain(int gain)
{
	this->m_bGain = gain&0xff;
}

int CIspDigitalGain::GetBGain(void)
{
	return this->m_bGain;
}

void CIspDigitalGain::SetGGain(int gain)
{
	this->m_gGain = gain&0xff;
}

int CIspDigitalGain::GetGGain(void)
{
	return this->m_gGain;
}

void CIspDigitalGain::SetGlobalGain(int gain)
{
	this->m_globalGain = gain&0xff;
}

int CIspDigitalGain::GetGlobalGain(void)
{
	return this->m_globalGain;
}

void CIspDigitalGain::SetGlobalEn(int val)
{
	this->m_globalEn = val&0x1;
}

void CIspDigitalGain::SetRgbEn(int val)
{
	this->m_rgbEn = val&0x1;
}

void CIspDigitalGain::SetGainStep(int step)
{
	this->m_gainStep = step&0xff;
}

int CIspDigitalGain::GetGainStep(void)
{
	return this->m_gainStep;
}

int CIspDigitalGain::Write(char *buf, int len)
{
	this->m_Linebuf.ClearPoint();
	len = this->m_Linebuf.Write(buf, len);

	return len;
}

int CIspDigitalGain::Read(char *buf, int len)
{
	int k;
	int	temp, max = 255;
	int GlobalEn = 0, RgbEn = 0;
	int Gpixel = this->m_gpixel;
	int GlobalGain, rGain, gGain1, gGain2, bGain, rgbGain;

	len = len;
	if(this->m_unit == 2)
		max = 1023;
	GlobalEn = this->m_globalEn;
	RgbEn = this->m_rgbEn;
	GlobalGain = this->m_globalGain;
	temp = this->m_gainStep;
	switch(temp)
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
	
	for(k = 0; k < this->m_width; k++)
	{
		temp = this->m_Linebuf.Read(k);
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
			*((char*)buf+k) = (char)temp;
		else
			*((short*)buf+k) = (short)temp;
		Gpixel = InverseBit(Gpixel);
	}
	this->m_bgline = InverseBit(this->m_bgline);
	this->m_gpixel = InverseBit(this->m_gpixel);
	
	return k;
}

int CIspDigitalGain::Process(char *src, char *dst)
{
	int i;

	for(i = 0; i < this->m_height; i++)
	{
		this->Write(src+i*this->m_width*this->m_unit, this->m_width);
		this->Read(dst+i*this->m_width*this->m_unit, this->m_width);
	}

	return i;
}

//============== class CIspCfaExtract ========================

CIspCfaExtract::CIspCfaExtract(int width, int height, int mode, int unit)
{
	int i;

	this->SetMode(mode);
	this->SetSize(width, height, unit);
	for(i = 0; i < 9; i++)
		this->m_pLinebuf[i] = &(this->m_Linebuf[i]);
	for(i = 0; i < 5; i++)
		this->m_pGblock[i] = this->m_Gblock[i];
	this->m_GBline = 1;
	this->m_Gfirst = 1;
	if(this->GetMode())
	{
		this->SetFemm(0x40);
		this->SetFemp(0x60);
		this->SetFemmax(0x0f);
		this->SetFemmin(0x05);
		this->SetFemx1(0x05);
		this->SetFemx2(0x0f);
	}
}

void CIspCfaExtract::SetSize(int width, int height, int unit)
{
	int i;

	for(i = 0; i < 9; i++)
		this->m_Linebuf[i].SetSize(width, 4, unit);
	this->m_unit = unit;
	this->m_width = width;
	this->m_height = height;
	this->Clear();
}

void CIspCfaExtract::SetOrder(int order)
{
	const int setting[][2] =
	{
		{ 1, 0 },	//	RAW_BG,
		{ 1, 1 },	//	RAW_GB,
		{ 0, 0 },	//	RAW_RG,
		{ 0, 1 } 	//	RAW_GR,
	};
	order = ((order <= RAW_GR) && (order >= RAW_BG)) ? (order-RAW_BG) : 0;
	//order = ((order >= RAW_GR) && (order <= RAW_BG)) ? (order-RAW_BG) : 0;
	this->m_Gfirst = setting[order][1];
	this->m_GBline = setting[order][0];
	this->Clear();
}

void CIspCfaExtract::Clear(void)
{
	this->m_line = 0;
	this->m_pos = 0;
}

int	CIspCfaExtract::Write(char *buf, int len)
{
	int pos = 0, pos1;

	if(len == 0)
		return 0;
	if(this->m_line & LAST5X5LINES)
		return 0;
	if(this->m_line == 5)
		return this->m_pLinebuf[8]->Write(buf, len);
	while(this->m_line < 5)
	{
		pos1 = this->m_pLinebuf[4+this->m_line]->Write(buf+pos, len);
		len -= pos1;
		pos += pos1;
		if(pos1 == 0)
			this->m_line++;
		if(len == 0)
			break;
	}
	if(this->m_line == 5)
	{
		this->m_Linebuf[0] = this->m_Linebuf[4];
		this->m_Linebuf[1] = this->m_Linebuf[5];
		this->m_Linebuf[2] = this->m_Linebuf[4];
		this->m_Linebuf[3] = this->m_Linebuf[5];
	}
	return pos;
}

int	CIspCfaExtract::Read(char *buf, int len)
{
	if( ((this->m_line & 0xf) == 0) && (this->m_pLinebuf[8]->IsFull() == 0) )
		return 0;
	this->LastLinePro();
	len = this->Interpolate(buf, len);
	this->LoopLinebuf();
	this->m_pos %= this->m_width;
	return len;
}

int	CIspCfaExtract::Interpolate(char *buf, int len)
{
	int gpixel, val[3], i, pos = 0, max = 255;
	CBaseConvert tools;

	if(this->m_unit == 2)
		max = 1024;
	len /= 3*this->m_unit;
	len = ((this->m_width - this->m_pos) > len) ? len : (this->m_width - this->m_pos);
	gpixel = (this->m_Gfirst + this->m_pos) % 2;
	for(; this->m_pos < len; this->m_pos++)
	{
		if(this->m_pos == 0)
			this->GetFirstGblock();
		else
			this->GetNextGblock();
		if(this->m_mode)
		{
			val[0] = this->GetEdge();
			tools.setvalue(buf+pos, val[0], 1);		//edeg is 8bit value, so unit = 1
			pos++;
		}
		else
		{
			if(this->m_GBline)
			{
				if(gpixel)
				{
					val[0] = this->GetRfromGB(this->m_pos);
					val[1] = this->m_pGblock[2][2];
					val[2] = this->GetRfromGR(this->m_pos);
				}
				else
				{
					val[0] = this->GetRfromBG(this->m_pos); 
					val[1] = this->m_pGblock[2][2];
					val[2] = this->m_pLinebuf[4]->Read(this->m_pos+4);
				}
			}
			else
			{
				if(gpixel)
				{
					val[0] = this->GetRfromGR(this->m_pos); 
					val[1] = this->m_pGblock[2][2];
					val[2] = this->GetRfromGB(this->m_pos);
				}
				else
				{
					val[0] = this->m_pLinebuf[4]->Read(this->m_pos+4);
					val[1] = this->m_pGblock[2][2];
					val[2] = this->GetRfromBG(this->m_pos); 
				}
			}
			gpixel = gpixel ? 0 : 1;
			for(i = 2; i >= 0; i--, pos += this->m_unit)
			{
				val[i] = (val[i] < 0) ? 0 : ((val[i] > max) ? max : val[i]);
				tools.setvalue(buf+pos, val[i], this->m_unit);
			}
		}
	}
	return pos;
}

int	CIspCfaExtract::GetRfromBG(int pos)		//GB line, get R at B pixel
{
	int x1, x2;

	x1 = this->m_pLinebuf[3]->Read(3+pos) + this->m_pLinebuf[5]->Read(3+pos) + 
		this->m_pLinebuf[3]->Read(5+pos) + this->m_pLinebuf[5]->Read(5+pos);
	x2 = this->m_pGblock[1][1] + this->m_pGblock[3][3] + 
		this->m_pGblock[1][3] + this->m_pGblock[3][1];
	x1 -= x2;
	x1 >>= 2;
	x1 += this->m_pGblock[2][2];
	return x1;
}

int	CIspCfaExtract::GetRfromGR(int pos)		//GR line, get R at G pixel
{
	int x1, x2;

	x1 = this->m_pLinebuf[4]->Read(3+pos) + this->m_pLinebuf[4]->Read(5+pos);
	x2 = this->m_pGblock[1][2] + this->m_pGblock[3][2];
	x1 -= x2;
	x1 >>= 1;
	x1 += this->m_pGblock[2][2];
	return x1;
}

int CIspCfaExtract::GetRfromGB(int pos)		//GB line, get R at G pixel
{
	int x1, x2;

	x1 = this->m_pLinebuf[3]->Read(4+pos) + this->m_pLinebuf[5]->Read(4+pos);
	x2 = this->m_pGblock[2][1] + this->m_pGblock[2][3];
	x1 -= x2;
	x1 >>= 1;
	x1 += this->m_pGblock[2][2];
	return x1;
}

void CIspCfaExtract::LastLinePro(void)
{
	if(this->m_pLinebuf[8]->IsFull())
		return;
	this->m_line |= LAST5X5LINES;
	this->m_line--;
	this->m_pLinebuf[8] = this->m_pLinebuf[6];
}

void CIspCfaExtract::LoopLinebuf(void)
{
	int i;
	CLineBayerBuf *p = this->m_pLinebuf[0];

	if(this->m_pos != this->m_width)
		return;
	for(i = 0; i < 8; i++)
		this->m_pLinebuf[i] = this->m_pLinebuf[i+1];
	this->m_pLinebuf[8] = p;
	this->m_pLinebuf[8]->ClearPoint();
	this->m_GBline = this->m_GBline ? 0 : 1;
	this->m_Gfirst = this->m_Gfirst ? 0 : 1;
}

int CIspCfaExtract::GetGpixel(int col, int row)
{
	int dh, dv, x1, x2;

	if( (this->m_Gfirst + col + row) % 2 )	
		return this->m_pLinebuf[col]->Read(row);

	x1 = this->m_pLinebuf[col]->Read(row-1) - this->m_pLinebuf[col]->Read(row+1);
	x1 = (x1 > 0) ? x1 : -x1;
	x2 = this->m_pLinebuf[col]->Read(row-2) + this->m_pLinebuf[col]->Read(row+2) - 
		(this->m_pLinebuf[col]->Read(row) << 1);
	x2 = (x2 > 0) ? x2 : -x2;
	dh = (x1 << 1) + x2;

	x1 = this->m_pLinebuf[col-1]->Read(row) - this->m_pLinebuf[col+1]->Read(row);
	x1 = (x1 > 0) ? x1 : -x1;
	x2 = this->m_pLinebuf[col-2]->Read(row) + this->m_pLinebuf[col+2]->Read(row) - 
		(this->m_pLinebuf[col]->Read(row) << 1);
	x2 = (x2 > 0) ? x2 : -x2;
	dv = (x1 << 1) + x2;

	if(dh < dv)
	{
		x1 = (this->m_pLinebuf[col]->Read(row-1) + this->m_pLinebuf[col]->Read(row+1)) << 2;
		x2 = (this->m_pLinebuf[col]->Read(row) << 1) - this->m_pLinebuf[col]->Read(row-2) - 
			this->m_pLinebuf[col]->Read(row+2);
	}
	else
	{
		x1 = (this->m_pLinebuf[col-1]->Read(row) + this->m_pLinebuf[col+1]->Read(row)) << 2;
		x2 = (this->m_pLinebuf[col]->Read(row) << 1) - this->m_pLinebuf[col-2]->Read(row) - 
			this->m_pLinebuf[col+2]->Read(row);
	}
	x1 += x2;
	x1 >>= 3;
	return x1;
}

void CIspCfaExtract::GetFirstGblock(void)
{
	int i, j;

	for(i = 0; i < 5; i++)
		for(j = 0; j < 5; j++)
			this->m_pGblock[i][j] = this->GetGpixel(j+2, i+2);
}

void CIspCfaExtract::GetNextGblock(void)
{
	int i;
	int *p = this->m_pGblock[0];

	for(i = 0; i < 5; i++)
		this->m_pGblock[0][i] = this->GetGpixel(i+2, this->m_pos+6);
	for(i = 0; i < 4; i++)
		this->m_pGblock[i] = this->m_pGblock[i+1];
	this->m_pGblock[4] = p;
}

//edge extract
void CIspCfaExtract::SetMode(int mode)
{
	this->m_mode = mode&0x1;
}

int CIspCfaExtract::GetMode(void)
{
	return this->m_mode;
}

void CIspCfaExtract::SetFemm(int femm)
{
	this->m_femm = femm&0xff;
}

int CIspCfaExtract::GetFemm(void)
{
	return this->m_femm;
}

void CIspCfaExtract::SetFemp(int femp)
{
	this->m_femp = femp&0xff;
}

int CIspCfaExtract::GetFemp(void)
{
	return this->m_femp;
}

void CIspCfaExtract::SetFemmin(int femmin)
{
	this->m_femmin = femmin&0xff;
}

int CIspCfaExtract::GetFemmin(void)
{
	return this->m_femmin;
}

void CIspCfaExtract::SetFemmax(int femmax)
{
	this->m_femmax = femmax&0xff;
}

int CIspCfaExtract::GetFemmax(void)
{
	return this->m_femmax;
}

void CIspCfaExtract::SetFemx1(int femx1)
{
	this->m_femx1 = femx1&0xff;
}

int CIspCfaExtract::GetFemx1(void)
{
	return this->m_femx1;
}

void CIspCfaExtract::SetFemx2(int femx2)
{
	this->m_femx2 = femx2&0xff;
}

int CIspCfaExtract::GetFemx2(void)
{
	return this->m_femx2;
}

int CIspCfaExtract::GetEdge(void)
{
	int x, temp;
	int femm, femp, femmin, femmax, femx1, femx2;

	femm = this->m_femm;
	femp = this->m_femp;
	femx1 = this->m_femx1;
	femx2 = this->m_femx2;
	femmin = this->m_femmin;
	femmax = this->m_femmax;

	x = this->m_pGblock[2][2] << 3;
	temp = this->m_pGblock[0][0] + this->m_pGblock[0][2] + this->m_pGblock[0][4] +
		this->m_pGblock[2][0] + this->m_pGblock[2][4] + this->m_pGblock[4][0] +
		this->m_pGblock[4][2] + this->m_pGblock[4][4];
	x -= temp;
//	x /= 8;
	x >>= 3;

	if( (x > femx2) || ((x + femx2) <= 0) )
		x *= femm;
	else if( (x > femx1) && (x <= femx2) )
	{
		x -= femx1;
		x *= femp;
	}
	else if( (x < femx1) && ((x+femx1) >= 0) )
	{
		x = 0;
	}
	else
	{
		x += femx1;
		x *= femp;
	}
//	x /= 16;
	x >>= 4;
//	x >>= 7;		//divided by 128, ??

	return (char) ( (x > femmax) ? femmax : ((x < -femmin) ? -femmin : x) );
}

int CIspCfaExtract::Process(char *src, char *dst)
{
	int size, pos = 0, pos1 = 0, size1, width = 0;
	int temp;

	size = this->m_width * this->m_height * this->m_unit;
	if(this->m_mode)
		size1 = size;
	else
		size1 = size * 3;
	temp = size1;
	this->SetOrder(RAW_BG);
	while(size)
	{
		width = this->Write(src+pos, size);
		pos += width;
		size -= width;
		width = this->Read(dst+pos1, size1);
		pos1 += width;
		size1 -= width;
	}
	while(width)
	{
		width = this->Read(dst+pos1, size1);
		pos1 += width;
		size1 -= width;
	}
	return temp;
}

//============== class CIspColorCorrect ========================

CIspColorCorrect::CIspColorCorrect(int width, int height, int unit)
{
	//int colorMatrix[9] = {0x50, 0xf8, 0xf8, 0xf8, 0x50, 0xf8, 0xf8, 0xf8, 0x50};
	int colorMatrix[9] = {0x40, 0x0, 0x0, 0x0, 0x40, 0x0, 0x0, 0x0, 0x40};
	int offset[3] = {0, 0, 0};

	this->m_width = width;
	this->m_height = height;
	this->m_unit = unit;
	this->SetColorMatrix(colorMatrix);
	this->SetColorOffset(offset);

}

void CIspColorCorrect::SetColorMatrix(int *p)
{
	int i, j;

	for(i = 0; i < 3; i++)
		for(j = 0; j < 3; j++)
			this->m_colormatrix[i][j] = p[i*3+j];
}

void CIspColorCorrect::SetColorOffset(int *p)
{
	int i;

	for(i = 0; i < 3; i++)
		this->m_offset[i] = p[i];
}

int CIspColorCorrect::Write(char *buf, int len)
{
	len = 3*this->m_unit;
	if(this->m_unit == 1)
	{
		this->m_b = buf[0]&0xff;
		this->m_g = buf[1]&0xff;
		this->m_r = buf[2]&0xff;
	}
	else
	{
		this->m_b = *((short*)buf)&0x3ff;
		this->m_g = *((short*)buf+1)&0x3ff;
		this->m_r = *((short*)buf+2)&0x3ff;
	}

	return len;
}

int CIspColorCorrect::Read(char *buf, int len)
{
	int max = 255, temp;

	len = 3*this->m_unit;
	if(this->m_unit == 2)
		max = 1023;
	
	//r ,output is bgr
	temp = (this->m_colormatrix[0][0]&0xff)*this->m_r + (this->m_colormatrix[0][1]&0xff)*this->m_g + (this->m_colormatrix[0][2]&0xff)*this->m_b;
	temp = (temp>>6) + (this->m_offset[0]&0xff);
	temp = (temp > max) ? max : (temp > 0 ? temp : 0);
	if(this->m_unit == 1)
		buf[2] = (char)temp;
	else
		*((short*)buf+2) = (short)temp;
	//g
	temp = (this->m_colormatrix[1][0]&0xff)*this->m_r + (this->m_colormatrix[1][1]&0xff)*this->m_g + (this->m_colormatrix[1][2]&0xff)*this->m_b;
	temp = (temp>>6) + (this->m_offset[1]&0xff);
	temp = (temp > max) ? max : (temp > 0 ? temp : 0);
	if(this->m_unit == 1)
		buf[1] = (char)temp;
	else
		*((short*)buf+1) = (short)temp;
	//b
	temp = (this->m_colormatrix[2][0]&0xff)*this->m_r + (this->m_colormatrix[2][1]&0xff)*this->m_g + (this->m_colormatrix[2][2]&0xff)*this->m_b;
	temp = (temp>>6) + (this->m_offset[2]&0xff);
	temp = (temp > max) ? max : (temp > 0 ? temp : 0);
	if(this->m_unit == 1)
		buf[0] = (char)temp;
	else
		*((short*)buf) = (short)temp;
	
	return len;
}

int CIspColorCorrect::Process(char *src, char *dst)
{
	int pos, size, pos1 = 0, len;

	size = this->m_width * this->m_height * this->m_unit * 3;

	for(pos = 0; pos < size; )
	{
		len = this->Write(src+pos, 1);
		pos += len;
		len = this->Read(dst+pos1, 1);
		pos1 += len;
	}

	return pos1;
}

//============== class CIspGammaCorrect ========================

CIspGammaCorrect::CIspGammaCorrect(int width, int height, int unit, int enable)
{
	int rgamma[17] = {0x0, 0x13, 0x38, 0x59, 0x79, 0x92, 0xa7, 0xb9, 0xc8, 0xd4, 0xdf, 0xe7, 0xee, 0xf4, 0xf9, 0xfc, 0xff};
	int ggamma[17] = {0x0, 0x13, 0x38, 0x59, 0x79, 0x92, 0xa7, 0xb9, 0xc8, 0xd4, 0xdf, 0xe7, 0xee, 0xf4, 0xf9, 0xfc, 0xff};
	int bgamma[17] = {0x0, 0x13, 0x38, 0x59, 0x79, 0x92, 0xa7, 0xb9, 0xc8, 0xd4, 0xdf, 0xe7, 0xee, 0xf4, 0xf9, 0xfc, 0xff};

	this->m_unit = unit;
	this->m_width = width;
	this->m_height = height;
	this->m_enable = enable;
	this->SetRGamma(rgamma);
	this->SetGGamma(ggamma);
	this->SetBGamma(bgamma);
}

void CIspGammaCorrect::SetRGamma(int *p)
{
	int i;

	for(i = 0; i < 17; i++)
		this->m_rgamma[i] = p[i]&0xff;
}

int* CIspGammaCorrect::GetRGamma(void)
{
	return this->m_rgamma;
}

void CIspGammaCorrect::SetBGamma(int *p)
{
	int i;

	for(i = 0; i < 17; i++)
		this->m_bgamma[i] = p[i]&0xff;
}

int* CIspGammaCorrect::GetBGamma(void)
{
	return this->m_bgamma;
}

void CIspGammaCorrect::SetGGamma(int *p)
{
	int i;

	for(i = 0; i < 17; i++)
		this->m_ggamma[i] = p[i]&0xff;
}

int* CIspGammaCorrect::GetGGamma(void)
{
	return this->m_ggamma;
}

int CIspGammaCorrect::Write(char *buf, int len)
{
	len = 3*this->m_unit;
	if(this->m_unit == 1)
	{
		this->m_b = (unsigned char)buf[0];
		this->m_g = (unsigned char)buf[1];
		this->m_r = (unsigned char)buf[2];
	}
	else
	{
		this->m_b = *((short*)buf)&0x3ff;
		this->m_g = *((short*)buf+1)&0x3ff;
		this->m_r = *((short*)buf+2)&0x3ff;
	}

	return len;
}

int CIspGammaCorrect::Read(char *buf, int len)
{
	len = 3;

	if(!this->m_enable)		//gamma correction disable
	{
		if(this->m_unit == 2)
		{
			buf[2] = (char)((this->m_r>>2)&0xff);
			buf[1] = (char)((this->m_g>>2)&0xff);
			buf[0] = (char)((this->m_b>>2)&0xff);
			return len;
		}
		else
		{
			buf[2] = (char)(this->m_r&0xff);
			buf[1] = (char)(this->m_g&0xff);
			buf[0] = (char)(this->m_b&0xff);
			return len;
		}
	}
	this->GammaCorrect(buf);

	return len;
}

int CIspGammaCorrect::GetGammaValue(int val, int rgb)
{
	int	x1;
	int	y1, y2;
	int	flag;
	int	result;

	val = val&0xff;
	flag = val >> 4;
	x1 = flag << 4;

	if(rgb == 0)	//r
	{
		y1 = this->m_rgamma[flag]&0xff;
		y2 = this->m_rgamma[flag+1]&0xff;
	}
	else if(rgb == 1)	//g
	{
		y1 = this->m_ggamma[flag]&0xff;
		y2 = this->m_ggamma[flag+1]&0xff;
	}
	else		//b
	{
		y1 = this->m_bgamma[flag]&0xff;
		y2 = this->m_bgamma[flag+1]&0xff;
	}

	result = y2 - y1;
	result *= val - x1;
	result >>= 4;
	result += y1;

	return result > 255 ? 255 : result;
}

void CIspGammaCorrect::GammaCorrect(char *buf)
{
	if(this->m_unit == 2)		//10 bit data
	{
		buf[2] = (char)this->GetGammaValue((char)(this->m_r>>2), 0);
		buf[1] = (char)this->GetGammaValue((char)(this->m_g>>2), 1);
		buf[0] = (char)this->GetGammaValue((char)(this->m_b>>2), 2);
	}
	else
	{
		buf[2] = (char)this->GetGammaValue((char)(this->m_r), 0);
		buf[1] = (char)this->GetGammaValue((char)(this->m_g), 1);
		buf[0] = (char)this->GetGammaValue((char)(this->m_b), 2);
	}
}

int CIspGammaCorrect::Process(char *src, char *dst)
{
	int pos, size, pos1 = 0, len;

	size = this->m_width * this->m_height * this->m_unit * 3;

	for(pos = 0; pos < size; )
	{
		len = this->Write(src+pos, 1);
		pos += len;
		len = this->Read(dst+pos1, 1);
		pos1 += len;
	}

	return pos1;
}

//============== class CIspColorConvert ========================

CIspColorConvert::CIspColorConvert(int width, int height)
{
	this->m_width = width;
	this->m_height = height;
}

CIspColorConvert::Write(char *buf, int len)
{
	len = 6;

	this->m_b[0] = buf[0];
	this->m_g[0] = buf[1];
	this->m_r[0] = buf[2];
	this->m_b[1] = buf[3];
	this->m_g[1] = buf[4];
	this->m_r[1] = buf[5];
	
	return len;
}

int CIspColorConvert::Read(char *buf, int len)
{
	int r, g, b, y, u, v;

	len = 4;
	r = (int)(this->m_r[0]&0xff);
	g = (int)(this->m_g[0]&0xff);
	b = (int)(this->m_b[0]&0xff);
	y = (77 * r + 150 * g + 29 * b) >> 8;
	u = ((128 * b - 43 * r - 85 * g) >> 8) + 128;
	buf[1] = (char)(y < 0 ? 0 : ((y > 255) ? 255 : y));
	buf[0] = (char)(u < 0 ? 0 : ((u > 255) ? 255 : u));

	r = (int)(this->m_r[1]&0xff);
	g = (int)(this->m_g[1]&0xff);
	b = (int)(this->m_b[1]&0xff);
	y = (77 * r + 150 * g + 29 * b) >> 8;
	v = ((128 * r - 107 * g - 21 * b) >> 8) + 128;
	buf[3] = (char)(y < 0 ? 0 : ((y > 255) ? 255 : y));
	buf[2] = (char)(v < 0 ? 0 : ((v > 255) ? 255 : v));
	
	return len;
}

int CIspColorConvert::Process(char *src, char *dst)
{
	int pos, size, pos1 = 0, len;

	size = this->m_width * this->m_height * 3;

	for(pos = 0; pos < size; )
	{
		len = this->Write(src+pos, 1);
		pos += len;
		len = this->Read(dst+pos1, 1);
		pos1 += len;
	}

	return pos1;
}

//============== class CIspEdgeEnhance ========================

CIspEdgeEnhance::CIspEdgeEnhance(int width, int height)
{
	this->m_width = width;
	this->m_height = height;
}

int CIspEdgeEnhance::Write(char *buf, char *edge, int len)
{
	len = 2;
	this->m_y = buf[1];
	this->m_uv = buf[0];
	this->m_edge = edge[0];

	return len;
}

int CIspEdgeEnhance::Read(char *buf, int len)
{
	int val1, val2;

	len = 2;
	val1 = this->m_y&0xff;
	val2 = this->m_edge;;
	val1 += val2;
	buf[1] = (char)(val1 > 255 ? 255 : (val1 < 0 ? 0 : (char)val1));
	buf[0] = (char)(this->m_uv);

	return len;
}

int CIspEdgeEnhance::Process(char *src, char *edge, char *dst)
{
	int pos, size, pos1 = 0, pos2 = 0, len;

	size = this->m_width * this->m_height * 2;

	for(pos = 0; pos < size; )
	{
		len = this->Write(src+pos, edge+pos2, 1);
		pos += len;
		pos2++;
		len = this->Read(dst+pos1, 1);
		pos1 += len;
	}

	return pos1;
}
