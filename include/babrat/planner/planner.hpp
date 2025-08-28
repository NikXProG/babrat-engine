#pragma once

#include <babrat/common/types/unique_ptr.hpp>
#include <babrat/planner/logical_operator.hpp>
#include <babrat/planner/binder.hpp>
#include <babrat/planner/binder_factory.hpp>

namespace babrat {

    namespace statement {
        class SqlStatement;
    }

    namespace planner {

        class Planner {

        public:

            shared_ptr<Binder> binder;

            unique_ptr<LogicalOperator> logicalPlan;

            ClientContext &context;

        public:

            virtual ~Planner() noexcept = default;

        public:

            explicit Planner(ClientContext &context) :
                binder(BinderFactory::create(context)),
                context(context) {}

        public:

            unique_ptr<statement::SqlStatement> validate(
                    unique_ptr<statement::SqlStatement> sqlStatement) {
                binder->bind(*sqlStatement);
                return sqlStatement;
            }

        public:

            void plan(
                    unique_ptr<statement::SqlStatement> sqlStatement){
                std::cout << "CREATE PLAN: " << sqlStatement->query << std::endl;
            }



        };



    } // namespace planner

} // namespace babrat