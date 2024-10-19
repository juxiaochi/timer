/**
 * @file utils.cpp
 * @author stroll (116356647@qq.com)
 * @brief 
 * @version 0.1
 * @date 2024-10-19
 * 
 * @copyright Copyright (c) 2024
 * 
 */

#include "utils.h"
#include <time.h>

namespace stroll
{

uint64_t GetTickCount(void)
{
    struct timespec ts;
    clock_gettime(CLOCK_MONOTONIC, &ts);
    return ts.tv_nsec + ts.tv_sec * 1000 * 1000 * 1000;
}

} // namespace stroll
