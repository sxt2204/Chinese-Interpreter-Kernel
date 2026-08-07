#ifndef IO_EXT_H
#define IO_EXT_H

#include <iostream>
#include <string>

inline void do_print_line(const std::string& text) { std::cout << text << std::endl; }
inline void do_output(const std::string& text) { std::cout << text; }
inline std::string do_input() { std::string l; std::getline(std::cin, l); return l; }

REGISTER_NATIVE_FUNC(do_print_line, [](const auto& args) -> Value {
    std::string line = "";
    for (const auto& a : args) line += valueToString(a);
    do_print_line(line);
    return 0.0;
});
REGISTER_NATIVE_FUNC(print_line, [](const auto& args) -> Value {
    std::string line = "";
    for (const auto& a : args) line += valueToString(a);
    do_print_line(line);
    return 0.0;
});
REGISTER_NATIVE_FUNC(do_output, [](const auto& args) -> Value {
    std::string text = "";
    for (const auto& a : args) text += valueToString(a);
    do_output(text);
    return 0.0;
});
REGISTER_NATIVE_FUNC(output, [](const auto& args) -> Value {
    std::string text = "";
    for (const auto& a : args) text += valueToString(a);
    do_output(text);
    return 0.0;
});
REGISTER_NATIVE_FUNC(do_input, [](const auto& args) -> Value { return do_input(); });
REGISTER_NATIVE_FUNC(input, [](const auto& args) -> Value { return do_input(); });

#endif
