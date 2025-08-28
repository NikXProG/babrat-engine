#pragma once

#include <stdexcept>
#include <string>
#include <utility>

namespace babrat {

    class BabratException : public std::runtime_error {

    public:

        enum class ExceptionType : uint8_t {
            INVALID_ERROR = 0,
            STORAGE_ERROR = 1,
            WAL_ERROR = 2,
            CATALOG_ERROR = 3,
            CONVERSION_ERROR = 4,
            SERIALIZE_ERROR = 5,
            CONNECTION_ERROR = 6,
            UNKNOWN_TYPE_ERROR = 7,
            OUT_OF_RANGE_ERROR = 8,
            CONTAINER_ERROR = 9,
            FATAL_ERROR = 10
        };

    private:

        ExceptionType m_type;

    public:

        ExceptionType getType() const { return m_type; }

    public:

        explicit BabratException(
                ExceptionType type,
                const std::string& message);

    };

}

