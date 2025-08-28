#pragma once

#include <stdexcept>
#include <babrat/common/types/string.hpp>
#include <babrat/common/babrat_api.hpp>

namespace babrat {

    /**
     * @brief The Throwable class is the superclass of all errors and exceptions
     *
     * Instances of two subclasses, Error and Exception, are used
     * to indicate that exceptional situations have occurred.
     *
     * instance CANNOT be created. Made protected by default for inherited classes
     */
    class ThrowBase : public std::runtime_error {

    private:

        string message;

    protected:

        BABRAT_API explicit ThrowBase(const string& message);

    };

} // namespace babrat
