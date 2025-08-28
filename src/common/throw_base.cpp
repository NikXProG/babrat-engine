#include <babrat/common/throw_base.hpp>

namespace babrat {

    ThrowBase::ThrowBase(
            const std::string& message) :
        std::runtime_error(message) {}

} // namespace babrat