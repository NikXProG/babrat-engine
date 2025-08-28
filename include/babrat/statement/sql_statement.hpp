#pragma once

#include <babrat/common/types/string.hpp>

namespace babrat {

    namespace statement {

        //base sql model
        class SqlStatement {

        public:

            virtual ~SqlStatement() = default;

        public:

            string query;

        public:

            virtual string toString() const = 0;

        };

    }

} // namespace babrat
