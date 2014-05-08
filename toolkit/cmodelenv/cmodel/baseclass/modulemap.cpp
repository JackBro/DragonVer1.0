
#include "modulemap.h"

CModuleMap::CModuleMap(int count)
{
	this->SetSize(count);
}

void CModuleMap::SetSize(int size)
{
	CSramBuf::SetSize(size, sizeof(TModuleMap));
	this->Clear();
	CBufferBase::SetSize(size, sizeof(TModuleMap));
}

void CModuleMap::Clear(void)
{
	int i;
	TModuleMap map = { NULL };

	for(i = 0; i < this->m_size; i++)
		this->Write(&map, i);
}

int CModuleMap::Write(TModuleMap *map, int index)
{
	return CSramBuf::Write((char *)map, sizeof(TModuleMap), index * sizeof(TModuleMap));
}

int CModuleMap::Read(TModuleMap *map, int index)
{
	return CSramBuf::Read((char *)map, sizeof(TModuleMap), index * sizeof(TModuleMap));
}

void CModuleMap::DeleteModule(CModuleBase *pmodule)
{
	int i;
	TModuleMap map;

	if(pmodule == NULL)
		return;
	for( i = 0; i < this->m_size; i++ )
	{
		if( this->Read(&map, i) )
		{
			if(map.pmodule == pmodule)
			{
				map.pmodule = NULL;
				this->Write(&map, i);
				return;
			}
		}
	}
}

void CModuleMap::DeleteModule(int index)
{
	TModuleMap map;

	map.pmodule = NULL;
	this->Write(&map, index);
}

