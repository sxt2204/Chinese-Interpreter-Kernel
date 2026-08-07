#ifndef CUSTOM_STRING_H
#define CUSTOM_STRING_H

#include <string>

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
};

#endif
