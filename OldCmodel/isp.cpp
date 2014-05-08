

#include "dbugPrint.h"

#include "isp.h"


//===================== class CIspRawBase ======================

int	CIspRawBase::FillImageInfo(ImageStream *pIn, ImageStream *pOut)
{
	int	width, height;
	int	flag;
	
	width = this->GetInterReg(REG_ISP_IMGWD+1);
	width <<= 8;
	width += this->GetInterReg(REG_ISP_IMGWD);
	height = this->GetInterReg(REG_ISP_IMGHT+1);
	height <<= 8;
	height += this->GetInterReg(REG_ISP_IMGHT);
	flag = CChipBase1::FillImageInfo(pIn, pOut);
	pOut->height = height;
	if( flag != VSYNC_PERIOD )
		return flag;

	if(width > pIn->width)
	{
		Gdbug.DbugMsg("ISP: ImageWidth Setting > InputImageWidth\n");
		return INVAILD_PERIOD;
	}
	if(height > pIn->height)
	{
		Gdbug.DbugMsg("ISP: ImageHeight Setting > InputImageHeight\n");
		return INVAILD_PERIOD;
	}
	pOut->width = width;

	return VSYNC_PERIOD;
}

int CIspRawBase::varify(ImageStream *pIn)
{
	UINT8 format;

	format = RAW_RG;
	if(this->GetInterReg(REG_ISP_BASCTL) & 0x20)		//BG line first
		format = RAW_BG;
	if(this->GetInterReg(REG_ISP_BASCTL) & 0x10)		//G pixel first
		format++;

#if 1
	if(pIn->format != format)
	{
		Gdbug.DbugMsg("ISP: Input image format not fit Isp setting!\n");
		return FAILED;
	}
#endif

	return CChipBase1::varify(pIn);
}

int CIspRawBase::VsyncInit(ImageStream *pIn)
{
	int	val;

	if(CChipBase1::VsyncInit(pIn) == FALSE)
		return FALSE;
	this->gBgline = 0;
	this->gGpixel = 0;
	val = this->GetInterReg(REG_ISP_BASCTL);
	if(val & 0x20)		//BG line first
		this->gBgline = 1;
	if(val & 0x10)		//G pixel first
		this->gGpixel = 1;
	return TRUE;
}

int CIspRawBase::Process(ImageStream *pIn, ImageStream *pOut)
{
	int flag;

	this->VsyncInit(pIn);
	if(this->varify(pIn) != SUCCEED)
		return FLOW_ERROR;

	flag = this->FillImageInfo(pIn, pOut);
	if(flag == VSTART_PERIOD)	return FLOW_NORMAL;
	if(flag == VEND_PERIOD)		return FLOW_FRAME;
	if(flag == INVAILD_PERIOD)	return FLOW_ERROR;
	if(flag == INSERT_BLANK)	return FLOW_ERROR;

	if(this->IsFrameOver(pOut))	return FLOW_FRAME;

	return CChipBase1::Process(pIn, pOut);
}


//===================== class CIspBlackLevel ======================

void CIspBlackLevel::BlackLevel(ImageStream *pIn, ImageStream *pOut)
{
	int	minVal = 0;
	int		k;
	int	temp;

	temp = this->gGpixel;
	for(k = 0; k < pOut->width; k++)
	{
		if(this->gGpixel)
			minVal = this->gBgline ? this->gReg[REG_ISP_GLEVEL1] : this->gReg[REG_ISP_GLEVEL2];
		else if(this->gBgline)
			minVal = this->gReg[REG_ISP_BLEVEL];
		else
			minVal = this->gReg[REG_ISP_RLEVEL];
		pOut->buf16[k] = (SINT16)((pIn->buf16[k] > minVal) ? pIn->buf16[k]-minVal : 0);
		this->gGpixel = InverseBit(this->gGpixel);
	}
	this->gBgline = InverseBit(this->gBgline);
	this->gGpixel = InverseBit(temp);
}

int CIspBlackLevel::Process(ImageStream *pIn, ImageStream *pOut)
{
	int flag;

	Gdbug.DbugPrintData(ISP_BLACKLEVEL_PRINT_INPUT, pIn);
	this->VsyncInit(pIn);
	if(this->varify(pIn) != SUCCEED)
		return FLOW_ERROR;

//	flag = this->FillImageInfo(pIn, pOut);
	flag = CChipBase1::FillImageInfo(pIn, pOut);
	if(flag == VSTART_PERIOD)	return FLOW_NORMAL;
//	if(flag == VEND_PERIOD)		return FLOW_NORMAL;
	if(flag == INVAILD_PERIOD)	return FLOW_ERROR;
	if(flag == INSERT_BLANK)	return FLOW_ERROR;

	if(this->IsFrameOver(pOut))	return FLOW_FRAME;

	if( (this->GetInterReg(REG_ISP_BLACKNCAP) & 0x80) == 0 )		//black level disable
		return CChipBase1::Process(pIn, pOut);

	this->BlackLevel(pIn, pOut);
	this->AddInline(pIn);
	return this->AddOutLine(pOut);
}


//===================== class CIspTestBar ======================

int CIspTestBar::Process(ImageStream *pIn, ImageStream *pOut)
{
	int flag;

	Gdbug.DbugPrintData(ISP_TESTBAR_PRINT_INPUT, pIn);
	this->VsyncInit(pIn);
	if(this->varify(pIn) != SUCCEED)
		return FLOW_ERROR;

//	flag = this->FillImageInfo(pIn, pOut);
	flag = CChipBase1::FillImageInfo(pIn, pOut);
	if(flag == VSTART_PERIOD)	return FLOW_NORMAL;
//	if(flag == VEND_PERIOD)		return FLOW_NORMAL;
	if(flag == INVAILD_PERIOD)	return FLOW_ERROR;
	if(flag == INSERT_BLANK)	return FLOW_ERROR;
	if(this->IsFrameOver(pOut))	return FLOW_FRAME;

//	if(this->GetInterReg(REG_ISP_BASCTL) & 0x80)			//testbar enable
//		return CChipBase1::Process(pIn, pOut);
	return CChipBase1::Process(pIn, pOut);
}


//===================== class CIspLensFallOff ======================

