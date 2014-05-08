

#include "dbugPrint.h"
#include "sensorIF.h"

CSensorIF::CSensorIF(void)
{
	this->RegInit();
}

CSensorIF::~CSensorIF(void)
{
}

void CSensorIF::RegInit(void)
{
	CChipBase1::RegInit();

	//this->SetReg(REG_SIF_HSYNC_STARTH,	0);
	this->SetReg(REG_SIF_HSYNC_STARTL,	0x4);
	this->SetReg(REG_SIF_HSYNC_STOPH,	0x2);
	this->SetReg(REG_SIF_HSYNC_STOPL,	0x83);

	//this->SetReg(REG_SIF_VSYNC_STARTH,	0x0);
	this->SetReg(REG_SIF_VSYNC_STARTL,	0x4);
	this->SetReg(REG_SIF_VSYNC_STOPH,	0x1);
	this->SetReg(REG_SIF_VSYNC_STOPL,	0xe3);

	this->SetReg(REG_SIF_SNR_MODE,		0x40);
	this->SetReg(REG_SIF_SNROP_MODE,	0x40);
}

//void CSensorIF::SetReg(int adr, int val)
//{
//	CChipBase1::SetReg(adr, val);
//	this->gReg[adr] = val;
//}

int CSensorIF::FillImageInfo(ImageStream *pIn, ImageStream *pOut)
{
	int hstart, hstop, vstart, vstop, width, height;
	int	flag;

	hstart = this->GetInterReg(REG_SIF_HSYNC_STARTH);
	hstart <<= 8;
	hstart += this->GetInterReg(REG_SIF_HSYNC_STARTL);
	hstop = this->GetInterReg(REG_SIF_HSYNC_STOPH);
	hstop <<= 8;
	hstop += this->GetInterReg(REG_SIF_HSYNC_STOPL);
	vstart = this->GetInterReg(REG_SIF_VSYNC_STARTH);
	vstart <<= 8;
	vstart += this->GetInterReg(REG_SIF_VSYNC_STARTL);
	vstop = this->GetInterReg(REG_SIF_VSYNC_STOPH);
	vstop <<= 8;
	vstop += this->GetInterReg(REG_SIF_VSYNC_STOPL);

	width = hstop - hstart + 1;
	height = vstop - vstart + 1;

	flag = CChipBase1::FillImageInfo(pIn, pOut);
	pOut->height = height;

	if(hstop <= hstart)
	{
		Gdbug.DbugMsg("SensorIF: hstop <= hstart\n");
		return INVAILD_PERIOD;
	}
	if(vstop < vstart)
	{
		Gdbug.DbugMsg("SensorIF: vstop <= vstart\n");
		return INVAILD_PERIOD;
	}
	if( flag != VSYNC_PERIOD )
		return flag;

	if(width > pIn->width)
	{
		Gdbug.DbugMsg("SensorIF: ImageWidth > InputImageWidth\n");
		return INVAILD_PERIOD;
	}
	if(height > pIn->height)
	{
		Gdbug.DbugMsg("SensorIF: ImageHeight > InputImageHeight\n");
		return INVAILD_PERIOD;
	}
	pOut->width = width;
	return VSYNC_PERIOD;
}

int CSensorIF::Process(ImageStream *pIn, ImageStream *pOut)
{
	int flag, i;
	int hstart, vstart, vstop;

	Gdbug.DbugPrintData(SIF_PRINT_INPUT, pIn);
	this->VsyncInit(pIn);
	if(this->varify(pIn) != SUCCEED)
		return FLOW_ERROR;

	flag = this->FillImageInfo(pIn, pOut);
	if(flag == VSTART_PERIOD)	return FLOW_NORMAL;
//	if(flag == VEND_PERIOD)		return FLOW_NORMAL;
	if(flag == INVAILD_PERIOD)	return FLOW_ERROR;
	if(flag == INSERT_BLANK)	return FLOW_ERROR;
	if(this->IsFrameOver(pOut))	return FLOW_FRAME;

	vstart = this->GetInterReg(REG_SIF_VSYNC_STARTH);
	vstart <<= 8;
	vstart += this->GetInterReg(REG_SIF_VSYNC_STARTL);
	vstop = this->GetInterReg(REG_SIF_VSYNC_STOPH);
	vstop <<= 8;
	vstop += this->GetInterReg(REG_SIF_VSYNC_STOPL);
	hstart = this->GetInterReg(REG_SIF_HSYNC_STARTH);
	hstart <<= 8;
	hstart += this->GetInterReg(REG_SIF_HSYNC_STARTL);

	if( (this->GetInterReg(REG_SIF_SNR_MODE) & 0x40) == 0 )		//raw data
	{
		if(this->gInLine < vstart)
			pOut->width = 0;
		else if(this->gInLine > vstop)
			pOut->width = 0;
		else
			memcpy(pOut->buf16, pIn->buf16+hstart, pOut->width * sizeof(short int));
	}
	else
	{
		if(this->gInLine < vstart)
			pOut->width = 0;
		else if(this->gInLine > vstop)
			pOut->width = 0;
		else
		{
			switch(this->GetInterReg(REG_SIF_YUV_FORMAT) & 3)
			{
			case 0:			//UYVY
				memcpy(pOut->buf16, pIn->buf16+hstart, pOut->width * sizeof(short int));
				break;
			case 1:			//VYUY
				for(i = 0; i < pOut->width; i+=2)
				{
					pOut->yuvBuf[i].y = pIn->yuvBuf[hstart+i].y;
					pOut->yuvBuf[i+1].y = pIn->yuvBuf[hstart+i+1].y;
					pOut->yuvBuf[i].uv = pIn->yuvBuf[hstart+i+1].uv;
					pOut->yuvBuf[i+1].uv = pIn->yuvBuf[hstart+i].uv;
				}
				break;
			case 2:			//YUYV
				for(i = 0; i < pOut->width; i+=2)
				{
					pOut->yuvBuf[i].y = pIn->yuvBuf[hstart+i].uv;
					pOut->yuvBuf[i+1].y = pIn->yuvBuf[hstart+i+1].uv;
					pOut->yuvBuf[i].uv = pIn->yuvBuf[hstart+i].y;
					pOut->yuvBuf[i+1].uv = pIn->yuvBuf[hstart+i+1].y;
				}
				break;
			case 3:			//YVYU
			default:
				for(i = 0; i < pOut->width; i+=2)
				{
					pOut->yuvBuf[i].y = pIn->yuvBuf[hstart+i+1].uv;
					pOut->yuvBuf[i+1].y = pIn->yuvBuf[hstart+i].uv;
					pOut->yuvBuf[i].uv = pIn->yuvBuf[hstart+i+1].y;
					pOut->yuvBuf[i+1].uv = pIn->yuvBuf[hstart+i].y;
				}
				break;
			}
		}
		pOut->format = YUV422;
	}

	this->AddInline(pIn);
	return this->AddOutLine(pOut);
}


