#pragma once

#include <babrat/common/error.hpp>

namespace babrat {

    class NotImplementedError final : public Error
    {

    public:

        explicit NotImplementedError(
            const std::string& sourceName);

    };

} // namespace babrat
