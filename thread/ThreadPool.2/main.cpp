#include "ThreadPool.hpp"

void add(int a, int b, int c)
{
    std::cout << "result: " << a + b + c << std::endl;
    std::this_thread::sleep_for(std::chrono::milliseconds(50000));
}

void sleep(int count)
{
    std::cout << "sleep start" << std::endl;
    auto start = std::chrono::system_clock::now();
    std::this_thread::sleep_for(std::chrono::milliseconds(count));
    auto end = std::chrono::system_clock::now();
    std::cout << "sleep end" << std::endl;
    std::chrono::duration<double> times = end - start;
    std::cout << "sleep times: " << times.count() << std::endl;
}

int main()
{
    ThreadPool tp(5);
    tp.enqueue(sleep, 100000);
    tp.enqueue(add, 20, 30, 40);
    return 0;
}