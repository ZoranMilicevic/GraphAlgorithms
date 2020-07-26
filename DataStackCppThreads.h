#pragma once

#include <mutex>
#include <condition_variable>
#include "DataNode.h"

template <class T>
class DataStackCppThreads
{
public:
	DataStackCppThreads():head(nullptr){}
	DataStackCppThreads& operator=(const DataStackCppThreads&) = delete;
	virtual ~DataStackCppThreads(){}

	virtual void push(const T& new_elem) 
	{
		std::unique_ptr<DataNode<T>> new_node(new DataNode<T>());
		new_node->data = = std::make_shared<T>(std::move(new_elem));

		std::lock_guard<std::mutex> lock(head_mutex);
		new_node->next = std::move(head);
		head = std::move(new_node);
	}

	virtual std::shared_ptr<T> pop() 
	{
		std::unique_lock<std::mutex> lock(head_mutex);

		cond_var.wait(lock, [this] {return head != nullptr; });

		std::unique_ptr<DataNode<T>> old_head = std::move(head);
		head = std::move(old_head->next);

		if (lock.owns_lock())
			lock.unlock();

		return old_head->data;
	}

	virtual std::shared_ptr<T> try_pop() 
	{
		std::lock_guard<std::mutex> lock(head_mutex);

		if (head == nullptr)
			return nullptr;

		std::unique_ptr<DataNode<T>> old_head = std::move(head);
		head = std::move(old_head->next);

		return old_head->data;
	}

	virtual bool empty() 
	{
		std::lock_guard<std::mutex> lock(head_mutex);
		return head == nullptr;
	}

private:
	std::unique_ptr<DataNode<T>> head;
	mutable std::mutex head_mutex;
	std::condition_variable cond_var;
};

