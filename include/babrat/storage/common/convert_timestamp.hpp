#pragma once

#include <chrono>

namespace babrat {

    namespace convert {

        inline std::chrono::system_clock::time_point uint64_to_timestamp(std::uint64_t timestamp)
        {
            return std::chrono::system_clock::time_point{
                std::chrono::system_clock::duration{timestamp}
            };
        }

        inline std::uint64_t timestamp_to_uint64(std::chrono::system_clock::time_point timestamp) {
            auto rep = std::chrono::duration_cast<std::chrono::milliseconds>(
                timestamp.time_since_epoch()).count();
            /*if (count < 0){
                throw std::runtime_error("Timestamp cannot be before epoch (1970-01-01)");
            }*/
            return static_cast<std::uint64_t>(rep);
        }

        inline std::uint64_t timestamp_now()
        {
            return timestamp_to_uint64(std::chrono::system_clock::now());
        }

    } // namespace storage

} // namespace babrat

