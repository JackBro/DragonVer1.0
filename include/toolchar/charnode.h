#ifndef _CHARNODE_H_
#define _CHARNODE_H_


#ifndef TOOLCHAR_DLL
	#define TOOLCHAR_DLL  __declspec(dllimport)
#endif

#include "tooltree/nodebase.h"
#include "toolchain/chaintools.h"

enum CHARFILTERenum
{
	CHAR_MISC	=	0x100,
	CHAR_DEC_DIGITAL,
	CHAR_HEX_DIGITAL,
	CHAR_OCT_DIGITAL,
	CHAR_CHAR,
	CHAR_WORD,
	CHAR_STRING,

	CHAR_COMMENTS,

	CHAR_BLANK,		// 
	CHAR_NEWLINE,		// 

	WORD_DIGFLOAT,	//floating data
	WORD_PLUS,		//++
	WORD_MINUS,		//--
	WORD_OR,		//||
	WORD_AND,		//&&
	
	WORD_PLUSE,		//+=
	WORD_MINUSE,	//-=
	WORD_MULE,		//*=
	WORD_DIVE,		///=
	WORD_MODE,		//%=
	WORD_ANDE,		//&=
	WORD_ORE,		//|=
	WORD_XORE,		//^=
	WORD_EQUAL,		//==
	WORD_NOTE,		//!=
	WORD_LESSE,		//<=
	WORD_BIGE		//>=
};

//=================================

class TOOLCHAR_DLL CCharNode : public CNodeBase
{
public:
	CCharNode(void);
	virtual ~CCharNode(void);

public:
	typedef struct tag_TCharKey {
		int			len;
		char		*key;
		CCharNode	*pnode;
	} TCharKey, *PTCharKey;

public:
	void  Connect(int len, char *key, CCharNode *pnode);
	void  ConnectH7(CCharNode *pnode);
	void  ConnectAny(CCharNode *pnode);
	void  ConnectOther(void);
	CCharNode *GetNode(char key);

private:
	void connectNode(int len, char *key, CCharNode *pnode);
	int VerfiyKey(int len, char *key);
	int	VerifyKey(char *src, int len, char key);
	int	VerifyKey(char *src, char *dst, int len1, int len2);

private:
	CChainTool<TCharKey>	m_node;
	CChainTool<CCharNode>	m_misc;
};

#endif

