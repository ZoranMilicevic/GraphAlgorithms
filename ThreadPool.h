#pragma once

#include <atomic>
#include <vector>
#include <thread>
#include <functional>
#include "DataQueueCppThreads.h"
#include "JoinThreads.h"
#include "ConfigurationParameters.h"

class ThreadPool
{
    std::atomic_bool done;
    DataQueueCppThreads<std::function<void()>> work_queue;
    std::vector<std::thread> threads;
    JoinThreads joiner;
    unsigned thread_count;
    inline static ThreadPool* pool;

    void worker_thread()
    {
        while (!done)
        {
            std::function<void()>* task = work_queue.try_pop();
            if (task != NULL)
                (*task)();
            else
                std::this_thread::yield();
        }
    }

public:
    ThreadPool(int numberOfThreads) : done(false), joiner(threads)
    {
        thread_count = numberOfThreads;
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
    }


    template<typename FunctionType>
    void submit(FunctionType f)
    {
        work_queue.push(f);
    }
};