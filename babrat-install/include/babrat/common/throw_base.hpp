#pragma once

#include <babrat/export.hpp>
#include <string>

namespace babrat {

    class ThrowBase : public std::runtime_error {

    private:

        std::string message;

    protected:

        BABRAT_API explicit ThrowBase(const std::string& message);

    public:

        ~ThrowBase() override = default;

        void* operator new(std::size_t) = delete;

    };

} // namespace babrat
