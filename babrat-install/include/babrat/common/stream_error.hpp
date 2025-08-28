#pragma once

#include <babrat/common/error.hpp>

namespace babrat {

    class StreamError final : public Error
    {

    public:

        explicit StreamError(
            const std::string& message);

    };

} // namespace babrat

