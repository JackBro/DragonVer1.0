

#include "internal.h"

const int gDither10_8[][4] = 
{
	{ 1, 3, 1, 3 },
	{ 4, 2, 4, 2 },
	{ 1, 3, 1, 3 },
	{ 4, 2, 4, 2 }
};

//==================== class CIspModule326 ===================

CIspModule326::CIspModule326(int width, int height, int unit)
{
	int coef[25] = {0, 0, 0, 0, 0, 
					0, -1, -1, -1, 0,
					0, -1, 8, -1, 0,
					0, -1, -1, -1, 0,
					0, 0, 0, 0, 0};
	this->SetIspCtrl(0xffff);
	this->SetBGLine(0x1);
	this->SetGpixel(0x1);
	this->m_width = width;
	this->m_height = height;
	this->m_unit = unit;
	this->m_rawdata = 1;
	this->m_gain = 0x40;
	this->m_dither = 0;
	this->SetOutputFile(0);
	this->SetNewEdge(0);
	this->SetCoefEdge(coef);
	this->SetDivEdge(8);
	this->m_edgesum = 0;
}

void CIspModule326::SetSize(int width, int height, int unit)
{
	this->m_width = width;
	this->m_height = height;
	this->m_unit = unit;
}

void CIspModule326::SetDpdthd(int dpdthd)
{
	this->m_dpdthd = dpdthd;
}

void CIspModule326::SetDither(int dither)
{
	this->m_dither = dither&0x1;
}

void CIspModule326::SetRawData(int raw)
{
	this->m_rawdata = raw;
}

void CIspModule326::SetGain(int gain)
{
	this->m_gain = gain;
}

void CIspModule326::SetIspCtrl(int ctrl)
{
	this->ispctrl = ctrl;
}

int CIspModule326::GetIspCtrl(void)
{
	return this->ispctrl;
}

void CIspModule326::SetBGLine(int bgline)
{
	this->bgline = bgline&0x1;
}

int CIspModule326::GetBGLine(void)
{
	return this->bgline&0x1;
}

void CIspModule326::SetGpixel(int gpixel)
{
	this->gpixel = gpixel&0x1;
}

void CIspModule326::SetDpdVersion(int ver)
{
	this->m_ver = ver;
}

void CIspModule326::SetNewEdge(int val)
{
	this->m_newedge = val;
}

int CIspModule326::GetGpixel(void)
{
	return this->gpixel&0x1;
}

void CIspModule326::SetOutputFile(int val)
{
	this->m_outputfile = val;
}

void CIspModule326::SetIspModuleEn(int flag, int en)
{
	int ispctrl;

	ispctrl = this->GetIspCtrl();
	if(en)
		this->SetIspCtrl(ispctrl|flag);
	else
		this->SetIspCtrl(ispctrl&(~(flag)));
}

void CIspModule326::SetCoefEdge(int *coef)
{
	int i;

	for(i = 0; i < 25; i++)
		this->m_coefedge[i] = (char)coef[i];
}

void CIspModule326::SetDivEdge(int div)
{
	this->m_divedge = div;
}

void CIspModule326::SetMaxEdge(int val)
{
	this->m_maxedge = val&0xff;
}

unsigned int CIspModule326::GetEdgeSum(void)
{
	return this->m_edgesum;
}

void CIspModule326::SetAFH(int *afh)
{
	int i;

	if(afh == NULL)
		return;
	for(i = 0; i < AF_H_AND_V_NUM; i++)
		this->m_afh[i] = afh[i];
}

void CIspModule326::SetAFV(int *afv)
{
	int i;

	if(afv == NULL)
		return;
	for(i = 0; i < AF_H_AND_V_NUM; i++)
		this->m_afv[i] = afv[i];
}

void CIspModule326::SetAFW(int *afw)
{
	int i;

	if(afw == NULL)
		return;
	for(i = 0; i < AF_WINDOW_NUM; i++)
		this->m_afw[i] = afw[i];
}

void CIspModule326::Bit8ToBit10(char *src, char *dst)
{
	short buf;
	int i;
	CFileBin fp;

	for(i = 0; i < this->m_size; i++)
	{
		buf = (((short)(*((char*)src+i))&0xff)<<2);
		//buf |= 0x03;			//append low 2bit 0 or 1
		*((short*)dst+i) = (short)buf;
	}
	this->m_unit = 2;
	this->m_size *= 2;
	if(this->m_outputfile)
	{
		fp.Open("src10.out", "wb");
		fp.Write(dst, this->m_size);
		fp.Close();
	}
}

void CIspModule326::RGB2BGR(char *buf)
{
	int i;
	char tmp;

	for(i = 0; i < this->m_height*this->m_width*3; i += 3)
	{
		tmp = buf[i];
		buf[i] = buf[i+2];
		buf[i+2] = tmp;
	}
}

int CIspModule326::Process(char *src, char *dst)
{
	int ispCtrl;
	int flag, len;
	CBufferBase edgeBuf;
	int dpdver = 0;
	CFileBin fp;

	ispCtrl = this->GetIspCtrl();
	this->m_size = this->m_width* this->m_height*this->m_unit;
	if(!this->m_rawdata)		//after cfa data
		this->m_size *= 3;
#if 1
	if(this->m_unit == 1)
		this->Bit8ToBit10(src, dst);
	else
		memcpy(dst, src, this->m_size);
#else
	memcpy(dst, src, this->m_size);
#endif
	len = this->m_size;
	//blacklevel->dpd->lensfalloff->digitalgain->cfa->colorcorrect->gamma correct->hsv->histo->rgb2yuv->edgeenhance
	if(ispCtrl&(BLACKLEVEL))	//enable blacklevel
	{
		memcpy(src, dst, len);
		CIspBlackLevel326 ispBlack(this->m_width, this->m_height, this->m_unit);
		//ispBlack.SetBOffset(0x10);
		//ispBlack.SetG1Offset(0x10);
		//ispBlack.SetG2Offset(0x10);
		//ispBlack.SetROffset(0x10);
		ispBlack.SetBGLine(this->bgline);
		ispBlack.SetGpixel(this->gpixel);
		ispBlack.Process(src, dst);
		if(this->m_outputfile)
		{
			fp.Open("blacklevel.out", "wb");
			fp.Write(dst, len);
			fp.Close();
		}
	}
	if(ispCtrl&(DPDDPC))	//enable dpd
	{
		memcpy(src, dst, len);

		if(this->m_ver)
			dpdver = 1;
		CIspDpdDpc326 ispDpd(this->m_width, this->m_height, this->m_unit, dpdver);
		ispDpd.SetBGLine(this->bgline);
		ispDpd.SetGpixel(this->gpixel);
		ispDpd.SetDpdThd(this->m_dpdthd);
		ispDpd.Process(src, dst);
		if(this->m_outputfile)
		{
			fp.Open("dpddpc.out", "wb");
			fp.Write(dst, len);
			fp.Close();
		}
	}
	if(ispCtrl&(LENSFALLOFF))	//enable LENSFALLOFF
	{
		memcpy(src, dst, len);
		CIspLensFallOff326 ispLens(this->m_width, this->m_height, this->m_unit);
		ispLens.Process(src, dst);
		if(this->m_outputfile)
		{
			fp.Open("lensfalloff.out", "wb");
			fp.Write(dst, len);
			fp.Close();
		}
	}
	if(ispCtrl&(DIGITALGAIN))	//enable DIGITALGAIN
	{
		memcpy(src, dst, len);
		CIspDigitalGain326 ispGain(this->m_width, this->m_height, this->m_unit);
		ispGain.SetGlobalEn(1);
		ispGain.SetGlobalGain(this->m_gain);
		ispGain.SetRgbEn(1);
		ispGain.SetBGLine(this->bgline);
		ispGain.SetGpixel(this->gpixel);
		ispGain.Process(src, dst);
		if(this->m_outputfile)
		{
			fp.Open("digitalgain.out", "wb");
			fp.Write(dst, len);
			fp.Close();
		}
	}
	if(ispCtrl&CFAEXTRACT)
	{
		memcpy(src, dst, len);
		CIspCfaExtract326 ispCfa(this->m_width, this->m_height, 0, this->m_unit);
		ispCfa.SetBGLine(this->bgline);
		ispCfa.SetGpixel(this->gpixel);
		ispCfa.Process(src, dst);
		len = 3*this->m_size;
		if(this->m_outputfile)
		{
			fp.Open("cfa.out", "wb");
			fp.Write(dst, len);
			fp.Close();
		}
	}
	if((ispCtrl&(EDGEENHANCE)) || this->m_newedge)	//enable EDGEENHANCE
	{
		edgeBuf.SetSize(this->m_size, 1);
		CIspCfaExtract326 ispEdge(this->m_width, this->m_height, 1, this->m_unit);
		if(this->m_newedge)	//new edge
			ispEdge.SetMode(2);
		ispEdge.SetBGLine(this->bgline);
		ispEdge.SetGpixel(this->gpixel);
		ispEdge.SetCoefEdge(this->m_coefedge);
		ispEdge.SetDivEdge(this->m_divedge);
		ispEdge.SetFemmax(this->m_maxedge);
		ispEdge.Process(src, edgeBuf.m_buf);
		if(this->m_outputfile)
		{
			fp.Open("edge.out", "wb");
			fp.Write(edgeBuf.m_buf, this->m_size);
			fp.Close();
		}
		if(ispCtrl&(AUTOFOCUS))
		{
			CIspAutoFocus af(this->m_width, this->m_height);
			af.SetAFH(this->m_afh);
			af.SetAFV(this->m_afv);
			af.SetAFWeight(this->m_afw);
			af.Process(edgeBuf.m_buf, &this->m_edgesum);
		}
	}
	if(ispCtrl&(COLORCORRECT))	//enable COLORCORRECT
	{
		memcpy(src, dst, len);
		CIspColorCorrect326 ispColorCorrect(this->m_width, this->m_height, this->m_unit);
		ispColorCorrect.Process(src, dst);
		if(this->m_outputfile)
		{
			fp.Open("colorcorrect.out", "wb");
			fp.Write(dst, len);
			fp.Close();
		}
	}
	if(ispCtrl&(GAMMACORRECT))	//enable GAMMACORRECT
	{
		flag = 1;
	}	
	else
		flag = 0;
#if 1
	memcpy(src, dst, len);
	CIspGammaCorrect326 ispGammaCorrect(this->m_width, this->m_height, this->m_unit, flag);
	ispGammaCorrect.SetDither(this->m_dither);
	ispGammaCorrect.Process(src, dst);
	len = len/this->m_unit;
	if(this->m_outputfile)
	{
		fp.Open("gamma.out", "wb");
		fp.Write(dst, len);
		fp.Close();
	}
#endif
	if(ispCtrl&(COLORCONVERT))
	{
		memcpy(src, dst, len);
		CIspColorConvert326 ispColorConvert(this->m_width, this->m_height);
		ispColorConvert.Process(src, dst);
		len = 2*len/3;
		if(this->m_outputfile)
		{
			fp.Open("colorconvert.out", "wb");
			fp.Write(dst, len);
			fp.Close();
		}
		if((ispCtrl&(EDGEENHANCE)) || this->m_newedge)	//enable EDGEENHANCE
		{
			memcpy(src, dst, len);
			CIspEdgeEnhance326 ispEdgeEnhance(this->m_width, this->m_height);
			ispEdgeEnhance.Process(src, edgeBuf.m_buf, dst);
			if(this->m_outputfile)
			{
				fp.Open("edgeenhance.out", "wb");
				fp.Write(dst, len);
				fp.Close();
			}
		}
	}
	else
		this->RGB2BGR(dst);

	return len;
}

