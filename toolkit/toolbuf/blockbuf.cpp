
#include "internal.h"


CBlockBuf::CBlockBuf(int block, int blocksize) : CSramBuf(block * blocksize)
{
	this->m_blocksize = blocksize;
}

void CBlockBuf::SetSize(int block, int blocksize)
{
	CSramBuf::SetSize(block * blocksize);
	this->m_blocksize = blocksize;
}

void CBlockBuf::SetSize(char *buf, int block, int blocksize)
{
	CSramBuf::SetSize(buf, block * blocksize);
	this->m_blocksize = blocksize;
}

int	CBlockBuf::Write(char *buf, int size, int start, int end)
{
	start -= start % this->m_blocksize;
	size -= size % this->m_blocksize;
	return CSramBuf::Write(buf, size, start, end);
}

int	CBlockBuf::Read(char *buf, int size, int start, int end)
{
	start -= start % this->m_blocksize;
	size -= size % this->m_blocksize;
	return CSramBuf::Read(buf, size, start, end);
}
