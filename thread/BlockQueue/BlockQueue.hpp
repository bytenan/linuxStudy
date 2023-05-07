#pragma once

#include <iostream>
#include <queue>
#include <pthread.h>

const static size_t gcap = 10; 

template<class T>
class BlockQueue
{
public:
    BlockQueue(const size_t &cap = gcap)
        : cap_(cap)
    {
        pthread_mutex_init(&mutex_, nullptr);
        pthread_cond_init(&pcond_, nullptr);
        pthread_cond_init(&ccond_, nullptr);
    }
    void push(const T &in)
    {
        pthread_mutex_lock(&mutex_);
        while (bq_.size() == cap_)
        {
            pthread_cond_wait(&pcond_, &mutex_);
        }
        bq_.push(in);
        pthread_cond_signal(&ccond_);
        pthread_mutex_unlock(&mutex_);
    }
    void pop(T *out)
    {
        pthread_mutex_lock(&mutex_);
        while (bq_.empty())
        {
            pthread_cond_wait(&ccond_, &mutex_);
        }
        *out = bq_.front();
        bq_.pop();
        pthread_cond_signal(&pcond_);
        pthread_mutex_unlock(&mutex_);
    }
    ~BlockQueue()
    {
        pthread_mutex_destroy(&mutex_);
        pthread_cond_destroy(&pcond_);
        pthread_cond_destroy(&ccond_);
    }

private:
    std::queue<T> bq_;
    size_t cap_;
    pthread_mutex_t mutex_;
    pthread_cond_t pcond_;
    pthread_cond_t ccond_;
};