//==================== class CIspTestBar326 =====================

int CIspTestBar326::Write(char *buf, int len)
{
	buf = buf;
	len = len;
	
	return 0;
}
int CIspTestBar326::Read(char *buf, int len)
{
	buf = buf;
	len = len;

	return 0;
}

//==================== class CIspBlackLevel326 =========================

CIspBlackLevel326::CIspBlackLevel326(int width, int height, int unit)
{
	this->m_rOffset = 0;
	this->m_g1Offset = 0;
	this->m_g2Offset = 0;
	this->m_bOffset = 0;
	this->m_bgline = 1;
	this->m_gpixel = 1;
	this->SetSize(width, height, unit);
}

void CIspBlackLevel326::SetSize(int width, int height, int unit)
{
	this->m_width = width;
	this->m_height = height;
	this->m_unit = unit;
	this->m_Linebuf.SetSize(width, 0, unit);
}

void CIspBlackLevel326::SetBGLine(int bgline)
{
	this->m_bgline = bgline&0x1;
}

int CIspBlackLevel326::GetBGLine(void)
{
	return this->m_bgline&0x1;
}

void CIspBlackLevel326::SetGpixel(int gpixel)
{
	this->m_gpixel = gpixel&0x1;
}

int CIspBlackLevel326::GetGpixel(void)
{
	return this->m_gpixel&0x1;
}

void CIspBlackLevel326::SetROffset(int roffset)
{
	this->m_rOffset = roffset&0x3ff;
}

void CIspBlackLevel326::SetG1Offset(int g1offset)
{
	this->m_g1Offset = g1offset&0x3ff;
}

void CIspBlackLevel326::SetG2Offset(int g2offset)
{
	this->m_g2Offset = g2offset&0x3ff;
}

void CIspBlackLevel326::SetBOffset(int boffset)
{
	this->m_bOffset = boffset&0x3ff;
}

int CIspBlackLevel326::GetROffset(void)
{
	return this->m_rOffset;
}

int CIspBlackLevel326::GetG1Offset(void)
{
	return this->m_g1Offset;
}

int CIspBlackLevel326::GetG2Offset(void)
{
	return this->m_g2Offset;
}

int CIspBlackLevel326::GetBOffset(void)
{
	return this->m_bOffset;
}

int CIspBlackLevel326::Write(char *buf, int len)
{
	if(len == 0)
		return 0;
	this->m_Linebuf.ClearPoint();
	len = this->m_Linebuf.Write(buf, len);
	
	return len;
}

int CIspBlackLevel326::Read(char *buf, int len)
{
	int	minVal = 0, val;
	int	k;
	int	temp;

	temp = this->m_gpixel;
	len /= this->m_unit;
	len = (this->m_width > len) ? len : this->m_width;

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
			*((char*)buf+k) = (char)(((val<<2) > minVal) ? ((((val<<2)-minVal)>>2) > 255 ? 255 : (((val<<2)-minVal)>>2)) : 0);
		this->m_gpixel = InverseBit(this->m_gpixel);
	}
	this->m_bgline = InverseBit(this->m_bgline);
	this->m_gpixel = InverseBit(temp);

	return len*this->m_unit;
}

int CIspBlackLevel326::Process(char *src, char *dst)
{
	int	size = this->m_width*this->m_height*this->m_unit;
	int size1, temp;
	int width = this->m_width, pos = 0, pos1 = 0;

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
	/*for(i = 0; i < this->m_height; i++)
	{
		len = this->Write(src+i*this->m_width*this->m_unit, this->m_width*2);
		len = this->Read(dst+i*this->m_width*this->m_unit, this->m_width*2);
	}
	return i;*/
}

//==================== class CIspLensFallOff326 =========================

CIspLensFallOff326::CIspLensFallOff326(int width, int height, int unit)
{
	this->m_lensCx = 0x140;
	this->m_lensCy = 0xf0;
	this->m_lensFocus = 0x43;
	this->m_line = 0;
	this->SetSize(width, height, unit);
}

void CIspLensFallOff326::SetSize(int width, int height, int unit)
{
	this->m_width = width;
	this->m_height = height;
	this->m_unit = unit;
	this->m_Linebuf.SetSize(width, 0, unit);
}

int CIspLensFallOff326::GetLensFocus(void)
{
	return this->m_lensFocus;
}

int CIspLensFallOff326::GetLensX(void)
{
	return this->m_lensCx;
}

int CIspLensFallOff326::GetLensY(void)
{
	return this->m_lensCy;
}

void CIspLensFallOff326::SetLensFocus(int f)
{
	this->m_lensFocus = f&0x1fff;
}

void CIspLensFallOff326::SetLensX(int x)
{
	this->m_lensCx = x&0xfff;
}

void CIspLensFallOff326::SetLensY(int y)
{
	this->m_lensCy = y&0xfff;
}

int CIspLensFallOff326::Write(char *buf, int len)
{
	if(len == 0)
		return 0;
	this->m_Linebuf.ClearPoint();
	len = this->m_Linebuf.Write(buf, len);

	return len;
}

int CIspLensFallOff326::Read(char *buf, int len)
{
	int	val1, val2;
	int	k;
	
	val2 = this->m_line+1;
	val2 -= this->m_lensCy;
	val2 *= val2;

	len /= this->m_unit;
	len = (this->m_width > len) ? len : this->m_width;

	for(k = 0; k < len; k++)
	{
		//fomula:
		//	(1+(x^2+y^2)/f^2) ^ 2
		val1 = k+1;				//In hw, k start from 1
		val1 -= this->m_lensCx;
		val1 *= val1;

		val1 += val2;
		val1 *= this->m_lensFocus;
		val1 >>= 18;		//because "1/f2<<24" change to "1/f2<<26", so ">>16" => ">>18"
		val1 += 256;
		val1 *= val1;
		val1 *= this->m_Linebuf.Read(k);
		val1 >>= 16;
		if(this->m_unit == 1)
			*((char*)buf+k) = (char)(val1 > 255 ? 255 : val1 > 0 ? val1 : 0);
		else
			*((short*)buf+k) = (short)(val1 > 1023 ? 1023 : val1 > 0 ? val1 : 0);
	}
	this->m_line++;

	return len*this->m_unit;
}

