#pragma once

#include <babrat/common/types/string.hpp>
#include "babrat/common/utils/string_utils.hpp"
#include <babrat/common/types/unordered_map.hpp>
#include <babrat/common/types/unordered_set.hpp>
#include <babrat/common/types/map.hpp>

namespace babrat {

    //===--------------------------------------------------------------------===//
    // container string with fast hash
    //===--------------------------------------------------------------------===//

    struct StringIgnoreCaseHashFunction {
        uint64_t operator()(const string &str) const {
            return utils::hashIgnoreCase(str);
        }
    };

    struct StringIgnoreCaseEquality {
        bool operator()(const string &a, const string &b) const {
            return utils::equalsIgnoreCase(a, b);
        }
    };

    struct StringIgnoreCaseCompare {
        bool operator()(const string &s1, const string &s2) const {
            return utils::lessThanIgnoreCase(s1, s2);
        }
    };

    template <typename T>
       using ignore_case_map_t =
           unordered_map<string, T, StringIgnoreCaseHashFunction, StringIgnoreCaseEquality>;

    using ignore_case_set_t =
        unordered_set<string, StringIgnoreCaseHashFunction, StringIgnoreCaseEquality>;

    template <typename T>
    using ignore_case_tree_t = map<string, T, StringIgnoreCaseCompare>;

} // namespace babrat