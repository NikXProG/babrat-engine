#pragma once
#include "babrat/storage/db_enums.hpp"

#include <string>

namespace babrat {

    namespace storage {

        template <typename Enum>
        struct EnumTraits;

        template<typename Enum>
        struct EnumEntry {
            Enum value;
            std::string_view name;
        };

        template <typename Enum>
            constexpr std::string_view EnumToString(Enum value) {
            for (const auto& entry : EnumTraits<Enum>::entries) {
                if (entry.value == value) {
                    return entry.name;
                }
            }
            throw std::invalid_argument("Invalid enum value");
        }

        template <typename Enum>
        constexpr Enum StringToEnum(std::string_view name) {

            for (const auto& entry : EnumTraits<Enum>::entries) {
                if (entry.name == name) {
                    return entry.value;
                }
            }
            throw std::invalid_argument("Invalid enum name");
        }



        template <>
        struct EnumTraits<WalOperationType> {

            static constexpr std::array<EnumEntry<WalOperationType>, 4> entries = {
                EnumEntry{WalOperationType::INVALID, "0"}, // invalid
                EnumEntry{WalOperationType::INSERT, "i"}, // insert
                EnumEntry{WalOperationType::DELETE,"d"}, // delete
                EnumEntry{WalOperationType::UPDATE,"u"} // update
            };

            static constexpr std::string_view name = "WalOperationType";

        };


        template <>
          struct EnumTraits<WalLevelType> {

            static constexpr std::array<EnumEntry<WalLevelType>, 5> entries = {
                EnumEntry{WalLevelType::INVALID, "0"}, // invalid
                EnumEntry{WalLevelType::POOL, "p"}, // pool
                EnumEntry{WalLevelType::SCHEMA, "s"}, // schema
                EnumEntry{WalLevelType::COLLECTION, "c"}, // collection
                EnumEntry{WalLevelType::OBJECT, "o"} // object
            };

            static constexpr std::string_view name = "WalOperationType";

        };

    } // namespace storage

} // namespace babrat