#pragma once

#include <memory>
#include "DataNode.h"

template <class T>
class DataStackSingleThread
{
public:
	DataStackSingleThread() :head(nullptr) {}
	virtual ~DataStackSingleThread() {}

	virtual void push(const T& new_elem)
	{
		std::unique_ptr<DataNode<T>> new_node(new DataNode<T>());
		new_node->data = std::make_shared<T>(std::move(new_elem));

		new_node->next = std::move(head);
		head = std::move(new_node);
	}

	virtual std::shared_ptr<T> pop()
	{
		if (empty())
			return nullptr;

		std::unique_ptr<DataNode<T>> old_head = std::move(head);
		head = std::move(old_head->next);

		return old_head->data;
	}
	
	virtual bool empty() const
	{
		return head == nullptr;
	}

protected:
	std::unique_ptr<DataNode<T>> head;
};
