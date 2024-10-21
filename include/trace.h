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
#include <time.h>

#ifdef TIMER_DEBUG
#define DEBUG(FMT, arg...) \
    TRACE("DEBUG", FMT, ##arg)
#else
#define DEBUG(FMT, arg...)
#endif

#define INFO(FMT, arg...) \
    TRACE("INFO", FMT, ##arg)

#define WARN(FMT, arg...) \
    TRACE("WARN", FMT, ##arg)

#define ERROR(FMT, arg...) \
    TRACE("ERROR", FMT, ##arg)

#define TRACE(LEVEL, FMT, arg...) \
    do { \
        struct timespec ts; \
        clock_gettime(CLOCK_REALTIME_COARSE, &ts); \
        printf("[%zd.%zd][%s][%s:%d] " FMT, \
            ts.tv_sec, ts.tv_nsec / 1000 / 1000, LEVEL, __func__, __LINE__, ##arg); \
    }while(0)
