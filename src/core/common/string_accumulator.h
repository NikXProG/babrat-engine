
#ifndef BABRAT_STRING_ACCUMULATOR_H

#define BABRAT_STRING_ACCUMULATOR_H

#include <iostream>
#include <sstream>

class StringAccumulator {

    std::ostringstream oss;
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

#endif // BABRAT_STRING_ACCUMULATOR_H