int CIspLensFallOff326::Process(char *src, char *dst)
{
	int	size = this->m_width*this->m_height*this->m_unit;
	int size1, temp;
	int width = this->m_width, pos = 0, pos1 = 0;

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

//==================== class CIspDpdDpc326 =========================

CIspDpdDpc326::CIspDpdDpc326(int width, int height, int unit, int ver)
{
//	int table[17] = {0x20, 0x20, 0x30, 0x38, 0x40, 0x50, 0x60, 0x70, 0x80, 0x90, 0xa0, 0xb0, 0xc0, 0xd0, 0xe0, 0xf0, 0xff};
	int table[17] = {0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10};

	this->m_ver = ver;
	this->m_line = 0;
	this->m_curline = 0;
	this->m_pos = 0;
	this->SetBGLine(0x1);
	this->SetGpixel(0x0);
	this->SetDpdThd(0x20);
	this->SetNoiseTable(table);
	this->SetSize(width, height, unit);
	
}

void CIspDpdDpc326::SetSize(int width, int height, int unit)
{
	int i;

	this->m_width = width;
	this->m_height = height;
	this->m_unit = unit;
	for(i = 0; i < 5; i++)
	{
		//this->m_Linebuf[i].SetSize(width, 2, this->m_unit);
		this->m_Linebuf[i].SetSize(width, 0, this->m_unit);
		this->m_pLinebuf[i] = &(this->m_Linebuf[i]);
	}
}

void CIspDpdDpc326::SetNoiseTable(int *val) 
{
	int i;
	
	for(i = 0; i < 17; i++)
	{
		this->m_noisetable[i] = val[i]&0xff;
	}
}

int* CIspDpdDpc326::GetNoiseTable(void)
{
	return this->m_noisetable;
}

void CIspDpdDpc326::SetDpdThd(int thd)
{
	this->m_dpdthd = thd&0xff;
}

int CIspDpdDpc326::GetDpdThd(void)
{
	return this->m_dpdthd;
}

void CIspDpdDpc326::SetBGLine(int bgline)
{
	this->m_bgline = bgline&0x1;
}

void CIspDpdDpc326::SetGpixel(int gpixel)
{
	this->m_gpixel = gpixel&0x1;
}

int CIspDpdDpc326::GetBGLine()
{
	return this->m_bgline;
}

int CIspDpdDpc326::GetGpixel()
{
	return this->m_gpixel;
}

int CIspDpdDpc326::Write(char *buf, int len)
{
	int pos = 0, pos1;

	if(len == 0)
		return 0;
	if(this->m_line & LAST3X3LINES)
		return 0;
	if((this->m_line&0xf) == 3)
	{
		//this->m_curline++;
		return this->m_pLinebuf[4]->Write(buf, len);
	}
	while(this->m_line < 3)
	{
		pos1 = this->m_pLinebuf[2+this->m_line]->Write(buf+pos, len);
		len -= pos1;
		pos += pos1;
		if(pos1 == 0)
			this->m_line++;
		if(this->m_line == 3)
			this->m_line |= FIRST3X3LINES;
		if(len == 0)
			break;
	}
	//if(this->m_line == 3)
	//{
	//	this->m_Linebuf[0] = this->m_Linebuf[4];
	//	this->m_Linebuf[1] = this->m_Linebuf[3];
	//}
	return pos;
}

int CIspDpdDpc326::Read(char *buf, int len)
{
	if( (((this->m_line & 0xf) == 0) || ((this->m_line&FIRST3X3LINES) == 0)) && (this->m_pLinebuf[4]->IsFull() == 0) )
		return 0;
	this->LastLinePro();
	if(!this->m_ver)
		len = this->DpdProcess(buf, len);
	else
		len = this->DpdProcessNew(buf, len);
	this->LoopLinebuf();
	this->m_pos %= this->m_width;
	return len; 
}

int CIspDpdDpc326::DpdProcess(char *buf, int len)
{
	int p[5], gpixel, max = 255;

	if(this->m_unit == 2)
		max = 1024;
	len /= this->m_unit;
	len = ((this->m_width - this->m_pos) > len) ? len : (this->m_width - this->m_pos);
	if((this->m_curline < 2) || (this->m_curline > this->m_height-3))
	{
		memcpy(buf, this->m_pLinebuf[2]->m_buf, len*this->m_unit);
		this->m_pos = this->m_width;
		return len*this->m_unit;
	}
	
	gpixel = (this->m_gpixel + this->m_pos)%2;
	
	for(; this->m_pos < len; this->m_pos++)	
	{
		if((this->m_pos < 2) || (this->m_pos > this->m_width -3))
			if(this->m_unit == 1)
				*((char*)buf+this->m_pos) = (char)this->m_pLinebuf[2]->Read(this->m_pos);
			else
				*((short*)buf+this->m_pos) = (short)this->m_pLinebuf[2]->Read(this->m_pos);
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
	//this->m_gpixel = InverseBit(this->m_gpixel);
	//this->m_bgline = InverseBit(this->m_bgline);
	return this->m_pos*this->m_unit;
}

int CIspDpdDpc326::DpdProcessNew(char *buf, int len)
{
	int p[9], gpixel, max = 255;

	if(this->m_unit == 2)
		max = 1024;
	len /= this->m_unit;
	len = ((this->m_width - this->m_pos) > len) ? len : (this->m_width - this->m_pos);
	if((this->m_curline < 2) || (this->m_curline > this->m_height-3))
	{
		memcpy(buf, this->m_pLinebuf[2]->m_buf, len*this->m_unit);
		this->m_pos = this->m_width;
		return len*this->m_unit;
	}
	
	gpixel = (this->m_gpixel + this->m_pos)%2;
	
	for(; this->m_pos < len; this->m_pos++)	
	{
		if((this->m_pos < 2) || (this->m_pos > this->m_width -3))
			if(this->m_unit == 1)
				*((char*)buf+this->m_pos) = (char)this->m_pLinebuf[2]->Read(this->m_pos);
			else
				*((short*)buf+this->m_pos) = (short)this->m_pLinebuf[2]->Read(this->m_pos);
		else
		{
			p[0] = this->m_pLinebuf[0]->Read(this->m_pos);
			p[1] = this->m_pLinebuf[2]->Read(this->m_pos-2);
			p[2] = this->m_pLinebuf[2]->Read(this->m_pos);
			p[3] = this->m_pLinebuf[2]->Read(this->m_pos+2);
			p[4] = this->m_pLinebuf[4]->Read(this->m_pos);
			p[5] = this->m_pLinebuf[0]->Read(this->m_pos-2);
			p[6] = this->m_pLinebuf[0]->Read(this->m_pos+2);
			p[7] = this->m_pLinebuf[4]->Read(this->m_pos+2);
			p[8] = this->m_pLinebuf[4]->Read(this->m_pos-2);
			if(this->m_unit == 1)
				*((char*)buf+this->m_pos) = (char)this->GetDpdNoiseNew(p, gpixel);
			else
				*((short*)buf+this->m_pos) = (short)this->GetDpdNoiseNew(p, gpixel);
		}
		gpixel = InverseBit(gpixel);
	}
	//this->m_gpixel = InverseBit(this->m_gpixel);
	//this->m_bgline = InverseBit(this->m_bgline);
	return this->m_pos*this->m_unit;
}

int CIspDpdDpc326::GetDpdNoise(int *p, int gpixel)
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
		//avg /= 3;

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

int CIspDpdDpc326::GetDpdNoiseNew(int *p, int gpixel)
{
	int DV, DH, DDL, DDR, thd, avg, var;
	int pixel;
	int i;
	int diff02, diff12, diff32, diff42, diff52, diff62, diff72, diff82;

	pixel = p[2];
	if(this->m_unit == 1)
	{
		for(i = 0; i < 5; i++)
			p[i] <<= 2;
	}
	DV = abs((p[2] << 1) - p[0] - p[4]);
	DH = abs((p[2] << 1) - p[1] - p[3]);
	DDL = abs((p[2] << 1) - p[5] - p[7]);
	DDR = abs((p[2] << 1) - p[6] - p[8]);
	thd = (this->m_dpdthd<<2) + 3;
	diff02 = abs(p[2] - p[0]);
	diff12 = abs(p[2] - p[1]);
	diff32 = abs(p[2] - p[3]);
	diff42 = abs(p[2] - p[4]);
	diff52 = abs(p[2] - p[5]);
	diff62 = abs(p[2] - p[6]);
	diff72 = abs(p[2] - p[7]);
	diff82 = abs(p[2] - p[8]);
	
	if((diff02<thd) || (diff12<thd) || (diff32<thd) || (diff42<thd)||		//not dead pixel
		(diff52<thd)||(diff62<thd) || (diff72<thd) || (diff82<thd))
	{
		if ((DV < DH) && (DV < DDL) && (DV < DDR))
		{
			avg = p[0] + p[2] + p[4];
			avg *= 21;
			avg >>= 6;
			//avg /= 3;

			var = abs(p[0] - avg) + abs(p[2] - avg) + abs(p[4] - avg);
			//var *= 21;
			//var >>= 6;
			//var /= 3;
			//avg = (pix0 + pix2 + pix4) / 3;
			//var = (abs(pix0 - avg) + abs(pix2 - avg) + abs(pix4 - avg)) / 3;
		}
		else if((DH < DV) && (DH < DDL) && (DH < DDR))
		{
			avg = p[1] + p[2] + p[3];
			avg *= 21;
			avg >>= 6;

			var = abs(p[1] - avg) + abs(p[2] - avg) + abs(p[3] - avg);
			//var *= 21;
			//var >>= 6;
		}
		else if((DDL < DH) && (DDL < DV) && (DDL < DDR))
		{
			avg = p[5] + p[2] + p[7];
			avg *= 21;
			avg >>= 6;

			var = abs(p[5] - avg) + abs(p[2] - avg) + abs(p[7] - avg);
			//var *= 21;
			//var >>= 6;
		}
		else
		{
			avg = p[6] + p[2] + p[8];
			avg *= 21;
			avg >>= 6;
				
			var = abs(p[6] - avg) + abs(p[2] - avg) + abs(p[8] - avg);
			//var *= 21;
			//var >>= 6;
		}
		var >>= 2;		//noise compare is 8bit
		if(this->m_unit == 1)
			avg >>= 2;
		if(var < GetNoiseValue((p[2]>>2), gpixel))
			return avg;

		return pixel;
	}
	else	//dead pixel
	{
		if ((DV < DH) && (DV < DDL) && (DV < DDR))
			var = (p[0] + p[4]) >> 1;
		else if((DH < DV) && (DH < DDL) && (DH < DDR))
			var = (p[1] + p[3]) >> 1;
		else if((DDL < DH) && (DDL < DV) && (DDL < DDR))
			var = (p[5] + p[7]) >> 1;
		else
			var = (p[6] + p[8]) >> 1;
		
		if(this->m_unit == 1)
			return var>>2;
		else
			return var;
	}
}

int CIspDpdDpc326::GetNoiseValue(int p, int gpixel)
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
	//return result;
	return gpixel ? result : result >> 1;
}

void CIspDpdDpc326::LastLinePro(void)
{
	if(this->m_pLinebuf[4]->IsFull())
		return;
	this->m_line |= LAST3X3LINES;
	this->m_line--;
	this->m_pLinebuf[4] = this->m_pLinebuf[2];
}

void CIspDpdDpc326::LoopLinebuf(void)
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
	this->m_curline++;
}

int CIspDpdDpc326::Process(char *src, char *dst)
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

//============== class CIspDigitalGain326 ========================

CIspDigitalGain326::CIspDigitalGain326(int width, int height, int unit)
{
	this->m_line = 0;
	this->SetBGLine(0x1);
	this->SetGpixel(0x1);
	this->SetRGain(0x40);
	this->SetBGain(0x40);
	this->SetGGain(0x40);
	this->SetGlobalGain(0x40);
	this->SetGlobalEn(1);
	this->SetRgbEn(1);
	this->SetGainStep(0);
	this->SetSize(width, height, unit);
}

void CIspDigitalGain326::SetSize(int width, int height, int unit)
{
	this->m_width = width;
	this->m_height = height;
	this->m_unit = unit;
	this->m_Linebuf.SetSize(width, 0, unit);
}

void CIspDigitalGain326::SetBGLine(int bgline)
{
	this->m_bgline = bgline&0x1;
}

void CIspDigitalGain326::SetGpixel(int gpixel)
{
	this->m_gpixel = gpixel&0x1;
}

int CIspDigitalGain326::GetBGLine()
{
	return this->m_bgline;
}

int CIspDigitalGain326::GetGpixel()
{
	return this->m_gpixel;
}

void CIspDigitalGain326::SetRGain(int gain)
{
	this->m_rGain = gain&0xff;
}

int CIspDigitalGain326::GetRGain(void)
{
	return this->m_rGain;
}

void CIspDigitalGain326::SetBGain(int gain)
{
	this->m_bGain = gain&0xff;
}

int CIspDigitalGain326::GetBGain(void)
{
	return this->m_bGain;
}

void CIspDigitalGain326::SetGGain(int gain)
{
	this->m_gGain = gain&0xff;
}

int CIspDigitalGain326::GetGGain(void)
{
	return this->m_gGain;
}

void CIspDigitalGain326::SetGlobalGain(int gain)
{
	this->m_globalGain = gain&0xff;
}

int CIspDigitalGain326::GetGlobalGain(void)
{
	return this->m_globalGain;
}

void CIspDigitalGain326::SetGlobalEn(int val)
{
	this->m_globalEn = val&0x1;
}

void CIspDigitalGain326::SetRgbEn(int val)
{
	this->m_rgbEn = val&0x1;
}

void CIspDigitalGain326::SetGainStep(int step)
{
	this->m_gainStep = step&0xff;
}

int CIspDigitalGain326::GetGainStep(void)
{
	return this->m_gainStep;
}

int CIspDigitalGain326::Write(char *buf, int len)
{
	if(len == 0)
		return 0;
	this->m_Linebuf.ClearPoint();
	len = this->m_Linebuf.Write(buf, len);

	return len;
}

