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

#include <unistd.h>

#include <cstdio>

#include "timer.h"
#include "trace.h"

using namespace stroll;

#define TEST_TITLE() INFO(">>>>>>>>>>>>>>>>>>>>>>>>>>>>>\n\n")

void test(void *arg) { INFO(" %s \n", (char *)arg); }

void test1(void *arg) {
    WARN(" %s\n", (char *)arg);
    // sleep(13);
}

void test_more_timer() {
    TEST_TITLE();
    Timer t("main", test, (void *)"test_more_timer0", 1000, 1000);
    Timer t1("main1", test1, (void *)"test_more_timer1", 3000, 1000);
    t.Start();
    t1.Start();
    sleep(10);
    
}

void test2(void *arg) {
    WARN(" %s\n", (char *)arg);
    sleep(3);
}

void test_delete_timer() {
    TEST_TITLE();
    {
        Timer t("main2", test2, (void *)"test_delete_timer", 1000, 0);
        t.Start();
        sleep(1);
    }
    
}

void test_start_stop_timer() {
    TEST_TITLE();
    Timer t("main3", test, (void *)"test_start_stop_timer", 1000, 1000);
    t.Start();
    
    sleep(3);
    t.Stop();
    INFO("stop timer main3 !!!\n");

    sleep(3);
    INFO("start timer main3 !!!\n");
    t.Start();

    sleep(3);
    INFO("update timer interval: 3s !!!\n");
    INFO("old interval: %d\n", t.SetInterval(3000));
    sleep(10);
    
}

void test3(void *arg) {
    WARN(" test3 =====>>>>>>>>\n");
    Timer *pt = static_cast<Timer *>(arg);
    pt->Stop();
}

void test_start_stop_timer1() {
    TEST_TITLE();
    Timer t("main4", test3, (void *)&t, 1000, 1000);
    t.Start();

    sleep(3);
    INFO("start timer main4 !!!\n");
    t.Start();

    sleep(3);
}

int main() {
    test_more_timer();
    test_delete_timer();
    test_start_stop_timer();
    test_start_stop_timer1();
    printf("press any key to exit\n");
    getchar();
    return 0;
}
