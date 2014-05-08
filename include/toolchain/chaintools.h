#ifndef _CHAIN_TOOLS_H_
#define _CHAIN_TOOLS_H_

#include "tools/global.h"

#define TEMPLATE_CHAIN		template <class T> 
#define CCHAINTOOL			CChainTool <T>

TEMPLATE_CHAIN
class  CChainTool
{
public:
	CChainTool(void);
	virtual ~CChainTool();

public:
	typedef struct tag_TChain {
		T		*pcur;
		struct tag_TChain *pnext; 
	} TChain, *PTChain;

public:
	T* Add(void);
	void DeleteUnit(T* unit);

public:
	T* GetLastUnit(void);
	T* GetUnit(int index);
	T* GetFirstUnit(void);
	T* GetNextUnit(T* unit);
	void Clear(void);
	int GetUnitCount();
	void SetUnitCount(int size);

private:
	int DeleteLastUnit(void);

public:
	T* operator[](int index);

private:
	TChain m_chain;
	int m_count;
};



TEMPLATE_CHAIN
CCHAINTOOL::CChainTool(void)
{
	this->m_count = 0;
	memset(&(this->m_chain), 0, sizeof(TChain));
}

TEMPLATE_CHAIN
CCHAINTOOL::~CChainTool(void)
{
	while(this->DeleteLastUnit());
}

TEMPLATE_CHAIN
T* CCHAINTOOL::GetLastUnit(void)
{
	TChain *ptr = &(this->m_chain);

	while(ptr->pnext != NULL)
		ptr = ptr->pnext;
	return ptr->pcur;
}

TEMPLATE_CHAIN
T* CCHAINTOOL::GetUnit(int index)
{
	int i = 0;
	TChain *ptr = &(this->m_chain);

	while( (i < index) && (ptr != NULL) )
	{
		if(ptr->pcur)
			i++;
		ptr = ptr->pnext;
	}
	
	if(ptr == NULL)
		return NULL;
	if(i != index)
		return NULL;
	if(ptr->pcur == NULL)	//solve ptr = this->m_chain bug
		ptr = ptr->pnext;
	return ptr->pcur;
}

TEMPLATE_CHAIN
T* CCHAINTOOL::operator[](int index)
{
	return this->GetUnit(index);
}

TEMPLATE_CHAIN
int CCHAINTOOL::GetUnitCount()
{
	//int i = 0;
	//TChain *ptr = &(this->m_chain);

	//while(ptr != NULL)
	//{
	//	if(ptr->pcur != NULL)
	//		i++;
	//	ptr = ptr->pnext;
	//}

	//return i;
	return this->m_count;
}

TEMPLATE_CHAIN
void CCHAINTOOL::SetUnitCount(int size)
{
	int i;

	if(this->GetUnitCount() == size)
		return;
	this->Clear();
	for(i = 0; i < size; i++)
		this->Add();
}

TEMPLATE_CHAIN
T* CCHAINTOOL::Add(void)
{
	TChain *ptr = &(this->m_chain);

	this->m_count++;
	if(ptr->pcur == NULL)
	{
		ptr->pcur = new T;
		return ptr->pcur;
	}
	while(ptr->pnext != NULL)
		ptr = ptr->pnext;
	if(ptr->pcur == NULL)
		ptr->pcur = new T;
	else
	{
		ptr->pnext = new TChain;
		ptr = ptr->pnext;
		ptr->pnext = NULL;
		ptr->pcur = new T;
	}
	return ptr->pcur;
}

TEMPLATE_CHAIN 
int CCHAINTOOL::DeleteLastUnit(void)
{
	int flag = 0;
	TChain *ptr = &(this->m_chain), *ptr1 = NULL;

	while(ptr->pnext != NULL)
	{
		ptr1 = ptr;
		ptr = ptr->pnext;
	}
	if(ptr->pcur != NULL)
	{
		delete ptr->pcur;
		this->m_count--;
		ptr->pcur = NULL;
	}
	if( (ptr != NULL) && (ptr != &(this->m_chain)) )
	{
		delete ptr;
		ptr1->pnext = NULL;
		flag = 1;
	}
	return flag;
}

TEMPLATE_CHAIN
void CCHAINTOOL::DeleteUnit(T* unit)
{
	TChain *ptr = &(this->m_chain), *ptr1 = NULL, *ptr2;

	if(ptr->pcur == unit)
	{
		delete ptr->pcur;
		ptr->pcur = NULL;
		this->m_count--;
		return;
	}
	while(ptr->pnext != NULL)
	{
		ptr1 = ptr;
		ptr = ptr->pnext;
		if(ptr->pcur == unit)
		{
			ptr2 = ptr->pnext;
			delete ptr->pcur;
			delete ptr1->pnext;
			ptr1->pnext = ptr2;
			this->m_count--;
			return;
		}
	}
}

TEMPLATE_CHAIN
T* CCHAINTOOL::GetFirstUnit(void)
{
	if(this->m_chain.pcur != NULL)
		return this->m_chain.pcur;
	if(this->m_chain.pnext != NULL)
		return this->m_chain.pnext->pcur;
	return NULL;
}

TEMPLATE_CHAIN
void CCHAINTOOL::Clear(void)
{
	TChain *ptr = &(this->m_chain), *ptr1 = NULL, *ptr2;

	while(ptr->pnext != NULL)
	{
		ptr1 = ptr;
		ptr = ptr->pnext;
		ptr2 = ptr->pnext;
		delete ptr->pcur;
		delete ptr1->pnext;
		ptr1->pnext = ptr2;
		ptr = ptr1;
	}

	if(ptr->pcur)
	{
		delete ptr->pcur;
		ptr->pcur = NULL;
	}
	ptr = NULL;
	this->m_count = 0;
}

TEMPLATE_CHAIN
T* CCHAINTOOL::GetNextUnit(T* unit)
{
	TChain *ptr = &(this->m_chain);

	while(ptr->pnext != NULL)
	{
		if(ptr->pcur == unit)
		{
			ptr = ptr->pnext;
			break;
		}
		ptr = ptr->pnext;
	}
	if(ptr->pcur == unit)
		return NULL;
	return ptr->pcur;
}

#endif


