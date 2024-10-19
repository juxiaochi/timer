/**
 * @file utils.h
 * @author stroll (116356647@qq.com)
 * @brief 工具类
 * @version 0.1
 * @date 2024-10-19
 *
 * @copyright Copyright (c) 2024
 *
 */

#pragma once

#include <stdint.h>

namespace stroll {

/// @brief 获取当前系统稳定递增时间，纳秒
/// @param  
/// @return 
uint64_t GetTickCount(void);

/// @brief 毫秒转纳秒
/// @param ms 
/// @return 
inline uint64_t MilliSecond2NaoSecond(uint32_t ms) { return ms * 1000 * 1000; }

}  // namespace sroll
