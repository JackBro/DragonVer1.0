
#include "algbase.h"


CAlgBase::CAlgBase()
{
	this->ResetCounter();
}

void CAlgBase::AddInline(TStream *input, int width)
{
	if( (input->flag & HREF_FLAG) == 0 )
		this->m_inphor++;
	if(this->m_inphor / width)
		this->m_inpver++;
	this->m_inphor %= width;
}

void CAlgBase::AddOutline(TStream *output, int width)
{
	if( (output->flag & HREF_FLAG) == 0 )
		this->m_outhor++;
	if(this->m_outhor / width)
		this->m_outver++;
	this->m_outhor %= width;
}

void CAlgBase::ResetCounter(void)
{
	this->m_inphor = 0;
	this->m_inpver = 0;
	this->m_outhor = 0;
	this->m_outver = 0;
}
