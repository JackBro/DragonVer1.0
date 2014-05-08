
#include "internal.h"



CLinkMap::CLinkMap()
{
	m_firsthnode = NULL;
	m_lasthnode = NULL;
	m_curNodeID = 0;
}

CLinkMap::~CLinkMap()
{
}

THeadNode* CLinkMap::FindNode(char *nodename)
{
	THeadNode *hnode;

	hnode = m_firsthnode;
	while (hnode)
	{
		if (strcmp(hnode->nodename, nodename) == 0)
			return hnode;
		hnode = hnode->nexthnode;
	}
	return NULL;
}

THeadNode* CLinkMap::FindNode(UINT32 adr)
{
	THeadNode *hnode;

	hnode = m_firsthnode;
	while (hnode)
	{
		if ( (UINT32)(hnode->nodeEntity) == adr)
			return hnode;
		hnode = hnode->nexthnode;
	}
	return NULL;
}

int CLinkMap::GenNodeID(void)
{
	m_curNodeID++;
	return m_curNodeID;
}

//int CLinkMap::AddNode(char *nodename)
//{
//	return AddNode(nodename, (void *) NULL);
//}


int CLinkMap::AddNode(char *nodename, void *nodeentity)
{
	THeadNode *hnode;

	if (FindNode(nodename))
		return 0;

	try
	{
		hnode = new THeadNode;
	}
	catch(...)
	{
		return -1;
	}

	hnode->firstedge = NULL;
	hnode->nexthnode = NULL;
	hnode->nodeID = GenNodeID();
	hnode->nodeEntity = nodeentity;
	strcpy(hnode->nodename, nodename);
	hnode->ndcount = 0;

	if (!m_firsthnode)
		m_firsthnode = hnode;
	else
		m_lasthnode->nexthnode = hnode;
	m_lasthnode =hnode;

	return (hnode->nodeID);
}

int CLinkMap::AddNode(char *nodename, char *actname)
{
	return AddNode(nodename, NULL, actname);
}

int CLinkMap::AddNode(char *nodename, void *nodeentity, char *actname)
{
	THeadNode *hnode;

	hnode = FindNode(actname);
	if (hnode)
		return AddNode(nodename, nodeentity, hnode);
	else
		return -1;
}

int CLinkMap::AddNode(char *nodename, THeadNode *actnode)
{
	return AddNode(nodename, NULL, actnode);

}

int CLinkMap::AddNode(char *nodename, void *nodeentity, THeadNode *actnode)
{
	THeadNode *hnode;
	TListNode *listnode;

	AddNode(nodename, nodeentity);
	hnode = FindNode(nodename);

	try
	{
		listnode = new TListNode;
	}
	catch(...)
	{
		return -1;
	}
	listnode->nodeID = hnode->nodeID;
	listnode->nextlnode = actnode->firstedge ;
	actnode->firstedge = listnode;
	actnode->ndcount++;
	return (hnode->nodeID);
}

int CLinkMap::GetListNodeCount(char *nodename)
{
	THeadNode *hnode;

	hnode = FindNode(nodename);
	return GetListNodeCount(hnode);

}

int CLinkMap::GetListNodeCount(THeadNode *hnode)
{
	return hnode->ndcount;
}


int CLinkMap::GetListNodeCount(UINT32 adr)
{
	THeadNode *hnode;

	hnode = FindNode(adr);
	return GetListNodeCount(hnode);
}

//void DrawMap(char *nodename, Graphics *graphics, int rowheight=80)
//{
//	
//}