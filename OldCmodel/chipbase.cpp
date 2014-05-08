

#include "chipbase.h"
#include "dbugPrint.h"





//==================== class CChipBase1 ===================

CChipBase1::CChipBase1(void)
{
	this->gModify = 1;
	this->RegInit();
}

CChipBase1::~CChipBase1(void)
{
}

void CChipBase1::RegInit(void)
{
	int i;

	for(i = 0; i < 256; i++)
		this->SetReg(i, 0);
	this->ClearCounter();
}

void CChipBase1::ClearCounter(void)
{
	this->gAllow = FRAME_STOP;
	this->gInLine	= 0;
	this->gOutLine	= 0;
}

void CChipBase1::SetReg(int adr, int val)
{
	this->gModify = 1;
	this->gReg[adr] = (UINT8)val;
}

int CChipBase1::GetReg(int adr)
{
	return this->gReg[adr];
}

void CChipBase1::SetInterReg(int adr, int val)
{
	this->gInterReg[adr] = (UINT8)val;
}

int CChipBase1::GetInterReg(int adr)
{
	return this->gInterReg[adr];
}

int CChipBase1::varify(ImageStream *pIn)
{
	if(pIn->width % 2)
	{
		Gdbug.DbugMsg("Image width must be even\n");
		return FAILED;
	}
	return SUCCEED;
}

void CChipBase1::VsyncInitInfo(void)
{
	this->ClearCounter();
	this->gAllow = FRAME_START;		//frame start
	if(this->gModify)
	{
		memcpy(this->gInterReg, this->gReg, 256);		//fill inter reg
		this->gModify = 0;
	}
}

int CChipBase1::VsyncInit(ImageStream *pIn)
{
	if( !(pIn->vs) )
		return FALSE;
	this->VsyncInitInfo();
	return TRUE;
}

void CChipBase1::AddInline(ImageStream *pIn)
{
	if( (pIn->width != 0) || (this->gInLine) )	//VSYNC & back porch Vblank
		this->gInLine++;
}

int CChipBase1::AddOutLine(ImageStream *pOut)
{
	if(pOut->width != 0)
	{
		this->gOutLine++;
		if(this->gOutLine >= pOut->height)
		{
			this->gAllow = FRAME_STOP;		
			return FLOW_FRAME;
		}
	}

	return FLOW_NORMAL;
}

int CChipBase1::IsFrameOver(ImageStream *pOut)
{
	if(this->gAllow == FRAME_START)
		return FALSE;

	pOut->width = 0;
	return TRUE;
}

int CChipBase1::FillImageInfo(ImageStream *pIn, ImageStream *pOut)
{
	pOut->format = pIn->format;
	pOut->vs = pIn->vs;

	if(this->gAllow == FRAME_STOP)		//frame not start
	{
		pOut->width = 0;
		return VEND_PERIOD;		//last frame's back porch period
	}
	if(pIn->width == 0)
	{
		pOut->width = 0;
		if( (this->gInLine < pIn->height) && (this->gInLine > 0) )		//output frame not over
			return INSERT_BLANK;
		else if(this->gInLine > 0)
		{
			return VEND_PERIOD;
		}
		return VSTART_PERIOD;
	}
	pOut->width = pIn->width;
	pOut->height = pIn->height;

	return VSYNC_PERIOD;
}

int CChipBase1::varify(ImageBlock *pIn)
{
	pIn->IsValid();
	return SUCCEED;
}

int CChipBase1::VsyncInit(ImageBlock *pIn)
{
	if( !(pIn->vs) )
		return FALSE;
	this->VsyncInitInfo();
	return TRUE;
}

void CChipBase1::AddInline(ImageBlock *pIn)
{
	if( (pIn->width != 0) || (this->gInLine) )	//VSYNC & back porch Vblank
		this->gInLine++;
}

int CChipBase1::AddOutLine(ImageBlock *pOut)
{
	if(pOut->width != 0)
	{
		this->gOutLine ++;
		if(this->gOutLine >= pOut->height)
		{
			this->gAllow = FRAME_STOP;		
			return FLOW_FRAME;
		}
	}

	return FLOW_NORMAL;
}