void CIspLensFallOff::LensFallOff(ImageStream *pIn, ImageStream *pOut)
{
	SINT32	x, y, focus, val1, val2;
	int	k;

	x = this->GetInterReg(REG_ISP_LFCENX+1);
	x <<= 8;
	x += this->GetInterReg(REG_ISP_LFCENX);

	y = this->GetInterReg(REG_ISP_LFCENY+1);
	y <<= 8;
	y += this->GetInterReg(REG_ISP_LFCENY);

	focus = this->GetInterReg(REG_ISP_LFFREC+1);
	focus <<= 8;
	focus += this->GetInterReg(REG_ISP_LFFREC);

	val2 = this->gInLine;
	val2 -= y;
	val2 *= val2;

	for(k = 0; k < pOut->width; k++)
	{
		//fomula:
		//	(1+(x^2+y^2)/f^2) ^ 2
		//
		val1 = k;
		val1 -= x;
		val1 *= val1;

		val1 += val2;
		val1 /= focus;
		val1 >>= 16;
		val1 += 256;
		val1 *= val1;
		val1 *= pIn->buf16[k];
		val1 >>= 16;

		pOut->buf16[k] = (SINT16)(val1 > 255 ? 255 : val1 > 0 ? val1 : 0);
		if(this->GetInterReg(REG_ISP_BLACKNCAP) & 1)		//10bit data
			pOut->buf16[k] = (SINT16)(val1 > 1023 ? 1023 : val1 > 0 ? (SINT16)val1 : 0);
	}
}

int CIspLensFallOff::Process(ImageStream *pIn, ImageStream *pOut)
{
	int flag;

	Gdbug.DbugPrintData(ISP_LENS_PRINT_INPUT, pIn);
	this->VsyncInit(pIn);
	if(this->varify(pIn) != SUCCEED)
		return FLOW_ERROR;

//	flag = this->FillImageInfo(pIn, pOut);
	flag = CChipBase1::FillImageInfo(pIn, pOut);
	if(flag == VSTART_PERIOD)	return FLOW_NORMAL;
//	if(flag == VEND_PERIOD)		return FLOW_NORMAL;
	if(flag == INVAILD_PERIOD)	return FLOW_ERROR;
	if(flag == INSERT_BLANK)	return FLOW_ERROR;
	if(this->IsFrameOver(pOut))	return FLOW_FRAME;

	if((this->GetInterReg(REG_ISP_BASCTL+1) & 2) == 0)		//lens falloff disable
		return CChipBase1::Process(pIn, pOut);

	this->LensFallOff(pIn, pOut);
	this->AddInline(pIn);
	return this->AddOutLine(pOut);
}


//===================== class CIspDpdNoise ======================

CIspDpdNoise::CIspDpdNoise(void)
{
	int i;

	for(i = 0; i < 4; i++)
		this->gpISbuf[i] = &(this->gISbuf[i]);
}

int CIspDpdNoise::GetNoiseValue(int p, int g_pixel)
{
	int	x1;
	int	y1, y2;
	int	flag;
	int		result;

	flag = p >> 4;
	x1 = flag << 4;

	y1 = this->GetInterReg(REG_ISP_NT0 + flag);
	y2 = this->GetInterReg(REG_ISP_NT0 + flag + 1);

	result = y2 - y1;
	result *= p - x1;
	result >>= 4;
	result += y1;
//	result = (g_pixel) ? result : result >> 1;
	return g_pixel ? result : result >> 1;
}

int CIspDpdNoise::GetDpdNoise(int *p, int g_pixel)
{
	int DV, DH, thld, ave, var;
	int pixel;

	pixel = p[2];
	DV = abs((p[2] << 1) - p[0] - p[4]);
	DH = abs((p[2] << 1) - p[1] - p[3]);
	thld = this->GetInterReg(REG_ISP_DPCTHD);

	if( (DV > thld) && (DH > thld) )
	{
		var = (p[1] + p[3]) >> 1;
		if(DV < DH)
			var = (p[0] + p[4]) >> 1;
		return (SINT16)var;
	}
	if (DH > DV)
	{
		ave = p[0] + p[2] + p[4];
		ave *= 21;		//I think it can be changed to: ave += 1;	ave /= 3;
		ave >>= 6;
		//ave /= 64;

		var = abs(p[0] - ave) + abs(p[2] - ave) + abs(p[4] - ave);
		var *= 21;
		var >>= 6;
		//var /= 64;
		//ave = (pix0 + pix2 + pix4) / 3;
		//var = (abs(pix0 - ave) + abs(pix2 - ave) + abs(pix4 - ave)) / 3;
	}
	else
	{
		ave = p[1] + p[2] + p[3];
		ave *= 21;		//I think it can be changed to: ave += 1;	ave /= 3;
		ave >>= 6;
		//ave /= 64;

		var = abs(p[1] - ave) + abs(p[2] - ave) + abs(p[3] - ave);
		var *= 21;
		var >>= 6;
		//var /= 64;
		//ave = (pix0 + pix2 + pix4) / 3;
		//var = (abs(pix0 - ave) + abs(pix2 - ave) + abs(pix4 - ave)) / 3;
	}

	if(this->GetInterReg(REG_ISP_BLACKNCAP) & 0x1)
	{
		var >>= 2;
		pixel >>= 2;
	}

	if( var > GetNoiseValue((UINT8)pixel, g_pixel) )
		return (SINT16)ave;

	return p[2];
}

void CIspDpdNoise::DpdNoiseInside(ImageStream *pIn, ImageStream *pOut)
{
	int k;
	int p[5];
	int Gpixel = this->gGpixel;

//	this->FillImageInfo(this->gpISbuf[2], pOut);
	pOut->width = this->gpISbuf[2]->width;
	for(k = 0; k < pOut->width; k++)
	{
		if( (k < 2) || (k > pOut->width - 3) )
			pOut->buf16[k] = pIn->buf16[k];
		else
		{
			p[0] = this->gpISbuf[0]->buf16[k];
			p[1] = this->gpISbuf[2]->buf16[k-2];
			p[2] = this->gpISbuf[2]->buf16[k];
			p[3] = this->gpISbuf[2]->buf16[k+2];
			p[4] = pIn->buf16[k];
			pOut->buf16[k] = (SINT16)this->GetDpdNoise(p, Gpixel);
		}
		Gpixel = InverseBit(Gpixel);
	}
	this->gGpixel = InverseBit(this->gGpixel);
	this->gBgline = InverseBit(this->gBgline);
}

