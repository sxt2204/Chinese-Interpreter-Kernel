#ifndef NATIVE_REGISTRY_H
#define NATIVE_REGISTRY_H

#include <string>
#include <vector>
#include <variant>
#include <unordered_map>
#include <functional>
#include <iostream>

using Value = std::variant<double, std::string>;

inline std::string valueToString(const Value& val) {
    if (std::holds_alternative<std::string>(val)) return std::get<std::string>(val);
    std::string s = std::to_string(std::get<double>(val));
    if (s.find('.') != std::string::npos) {
        s.erase(s.find_last_not_of('0') + 1, std::string::npos);
        if (s.back() == '.') s.pop_back();
    }
    return s;
}

inline double valueToDouble(const Value& val) {
    if (std::holds_alternative<double>(val)) return std::get<double>(val);
    try {
        return std::stod(std::get<std::string>(val));
    } catch (...) {
        return 0.0;
    }
}

using NativeFunction = std::function<Value(const std::vector<Value>& args)>;

class NativeRegistry {
private:
    std::unordered_map<std::string, NativeFunction> funcs;
    NativeRegistry() = default;

public:
    static NativeRegistry& instance() {
        static NativeRegistry reg;
        return reg;
    }

    void registerFunc(const std::string& name, NativeFunction fn) {
        funcs[name] = std::move(fn);
    }

    bool hasFunc(const std::string& name) const {
        return funcs.find(name) != funcs.end();
    }

    Value call(const std::string& name, const std::vector<Value>& args) {
        auto it = funcs.find(name);
        if (it != funcs.end()) {
            return it->second(args);
        }
        std::cerr << "[ERROR] 原生 C++ 函数未找到: " << name << std::endl;
        return 0.0;
    }
};

struct NativeFunctionRegistrar {
    NativeFunctionRegistrar(const std::string& name, NativeFunction fn) {
        NativeRegistry::instance().registerFunc(name, fn);
    }
};

#define REGISTER_NATIVE_FUNC(name, fn) \
    static NativeFunctionRegistrar _registrar_##name(#name, fn);

#endif // NATIVE_REGISTRY_H
