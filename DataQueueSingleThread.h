#pragma once

#include "DataNode.h"

template <class T>
class DataQueueSingleThread
{
public:
	DataQueueSingleThread() :head(nullptr), tail(nullptr) {};
	virtual ~DataQueueSingleThread() {}

	virtual void push(const T& new_elem)
	{
		std::unique_ptr<DataNode<T>> new_node(new DataNode<T>());
		new_node->data = std::make_shared<T>(std::move(new_elem));

		if (tail == nullptr) 
		{
			tail = new_node.get();
			head = std::move(new_node);
		}
		else 
		{
			DataNode<T>* new_tail = new_node.get();
			tail->next = std::move(new_node);
			tail = new_tail;
		}
	};

	virtual std::shared_ptr<T> pop()
	{
		if (empty())
			return nullptr;

		std::unique_ptr<DataNode<T>> old_head = std::move(head);
		head = std::move(old_head->next);

		if (head == nullptr) tail = nullptr;

		return old_head->data;
	};

	virtual bool empty() const
	{
		return head == nullptr;
	};

protected:
	std::unique_ptr<DataNode<T>> head;
	DataNode<T> *tail;
};
