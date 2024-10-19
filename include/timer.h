/**
 * @file timer.h
 * @author stroll (116356647@qq.com)
 * @brief 实现单次/周期性定时器，支持启动/停止/修改定时器时间
 * @version 0.1
 * @date 2024-10-19
 *
 * @copyright Copyright (c) 2024
 *
 */

#pragma once

#include <cstdint>
#include <functional>
#include <memory>
#include <string>

namespace stroll {

struct TimerNode;
using TimerNodePtr = std::shared_ptr<TimerNode>;

class Timer final {
   public:
    /// @brief 定时器执行句柄，一个void*类型形参，无返回值
    using Handler = std::function<void(void *)>;

   public:
    /// @brief 定义一个定时器
    /// @param name 定时器的名称，尽量唯一，在定时器执行超时时会打印出来
    /// @param handler 执行句柄
    /// @param arg 透传数据
    /// @param intervalMS 执行间隔-单位毫秒，>0时表示是周期性定时器
    /// @param delayMS 延迟执行- 第一次执行时/stop后重新执行时生效
    Timer(const char *name, Handler handler, void *arg, uint32_t intervalMS = 0, uint32_t delayMS = 0);
    ~Timer();

    /// @brief 开始/停止定时器，支持重新开始
    /// @return =0，成功
    /// @return =其他，失败
    int Start();
    int Stop();

    /// @brief 获取定时器执行间隔
    /// @return 定时器执行间隔
    int GetInterval() const { return intervalMS_; }

    /// @brief 设置定时器执行间隔,仅支持周期性定时器
    /// @param intervalMS
    /// @return 返回之前的执行间隔
    int SetInterval(uint32_t intervalMS);

    /// @brief 判断是否是单此定时器
    /// @return
    int IsSingleSlot() const { return intervalMS_ == 0; }

   private:
    uint32_t intervalMS_;
    uint32_t delayMS_;
    Handler handler_;
    void *arg_;
    std::string name_;
    TimerNodePtr node_;
};

}  // namespace stroll
