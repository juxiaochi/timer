/**
 * @file timer_manager.h
 * @author stroll (116356647@qq.com)
 * @brief 定时器管理者, 调度定时器
 * @version 0.1
 * @date 2024-10-19
 * 
 * @copyright Copyright (c) 2024
 * 
 */

#pragma once

#include <queue>
#include <mutex>
#include <string>

#include "timer.h"
#include "timer_node.h"
#include "thread_pool.h"

namespace stroll
{

class TimerManager final
{
    //< 调度线程轮询步长(毫秒)，如果想要定时器调度精度高，就减小轮询步长
    //< 过小的步长，会导致CPU占用率较高，建议在10ms以上
    static constexpr unsigned kPollStepMS = 10;

    //< 线程池中线程最小数
    static constexpr unsigned kMinThreadNum = 4;

public:
    ~TimerManager();

    static TimerManager& Instance()
    {
        static TimerManager inst;
        return inst;
    }

    TimerNodePtr AddTimer(std::string &name, Timer::Handler, void *arg, uint32_t interval_ms, uint32_t delay_ms);
    int DeleteTimer(TimerNodePtr node);

    int Start(TimerNodePtr node) { return node->Start(); }
    int Stop(TimerNodePtr node) { return node->Stop(); }

    int SetInterval(TimerNodePtr node, uint32_t interval_ms);

private:
    TimerManager();
    unsigned MakeThreadNumber(void);

    void Schedule(void *);
    void RunTimer(TimerNodePtr, void *);
    
    void UpdateHeap(TimerNodePtr node) {
        heap_.pop();
        node->UpdateExpireTime();
        heap_.push(node);
    }

    int AddTack2ThreadPool() {
        return thread_pool_.Notify([this](void *arg) { Schedule(arg); }, nullptr);
    }

    TimerNodePtr GetWorkNode(void);

private:
    bool exit_flag_ = false;
    ThreadPool thread_pool_;
    std::mutex mtx_;
    std::priority_queue<TimerNodePtr> heap_;

};

} // namespace stroll
