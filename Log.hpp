#pragma once

#include <mbed.h>

extern Mutex s_stdio_mutex;

#define LOG(...) { mbed::ScopedLock<Mutex> lock(s_stdio_mutex); printf(__VA_ARGS__); } enum {}
