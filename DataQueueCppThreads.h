#pragma once

#include <mutex>
#include <condition_variable>
#include <atomic>
#include "DataNode.h"
#include <iostream>

//using a dummy node

template<class T>
class DataQueueCppThreads
{
public:
	DataQueueCppThreads(): head(new DataNode<T>()), tail(head), head_mutex(), tail_mutex(), cond_var(){};
	DataQueueCppThreads& operator=(const DataQueueCppThreads&) = delete;
	virtual ~DataQueueCppThreads() 
	{
		delete head;
	}

	virtual void push(const T& new_elem)
	{
		T* new_data = new T(std::move(new_elem));
		DataNode<T>* new_node = new DataNode<T>();

		tail_mutex.lock();

		tail->data = new_data; //!!!WRITING THE NEW VALUE INTO THE DUMMY NODE
		tail->next = new_node;
		tail = new_node;

		tail_mutex.unlock();
		//cond_var.notify_all();
	}

	//todo fix pop
	virtual T* pop()
	{
		std::unique_lock<std::mutex> head_lock(head_mutex);
		cond_var.wait(head_lock, [&] { return head != get_tail(); });

		DataNode<T>* old_head = head;
		head = head->next;

		if(head_lock.owns_lock())
			head_lock.unlock();

		return old_head->data;
	}

	virtual T* try_pop()
	{
		std::lock_guard<std::mutex> head_lock(head_mutex);

		if (head == get_tail())
			return NULL;

		DataNode<T>* old_head = head;
		head = head->next;
		
		return old_head->data;
	}

protected:
	DataNode<T>* get_tail()
	{
		std::lock_guard<std::mutex> tail_lock(tail_mutex);
		return tail;
	}

	DataNode<T>* head;
	DataNode<T>* tail;

	mutable std::mutex head_mutex;
	mutable std::mutex tail_mutex;
	std::condition_variable cond_var;
};


