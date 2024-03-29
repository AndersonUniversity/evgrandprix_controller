#include "SystemReport.hpp"

SystemReport::SystemReport(uint32_t sample_rate) :
max_thread_count(8),
sample_time_ms(sample_rate),
prev_idle_time(0)
{
    thread_stats = new mbed_stats_thread_t[max_thread_count];

    // Collect the static system information
    mbed_stats_sys_get(&sys_stats);

    printf("=============================== SYSTEM INFO  ================================\r\n");
    printf("Mbed OS Version: %ld \r\n", sys_stats.os_version);
    printf("CPU ID: 0x%lx \r\n", sys_stats.cpu_id);
    printf("Compiler ID: %d \r\n", sys_stats.compiler_id);
    printf("Compiler Version: %ld \r\n", sys_stats.compiler_version);

    for (int i = 0; i < MBED_MAX_MEM_REGIONS; i++) {
        if (sys_stats.ram_size[i] != 0) {
            printf("RAM%d: Start 0x%lx Size: 0x%lx \r\n", i, sys_stats.ram_start[i], sys_stats.ram_size[i]);
        }
    }
    for (int i = 0; i < MBED_MAX_MEM_REGIONS; i++) {
        if (sys_stats.rom_size[i] != 0) {
            printf("ROM%d: Start 0x%lx Size: 0x%lx \r\n", i, sys_stats.rom_start[i], sys_stats.rom_size[i]);
        }
    }
}

SystemReport::~SystemReport()
{
    free(thread_stats);
}

void SystemReport::report_state()
{
    report_cpu_stats();
    report_heap_stats();
    report_thread_stats();

    // Clear next line to separate subsequent report logs
    printf("\r\n");
}


void SystemReport::report_cpu_stats()
{
    printf("================= CPU STATS =================\r\n");

    // Collect and print cpu stats
    mbed_stats_cpu_get(&cpu_stats);

    printf("Uptime %-20lld Idle %-20lld Sleep %-20lld Deep Sleep %-20lld\r\n",
      cpu_stats.uptime, cpu_stats.idle_time, cpu_stats.sleep_time, cpu_stats.deep_sleep_time);

    uint64_t diff = (cpu_stats.idle_time - prev_idle_time);
    uint8_t idle = (diff * 100) / (sample_time_ms * 1000);  // usec;
    uint8_t usage = 100 - idle;  // usec;
    prev_idle_time = cpu_stats.idle_time;

    printf("Idle: %d%% Usage: %d%% \r\n", idle, usage);
}

void SystemReport::report_heap_stats()
{
    printf("================ HEAP STATS =================\r\n");

    // Collect and print heap stats
    mbed_stats_heap_get(&heap_stats);

    printf("Current heap: %lu\r\n", heap_stats.current_size);
    printf("Max heap size: %lu\r\n", heap_stats.max_size);
}

void SystemReport::report_thread_stats()
{
    printf("================ THREAD STATS ===============\r\n");

    // Collect and print running thread stats
    int count = mbed_stats_thread_get_each(thread_stats, max_thread_count);

    for (int i = 0; i < count; i++) {
        printf("ID: 0x%lx \r\n",        thread_stats[i].id);
        printf("Name: %s \r\n",         thread_stats[i].name);
        printf("State: %ld \r\n",       thread_stats[i].state);
        printf("Priority: %ld \r\n",    thread_stats[i].priority);
        printf("Stack Size: %ld \r\n",  thread_stats[i].stack_size);
        printf("Stack Space: %ld \r\n", thread_stats[i].stack_space);
    }
}
