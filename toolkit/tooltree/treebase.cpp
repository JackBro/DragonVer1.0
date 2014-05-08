
#include "internal.h"

CLeafBase::CLeafBase()
{
	this->m_level = 0;
}

CLeafBase::~CLeafBase()
{
}

void CLeafBase::SetLevel(int level)
{
	this->m_level = level;
}

int CLeafBase::GetLevel(void)
{
	return this->m_level;
}

void CLeafBase::saveLeafInit(char *str)
{
	int i;

	str[0] = 0;
	for(i = 0; i < this->m_level; i++)
		sprintf(str, "%s\t", str);
}

//==============================================

CKeyLeaf::CKeyLeaf()
{
	this->m_mode = LEAF_INTVALUE;
}

CKeyLeaf::~CKeyLeaf()
{
}

void CKeyLeaf::SetKey(char *key, int val)
{
	strcpy(this->m_key, key);
	this->m_value = val;
	this->m_mode = LEAF_INTVALUE;
}

void CKeyLeaf::SetKey(char *key, char *vstr)
{
	strcpy(this->m_key, key);
	strcpy(this->m_vstr, vstr);
	this->m_mode = LEAF_STRVALUE;
}

int	CKeyLeaf::GetLeafMode(void)
{
	return this->m_mode;
}

char *CKeyLeaf::GetKey(void)
{
	return this->m_key;
}

int	CKeyLeaf::GetKeyVal(void)
{
	return this->m_value;
}

char *CKeyLeaf::GetKeyStr(void)
{
	return this->m_vstr;
}

void CKeyLeaf::saveFoldHeader(CFileBin *fb)
{
	char str[100];

	if( (this->m_mode != LEAF_INTVALUE) && (this->m_mode != LEAF_STRVALUE) )
		return;
	this->saveLeafInit(str);
	switch(this->m_mode)
	{
	case LEAF_INTVALUE:
		sprintf(str, "%s%s(%d)\n", str, this->m_key, this->m_value);
		break;
	case LEAF_STRVALUE:
		sprintf(str, "%s%s(\"%s\")\n", str, this->m_key, this->m_vstr);
		break;
	default:
		break;
	}
	fb->Write(str, (int)strlen(str));
	this->saveLeafInit(str);
	sprintf(str, "%s{\n", str);
	fb->Write(str, (int)strlen(str));
}

void CKeyLeaf::saveFoldTail(CFileBin *fb)
{
	char str[100];

	if(this->m_mode != LEAF_INVALID)
	{
		this->saveLeafInit(str);
		sprintf(str, "%s}\n\n", str);
		fb->Write(str, (int)strlen(str));
	}
}

void CKeyLeaf::saveLeaf(CFileBin *fb)
{
	char str[100];

	if( (this->m_mode != LEAF_INTVALUE) && (this->m_mode != LEAF_STRVALUE) )
		return;
	this->saveLeafInit(str);
	if(this->GetLeafMode() == LEAF_INTVALUE)
		sprintf(str, "%s%s = %d;\n", str, this->GetKey(), this->GetKeyVal());
	else
		sprintf(str, "%s%s = \"%s\";\n", str, this->GetKey(), this->GetKeyStr());
	fb->Write(str, (int)strlen(str));
}

//======================================
CCntLeaf::CCntLeaf(void)
{
}

CCntLeaf::~CCntLeaf(void)
{
}

void CCntLeaf::Connect(int src, int oupin, int dst, int inpin)
{
	this->m_mode = LEAF_CONNECT;
	this->m_dst = dst;
	this->m_src = src;
	this->m_inpin = inpin;
	this->m_outpin = oupin;
}

void CCntLeaf::GetConnect(int *src, int *oupin, int *dst, int *inpin)
{
	*src = this->m_src;
	*dst = this->m_dst;
	*inpin = this->m_inpin;
	*oupin = this->m_outpin;
}