int CIspDigitalGain326::Read(char *buf, int len)
{
	int k;
	int	temp, max = 255;
	int GlobalEn = 0, RgbEn = 0;
	int Gpixel = this->m_gpixel;
	int GlobalGain, rGain, gGain1, gGain2, bGain, rgbGain;

	len /= this->m_unit;
	len = (this->m_width > len) ? len : this->m_width;

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
	
	for(k = 0; k < len; k++)
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
	
	return len*this->m_unit;
}

int CIspDigitalGain326::Process(char *src, char *dst)
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

//============== class CIspCfaExtract326 ========================

CIspCfaExtract326::CIspCfaExtract326(int width, int height, int mode, int unit)
{
	int i;
	char coef[25] = {0, 0, 0, 0, 0, 
					0, -1, -1, -1, 0,
					0, -1, 8, -1, 0,
					0, -1, -1, -1, 0,
					0, 0, 0, 0, 0};
	/*char coef[25] = {-1, 0, -1, 0, -1, 
					0, 0, 0, 0, 0,
					-1, 0, 8, 0, -1,
					0, 0, 0, 0, 0,
					-1, 0, -1, 0, -1};*/


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
		this->SetFemmax(0xff);
		this->SetFemmin(0x05);
		this->SetFemx1(0x05);
		this->SetFemx2(0x0f);
		this->SetCoefEdge(coef);
		this->SetDivEdge(8);
	}
}

void CIspCfaExtract326::SetSize(int width, int height, int unit)
{
	int i;

	for(i = 0; i < 9; i++)
		this->m_Linebuf[i].SetSize(width, 0, unit);
	this->m_unit = unit;
	this->m_width = width;
	this->m_height = height;
	this->Clear();
}

void CIspCfaExtract326::SetBGLine(int bgline)
{
	this->m_GBline = bgline&0x1;
}

void CIspCfaExtract326::SetGpixel(int gpixel)
{
	this->m_Gfirst = gpixel&0x1;
}

int CIspCfaExtract326::GetBGLine()
{
	return this->m_GBline;
}

int CIspCfaExtract326::GetGpixel()
{
	return this->m_Gfirst;
}

void CIspCfaExtract326::SetOrder(int order)
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

void CIspCfaExtract326::Clear(void)
{
	this->m_line = 0;
	this->m_pos = 0;
	this->m_curline = 0;
}

int	CIspCfaExtract326::Write(char *buf, int len)
{
	int pos = 0, pos1;

	if(len == 0)
		return 0;
	if(this->m_line & LAST5X5LINES)
		return 0;
	if(this->m_line == 5)
	{
		//this->m_curline++;
		return this->m_pLinebuf[8]->Write(buf, len);
	}
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
	/*if(this->m_line == 5)
	{
		this->m_Linebuf[0] = this->m_Linebuf[4];
		this->m_Linebuf[1] = this->m_Linebuf[5];
		this->m_Linebuf[2] = this->m_Linebuf[4];
		this->m_Linebuf[3] = this->m_Linebuf[5];
	}*/
	return pos;
}

int	CIspCfaExtract326::Read(char *buf, int len)
{
	if( ((this->m_line & 0xf) == 0) && (this->m_pLinebuf[8]->IsFull() == 0) )
		return 0;
	this->LastLinePro();
	len = this->Interpolate(buf, len);
	this->LoopLinebuf();
	this->m_pos %= this->m_width;
	return len;
}

int	CIspCfaExtract326::Interpolate(char *buf, int len)
{
	int gpixel, val[3], i, pos = 0, max = 255;
	CBaseConvert tools;

	if(this->m_unit == 2)
		max = 1024;
	if(!this->m_mode)		//cfa
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
			if(this->m_pos != this->m_width - 1)
			{
				if(this->m_mode == 1) //old edge
					val[0] = this->GetEdge(gpixel);
				else		//new edge
					val[0] = this->GetEdge5x5(gpixel);
			}
			//tools.setvalue(buf+pos, val[0], this->m_unit);		
			tools.setvalue(buf+pos, val[0], 2);		
			pos += 2;	//this->m_unit;
			gpixel = gpixel ? 0 : 1;
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
					val[2] = this->m_pLinebuf[4]->Read(this->m_pos);
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
					val[0] = this->m_pLinebuf[4]->Read(this->m_pos);
					val[1] = this->m_pGblock[2][2];
					val[2] = this->GetRfromBG(this->m_pos); 
				}
			}
			gpixel = gpixel ? 0 : 1;
			for(i = 0; i <= 2; i++, pos += this->m_unit)
			{
				val[i] = (val[i] < 0) ? 0 : ((val[i] > max) ? max : val[i]);
				tools.setvalue(buf+pos, val[i], this->m_unit);
			}
		}
	}
	return pos;
}

int	CIspCfaExtract326::GetRfromBG(int pos)		//GB line, get R at B pixel
{
	int x1, x2;
	int max = 255;

	if(this->m_curline < 4)
	{
		x1 = this->m_pLinebuf[5]->Read(this->EdgeCol(pos-1)) + this->m_pLinebuf[5]->Read(this->EdgeCol(pos-1)) + 
		this->m_pLinebuf[5]->Read(this->EdgeCol(1+pos)) + this->m_pLinebuf[5]->Read(this->EdgeCol(1+pos));
	}
	else if(this->m_curline > this->m_height -5)
	{
		x1 = this->m_pLinebuf[3]->Read(this->EdgeCol(pos-1)) + this->m_pLinebuf[3]->Read(this->EdgeCol(pos-1)) + 
		this->m_pLinebuf[3]->Read(this->EdgeCol(1+pos)) + this->m_pLinebuf[3]->Read(this->EdgeCol(1+pos));
	}
	else
	{
		x1 = this->m_pLinebuf[3]->Read(this->EdgeCol(pos-1)) + this->m_pLinebuf[5]->Read(this->EdgeCol(pos-1)) + 
			this->m_pLinebuf[3]->Read(this->EdgeCol(1+pos)) + this->m_pLinebuf[5]->Read(this->EdgeCol(1+pos));
	}
	if(this->m_pos < 2)		//hx: hw implement
		x2 = this->m_pGblock[3][3] + this->m_pGblock[3][3] + 
		this->m_pGblock[3][3] + this->m_pGblock[3][3];
	else if(this->m_pos < 4 && this->m_pos >=2)
		x2 = this->m_pGblock[3][3] + this->m_pGblock[3][1] + 
		this->m_pGblock[3][3] + this->m_pGblock[3][1];
	else if(this->m_pos > this->m_width -3)
		x2 = this->m_pGblock[1][3] + this->m_pGblock[1][1] + 
		this->m_pGblock[1][3] + this->m_pGblock[1][1];
	//else if(this->m_pos > this->m_width -5)
	//	x2 = this->m_pGblock[1][1] + this->m_pGblock[1][3] + 
	//	this->m_pGblock[1][3] + this->m_pGblock[1][1];
	else
		x2 = this->m_pGblock[1][1] + this->m_pGblock[3][3] + 
		this->m_pGblock[1][3] + this->m_pGblock[3][1];
#if 0
	x1 -= x2;
	x1 >>= 2;
	x1 += this->m_pGblock[2][2];
#else
	x1 = (this->m_pGblock[2][2]<<2) + x1 - x2;
	x1 >>= 2;
#endif
	if(this->m_unit == 2)
		max = 1023;
	return (x1 > max) ? max : (x1 < 0 ? 0 : x1);
}

int	CIspCfaExtract326::GetRfromGR(int pos)		//GR line, get R at G pixel
{
	int x1, x2;
	int max = 255;

	x1 = this->m_pLinebuf[4]->Read(this->EdgeCol(pos-1)) + this->m_pLinebuf[4]->Read(this->EdgeCol(1+pos));
	if(this->m_pos < 4)
		x2 = this->m_pGblock[3][2] + this->m_pGblock[3][2];
	else if(this->m_pos > this->m_width -3)
		x2 = this->m_pGblock[1][2] + this->m_pGblock[1][2];
	else
		x2 = this->m_pGblock[1][2] + this->m_pGblock[3][2];
#if 0
	x1 -= x2;
	x1 >>= 1;
	x1 += this->m_pGblock[2][2];
#else
	x1 = (this->m_pGblock[2][2]<<1) + x1 - x2;
	x1 >>= 1;
#endif
	if(this->m_unit == 2)
		max = 1023;
	return (x1 > max) ? max : (x1 < 0 ? 0 : x1);
}

int CIspCfaExtract326::GetRfromGB(int pos)		//GB line, get R at G pixel
{
	int x1, x2;
	int max = 255;

	if(this->m_curline < 4)
		x1 = this->m_pLinebuf[5]->Read(this->EdgeCol(pos)) + this->m_pLinebuf[5]->Read(this->EdgeCol(pos));
	else if(this->m_curline > this->m_height -5)
		x1 = this->m_pLinebuf[3]->Read(this->EdgeCol(pos)) + this->m_pLinebuf[3]->Read(this->EdgeCol(pos));
	else
		x1 = this->m_pLinebuf[3]->Read(this->EdgeCol(pos)) + this->m_pLinebuf[5]->Read(this->EdgeCol(pos));
	if(this->m_pos < 3 )			//hx: hw implement
		x2 = this->m_pGblock[2][3] + this->m_pGblock[2][3];
	else
		x2 = this->m_pGblock[2][1] + this->m_pGblock[2][3];
//	x2 = this->m_pGblock[2][3] + this->m_pGblock[2][3];
#if 0
	x1 -= x2;
	x1 >>= 1;
	x1 += this->m_pGblock[2][2];
#else
	x1 = (this->m_pGblock[2][2]<<1) + x1 - x2;
	x1 >>= 1;
#endif
	if(this->m_unit == 2)
		max = 1023;
	return (x1 > max) ? max : (x1 < 0 ? 0 : x1);
}

void CIspCfaExtract326::LastLinePro(void)
{
	if(this->m_pLinebuf[8]->IsFull())
		return;
	this->m_line |= LAST5X5LINES;
	this->m_line--;
	this->m_pLinebuf[8] = this->m_pLinebuf[6];
}

void CIspCfaExtract326::LoopLinebuf(void)
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
	this->m_curline++;
}

int CIspCfaExtract326::EdgeLine(int num)
{
	if((this->m_curline < 4 && num < 4) || (this->m_curline > this->m_height -5 && num > 4))
		return 8-num;
	else
		return num;
}

int CIspCfaExtract326::EdgeCol(int num)
{
	if((this->m_pos < 4 && num < this->m_pos))
		return 2*this->m_pos - num;
	else if(this->m_pos > this->m_width -5 && this->m_pos < this->m_width -2 && num > this->m_width - 2)
		return 2*this->m_pos - num + 4;
	else if(this->m_pos > this->m_width-3)
	{
		if(num == this->m_pos +1 || num == this->m_pos +2)
			return 2*this->m_pos - num;
		else if(num == this->m_pos +3 || num == this->m_pos +4)
			return this->m_width -1;
		else
			return num;
	}
	else
		return num;
}