void CIspDpdNoise::DpdNoise(ImageStream *pIn, ImageStream *pOut)
{
	int height;
	int i;

	height = this->GetInterReg(REG_ISP_IMGHT+1);
	height <<= 8;
	height += this->GetInterReg(REG_ISP_IMGHT);
	if(this->gInLine < 2)
	{
		i = this->gInLine;
		this->gpISbuf[i]->format = pIn->format;
		this->gpISbuf[i]->width = pIn->width;
//		CIspRawBase::FillImageInfo(pIn, this->gpISbuf[i]);
		CChipBase1::CopyImageStream(pIn, this->gpISbuf[i]);
		this->gpISbuf[i]->vs = 0;
		pOut->width = 0;
	}
	else if(this->gInLine > height+1)
	{
		pOut->width = 0;
	}

	else if( (this->gInLine == 2) || (this->gInLine == 3) )
	{
		i = this->gInLine;
		this->gpISbuf[i]->format = pIn->format;
		this->gpISbuf[i]->width = pIn->width;
//		CIspRawBase::FillImageInfo(pIn, this->gpISbuf[i]);
		CChipBase1::CopyImageStream(pIn, this->gpISbuf[i]);
//		pOut->format = this->gpISbuf[i-2]->format;
//		pOut->width = this->gpISbuf[i-2]->width;
//		CIspRawBase::FillImageInfo(this->gpISbuf[i-2], pOut);
		CChipBase1::CopyImageStream(this->gpISbuf[i-2], pOut);
	}
	else if( (this->gInLine == height) || (this->gInLine == height + 1) )
	{
		i = this->gInLine - height + 2;
		pOut->format = this->gpISbuf[i]->format;
		pOut->width = this->gpISbuf[i]->width;
//		CIspRawBase::FillImageInfo(this->gpISbuf[i], pOut);
		CChipBase1::CopyImageStream(this->gpISbuf[i], pOut);
	}
	else
	{
		this->DpdNoiseInside(pIn, pOut);
		this->gpISbuf[0]->format = pIn->format;
		this->gpISbuf[0]->width = pIn->width;
//		CIspRawBase::FillImageInfo(pIn, this->gpISbuf[0]);
		CChipBase1::CopyImageStream(pIn, this->gpISbuf[0]);
		this->AdjustISPoint(this->gpISbuf, 4);
	}
}

int CIspDpdNoise::Process(ImageStream *pIn, ImageStream *pOut)
{
	int flag;

	Gdbug.DbugPrintData(ISP_DPD_PRINT_INPUT, pIn);
	this->VsyncInit(pIn);
	if(this->varify(pIn) != SUCCEED)
		return FLOW_ERROR;

//	flag = this->FillImageInfo(pIn, pOut);
	flag = CChipBase1::FillImageInfo(pIn, pOut);
	if(flag == VSTART_PERIOD)	return FLOW_NORMAL;
	if(flag == INVAILD_PERIOD)	return FLOW_ERROR;
	if(flag == INSERT_BLANK)	return FLOW_ERROR;
	if(this->IsFrameOver(pOut))	return FLOW_FRAME;

	if((this->GetInterReg(REG_ISP_BASCTL+1) & 4) == 0)		//dpd/dpc disable
		return CChipBase1::Process(pIn, pOut);

	this->DpdNoise(pIn, pOut);
	this->AddInline(pIn);
	return this->AddOutLine(pOut);
}


//===================== class CIspDigitalGain ======================

void CIspDigitalGain::GainProcess(ImageStream *pIn, ImageStream *pOut)
{
	int k;
	UINT32	temp, max = 255;
	int GlobalEn = 0, RgbEn = 0;
	int Gpixel = this->gGpixel;
	int GlobalGain, rGain, gGain1, gGain2, bGain, rgbGain;

	if(this->GetInterReg(REG_ISP_GLBGAIN+1) & 0x20)		//global gain enable
		GlobalEn = 1;
	if(this->GetInterReg(REG_ISP_BASCTL) & 0x40)			//RGB gain enable
		RgbEn = 1;
	if(this->GetInterReg(REG_ISP_BLACKNCAP) & 1)			//10bit raw data
		max = 1023;
	GlobalGain = this->GetInterReg(REG_ISP_GLBGAIN);
	temp = this->GetInterReg(REG_ISP_GLBGAIN+1);
	switch(temp & 3)
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
	rGain = this->GetInterReg(REG_ISP_RGAIN);
	gGain1 = this->GetInterReg(REG_ISP_GGAIN);
	gGain2 = this->GetInterReg(REG_ISP_GGAIN);
	bGain = this->GetInterReg(REG_ISP_BGAIN);
	
	for(k = 0; k < pOut->width; k++)
	{
		temp = pIn->buf16[k];
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
				rgbGain = this->gBgline ? gGain1 : gGain2;
			else if(this->gBgline)
				rgbGain = bGain;
			else
				rgbGain = rGain;
			temp *= rgbGain;
			temp >>= 6;
			if(temp > max)
				temp = max;
		}
		pOut->buf16[k] = (SINT16)temp;
		Gpixel = InverseBit(Gpixel);
	}
	this->gBgline = InverseBit(this->gBgline);
	this->gGpixel = InverseBit(this->gGpixel);
}

int CIspDigitalGain::Process(ImageStream *pIn, ImageStream *pOut)
{
	int flag;

	Gdbug.DbugPrintData(ISP_GAIN_PRINT_INPUT, pIn);
	this->VsyncInit(pIn);
	if(this->varify(pIn) != SUCCEED)
		return FLOW_ERROR;

//	flag = this->FillImageInfo(pIn, pOut);
	flag = CChipBase1::FillImageInfo(pIn, pOut);
	if(flag == VSTART_PERIOD)	return FLOW_NORMAL;
	if(flag == VEND_PERIOD)		return FLOW_FRAME;
	if(flag == INVAILD_PERIOD)	return FLOW_ERROR;
	if(flag == INSERT_BLANK)	return FLOW_ERROR;
	if(this->IsFrameOver(pOut))	return FLOW_FRAME;

	if( ((this->GetInterReg(REG_ISP_GLBGAIN+1) & 0x20) == 0) &&		//global gain disable
		((this->GetInterReg(REG_ISP_BASCTL+1) & 0x40) == 0) )		//digital gain disable
		return CChipBase1::Process(pIn, pOut);

	this->GainProcess(pIn, pOut);
	this->AddInline(pIn);
	return this->AddOutLine(pOut);
}


