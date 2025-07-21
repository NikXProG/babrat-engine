#pragma once

#include <string>

namespace babrat {

    namespace db {

        class DbConnect final {

        };


        class IDbController {

        public:

            virtual const std::string& getDatabase() const = 0;

        };


    }

}
