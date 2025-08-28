#pragma once

#include <string>

#include <babrat/common/error.hpp>

namespace babrat {

    /**
     * @brief Thrown when an empty string is encountered where it's not allowed.
     *
     * @param sourceName Name of the parameter/variable that was empty
     */
    class StringEmptyError final : public FatalError {

    public:

        BABRAT_API explicit StringEmptyError(
            const std::string& sourceName);

    };

} // namespace babrat

