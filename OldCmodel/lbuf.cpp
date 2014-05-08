
#include "dbugPrint.h"
#include "lbuf.h"


//================ class CLbufZoom =========

int CLbufZoom::FillImageInfo(ImageStream *pIn, ImageStream *pOut)
{
	int flag;
	int	width, height;
	
	height = this->GetInterReg(REG_LBUF_SRCHEIGHT_H);
	height <<= 8;
	height += this->GetInterReg(REG_LBUF_SRCHEIGHT_L);
	width = this->GetInterReg(REG_LBUF_SRCWIDTH_H);
	width <<= 8;
	width += this->GetInterReg(REG_LBUF_SRCWIDTH_L);

	flag = CChipBase1::FillImageInfo(pIn, pOut);
	if( flag != VSYNC_PERIOD )
		return flag;

	if(height > pIn->height)
		height = pIn->height;
//		return INVAILD_PERIOD;
	pOut->height = height;
	pOut->width = width;

	return VSYNC_PERIOD;
}

int CLbufZoom::Process(ImageStream *pIn, ImageStream *pOut)
{
	int flag;

	Gdbug.DbugPrintData(LBUF_ZOOM_PRINT_INPUT, pIn);
	this->VsyncInit(pIn);
	if(this->varify(pIn) != SUCCEED)
		return FLOW_ERROR;

	flag = this->FillImageInfo(pIn, pOut);
	if(flag == VSTART_PERIOD)	return FLOW_NORMAL;
//	if(flag == VEND_PERIOD)		return FLOW_NORMAL;
	if(flag == INVAILD_PERIOD)	return FLOW_ERROR;
	if(flag == INSERT_BLANK)	return FLOW_NORMAL;
	if(this->IsFrameOver(pOut))	return FLOW_FRAME;

	return CChipBase1::Process(pIn, pOut);
}


//================ class CLbuf2Block =========

int CLbuf2Block::FillImageInfo(ImageStream *pIn, ImageBlock *pOut)
{
	int flag;
	int	width, height;
	
	width = this->GetInterReg(REG_LBUF_DSTWIDTH_H);
	width <<= 8;
	width += this->GetInterReg(REG_LBUF_DSTWIDTH_L);
	height = this->GetInterReg(REG_LBUF_DSTHEIGHT_H);
	height <<= 8;
	height += this->GetInterReg(REG_LBUF_DSTHEIGHT_L);

	flag = CChipBase1::FillImageInfo(pIn, pOut);
	if( flag != VSYNC_PERIOD )
		return flag;

	if(height > pIn->height)
		height = pIn->height;
	switch(pIn->format)
	{
	case YUV422:
		pOut->height = (height + 7) >> 3;		//only 422 format
		pOut->width = ((width + 15) >> 4) << 4;
		return VSYNC_PERIOD;
	case YUV411:
	case YUV420:
	case YUV400:
	default:
		return INVAILD_PERIOD;
	}
}

void CLbuf2Block::OutputBlock(ImageStream *pIn, ImageBlock *pOut)
{
//	int i, j, offset;
	int line;

	switch(pIn->format)
	{
	case YUV422:
		line = this->gInLine % 8;
		this->gISbuf[line].format = pIn->format;
		this->gISbuf[line].width = pIn->width;
//		this->gISbuf[line].height = pIn->height;
		CChipBase1::CopyImageStream(pIn, &(this->gISbuf[line]));

		pOut->format = YUV422BLK;
		if( ((this->gInLine % 8) == 7) || (this->gInLine == (pIn->height - 1)) )
		{
			CChipBase1::yuv422ToBlock(this->gISbuf, pOut);
			pOut->width = (pOut->width + 15) >> 4;
		}
		else
		{
			pOut->width = 0;
		}
		break;

	case YUV411:
	case YUV420:
	case YUV400:
	default:
		pOut->width = 0;
		return;
	}
}

int	CLbuf2Block::Process(ImageStream *pIn, ImageBlock *pOut)
{
	int flag;

	Gdbug.DbugPrintData(LBUF_2BLOCKPRINT_INPUT, pIn);
	this->VsyncInit(pIn);
	if(this->varify(pIn) != SUCCEED)
		return FLOW_ERROR;

	flag = this->FillImageInfo(pIn, pOut);
	if(flag == VSTART_PERIOD)	return FLOW_NORMAL;
//	if(flag == VEND_PERIOD)		return FLOW_NORMAL;
	if(flag == INVAILD_PERIOD)	return FLOW_ERROR;
	if(flag == INSERT_BLANK)	return FLOW_NORMAL;

	if(this->IsFrameOver(pOut))	return FLOW_FRAME;
	this->OutputBlock(pIn, pOut);
//	Gdbug.DbugPrintData(LBUF_2BLOCKPRINT_INPUT, pOut);
	this->AddInline(pIn);
	return this->AddOutLine(pOut);
}


//================ class CLbuf =========

CLbuf::CLbuf(void)
{
	this->RegInit();
}

CLbuf::~CLbuf(void)
{
}

void CLbuf::RegInit(void)
{
	CChipBase1::RegInit();

	this->SetReg(REG_LBUF_DSTWIDTH_H,	0x02);
	this->SetReg(REG_LBUF_DSTWIDTH_L,	0x80);
	this->SetReg(REG_LBUF_DSTHEIGHT_H,	0x01);
	this->SetReg(REG_LBUF_DSTHEIGHT_L,	0xe0);

	this->SetReg(REG_LBUF_SRCWIDTH_H,	0x02);
	this->SetReg(REG_LBUF_SRCWIDTH_L,	0x80);
	this->SetReg(REG_LBUF_SRCHEIGHT_H,	0x01);
	this->SetReg(REG_LBUF_SRCHEIGHT_L,	0xe0);
}

void CLbuf::SetReg(int adr, int val)
{
	CChipBase1::SetReg(adr, val);

	this->gZoom.SetReg(adr, val);
	this->gBlock.SetReg(adr, val);
}

int	CLbuf::Process(ImageStream *pIn, ImageBlock *pOut)
{
	Gdbug.DbugPrintData(LBUF_PRINT_INPUT, pIn);

	if(this->gZoom.Process(pIn, &(this->gISbuf)) == FLOW_ERROR)
		return FLOW_ERROR;
	return this->gBlock.Process(&(this->gISbuf), pOut);
}

int	CLbuf::Process(ImageBlock *pIn, ImageStream *pOut)
{
	pOut->IsValid();
	pIn->IsValid();
	return FLOW_FRAME;
}


