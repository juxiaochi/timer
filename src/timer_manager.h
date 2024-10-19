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

namespace stroll
{

class TimerManager final
{
public:
    static TimerManager& Instance()
    {
        static TimerManager inst;
        return inst;
    }

private:
    TimerManager();

public:
    ~TimerManager();

};

} // namespace stroll
