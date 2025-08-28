
#include "babrat/common/error.hpp"

namespace babrat {

    Error::Error(
            ErrorType type,
            const std::string& message) :
        ThrowBase(message), m_type(type) {}

    Error::ErrorType Error::getType() const
    {
        return m_type;
    }

    InternalError::InternalError(const std::string& message) :
        Error(ErrorType::INTERNAL_ERROR, message){}

    IOError::IOError(const std::string& message) :
        Error(ErrorType::IO_ERROR, message){}

    FatalError::FatalError(const std::string& message) :
        Error(ErrorType::FATAL_ERROR, message){}

    ConfigError::ConfigError(const std::string& message) :
        Error(ErrorType::CONFIG_ERROR, message){}


} // namespace babrat