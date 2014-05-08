
#include "pinmap.h"

CPinMap::CPinMap(int count)
{
	this->SetSize(count);
}

void CPinMap::SetSize(int size)
{
	CSramBuf::SetSize(size, sizeof(TPinMap));
	this->Clear();
}

void CPinMap::Clear(void)
{
	int i;
	TPinMap map = { NULL };

	for(i = 0; i < this->m_size; i++)
		this->Write(&map, i);
}

int CPinMap::Write(TPinMap *map, int index)
{
	return CSramBuf::Write((char *)map, sizeof(TPinMap), index * sizeof(TPinMap));
}

int CPinMap::Read(TPinMap *map, int index)
{
	return CSramBuf::Read((char *)map, sizeof(TPinMap), index * sizeof(TPinMap));
}

void CPinMap::DeleteModule(CPinBase *ppin)
{
	int i;
	TPinMap map;

	if(ppin == NULL)
		return;
	for( i = 0; i < this->m_size; i++ )
	{
		if( this->Read(&map, i) )
		{
			if(map.pin == ppin)
			{
				map.pin = NULL;
				this->Write(&map, i);
				return;
			}
		}
	}
}

void CPinMap::DeleteModule(int index)
{
	TPinMap map;

	map.pin = NULL;
	this->Write(&map, index);
}

void CPinMap::Reset(void)
{
	int i;
	TPinMap map;

	for( i = 0; i < this->m_size; i++ )
	{
		if( this->Read(&map, i) )
		{
			if(map.pin)
				map.pin->Reset();
		}
	}
}

