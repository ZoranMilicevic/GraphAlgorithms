#pragma once

#include <mutex>
#include <condition_variable>
#include <atomic>
#include "DataNode.h"

template <class T>
class DataStackCppThreads
{
public:
	DataStackCppThreads():head(nullptr), _size(0){}
	DataStackCppThreads& operator=(const DataStackCppThreads&) = delete;
	virtual ~DataStackCppThreads(){
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
		new_node->data = = std::make_shared<T>(std::move(new_elem));

		std::lock_guard<std::mutex> lock(head_mutex);
		new_node->next = std::move(head);
		head = std::move(new_node);
		_size++;
	}

	virtual std::shared_ptr<T> pop() 
	{
		std::unique_lock<std::mutex> lock(head_mutex);

		cond_var.wait(lock, [this] {return head != nullptr; });

		std::unique_ptr<DataNode<T>> old_head = std::move(head);
		head = std::move(old_head->next);

		if (lock.owns_lock())
			lock.unlock();

		_size--;

		return old_head->data;
	}

	virtual std::shared_ptr<T> try_pop() 
	{
		std::lock_guard<std::mutex> lock(head_mutex);

		if (head == nullptr)
			return nullptr;

		std::unique_ptr<DataNode<T>> old_head = std::move(head);
		head = std::move(old_head->next);
		_size--;

		return old_head->data;
	}

	virtual bool empty() 
	{
		std::lock_guard<std::mutex> lock(head_mutex);
		return head == nullptr;
	}

	virtual unsigned size() const
	{
		return _size;
	}

	void split(std::shared_ptr<DataStackCppThreads> to_fill, int transfer)
	{
		for (int i = 0; i < transfer; i++)
			to_fill->push(*pop());
	}

private:
	std::unique_ptr<DataNode<T>> head;
	mutable std::mutex head_mutex;
	std::condition_variable cond_var;
	std::atomic_uint _size;
};