int CChipBase1::IsFrameOver(ImageBlock *pOut)
{
	if(this->gAllow == FRAME_START)
		return FALSE;

	pOut->width = 0;
	return TRUE;
}

int CChipBase1::FillImageInfo(ImageStream *pIn, ImageBlock *pOut)
{
	switch(pIn->format)
	{
	case YUV422:
		pOut->format = YUV422BLK;
		break;
	case YUV411:
		pOut->format = YUV411BLK;
		break;
	case YUV420:
		pOut->format = YUV420BLK;
		break;
	case YUV400:
		pOut->format = YUV400BLK;
		break;
	default:
		return INVAILD_PERIOD;
	}
	pOut->vs = pIn->vs;

	if(this->gAllow == FRAME_STOP)		//frame not start
	{
		pOut->width = 0;
		return VEND_PERIOD;		//last frame's back porch period
	}
	if(pIn->width == 0)
	{
		pOut->width = 0;
		if( (this->gInLine < pIn->height) && (this->gInLine > 0) )		//output frame not over
			return INSERT_BLANK;
		else if(this->gInLine > 0)
		{
			return VEND_PERIOD;
		}
		return VSTART_PERIOD;
	}

	return VSYNC_PERIOD;
}

int CChipBase1::FillImageInfo(ImageBlock *pIn, ImageBlock *pOut)
{
	pOut->format = pIn->format;
	pOut->vs = pIn->vs;
	pOut->height = pIn->height;
	pOut->width = 0;

	if(this->gAllow == FRAME_STOP)		//frame not start
	{
		pOut->width = 0;
		return VEND_PERIOD;		//last frame's back porch period
	}
	if(pIn->width == 0)
	{
		pOut->width = 0;
		if( (this->gInLine < pIn->height) && (this->gInLine > 0) )		//output frame not over
			return INSERT_BLANK;
		else if(this->gInLine > 0)
		{
			return VEND_PERIOD;
		}
		return VSTART_PERIOD;
	}

	return VSYNC_PERIOD;
}

void CChipBase1::AdjustISPoint(ImageStream **pIs, int count)
{
	ImageStream *p;
	int i;

	p = pIs[0];
	for(i = 0; i < count-1; i++)
		pIs[i] = pIs[i+1];
	pIs[count-1] = p;
}

void CChipBase1::CopyImageStream(ImageStream *pIn, ImageStream *pOut)
{
	int size = pOut->width * sizeof(short int);

	if(pOut->format == RAWRGB)
		size *= 3;
	if(pOut->format == YUV444)
		size *= 2;
	memcpy(pOut->buf16, pIn->buf16, size);
}

int	CChipBase1::Process(ImageStream *pIn, ImageStream *pOut)
{
	this->CopyImageStream(pIn, pOut);
	this->AddInline(pIn);
	return this->AddOutLine(pOut);
}

void CChipBase1::yuv422ToBlock(ImageStream *pIn, ImageBlock *pOut)
{
	int i, j, k, offset, adr;

	for(i = 0, offset = 0; i < 8; i++, offset += 8)
	{
		adr = offset;
		for(j = 0; j < pOut->width; j += 16)
		{
			for(k = 0; k < 8; k++)
				pOut->buf8[adr+k] = pIn[i].yuvBuf[j+k].y;		//first Y block
			adr += 64;
			for(k = 0; k < 8; k++)
				pOut->buf8[adr+k] = pIn[i].yuvBuf[j+8+k].y;		//Second Y block
			adr += 64;
			for(k = 0; k < 8; k++)
				pOut->buf8[adr+k] = pIn[i].yuvBuf[j+(k<<1)].uv;		//U block
			adr += 64;
			for(k = 0; k < 8; k++)
				pOut->buf8[adr+k] = pIn[i].yuvBuf[j+(k<<1)+1].uv;		//V block
			adr += 64;
		}
	}
}

//=============== class CYuv422Base =================

int	 CYuv422Base::varify(ImageStream *pIn)
{
	if(pIn->format != YUV422)
	{
		Gdbug.DbugMsg("Input stream not yuv422 format!\n");
		return FAILED;
	}
	return CChipBase1::varify(pIn);
}




