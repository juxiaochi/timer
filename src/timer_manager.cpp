/**
 * @file timer_manager.cpp
 * @author stroll (116356647@qq.com)
 * @brief
 * @version 0.1
 * @date 2024-10-19
 *
 * @copyright Copyright (c) 2024
 *
 */

#include "timer_manager.h"

#include "thread_pool.h"
#include "trace.h"
#include "utils.h"

namespace stroll {

//< 调度线程轮询步长(毫秒)，如果想要定时器调度精度高，就减小轮询步长
//< 过小的步长，会导致CPU占用率较高，建议在10ms以上
static const unsigned kPollStepMS = 5;

//< 线程池中线程最小数
static const unsigned kMinThreadNum = 4;


TimerManager::TimerManager() : thread_pool_(MakeThreadNumber()) {
    //< 启动一个调度线程
    AddTack2ThreadPool();
}

TimerManager::~TimerManager() {
    exit_flag_ = true;
}

TimerNodePtr TimerManager::AddTimer(std::string &name, Timer::Handler h, void *arg, uint32_t interval_ms,
                                    uint32_t delay_ms) {
    TimerNodePtr ret = std::make_shared<TimerNode>(name, h, arg, interval_ms, delay_ms);
    std::lock_guard<std::mutex> guard(mtx_);
    heap_.push(ret);
    return ret;
}

int TimerManager::DeleteTimer(TimerNodePtr node) {
    WARN("delete timer:%s, status:%d\n", node->name.c_str(), (int)node->status);
    std::lock_guard<std::mutex> guard(mtx_);
    node->Delete();
    if (node->status != TimerStatus::kRunning)
    {
        return 0;
    }

    WARN("[%s]destroy timer object, wait timer finish!!!\n", node->name.c_str());
    while(node->status == TimerStatus::kRunning) {
        std::this_thread::sleep_for(std::chrono::milliseconds(10));
    }

    DEBUG("delete timer:%s success!!!\n", node->name.c_str());
    return 0;
}

int TimerManager::SetInterval(TimerNodePtr node, uint32_t interval_ms) {
    if (node->interval_ns == 0) {
        ERROR("[%s] timer is single shot\n", node->name.c_str());
        return 0;
    }
    
    auto interval_ns = MilliSecond2NaoSecond(interval_ms);
    std::swap(node->interval_ns, interval_ns);
    return interval_ns / 1000 / 1000;
}

unsigned TimerManager::MakeThreadNumber(void) {
    unsigned thread_num = std::thread::hardware_concurrency();
    thread_num = thread_num < kMinThreadNum ? kMinThreadNum : thread_num;
    return thread_num;
}

void TimerManager::Schedule(void *arg) {
    while (!exit_flag_) {
        std::unique_lock<std::mutex> lock(mtx_);
        auto node = GetWorkNode();
        if (!node) {
            lock.unlock();
            std::this_thread::sleep_for(std::chrono::milliseconds(kPollStepMS));
            continue;
        }

        //< 有待处理的任务
        //< 需要判断线程池是否有空闲的线程，否则会出现调度线程无法唤醒
        if (thread_pool_.GetIdleThreadNum() == 0) {
            WARN("no idle thread !!!\n");
            lock.unlock();
            std::this_thread::sleep_for(std::chrono::milliseconds(kPollStepMS));
            continue;
        }

        //< 就地执行任务，唤醒另一个线程去执行调度程序
        UpdateHeap();
        // 执行前设置为运行中状态
        node->UpdateStatus(TimerStatus::kRunning);
        lock.unlock();
        AddTack2ThreadPool();
        
        //< 就地执行任务
        RunTimer(node, arg);
        break;
    }
}

TimerNodePtr TimerManager::GetWorkNode(void) {
    uint64_t now = GetTickCount();
    while (true) {
        if (heap_.empty()) {
            return nullptr;
        }

        auto node = heap_.top();
        if (node->delete_flag)
        {
            heap_.pop();
            continue;
        }

        if (node->expire_time > now) {
            return nullptr;
        }

        //< 运行超时的定时器，要等待到下一个调度周期
        if (node->status == TimerStatus::kRunning) {
            WARN("[%s] run timeout \n", node->name.c_str());
            UpdateHeap();
            continue;
        }
        return node;
    }
}

void TimerManager::RunTimer(TimerNodePtr node, void *arg) {
    // 执行完成后设置为激活状态
    node->handler(node->arg);
    node->UpdateStatus(TimerStatus::kActivate);
    if (node->interval_ns == 0 || node->pause_flag) {
        node->UpdateStatus(TimerStatus::kStop);
    }
}

}  // namespace stroll
