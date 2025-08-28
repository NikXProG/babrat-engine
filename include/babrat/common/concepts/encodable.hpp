#pragma once

#pragma once

#include <babrat/common/types/string.hpp>
#include <type_traits>

namespace babrat {

    //===--------------------------------------------------------------------===//
    // Base template for serialize
    //===--------------------------------------------------------------------===//

    class Jsonable;

    // checks if the type inherits encodable
    template<typename T>
    constexpr bool is_encodable_v = std::is_base_of_v<Jsonable, std::remove_cvref_t<T>>;

    // if type T really inherits from encodable then...
    template<typename T>
    using only_if_jsonable = std::enable_if_t<is_encodable_v<T>>;

} // namespace babrat
