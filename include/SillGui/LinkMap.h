#ifndef _LINKMAP_H_
#define _LINKMAP_H_


#ifndef SILLGUI_DLL
	#define SILLGUI_DLL  _declspec(dllimport)
#endif



#define Max_NodeName 200
#define NULL 0
#define UINT32	unsigned long 

struct TListNode{
	int			nodeID;
	TListNode	*nextlnode;
};

struct THeadNode{
	int			nodeID;
	char		nodename[Max_NodeName];
	void		*nodeEntity;
	TListNode	*firstedge;
	//TListNode	*lastedge;
	THeadNode	*nexthnode;
	int			ndcount;
};


class SILLGUI_DLL CLinkMap
{
private:
	//THeadNode	*m_curhnode;
	int			m_curNodeID;
	THeadNode	*m_firsthnode;
	THeadNode	*m_lasthnode;

private:
	int GenNodeID(void);

public:
	CLinkMap();
	virtual ~CLinkMap();

public:
	THeadNode* FindNode(char *nodename);
	THeadNode* FindNode(UINT32 adr);
	//int AddNode(char *nodename);
	int AddNode(char *nodename, void *nodeentity = NULL);
	int AddNode(char *nodename, char *actname);
	int AddNode(char *nodename, void *nodeentity, char *actname);
	int AddNode(char *nodename, THeadNode *actnode);
	int AddNode(char *nodename, void *nodeentity, THeadNode *actnode);
	//void DrawMap(char *nodename, Graphics *graphics, int rowheight=80);
	int GetListNodeCount(char *nodename);
	int GetListNodeCount(THeadNode *hnode);
	int GetListNodeCount(UINT32 adr);

};
//
#endif
