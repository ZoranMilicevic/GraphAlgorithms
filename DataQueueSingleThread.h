#pragma once

#include "DataNode.h"

template <class T>
class DataQueueSingleThread
{
public:
	DataQueueSingleThread() :head(NULL), tail(NULL) {};
	DataQueueSingleThread(T* head) :head(head), tail(head) {};
	DataQueueSingleThread(T* head, T* tail) : head(head), tail(tail) {};
	virtual ~DataQueueSingleThread() 
	{
		delete head;
	}

	virtual void push(const T& new_elem)
	{
		T* new_data = new T(std::move(new_elem));
		DataNode<T>* new_node = new DataNode<T>();
		new_node->data = new_data;

		tail->next = new_node;
		tail = new_node;
	};

	virtual T* pop()
	{
		if (empty())
			return NULL;

		DataNode<T>* old_head = head;
		head = head->next;

		return old_head->data;
	};

	virtual bool empty() const
	{
		return head == NULL;
	};

protected:
	DataNode<T>* head;
	DataNode<T>* tail;
};
