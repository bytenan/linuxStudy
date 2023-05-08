#pragma once

#include <iostream>
#include <string>
#include <vector>
#include <queue>
#include <unistd.h>
#include <pthread.h>

const static size_t gthreadNums = 10;

enum ERROR
{
    PTHREAD_CREATE_ERROR = 1
};

template <class T>
class ThreadPool;

template <class T>
class Context
{
public:
    ThreadPool<T> *tp_;
    Context(ThreadPool<T> *tp)
        : tp_(tp)
    {
    }
};

template <class T>
class ThreadPool
{
public:
    ThreadPool(const size_t &threadNums = gthreadNums)
        : threadNums_(threadNums), ctx_(this)
    {
        threads_.resize(threadNums_);
        pthread_mutex_init(&mutex_, nullptr);
        pthread_cond_init(&cond_, nullptr);

        for (size_t i = 0; i < threadNums_; ++i)
        {
            pthread_t tid;
            int n = pthread_create(&tid, nullptr, start_routine, &ctx_);
            if (n != 0)
            {
                std::cerr << "pthread_create error" << std::endl;
                exit(PTHREAD_CREATE_ERROR);
            }
            threads_.push_back(tid);
        }
        std::cout << "ThreadPool create success" << std::endl;
    }
    void push(const T &in)
    {
        pthread_mutex_lock(&mutex_);
        tasks_.push(in);
        pthread_cond_signal(&cond_);
        pthread_mutex_unlock(&mutex_);
    }
    void pop(T *out)
    {
        *out = tasks_.front();
        tasks_.pop();
    }
    static void *start_routine(void *args)
    {
        Context<T> *ctx = static_cast<Context<T> *>(args);
        T t;
        while (true)
        {
            pthread_mutex_lock(&ctx->tp_->mutex_);
            while (ctx->tp_->tasks_.empty())
            {
                pthread_cond_wait(&ctx->tp_->cond_, &ctx->tp_->mutex_);
            }
            ctx->tp_->pop(&t);
            pthread_mutex_unlock(&ctx->tp_->mutex_);
            std::cout << "Get the results of the task# " << t() << std::endl;
        }
        delete ctx;
        pthread_exit(nullptr);
    }
    ~ThreadPool()
    {
        pthread_mutex_destroy(&mutex_);
        pthread_cond_destroy(&cond_);
    }

private:
    size_t threadNums_;
    std::vector<pthread_t> threads_;
    std::queue<T> tasks_;
    pthread_mutex_t mutex_;
    pthread_cond_t cond_;
    Context<T> ctx_;
};