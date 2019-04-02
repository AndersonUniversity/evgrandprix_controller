/* mbed Microcontroller Library
 * Copyright (c) 2018 ARM Limited
 * SPDX-License-Identifier: Apache-2.0
 */

#pragma once

#include <mbed.h>

/**
 *  System Reporting library. Provides runtime information on device:
 *      - CPU sleep, idle, and wake times
 *      - Heap and stack usage
 *      - Thread information
 *      - Static system information
 */
class SystemReport {
    mbed_stats_heap_t   heap_stats;
    mbed_stats_cpu_t    cpu_stats;
    mbed_stats_sys_t    sys_stats;

    mbed_stats_thread_t *thread_stats;
    uint8_t   thread_count;
    uint8_t   max_thread_count;
    uint32_t  sample_time_ms;
    uint64_t prev_idle_time;

public:
    /**
     *  SystemReport - Sample rate in ms is required to handle the CPU percent awake logic
     */
    SystemReport(uint32_t sample_rate);
    ~SystemReport();

    uint32_t sample_time() const { return sample_time_ms; }

    /**
     *  Report on each Mbed OS Platform stats API
     */
    void report_state();

    /**
     *  Report CPU idle and awake time in terms of percentage
     */
    void report_cpu_stats();

    /**
     *  Report current heap stats. Current heap refers to the current amount of
     *  allocated heap. Max heap refers to the highest amount of heap allocated
     *  since reset.
     */
    void report_heap_stats();

    /**
     *  Report active thread stats
     */
    void report_thread_stats();
};
