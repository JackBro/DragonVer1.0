
#include "internal.h"


CEmuHd::CEmuHd(int c_num, int h_num, int s_num, int s_size)
{
	CEmuHdBase::CEmuHdBase(c_num, h_num, s_num, s_size);
	this->Init();
}

CEmuHd::~CEmuHd(void)
{
}

void CEmuHd::Init(void)
{
	memset(this->m_path, 0, HD_PATH_MAX_LENGTH);
}

void CEmuHd::SetSize(int c_num, int h_num, int s_num, int s_size)
{
	CEmuHdBase::SetSize(c_num, h_num, s_num, s_size);
}

void CEmuHd::GetInfo(PTHdInfo pinfo)
{
	if(pinfo == NULL)
		return;
	memcpy(pinfo, &(this->m_info), sizeof(THdInfo));
}

void CEmuHd::Open(char *src)
{
	int size = 0;
	CFileBin fb;

	memset(this->m_buf, 0, this->m_totalsize);
	if(src == NULL)
		return;
	strcpy(this->m_path, src);
	fb.Open(this->m_path, "rb");
	size = fb.GetFileLength();
	size = (size > this->m_totalsize) ? this->m_totalsize : size;
	fb.Read(this->m_buf, size);
	fb.Close();
}

void CEmuHd::Close(void)
{
	CFileBin fb;

	if(this->m_path != NULL)
	{
		fb.Open(this->m_path, "wb");
		fb.Write(this->m_buf, this->m_totalsize);
		fb.Close();
	}
}

int CEmuHd::Read(char *buf, int c_idx, int h_idx, int s_idx, int s_cnt)
{
	return CEmuHdBase::Read(buf, c_idx, h_idx, s_idx, s_cnt);
}

int CEmuHd::Write(char *buf, int c_idx, int h_idx, int s_idx, int s_cnt)
{
	return CEmuHdBase::Write(buf, c_idx, h_idx, s_idx, s_cnt);
}

int CEmuHd::ReadEx(char *buf, int s_idx, int s_cnt)
{
	int cylinder = 0, head = 0, sector = 0;

	sector = s_idx % this->m_info.s_num;
	head = s_idx / this->m_info.s_num;
	cylinder = head / this->m_info.h_num;
	head %= this->m_info.h_num ;
	return CEmuHdBase::Read(buf, cylinder, head, sector, s_cnt);
}

int CEmuHd::WriteEx(char *buf, int s_idx, int s_cnt)
{
	int cylinder = 0, head = 0, sector = 0;

	sector = s_idx % this->m_info.s_num;
	head = s_idx / this->m_info.s_num;
	cylinder = head / this->m_info.h_num;
	head %= this->m_info.h_num ;
	return CEmuHdBase::Write(buf, cylinder, head, sector, s_cnt);
}
