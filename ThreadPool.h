#pragma once

#include <atomic>
#include <vector>
#include <thread>
#include <functional>
#include "DataQueueCppThreads.h"

class ThreadPool
{
    std::atomic_bool done;
    DataQueueCppThreads<std::function<void()>> work_queue;
    std::vector<std::thread> threads;

    void worker_thread()
    {
        while (!done)
        {
            auto task = work_queue.pop();
            if (!done && task)
                (*task)();
        }
    }

public:
    ThreadPool(int numberOfThreads) : done(false)
    {
        unsigned const thread_count = numberOfThreads;
        try
        {
          for (unsigned i = 0; i < thread_count; ++i) 
            threads.push_back(std::thread(&ThreadPool::worker_thread, this));

          for (auto&& thr : threads)
            thr.detach();

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
          work_queue.push([]()->void { int o=0; o++; });
        }
    }

    template<typename FunctionType>
    void submit(const FunctionType& f)
    {
        work_queue.push(f);
    }
};