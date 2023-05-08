#include "BlockQueue.hpp"
#include "task.hpp"
#include <memory>
#include <ctime>
#include <cstdlib>
#include <unistd.h>

void *producer(void *args)
{
    BlockQueue<Task> *bq = static_cast<BlockQueue<Task> *>(args);
    while (true)
    {
        int x = rand() % 10;
        int y = rand() % 10;
        char op = ops[rand() % 5];
        Task t(x, y, op, mymath);
        bq->push(t);
        std::cout << "生产# " << t.toString() << std::endl;
    }
}

void *consumer(void *args)
{
    BlockQueue<Task> *bq = static_cast<BlockQueue<Task> *>(args);
    while (true)
    {
        sleep(1);
        Task t;
        bq->pop(&t);
        std::cout << "消费# " << t() << std::endl;
    }
}

int main()
{
    srand((unsigned int)time(nullptr));
    pthread_t p, c;
    BlockQueue<Task> *bq = new BlockQueue<Task>();
    pthread_create(&p, nullptr, producer, bq);
    pthread_create(&c, nullptr, consumer, bq);
    pthread_join(p, nullptr);
    pthread_join(c, nullptr);
    delete bq;
    return 0;
}