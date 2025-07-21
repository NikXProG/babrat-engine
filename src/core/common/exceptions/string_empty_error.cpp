#include "string_empty_error.h"
#include <format>

namespace babrat {

    StringEmptyError::StringEmptyError(const std::string &sourceName) :
            BabratFatalError(
                    std::format("{} cannot be empty", sourceName)){}


}
