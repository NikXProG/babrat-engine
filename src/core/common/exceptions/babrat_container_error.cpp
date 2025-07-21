#include "babrat_container_error.h"

namespace babrat {

    BabratContainerError::BabratContainerError(const std::string &message) :
        BabratException(ExceptionType::CONTAINER_ERROR, message) {}

}
