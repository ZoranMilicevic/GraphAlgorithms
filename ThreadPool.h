#pragma once

#include <atomic>
#include <vector>
#include <thread>
#include <functional>
#include "DataQueueCppThreads.h"
#include "JoinThreads.h"


class ThreadPool
{
    std::atomic_bool done;
    DataQueueCppThreads<std::function<void()>> work_queue;
    std::vector<std::thread> threads;
    JoinThreads joiner;

    void worker_thread()
    {
        while (!done)
        {
            auto task = work_queue.pop();
            if (task != nullptr)
                (*task)();
        }
    }

public:
    ThreadPool(int numberOfThreads) : done(false), joiner(threads)
    {
        unsigned const thread_count = numberOfThreads;
        try
        {
            for (unsigned i = 0; i < thread_count; ++i)
                threads.push_back(std::thread(&ThreadPool::worker_thread, this));
        }
        catch (...)
        {
            done = true;
            throw;
        }
    }

    ~ThreadPool()
    {
        done = true;
        for (size_t i = 0; i < threads.size(); i++) 
        {
          work_queue.push(nullptr);
        }
    }

    static std::shared_ptr<ThreadPool> getInstance(int numberOfThreads = 0)
    {
      static std::shared_ptr<ThreadPool> pool = std::make_shared<ThreadPool>(numberOfThreads);
      return pool;
    }

    void joinThreadsFromPool()
    {
        joiner.joinThreads();
    }

    template<typename FunctionType>
    void submit(FunctionType f)
    {
        work_queue.push(f);
    }
};