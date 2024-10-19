/**
 * @file timer_node.h
 * @author stroll (116356647@qq.com)
 * @brief
 * @version 0.1
 * @date 2024-10-19
 *
 * @copyright Copyright (c) 2024
 *
 */

#pragma once

#include <cstdint>
#include <string>
#include <mutex>

#include "timer.h"

namespace stroll {

enum class TimerStatus {
    kIDLE = 0,
    kStop,
    kActivate,
    kRunning,
};

struct TimerNode final {
    std::mutex mtx;
    Timer::Handler handler;
    void *arg = nullptr;
    std::string name {"null"};
    uint64_t delay_ns = 0;     //< 延迟时间
    uint64_t interval_ns = 0;  //< 间隔时间
    uint64_t start_tick = 0;   //< 开始时间
    uint64_t expire_time = -1;  //< 到期时间
    TimerStatus status = TimerStatus::kIDLE;
    bool pause_flag = true;  // 挂起标志

    TimerNode(std::string &name, Timer::Handler, void *arg, uint32_t interval_ms, uint32_t delay_ms);
    ~TimerNode() = default;

    int Start();
    int Stop();
    int StopAndWait();

    int UpdateStatus(TimerStatus s) {
        std::lock_guard<std::mutex> lk(mtx);
        return UpdateStatusImpl(s);
    }

    void UpdateExpireTime() {
        if (interval_ns == 0) {
            expire_time = -1;
        }
        expire_time += interval_ns;
    }
    
private:
    int UpdateStatusImpl(TimerStatus s);
};

}  // namespace stroll
