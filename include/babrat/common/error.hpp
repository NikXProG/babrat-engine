#pragma once

#include <cstdint>
#include <babrat/common/types/string.hpp>
#include <babrat/common/throw_base.hpp>

namespace babrat {

    /**
     * @brief Base class for all error types in the system.
     *
     * Provides categorization of errors through ErrorType enum.
     * All specific error classes inherit from this base.
     */
    class Error : public ThrowBase {

    public:

        enum class ErrorType : uint8_t {
            INVALID_ERROR = 0, // default or unknown type error
            IO_ERROR = 1, // input or output stream base error
            CONFIG_ERROR = 2, // invalid configuration
            INTERNAL_ERROR = 3, // internal system failure
            FATAL_ERROR = 4 // critical unrecoverable error
        };

    private:

        ErrorType m_type;

    public:

        BABRAT_API ErrorType getType() const;

    public:

        BABRAT_API explicit Error(
            ErrorType type,
            const string& error_message);

    };

    /**
     * @brief Thrown for input/output operation failures.
     *
     * Covers:
     * - File read/write errors
     * - Stream operation failures
     * - Device I/O problems
     */
    class IOError : public Error
    {

    public:

        BABRAT_API explicit IOError(
            const string& message);

    };

    /**
     * @brief Thrown for configuration-related errors.
     */
    class ConfigError : public Error
    {

    public:

        BABRAT_API explicit ConfigError(
            const string& message);

    };

    /**
     * @brief Thrown for unrecoverable critical errors.
     *
     * Indicates situations where:
     * - System cannot continue operation
     * - Data integrity is compromised
     * - Immediate termination required
     */
    class FatalError : public Error
    {

    public:

        BABRAT_API explicit FatalError(
            const string& message);

    };

    /**
     * @brief Thrown for internal system failures.
     *
     * Represents:
     * - Bugs in system implementation
     */
    class InternalError final : public Error
    {

    public:

        BABRAT_API explicit InternalError(
            const string& message);

    };



} // namespace babrat
