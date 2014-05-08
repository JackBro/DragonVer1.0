

#include "dbugPrint.h"

#include "ipp.h"


//====================== class CIppZoom ==========================

int	CIppZoom::FillImageInfo(ImageStream *pIn, ImageStream *pOut)
{
	int	width, height, px, py, sizex, sizey;
	int	flag;
	
	width = this->GetInterReg(REG_IPP_IMGWD+1);
	width <<= 8;
	width += this->GetInterReg(REG_IPP_IMGWD);
	height = this->GetInterReg(REG_IPP_IMGHT+1);
	height <<= 8;
	height += this->GetInterReg(REG_IPP_IMGHT);

	px = this->GetInterReg(REG_IPP_SIZSTX+1);
	px <<= 8;
	px += this->GetInterReg(REG_IPP_SIZSTX);
	py = this->GetInterReg(REG_IPP_SIZSTY+1);
	py <<= 8;
	py += this->GetInterReg(REG_IPP_SIZSTY);

	sizex = this->GetInterReg(REG_IPP_SIZWD+1);
	sizex <<= 8;
	sizex += this->GetInterReg(REG_IPP_SIZWD);
	sizey = this->GetInterReg(REG_IPP_SIZHT+1);
	sizey <<= 8;
	sizey += this->GetInterReg(REG_IPP_SIZHT);

	flag = CChipBase1::FillImageInfo(pIn, pOut);
	if( flag != VSYNC_PERIOD )
		return flag;

	if(width > pIn->width)
	{
		Gdbug.DbugMsg("IPP: ImageWidth Setting > InputImageWidth\n");
		return INVAILD_PERIOD;
	}
	if(height > pIn->height)
	{
		Gdbug.DbugMsg("IPP: ImageHeight Setting > InputImageHeight\n");
		return INVAILD_PERIOD;
	}
	if( (px + sizex > width) || (py + sizey > height) )
	{
		Gdbug.DbugMsg("IPP: Sizer window's offset or size error\n");
		return INVAILD_PERIOD;
	}
	pOut->height = sizey;
	pOut->width = sizex;

	return VSYNC_PERIOD;
}

void CIppZoom::Zoom(ImageStream *pIn, ImageStream *pOut)
{
	int	px, py, sizex, sizey;
	
	px = this->GetInterReg(REG_IPP_SIZSTX+1);
	px <<= 8;
	px += this->GetInterReg(REG_IPP_SIZSTX);
	py = this->GetInterReg(REG_IPP_SIZSTY+1);
	py <<= 8;
	py += this->GetInterReg(REG_IPP_SIZSTY);

	sizex = this->GetInterReg(REG_IPP_SIZWD+1);
	sizex <<= 8;
	sizex += this->GetInterReg(REG_IPP_SIZWD);
	sizey = this->GetInterReg(REG_IPP_SIZHT+1);
	sizey <<= 8;
	sizey += this->GetInterReg(REG_IPP_SIZHT);

	if(this->gInLine < py)
	{
		pOut->width = 0;
		return;
	}
	memcpy(pOut->buf16, pIn->buf16+px, pOut->width << 1);
}

int CIppZoom::Process(ImageStream *pIn, ImageStream *pOut)
{
	int flag;

	Gdbug.DbugPrintData(IPP_ZOOM_PRINT_INPUT, pIn);
	this->VsyncInit(pIn);
	if(this->varify(pIn) != SUCCEED)
		return FLOW_ERROR;

	flag = this->FillImageInfo(pIn, pOut);
	if(flag == VSTART_PERIOD)	return FLOW_NORMAL;
	if(flag == VEND_PERIOD)		return FLOW_FRAME;
	if(flag == INVAILD_PERIOD)	return FLOW_ERROR;
	if(flag == INSERT_BLANK)	return FLOW_ERROR;
	if(this->IsFrameOver(pOut))	return FLOW_FRAME;

	this->Zoom(pIn, pOut);
	this->AddInline(pIn);
	return this->AddOutLine(pOut);
}


//====================== class CIppSpecial ==========================

