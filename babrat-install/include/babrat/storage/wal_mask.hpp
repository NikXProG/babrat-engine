#pragma once
#include <fmt/format.h>
#include <babrat/interfaces/encodable.hpp>
#include <boost/beast/core/detail/base64.hpp>

namespace babrat {

    namespace storage {


        enum Operation {
            INSERT,
            DELETE,
            UPDATE
        };


        class WALMask : public Encodable {


        public:

            WALMask() = default;

            explicit WALMask(
                    std::uint64_t timestamp,
                    Operation type,
                    std::string data);

        public:

            ~WALMask() noexcept override = default;

        public:

            Operation getType() const;

            const std::string &getData() const;

            std::uint64_t getTimestamp() const;

        public:

            std::vector<uint8_t> encode() const override;

            void decode(const std::vector<uint8_t> &encoded) override;

        private:

            Operation m_type;
            std::string m_data;
            std::uint64_t m_timestamp;

        };
    }
}