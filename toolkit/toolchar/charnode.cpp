
#include "internal.h"


CCharNode::CCharNode(void)
{
	this->SetNodeProp(CHAR_MISC);
}

CCharNode::~CCharNode(void)
{
	int i, count;
	TCharKey *ptr = NULL;

	count = this->m_node.GetUnitCount();
	for(i = 0; i < count; i++)
	{
		ptr = this->m_node[i];
		delete []ptr->key;
		ptr->key = NULL;
	}
}

void CCharNode::connectNode(int len, char *key, CCharNode *pnode)
{
	TCharKey *ptr = NULL;

	ptr = this->m_node.Add();
	if(ptr == NULL)
		return;
	ptr->len = len;
	ptr->key = new char[ptr->len];
	if(ptr->key == NULL)
	{
		this->m_node.DeleteUnit(ptr);
		return;
	}
	memcpy(ptr->key, key, len);
	ptr->pnode = pnode;
}

int CCharNode::VerfiyKey(int len, char *key)
{
	int i, count;
	TCharKey *ptr = NULL;

	count = this->m_node.GetUnitCount();
	for(i = 0; i < count; i++)
	{
		ptr = this->m_node[i];
		if(this->VerifyKey(ptr->key, key, ptr->len, len))
			return 1;
	}
	return 0;
}

void CCharNode::Connect(int len, char *key, CCharNode *pnode)
{
	if(len == 0)
		return;

	if(this->VerfiyKey(len, key))
		return;
	this->connectNode(len, key, pnode);
}

void CCharNode::ConnectH7(CCharNode *pnode)
{
	int i;
	char key[128];

	for(i = 0; i < 128; i++)
		key[i] = (char)(i | 0x80);

	this->Connect(128, key, pnode);
}

void CCharNode::ConnectAny(CCharNode *pnode)
{
	int i;
	char key[256];

	for(i = 0; i < 256; i++)
		key[i] = (char)i;
	this->Connect(256, key, pnode);
}

void CCharNode::ConnectOther(void)
{
	CCharNode *ptr;
	int i;
	char key;

	for(i = 0; i < 256; i++)
	{
		key = (char)i;
		if(this->VerfiyKey(1, &key))
			continue;
		ptr = this->m_misc.Add();
		ptr->SetNodeProp(i);
		this->Connect(1, &key, ptr);
	}
}

CCharNode *CCharNode::GetNode(char key)
{
	int i, count;
	TCharKey *ptr = NULL;

	count = this->m_node.GetUnitCount();
	for(i = 0; i < count; i++)
	{
		ptr = this->m_node[i];
		if(this->VerifyKey(ptr->key, ptr->len, key))
			return ptr->pnode;
	}
	return NULL;
}

int	CCharNode::VerifyKey(char *src, int len, char key)
{
	int i;

	for(i = 0; i < len; i++)
	{
		if(src[i] == key)
			return 1;
	}
	return 0;
}

int	CCharNode::VerifyKey(char *src, char *dst, int len1, int len2)
{
	int i;

	for(i = 0; i < len2; i++)
	{
		if(this->VerifyKey(src, len1, dst[i]))
			return 1;
	}
	return 0;
}


