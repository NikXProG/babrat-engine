#pragma once

#include <babrat/common/error.hpp>

namespace babrat {

    /**
     * @brief Thrown when stream operations fail unexpectedly.
     *
     * Represents serious stream-related errors including:
     * - Failed read/write operations
     * - Stream corruption detected
     * - Invalid stream state
     */
    class StreamError final : public IOError
    {

    public:

        BABRAT_API explicit StreamError(
            const std::string& message);

    };

} // namespace babrat

