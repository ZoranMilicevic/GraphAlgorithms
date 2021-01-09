#pragma once

#include <mutex>
#include <condition_variable>
#include <deque>
#include <optional>

template<class T>
class deque_threadsafe
{
public:
	deque_threadsafe() {};
	deque_threadsafe& operator=(const deque_threadsafe&) = delete;

	virtual void push(const T& new_elem)
	{
		m.lock();
		queue.push_front(new_elem);
		m.unlock();
		cond_var.notify_one();
	}

	virtual T pop()
	{
		std::unique_lock<std::mutex> ul(m);
		cond_var.wait(ul, [&] { return queue.size() != 0; });

		auto retVal = queue.back();
		queue.pop_back();

		return retVal;
	}

	virtual std::optional<T> try_pop()
	{
		std::lock_guard<std::mutex> lg(m);

		if (queue.size() == 0)
			return std::nullopt;

		auto retVal = std::make_optional<T>(queue.back());
		queue.pop_back();

		return retVal;
	}

protected:
	std::deque<T> queue;
	mutable std::mutex m;
	std::condition_variable cond_var;
};
