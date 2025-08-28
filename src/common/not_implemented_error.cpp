
#include <../../include/babrat/common/error/not_implemented_error.hpp>
#include <fmt/core.h>
#include "babrat/common/error.hpp"

namespace babrat {

    NotImplementedError::NotImplementedError(const std::string &sourceName) :
            FatalError(fmt::format("source `{}` not implemented.", sourceName)){}

} // namespace babrat
