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
    ThreadPool() : done(false), joiner(threads)
    {
        unsigned const thread_count = ConfigurationParameters::thread_count;
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

    static ThreadPool* getInstance()
    {
        static ThreadPool* pool = new ThreadPool();
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

    template<typename FunctionType>
    void submit(FunctionType f)
    {
        work_queue.push(f);
    }
};