//===================== class CIspInterpolate ======================

CIspInterpolate::CIspInterpolate(void)
{
	int i;

	for(i = 0; i < 9; i++)
		this->gpISbuf[i] = &(this->gISbuf[i]);
	for(i = 0; i < 5; i++)
		this->gpBlock5X5[i] = this->gGBlock5X5[i];
}

int	CIspInterpolate::FillImageInfo(ImageStream *pIn, ImageStream *pOut, ImageStream *pEdge)
{
	int flag;

	pEdge->Clear();
	CChipBase1::FillImageInfo(pIn, pOut);
	pEdge->format = YUV422;
	flag = CChipBase1::FillImageInfo(pIn, pOut);
	pOut->format = RAWRGB;
	return flag;
}

int CIspInterpolate::GetGPixel(int col, int row)
{
	int dh, dv, x1, x2;

	if( ((this->gGpixel + col + row) % 2) != 0)	//G pixel
		return this->gpISbuf[col]->buf16[row];

	x1 = this->gpISbuf[col]->buf16[row-1] - this->gpISbuf[col]->buf16[row+1];
	x1 = (x1 > 0) ? x1 : -x1;
	x2 = this->gpISbuf[col]->buf16[row-2] + this->gpISbuf[col]->buf16[row+2] - 
		(this->gpISbuf[col]->buf16[row] << 1);
	x2 = (x2 > 0) ? x2 : -x2;
	dh = (x1 << 1) + x2;

	x1 = this->gpISbuf[col-1]->buf16[row] - this->gpISbuf[col+1]->buf16[row];
	x1 = (x1 > 0) ? x1 : -x1;
	x2 = this->gpISbuf[col-2]->buf16[row] + this->gpISbuf[col+2]->buf16[row] - 
		(this->gpISbuf[col]->buf16[row] << 1);
	x2 = (x2 > 0) ? x2 : -x2;
	dv = (x1 << 1) + x2;

	if(dh < dv)
	{
		x1 = (this->gpISbuf[col]->buf16[row-1] + this->gpISbuf[col]->buf16[row+1]) << 2;
		x2 = (this->gpISbuf[col]->buf16[row] << 1) - this->gpISbuf[col]->buf16[row-2] - 
			this->gpISbuf[col]->buf16[row+2];
	}
	else
	{
		x1 = (this->gpISbuf[col-1]->buf16[row] + this->gpISbuf[col+1]->buf16[row]) << 2;
		x2 = (this->gpISbuf[col]->buf16[row] << 1) - this->gpISbuf[col-2]->buf16[row] - 
			this->gpISbuf[col+2]->buf16[row];
	}
	x1 += x2;
	x1 /= 8;
	return x1;
}

void CIspInterpolate::printGblock(int pos)
{
	int i, j;
	FILE *fp;

	fp = fopen("aa.txt", "wt");
	fprintf(fp, "gInLine = 0x%x, pos = 0x%x\n", (int)this->gInLine, pos);
	fprintf(fp, "Gpixel = 0x%x\n", (int)this->gGpixel);
	for(i = 0; i < 9; i++)
	{
		for(j = 0; j < 9; j++)
		{
			fprintf(fp, "0x%x", this->gpISbuf[i]->buf16[j+pos]);
			if( ((this->gGpixel + i + j + pos) % 2) != 0)	//G pixel
				fprintf(fp, " (G), ");
			else
				fprintf(fp, " (B), ");

		}
		fprintf(fp, "\n");
	}
	fprintf(fp, "\n");

	for(i = 0; i < 5; i++)
	{
		for(j = 0; j < 5; j++)
			fprintf(fp, "0x%x, ", this->gpBlock5X5[j][i]);
		fprintf(fp, "\n");
	}
	fclose(fp);
}

void CIspInterpolate::GetFirstGBlock5X5(void)
{
	int i, j;

	for(i = 0; i < 5; i++)
		for(j = 0; j < 5; j++)
			this->gpBlock5X5[j][i] = this->GetGPixel(i+2, j+2);

//	this->printGblock(0);
}

void CIspInterpolate::GetNextGBlock5X5(int pos)
{
	int i;
	int *p = this->gpBlock5X5[0];

	for(i = 0; i < 5; i++)
		this->gpBlock5X5[0][i] = this->GetGPixel(i+2, pos+6);

	for(i = 0; i < 4; i++)
		this->gpBlock5X5[i] = this->gpBlock5X5[i+1];
	this->gpBlock5X5[4] = p;

//	this->printGblock(pos);
}

int CIspInterpolate::GetRfromBG(int pos)		//GB line, get R at B pixel
{
	int x1, x2;

	x1 = this->gpISbuf[3]->buf16[3+pos] + this->gpISbuf[5]->buf16[3+pos] + 
		this->gpISbuf[3]->buf16[5+pos] + this->gpISbuf[5]->buf16[5+pos];
	x2 = this->gpBlock5X5[1][1] + this->gpBlock5X5[3][3] + 
		this->gpBlock5X5[1][3] + this->gpBlock5X5[3][1];

	x1 -= x2;
	x1 /= 4;
	x1 += this->gpBlock5X5[2][2];
	return x1;
}

int CIspInterpolate::GetRfromGR(int pos)		//GR line, get R at G pixel
{
	int x1, x2;

	x1 = this->gpISbuf[4]->buf16[3+pos] + this->gpISbuf[4]->buf16[5+pos];
	x2 = this->gpBlock5X5[1][2] + this->gpBlock5X5[3][2];

	x1 -= x2;
	x1 /= 2;
	x1 += this->gpBlock5X5[2][2];
	return x1;
}

int CIspInterpolate::GetRfromGB(int pos)		//GB line, get R at G pixel
{
	int x1, x2;

	x1 = this->gpISbuf[3]->buf16[4+pos] + this->gpISbuf[5]->buf16[4+pos];
	x2 = this->gpBlock5X5[2][1] + this->gpBlock5X5[2][3];

	x1 -= x2;
	x1 /= 2;
	x1 += this->gpBlock5X5[2][2];
	return x1;
}

