#pragma once

#include "DataNode.h"

template <class T>
class DataQueueSingleThread
{
public:
	DataQueueSingleThread() :head(nullptr), tail(nullptr) {};
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
		new_node->next = nullptr;

		if (tail == nullptr)
			head = tail = new_node;
		else 
		{
			tail->next = new_node;
			tail = new_node;
		}
	};

	virtual T* pop()
	{
		if (empty())
			return nullptr;

		DataNode<T>* old_head = head;
		head = head->next;

		if (head == nullptr) tail = nullptr;

		return old_head->data;
	};

	virtual bool empty() const
	{
		return head == nullptr;
	};

protected:
	DataNode<T>* head;
	DataNode<T>* tail;
};
