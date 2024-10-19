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

namespace stroll {

Timer::Timer(const char *name, Handler handler, void *arg, uint32_t intervalMS, uint32_t delayMS)
    : intervalMS_(intervalMS), delayMS_(delayMS), handler_(handler), arg_(arg), name_(name) {}

Timer::~Timer() {}

int Timer::Start() { return 0; }

int Timer::Stop() { return 0; }

int Timer::SetInterval(uint32_t intervalMS) { return 0; }

}  // namespace stroll
