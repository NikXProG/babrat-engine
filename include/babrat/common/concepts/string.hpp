#pragma once

#include <babrat/common/types/string.hpp>
#include <type_traits>

namespace babrat {

    //===--------------------------------------------------------------------===//
    // Base template for serialize
    //===--------------------------------------------------------------------===//

    // checks if the type inherits string
    template<typename T>
    constexpr bool is_string_v = std::is_same_v<std::remove_cv_t<std::remove_reference_t<T>>, string>;

    // if type T really inherits from string then...
    template<typename T>
    using only_if_string = std::enable_if_t<is_string_v<T>>;

} // namespace babrat