#pragma once

#include <string>
#include <babrat/common/throw_base.hpp>

namespace babrat {

    class Error : public ThrowBase {

    public:

        enum class ErrorType : uint8_t {
            IO_ERROR,
            FATAL_ERROR,
            CONFIG_ERROR,
            FACTORY_ERROR,
            INTERNAL_ERROR
        };

    private:

        ErrorType m_type;

    public:

        BABRAT_API ErrorType getType() const;

    public:

        BABRAT_API explicit Error(
            ErrorType type,
            const std::string& error_message);

    };

    class FatalError : public Error
    {

    public:

        BABRAT_API explicit FatalError(
            const std::string& message);

    };

    class ConfigError : public Error
    {

    public:

        BABRAT_API explicit ConfigError(
            const std::string& message);

    };

    class IOError : public Error
    {

    public:

        BABRAT_API explicit IOError(
            const std::string& message);

    };

    class FactoryError
    {

    public:

        BABRAT_API explicit FactoryError(
            const std::string& message);

    };

    class InternalError final : public Error
    {

    public:

        BABRAT_API explicit InternalError(
            const std::string& message);

    };



} // namespace babrat
