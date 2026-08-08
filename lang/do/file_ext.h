#ifndef FILE_EXT_H
#define FILE_EXT_H

#include "../../source/interpreter/native_registry.h"
#include <fstream>
#include <sstream>
#include <string>
#include <filesystem>

inline std::string do_read_file(const std::string& path) {
    std::ifstream file(path);
    if (!file.is_open()) return "";
    std::ostringstream buf;
    buf << file.rdbuf();
    return buf.str();
}

inline double do_write_file(const std::string& path, const std::string& content) {
    std::ofstream file(path);
    if (!file.is_open()) return 0.0;
    file << content;
    return 1.0;
}

inline double do_append_file(const std::string& path, const std::string& content) {
    std::ofstream file(path, std::ios::app);
    if (!file.is_open()) return 0.0;
    file << content;
    return 1.0;
}

inline double do_file_exists(const std::string& path) {
    return std::filesystem::exists(path) ? 1.0 : 0.0;
}

REGISTER_NATIVE_FUNC(do_read_file, [](const std::vector<Value>& args) -> Value {
    if (args.empty()) return "";
    return do_read_file(valueToString(args[0]));
});

REGISTER_NATIVE_FUNC(do_write_file, [](const std::vector<Value>& args) -> Value {
    if (args.size() < 2) return 0.0;
    return do_write_file(valueToString(args[0]), valueToString(args[1]));
});

REGISTER_NATIVE_FUNC(do_append_file, [](const std::vector<Value>& args) -> Value {
    if (args.size() < 2) return 0.0;
    return do_append_file(valueToString(args[0]), valueToString(args[1]));
});

REGISTER_NATIVE_FUNC(do_file_exists, [](const std::vector<Value>& args) -> Value {
    if (args.empty()) return 0.0;
    return do_file_exists(valueToString(args[0]));
});

#endif
