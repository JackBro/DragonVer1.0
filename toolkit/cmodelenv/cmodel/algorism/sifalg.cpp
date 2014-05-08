
#include "sifalg.h"


CSifAlg::CSifAlg()
{
	this->ResetCounter();
}

void CSifAlg::Sizer(TStream *input, TStream *output, TSize src, TPoint start, TPoint end)
{
	output->flag = (input->flag & VSYNC_FLAG) | (input->flag & STREAM_TYPE);
	if( (input->flag & HREF_FLAG) == 0 )
		return;

	*output = *input;
	if( (this->m_inphor < start.x) || (this->m_inphor > end.x) )
		output->flag &= ~HREF_FLAG;
	if( (this->m_inpver < start.y) || (this->m_inpver > end.y) )
		output->flag &= ~HREF_FLAG;
	CAlgBase::AddInline(input, src.cx);
	CAlgBase::AddOutline(output, end.x-end.y+1);
}

void CSifAlg::Convert(TStream *input, TStream *output, TSize src, TPoint start, TPoint end)
{
//	int type = input->flag & STREAM_TYPE;

	input->flag &= ~STREAM_TYPE;
	input->flag |= TYUV422_TYPE;
	if( (input->flag & HREF_FLAG) == 0 )
		return;

	*output = *input;
	if( (this->m_inphor < start.x) || (this->m_inphor > end.x) )
		output->flag &= ~HREF_FLAG;
	if( (this->m_inpver < start.y) || (this->m_inpver > end.y) )
		output->flag &= ~HREF_FLAG;
	CAlgBase::AddInline(input, src.cx);
	CAlgBase::AddOutline(output, end.x-end.y+1);
	//switch(type)
	//{
	//case UYVY_TYPE:
	//case VYUY_TYPE:
	//case YUYV_TYPE:
	//case YVYU_TYPE:
	//default:
	//	break;
	//}
}