void CCntLeaf::saveLeaf(CFileBin *fb)
{
	char str[100];

	CKeyLeaf::saveLeaf(fb);
	if(this->m_mode != LEAF_CONNECT)
		return;
	this->saveLeafInit(str);
	sprintf(str, "%sU%dO%d - U%dI%d;\n", str, this->m_src, this->m_outpin, 
		this->m_dst, this->m_inpin);
	fb->Write(str, (int)strlen(str));
}


//====================================

CKeyTree::CKeyTree()
{
	this->m_mode = LEAF_INVALID;
}

CKeyTree::~CKeyTree()
{
}

CCntLeaf *CKeyTree::GetLeaf(int index)
{
	return this->m_leaf[index];
}

CCntLeaf *CKeyTree::GetLeaf(char *key)
{
	int i, count;
	CCntLeaf *pf;

	count = this->m_leaf.GetUnitCount();
	for(i = 0; i < count; i++)
	{
		pf = this->m_leaf[i];
		if(strcmp(pf->GetKey(), key) == 0)
			return pf;
	}
	return NULL;
}

CCntLeaf *CKeyTree::AddLeaf(void)
{
	CCntLeaf *pf;

	pf = this->m_leaf.Add();
	pf->SetLevel(this->GetLevel()+1);
	return pf;
}

void CKeyTree::AddLeaf(int src, int oupin, int dst, int inpin)
{
	this->AddLeaf()->Connect(src, oupin, dst, inpin);
}

int	CKeyTree::GetLeafCount(void)
{
	return this->m_leaf.GetUnitCount();
}

void CKeyTree::AddLeaf(char *key, int val)
{
	this->AddLeaf()->SetKey(key, val);
}
	
void CKeyTree::AddLeaf(char *key, char *str)
{
	this->AddLeaf()->SetKey(key, str);
}

//CKeyTree *CKeyTree::GetFold(int index)
//{
//	return this->m_fold[index];
//}

CKeyTree *CKeyTree::GetFold(char *key, int val)
{
	int i, count;
	CKeyTree *pf;

	count = this->m_fold.GetUnitCount();
	for(i = 0; i < count; i++)
	{
		pf = this->m_fold[i];
		if( (strcmp(pf->GetKey(), key) == 0) && (pf->GetKeyVal() == val) )
			return pf;
	}
	return NULL;
}

CKeyTree *CKeyTree::GetFold(char *key, char *str)
{
	int i, count;
	CKeyTree *pf;

	count = this->m_fold.GetUnitCount();
	for(i = 0; i < count; i++)
	{
		pf = this->m_fold[i];
		if( (strcmp(pf->GetKey(), key) == 0) && (strcmp(pf->GetKeyStr(), str) == 0) )
			return pf;
	}
	return NULL;
}

CKeyTree *CKeyTree::AddFold(void)
{
	CKeyTree *pt;

	pt = this->m_fold.Add();
	pt->SetLevel(this->GetLevel()+1);
	return pt;
}

CKeyTree *CKeyTree::AddFold(char *key, int val)
{
	CKeyTree *pt;

	pt = this->AddFold();
	pt->SetKey(key, val);
	return pt;
}

CKeyTree *CKeyTree::AddFold(char *key, char *str)
{
	CKeyTree *pt;

	pt = this->AddFold();
	pt->SetKey(key, str);
	return pt;
}

int	CKeyTree::GetFoldCount(void)
{
	return this->m_fold.GetUnitCount();
}

void CKeyTree::Save(CFileBin *fb)
{
	int count, i;
	CCntLeaf *kl;

	this->saveFoldHeader(fb);
	count = this->m_leaf.GetUnitCount();
	for(i = 0; i < count; i++)
	{
		kl = this->m_leaf[i];
		kl->saveLeaf(fb);
	}
	count = this->m_fold.GetUnitCount();
	for(i = 0; i < count; i++)
		this->m_fold[i]->Save(fb);

	this->saveFoldTail(fb);
}

void CKeyTree::Clear(void)
{
	int i, count;

	this->m_leaf.Clear();
	count = this->m_fold.GetUnitCount();
	for(i = 0; i < count; i++)
		this->m_fold[i]->Clear();
	this->m_fold.Clear();
}
