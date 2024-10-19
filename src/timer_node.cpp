/**
 * @file timer_node.cpp
 * @author stroll (116356647@qq.com)
 * @brief
 * @version 0.1
 * @date 2024-10-19
 *
 * @copyright Copyright (c) 2024
 *
 */

#include "timer_node.h"

#include "timer.h"
#include "trace.h"
#include "utils.h"

namespace stroll {

bool operator<(const TimerNode &lhs, const TimerNode &rhs) {
    return lhs.expire_time > rhs.expire_time;
}

TimerNode::TimerNode(std::string &name, Timer::Handler h, void *arg, uint32_t interval_ms, uint32_t delay_ms)
    : handler(h),
      arg(arg),
      name(name),
      delay_ns(MilliSecond2NaoSecond(delay_ms)),
      interval_ns(MilliSecond2NaoSecond(interval_ms)) {}

int TimerNode::Start() {
    std::lock_guard<std::mutex> lock(mtx);
    if (!pause_flag) {
        return 0;  //< 已经启动，直接返回
    }

    start_tick = GetTickCount();
    expire_time = start_tick + delay_ns;
    UpdateStatusImpl(TimerStatus::kActivate);
    pause_flag = false;
    return 0;
}

int TimerNode::Stop() {
    std::lock_guard<std::mutex> lock(mtx);
    if (pause_flag) {
        return 0;
    }

    //< 直接设置最大超时时间
    pause_flag = true;
    expire_time = static_cast<uint64_t>(-1);
    UpdateStatusImpl(TimerStatus::kStop);
    return 0;
}

int TimerNode::StopAndWait() {
    return 0;
}

int TimerNode::UpdateStatusImpl(TimerStatus s) {
    switch (s) {
        case TimerStatus::kActivate:
            break;

        case TimerStatus::kStop:
            //< 正在运行时不能直接切换到停止，先切换到激活状态
            if (status == TimerStatus::kRunning) {
                WARN("current status: %d, want:%d!!!\n", (int)status, (int)s);
                return -1;
            }
            break;

        case TimerStatus::kRunning:
            if (status == TimerStatus::kStop) {
                WARN("current status: %d, want:%d!!!\n", (int)status, (int)s);
                return -1;
            }
            break;

        default:
            ERROR("unknow status:%d!!!\n", (int)s);
            return -1;
    }

    status = s;
    return 0;
}

}  // namespace stroll
