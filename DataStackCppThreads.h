#pragma once

#include <mutex>
#include <condition_variable>
#include "DataNode.h"

template <class T>
class DataStackCppThreads
{
public:
	DataStackCppThreads():head(NULL){}
	DataStackCppThreads(DataNode<T>* head) : head(head) {}
	DataStackCppThreads& operator=(const DataStackCppThreads&) = delete;
	virtual ~DataStackCppThreads(){}

	virtual void push(const T& new_elem) 
	{
		T* new_data = new T(std::move(new_elem));
		DataNode<T>* new_node = new DataNode<T>();
		new_node->data = new_data;

		std::lock_guard<std::mutex> lock(head_mutex);
		
		new_node->next = head;
		head = new_node;
	}

	virtual T* pop() 
	{
		std::unique_lock<std::mutex> lock(head_mutex);

		cond_var.wait(lock, [this] {return head != NULL; });

		DataNode<T>* old_head = head;
		head = head->next;

		if (lock.owns_lock())
			lock.unlock();

		return old_head->data;
	}

	virtual T* try_pop() 
	{
		std::lock_guard<std::mutex> lock(head_mutex);

		if (head == NULL)
			return NULL;

		DataNode<T>* old_head = head;
		head = head->next;

		return old_head->data;
	}

	virtual bool empty() 
	{
		std::lock_guard<std::mutex> lock(head_mutex);
		return head == NULL;
	}

private:
	DataNode<T>* head;
	mutable std::mutex head_mutex;
	std::condition_variable cond_var;
};