void CIppSpecial::Sephia(ImageStream *pIn, ImageStream *pOut)
{
	UINT8 u, v;
	int i;

	u = (UINT8)this->GetInterReg(REG_IPP_UOFFSET);
	v = (UINT8)this->GetInterReg(REG_IPP_VOFFSET);
	for(i = 0; i < pOut->width; i+=2)
	{
		pOut->yuvBuf[i].y = pIn->yuvBuf[i].y;
		pOut->yuvBuf[i+1].y = pIn->yuvBuf[i+1].y;
		pOut->yuvBuf[i].uv = u;
		pOut->yuvBuf[i+1].uv = v;
	}
}

void CIppSpecial::ColorRange(ImageStream *pIn, ImageStream *pOut)
{
	int utop, ubottom, vtop, vbottom, val;
	int i;

	utop = this->GetInterReg(REG_IPP_UUPTHRS);
	ubottom = this->GetInterReg(REG_IPP_UDWTHRS);
	vtop = this->GetInterReg(REG_IPP_VUPTHRS);
	vbottom = this->GetInterReg(REG_IPP_VDWTHRS);

	for(i = 0; i < pOut->width; i += 2)
	{
		pOut->yuvBuf[i].y = pIn->yuvBuf[i].y;
		pOut->yuvBuf[i+1].y = pIn->yuvBuf[i+1].y;
		val = pIn->yuvBuf[i].uv;
		pOut->yuvBuf[i].uv = (UINT8)(val > utop ? 0x0 :(val < ubottom ? 0x0 : val));
		val = pIn->yuvBuf[i+1].uv;
		pOut->yuvBuf[i+1].uv = (UINT8)(val > vtop ? 0x0 :(val < vbottom ? 0x0 : val));
	}
}

void CIppSpecial::Negative(ImageStream *pIn, ImageStream *pOut)
{
	int		i;
	RawRgbType	rgb[2];

	for(i = 0; i < pOut->width; i += 2)
	{
		this->gTools.yuv422Torgb(pIn->yuvBuf+i, rgb);
		rgb[0].r = 255 - rgb[0].r;
		rgb[0].g = 255 - rgb[0].g;
		rgb[0].b = 255 - rgb[0].b;
		rgb[1].r = 255 - rgb[1].r;
		rgb[1].g = 255 - rgb[1].g;
		rgb[1].b = 255 - rgb[1].b;
		this->gTools.rgbToyuv422(rgb, pOut->yuvBuf+i);
	}
}

void CIppSpecial::Special(ImageStream *pIn, ImageStream *pEdge, ImageStream *pOut)
{
	int	flag;

	pEdge->IsValid();
	flag = (this->GetInterReg(REG_IPP_SPECTRL) & 0xf) >> 1;
	switch(flag)
	{
	case 0:
		this->Sephia(pIn, pOut);
		break;
	case 1:
		this->ColorRange(pIn, pOut);
		break;
	case 2:
		this->Negative(pIn, pOut);
		break;
	case 3:
	case 4:
	default:
		this->CopyImageStream(pIn, pOut);
		break;
	}
}

int CIppSpecial::Process(ImageStream *pIn, ImageStream *pEdge, ImageStream *pOut)
{
	int flag;

	Gdbug.DbugPrintData(IPP_SPECIAL_PRINT_INPUT, pIn);
	this->VsyncInit(pIn);
	if(this->varify(pIn) != SUCCEED)
		return FLOW_ERROR;

	flag = this->FillImageInfo(pIn, pOut);
	if(flag == VSTART_PERIOD)	return FLOW_NORMAL;
//	if(flag == VEND_PERIOD)		return FLOW_NORMAL;
	if(flag == INVAILD_PERIOD)	return FLOW_ERROR;
	if(flag == INSERT_BLANK)	return FLOW_ERROR;
	if(this->IsFrameOver(pOut))	return FLOW_FRAME;

	if( (this->GetInterReg(REG_IPP_SPECTRL) & 0x1) == 0 )		//special effect disable
		return CChipBase1::Process(pIn, pOut);

	this->Special(pIn, pEdge, pOut);
	this->AddInline(pIn);
	return this->AddOutLine(pOut);
}


//================ class CIppPrefilter =========

