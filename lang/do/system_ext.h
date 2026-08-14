#ifndef SYSTEM_EXT_H
#define SYSTEM_EXT_H

#include "../../source/interpreter/native_registry.h"
#include <ctime>
#include <thread>
#include <chrono>
#include <random>

inline double do_time_timestamp() {
    return static_cast<double>(std::time(nullptr));
}

inline void do_sleep_ms(double ms) {
    std::this_thread::sleep_for(std::chrono::milliseconds(static_cast<long long>(ms)));
}

inline double do_random_double(double minVal, double maxVal) {
    static std::mt19937 gen(std::random_device{}());
    std::uniform_real_distribution<double> dis(static_cast<double>(minVal), static_cast<double>(maxVal));
    return static_cast<double>(dis(gen));
}

inline double do_random_int(double minVal, double maxVal) {
    static std::mt19937 gen(std::random_device{}());
    std::uniform_int_distribution<int> dis(static_cast<int>(minVal), static_cast<int>(maxVal));
    return static_cast<double>(dis(gen));
}

REGISTER_NATIVE_FUNC(do_time_timestamp, [](const std::vector<Value>& args) -> Value {
    return do_time_timestamp();
});

REGISTER_NATIVE_FUNC(do_sleep_ms, [](const std::vector<Value>& args) -> Value {
    if (!args.empty()) do_sleep_ms(valueToDouble(args[0]));
    return 0.0;
});

REGISTER_NATIVE_FUNC(do_random_double, [](const std::vector<Value>& args) -> Value {
    if (args.size() < 2) return 0.0;
    return do_random_double(valueToDouble(args[0]), valueToDouble(args[1]));
});

#endif
