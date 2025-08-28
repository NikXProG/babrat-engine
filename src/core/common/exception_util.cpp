
#include "exception_util.h"
#include "unordered_map"

namespace babrat {

    static const std::unordered_map<BabratException::ExceptionType, std::string> TYPE_MAP = {
            {BabratException::ExceptionType::CATALOG_ERROR,      "CatalogError"},
            {BabratException::ExceptionType::WAL_ERROR,          "WalError"},
            {BabratException::ExceptionType::OUT_OF_RANGE_ERROR, "OutOfRangeError"},
            {BabratException::ExceptionType::CONVERSION_ERROR,   "ConversionError"},
            {BabratException::ExceptionType::UNKNOWN_TYPE_ERROR, "UnknownTypeError"},
            {BabratException::ExceptionType::INVALID_ERROR,      "InvalidError"},
            {BabratException::ExceptionType::STORAGE_ERROR,      "StorageError"},
            {BabratException::ExceptionType::SERIALIZE_ERROR,    "SerializeError"},
            {BabratException::ExceptionType::CONNECTION_ERROR,   "ConnectionError"},
            {BabratException::ExceptionType::CONTAINER_ERROR,    "ContainerError"},
            {BabratException::ExceptionType::FATAL_ERROR,       "FatalError"}
    };

    std::string exceptionTypeToString(babrat::BabratException::ExceptionType type) {
        auto it = TYPE_MAP.find(type);
        return it != TYPE_MAP.end() ? it->second : "UnknownError";
    }

    BabratException::ExceptionType stringToExceptionType(const std::string &type) {
        for(const auto& i : TYPE_MAP){
            if (i.second == type){
                return i.first;
            }
        }
        return  BabratException::ExceptionType::INVALID_ERROR;

    }


}

