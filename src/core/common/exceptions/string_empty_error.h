#pragma once
#include "babrat_fatal_error.h"


namespace babrat {

    class StringEmptyError : public BabratFatalError  {

    public:

        explicit StringEmptyError(const std::string& sourceName);

    };

}

