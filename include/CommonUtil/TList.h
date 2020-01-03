#pragma once

#include <stdlib.h>
#include <stdio.h>
#define NEW_NODE getSingleNode
#define DELETE_NODE putSingleNode

#define MAX_UNUSEDNODE_SIZE 1000

#if 1
template <class T>
class TList
{
struct SingleNode{
	T element;
	struct SingleNode *next;
	struct SingleNode *prev;
	struct SingleNode *head;
};

public:
	class TIterator{

	public:

		TIterator()
		{
			m_base = NULL;
		}

		TIterator(SingleNode *node)
		{
			m_base = node;
		}

		T operator*() const
		{	// return designated value
			return m_base->element;
			//return ((reference)**(_Mybase_iter *)this);
		}

		TIterator operator++()
		{	// preincrement
			if(m_base == m_base->head){
				printf("Error : TList operator++()1 -> m_base is head\n");
				exit(0);
			}

			m_base = m_base->next;

			if(m_base == m_base->head){
				printf("Error : TList operator++()2 -> m_base is head\n");
				exit(0);
			}

			return *this;
		}

		TIterator operator+=(int idx)
		{	// preincrement
			for(int i=0; i<idx; i++)
				(*this)++;

			return *this;
		}

		TIterator operator+(int idx)
		{	
			SingleNode *tmp = m_base;
			for(int i=0; i<idx; i++)
			{
				if(tmp == tmp->head){
					printf("Error : TList operator+(int idx)1 -> m_base is head (idx :%d)\n", idx);
					exit(0);
				}
				tmp = tmp->next;
			}

			if(tmp == tmp->head){
				printf("Error : TList operator+(int idx)2 -> m_base is head (idx :%d)\n", idx);
				exit(0);
			}

			return TIterator(tmp);
		}

		TIterator operator++(int)
		{	// postincrement
			if(m_base == m_base->head){
				printf("Error : TList operator++(int) -> m_base is head\n");
				exit(0);
			}

			TIterator _Tmp(m_base);
			m_base = m_base->next;

			return _Tmp;
		}

		TIterator operator--()
		{	// predecrement
			m_base = m_base->prev;

			if(m_base == m_base->head){
				printf("Error : TList operator--() -> m_base is head\n");
				exit(0);
			}

			return *this;
		}

		TIterator operator-=(int idx)
		{	// preincrement
			for(int i=0; i<idx; i++)
				(*this)--;

			return *this;
		}

		TIterator operator-(int idx)
		{	
			SingleNode *tmp = m_base;
			for(int i=0; i<idx; i++)
			{
				tmp = tmp->prev;

				if(tmp == tmp->head){
					printf("Error : TList operator-(int idx)1 -> m_base is head (idx :%d)\n", idx);
					exit(0);
				}
			}

			if(tmp == tmp->head){
				printf("Error : TList operator-(int idx)2 -> m_base is head (idx :%d)\n", idx);
				exit(0);
			}

			return TIterator(tmp);
		}

		TIterator operator--(int)
		{	// postdecrement
			TIterator _Tmp(m_base);
			if(m_base == m_base->head){
				printf("Error : TList operator--(int)1 -> m_base is head\n");
				exit(0);
			}
			m_base = m_base->prev;

			if(m_base == m_base->head){
				printf("Error : TList operator--(int)2 -> m_base is head\n");
				exit(0);
			}

			return _Tmp;
		}

		bool operator==(TIterator& rhs)
		{	// postdecrement
			return m_base == rhs.m_base;
		}

		bool operator!=(TIterator& rhs)
		{	// postdecrement
			return m_base != rhs.m_base;
		}

		friend class TList;

private :
		SingleNode *getM_base()
		{
			return m_base;
		}
		void setM_base(SingleNode *node)
		{
			m_base = node;
		}
	
		SingleNode *m_base;
	};

	TList()
	{
		initUnusedSingleNode(MAX_UNUSEDNODE_SIZE);
		head = NEW_NODE();

		list_size = 0;
		head->next = head;
		head->prev = head;
		head->head = head;
		max_size = -1;

	}

	virtual ~TList()
	{
		deleteList();
		deleteSingleNode(head);
		deleteUnusedNodeList();
	}

	void clear()
	{
		deleteList();
	}

	virtual void setMaxSize(int size)
	{
		max_size = size;
	}

	virtual bool push_back(T e)
	{
		if(max_size != -1 && max_size <= list_size)
			return false;
		SingleNode *node = createSingleNode(e);

		insert(head->prev->next, node);
		return true;
	}
	virtual bool push_front(T e)
	{
		if(max_size != -1 && max_size <= list_size)
			return false;

		SingleNode *node = createSingleNode(e);

		insert(head->next, node);
		return true;
	}
#if 1
	void pop_front()
	{
		SingleNode *node = head->next;

		if(erase(node) == true)
		{
			deleteSingleNode(node);
		}
		else
		{
			printf("Error :: pop_back()\n");
		}
	}

	void pop_back()
	{
		SingleNode *node = head->prev;

		if(erase(node) == true)
		{
			deleteSingleNode(node);
		}
		else
		{
			printf("Error :: pop_back()\n");
		}
	}
#else
	virtual T pop_front()
	{
		SingleNode *node = head->next;
		T element;
		
		if(erase(node) == true)
		{
			element = (T)node->element;
			//deleteSingleNode(node);
		}
		else
		{
			printf("Error :: pop_back()\n");
		}

		return element;
	}

