#pragma once

#include <string>
#include <babrat/storage/db_enums.hpp>
#include <babrat/common/unknown_type_exception.hpp>

namespace babrat {

    namespace convert {

        const std::string& variantToString(storage::SearchTreeVariant variant) {

            switch (variant)
            {
                case storage::SearchTreeVariant::b_plus:
                    return "b_plus";
                case storage::SearchTreeVariant::red_black:
                    return "red_black";

            }

            throw std::runtime_error("Invalid SearchTreeVariant");

        }
    } // namespace convert

} // namespace babrat