int CIspInterpolate::GetEdge(void)
{
	int x, temp;
	int femm, femp, femmin, femmax, femx1, femx2;

	femm = this->GetInterReg(REG_ISP_FEM_M);
	femp = this->GetInterReg(REG_ISP_FEM_P);
	femx1 = this->GetInterReg(REG_ISP_FEM_X1);
	femx2 = this->GetInterReg(REG_ISP_FEM_X2);
	femmin = this->GetInterReg(REG_ISP_FEM_MIN);
	femmax = this->GetInterReg(REG_ISP_FEM_MAX);

	x = this->gpBlock5X5[2][2] << 3;
	temp = this->gpBlock5X5[0][0] + this->gpBlock5X5[0][2] + this->gpBlock5X5[0][4] +
		this->gpBlock5X5[2][0] + this->gpBlock5X5[2][4] + this->gpBlock5X5[4][0] +
		this->gpBlock5X5[4][2] + this->gpBlock5X5[4][4];
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

	return (SINT8) ( (x > femmax) ? femmax : ((x < -femmin) ? -femmin : x) );
}

void CIspInterpolate::OutputStream(ImageStream *pOut, ImageStream *pEdge)
{
	int		i;
	int		width = this->gpISbuf[4]->width - 8;
	int	maxval = 255;
	int	gpixel = this->gGpixel;

	pOut->width = width;
	pEdge->width = width;
	if(this->GetInterReg(REG_ISP_BLACKNCAP) & 1)		//10 bit
		maxval = 1023;
	for(i = 0; i < width; i++)
	{
		if(i == 0 )
			this->GetFirstGBlock5X5();
		else
			this->GetNextGBlock5X5(i);
		if(this->gBgline)	//B G line
		{
			if(gpixel)
			{
				pOut->rgbBuf[i].r = (SINT16)this->GetRfromGB(i); 
				pOut->rgbBuf[i].g = (SINT16)this->gpBlock5X5[2][2];
				pOut->rgbBuf[i].b = (SINT16)this->GetRfromGR(i);
			}
			else
			{
				pOut->rgbBuf[i].r = (SINT16)this->GetRfromBG(i); 
				pOut->rgbBuf[i].g = (SINT16)this->gpBlock5X5[2][2];
				pOut->rgbBuf[i].b = (SINT16)this->gpISbuf[4]->buf16[i+4];
			}
		}
		else			//R G line
		{
			if(gpixel)
			{
				pOut->rgbBuf[i].r = (SINT16)this->GetRfromGR(i); 
				pOut->rgbBuf[i].g = (SINT16)this->gpBlock5X5[2][2];
				pOut->rgbBuf[i].b = (SINT16)this->GetRfromGB(i);
			}
			else
			{
				pOut->rgbBuf[i].r = (SINT16)this->gpISbuf[4]->buf16[i+4];
				pOut->rgbBuf[i].g = (SINT16)this->gpBlock5X5[2][2];
				pOut->rgbBuf[i].b = (SINT16)this->GetRfromBG(i); 
			}
		}
		pOut->rgbBuf[i].r = (SINT16)(pOut->rgbBuf[i].r > maxval ? maxval : 
							(pOut->rgbBuf[i].r > 0 ? pOut->rgbBuf[i].r : 0));
		pOut->rgbBuf[i].g = (SINT16)(pOut->rgbBuf[i].g > maxval ? maxval : 
							(pOut->rgbBuf[i].g > 0 ? pOut->rgbBuf[i].g : 0));
		pOut->rgbBuf[i].b = (SINT16)(pOut->rgbBuf[i].b > maxval ? maxval : 
							(pOut->rgbBuf[i].b > 0 ? pOut->rgbBuf[i].b : 0));

		pEdge->yuvBuf[i].y = (UINT8)this->GetEdge();
		pEdge->yuvBuf[i].uv = 128;
		gpixel = InverseBit(gpixel);
	}
	this->gBgline = InverseBit(this->gBgline);
	this->gGpixel = InverseBit(this->gGpixel);
}

void CIspInterpolate::CopyToLineBuf(ImageStream *pIn, int line)
{
	int i;

	this->gpISbuf[line]->vs = 0;
	this->gpISbuf[line]->width = pIn->width + 8;
	memcpy(this->gpISbuf[line]->buf16+4, pIn->buf16, pIn->width * sizeof(short int));

	for(i = 0; i < 4; i++)
		this->gpISbuf[line]->buf16[i] = pIn->buf16[4-i];
	for(i = 0; i < 4; i++)
		this->gpISbuf[line]->buf16[pIn->width+4+i] = pIn->buf16[pIn->width-2-i];
}

void CIspInterpolate::Interpolate(ImageStream *pIn, ImageStream *pOut, ImageStream *pEdge)
{
	int height;

	height = this->GetInterReg(REG_ISP_IMGHT+1);
	height <<= 8;
	height += this->GetInterReg(REG_ISP_IMGHT);
	if(this->gInLine > height+3)		//outside region, cant enter is right
	{
		pOut->width = 0;
		return;
	}
	else if(this->gInLine == 0)					//fill internal line buffer
	{
		this->CopyToLineBuf(pIn, 4);
		pOut->width = 0;
		return;
	}
	else if(this->gInLine < 4)
	{
		this->CopyToLineBuf(pIn, 4 - this->gInLine);
		this->CopyToLineBuf(pIn, 4 + this->gInLine);
		pOut->width = 0;
		return;
	}
	else if(this->gInLine == 4)
	{
		this->CopyToLineBuf(pIn, 0);
		this->CopyToLineBuf(pIn, 8);
	}
	else if(this->gInLine >= height)
	{
		this->AdjustISPoint(this->gpISbuf, 9);
		CChipBase1::CopyImageStream(this->gpISbuf[8-((this->gInLine-height+1) << 1)], this->gpISbuf[8]);
	}
	else if(this->gInLine < height)
	{
		this->AdjustISPoint(this->gpISbuf, 9);
		this->CopyToLineBuf(pIn, 8);
	}
	this->OutputStream(pOut, pEdge);
}

int CIspInterpolate::Process(ImageStream *pIn, ImageStream *pOut, ImageStream *pEdge)
{
	int flag;

	Gdbug.DbugPrintData(ISP_INTERPOLATE_PRINT_INPUT, pIn);
	this->VsyncInit(pIn);
	if(this->varify(pIn) != SUCCEED)
		return FLOW_ERROR;

	flag = this->FillImageInfo(pIn, pOut, pEdge);
	if(flag == VSTART_PERIOD)	return FLOW_NORMAL;
	if(flag == INVAILD_PERIOD)	return FLOW_ERROR;
	if(flag == INSERT_BLANK)	return FLOW_ERROR;
	if(this->IsFrameOver(pOut))	return FLOW_FRAME;

	this->Interpolate(pIn, pOut, pEdge);
	this->AddInline(pIn);
	return this->AddOutLine(pOut);
}



