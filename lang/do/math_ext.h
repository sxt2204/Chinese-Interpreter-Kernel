#ifndef MATH_EXT_H
#define MATH_EXT_H

#include "../../source/interpreter/native_registry.h"
#include <cmath>
#include <algorithm>

inline double do_sqrt(double x) { return std::sqrt(x); }
inline double do_abs(double x) { return std::abs(x); }
inline double do_ceil(double x) { return std::ceil(x); }
inline double do_floor(double x) { return std::floor(x); }
inline double do_round(double x) { return std::round(x); }
inline double do_max(double a, double b) { return std::max(a, b); }
inline double do_min(double a, double b) { return std::min(a, b); }
inline double do_sin(double x) { return std::sin(x); }
inline double do_cos(double x) { return std::cos(x); }
inline double do_tan(double x) { return std::tan(x); }
inline double do_log(double x) { return std::log10(x); }
inline double do_ln(double x) { return std::log(x); }

REGISTER_NATIVE_FUNC(do_sqrt, [](const auto& args) -> Value { return args.empty() ? 0.0 : do_sqrt(valueToDouble(args[0])); });
REGISTER_NATIVE_FUNC(sqrt, [](const auto& args) -> Value { return args.empty() ? 0.0 : do_sqrt(valueToDouble(args[0])); });
REGISTER_NATIVE_FUNC(do_abs, [](const auto& args) -> Value { return args.empty() ? 0.0 : do_abs(valueToDouble(args[0])); });
REGISTER_NATIVE_FUNC(abs, [](const auto& args) -> Value { return args.empty() ? 0.0 : do_abs(valueToDouble(args[0])); });
REGISTER_NATIVE_FUNC(do_ceil, [](const auto& args) -> Value { return args.empty() ? 0.0 : do_ceil(valueToDouble(args[0])); });
REGISTER_NATIVE_FUNC(ceil, [](const auto& args) -> Value { return args.empty() ? 0.0 : do_ceil(valueToDouble(args[0])); });
REGISTER_NATIVE_FUNC(do_floor, [](const auto& args) -> Value { return args.empty() ? 0.0 : do_floor(valueToDouble(args[0])); });
REGISTER_NATIVE_FUNC(floor, [](const auto& args) -> Value { return args.empty() ? 0.0 : do_floor(valueToDouble(args[0])); });
REGISTER_NATIVE_FUNC(do_round, [](const auto& args) -> Value { return args.empty() ? 0.0 : do_round(valueToDouble(args[0])); });
REGISTER_NATIVE_FUNC(round, [](const auto& args) -> Value { return args.empty() ? 0.0 : do_round(valueToDouble(args[0])); });
REGISTER_NATIVE_FUNC(do_max, [](const auto& args) -> Value { return args.size() < 2 ? 0.0 : do_max(valueToDouble(args[0]), valueToDouble(args[1])); });
REGISTER_NATIVE_FUNC(max, [](const auto& args) -> Value { return args.size() < 2 ? 0.0 : do_max(valueToDouble(args[0]), valueToDouble(args[1])); });
REGISTER_NATIVE_FUNC(do_min, [](const auto& args) -> Value { return args.size() < 2 ? 0.0 : do_min(valueToDouble(args[0]), valueToDouble(args[1])); });
REGISTER_NATIVE_FUNC(min, [](const auto& args) -> Value { return args.size() < 2 ? 0.0 : do_min(valueToDouble(args[0]), valueToDouble(args[1])); });
REGISTER_NATIVE_FUNC(do_sin, [](const auto& args) -> Value { return args.empty() ? 0.0 : do_sin(valueToDouble(args[0])); });
REGISTER_NATIVE_FUNC(sin, [](const auto& args) -> Value { return args.empty() ? 0.0 : do_sin(valueToDouble(args[0])); });
REGISTER_NATIVE_FUNC(do_cos, [](const auto& args) -> Value { return args.empty() ? 0.0 : do_cos(valueToDouble(args[0])); });
REGISTER_NATIVE_FUNC(cos, [](const auto& args) -> Value { return args.empty() ? 0.0 : do_cos(valueToDouble(args[0])); });
REGISTER_NATIVE_FUNC(do_tan, [](const auto& args) -> Value { return args.empty() ? 0.0 : do_tan(valueToDouble(args[0])); });
REGISTER_NATIVE_FUNC(tan, [](const auto& args) -> Value { return args.empty() ? 0.0 : do_tan(valueToDouble(args[0])); });

#endif
