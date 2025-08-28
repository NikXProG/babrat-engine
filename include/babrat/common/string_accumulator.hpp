#pragma once

#include <sstream>

class StringAccumulator {

private:

    std::ostringstream oss;

private:

    bool first = true;

public:

    explicit StringAccumulator(const std::string& className) {
        oss << className << "{";
    }

public:

    template <typename T>
    StringAccumulator& accumulate(std::string_view name, T&& value) {
        if (!first) oss << ", ";
        first = false;
        oss << name << "=" << std::forward<T>(value);
        return *this;
    }

    StringAccumulator& accumulate(std::string_view name, const char* value) {
        return accumulate(name, std::string_view(value));
    }

public:

    template <typename T>
    StringAccumulator& accumulateValue(T&& value) {
        if (!first) oss << ", ";
        first = false;
        oss << std::forward<T>(value);
        return *this;
    }

    StringAccumulator& accumulateValue(const char* value) {
        return accumulateValue(std::string_view(value));
    }

public:

    std::string str() const {
        return oss.str() + "}";
    }


};
