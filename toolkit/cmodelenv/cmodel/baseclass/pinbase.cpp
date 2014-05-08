

#include "pinbase.h"

//==================== class CPinBase ===================

CPinBase::CPinBase(int delay) : CChannelBuf(delay, sizeof(TStream))
{
	//this->m_src = NULL;
	//this->m_dst = NULL;
	this->m_fileend = 0;
	this->Clear();
}

CPinBase::~CPinBase()
{
}

void CPinBase::Clear(void)
{
	TStream stream;
	int i;

	memset(&stream, 0, sizeof(TStream));
	for(i = 0; i < this->m_size; i++)
		this->Write(&stream);
}

int	CPinBase::Write(TStream *stream)
{
	this->WriteDbugFile(stream);
	return CChannelBuf::Write( (char *)stream, sizeof(TStream) );
}

int CPinBase::Read(TStream *stream)
{
	return CChannelBuf::Read( (char *)stream, sizeof(TStream) );
}

void CPinBase::SetDelayCount(int delay)
{
	CChannelBuf::SetSize(delay, sizeof(TStream));
	this->Clear();
}

const int gUnitSize[] =
{
	1, 1, 2, 2, 4,	//SIN8_TYPE	--	SINT32_TYPE
	4, 1, 2, 3, 3,	//UINT32_TYPE	--	YUV444_TYPE
	2, 2, 2, 2, 2	//YUV422_TYPE	--	YVYU_TYPE
};

int CPinBase::ReadInputFile(TStream *stream)
{
	int pos = stream->flag & STREAM_TYPE;

	if(pos < TOTAL_STREAM_TYPE)
	{
		return this->m_input.Read( &(stream->val8), gUnitSize[pos]);
	}
	return 0;
}


int	CPinBase::SelfDriver(TStream *stream)
{
	int len = 0;

	if(this->m_fileend == 0)
	{
		len = this->ReadInputFile(stream);
		if(len)
			stream->flag |= HREF_FLAG;
		else
			this->m_fileend = 1;
	}
	this->Write(stream);
	return len;
}

void CPinBase::WriteDbugFile(TStream *stream)
{
	int pos;

	if( (stream->flag & HREF_FLAG) == 0 )	//invalid stream
		return;
	
	pos = stream->flag & STREAM_TYPE;
	if(pos < TOTAL_STREAM_TYPE)
		this->m_dbug.Write(&(stream->val8), gUnitSize[pos]);
}

void CPinBase::Reset(void)
{
	CChannelBuf::ClearPoint();
	this->m_input.Fseek(0, SEEK_SET);
	this->m_fileend = 0;
}

void CPinBase::SetPinId(int id)
{
	this->m_id = id;
}

