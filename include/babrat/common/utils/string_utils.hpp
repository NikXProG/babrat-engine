#pragma once

#include <algorithm>
#include <fmt/core.h>
#include <babrat/common/types/string.hpp>

namespace babrat {

    namespace utils {

        template <typename... Args>
        string format(const string& msg, Args... args)
        {
            return fmt::format(
                    fmt::runtime(msg),
                    std::forward<Args>(args)...);
        }

        inline string toLower(const string& str) {
            string result;
            result.reserve(str.size());
            std::ranges::transform(str, std::back_inserter(result),
                [](const unsigned char c) { return std::tolower(c); });
            return result;
        }

        inline std::uint32_t hashIgnoreCase(const string& str) {

            uint32_t hash = 0;

            for (const char c : str) {
                const auto lower_c = static_cast<uint32_t>(std::tolower(c));
                hash += lower_c;
                hash += hash << 10;
                hash ^= hash >> 6;
            }

            hash += hash << 3;
            hash ^= hash >> 11;
            hash += hash << 15;

            return hash;
        }


        inline bool equalsIgnoreCase(const string& str1, const string& str2)
        {
            const size_t firstStrSize = str1.size();

            if (firstStrSize != str2.size()) {
                return false;
            }

            for (size_t i = 0; i < firstStrSize; ++i) {

                // Quick check: if the characters are already equal
                if (str1[i] == str2[i]) continue;

                if (std::tolower(static_cast<unsigned char>(str1[i])) !=
                    std::tolower(static_cast<unsigned char>(str2[i]))) {
                    return false;
                }

            }
            return true;
        }

        inline bool lessThanIgnoreCase(const string &s1, const string &s2) {

            auto it1 = s1.begin();
            auto it2 = s2.begin();
            auto end1 = s1.end();
            auto end2 = s2.end();

            while (it1 != end1 && it2 != end2) {
                const auto c1 = static_cast<unsigned char>(*it1);
                const auto c2 = static_cast<unsigned char>(*it2);

                if (c1 != c2) {
                    const auto upper1 = static_cast<unsigned char>(std::toupper(c1));
                    const auto upper2 = static_cast<unsigned char>(std::toupper(c2));

                    if (upper1 != upper2) {
                        return upper1 < upper2;
                    }
                }

                ++it1;
                ++it2;
            }

            return s1.length() < s2.length();
        }


    } // namespace utils

} // namespace babrat