CIppPrefilter::CIppPrefilter(void)
{
	int i;

	for(i = 0; i < 8; i++)
		this->gpISbuf[i] = &(this->gISbuf[i]);
	for(i = 0; i < 7; i++)
		this->gFilter[i] = 0;
	this->gFilter[3] = 256;
}

int	 CIppPrefilter::HorFirstUnit(ImageStream *pIn, pYuv444Type dat)
{
	int i, j;

	for(i = 0; i < 4; i++)
	{
		j = 3 - i;
		dat[i].y = pIn->yuvBuf[j].y;
		if( j % 2)
		{
			dat[i].u = pIn->yuvBuf[j-1].uv;
			dat[i].v = pIn->yuvBuf[j].uv;
		}
		else
		{
			dat[i].u = pIn->yuvBuf[j].uv;
			dat[i].v = pIn->yuvBuf[j+1].uv;
		}
	}
	//dat[0].y = pIn->yuvBuf[3].y;
	//dat[0].u = pIn->yuvBuf[2].uv;
	//dat[0].v = pIn->yuvBuf[3].uv;

	//dat[1].y = pIn->yuvBuf[2].y;
	//dat[1].u = pIn->yuvBuf[2].uv;
	//dat[1].v = pIn->yuvBuf[3].uv;

	//dat[2].y = pIn->yuvBuf[1].y;
	//dat[2].u = pIn->yuvBuf[0].uv;
	//dat[2].v = pIn->yuvBuf[1].uv;

	//dat[3].y = pIn->yuvBuf[0].y;
	//dat[3].u = pIn->yuvBuf[0].uv;
	//dat[3].v = pIn->yuvBuf[1].uv;

	for(i = 0; i < 3; i++)
		dat[6-i] = dat[i];

	return 0;
}

int CIppPrefilter::HorNextUnit(UINT8 y, UINT8 u, UINT8 v, int pos, pYuv444Type dat)
{
//	int num;
//	num = (pos+4) % 7;
	dat[pos].y = y;
	dat[pos].u = u;
	dat[pos].v = v;
	pos++;
	return pos % 7;
}

Yuv444Type CIppPrefilter::GetFilterResult(pYuv444Type dat, int *filter, int pos)
{
	int temp, i;
	Yuv444Type yuv;

	temp = 0;
	for(i = 0; i < 7; i++)
		temp += dat[(i+pos) % 7].y * filter[i];
	temp >>= 8;
	yuv.y = (UINT8)temp;

	temp = 0;
	for(i = 0; i < 7; i++)
		temp += dat[(i+pos) % 7].u * filter[i];
	temp >>= 8;
	yuv.u = (UINT8)temp;

	temp = 0;
	for(i = 0; i < 7; i++)
		temp += dat[(i+pos) % 7].v * filter[i];
	temp >>= 8;
	yuv.v = (UINT8)temp;

//	printyuv(dat, pos, filter, yuv);
	return yuv;
}

void CIppPrefilter::HorPrefilter(ImageStream *pIn, int num)
{
	Yuv444Type	dat[7];
	int		i, pos = 0, j;

	for(i = 0; i < pIn->width; i++)
	{
		if(i == 0)
			pos = this->HorFirstUnit(pIn, dat);
		else if(i > pIn->width-4)
		{
			j = (pIn->width << 1) - i - 5;
			if(j % 2)
				pos = this->HorNextUnit(pIn->yuvBuf[j].y, pIn->yuvBuf[j-1].uv, pIn->yuvBuf[j].uv, pos, dat);
			else
				pos = this->HorNextUnit(pIn->yuvBuf[j].y, pIn->yuvBuf[j].uv, pIn->yuvBuf[j+1].uv, pos, dat);
		}
		else
		{
			if((i+3) % 2)
			{
				pos = this->HorNextUnit(pIn->yuvBuf[i+3].y, pIn->yuvBuf[i+2].uv, pIn->yuvBuf[i+3].uv, pos, dat);
			}
			else
				pos = this->HorNextUnit(pIn->yuvBuf[i+3].y, pIn->yuvBuf[i+3].uv, pIn->yuvBuf[i+4].uv, pos, dat);
		}

		this->gpISbuf[num]->yuv444[i] = this->GetFilterResult(dat, this->gFilter, pos);
	}
//	this->gpISbuf[num]->vs = 0;
	this->gpISbuf[num]->width = pIn->width;
	this->gpISbuf[num]->format = YUV444;
}

