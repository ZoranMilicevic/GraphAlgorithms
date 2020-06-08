#pragma once

#include <mutex>
#include <condition_variable>
#include <atomic>
#include "DataNode.h"
#include "ConfigurationParameters.h"
#include <iostream>

//using a dummy node

template<class T>
class DataQueueCppThreads
{
public:
	DataQueueCppThreads(): head(new DataNode<T>()), tail(head), head_mutex(), tail_mutex(), cond_var(){};
	DataQueueCppThreads& operator=(const DataQueueCppThreads&) = delete;
	virtual ~DataQueueCppThreads() {}

	virtual void push(const T& new_elem)
	{
		T* new_data = new T(std::move(new_elem));
		DataNode<T>* new_node = new DataNode<T>();

		tail_mutex.lock();

		tail->data = new_data; //!!!WRITING THE NEW VALUE INTO THE DUMMY NODE
		tail->next = new_node;
		tail = new_node;

		tail_mutex.unlock();
		cond_var.notify_all();
		std::cout << "NOTIFIED ALL" << std::endl;
	}

	virtual T* pop()
	{
		std::unique_lock<std::mutex> head_lock(head_mutex);
		cond_var.wait_for(head_lock, ConfigurationParameters::cond_var_wait_time, [&] { std::cout << "CHECKED" << std::endl; return head != get_tail(); });

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
	DataNode<T>* head;
	DataNode<T>* tail;

	mutable std::mutex head_mutex;
	mutable std::mutex tail_mutex;
	std::condition_variable cond_var;
};


