#ifndef SYSTEM_EXT_H
#define SYSTEM_EXT_H

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

inline double do_random_int(double minVal, double maxVal) {
    static std::mt19937 gen(std::random_device{}());
    std::uniform_int_distribution<long long> dis(static_cast<long long>(minVal), static_cast<long long>(maxVal));
    return static_cast<double>(dis(gen));
}

REGISTER_NATIVE_FUNC(do_time_timestamp, [](const std::vector<Value>& args) -> Value {
    return do_time_timestamp();
});

REGISTER_NATIVE_FUNC(do_sleep_ms, [](const std::vector<Value>& args) -> Value {
    if (!args.empty()) do_sleep_ms(valueToDouble(args[0]));
    return 0.0;
});

REGISTER_NATIVE_FUNC(do_random_int, [](const std::vector<Value>& args) -> Value {
    if (args.size() < 2) return 0.0;
    return do_random_int(valueToDouble(args[0]), valueToDouble(args[1]));
});

#endif
