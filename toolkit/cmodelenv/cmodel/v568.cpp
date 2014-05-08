

#include "v568.h"


//==================== class CV568Chip ===================


CV568Chip::CV568Chip() : CChipBase(gChipPara[IDD_V568])
{
	this->InitProcess();

	this->SetClkModule(&(this->m_pll));
//	this->m_pll.SetMainChip(this);
}

CV568Chip::~CV568Chip()
{
}

void CV568Chip::InitModuleIndex(void)
{
	int i;
	CChipBase *map[V568_TOTAL_MODULES];

	map[1]	=	&(this->m_jpeg);
	map[2]	=	&(this->m_lcdif);
	map[3]	=	&(this->m_sif);
	map[4]	=	&(this->m_lbuf);
	map[5]	=	&(this->m_ge);

	map[6]	=	&(this->m_cpm);
	map[7]	=	&(this->m_hostif);
	map[8]	=	&(this->m_ipp);
	map[9]	=	&(this->m_isp);
	map[10]	=	&(this->m_audif);

	map[11]	=	&(this->m_lcdc);
	map[12]	=	&(this->m_marb);
	map[13]	=	&(this->m_pll);
	map[14]	=	&(this->m_sto);
	map[15]	=	&(this->m_usbc);

	map[16]	=	&(this->m_sensor);
	map[17]	=	&(this->m_panel);
	map[18]	=	&(this->m_sdcard);
	map[19]	=	&(this->m_audadc);
	
	for(i = 1; i < V568_TOTAL_MODULES; i++)
		this->SetModuleIndex(map[i], i);
}


void CV568Chip::OnWriteReg(int adr, int val)
{
	this->SetReg(adr, val);
}

void CV568Chip::InitRegProp(void)
{
}

int CV568Chip::CanRun(int clk)
{
	clk = 0;
	return 1;
}

void CV568Chip::doProcess(TStream *input, TStream *output)
{
	input = output;
}


void CV568Chip::SelfDriver(TStream *input, int index)
{
	input = input;
	index = 0;
}


void CV568Chip::ResetCounter(void)
{
}


