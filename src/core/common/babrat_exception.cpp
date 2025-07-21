#include "babrat_exception.h"

#include <unordered_map>
#include <format>

namespace babrat {

    BabratException::BabratException(
            BabratException::ExceptionType type,
            const std::string& error_message) :
                std::runtime_error(error_message), m_type(type) {}


}


