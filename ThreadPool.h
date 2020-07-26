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
    inline static ThreadPool* pool;

    void worker_thread()
    {
        while (!done)
        {
            
            auto task = work_queue.try_pop();
            if (task != nullptr)
                (*task)();
            else
                std::this_thread::yield();
                
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
    }

    static ThreadPool* getInstance(int numberOfThreads = 0)
    {
        if(pool == nullptr)
            pool = new ThreadPool(numberOfThreads);
        return pool;
    }

    void joinThreadsFromPool()
    {
        joiner.joinThreads();
    }

    void stopThreads()
    {
        done = true;
    }

    static void destroy_pool()
    {
        delete pool;
        pool = nullptr;
    }


    template<typename FunctionType>
    void submit(FunctionType f)
    {
        work_queue.push(f);
    }
};