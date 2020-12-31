#pragma once

#include <memory>
#include "DataNode.h"

template <class T>
class DataStack
{
public:
	DataStack() :head(nullptr), _size(0) {}
	virtual ~DataStack() {
		//must be like this, because if not, too many nodes
		//will cause to many destructors called recursivly 
		//which will result in stack overflow
		auto curNode = std::move(head);
		while (curNode) {
			curNode = std::move(curNode->next);
		}
	}

	virtual void push(const T& new_elem)
	{
		std::unique_ptr<DataNode<T>> new_node(new DataNode<T>());
		new_node->data = std::make_shared<T>(std::move(new_elem));
		_size++;

		new_node->next = std::move(head);
		head = std::move(new_node);
	}

	virtual std::shared_ptr<T> pop()
	{
		if (empty())
			return nullptr;

		std::unique_ptr<DataNode<T>> old_head = std::move(head);
		head = std::move(old_head->next);
		_size--;

		return old_head->data;
	}
	
	virtual bool empty() const
	{
		return head == nullptr;
	}

	virtual unsigned size() const
	{
		return _size;
	}

	void split(std::shared_ptr<DataStack> to_fill, int transfer)
	{
		for (int i = 0; i < transfer; i++)
			to_fill->push(*pop());
	}

protected:
	std::unique_ptr<DataNode<T>> head;
	unsigned _size;
};
