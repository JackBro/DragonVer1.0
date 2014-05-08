

#include "dbugPrint.h"

#include "busif.h"


CBusIF::CBusIF(void)
{
	this->gpSif = NULL;
	this->gpIsp = NULL;
	this->gpIpp = NULL;
	this->gpJbuf = NULL;
	this->gpJpeg = NULL;
	this->gpLbuf = NULL;
	this->gpLcdIF = NULL;

	this->RegInit();
}

CBusIF::~CBusIF(void)
{
}

void CBusIF::RegInit(void)
{
	CChipBase1::RegInit();

	this->SetReg(REG_BIU_OP_MODE,	0x18);
	this->SetReg(REG_BIU_SEL_PORT,	0x1);
}

int CBusIF::NextModule(CChipBase1 * pSrc, CChipBase1 ** pDst)
{
	int count = 0;
	int mode = this->GetReg(REG_BIU_OP_MODE);

	if(pSrc == NULL)
		return 0;

	if(pSrc == this->gpSif)
	{
		switch(mode & 7)
		{
		case MODE_VIEW:
		case MODE_CAP_VIDEO:
			count = 1;
			*(pDst+0) = this->gpIsp;
			break;

		case MODE_DISPLAY:
		case MODE_ENCODER:
		case MODE_DECODER:
		case MODE_DIRECTDISPLAY:
		case MODE_THROUGH:
		default:
			break;
		}
	}
	if(pSrc == this->gpIsp)
	{
		switch(mode & 7)
		{
		case MODE_VIEW:
		case MODE_CAP_VIDEO:
			count = 1;
			pDst[0] = this->gpIpp;
			break;

		case MODE_DISPLAY:
		case MODE_ENCODER:
		case MODE_DECODER:
		case MODE_DIRECTDISPLAY:
		case MODE_THROUGH:
			break;
		default:
			break;
		}
	}
	if(pSrc == this->gpIpp)
	{
		switch(mode & 7)
		{
		case MODE_VIEW:
		case MODE_CAP_VIDEO:
			count = 2;
			pDst[0] = this->gpLcdIF;
			pDst[1] = this->gpLbuf;
			break;

		case MODE_DISPLAY:
			count = 1;
			pDst[0] = this->gpLcdIF;
			break;
		case MODE_ENCODER:
		case MODE_DECODER:

		case MODE_DIRECTDISPLAY:
		case MODE_THROUGH:
			break;
		default:
			break;
		}
	}
	if(pSrc == this->gpLbuf)
	{
		switch(mode & 7)
		{
		case MODE_VIEW:
		case MODE_CAP_VIDEO:
			count = 1;
			pDst[0] = this->gpJpeg;
			break;

		case MODE_DISPLAY:
		case MODE_DECODER:
			count = 1;
			pDst[0] = this->gpIpp;
			break;

		case MODE_ENCODER:
		case MODE_DIRECTDISPLAY:
		case MODE_THROUGH:
			break;
		default:
			break;
		}
	}
	if(pSrc == this->gpJpeg)
	{
		switch(mode & 7)
		{
		case MODE_VIEW:
			break;

		case MODE_CAP_VIDEO:
		case MODE_ENCODER:
			count = 1;
			pDst[0] = this->gpJbuf;
			break;

		case MODE_DISPLAY:
		case MODE_DECODER:
			count = 1;
			pDst[0] = this->gpLbuf;
			break;

		case MODE_DIRECTDISPLAY:
		case MODE_THROUGH:
			break;
		default:
			break;
		}
	}
	return count;
}