//===================== class CIspRgbBase ======================

int CIspRgbBase::varify(ImageStream *pIn)
{
	if(pIn->format != RAWRGB)
	{
		Gdbug.DbugMsg("ISP: Input image format not equal Isp register setting!\n");
		return FAILED;
	}
	return CChipBase1::varify(pIn);
};


//===================== class CIspColorCorrect ======================

void CIspColorCorrect::ColorCorrect(ImageStream *pIn, ImageStream *pOut)
{
	int		i, k;
	SINT16	maxval = 255;
	int		cmat[3][3], off[3], temp;

	if(this->GetInterReg(REG_ISP_BLACKNCAP) & 1)	//10 bit sensor
		maxval = 1023;
	for(i = 0; i < 3; i++)
		for(k = 0; k < 3; k++)
			cmat[i][k] = this->GetInterReg(REG_ISP_CMAT11 + 3 * i + k);
	off[0] = this->GetInterReg(REG_ISP_OFSR);
	off[1] = this->GetInterReg(REG_ISP_OFSG);
	off[2] = this->GetInterReg(REG_ISP_OFSB);
	for(i = 0; i < pOut->width; i++)
	{
		temp = cmat[0][0] * pIn->rgbBuf[i].r + cmat[0][1] * pIn->rgbBuf[i].g + 
							cmat[0][2] * pIn->rgbBuf[i].b;
		pOut->rgbBuf[i].r = (SINT16)((temp >> 6) + off[0]);

		temp = cmat[1][0] * pIn->rgbBuf[i].r + cmat[1][1] * pIn->rgbBuf[i].g + 
							cmat[1][2] * pIn->rgbBuf[i].b;
		pOut->rgbBuf[i].g = (SINT16)((temp >> 6) + off[1]);

		temp = cmat[2][0] * pIn->rgbBuf[i].r + cmat[2][1] * pIn->rgbBuf[i].g + 
							cmat[2][2] * pIn->rgbBuf[i].b;
		pOut->rgbBuf[i].b = (SINT16)((temp >> 6) + off[2]);

		pOut->rgbBuf[i].r = (SINT16)(pOut->rgbBuf[i].r > maxval ? maxval : 
							(pOut->rgbBuf[i].r > 0 ? pOut->rgbBuf[i].r : 0));
		pOut->rgbBuf[i].g = (SINT16)(pOut->rgbBuf[i].g > maxval ? maxval : 
							(pOut->rgbBuf[i].g > 0 ? pOut->rgbBuf[i].g : 0));
		pOut->rgbBuf[i].b = (SINT16)(pOut->rgbBuf[i].b > maxval ? maxval : 
							(pOut->rgbBuf[i].b > 0 ? pOut->rgbBuf[i].b : 0));
	}
}

int CIspColorCorrect::Process(ImageStream *pIn, ImageStream *pOut)
{
	int flag;

	Gdbug.DbugPrintData(ISP_COLORCORRECT_PRINT_INPUT, pIn);
	this->VsyncInit(pIn);
	if(this->varify(pIn) != SUCCEED)
		return FLOW_ERROR;

//	flag = this->FillImageInfo(pIn, pOut);
	flag = CChipBase1::FillImageInfo(pIn, pOut);
	if(flag == VSTART_PERIOD)	return FLOW_NORMAL;
	if(flag == VEND_PERIOD)		return FLOW_FRAME;
	if(flag == INVAILD_PERIOD)	return FLOW_ERROR;
	if(flag == INSERT_BLANK)	return FLOW_ERROR;
	if(this->IsFrameOver(pOut))	return FLOW_FRAME;

	if((this->GetInterReg(REG_ISP_BASCTL+1) & 0x10) == 0)		//color correction disable
		return CChipBase1::Process(pIn, pOut);
	this->ColorCorrect(pIn, pOut);
	this->AddInline(pIn);
	return this->AddOutLine(pOut);
}


//===================== class CIspGammaCorrect ======================

int CIspGammaCorrect::GetGammaValue(int val, int regadr)
{
	int	x1;
	int	y1, y2;
	int	flag;
	int		result;

	flag = val >> 4;
	x1 = flag << 4;

	y1 = this->GetInterReg(regadr + flag);
	y2 = this->GetInterReg(regadr + flag + 1);

	result = y2 - y1;
	result *= val - x1;
	result >>= 4;
	result += y1;

	return result > 255 ? 255 : result;
}

void CIspGammaCorrect::GammaCorrect(ImageStream *pIn, ImageStream *pOut)
{
	int i;
	int flag = 0;

	flag = this->GetInterReg(REG_ISP_BLACKNCAP) & 1;
	for(i = 0; i < pOut->width; i++)
	{
		if(flag)		//10 bit data
		{
			pOut->rgbBuf[i].r = (SINT16)this->GetGammaValue((UINT8)(pIn->rgbBuf[i].r >> 2), REG_ISP_RGMST0);
			pOut->rgbBuf[i].g = (SINT16)this->GetGammaValue((UINT8)(pIn->rgbBuf[i].g >> 2), REG_ISP_GGMST0);
			pOut->rgbBuf[i].b = (SINT16)this->GetGammaValue((UINT8)(pIn->rgbBuf[i].b >> 2), REG_ISP_BGMST0);
		}
		else
		{
			pOut->rgbBuf[i].r = (SINT16)this->GetGammaValue((UINT8)(pIn->rgbBuf[i].r), REG_ISP_RGMST0);
			pOut->rgbBuf[i].g = (SINT16)this->GetGammaValue((UINT8)(pIn->rgbBuf[i].g), REG_ISP_GGMST0);
			pOut->rgbBuf[i].b = (SINT16)this->GetGammaValue((UINT8)(pIn->rgbBuf[i].b), REG_ISP_BGMST0);
		}
	}
}

