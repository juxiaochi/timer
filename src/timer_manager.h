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
#include <vector>

#include "timer.h"
#include "timer_node.h"
#include "thread_pool.h"

namespace stroll
{

class TimerManager final
{
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
    inline unsigned MakeThreadNumber(void);

    void Schedule(void *);
    void RunTimer(TimerNodePtr, void *);
    
    void UpdateHeap() {
        auto node = heap_.top();
        heap_.pop();
        node->UpdateExpireTime();
        heap_.push(node);
    }

    int AddTack2ThreadPool() {
        return thread_pool_.Notify([this](void *arg) { Schedule(arg); }, nullptr);
    }

    TimerNodePtr GetWorkNode(void);

private:
    struct TimerNodePtrCompare
    {
        bool operator()(const TimerNodePtr &lh, const TimerNodePtr &rh) {
            return lh->expire_time > rh->expire_time;
        }
    };
    using Heap = std::priority_queue<TimerNodePtr, 
                        std::vector<TimerNodePtr>, 
                        TimerNodePtrCompare>;
    bool exit_flag_ = false;
    ThreadPool thread_pool_;
    std::mutex mtx_;
    Heap heap_;

};

} // namespace stroll
