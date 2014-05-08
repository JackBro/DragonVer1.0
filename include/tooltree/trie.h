#ifndef _TRIE_H_
#define _TRIE_H_


#ifndef TOOLTREE_DLL
	#define TOOLTREE_DLL  __declspec(dllimport)
#endif



#define MAX_WORD_LENGTH 128

typedef enum { LEAF, BRANCH } NodeKind;

struct TrieNode
{
	char symbol;
	TrieNode *next;
	NodeKind kind;
	union
	{
		TrieNode * first;
		char word[MAX_WORD_LENGTH];
	};
};//TrieNode;

class TOOLTREE_DLL CTrie
{
 public:
 	CTrie();
	virtual ~CTrie();

public:
	void Release(TrieNode * node);
	void Insert(const char * word, const char* leafvalue);
	char * MatchWord(char * word);
	TrieNode * MatchPrefix(char c, TrieNode* p);
	TrieNode * ConstructNode(const char *word, unsigned int idx, const char* leafValue);
	TrieNode * GetRoot();

private:
	TrieNode* Root;
};

#endif

