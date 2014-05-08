#ifndef __TOOLTREE_MULTIWAYTREE_H__
#define __TOOLTREE_MULTIWAYTREE_H__


#define MAX_CHILDREN 3


template <class T> 
class  CMultiwayTree 
{
public:
	CMultiwayTree(void);
	virtual ~CMultiwayTree(void);
	CMultiwayTree* NewNode(T& data);
	CMultiwayTree* AddSibNode(T& data);
	CMultiwayTree* LastSibling();

public:
	CMultiwayTree*	child[MAX_CHILDREN];	// point to child node
	CMultiwayTree*	sibling;				// point to sibling node
	CMultiwayTree*	father;					// point to father node

public:
	T	m_userData;
};


template <class T> 
CMultiwayTree<T>::CMultiwayTree(void):sibling(NULL),father(NULL)
{
	for( int i = 0; i < MAX_CHILDREN; i++ ) 
		child[i] = NULL;
}

template <class T>
CMultiwayTree<T>::~CMultiwayTree(void)
{
	for( int i = 0; i < MAX_CHILDREN; i++ ) 
		if( child[i] ) 
			delete child[i];

	if( sibling ) 
		delete sibling;
}

template <class T>
CMultiwayTree<T>* CMultiwayTree<T>::NewNode(T& data)
{
	CMultiwayTree<T> * t = new CMultiwayTree<T>();
	t->m_userData = data;
	return t;
}

template <class T>
CMultiwayTree<T>* CMultiwayTree<T>::AddSibNode(T& data)
{
	CMultiwayTree<T> * t = this, *tf = NULL;
	if ( !t )
		return t;
	
	if ( t->father ) tf = t->father;

	while(t->sibling)
		t = t->sibling;
	t->sibling = NewNode(data);
	t = t->sibling;
	t->father = tf;
	return t;
}

template <class T>
CMultiwayTree<T>* CMultiwayTree<T>::LastSibling()
{
	CMultiwayTree<T> *t = this;
	while(t->sibling) t = t->sibling;
	return t;
}
#endif
