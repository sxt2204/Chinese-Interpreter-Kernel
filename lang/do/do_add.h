#ifndef DO_ADD_H
#define DO_ADD_H

#include "../../source/interpreter/native_registry.h"

inline double do_add(double a, double b) {
    return a + b;
}

REGISTER_NATIVE_FUNC(do_add, [](const std::vector<Value>& args) -> Value {
    if (args.size() < 2) return 0.0;
    return do_add(valueToDouble(args[0]), valueToDouble(args[1]));
});

REGISTER_NATIVE_FUNC(add, [](const std::vector<Value>& args) -> Value {
    if (args.size() < 2) return 0.0;
    return do_add(valueToDouble(args[0]), valueToDouble(args[1]));
});

#endif
