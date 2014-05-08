

#include "sif.h"
#include "./parameter/vregdef.h"


//==================== class CSifModule ===================

CSifModule::CSifModule() : CChipBase(gChipPara[IDD_SIF])
{
	this->InitProcess();
}

CSifModule::~CSifModule()
{
}

void CSifModule::InitModuleIndex(void)
{
}

void CSifModule::OnWriteReg(int adr, int val)
{
	this->SetReg(adr, val);
}

void CSifModule::InitRegProp(void)
{
}

int CSifModule::CanRun(int clk)
{
	clk = 0;
	return 1;
}

void CSifModule::doProcess(TStream *input, TStream *output)
{
	TSize src;
	TPoint start, end;
	int high, low;
	int mode;

	high		= this->GetReg(V558_REG_SIF_COLMAXH);
	low			= this->GetReg(V558_REG_SIF_COLMAXL);
	src.cx	= (unsigned short)((high << 8) + low + 1);
	high		= this->GetReg(V558_REG_SIF_ROWMAXH);
	low			= this->GetReg(V558_REG_SIF_ROWMAXL);
	src.cy	= (unsigned short)((high << 8) + low + 1);

	high	= this->GetReg(V558_REG_SIF_HSYNC_STARTH);
	low		= this->GetReg(V558_REG_SIF_HSYNC_STARTL);
	start.x	= (unsigned short)((high << 8) + low);
	high	= this->GetReg(V558_REG_SIF_HSYNC_STOPH);
	low		= this->GetReg(V558_REG_SIF_HSYNC_STOPL);
	end.x	= (unsigned short)((high << 8) + low);

	high	= this->GetReg(V558_REG_SIF_VSYNC_STARTH);
	low		= this->GetReg(V558_REG_SIF_VSYNC_STARTL);
	start.y	= (unsigned short)((high << 8) + low);
	high	= this->GetReg(V558_REG_SIF_VSYNC_STOPH);
	low		= this->GetReg(V558_REG_SIF_VSYNC_STOPL);
	end.y	= (unsigned short)((high << 8) + low);

	mode = this->GetReg(V558_REG_SIF_SNR_MODE);
	if( (mode & 0x40) == 0)
		CSifAlg::Sizer(input, output, src, start, end);
	if(mode & 0x40)
	{
		CSifAlg::Convert(input, output+1, src, start, end);
	}
}

void CSifModule::SelfDriver(TStream *input, int index)
{
	int mode;

	mode = this->GetReg(V558_REG_SIF_SNR_MODE);
	input->flag = SENSOR_BUSWIDTH;
	if(mode & 0x40)
	{
		mode = this->GetReg(V558_REG_SIF_YUV_FORMAT);
		input->flag = UYVY_TYPE + (mode & 3);
	}

	if( (this->m_inphor == 0) && (this->m_inpver == 0) )
		input[index].flag |= VSYNC_FLAG;
}

void CSifModule::ResetCounter(void)
{
	CSifAlg::ResetCounter();
	this->UpdateRegister();
}

void CSifModule::UpdateRegister(void)
{
	int i;

	for(i = 0; i < 0xff; i++)
		this->UpdateReg(i);
}
