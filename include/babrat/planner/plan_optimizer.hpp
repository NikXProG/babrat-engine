#pragma once


namespace babrat {

    namespace planner {

        class PlanOptimizer {

        public:

            virtual ~PlanOptimizer() noexcept = default;

        public:

            virtual void setRelation(RelNode root);

        public:

            virtual RelNode findBestPlan() = 0;

        };

    } // namespace planner

} // namespace babrat