int CIspCfaExtract326::GetGpixel(int col, int row)
{
	int dh, dv, x1, x2;
	int max = 255;

	if( (this->m_Gfirst + col + row) % 2 )	
		return this->m_pLinebuf[this->EdgeLine(col)]->Read(this->EdgeCol(row));

	x1 = this->m_pLinebuf[this->EdgeLine(col)]->Read(this->EdgeCol(row-1)) - this->m_pLinebuf[this->EdgeLine(col)]->Read(this->EdgeCol(row+1));
	x1 = (x1 > 0) ? x1 : -x1;
	x2 = this->m_pLinebuf[this->EdgeLine(col)]->Read(this->EdgeCol(row-2)) + this->m_pLinebuf[this->EdgeLine(col)]->Read(this->EdgeCol(row+2)) - 
		(this->m_pLinebuf[this->EdgeLine(col)]->Read(this->EdgeCol(row)) << 1);
	x2 = (x2 > 0) ? x2 : -x2;
	dh = (x1 << 1) + x2;

	x1 = this->m_pLinebuf[this->EdgeLine(col-1)]->Read(this->EdgeCol(row)) - this->m_pLinebuf[this->EdgeLine(col+1)]->Read(this->EdgeCol(row));
	x1 = (x1 > 0) ? x1 : -x1;
	x2 = this->m_pLinebuf[this->EdgeLine(col-2)]->Read(this->EdgeCol(row)) + this->m_pLinebuf[this->EdgeLine(col+2)]->Read(this->EdgeCol(row)) - 
		(this->m_pLinebuf[this->EdgeLine(col)]->Read(this->EdgeCol(row)) << 1);
	x2 = (x2 > 0) ? x2 : -x2;
	dv = (x1 << 1) + x2;

	if(dh < dv)
	{
		x1 = (this->m_pLinebuf[this->EdgeLine(col)]->Read(this->EdgeCol(row-1)) + this->m_pLinebuf[this->EdgeLine(col)]->Read(this->EdgeCol(row+1))) << 2;
		x2 = (this->m_pLinebuf[this->EdgeLine(col)]->Read(this->EdgeCol(row)) << 1) - this->m_pLinebuf[this->EdgeLine(col)]->Read(this->EdgeCol(row-2)) - 
			this->m_pLinebuf[this->EdgeLine(col)]->Read(this->EdgeCol(row+2));
	}
	else
	{
		x1 = (this->m_pLinebuf[this->EdgeLine(col-1)]->Read(this->EdgeCol(row)) + this->m_pLinebuf[this->EdgeLine(col+1)]->Read(this->EdgeCol(row))) << 2;
		x2 = (this->m_pLinebuf[this->EdgeLine(col)]->Read(this->EdgeCol(row)) << 1) - this->m_pLinebuf[this->EdgeLine(col-2)]->Read(this->EdgeCol(row)) - 
			this->m_pLinebuf[this->EdgeLine(col+2)]->Read(this->EdgeCol(row));
	}
	x1 += x2;
	x1 >>= 3;
	if(this->m_unit == 2)
		max = 1023;
//	return (x1 > max) ? max : x1;
	return (x1 > max) ? max : (x1 < 0 ? 0 : x1);
}

void CIspCfaExtract326::GetFirstGblock(void)
{
	int i, j;

	for(i = 0; i < 5; i++)
		for(j = 0; j < 5; j++)
			this->m_pGblock[i][j] = this->GetGpixel(j+2, i-2);
}

void CIspCfaExtract326::GetNextGblock(void)
{
	int i;
	int *p = this->m_pGblock[0];

	for(i = 0; i < 5; i++)
		this->m_pGblock[0][i] = this->GetGpixel(i+2, this->m_pos+2);
	for(i = 0; i < 4; i++)
		this->m_pGblock[i] = this->m_pGblock[i+1];
	this->m_pGblock[4] = p;
}

//edge extract
void CIspCfaExtract326::SetMode(int mode)
{
	this->m_mode = mode&0x3;
}

int CIspCfaExtract326::GetMode(void)
{
	return this->m_mode;
}

void CIspCfaExtract326::SetFemm(int femm)
{
	this->m_femm = femm&0xff;
}

int CIspCfaExtract326::GetFemm(void)
{
	return this->m_femm;
}

void CIspCfaExtract326::SetFemp(int femp)
{
	this->m_femp = femp&0xff;
}

int CIspCfaExtract326::GetFemp(void)
{
	return this->m_femp;
}

void CIspCfaExtract326::SetFemmin(int femmin)
{
	this->m_femmin = femmin&0xff;
}

int CIspCfaExtract326::GetFemmin(void)
{
	return this->m_femmin;
}

void CIspCfaExtract326::SetFemmax(int femmax)
{
	this->m_femmax = femmax&0xff;
}

int CIspCfaExtract326::GetFemmax(void)
{
	return this->m_femmax;
}

void CIspCfaExtract326::SetFemx1(int femx1)
{
	this->m_femx1 = femx1&0xff;
}

int CIspCfaExtract326::GetFemx1(void)
{
	return this->m_femx1;
}

void CIspCfaExtract326::SetFemx2(int femx2)
{
	this->m_femx2 = femx2&0xff;
}

int CIspCfaExtract326::GetFemx2(void)
{
	return this->m_femx2;
}

void CIspCfaExtract326::SetCoefEdge(char *coef)
{
	char i, j;

	for(i = 0; i < 5; i++)
		for(j = 0; j < 5; j++)
			this->m_coefedge[i][j] = *(coef+i*5+j);
}

void CIspCfaExtract326::SetDivEdge(int div)
{
	this->m_divedge = div;
}

int CIspCfaExtract326::GetEdge(int gpixel)
{
	int x, temp;
	int femm, femp, femmin, femmax, femx1, femx2;
	char ret;
	int flag;
	
	femm = this->m_femm;
	femp = this->m_femp;
	femx1 = this->m_femx1;
	femx2 = this->m_femx2;
	femmin = this->m_femmin;
	femmax = this->m_femmax;

	x = this->m_pGblock[2][2] << 3;
	if(gpixel)
	{
		if(this->m_pos < 4)
		{
			this->m_pGblock[0][0] = this->m_pGblock[4][0];
			this->m_pGblock[0][2] = this->m_pGblock[4][2];
			this->m_pGblock[0][4] = this->m_pGblock[4][4];
		}
		else if(this->m_pos == this->m_width-2 || this->m_pos == this->m_width-1)
		{
			this->m_pGblock[4][0] = this->m_pGblock[0][0];
			this->m_pGblock[4][2] = this->m_pGblock[0][2];
			this->m_pGblock[4][4] = this->m_pGblock[0][4];
		}
		//	temp = this->m_pGblock[2][0] + this->m_pGblock[2][4] + 2*this->m_pGblock[4][0] +
		//		2*this->m_pGblock[4][2] + 2*this->m_pGblock[4][4];
		//else
//			temp = this->m_pGblock[0][0] + this->m_pGblock[0][2] + this->m_pGblock[0][4] +
//				this->m_pGblock[2][0] + this->m_pGblock[2][4] + this->m_pGblock[4][0] +
//				this->m_pGblock[4][2] + this->m_pGblock[4][4];
	}
	else
	{
		if(this->m_pos == 0)
		{
			this->m_pGblock[4][0] = this->m_pGblock[4][4];
			this->m_pGblock[0][0] = this->m_pGblock[4][0];
			this->m_pGblock[0][4] = this->m_pGblock[4][4];
			this->m_pGblock[2][0] = this->m_pGblock[2][4];
			this->m_pGblock[0][2] = this->m_pGblock[4][2];
		}	
		//	temp = 4*this->m_pGblock[4][4] + 2*this->m_pGblock[2][4] + 2*this->m_pGblock[4][2];
		else if(this->m_pos == 1)
		{
			this->m_pGblock[0][0] = this->m_pGblock[4][0];
			this->m_pGblock[0][2] = this->m_pGblock[4][2];
			this->m_pGblock[0][4] = this->m_pGblock[4][4];
			this->m_pGblock[2][0] = this->m_pGblock[2][4];
		}
		else if(this->m_pos == 2 || this->m_pos == 3)
		{
			this->m_pGblock[0][0] = this->m_pGblock[4][0];
			this->m_pGblock[0][2] = this->m_pGblock[4][2];
			this->m_pGblock[0][4] = this->m_pGblock[4][4];
		}
		else if(this->m_pos == this->m_width-2)
		{
			this->m_pGblock[4][0] = this->m_pGblock[0][0];
			this->m_pGblock[4][2] = this->m_pGblock[0][2];
			this->m_pGblock[4][4] = this->m_pGblock[0][4];
		}
		else if(this->m_pos == this->m_width-1)
			return 0;
//			temp = 2*this->m_pGblock[2][4] + 2*this->m_pGblock[4][0] +
//				2*this->m_pGblock[4][2] + 2*this->m_pGblock[4][4];
		//else if(this->m_pos == 2)
		//{
		//	temp = this->m_pGblock[0][2] + this->m_pGblock[0][4] +
		//		2*this->m_pGblock[2][4] + 2*this->m_pGblock[4][0] +
		//		this->m_pGblock[4][2] + this->m_pGblock[4][4];
		//else if(this->m_pos == 3 || this->m_pos == 4)
		//	temp = this->m_pGblock[0][2] + 2*this->m_pGblock[0][4] +
		//		this->m_pGblock[2][0] + this->m_pGblock[2][4] + this->m_pGblock[4][0] +
		//		this->m_pGblock[4][2] + this->m_pGblock[4][4];
		//else
		//	temp = this->m_pGblock[0][0] + this->m_pGblock[0][2] + this->m_pGblock[0][4] +
		//		this->m_pGblock[2][0] + this->m_pGblock[2][4] + this->m_pGblock[4][0] +
		//		this->m_pGblock[4][2] + this->m_pGblock[4][4];
	}
	temp = this->m_pGblock[0][0] + this->m_pGblock[0][2] + this->m_pGblock[0][4] +
			this->m_pGblock[2][0] + this->m_pGblock[2][4] + this->m_pGblock[4][0] +
			this->m_pGblock[4][2] + this->m_pGblock[4][4];
	if(x >= temp)
	{
		flag = 0;
		x -= temp;
	}
	else
	{
		flag = 1;
		x = temp - x;
	}
	x >>= 3;
#if 0
	if( (x > femx2) || ((x + femx2) <= 0) )
		x *= femm;
	else if( (x > femx1) && (x <= femx2) )
	{
		x -= femx1;
		x *= femp;
	}
	else if( (x <= femx1) && ((x+femx1) > 0) )
	{
		x = 0;
	}
	else
	{
		x += femx1;
		x *= femp;
	}
#else
	if(x > femx2)
		x *= femm;
	else if( (x > femx1) && (x <= femx2) )
	{
		x -= femx1;
		x *= femp;
	}
	else
	{
		x = 0;
	}
#endif
//	x /= 16;
#if 0
	x >>= 4;
#else
//	x = (x >= 0) ? x/0x80 : -(abs(x)/0x80);
//	x = (x >= 0) ? x >> 2 : -(abs(x)>>2);
	x /= 16;
	if(flag)
		x = (~x+1);
	x >>= 2;
#endif
//	x >>= 7;		//divided by 128, ??

	//return (char) ( (x > femmax) ? femmax : ((x < -femmin) ? -femmin : x) );
	ret = (char)( (x > femmax) ? femmax : ((x < -femmin) ? -femmin : x));
	//if(ret >= 0)
	if((!flag) || (ret == 0))
		return ((int)ret & 0xff);
	else
		return (((int)ret & 0xff) | 0x100);
	
}

