#pragma once

#include "../babrat_exception.h"

namespace babrat {

    class BabratFatalError : public BabratException {

    public:

        explicit BabratFatalError(const std::string& error_message);

    };

}
