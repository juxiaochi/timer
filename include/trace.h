/**
 * @file trace.h
 * @author stroll (116356647@qq.com)
 * @brief 日志模块，简单封装printf
 * @version 0.1
 * @date 2024-10-19
 * 
 * @copyright Copyright (c) 2024
 * 
 */

#pragma once

#include <cstdio>

#define INFO(FMT, arg...) \
    printf("[%s:%d]" FMT, __func__, __LINE__, ##arg)

#define WARN(FMT, arg...) \
    printf("[%s:%d]" FMT, __func__, __LINE__, ##arg)

#define ERROR(FMT, arg...) \
    printf("[%s:%d]" FMT, __func__, __LINE__, ##arg)
