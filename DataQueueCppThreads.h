#pragma once

#include <mutex>
#include <condition_variable>
#include <atomic>
#include "DataNode.h"

//using a dummy node

template<class T>
class DataQueueCppThreads
{
public:
	DataQueueCppThreads() : head(new DataNode<T>()), tail(head.get()), head_mutex(), tail_mutex(), cond_var(), _size(0) {};
	DataQueueCppThreads& operator=(const DataQueueCppThreads&) = delete;
	virtual ~DataQueueCppThreads()
	{
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
		std::shared_ptr<T> new_data = std::make_shared<T>(std::move(new_elem));
		DataNode<T>* new_tail = new_node.get();

		tail_mutex.lock();

		tail->data = new_data; //!!!WRITING THE NEW VALUE INTO THE DUMMY NODE
		tail->next = std::move(new_node);
		tail = new_tail;

		tail_mutex.unlock();
		_size++;
		cond_var.notify_one();
	}

	//todo fix pop
	virtual std::shared_ptr<T> pop()
	{
		std::unique_lock<std::mutex> head_lock(head_mutex);
		cond_var.wait(head_lock, [&] { return _size != 0; });

		std::shared_ptr<DataNode<T>> old_head = std::move(head);
		head = std::move(old_head->next);
		_size--;

		if (head_lock.owns_lock())
			head_lock.unlock();

		return old_head->data;
	}

	virtual std::shared_ptr<T> try_pop()
	{
		std::lock_guard<std::mutex> head_lock(head_mutex);

		if (head.get() == get_tail())
			return nullptr;

		std::shared_ptr<DataNode<T>> old_head = std::move(head);
		head = std::move(old_head->next);
		_size--;

		return old_head->data;
	}

protected:
	DataNode<T>* get_tail()
	{
		std::lock_guard<std::mutex> tail_lock(tail_mutex);
		return tail;
	}

	std::unique_ptr<DataNode<T>> head;
	DataNode<T>* tail;

	mutable std::mutex head_mutex;
	mutable std::mutex tail_mutex;
	std::condition_variable cond_var;
	std::atomic_uint _size;
};
