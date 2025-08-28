#pragma once

#include <babrat/planner/logical_component.hpp>
#include <babrat/common/types/unique_ptr.hpp>
#include <babrat/common/types/vector.hpp>

namespace babrat {

    namespace planner {

        enum LogicalOperatorType {
            Create,
            CreateDatabase,
            CreateSchema,
            CreateTable,
            CreateIndex,
            Select,
            Delete,
            Join,
            Aggregate,

        };

        class LogicalOperator {

        private:

            LogicalOperatorType m_type [[maybe_unused]];

            vector<unique_ptr<LogicalComponent>> m_components;

        public:

            explicit LogicalOperator(LogicalOperatorType type) : m_type(type) {}

        public:

            LogicalOperator(LogicalOperatorType type, vector<unique_ptr<LogicalComponent>> components) :
                    m_type(type),
                    m_components(std::move(components)) {}

        public:

            virtual ~LogicalOperator() noexcept = default;

        };


        class LogicalProject : public LogicalOperator {


        public:




        };

        class LogicalFilter : public LogicalOperator {



        };


    } // namespace planner

} // namespace babrat