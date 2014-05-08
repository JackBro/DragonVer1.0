
#include "internal.h"


CEmuHdBase::CEmuHdBase(int c_num, int h_num, int s_num, int s_size)
{
	CBlockBuf::CBlockBuf((c_num * h_num * s_num), s_size);
	this->m_info.c_num  = c_num;
	this->m_info.h_num  = h_num;
	this->m_info.s_num  = s_num;
	this->m_info.s_size = s_size;
}

CEmuHdBase::~CEmuHdBase(void)
{
}

void CEmuHdBase::SetSize(int c_num, int h_num, int s_num, int s_size)
{
	CBlockBuf::SetSize((c_num * h_num * s_num), s_size);
	this->m_info.c_num  = c_num;
	this->m_info.h_num  = h_num;
	this->m_info.s_num  = s_num;
	this->m_info.s_size = s_size;
}

void CEmuHdBase::SetSize(char *buf, int c_num, int h_num, int s_num, int s_size)
{
	CBlockBuf::SetSize(buf, (c_num * h_num * s_num), s_size);
	this->m_info.c_num  = c_num;
	this->m_info.h_num  = h_num;
	this->m_info.s_num  = s_num;
	this->m_info.s_size = s_size;
}

int CEmuHdBase::Read(char *buf, int c_idx, int h_idx, int s_idx, int s_cnt)
{
	int count = s_cnt, total = 0, offset = 0, size = 0;

	if( (buf == NULL) || (s_cnt == 0) )
		return 0;

	offset = (c_idx * this->m_info.h_num + h_idx) * this->m_info.s_num + s_idx;
	total = this->m_info.c_num * this->m_info.h_num * this->m_info.s_num;
	if(offset + count > total)
		count = total - offset;
	offset *= this->m_info.s_size;
	size = count * this->m_info.s_size;
	return CBlockBuf::Read(buf, size, offset, (offset + size));
}

int CEmuHdBase::Write(char *buf, int c_idx, int h_idx, int s_idx, int s_cnt)
{
	int count = s_cnt, total = 0, offset = 0, size = 0;

	if( (buf == NULL) || (s_cnt == 0) )
		return 0;

	offset = (c_idx * this->m_info.h_num + h_idx) * this->m_info.s_num + s_idx;
	total = this->m_info.c_num * this->m_info.h_num * this->m_info.s_num;
	if(offset + count > total)
		count = total - offset;
	offset *= this->m_info.s_size;
	size = count * this->m_info.s_size;
	return CBlockBuf::Write(buf, size, offset, (offset + size));
}
