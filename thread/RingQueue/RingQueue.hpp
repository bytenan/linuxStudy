#pragma once

#include <iostream>
#include <vector>
#include <pthread.h>
#include <semaphore.h>

const static size_t gcap = 10;

template<class T>
class RingQueue
{
public:
    RingQueue(const size_t &cap = gcap)
        : cap_(cap)
    {
        rq_.resize(cap_);
        pthread_mutex_init(&p_mutex_, nullptr);
        pthread_mutex_init(&c_mutex_, nullptr);
        sem_init(&sem_space_, 0, cap_);
        sem_init(&sem_data_, 0, 0);
    }
    void push(const T &in)
    {
        sem_wait(&sem_space_);
        pthread_mutex_lock(&p_mutex_);
        rq_[p_step_++] = in;
        p_step_ %= cap_;
        pthread_mutex_unlock(&p_mutex_);
        sem_post(&sem_data_);
    }
    void pop(T *out)
    {
        sem_wait(&sem_data_);
        pthread_mutex_lock(&c_mutex_);
        *out = rq_[c_step_++];
        c_step_ %= cap_;
        pthread_mutex_unlock(&c_mutex_);
        sem_post(&sem_space_);
    }
    ~RingQueue()
    {
        pthread_mutex_destroy(&p_mutex_);
        pthread_mutex_destroy(&c_mutex_);
        sem_destroy(&sem_space_);
        sem_destroy(&sem_data_);
    }
private:
    std::vector<T> rq_;
    size_t cap_;
    sem_t sem_space_;
    sem_t sem_data_;
    pthread_mutex_t p_mutex_;
    pthread_mutex_t c_mutex_;
    int p_step_;
    int c_step_;
};