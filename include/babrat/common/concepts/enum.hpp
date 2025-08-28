#pragma once

#include <babrat/common/types/string.hpp>
#include <type_traits>

namespace babrat {

    //===--------------------------------------------------------------------===//
    // Base template for serialize
    //===--------------------------------------------------------------------===//

    // checks if the type enum
    template<typename T>
    constexpr bool is_enum_v =
            std::is_enum_v<T> &&
            (std::is_same_v<std::underlying_type_t<T>, int32_t> ||
             std::is_same_v<std::underlying_type_t<T>, uint32_t>);

    // if type T really enum then...
    template<typename T>
    using only_if_enum_v =
            std::enable_if_t<is_enum_v<T> && !std::is_convertible_v<T, std::underlying_type_t<T>>, bool>;

} // namespace babrat