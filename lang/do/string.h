#ifndef CUSTOM_STRING_H
#define CUSTOM_STRING_H

#include <string>
#include <algorithm>
#include <cctype>

class StringObject {
public:
    std::string value;

    explicit StringObject(std::string str = "") : value(std::move(str)) {}

    double get_length() const {
        return static_cast<double>(value.length());
    }

    void append(char c) {
        value += c;
    }

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

    double contains(const std::string& sub) const {
        return (value.find(sub) != std::string::npos) ? 1.0 : 0.0;
    }
};

#endif