void CIppPrefilter::FillFilter(void)
{
	this->gFilter[3] = this->GetInterReg(REG_IPP_FIRCF+1);
	this->gFilter[2] = this->GetInterReg(REG_IPP_FIRCF+0);
	this->gFilter[1] = this->GetInterReg(REG_IPP_FIRCF+3);
	this->gFilter[0] = this->GetInterReg(REG_IPP_FIRCF+2);
	this->gFilter[4] = this->gFilter[2];
	this->gFilter[5] = this->gFilter[1];
	this->gFilter[6] = this->gFilter[0];
}

void CIppPrefilter::VerPrefilter(ImageStream *pOut)
{
	Yuv444Type	dat[7], temp;
	int		i, j;

	pOut->width = this->gpISbuf[3]->width;

	for(i = 0; i < pOut->width; i++)
	{
		for(j = 0; j < 7; j++)
			dat[j] = this->gpISbuf[j]->yuv444[i];
		temp = this->GetFilterResult(dat, this->gFilter, 0);
		pOut->yuvBuf[i].y = temp.y;
		pOut->yuvBuf[i].uv = temp.u;
		if(i % 2)
			pOut->yuvBuf[i].uv = temp.v;
	}
}

void CIppPrefilter::Prefilter(ImageStream *pIn, ImageStream *pOut)
{
	int height;

	height = this->GetInterReg(REG_IPP_IMGHT);
	height <<= 8;
	height += this->GetInterReg(REG_IPP_IMGHT+1);
	if(this->gInLine > height+2)		//outside region, cant enter is right
	{
		pOut->width = 0;
		return;
	}
	else if(this->gInLine == 0)					//fill internal line buffer
	{
		this->HorPrefilter(pIn, 3);
		pOut->width = 0;
		return;
	}
	else if(this->gInLine < 3)
	{
		this->HorPrefilter(pIn, 3 - this->gInLine);
		this->gpISbuf[3+this->gInLine]->width = pIn->width;
		this->gpISbuf[3+this->gInLine]->format = YUV444;
		this->CopyImageStream(this->gpISbuf[3-this->gInLine], this->gpISbuf[3+this->gInLine]);
		pOut->width = 0;
		return;
	}
	else if(this->gInLine == 3)
	{
		this->HorPrefilter(pIn, 0);
		this->gpISbuf[6]->width = pIn->width;
		this->gpISbuf[6]->format = YUV444;
		this->CopyImageStream(this->gpISbuf[0], this->gpISbuf[6]);
	}
	else if(this->gInLine >= height)
	{
		this->AdjustISPoint(this->gpISbuf, 7);
		this->CopyImageStream(this->gpISbuf[6-((this->gInLine-height+1) << 1)], this->gpISbuf[6]);
	}
	else if(this->gInLine < height)
	{
		this->AdjustISPoint(this->gpISbuf, 7);
		this->HorPrefilter(pIn, 6);
	}
	this->VerPrefilter(pOut);
}

int CIppPrefilter::Process(ImageStream *pIn, ImageStream *pOut)
{
	int flag;

	Gdbug.DbugPrintData(IPP_PREFILTER_PRINT_INPUT, pIn);
	if(this->VsyncInit(pIn))
		this->FillFilter();
	if(this->varify(pIn) != SUCCEED)
		return FLOW_ERROR;

	flag = this->FillImageInfo(pIn, pOut);
	if(flag == VSTART_PERIOD)	return FLOW_NORMAL;
//	if(flag == VEND_PERIOD)		return FLOW_NORMAL;
	if(flag == INVAILD_PERIOD)	return FLOW_ERROR;
	if(flag == INSERT_BLANK)	return FLOW_ERROR;
	if(this->IsFrameOver(pOut))	return FLOW_FRAME;

	if( (this->GetInterReg(REG_IPP_SIZCTRL) & 0x4) == 0 )		//prefilter disable
		return CChipBase1::Process(pIn, pOut);

	this->Prefilter(pIn, pOut);
	this->AddInline(pIn);

	return this->AddOutLine(pOut);
}


