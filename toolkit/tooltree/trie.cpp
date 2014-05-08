
#include "internal.h"

CTrie::CTrie()
{
	Root = new TrieNode;
	Root->kind = BRANCH;
	Root->first = this->ConstructNode("", 0, "");
	Root->next = NULL;
}

CTrie::~CTrie()
{
	this->Release(Root);
}

void CTrie::Release(TrieNode * node)
{
	if (node == NULL)
		return;

	TrieNode * p1, *p2 = NULL;
	p1 = node->next;
	node->next = NULL;

	if (node->kind == BRANCH)
	{
		node->first = NULL;
		p2 = node->first;
	}	
	
	delete node;
	
	this->Release(p1);
	if (node->kind == BRANCH)
           this->Release(p2);
}

void CTrie::Insert(const char *word, const char *leafvalue)
{
	unsigned int i = 0;
	size_t len = strlen(word);
	TrieNode *tmpNode = Root->first, *currentRoot, *p;
	
	while (i < len)
	{
		currentRoot = tmpNode;
		tmpNode = MatchPrefix(word[i], tmpNode);
		if (NULL == tmpNode)
		{
			tmpNode = currentRoot;
			break;
		}
		++i;
		if ( tmpNode->kind == LEAF )
			break;
	}

	if (len == i)
		return;
	
	p = this->ConstructNode(word, i, leafvalue);
	p->next = tmpNode->next;
	tmpNode->next = p;
}

char * CTrie::MatchWord(char * word)
{
	TrieNode * pSearchNode = Root->first;
	unsigned int i = 0;
	
	while(i < strlen(word))
	{
		pSearchNode = MatchPrefix(word[i], pSearchNode);
		
		if (!pSearchNode)
			return NULL;
		
		if ( pSearchNode->kind == LEAF )
			break;
		i++;			
	}

	if( (pSearchNode->kind == LEAF) && (i == strlen(word) - 1) )	
		return pSearchNode->word;
	else
		return NULL;
}

TrieNode *CTrie::MatchPrefix(char c, TrieNode * tmpNode)
{
	while (tmpNode != NULL)
	{
		if (tmpNode->symbol == c)
		{
			if (tmpNode->kind == LEAF)
				return tmpNode;	
			else
				return tmpNode->first;
		}
		else tmpNode = tmpNode->next;
	}

	return NULL;
}

TrieNode * CTrie::ConstructNode(const char *word, unsigned int i, const char *leafValue)
{
	ASSERT( MAX_WORD_LENGTH >= strlen(leafValue));
	TrieNode *p = new TrieNode;
	
	if ( i >= strlen(word) )
	{
		p->kind =LEAF;
		p->symbol = '$';
		p->next = NULL;
		sprintf(p->word, "%s", leafValue);
	}

	if (i < strlen(word))
	{
		p->kind = BRANCH;
		p->symbol = word[i];
		p->next = NULL;
		p->first = this->ConstructNode(word, ++i, leafValue);
	}

	return p;
}

TrieNode * CTrie::GetRoot()
{
	return this->Root;
}
