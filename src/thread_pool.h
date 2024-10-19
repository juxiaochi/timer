/**
 * @file thread_pool.h
 * @author stroll (116356647@qq.com)
 * @brief 线程池，定时器使用线程池来调度
 * @version 0.1
 * @date 2024-10-19
 *
 * @copyright Copyright (c) 2024
 *
 */

#pragma once

#include <condition_variable>
#include <functional>
#include <mutex>
#include <thread>
#include <vector>

namespace stroll {

class ThreadPool final {
   public:
    using Task = std::function<void(void *)>;

    struct TaskInfo
    {
        Task task;
        void *arg;
    };
    
   public:
    explicit ThreadPool(unsigned threadNum);
    ~ThreadPool();

   private:
    void run(void);

   private:
    bool exit_flag_ = false;
    unsigned free_thread_num_ = 0;
    TaskInfo task_;
    std::mutex mtx_;
    std::condition_variable cond_;
    std::vector<std::thread> threads_;
};

}  // namespace stroll
