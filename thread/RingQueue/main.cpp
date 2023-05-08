#include "RingQueue.hpp"
#include <ctime>
#include <cstdlib>
#include <unistd.h>

void *producer(void *args)
{
    RingQueue<int> *rq = static_cast<RingQueue<int> *>(args);
    while (true)
    {
        int n = rand() % 10;
        rq->push(n);
        std::cout << "push# " << n << std::endl;
    }
}
void *consumer(void *args)
{
    RingQueue<int> *rq = static_cast<RingQueue<int> *>(args);
    while (true)
    {
        sleep(1);
        int n;
        rq->pop(&n);
        std::cout << "pop# " << n << std::endl;
    }
}

int main()
{
    srand((unsigned int)time(nullptr));
    RingQueue<int> *rq = new RingQueue<int>();
    pthread_t p, c;
    pthread_create(&p, nullptr, producer, rq);
    pthread_create(&p, nullptr, consumer, rq);
    pthread_join(p, nullptr);
    pthread_join(c, nullptr);
    delete rq;
    return 0;
}