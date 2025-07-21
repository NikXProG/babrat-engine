#include "babrat_storage_error.h"

namespace babrat {

    BabratStorageError::BabratStorageError(const std::string& message) :
            BabratException(ExceptionType::STORAGE_ERROR, message){}

}
