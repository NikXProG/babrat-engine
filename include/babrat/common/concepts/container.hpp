#pragma once

#include <babrat/common/types/string.hpp>
#include <type_traits>

namespace babrat {

    //===--------------------------------------------------------------------===//
    // Base template for serialize
    //===--------------------------------------------------------------------===//

    // basic concept for container type
    template<typename T>
    concept Container = !std::is_same_v<T, std::string> && requires(T a) {
        typename T::value_type;
        typename T::reference;
        typename T::const_reference;
        typename T::iterator;
        typename T::const_iterator;
        typename T::size_type;
        { a.begin() } -> std::same_as<typename T::iterator>;
        { a.end() } -> std::same_as<typename T::iterator>;
        { a.cbegin() } -> std::same_as<typename T::const_iterator>;
        { a.cend() } -> std::same_as<typename T::const_iterator>;
        { a.clear() } -> std::same_as<void>;
    };

    // concept containers
    template<typename T>
    concept SequenceContainer = Container<T> &&
                                requires(T a, typename T::size_type count) { a.resize(count); };

    template<typename T>
    concept AssociativeContainer = Container<T> &&
                                   requires { typename T::key_type; };


} // namespace babrat