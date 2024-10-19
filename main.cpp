/**
 * @file main.cpp
 * @author stroll (116356647@qq.com)
 * @brief 
 * @version 0.1
 * @date 2024-10-19
 * 
 * @copyright Copyright (c) 2024
 * 
 */


#include "timer.h"

#include <cstdio>
#include <unistd.h>

using namespace stroll;

void test(void *arg)
{
    struct timespec ts;
    clock_gettime(CLOCK_MONOTONIC, &ts);
    printf("[%s] %ld.%ld\n", (char *)arg, ts.tv_sec, ts.tv_nsec/1000/1000);
}

void test1(void *arg)
{
    test(arg);
    // sleep(13);
}

int main()
{
    Timer t("main", test, (void *)"test0", 1000, 1000);
    Timer t1("main1", test1, (void *)"test1", 3000, 1000);
    t.Start();
    t1.Start();
    printf("press any key to exit\n");
    getchar();
    return 0;
}
