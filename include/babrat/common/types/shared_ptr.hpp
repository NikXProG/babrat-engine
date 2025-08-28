#pragma once

#include <memory>

namespace babrat {

    using std::shared_ptr;

    using std::make_shared;

    template<typename T>
    using enable_shared_from_this = std::enable_shared_from_this<T>;

} // namespace babrat
