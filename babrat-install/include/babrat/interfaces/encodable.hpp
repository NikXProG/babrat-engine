#pragma once

#include <iostream>

namespace babrat {

    class Encodable {

    public:

        virtual std::vector<uint8_t> encode() const = 0;
        virtual void decode(const std::vector<uint8_t>& encoded) = 0;

    public:

        virtual ~Encodable() noexcept = default;

    };

} // namespace babrat


