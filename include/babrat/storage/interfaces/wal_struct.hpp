#pragma once


#include <babrat/storage/db_enums.hpp>

namespace babrat {

    namespace storage {

        class IWALStruct {

        public:

            virtual WalOperationType getType() const = 0;

            virtual const std::string &getData() const = 0;

            virtual std::uint64_t getTimestamp() const = 0;

        public:

            virtual void setType(WalOperationType type) = 0;

            virtual void setData(const std::string& data) = 0;

            virtual void setTimestamp(std::uint64_t timestamp) = 0;

            /*virtual void write(
                std::uint64_t timestamp,
                WalOperationType type,
                const std::vector<uint8_t>& data) = 0;*/

        public:

            virtual ~IWALStruct() noexcept = default;

        };

    } // namespace storage

} // namespace babrat