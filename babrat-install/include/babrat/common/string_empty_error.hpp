#pragma once

#include <babrat/common/error.hpp>

namespace babrat {

    class StringEmptyError final : public Error {

    public:

        explicit StringEmptyError(
            const std::string& sourceName);

    };

}

