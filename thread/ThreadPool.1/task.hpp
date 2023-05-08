#pragma once

#include <iostream>
#include <functional>
#include <string>
#include <cstdio>
#include <cstdlib>

const char ops[5] = {'+', '-', '*', '/', '%'};

class Task
{
public:
    using func_t = std::function<int(int, int, char)>;

    Task()
    {
    }
    Task(const int &x, const int &y, const char &op, func_t callback)
        : x_(x), y_(y), op_(op), callback_(callback)
    {
    }
    std::string operator()()
    {
        int res = callback_(x_, y_, op_);
        char buf[1024];
        snprintf(buf, sizeof buf, "%d %c %d = %d", x_, op_, y_, res);
        return buf;
    }
    std::string toString()
    {
        char buf[1024];
        snprintf(buf, sizeof buf, "%d %c %d = ?", x_, op_, y_);
        return buf;
    }
    ~Task() {}

private:
    int x_;
    int y_;
    char op_;
    func_t callback_;
};

int mymath(int x, int y, char op)
{
    int res;
    switch (op)
    {
    case '+':
        res = x + y;
        break;
    case '-':
        res = x - y;
        break;
    case '*':
        res = x * y;
        break;
    case '/':
    {
        if (0 == y)
        {
            std::cerr << "divide by zero" << std::endl;
            res = -1;
        }
        else
        {
            res = x / y;
        }
    }
    break;
    case '%':
    {
        if (0 == y)
        {
            std::cerr << "divide by zero" << std::endl;
            res = -1;
        }
        else
        {
            res = x % y;
        }
    }
    break;
    default:
        std::cerr << "undefined operator" << std::endl;
        exit(1);
        break;
    }
    return res;
}