	virtual T pop_back()
	{
		SingleNode *node = head->prev;
		T element;

		if(erase(node) == true)
		{
			element = (T)node->element;
			//deleteSingleNode(node);
		}
		else
		{
			printf("Error :: pop_back()\n");
		}

		return element;

	}
#endif
	virtual T front()
	{
		T element;

		if(head->next == head){
			printf("Error :: front()\n");
		}else{
			element = (T)head->next->element;
		}

		return element;
	}

	virtual T back()
	{
		T element;

		if(head->prev == head){
			printf("Error :: back()\n");
		}else{
			element = (T)head->prev->element;
		}

		return element;
	}
	int size()
	{
		return list_size;
	}
private :
	virtual bool insert(SingleNode *where, SingleNode *node)
	{
		node->next = where;
		node->prev = where->prev;
		where->prev->next = node;
		where->prev = node;

		list_size++;
		return true;
	}

	virtual bool erase(SingleNode *where)
	{
		if(list_size == 0)
		{
			printf("Error : ListSize is 0 (Can't erase)!!!\n");
			return false;
		}
		else
		{
			where->prev->next = where->next;
			where->next->prev = where->prev;

			list_size--;
			return true;
		}
	}

	void deleteList()
	{
		while(size() > 0)
			pop_front();
	}

	SingleNode *createSingleNode(T e)
	{
		SingleNode *node = NEW_NODE();
		node->element = e;
		node->next = NULL;
		node->head = head;
		return node;
	}

	void deleteSingleNode(SingleNode *node)
	{
		node->next = NULL;
		node->prev = NULL;
		node->head = NULL;

		DELETE_NODE(node);
	}

	void initUnusedSingleNode(int size)
	{
		unused_single_node_list_size = 0;
		unused_single_node_list = NULL;
#ifdef UseMemoryManager
		for(int i = 0 ; i< size ; i++)
		{
			SingleNode *node = new SingleNode();
			node->next = unused_single_node_list;
			unused_single_node_list = node;
			unused_single_node_list_size++;
		}
#endif

	}


	SingleNode *getSingleNode()
	{
		SingleNode *node; 
#ifdef UseMemoryManager
		if(unused_single_node_list)
		{
			node = unused_single_node_list;
			unused_single_node_list = unused_single_node_list->next;
			unused_single_node_list_size--;
		}
		else
		{
			node = new SingleNode();
		}
#else
		node = new SingleNode();
#endif

		node->next = NULL;
		node->prev = NULL;
		return node; 
	}

	void putSingleNode(SingleNode *node)
	{
		node->next = NULL;
		node->prev = NULL;

#ifdef UseMemoryManager
		if(unused_single_node_list_size>MAX_UNUSEDNODE_SIZE)
		{
			delete node;
		}
		else
		{
			node->next = unused_single_node_list;
			unused_single_node_list = node;
			unused_single_node_list_size++;
		}
#else
		delete node;
#endif
	}
	void deleteUnusedNodeList()
	{
		SingleNode *tmp;
		while(unused_single_node_list)
		{
			tmp = unused_single_node_list;
			unused_single_node_list = unused_single_node_list->next;
			delete tmp;
		}
		unused_single_node_list_size = 0;

	}

private :
	SingleNode *head;
	int list_size;
	int max_size;


	SingleNode *unused_single_node_list;
	int unused_single_node_list_size;
};
#else
template <class T>
class TList
{
public :
	TList()
	{
		list_size = 0;
		head = NULL;
		tail = NULL;
		max_size = -1;
	}

	virtual ~TList()
	{
		deleteList();
	}

	void clear()
	{
		deleteList();
	}

	virtual void setMaxSize(int size)
	{
		max_size = size;
	}

	virtual bool push_back(T e)
	{
		if(max_size != -1 && max_size <= list_size)
			return false;
		SingleNode *node = createSingleNode(e);

		if(head)
		{
			tail->next = node;
			node->prev = tail;
			tail = node;
		}
		else
		{
			head = tail = node;
		}
		list_size++;
		return true;
	}
	virtual bool push_front(T e)
	{
		if(max_size != -1 && max_size <= list_size)
			return false;

		SingleNode *node = createSingleNode(e);

		if(head)
		{
			node->next = head;
			head->prev = node;
			head = node;
		}
		else
		{
			head = tail = node;
		}
		

		list_size++;
		return true;
	}
	virtual T pop_front()
	{
		if(!head){
			printf("Error :: pop_back()\n");
			return NULL;
		}

		list_size--;

		SingleNode *node = head;
		head = node->next;
		if(head)
		{
			head->prev = NULL;
		}
		else
		{
			tail = NULL;
		}
			
		T element = (T)node->element;
		deleteSingleNode(node);
		return element;

	}

	virtual T pop_back()
	{
		if(!tail){
			printf("Error :: pop_back()\n");
			return NULL;
		}

		list_size--;

		SingleNode *node = tail;
		tail = node->prev;
		if(tail)
		{
			tail->next = NULL;
		}
		else
		{
			head = NULL;
		}
			
		T element = (T)node->element;
		deleteSingleNode(node);

		return element;

	}

	virtual T front()
	{
		if(!head){
			printf("Error :: pop_back()\n");
			return NULL;
		}else{
			return (T)head->element;
		}
	}
	virtual T back()
	{
		if(!tail){
			printf("Error :: pop_back()\n");
			return NULL;
		}else{
			return (T)tail->element;
		}
	}
	int size()
	{
		return list_size;
	}
private :
	void deleteList()
	{
		while(size() > 0)
			pop_front();
	}

	SingleNode *createSingleNode(T e)
	{
		SingleNode *node = NEW_NODE();
		node->element = e;
		node->next = NULL;
		node->prev = NULL;
		return node;
	}

private :
	SingleNode *head;
	SingleNode *tail;
	//	SingleNode *prev;
	int list_size;
	int max_size;
	
};
#endif