//================ class CIppSizerBase =========

CIppSizerBase::CIppSizerBase(void)
{
	this->gpISbuf[0] = &(this->gISbuf[0]);
	this->gpISbuf[1] = &(this->gISbuf[1]);
	this->gUp = 1;
	this->gDown = 1;
	this->gVer = 0;
}

void CIppSizerBase::CheckFactor(int up, int down)
{
	this->gTools.AdjustFactor(&up, &down);
	this->gUp = up;
	this->gDown = down;
}

#if 1
UINT8 CIppSizerBase::GetYuv(UINT8 ul, UINT8 ur, UINT8 dl, UINT8 dr)
{
	int val[4];

	val[0] = ul;
	val[0] *= this->gUp - this->gHor;
	val[1] = ur;
	val[1] *= this->gHor;

	val[2] = dl;
	val[2] *= this->gUp - this->gHor;
	val[3] = dr;
	val[3] *= this->gHor;

	val[0] += val[1];
	val[0] *= this->gUp - this->gVer;
	val[2] += val[3];
	val[2] *= this->gVer;

	val[0] += val[2];
	val[0] /= this->gUp;

	return (UINT8)val[0];
}
#else
UINT8 CIppSizerBase::GetYuv(UINT16 ul, UINT16 ur, UINT16 dl, UINT16 dr)
{
	ul *= this->gDown - this->gHor;
	ur *= this->gHor;
	dl *= this->gDown - this->gHor;
	dr *= this->gHor;

	ul += ur;
	ul *= this->gDown - this->gVer;
	dl += dr;
	dl *= this->gVer;

	ul += dl;
	ul /= this->gDown;

	return (UINT8)ul;
}
#endif

void CIppSizerBase::SizerOut(ImageStream *pOut)
{
	int i;
	int offset = 0;

	this->gHor = 0;
	for(i = 0; i < this->gpISbuf[1]->width; i++)
	{
		this->gHor += this->gDown;
		if(this->gHor >= this->gUp)
		{
			this->gHor -= this->gUp;

			pOut->yuvBuf[offset].y = this->GetYuv(this->gpISbuf[0]->yuvBuf[i-1].y,
				this->gpISbuf[0]->yuvBuf[i].y, this->gpISbuf[1]->yuvBuf[i-1].y,
				this->gpISbuf[1]->yuvBuf[i].y);

			if(offset % 2)
			{
				if(i % 2)
					pOut->yuvBuf[offset].uv = this->GetYuv(this->gpISbuf[0]->yuvBuf[i].uv,
						this->gpISbuf[0]->yuvBuf[i].uv, this->gpISbuf[1]->yuvBuf[i].uv,
						this->gpISbuf[1]->yuvBuf[i].uv);
				else if(i < this->gpISbuf[1]->width - 1)
					pOut->yuvBuf[offset].uv = this->GetYuv(this->gpISbuf[0]->yuvBuf[i-1].uv,
						this->gpISbuf[0]->yuvBuf[i+1].uv, this->gpISbuf[1]->yuvBuf[i-1].uv,
						this->gpISbuf[1]->yuvBuf[i+1].uv);
			}
			else
			{
				if(i % 2)
					pOut->yuvBuf[offset].uv = this->GetYuv(this->gpISbuf[0]->yuvBuf[i-1].uv,
						this->gpISbuf[0]->yuvBuf[i-1].uv, this->gpISbuf[1]->yuvBuf[i-1].uv,
						this->gpISbuf[1]->yuvBuf[i-1].uv);
				else
					pOut->yuvBuf[offset].uv = this->GetYuv(this->gpISbuf[0]->yuvBuf[i-2].uv,
						this->gpISbuf[0]->yuvBuf[i].uv, this->gpISbuf[1]->yuvBuf[i-2].uv,
						this->gpISbuf[1]->yuvBuf[i].uv);
			}
			offset ++;
		}
	}
	if(offset % 2)
		pOut->width = offset - 1;
	else
		pOut->width = offset;
}

