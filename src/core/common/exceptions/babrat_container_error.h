#pragma once

#include "../babrat_exception.h"

namespace babrat{


    class BabratContainerError : public BabratException{

    public:

        explicit BabratContainerError(const std::string& message);

    };

}
