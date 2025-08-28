#pragma once

#include <cstdint>
#include <fmt/format.h>

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
            INVALID_EXCEPTION = 0, // default type exception
            STORAGE_EXCEPTION = 1, // database storage type exception
            WAL_EXCEPTION = 2, // database wal type exception
            CATALOG_EXCEPTION = 3, // database catalog type exception
            CONVERSION_EXCEPTION = 4, // exception type for failed convert
            SERIALIZE_EXCEPTION = 5, // exception type for serializable objects (needed for persistence)
            CONNECTION_EXCEPTION = 6, // exception type related to db connection
            UNKNOWN_TYPE_EXCEPTION = 7, // unsupported/unknown data types
            OUT_OF_RANGE_EXCEPTION = 8, // out of range exception
            CONTAINER_EXCEPTION = 9 // database container type exception
        };

    private:

        ExceptionType m_type;

    public:

        BABRAT_API ExceptionType getType() const;

    public:

        BABRAT_API explicit Exception(
                ExceptionType type,
                const string& message);

    private:

        template <typename... Args>
        static string formatMessage(const string& msg, Args... args)
        {
            const std::size_t num_args = sizeof...(Args);

            // just return message if argument size is 0
            if (num_args == 0) {
                return msg;
            }

            try
            {
                // fmt library itself binds the format of args in runtime
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

    /**
     * @brief Thrown when storage operations fail.
     *
     * Covers errors related to:
     * - File I/O (read/write failures)
     * - Disk space issues
     * - Memory allocation errors
     * - Data corruption
     */
    class StorageException : public Exception {

    public:

        BABRAT_API explicit StorageException(const string& message);

        template <typename... Args>
        explicit StorageException(const string &format, Args &&...args) :
            StorageException(formatMessage(format, std::forward<Args>(args)...)) {}

    };

    /**
     * @brief Thrown when key-value container operations fail.
     *
     * Common cases:
     * - Key not found (lookup failure)
     * - Duplicate key (insertion conflict)
     * - Container capacity exceeded
     * - Structural corruption (e.g., broken B-tree node)
     */
    class ContainerException : public Exception {

    public:

        BABRAT_API explicit ContainerException(const string& message);

        template <typename... Args>
        explicit ContainerException(const string &format, Args &&...args) :
            ContainerException(formatMessage(format, std::forward<Args>(args)...)) {}

    };

    /**
     * @brief Thrown for write-ahead log (WAL) related failures.
     *
     * Covers:
     * - WAL corruption
     * - Invalid log entries
     * - WAL file I/O errors
     */
    class WalException : public Exception {

    public:

        BABRAT_API explicit WalException(const string& message);

        template <typename... Args>
        explicit WalException(const string &format, Args &&...args) :
            WalException(formatMessage(format, std::forward<Args>(args)...)) {}

    };

    /**
     * @brief Thrown for database catalog operations failures.
     */
    class CatalogException : public Exception {

    public:

        BABRAT_API explicit CatalogException(const string& message);

        template <typename... Args>
        explicit CatalogException(const string &format, Args &&...args) :
            CatalogException(formatMessage(format, std::forward<Args>(args)...)) {}

    };

    /**
     * @brief Thrown for data type conversion failures.
     *
     * Common cases:
     * - Invalid type casting
     */
    class ConversionException : public Exception {

    public:

        BABRAT_API explicit ConversionException(const string& message);

        template <typename... Args>
        explicit ConversionException(const string &format, Args &&...args) :
            ConversionException(formatMessage(format, std::forward<Args>(args)...)) {}

    };

    /**
     * @brief Thrown for data serialization/deserialization failures.
     *
     * Covers:
     * - Invalid serialized formats
     */
    class SerializeException : public Exception {

    public:

        BABRAT_API explicit SerializeException(const string& message);

        template <typename... Args>
        explicit SerializeException(const string &format, Args &&...args) :
            SerializeException(formatMessage(format, std::forward<Args>(args)...)) {}

    };

    /**
     * @brief Thrown for database connection failures.
     *
     * Includes:
     * - Connection timeouts
     * - Authentication failures
     */
    class ConnectionException : public Exception {

    public:

        BABRAT_API explicit ConnectionException(const string& message);

        template <typename... Args>
        explicit ConnectionException(const string &format, Args &&...args) :
            ConnectionException(formatMessage(format, std::forward<Args>(args)...)) {}

    };

    /**
     * @brief Thrown when encountering unsupported/unknown data types.
     *
     * Typical cases:
     * - Unregistered type IDs
     */
    class UnknownTypeException : public Exception {

    public:

        BABRAT_API explicit UnknownTypeException(const string& message);

        template <typename... Args>
        explicit UnknownTypeException(const string &format, Args &&...args) :
            UnknownTypeException(formatMessage(format, std::forward<Args>(args)...)) {}

    };

    /**
     * @brief Thrown for boundary violation errors.
     */
    class OutOfRangeException : public Exception {

    public:

        BABRAT_API explicit OutOfRangeException(const string& message);

        template <typename... Args>
        explicit OutOfRangeException(const string &format, Args &&...args) :
            OutOfRangeException(formatMessage(format, std::forward<Args>(args)...)) {}

    };

} // namespace babrat

