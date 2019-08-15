#include <iostream>
#include <hash_map>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <ext/hash_map>
#include <thread>
#include "threadpool-cpp11/threadpool.hpp"

using namespace std;
using namespace __gnu_cxx;

namespace stdext=__gnu_cxx;

using namespace cpp11threadpool;
using namespace cpp11threadpool::threadpool;


void sum(int a)
{
    std::cout<<"这是执行了啊"<<"  param is "<<a<<std::endl;
}


/*
这个线程池是GitHub上有个大神基于boost写的，后来经由cocos2d改成了基于STL的了
文件夹下的makefile是将此线程池生成一个静态库
直接 g++ -o main.out -std=c++11 -g -Wall -L. -lthreadpool -pthread即可
*/
int main(int argc, char *argv[])
{
    threadpool::pool _pool(3);
     std::thread::hardware_concurrency();
    bool b= _pool.schedule(std::bind(sum,34));
    _pool.schedule(std::bind(sum,56));
    _pool.schedule(std::bind(sum,75));
    _pool.schedule(std::bind(sum,235));
    std::cout<<" bool is "<<b<<std::endl;

    std::cout<<"na ni..\n"<<std::endl;

    getchar();
    return 0;
}
