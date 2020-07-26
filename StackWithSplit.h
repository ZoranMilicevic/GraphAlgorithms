#pragma once

#include <memory>
#include "DataNode.h"


template<typename T>
class StackWithSplit
{
public:
	StackWithSplit() :head(nullptr), size(0) {}
	virtual ~StackWithSplit() {}

	virtual void push(const T& new_elem)
	{
		std::unique_ptr<DataNode<T>> new_node(new DataNode<T>());
		new_node->data = std::make_shared<T>(std::move(new_elem));

		new_node->next = std::move(head);
		head = std::move(new_node);
		size++;
	}

	virtual std::shared_ptr<T> pop()
	{
		if (empty())
			return nullptr;

		std::unique_ptr<DataNode<T>> old_head = std::move(head);
		head = std::move(old_head->next);
		size--;

		return old_head->data;
	}

	virtual bool empty() const
	{
		return head == nullptr;
	}

	virtual int get_size()
	{
		return size;
	}

	void split(std::shared_ptr<StackWithSplit> to_fill, int transfer)
	{
		for(int i = 0; i<transfer; i++)
		{
			to_fill->push(*pop());
		}
	}

protected:
	std::unique_ptr<DataNode<T>> head;
	int size;
};