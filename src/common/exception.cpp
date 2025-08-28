#include <babrat/common/exception.hpp>

namespace babrat {

    Exception::Exception(
            ExceptionType type,
            const std::string& message) :
                ThrowBase(message), m_type(type) {}

    Exception::ExceptionType Exception::getType() const
    {
        return m_type;
    }

    StorageException::StorageException(const std::string& message)
         : Exception(ExceptionType::STORAGE_EXCEPTION, message) {}

    WalException::WalException(const std::string& message)
        : Exception(ExceptionType::WAL_EXCEPTION, message) {}

    CatalogException::CatalogException(const std::string& message)
        : Exception(ExceptionType::CATALOG_EXCEPTION, message) {}

    ConversionException::ConversionException(const std::string& message)
        : Exception(ExceptionType::CONVERSION_EXCEPTION, message) {}

    SerializeException::SerializeException(const std::string& message)
        : Exception(ExceptionType::SERIALIZE_EXCEPTION, message) {}

    ConnectionException::ConnectionException(const std::string& message)
        : Exception(ExceptionType::CONNECTION_EXCEPTION, message) {}

    UnknownTypeException::UnknownTypeException(const std::string& message)
        : Exception(ExceptionType::UNKNOWN_TYPE_EXCEPTION, message) {}

    OutOfRangeException::OutOfRangeException(const std::string& message)
        : Exception(ExceptionType::OUT_OF_RANGE_EXCEPTION, message) {}

    ContainerException::ContainerException(const std::string& message)
        : Exception(ExceptionType::CONTAINER_EXCEPTION, message) {}

} // namespace babrat


