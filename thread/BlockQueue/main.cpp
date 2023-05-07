#include "BlockQueue.hpp"
#include <memory>
#include <ctime>
#include <cstdlib>
#include <unistd.h>

void *producer(void *args)
{
    BlockQueue<int> *bq = static_cast<BlockQueue<int> *>(args);
    while (true)
    {
        int n = rand();
        bq->push(n);
        std::cout << "生产# " << n++ << std::endl;
    }
}

void *consumer(void *args)
{
    BlockQueue<int> *bq = static_cast<BlockQueue<int> *>(args);
    while (true)
    {
        sleep(1);
        int res = 0;
        bq->pop(&res);
        std::cout << "消费# " << res << std::endl;
    }
}

int main()
{
    srand((unsigned int)time(nullptr));
    pthread_t p, c;
    BlockQueue<int> *bq = new BlockQueue<int>();
    pthread_create(&p, nullptr, producer, bq);
    pthread_create(&c, nullptr, consumer, bq);
    pthread_join(p, nullptr);
    pthread_join(c, nullptr);
    delete bq;
    return 0;
}