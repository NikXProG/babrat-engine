#include "babrat_fatal_error.h"
namespace babrat {


    BabratFatalError::BabratFatalError(
            const std::string& error_message) :
            BabratException::BabratException(
                    ExceptionType::FATAL_ERROR,
                    error_message){}


}