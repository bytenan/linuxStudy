#pragma once

#include <iostream>
#include <vector>
#include <queue>
#include <memory>
#include <functional>
#include <thread>
#include <mutex>
#include <condition_variable>

class ThreadPool
{
public:
    ThreadPool(size_t);
    ~ThreadPool();

private:
    std::vector<std::thread> workers_;
    std::queue<std::function<void()>> tasks_;
    std::mutex mutex_;
    std::condition_variable condition_;
    bool stop_;
};

inline ThreadPool::ThreadPool(size_t threadNums)
    : stop_(false)
{
    for (size_t i = 0; i < threadNums; ++i)
    {
        workers_.emplace_back(
            [this]
            {
                for (;;)
                {
                    std::function<void()> task;
                    {
                        std::unique_lock<std::mutex> lock(this->mutex_);
                        while (this->tasks_.empty())
                        {
                            this->condition_.wait(lock);
                        }
                        task = std::move(this->tasks_.front());
                        this->tasks_.pop();
                    }
                    task();
                }
            });
    }
}

inline ThreadPool::~ThreadPool()
{
}