//New 3x3 edge enhance
int CIspCfaExtract326::GetEdge5x5(int gpixel)
{
	int x, temp;
	int femm, femp, femmin, femmax, femx1, femx2;
	char ret;
	int flag;
	
	femm = this->m_femm;
	femp = this->m_femp;
	femx1 = this->m_femx1;
	femx2 = this->m_femx2;
	femmin = this->m_femmin;
	femmax = this->m_femmax;

	if(gpixel)
	{
		if(this->m_pos < 4)
		{
			this->m_pGblock[0][0] = this->m_pGblock[4][0];
			this->m_pGblock[0][2] = this->m_pGblock[4][2];
			this->m_pGblock[0][4] = this->m_pGblock[4][4];
		}
		else if(this->m_pos == this->m_width-2 || this->m_pos == this->m_width-1)
		{
			this->m_pGblock[4][0] = this->m_pGblock[0][0];
			this->m_pGblock[4][2] = this->m_pGblock[0][2];
			this->m_pGblock[4][4] = this->m_pGblock[0][4];
		}
	}
	else
	{
		if(this->m_pos == 0)
		{
			this->m_pGblock[4][0] = this->m_pGblock[4][4];
			this->m_pGblock[0][0] = this->m_pGblock[4][0];
			this->m_pGblock[0][4] = this->m_pGblock[4][4];
			this->m_pGblock[2][0] = this->m_pGblock[2][4];
			this->m_pGblock[0][2] = this->m_pGblock[4][2];
		}	
		else if(this->m_pos == 1)
		{
			this->m_pGblock[0][0] = this->m_pGblock[4][0];
			this->m_pGblock[0][2] = this->m_pGblock[4][2];
			this->m_pGblock[0][4] = this->m_pGblock[4][4];
			this->m_pGblock[2][0] = this->m_pGblock[2][4];
		}
		else if(this->m_pos == 2 || this->m_pos == 3)
		{
			this->m_pGblock[0][0] = this->m_pGblock[4][0];
			this->m_pGblock[0][2] = this->m_pGblock[4][2];
			this->m_pGblock[0][4] = this->m_pGblock[4][4];
		}
		else if(this->m_pos == this->m_width-2)
		{
			this->m_pGblock[4][0] = this->m_pGblock[0][0];
			this->m_pGblock[4][2] = this->m_pGblock[0][2];
			this->m_pGblock[4][4] = this->m_pGblock[0][4];
		}
		else if(this->m_pos == this->m_width-1)
			return 0;
	}
	temp =  this->m_pGblock[0][0]*this->m_coefedge[0][0] + this->m_pGblock[0][1]*this->m_coefedge[0][1] +
			this->m_pGblock[0][2]*this->m_coefedge[0][2] + this->m_pGblock[0][3]*this->m_coefedge[0][3] +
			this->m_pGblock[0][4]*this->m_coefedge[0][4] + this->m_pGblock[1][0]*this->m_coefedge[1][0] +
			this->m_pGblock[1][1]*this->m_coefedge[1][1] + this->m_pGblock[1][2]*this->m_coefedge[1][2] +
			this->m_pGblock[1][3]*this->m_coefedge[1][3] + this->m_pGblock[1][4]*this->m_coefedge[1][4] +
			this->m_pGblock[2][0]*this->m_coefedge[2][0] + this->m_pGblock[2][1]*this->m_coefedge[2][1] +
			this->m_pGblock[2][2]*this->m_coefedge[2][2] + this->m_pGblock[2][3]*this->m_coefedge[2][3] +
			this->m_pGblock[2][4]*this->m_coefedge[2][4] + this->m_pGblock[3][0]*this->m_coefedge[3][0] +
			this->m_pGblock[3][1]*this->m_coefedge[3][1] + this->m_pGblock[3][2]*this->m_coefedge[3][2] +
			this->m_pGblock[3][3]*this->m_coefedge[3][3] + this->m_pGblock[3][4]*this->m_coefedge[3][4] +
			this->m_pGblock[4][0]*this->m_coefedge[4][0] + this->m_pGblock[4][1]*this->m_coefedge[4][1] +
			this->m_pGblock[4][2]*this->m_coefedge[4][2] + this->m_pGblock[4][3]*this->m_coefedge[4][3] +
			this->m_pGblock[4][4]*this->m_coefedge[4][4];
	if(temp >= 0)
	{
		flag = 0;
		x = temp;
	}
	else
	{
		flag = 1;
		x = -temp;
	}
	//x >>= 3;
	x /= this->m_divedge;
#if 0
	if( (x > femx2) || ((x + femx2) <= 0) )
		x *= femm;
	else if( (x > femx1) && (x <= femx2) )
	{
		x -= femx1;
		x *= femp;
	}
	else if( (x <= femx1) && ((x+femx1) > 0) )
	{
		x = 0;
	}
	else
	{
		x += femx1;
		x *= femp;
	}
#else
	if(x > femx2)
		x *= femm;
	else if( (x > femx1) && (x <= femx2) )
	{
		x -= femx1;
		x *= femp;
	}
	else
	{
		x = 0;
	}
#endif
//	x /= 16;
#if 0
	x >>= 4;
#else
//	x = (x >= 0) ? x/0x80 : -(abs(x)/0x80);
//	x = (x >= 0) ? x >> 2 : -(abs(x)>>2);
	x /= 16;
	if(flag)
		x = (~x+1);
	x >>= 2;
#endif
//	x >>= 7;		//divided by 128, ??

	//return (char) ( (x > femmax) ? femmax : ((x < -femmin) ? -femmin : x) );
	ret = (char)( (x > femmax) ? femmax : ((x < -femmin) ? -femmin : x));
	//if(ret >= 0)
	if((!flag) || (ret == 0))
		return ((int)ret & 0xff);
	else
		return (((int)ret & 0xff) | 0x100);
	
}