int CIspGammaCorrect::Process(ImageStream *pIn, ImageStream *pOut)
{
	int flag, i;

	Gdbug.DbugPrintData(ISP_GAMMACORRECT_PRINT_INPUT, pIn);
	this->VsyncInit(pIn);
	if(this->varify(pIn) != SUCCEED)
		return FLOW_ERROR;

//	flag = this->FillImageInfo(pIn, pOut);
	flag = CChipBase1::FillImageInfo(pIn, pOut);
	if(flag == VSTART_PERIOD)	return FLOW_NORMAL;
	if(flag == VEND_PERIOD)		return FLOW_FRAME;
	if(flag == INVAILD_PERIOD)	return FLOW_ERROR;
	if(flag == INSERT_BLANK)	return FLOW_ERROR;
	if(this->IsFrameOver(pOut))	return FLOW_FRAME;

	if((this->GetInterReg(REG_ISP_BASCTL+1) & 0x8) == 0)		//gamma correction disable
	{
		if(this->GetInterReg(REG_ISP_BLACKNCAP) & 1)
		{
			for(i = 0; i < pOut->width; i++)
			{
				pOut->rgbBuf[i].r = pIn->rgbBuf[i].r >> 2;
				pOut->rgbBuf[i].g = pIn->rgbBuf[i].g >> 2;
				pOut->rgbBuf[i].b = pIn->rgbBuf[i].b >> 2;
			}
			this->AddInline(pIn);
			return this->AddOutLine(pOut);
		}
		else
			return CChipBase1::Process(pIn, pOut);
	}
	this->GammaCorrect(pIn, pOut);
	this->AddInline(pIn);
	return this->AddOutLine(pOut);
}


//===================== class CIspColorConvert ======================

int	CIspColorConvert::FillImageInfo(ImageStream *pIn, ImageStream *pOut)
{
	int flag;

//	flag = CIspRgbBase::FillImageInfo(pIn, pOut);
	flag = CChipBase1::FillImageInfo(pIn, pOut);
	pOut->format = YUV422;
	return flag;
}

void CIspColorConvert::ColorConvert(ImageStream *pIn, ImageStream *pOut)
{
	int i;

#if 1
	for(i = 0; i < pOut->width; i += 2)
	{
		this->gTools.rgbToyuv422(pIn->rgbBuf+i, pOut->yuvBuf+i);
	}
#endif
#if 0
	int y, u, v;
	int r, g, b;

	for(i = 0; i < pOut->width; i++)
	{
		r = pIn->rgbBuf[i].r;
		g = pIn->rgbBuf[i].g;
		b = pIn->rgbBuf[i].b;
		y = (77 * r + 150 * g + 29 * b) >> 8;
		u = ((128 * b - 43 * r - 85 * g) >> 8) + 128;
		v = ((128 * r - 107 * g - 21 * b) >> 8) + 128;

		y = (y > 255) ? 255 : ((y < 0) ? 0 : y);
		u = (u > 255) ? 255 : ((u < 0) ? 0 : u);
		v = (v > 255) ? 255 : ((v < 0) ? 0 : v);

		pOut->yuvBuf[i].y = y;
		pOut->yuvBuf[i].uv = u;
		if(i % 2)
			pOut->yuvBuf[i].uv = v;
	}
#endif

}

int CIspColorConvert::Process(ImageStream *pIn, ImageStream *pOut)
{
	int flag;

	Gdbug.DbugPrintData(ISP_COLORCONVERT_PRINT_INPUT, pIn);
	this->VsyncInit(pIn);
	if(this->varify(pIn) != SUCCEED)
		return FLOW_ERROR;

	flag = this->FillImageInfo(pIn, pOut);
	if(flag == VSTART_PERIOD)	return FLOW_NORMAL;
	if(flag == VEND_PERIOD)		return FLOW_FRAME;
	if(flag == INVAILD_PERIOD)	return FLOW_ERROR;
	if(flag == INSERT_BLANK)	return FLOW_ERROR;
	if(this->IsFrameOver(pOut))	return FLOW_FRAME;

	this->ColorConvert(pIn, pOut);
	this->AddInline(pIn);
	return this->AddOutLine(pOut);
}


//===================== class CIspEdgeEnhance ======================

void CIspEdgeEnhance::EdgeEnhance(ImageStream *pIn, ImageStream *pEdge, ImageStream *pOut)
{
	int i;
	int val1, val2;

	for(i = 0; i < pOut->width; i++)
	{
		val1 = pIn->yuvBuf[i].y & 0xff;
		val2 = pEdge->yuvBuf[i].y;
		val1 += val2;
		if(val2 != 0)
		{
			val2 = val2;
		}
		pOut->yuvBuf[i].y = (UINT8)(val1 > 255 ? 255 : (val1 < 0 ? 0 : (UINT8)val1));
//		pOut->yuvBuf[i].y = pIn->yuvBuf[i].y + pEdge->yuvBuf[i].y;
		pOut->yuvBuf[i].uv = (UINT8)(pIn->yuvBuf[i].uv);
	}
}

int CIspEdgeEnhance::Process(ImageStream *pIn, ImageStream *pEdge, ImageStream *pOut)
{
	int flag;

	Gdbug.DbugPrintData(ISP_EDGEENHANCE_PRINT_INPUT, pIn);
	Gdbug.DbugPrintData(ISP_EDGE_PRINT_INPUT, pEdge);
	this->VsyncInit(pIn);
	if(this->varify(pIn) != SUCCEED)
		return FLOW_ERROR;

//	flag = this->FillImageInfo(pIn, pOut);
	flag = CChipBase1::FillImageInfo(pIn, pOut);
	if(flag == VSTART_PERIOD)	return FLOW_NORMAL;
	if(flag == VEND_PERIOD)		return FLOW_FRAME;
	if(flag == INVAILD_PERIOD)	return FLOW_ERROR;
	if(flag == INSERT_BLANK)	return FLOW_ERROR;

	if(this->IsFrameOver(pOut))	return FLOW_FRAME;

	if((this->GetInterReg(REG_ISP_BASCTL+1) & 0x1) == 0)		//edge enhance disable
		return CChipBase1::Process(pIn, pOut);

	this->EdgeEnhance(pIn, pEdge, pOut);
	this->AddInline(pIn);
	return this->AddOutLine(pOut);
}







//===================== class CIsp ======================

CIsp::CIsp(void)
{
	this->RegInit();
}

CIsp::~CIsp(void)
{
}

