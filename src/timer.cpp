/**
 * @file timer.cpp
 * @author stroll (116356647@qq.com)
 * @brief
 * @version 0.1
 * @date 2024-10-19
 *
 * @copyright Copyright (c) 2024
 *
 */

#include "timer.h"

#include "timer_manager.h"

namespace stroll {

Timer::Timer(const char *name, Handler handler, void *arg, uint32_t intervalMS, uint32_t delayMS)
    : intervalMS_(intervalMS), name_(name) {
    node_ = TimerManager::Instance().AddTimer(name_, handler, arg, intervalMS, delayMS);
}

Timer::~Timer() {
    TimerManager::Instance().DeleteTimer(node_);
}

int Timer::Start() { return TimerManager::Instance().Start(node_); }

int Timer::Stop() { return TimerManager::Instance().Stop(node_); }

int Timer::SetInterval(uint32_t intervalMS) { return TimerManager::Instance().SetInterval(node_, intervalMS); }

}  // namespace stroll
