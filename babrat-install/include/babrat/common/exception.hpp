#pragma once

#include <string>
#include <fmt/core.h>

#include <babrat/common/throw_base.hpp>
#include <babrat/common/error.hpp>

namespace babrat {

    /**
    * @brief Base exception class for database-related errors during runtime.
    *
    * This class serves as the foundation for all exceptions that may occur during
    * database operations. Derived exceptions should provide meaningful error messages
    * suitable for user communication.
    */
    class Exception : public ThrowBase {

    public:

        enum class ExceptionType : uint8_t {
            INVALID_EXCEPTION = 0,
            STORAGE_EXCEPTION = 1,
            WAL_EXCEPTION = 2,
            CATALOG_EXCEPTION = 3,
            CONVERSION_EXCEPTION = 4,
            SERIALIZE_EXCEPTION = 5,
            CONNECTION_EXCEPTION = 6,
            UNKNOWN_TYPE_EXCEPTION = 7,
            OUT_OF_RANGE_EXCEPTION = 8,
            CONTAINER_EXCEPTION = 9
        };

    private:

        ExceptionType m_type;

    public:

        BABRAT_API ExceptionType getType() const;

    public:

        BABRAT_API explicit Exception(
                ExceptionType type,
                const std::string& message);

    private:

        template <typename... Args>
        static std::string formatMessage(const std::string& msg, Args... args)
        {
            const std::size_t num_args = sizeof...(Args);

            if (num_args == 0) {
                return msg;
            }

            try
            {
                return fmt::format(
                    fmt::runtime(msg),
                    std::forward<Args>(args)...);

            } catch (std::exception &ex) {

                throw InternalError(fmt::format(
                    "Formatting failed for message template: \"{}\"\nError common: {}",
                      msg,
                      ex.what()));

            }

        }

    };

    class StorageException : public Exception {

    public:

        BABRAT_API explicit StorageException(const std::string& message);

        template <typename... Args>
        explicit StorageException(const std::string &format, Args &&...args) :
            StorageException(formatMessage(format, std::forward<Args>(args)...)) {}

    };

    class ContainerException : public Exception {

    public:

        BABRAT_API explicit ContainerException(const std::string& message);

        template <typename... Args>
        explicit ContainerException(const std::string &format, Args &&...args) :
            ContainerException(formatMessage(format, std::forward<Args>(args)...)) {}

    };

    class WalException : public Exception {

    public:

        BABRAT_API explicit WalException(const std::string& message);

        template <typename... Args>
        explicit WalException(const std::string &format, Args &&...args) :
            WalException(formatMessage(format, std::forward<Args>(args)...)) {}

    };

    class CatalogException : public Exception {

    public:

        BABRAT_API explicit CatalogException(const std::string& message);

        template <typename... Args>
        explicit CatalogException(const std::string &format, Args &&...args) :
            CatalogException(formatMessage(format, std::forward<Args>(args)...)) {}

    };

    class ConversionException : public Exception {

    public:

        BABRAT_API explicit ConversionException(const std::string& message);

        template <typename... Args>
        explicit ConversionException(const std::string &format, Args &&...args) :
            ConversionException(formatMessage(format, std::forward<Args>(args)...)) {}

    };

    class SerializeException : public Exception {

    public:

        BABRAT_API explicit SerializeException(const std::string& message);

        template <typename... Args>
        explicit SerializeException(const std::string &format, Args &&...args) :
            SerializeException(formatMessage(format, std::forward<Args>(args)...)) {}

    };

    class ConnectionException : public Exception {

    public:

        BABRAT_API explicit ConnectionException(const std::string& message);

        template <typename... Args>
        explicit ConnectionException(const std::string &format, Args &&...args) :
            ConnectionException(formatMessage(format, std::forward<Args>(args)...)) {}

    };

    class UnknownTypeException : public Exception {

    public:

        BABRAT_API explicit UnknownTypeException(const std::string& message);

        template <typename... Args>
        explicit UnknownTypeException(const std::string &format, Args &&...args) :
            UnknownTypeException(formatMessage(format, std::forward<Args>(args)...)) {}

    };

    class OutOfRangeException : public Exception {

    public:

        BABRAT_API explicit OutOfRangeException(const std::string& message);

        template <typename... Args>
        explicit OutOfRangeException(const std::string &format, Args &&...args) :
            OutOfRangeException(formatMessage(format, std::forward<Args>(args)...)) {}

    };

} // namespace babrat

