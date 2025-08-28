#pragma once

#include <babrat/common/types/string.hpp>
#include <type_traits>

namespace babrat {

    //===--------------------------------------------------------------------===//
    // Base template for serialize
    //===--------------------------------------------------------------------===//

    class Serializable;

    // checks if the type inherits Serializable
    template<typename T>
    constexpr bool is_serializable_v = std::is_base_of_v<Serializable, std::remove_cvref_t<T>>;

    // if type T really inherits from serializable then...
    template<typename T>
    using only_if_serializable = std::enable_if_t<is_serializable_v<T>>;

} // namespace babrat