void CIsp::RegInit(void)
{
	CChipBase1::RegInit();
	this->SetReg(REG_ISP_BASCTL,	0x33);
	this->SetReg(REG_ISP_BASCTL+1,	0x38);
	this->SetReg(REG_ISP_WINWD,		0x40);
	this->SetReg(REG_ISP_WINWD+1,	0x1);
	this->SetReg(REG_ISP_WINHT,		0xf0);
	//this->SetReg(REG_ISP_WINHT+1,	0x0);
	this->SetReg(REG_ISP_IMGWD,		0x80);
	this->SetReg(REG_ISP_IMGWD+1,	0x2);
	this->SetReg(REG_ISP_IMGHT,		0xe0);
	this->SetReg(REG_ISP_IMGHT+1,	0x1);
	this->SetReg(REG_ISP_FEM_M,		0x8);
	this->SetReg(REG_ISP_FEM_P,		0xc);
	this->SetReg(REG_ISP_FEM_X1,	0x5);
	this->SetReg(REG_ISP_FEM_X2,	0xf);
	this->SetReg(REG_ISP_FEM_MIN,	0x5);
	this->SetReg(REG_ISP_FEM_MAX,	0xf);
	this->SetReg(REG_ISP_LFCENX,	0x40);
	this->SetReg(REG_ISP_LFCENX+1,	0x1);
	this->SetReg(REG_ISP_LFCENY,	0xf0);
	//this->SetReg(REG_ISP_LFCENY+1,	0x0);
	this->SetReg(REG_ISP_LFFREC,	0x43);
	//this->SetReg(REG_ISP_LFFREC+1,	0x0);
	this->SetReg(REG_ISP_RGAIN,		0x40);
	this->SetReg(REG_ISP_GGAIN,		0x40);
	this->SetReg(REG_ISP_BGAIN,		0x40);
	this->SetReg(REG_ISP_GLBGAIN,	0x40);
	//this->SetReg(REG_ISP_GLBGAIN+1,	0);
	//this->SetReg(REG_ISP_CRADJ,		0x0);
	//this->SetReg(REG_ISP_CBADJ,		0x0);
	//this->SetReg(REG_ISP_CROFS,		0x0);
	//this->SetReg(REG_ISP_CBOFS,		0x0);
	this->SetReg(REG_ISP_AFCTRL,	0x33);
	//this->SetReg(REG_ISP_AFCTRL+1,	0x0);
	this->SetReg(REG_ISP_AFWHA,		0x5);
	this->SetReg(REG_ISP_AFWHB,		0xf);
	this->SetReg(REG_ISP_AFWHC,		0x28);
	this->SetReg(REG_ISP_AFWVA,		0x5);
	this->SetReg(REG_ISP_AFWVB,		0xa);
	this->SetReg(REG_ISP_AFWVC,		0x1e);

	this->SetReg(REG_ISP_AEWF,		0x21);
	this->SetReg(REG_ISP_AEWF+1,	0x43);
	//this->SetReg(REG_ISP_YBOT,		0x0);
	this->SetReg(REG_ISP_YTOP,		0xff);
	this->SetReg(REG_ISP_QTOP,		0xff);
	this->SetReg(REG_ISP_ITOP,		0xff);
	this->SetReg(REG_ISP_GBOT,		0x14);
	this->SetReg(REG_ISP_GTOP,		0x3f);
	this->SetReg(REG_ISP_AWBCTRL,	0x3c);
	this->SetReg(REG_ISP_AWBCTRL+1,	0xc4);

	//================= add black level register ==========
	this->SetReg(REG_ISP_RLEVEL,	0x3);
	this->SetReg(REG_ISP_BLEVEL,	0x3);
	this->SetReg(REG_ISP_GLEVEL1,	0x3);
	this->SetReg(REG_ISP_GLEVEL2,	0x3);
	//this->SetReg(REG_ISP_BLACKNCAP,	0x0);
}

void CIsp::SetReg(int adr, int val)
{
	CChipBase1::SetReg(adr, val);
	this->gBlack.SetReg(adr, val);
	this->gTestBar.SetReg(adr, val);
	this->gLens.SetReg(adr, val);
	this->gDpd.SetReg(adr, val);
	this->gDigitalGain.SetReg(adr, val);
	this->gInterpolate.SetReg(adr, val);
	this->gColorCorrect.SetReg(adr, val);
	this->gGammaCorrect.SetReg(adr, val);
	this->gColorConvert.SetReg(adr, val);
	this->gEdgeEnhance.SetReg(adr, val);

	this->gSize.SetReg(adr, val);
}

int CIsp::Process(ImageStream *pIn, ImageStream *pOut, ImageStream *pEdge)
{
	Gdbug.DbugPrintData(ISP_PRINT_INPUT, pIn);

	if(pIn->format == YUV422)
	{
//		CChipBase1::varify(pIn);
//		CChipBase1::VsyncInit(pIn);
		CChipBase1::FillImageInfo(pIn, pOut);
		return CChipBase1::Process(pIn, pOut);
	}

	if(this->gSize.Process(pIn, &(this->gISbuf[1])) == FLOW_ERROR)					
		return FLOW_ERROR;

	if(this->gBlack.Process(&(this->gISbuf[1]), &(this->gISbuf[0])) == FLOW_ERROR)					
		return FLOW_ERROR;
	if(this->gTestBar.Process(&(this->gISbuf[0]), &(this->gISbuf[1])) == FLOW_ERROR)		
		return FLOW_ERROR;
	if(this->gLens.Process(&(this->gISbuf[1]), &(this->gISbuf[0])) == FLOW_ERROR)
		return FLOW_ERROR;
	if(this->gDpd.Process(&(this->gISbuf[0]), &(this->gISbuf[1])) == FLOW_ERROR)	
		return FLOW_ERROR;
	if(this->gDigitalGain.Process(&(this->gISbuf[1]), &(this->gISbuf[0])) == FLOW_ERROR)	
		return FLOW_ERROR;
	if(this->gInterpolate.Process(&(this->gISbuf[0]), &(this->gISbuf[1]), pEdge) == FLOW_ERROR)	
		return FLOW_ERROR;
	if(this->gColorCorrect.Process(&(this->gISbuf[1]), &(this->gISbuf[0])) == FLOW_ERROR)	
		return FLOW_ERROR;
	if(this->gGammaCorrect.Process(&(this->gISbuf[0]), &(this->gISbuf[1])) == FLOW_ERROR)	
		return FLOW_ERROR;
	if(this->gColorConvert.Process(&(this->gISbuf[1]), &(this->gISbuf[0])) == FLOW_ERROR)	
		return FLOW_ERROR;

	return this->gEdgeEnhance.Process(&(this->gISbuf[0]), pEdge, pOut);
}




