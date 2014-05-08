
#include "internal.h"

CFilter444::CFilter444(int width, int unit)
{
	this->SetSize(width, unit);
}

CFilter444::~CFilter444(void)
{
}

CFilterRgb::CFilterRgb(int width, int unit)
{
	this->SetSize(width, unit);
}

CFilterRgb::~CFilterRgb(void)
{
}

CFilter411::CFilter411(int width, int unit)
{
	this->SetSize(width, unit);
}

CFilter411::~CFilter411(void)
{
}

void CFilter411::SetFormat(int format)
{
	int i;

	for(i = 0; i < 7; i++)
		this->m_Linebuf[i].SetFormat(format);
	this->Clear();
}

CFilter422::CFilter422(int width, int unit)
{
	this->SetSize(width, unit);
}

CFilter422::~CFilter422(void)
{
}

void CFilter422::SetFormat(int format)
{
	int i;

	for(i = 0; i < 7; i++)
		this->m_Linebuf[i].SetFormat(format);
	this->Clear();
}

