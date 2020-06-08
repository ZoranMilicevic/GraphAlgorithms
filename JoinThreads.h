#pragma once

#include <vector>
#include <thread>

class JoinThreads
{
public:
    explicit JoinThreads(std::vector<std::thread>& threads) : threads(threads){}
    ~JoinThreads()
    {
        for (unsigned long i = 0; i < threads.size(); ++i)
        {
            if (threads[i].joinable())
                threads[i].join();
        }
    }

    void joinThreads()
    {
        for (unsigned long i = 0; i < threads.size(); ++i)
        {
            if (threads[i].joinable())
                threads[i].join();
        }
    }

private:
    std::vector<std::thread>& threads;
};
