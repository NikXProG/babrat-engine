#pragma once

#include "../babrat_exception.h"

namespace babrat {

    class BabratStorageError : public BabratException {

    public:

        explicit BabratStorageError(const std::string& message);

    };

}

