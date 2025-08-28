#include <../../include/babrat/common/error/string_empty_error.hpp>
#include <fmt/core.h>

namespace babrat {

    StringEmptyError::StringEmptyError(const std::string &sourceName) :
            FatalError(fmt::format("{} cannot be empty", sourceName)) {}

} // namespace babrat
