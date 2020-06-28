#pragma once

#include "DataNode.h"

template <class T>
class DataStackSingleThread
{
public:
	DataStackSingleThread() :head(nullptr) {}
	DataStackSingleThread(DataNode<T>* head) :head(head) {}
	virtual ~DataStackSingleThread() {}

	virtual void push(const T& new_elem)
	{
		T* new_data = new T(std::move(new_elem));
		DataNode<T>* new_node = new DataNode<T>();
		new_node->data = new_data;

		new_node->next = head;
		head = new_node;
	}

	virtual T* pop()
	{
		if (empty())
			return nullptr;

		DataNode<T>* old_head = head;
		head = head->next;

		return old_head->data;
	}
	
	virtual bool empty() const
	{
		return head == nullptr;
	}

protected:
	DataNode<T>* head;
};