void CIppSizerBase::Sizer(ImageStream *pIn, ImageStream *pOut)
{
	if(this->gUp == this->gDown)
	{
		CChipBase1::CopyImageStream(pIn, pOut);
		return;
	}
	this->gVer += this->gDown;
	if(this->gInLine == 0)
	{
		this->gpISbuf[0]->format = pIn->format;
		this->gpISbuf[0]->width = pIn->width;
		CChipBase1::CopyImageStream(pIn, this->gpISbuf[0]);
		this->gpISbuf[1]->format = pIn->format;
		this->gpISbuf[1]->width = pIn->width;
		CChipBase1::CopyImageStream(pIn, this->gpISbuf[1]);
	}
	else
	{
		CChipBase1::AdjustISPoint(this->gpISbuf, 2);
		this->gpISbuf[1]->format = pIn->format;
		this->gpISbuf[1]->width = pIn->width;
		CChipBase1::CopyImageStream(pIn, this->gpISbuf[1]);
	}
		
	if(this->gVer >= this->gUp)
	{
		this->gVer -= this->gUp;
		this->SizerOut(pOut);
	}
	else
		pOut->width = 0;
}

//================ class CIppDisplaySizer =========

int CIppDisplaySizer::Process(ImageStream *pIn, ImageStream *pOut)
{
	int flag;

	Gdbug.DbugPrintData(IPP_DISPSIZER_PRINT_INPUT, pIn);
	if(this->VsyncInit(pIn) == TRUE)
		this->CheckFactor((UINT8)this->GetInterReg(REG_IPP_DPUPFT), (UINT8)this->GetInterReg(REG_IPP_DPDWFT));
	if(this->varify(pIn) != SUCCEED)
		return FLOW_ERROR;

	flag = this->FillImageInfo(pIn, pOut);
	if(flag == VSTART_PERIOD)	return FLOW_NORMAL;
	if(flag == VEND_PERIOD)		return FLOW_FRAME;
	if(flag == INVAILD_PERIOD)	return FLOW_ERROR;
	if(flag == INSERT_BLANK)	return FLOW_ERROR;

	if(this->IsFrameOver(pOut))	return FLOW_FRAME;

	if( (this->GetInterReg(REG_IPP_SIZCTRL) & 0x10) == 0 )		//display sizer disable
		return CChipBase1::Process(pIn, pOut);

	this->Sizer(pIn, pOut);
	this->AddInline(pIn);
	return this->AddOutLine(pOut);
}


//================ class CIppCaptureSizer =========

int CIppCaptureSizer::Process(ImageStream *pIn, ImageStream *pOut)
{
	int flag;

	Gdbug.DbugPrintData(IPP_CAPSIZER_PRINT_INPUT, pIn);
	if(this->VsyncInit(pIn) == TRUE)
		this->CheckFactor((UINT8)this->GetInterReg(REG_IPP_CPUPFT), (UINT8)this->GetInterReg(REG_IPP_CPDWFT));
	if(this->varify(pIn) != SUCCEED)
		return FLOW_ERROR;

	flag = this->FillImageInfo(pIn, pOut);
	if(flag == VSTART_PERIOD)	return FLOW_NORMAL;
	if(flag == VEND_PERIOD)		return FLOW_FRAME;
	if(flag == INVAILD_PERIOD)	return FLOW_ERROR;
	if(flag == INSERT_BLANK)	return FLOW_ERROR;

	if(this->IsFrameOver(pOut))	return FLOW_FRAME;

	if( (this->GetInterReg(REG_IPP_SIZCTRL) & 0x2) == 0 )		//capture sizer disable
		return CChipBase1::Process(pIn, pOut);

	this->Sizer(pIn, pOut);
	this->AddInline(pIn);
	return this->AddOutLine(pOut);
}


//================ class CIpp =========

CIpp::CIpp(void)
{
	this->RegInit();
}

CIpp::~CIpp(void)
{
}