int CIspCfaExtract326::Process(char *src, char *dst)
{
	int size, pos = 0, pos1 = 0, size1, width = 0;
	int temp;

	size = this->m_width * this->m_height * this->m_unit;
	if(this->m_mode)
		size1 = size;
	else
		size1 = size * 3;
	temp = size1;
	//this->SetOrder(RAW_BG);
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

//============== class CIspColorCorrect326 ========================

CIspColorCorrect326::CIspColorCorrect326(int width, int height, int unit)
{
	//int colorMatrix[9] = {0x40, 0xf3, 0xf3, 0xf3, 0x40, 0xf3, 0xf3, 0xf3, 0x40};
	int colorMatrix[9] = {0x60, 0xf9, 0xe5, 0xe7, 0x50, 0x05, 0xf3, 0xe6, 0x5e};
	int offset[3] = {0, 0, 0};

	this->SetSize(width, height, unit);
	this->SetColorMatrix(colorMatrix);
	this->SetColorOffset(offset);
}

void CIspColorCorrect326::SetSize(int width, int height, int unit)
{
	this->m_width = width;
	this->m_height = height;
	this->m_unit = unit;
}

void CIspColorCorrect326::SetColorMatrix(int *p)
{
	int i, j;

	for(i = 0; i < 3; i++)
		for(j = 0; j < 3; j++)
			this->m_colormatrix[i][j] = p[i*3+j]&0xff;
}

void CIspColorCorrect326::SetColorOffset(int *p)
{
	int i;

	for(i = 0; i < 3; i++)
		this->m_offset[i] = p[i]&0xff;
}

int CIspColorCorrect326::Mul10(int p, int c)
{
	int val;

	p &= 0x3ff;
	c &= 0xff;
	if(c&0x80)	//minus
	{
		c = (~(c-1))&0xff;
		val = -p*c;
	}
	else
		val = p*c;
	return val;
}
unsigned char CIspColorCorrect326::GetSingleBit(unsigned char X, int whichBit)
{
	return (( X & (1<<whichBit) ) != 0 ) ;
}

int CIspColorCorrect326::ColMul(int P, unsigned char A)
{
	int	AxP;	// [11:0], output
	int	Neg128;
	unsigned char AN, AbsValNeg, AF;	// [7:0]
	// all of the following are [11:0]
	int	A1p01, A1p02, A1p04, A1p08, A1p16, A1p32,
					s1p01N1p02, s1p04N1p08, s1p16N1p32,
					s1p64, S01x08, S01x64, S01x64P, S01x64N,
					S16x64;

	Neg128 = (A == 0x80);
	AN = (unsigned char)(~(A - 1));

//	assign  AbsValNeg = Neg128 ? 8'hff : {A[7],AN[6:0]};	
	AbsValNeg = Neg128 ? 0xff : ((GetSingleBit(A,7)<<7)+(AN&0x7f));

//	assign  AF = A[7] ? AbsValNeg : A;	
	AF = GetSingleBit(A,7) ? AbsValNeg : A;

//	wire    [9:0]   A1p01 = AF[6] ? {1'b0,P,1'b0} : 10'h000;
//	wire    [9:0]   A1p02 = AF[5] ? {2'b0,P} : 10'h000;
//	wire    [9:0]   s1p01N1p02 = A1p01 + A1p02;
	A1p01 = GetSingleBit(AF,6) ? (P<<1) : 0;
	A1p02 = GetSingleBit(AF,5) ? P : 0;
	s1p01N1p02 = (A1p01 + A1p02) & 0xfff;
	
//	wire    [9:0]   A1p04 = AF[4] ? {3'b0,P[7:1]} : 10'h000;
//	wire    [9:0]   A1p08 = AF[3] ? {4'b0,P[7:2]} : 10'h000;
//	wire    [9:0]   s1p04N1p08 = A1p04 + A1p08;
	A1p04 = GetSingleBit(AF,4) ? (P>>1) : 0 ;
	A1p08 = GetSingleBit(AF,3) ? (P>>2) : 0 ;
	s1p04N1p08 = (A1p04 + A1p08) & 0xfff ;
	
//	wire    [9:0]   A1p16 = AF[2] ? {5'b0,P[7:3]} : 10'h000;
//	wire    [9:0]   A1p32 = AF[1] ? {6'b0,P[7:4]} : 10'h000;
//	wire    [9:0]   s1p16N1p32 = A1p16 + A1p32;
	A1p16 = GetSingleBit(AF,2) ? (P>>3) : 0 ;
	A1p32 = GetSingleBit(AF,1) ? (P>>4) : 0 ;
	s1p16N1p32 = (A1p16 + A1p32) & 0xfff ;
	
//	wire    [9:0]   s1p64 = AF[0] ? P[7:5] : 10'h000;
	s1p64 = GetSingleBit(AF,0) ? (P>>5) : 0 ;
	
//	wire    [9:0]   S01x08 = s1p01N1p02 + s1p04N1p08; 
//	wire    [9:0]   S16x64 = s1p16N1p32 + s1p64;
	S01x08 = (s1p01N1p02 + s1p04N1p08) & 0xfff; 
	S16x64 = (s1p16N1p32 + s1p64) & 0xfff;
	
//	wire    [9:0]   S01x64 = S01x08 + S16x64;
	S01x64 = (S01x08 + S16x64) & 0xfff;

//	wire    [9:0]   S01x64P = {1'b0,S01x64[9:1]};
//	wire    [9:0]   S01x64N =  ~S01x64P + 1;
	S01x64P = (S01x64>>1) & 0xfff ;
	S01x64N =  (~S01x64P + 1) & 0xfff;
	
//	wire    [9:0]   AxP = AF[7] ? S01x64N : S01x64P;
	AxP = ((GetSingleBit(AF,7) ? S01x64N : S01x64P) & 0xfff);

	return AxP ;
}

int CIspColorCorrect326::ColSum(int AxR, 
								int AxG, 
								int AxB, 
								int ColOfst)
{
	int	PxSum ;
	int	SUMrg, SUMbo, SUMrgbo;

	SUMrg =  (((AxR&0x800)!=0)<<12)+AxR
			+(((AxG&0x800)!=0)<<12)+AxG ;
	SUMrg &= 0x1fff ;	// SUMrg is [12:0]
	
	SUMbo =  (((AxB&0x800)!=0)<<12)+AxB
			+(((ColOfst&0x80)!=0)?0x70:0x00) + ColOfst ;
	SUMbo &= 0x1fff ;	// SUMbo is [10:0]
	
	SUMrgbo =(((SUMrg&0x1000)!=0)<<13)+SUMrg
			+(((SUMbo&0x1000)!=0)<<13)+SUMbo ;
	
	if (SUMrgbo & (1<<13))	// SignBit, [11]
		PxSum = 0 ;
	else if (SUMrgbo & 0x1c00)		// OvFlag, |[10:8]
		PxSum = 0x3ff ;
	else
	 	PxSum = SUMrgbo & 0x3ff ;

	return PxSum ;
}

int CIspColorCorrect326::Write(char *buf, int len)
{
	len = 3*this->m_unit;
	if(this->m_unit == 1)
	{
		this->m_r = buf[0]&0xff;
		this->m_g = buf[1]&0xff;
		this->m_b = buf[2]&0xff;
	}
	else
	{
		this->m_r = *((short*)buf)&0x3ff;
		this->m_g = *((short*)buf+1)&0x3ff;
		this->m_b = *((short*)buf+2)&0x3ff;
	}

	return len;
}

int CIspColorCorrect326::Read(char *buf, int len)
{
	int max = 255, temp;

	len = 3*this->m_unit;
	if(this->m_unit == 2)
		max = 1023;
	
	//r ,output is bgr
	//temp = (this->m_colormatrix[0][0]&0xff)*this->m_r + (this->m_colormatrix[0][1]&0xff)*this->m_g + (this->m_colormatrix[0][2]&0xff)*this->m_b;
	temp = this->ColSum(this->ColMul(this->m_r, (unsigned char)(this->m_colormatrix[0][0]&0xff)),
						this->ColMul(this->m_g, (unsigned char)(this->m_colormatrix[0][1]&0xff)),
						this->ColMul(this->m_b, (unsigned char)(this->m_colormatrix[0][2]&0xff)),
						(this->m_offset[0]&0xff));	
	temp = (temp > max) ? max : (temp > 0 ? temp : 0);
	if(this->m_unit == 1)
		buf[0] = (char)temp;
	else
		*((short*)buf) = (short)temp;
	//g
	//temp = (this->m_colormatrix[1][0]&0xff)*this->m_r + (this->m_colormatrix[1][1]&0xff)*this->m_g + (this->m_colormatrix[1][2]&0xff)*this->m_b;
	temp = this->ColSum(this->ColMul(this->m_r, (unsigned char)(this->m_colormatrix[1][0]&0xff)),
						this->ColMul(this->m_g, (unsigned char)(this->m_colormatrix[1][1]&0xff)),
						this->ColMul(this->m_b, (unsigned char)(this->m_colormatrix[1][2]&0xff)),
						(this->m_offset[1]&0xff));
	temp = (temp > max) ? max : (temp > 0 ? temp : 0);
	if(this->m_unit == 1)
		buf[1] = (char)temp;
	else
		*((short*)buf+1) = (short)temp;
	//b
	//temp = (this->m_colormatrix[2][0]&0xff)*this->m_r + (this->m_colormatrix[2][1]&0xff)*this->m_g + (this->m_colormatrix[2][2]&0xff)*this->m_b;
	temp = this->ColSum(this->ColMul(this->m_r, (unsigned char)(this->m_colormatrix[2][0]&0xff)),
						this->ColMul(this->m_g, (unsigned char)(this->m_colormatrix[2][1]&0xff)),
						this->ColMul(this->m_b, (unsigned char)(this->m_colormatrix[2][2]&0xff)),
						(this->m_offset[2]&0xff));
	temp = (temp > max) ? max : (temp > 0 ? temp : 0);
	if(this->m_unit == 1)
		buf[2] = (char)temp;
	else
		*((short*)buf+2) = (short)temp;
	
	return len;
}

int CIspColorCorrect326::Process(char *src, char *dst)
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

//============== class CIspGammaCorrect326 ========================

CIspGammaCorrect326::CIspGammaCorrect326(int width, int height, int unit, int enable)
{
	int rgamma[17] = {0x03, 0x13, 0x2b, 0x48, 0x62, 0x7b, 0x93, 0xa8, 0xba, 0xca, 0xd7, 0xe3, 0xec, 0xf4, 0xfa, 0xff, 0xff};
	int ggamma[17] = {0x03, 0x13, 0x2b, 0x48, 0x62, 0x7b, 0x93, 0xa8, 0xba, 0xca, 0xd7, 0xe3, 0xec, 0xf4, 0xfa, 0xff, 0xff};
	int bgamma[17] = {0x03, 0x13, 0x2b, 0x48, 0x62, 0x7b, 0x93, 0xa8, 0xba, 0xca, 0xd7, 0xe3, 0xec, 0xf4, 0xfa, 0xff, 0xff};
	//int rgamma[17] = {0x0, 0x04, 0x16, 0x30, 0x4e, 0x68, 0x81, 0x98, 0xac, 0xbe, 0xcd, 0xda, 0xe4, 0xed, 0xf5, 0xfb, 0xff};
	//int ggamma[17] = {0x0, 0x04, 0x16, 0x30, 0x4e, 0x68, 0x81, 0x98, 0xac, 0xbe, 0xcd, 0xda, 0xe4, 0xed, 0xf5, 0xfb, 0xff};
	//int bgamma[17] = {0x0, 0x04, 0x16, 0x30, 0x4e, 0x68, 0x81, 0x98, 0xac, 0xbe, 0xcd, 0xda, 0xe4, 0xed, 0xf5, 0xfb, 0xff};

	this->SetSize(width, height, unit);
	this->SetEnable(enable);
	this->SetRGamma(rgamma);
	this->SetGGamma(ggamma);
	this->SetBGamma(bgamma);
	this->SetDither(0);
}

void CIspGammaCorrect326::SetSize(int width, int height, int unit)
{
	this->m_unit = unit;
	this->m_width = width;
	this->m_height = height;
}

void CIspGammaCorrect326::SetEnable(int enable)
{
	this->m_enable = enable;
}

void CIspGammaCorrect326::SetDither(int dither)
{
	this->m_dither = dither&0x1;
}

void CIspGammaCorrect326::SetDitherValue(int val)
{
	this->m_dither_val = val;
}

void CIspGammaCorrect326::SetRGamma(int *p)
{
	int i;

	for(i = 0; i < 17; i++)
		this->m_rgamma[i] = p[i]&0xff;
}

int* CIspGammaCorrect326::GetRGamma(void)
{
	return this->m_rgamma;
}

void CIspGammaCorrect326::SetBGamma(int *p)
{
	int i;

	for(i = 0; i < 17; i++)
		this->m_bgamma[i] = p[i]&0xff;
}

int* CIspGammaCorrect326::GetBGamma(void)
{
	return this->m_bgamma;
}

void CIspGammaCorrect326::SetGGamma(int *p)
{
	int i;

	for(i = 0; i < 17; i++)
		this->m_ggamma[i] = p[i]&0xff;
}

int* CIspGammaCorrect326::GetGGamma(void)
{
	return this->m_ggamma;
}

int CIspGammaCorrect326::Write(char *buf, int len)
{
	len = 3*this->m_unit;
	if(this->m_unit == 1)
	{
		this->m_r = (unsigned char)buf[0];
		this->m_g = (unsigned char)buf[1];
		this->m_b = (unsigned char)buf[2];
	}
	else
	{
		this->m_r = *((short*)buf)&0x3ff;
		this->m_g = *((short*)buf+1)&0x3ff;
		this->m_b = *((short*)buf+2)&0x3ff;
	}

	return len;
}

int CIspGammaCorrect326::Read(char *buf, int len)
{
	len = 3;

	if(!this->m_enable)		//gamma correction disable
	{
		if(this->m_unit == 2)
		{
			buf[0] = (char)((Dither(this->m_r)>>2)&0xff);
			buf[1] = (char)((Dither(this->m_g)>>2)&0xff);
			buf[2] = (char)((Dither(this->m_b)>>2)&0xff);
			return len;
		}
		else
		{
			buf[0] = (char)(this->m_r&0xff);
			buf[1] = (char)(this->m_g&0xff);
			buf[2] = (char)(this->m_b&0xff);
			return len;
		}
	}
	this->GammaCorrect(buf);

	return len;
}

//before 10bits->8bits, add dither to 10bits data
int CIspGammaCorrect326::Dither(int val)
{
	int x;

	x = (val&0x3ff) + (this->m_dither ? this->m_dither_val : 0);

	return (x > 1023) ? 1023 : ((x < 0) ? 0 : x);
}

/* 10 bit data input, if data is 8bit, left move 2bit */
int CIspGammaCorrect326::GetGammaValue(int val, int rgb)
{
	int	x1;
	int	y1, y2;
	int	flag;
	int	result;

	//val = val&0xff;
	flag = val >> 6;
	//x1 = flag << 4;
	x1 = val&0x3f;

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
	result *= x1;
	result >>= 6;
	result += y1;

	return result > 255 ? 255 : result;
}

void CIspGammaCorrect326::GammaCorrect(char *buf)
{
	if(this->m_unit == 2)		//10 bit data
	{
		buf[0] = (char)this->GetGammaValue(Dither(this->m_r), 0);
		buf[1] = (char)this->GetGammaValue(Dither(this->m_g), 1);
		buf[2] = (char)this->GetGammaValue(Dither(this->m_b), 2);
	}
	else
	{
		buf[0] = (char)this->GetGammaValue((this->m_r<<2), 0);
		buf[1] = (char)this->GetGammaValue((this->m_g<<2), 1);
		buf[2] = (char)this->GetGammaValue((this->m_b<<2), 2);
	}
}

int CIspGammaCorrect326::Process(char *src, char *dst)
{
#if 0
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
#else
	int i, j;

	for(i = 0; i < this->m_height; i++)
	{
		for(j = 0; j < this->m_width; j++)
		{
			this->Write(src+i*this->m_width*this->m_unit*3+j*this->m_unit*3, 1);
			if(this->m_dither)
				this->SetDitherValue(gDither10_8[i % 4][j % 4] - 2);
			this->Read(dst+i*this->m_width*3+j*3, 1);
		}
	}
	
	return this->m_width*this->m_height*3;
#endif


}

//============== class CIspColorConvert326 ========================

CIspColorConvert326::CIspColorConvert326(int width, int height)
{
	this->SetSize(width, height);
}

void CIspColorConvert326::SetSize(int width, int height)
{
	this->m_width = width;
	this->m_height = height;
}

CIspColorConvert326::Write(char *buf, int len)
{
	len = 6;

	this->m_r[0] = buf[0];
	this->m_g[0] = buf[1];
	this->m_b[0] = buf[2];
	this->m_r[1] = buf[3];
	this->m_g[1] = buf[4];
	this->m_b[1] = buf[5];
	
	return len;
}

int CIspColorConvert326::Read(char *buf, int len)
{
	int r, g, b, y1, u1, v1, y2, u2, v2, tmp;

	len = 4;
	r = (int)(this->m_r[0]&0xff);
	g = (int)(this->m_g[0]&0xff);
	b = (int)(this->m_b[0]&0xff);
//	y = (77 * r + 150 * g + 29 * b) >> 8;
//	u = ((128 * b - 43 * r - 85 * g) >> 8) + 128;
	y1 = ((77 * r)>>8) + ((150 * g)>>8) + ((29 * b)>>8);
	u1 = ((((128 * b)>>7) - ((43 * r)>>7) - ((85 * g)>>7))>>1) + 128;
	v1 = ((((128 * r)>>7) - ((107 * g)>>7) - ((21 * b)>>7))>>1) + 128;
	
	r = (int)(this->m_r[1]&0xff);
	g = (int)(this->m_g[1]&0xff);
	b = (int)(this->m_b[1]&0xff);
//	y = (77 * r + 150 * g + 29 * b) >> 8;
//	v = ((128 * r - 107 * g - 21 * b) >> 8) + 128;
	y2 = ((77 * r)>>8) + ((150 * g)>>8) + ((29 * b)>>8);
	u2 = ((((128 * b)>>7) - ((43 * r)>>7) - ((85 * g)>>7))>>1) + 128;
	v2 = ((((128 * r)>>7) - ((107 * g)>>7) - ((21 * b)>>7))>>1) + 128;

	tmp = (u1+u2)>>1;
	buf[0] = (char)(y1 < 0 ? 0 : ((y1 > 255) ? 255 : y1));
	buf[1] = (char)(tmp < 0 ? 0 : ((tmp > 255) ? 255 : tmp));

	tmp = (v1+v2)>>1;
	buf[2] = (char)(y2 < 0 ? 0 : ((y2 > 255) ? 255 : y2));
	buf[3] = (char)(tmp < 0 ? 0 : ((tmp > 255) ? 255 : tmp));
	
	return len;
}

int CIspColorConvert326::Process(char *src, char *dst)
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

//============== class CIspEdgeEnhance326 ========================

CIspEdgeEnhance326::CIspEdgeEnhance326(int width, int height)
{
	this->SetSize(width, height);
}

void CIspEdgeEnhance326::SetSize(int width, int height)
{
	this->m_width = width;
	this->m_height = height;
}

int CIspEdgeEnhance326::Write(char *buf, char *edge, int len)
{
	len = 2;
	this->m_y = buf[0];
	this->m_uv = buf[1];
	this->m_edge = *((short*)edge)&0x1ff;

	return len;
}

int CIspEdgeEnhance326::Read(char *buf, int len)
{
	int val1, val2, flag = 0;

	len = 2;
	val1 = this->m_y&0xff;
	val2 = this->m_edge;
	flag = this->m_edge&0x100;
	if(flag)
		val1 -= ((~val2+1)&0xff);
	else
		val1 += (val2&0xff);
	buf[0] = (char)(val1 > 255 ? 255 : (val1 < 0 ? 0 : (char)val1));
	buf[1] = (char)(this->m_uv);

	return len;
}

int CIspEdgeEnhance326::Process(char *src, char *edge, char *dst)
{
	int pos, size, pos1 = 0, pos2 = 0, len;

	size = this->m_width * this->m_height * 2;

	for(pos = 0; pos < size; )
	{
		len = this->Write(src+pos, edge+pos2, 1);
		pos += len;
		pos2 += 2;
		len = this->Read(dst+pos1, 1);
		pos1 += len;
	}

	return pos1;
}


//============= class CIspAutoFocus =================================


CIspAutoFocus::CIspAutoFocus(int width, int height)
{
	this->SetSize(width, height);
	this->SetInitVal();
}

CIspAutoFocus::~CIspAutoFocus()
{
}

void CIspAutoFocus::SetInitVal(void)
{
	int afh[] = {0, 160, 160, 160, 160};
	int afv[] = {0, 120, 120, 120, 120};
	int afw[] = {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1};
	int i, j;

	this->SetAFH(afh);
	this->SetAFV(afv);
	this->SetAFWeight(afw);
	this->m_line = 0;
	this->m_sum = 0;
	for(i = 0; i < AF_H_AND_V_NUM-1; i++)
		for(j = 0; j < AF_H_AND_V_NUM-1; j++)
			this->m_win_sum[i][j] = 0;
}

void CIspAutoFocus::SetSize(int width, int height)
{
	this->m_width = width;
	this->m_height = height;
	this->m_linebuf.SetSize(this->m_width, 0, 2);
}

void CIspAutoFocus::SetAFH(int *afh)
{
	int i;

	if(afh == NULL)
		return;
	for(i = 0; i < AF_H_AND_V_NUM; i++)
		this->m_afh[i] = afh[i];
}

void CIspAutoFocus::SetAFV(int *afv)
{
	int i;

	if(afv == NULL)
		return;
	for(i = 0; i < AF_H_AND_V_NUM; i++)
		this->m_afv[i] = afv[i];
}

void CIspAutoFocus::SetAFWeight(int *weight)
{
	int i;

	if(weight == NULL)
		return;
	for(i = 0; i < AF_WINDOW_NUM; i++)
		this->m_afweight[i] = weight[i];
}

int CIspAutoFocus::Write(char *buf, int len)
{
	this->m_linebuf.ClearPoint();
	len = this->m_linebuf.Write(buf, len*2);
	
	return len;
}

int CIspAutoFocus::Read(int len)
{
	int j;
	int val;
	int winRow, winCol;
	int flag;

	for(j = 0; j < len; j++)
	{
		winRow = this->GetWinRow();
		winCol = this->GetWinCol(j);
		if((winRow == 0xff) || (winCol == 0xff))
			continue;
		val = this->m_linebuf.Read(j);
		val &= 0x1ff;
		flag = val&0x100;
		if(flag)
			val = (~val+1)&0xff;
		else
			val = val&0xff;
		this->m_win_sum[winRow][winCol] += val;
	}
	this->m_line ++;

	return j*2;
}

int CIspAutoFocus::GetWinRow(void)
{
	int row = 0;
	int i;

	for(i = 0; i < AF_H_AND_V_NUM; i++)
	{
		row += this->m_afv[i];
		if(this->m_line < row)
			break;
	}
	if((i == 0) || (i == AF_H_AND_V_NUM))
		return 0xff;
	else
		return i-1;
}

int CIspAutoFocus::GetWinCol(int col)
{
	int count = 0;
	int i;

	for(i = 0; i < AF_H_AND_V_NUM; i++)
	{
		count += this->m_afh[i];
		if(col < count)
			break;
	}
	if((i == 0) || (i == AF_H_AND_V_NUM))
		return 0xff;
	else
		return i-1;	
}

unsigned int CIspAutoFocus::GetWinSum(void)
{
	int i, j;
	unsigned long edgeSum = 0;
	unsigned int weightSum = 0;
	
	for(i = 0; i < AF_H_AND_V_NUM-1; i++)
	{
		for(j = 0; j < AF_H_AND_V_NUM-1; j++)
		{
			weightSum += this->m_afweight[i*(AF_H_AND_V_NUM-1)+j];
			edgeSum += (this->m_afweight[i*(AF_H_AND_V_NUM-1)+j]*this->m_win_sum[i][j]);
		}
	}
	edgeSum /= weightSum;
	return (unsigned int)edgeSum;
}

int CIspAutoFocus::Process(char *edge, unsigned int *afsum)
{
	int pos, size, pos1 = 0, len;

	size = this->m_width * this->m_height * 2;

	for(pos = 0; pos < size; )
	{
		len = this->Write(edge+pos, this->m_width);
		pos += len;
		len = this->Read(this->m_width);
		pos1 += len;
	}

	*afsum = this->GetWinSum();

	return pos1;
}

