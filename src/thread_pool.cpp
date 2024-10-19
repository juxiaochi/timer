/**
 * @file thread_pool.cpp
 * @author stroll (116356647@qq.com)
 * @brief
 * @version 0.1
 * @date 2024-10-19
 *
 * @copyright Copyright (c) 2024
 *
 */

#include "thread_pool.h"

#include "trace.h"

namespace stroll {

ThreadPool::ThreadPool(unsigned threadNum) {
    threads_.reserve(threadNum);
    for (auto i = 0; i < threadNum; ++i) {
        threads_.emplace_back(std::thread(&ThreadPool::run, this));
    }
}

ThreadPool::~ThreadPool() {
    {
        std::lock_guard<std::mutex> guard(mtx_);
        exit_flag_ = true;
        cond_.notify_all();
    }

    for (auto &t : threads_) {
        t.join();
    }
}

void ThreadPool::run(void) {
    while (true) {
        std::unique_lock<std::mutex> lock(mtx_);
        free_thread_num_ += 1;
        cond_.wait(lock);

        //< 线程退出
        if (exit_flag_) {
            WARN("thread pool exit !!!\n");
            break;
        }

        //< 误唤醒
        if (!task_.task) {
            WARN("thread pool wake up !!!\n");
            continue;
        }
        free_thread_num_ -= 1;

        using std::swap;
        TaskInfo tmp_task;
        swap(tmp_task, task_);
        lock.unlock();

        //< 执行 任务
        tmp_task.task(tmp_task.arg);
    }
}

}  // namespace stroll
