#include "ThreadPool.hpp"
#include "task.hpp"
#include <cstdlib>
#include <ctime>
#include <unistd.h>

int main()
{
    srand((unsigned int)time(nullptr));
    ThreadPool<Task> *tp = new ThreadPool<Task>();
    while (true)
    {
        int x = rand() % 10;
        int y = rand() % 10;
        char op = ops[rand() % 5];
        tp->push(Task(x, y, op, mymath));
        sleep(1);
    }
    return 0;
}