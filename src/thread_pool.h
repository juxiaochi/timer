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
    explicit ThreadPool(int threadNum);
    ~ThreadPool();

    /// @brief 唤醒一个线程去支持task， 线程池中线程全忙时会失败
    /// @param t 任务
    /// @param arg 自定义参数，仅透传
    /// @return =0:成功， 其他:失败
    int Notify(Task t, void *arg);

    int GetIdleThreadNum(void) const {
        std::lock_guard<std::mutex> guard(mtx_);
        return idle_thread_num_;
    }

   private:
    void run(void);

   private:
    bool exit_flag_ = false;
    unsigned idle_thread_num_ = 0;
    TaskInfo task_;
    mutable std::mutex mtx_;
    std::condition_variable cond_;
    std::vector<std::thread> threads_;
};

}  // namespace stroll
