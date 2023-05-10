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
    ThreadPool(size_t threadNums);
    template<class Function, class... Args>
    void enqueue(Function &&f, Args &&...args);
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
        workers_.emplace_back(
            [this]
            {
                for (;;)
                {
                    std::function<void()> task;
                    {
                        std::unique_lock<std::mutex> lock(this->mutex_);
                        this->condition_.wait(lock, [this]
                                              { return this->stop_ || !this->tasks_.empty(); });
                        if (this->stop_ && this->tasks_.empty())
                            return;
                        task = std::move(this->tasks_.front());
                        this->tasks_.pop();
                    }
                    task();
                }
            });
}

template<class Function, class... Args>
void ThreadPool::enqueue(Function &&f, Args &&...args)
{
    auto task = std::bind(std::forward<Function>(f), std::forward<Args>(args)...);
    {
        std::unique_lock<std::mutex> lock(mutex_);
        tasks_.emplace([task]{task(); });
    }
    condition_.notify_all();
}

inline ThreadPool::~ThreadPool()
{
    {
        std::unique_lock<std::mutex> lock(this->mutex_);
        stop_ = true;
    }
    condition_.notify_all();
    for (std::thread &worker : workers_)
        worker.join();
}