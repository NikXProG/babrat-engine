#pragma once

#include <babrat/serialization/json_serializer.hpp>
#include <babrat/serialization/json_deserializer.hpp>

namespace babrat {

    namespace serialization {

    /**
      * serializes a value that must have a `serializeTo` method
      *
      * @note must have the signature for method serializeTo:
      * @code{.cpp}
      * void(serialization::Serializer::Serializer& ser)
      * @endcode
      *
      *
      * @param json
      * @param value
      * @param skipIfEmpty
      */
    template <typename T>
        void toJson(json& json, const T &value, bool skipIfEmpty) {
            JsonSerializer serializer(json, skipIfEmpty);
            value.serializeTo(serializer);
        }

    } // namespace serialization

} // namespace  babrat