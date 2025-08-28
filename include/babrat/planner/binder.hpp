#pragma once

#include <babrat/common/types/shared_ptr.hpp>
#include <babrat/common/babrat_api.hpp>
#include <babrat/main/client_context.hpp>
#include <babrat/statement/sql_statement.hpp>

namespace babrat {

    namespace planner {

        class Binder {

        public:

            /**
              * Client context for catalog entry
              */
            ClientContext &context;

        public:

            explicit Binder(ClientContext &context) : context(context) {

            }

        public:

            void bind(statement::SqlStatement &statement) {
                std::cout << "VALIDATE: " << statement.query << std::endl;
            }

        };

    }


} // namespace babrat