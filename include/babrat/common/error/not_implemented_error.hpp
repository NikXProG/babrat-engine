#pragma once

#include <string>

#include <babrat/common/error.hpp>

namespace babrat {

    /**
     * @brief Thrown when encountering unimplemented functionality.
     *
     * @param sourceName Name of the unimplemented method/feature
     *
     * @note This error should be replaced with actual implementation
     *       during development process.
     */
    class NotImplementedError final : public FatalError
    {

    public:

        BABRAT_API explicit NotImplementedError(
            const std::string& sourceName);

    };

} // namespace babrat
