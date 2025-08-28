#pragma once

#include <string>
#include <format>

#include "babrat_exception.h"
#include "babrat_container_error.h"
#include "babrat_storage_error.h"


namespace babrat {

    namespace utils {

        std::string exceptionTypeToString(BabratException::ExceptionType type);

        BabratException::ExceptionType stringToExceptionType(const std::string& type);


    }



}