void CIpp::RegInit(void)
{
	CChipBase1::RegInit();

//	this->SetReg(REG_IPP_SPECTRL,	0x00);
	this->SetReg(REG_IPP_SIZCTRL,	0x1f);
	this->SetReg(REG_IPP_IMGWD,		0x80);
	this->SetReg(REG_IPP_IMGWD+1,	0x2);
	this->SetReg(REG_IPP_IMGHT,		0xe0);
	this->SetReg(REG_IPP_IMGHT+1,	0x1);
	this->SetReg(REG_IPP_SIZWD,		0x80);
	this->SetReg(REG_IPP_SIZWD+1,	0x2);
	this->SetReg(REG_IPP_SIZHT,		0xe0);
	this->SetReg(REG_IPP_SIZHT+1,	0x1);
//	this->SetReg(REG_IPP_SIZSTX,	0x0);
//	this->SetReg(REG_IPP_SIZSTY,	0x0);
	this->SetReg(REG_IPP_FIRCF,		0x35);
	this->SetReg(REG_IPP_FIRCF+1,	0x3e);
	this->SetReg(REG_IPP_FIRCF+2,	0x0d);
	this->SetReg(REG_IPP_FIRCF+3,	0x1f);
	this->SetReg(REG_IPP_PIXRATE,	0x3);
	this->SetReg(REG_IPP_CPUPFT,	0x01);
	this->SetReg(REG_IPP_CPDWFT,	0x01);
	this->SetReg(REG_IPP_DPUPFT,	0x01);
	this->SetReg(REG_IPP_DPDWFT,	0x01);
	this->SetReg(REG_IPP_CAPWD,		0x80);
	this->SetReg(REG_IPP_CAPWD+1,	0x02);
	this->SetReg(REG_IPP_CAPHT,		0xe0);
	this->SetReg(REG_IPP_CAPHT+1,	0x01);
//	this->SetReg(REG_IPP_DISWD,		0x00);
	this->SetReg(REG_IPP_DISWD+1,	0x80);
//	this->SetReg(REG_IPP_DISHT,		0x00);
	this->SetReg(REG_IPP_DISHT+1,	0x60);
	this->SetReg(REG_IPP_YOFFSET,	0x80);
	this->SetReg(REG_IPP_UOFFSET,	0x80);
	this->SetReg(REG_IPP_VOFFSET,	0x80);
	this->SetReg(REG_IPP_UUPTHRS,	0xc0);
	this->SetReg(REG_IPP_UDWTHRS,	0x40);
	this->SetReg(REG_IPP_VUPTHRS,	0x60);
//	this->SetReg(REG_IPP_VDWTHRS,	0x00);
	this->SetReg(REG_IPP_FRMDP,		0xff);
	this->SetReg(REG_IPP_FRMDP+1,	0xff);
}

void CIpp::SetReg(int adr, int val)
{
	CChipBase1::SetReg(adr, val);

	this->gZoom.SetReg(adr, val);
	this->gSpecial.SetReg(adr, val);
	this->gPrefilter.SetReg(adr, val);
	this->gDisplay.SetReg(adr, val);
	this->gCapture.SetReg(adr, val);
}

int CIpp::Process(ImageStream *pIn, ImageStream *pEdge, ImageStream *pOutLcd, ImageStream *pOutLbuf)
{
	Gdbug.DbugPrintData(IPP_PRINT_INPUT, pIn);
	Gdbug.DbugPrintData(IPP_EDGEPRINT_INPUT, pEdge);

	if(this->gZoom.Process(pIn, &(this->gISbuf[0])) == FLOW_ERROR)
		return FLOW_ERROR;
	if(this->gSpecial.Process(&(this->gISbuf[0]), pEdge, &(this->gISbuf[1])) == FLOW_ERROR)
		return FLOW_ERROR;
	if(this->gPrefilter.Process(&(this->gISbuf[1]), &(this->gISbuf[0])) == FLOW_ERROR)
		return FLOW_ERROR;
	if(this->gDisplay.Process(&(this->gISbuf[0]), pOutLcd) == FLOW_ERROR)
		return FLOW_ERROR;

	return this->gCapture.Process(&(this->gISbuf[0]), pOutLbuf);
}

