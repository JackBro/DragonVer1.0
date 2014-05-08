
#include "internal.h"


CMemPool::CMemPool(int size, int unit) : CBufferBase(size, unit)
{
	CMemPool::SetSize(size, unit);
}

CMemPool::~CMemPool()
{
}

void CMemPool::Init(void)
{
	int size;
	TChunk *ptr;

	size = this->GetTotalSize();
	if(size < (sizeof(TChunk) << 1))
		return;
	ptr = (TChunk *)(this->m_buf+size-sizeof(TChunk));
	ptr->size = 0;
	ptr->nextpos = 0;
	ptr = (TChunk *)this->m_buf;
	ptr->size = 0;
	ptr->nextpos = size - sizeof(TChunk);
}

void CMemPool::SetSize(int size, int unit)
{
	CBufferBase::SetSize(size, unit);
	this->Init();
}

void CMemPool::SetSize(char *buf, int size, int unit)
{
	CBufferBase::SetSize(buf, size, unit);
	this->Init();
}

void *CMemPool::Malloc(int size)
{
	TChunk *p1, *p2;
	int gap, pos, pos1 = 0, pos2 = 0, flag = 1;

	if(size <= 0)
		return NULL;
	if(size % sizeof(int))
		size += sizeof(int) - size % sizeof(int);

	gap = this->GetTotalSize();
	if(gap < (sizeof(TChunk) << 1))
		return NULL;
	p1 = (TChunk *)this->m_buf;
	while(flag)
	{
		if(p1->size == 0)	//unused memory
		{
			if(p1->nextpos == 0)	//the last chunk
				break;
			pos = pos2 + p1->nextpos;
			p2 = (TChunk *)(this->m_buf + pos);
			while( (p2->size == 0) && (p2->nextpos != 0) )	//combine unused memory
			{
				p1->nextpos += p2->nextpos;
				pos += p2->nextpos;
				p2 = (TChunk *)(this->m_buf + pos);
			}
			if(size == (int)(p1->nextpos - sizeof(TChunk)))
			{
				p1->size = size;
				return this->m_buf + pos2 + sizeof(TChunk);
			}
			if( (gap > p1->nextpos) && 
				(p1->nextpos > (int)(size + sizeof(TChunk))) )
			{
				gap = p1->nextpos;
				pos1 = pos2;
			}
		}
		do {
			pos2 += p1->nextpos;
			p1 = (TChunk *)(this->m_buf + pos2);
		} while(p1->size);
	}

	p1 = (TChunk *)(this->m_buf + pos1);
	if( p1->nextpos < (int)(size + (sizeof(TChunk) << 1)) )
	{
		size = p1->nextpos - sizeof(TChunk);
		p1->size = size;
		return p1;
	}
	p1->size = size;
	pos2 = p1->nextpos;
	p1->nextpos = p1->size + sizeof(TChunk);
	p1 = (TChunk *)(this->m_buf + pos1 + p1->nextpos);
	p1->size = 0;
	p1->nextpos = pos2 - (size + sizeof(TChunk));

	return this->m_buf + pos1 + sizeof(TChunk);
}

void CMemPool::Free(void *ptr)
{
	TChunk *p1;

	if(this->GetTotalSize() < (sizeof(TChunk) << 1))
		return;
	p1 = (TChunk *)( (char *)ptr - sizeof(TChunk) );
	p1->size = 0;
}

