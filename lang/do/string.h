#ifndef CUSTOM_STRING_H
#define CUSTOM_STRING_H

#include "../../source/interpreter/native_registry.h"
#include <algorithm>
#include <cctype>
#include <string>

class StringObject {
public:
  std::string value;

  explicit StringObject(std::string str = "") : value(std::move(str)) {}

  double get_length() const { return static_cast<double>(value.length()); }

  void append(char c) { value += c; }

  std::string to_upper() const {
    std::string res = value;
    std::transform(res.begin(), res.end(), res.begin(), ::toupper);
    return res;
  }

  std::string to_lower() const {
    std::string res = value;
    std::transform(res.begin(), res.end(), res.begin(), ::tolower);
    return res;
  }

  double contains(const std::string &sub) const {
    return (value.find(sub) != std::string::npos) ? 1.0 : 0.0;
  }
};

REGISTER_NATIVE_FUNC(string_get_length,
                     [](const std::vector<Value> &args) -> Value {
                       if (args.empty())
                         return 0.0;
                       std::string str = valueToString(args[0]);
                       return StringObject(str).get_length();
                     });

REGISTER_NATIVE_FUNC(string_length,
                     [](const std::vector<Value> &args) -> Value {
                       if (args.empty())
                         return 0.0;
                       std::string str = valueToString(args[0]);
                       return StringObject(str).get_length();
                     });

REGISTER_NATIVE_FUNC(string_to_upper,
                     [](const std::vector<Value> &args) -> Value {
                       if (args.empty())
                         return "";
                       std::string str = valueToString(args[0]);
                       return StringObject(str).to_upper();
                     });

REGISTER_NATIVE_FUNC(string_to_lower,
                     [](const std::vector<Value> &args) -> Value {
                       if (args.empty())
                         return "";
                       std::string str = valueToString(args[0]);
                       return StringObject(str).to_lower();
                     });

REGISTER_NATIVE_FUNC(string_append,
                     [](const std::vector<Value> &args) -> Value {
                       if (args.size() < 2)
                         return "";
                       std::string str = valueToString(args[0]);
                       std::string argStr = valueToString(args[1]);
                       StringObject obj(str);
                       if (!argStr.empty())
                         obj.append(argStr[0]);
                       return obj.value;
                     });

REGISTER_NATIVE_FUNC(string_contains,
                     [](const std::vector<Value> &args) -> Value {
                       if (args.size() < 2)
                         return 0.0;
                       std::string str = valueToString(args[0]);
                       std::string sub = valueToString(args[1]);
                       return StringObject(str).contains(sub);
                     });

#endif
