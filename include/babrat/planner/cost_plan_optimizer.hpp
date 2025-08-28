#pragma once

#include <babrat/planner/plan_optimizer.hpp>
#include <babrat/planner/rel_node.hpp>

namespace babrat {

    namespace planner {

        class CostPlanOptimizer : public PlanOptimizer {

        public:

            void setRelation(RelNode root) override {
                m_root = root;
            }

        public:

            RelNode findBestPlan() override {
                return RelNode{};
            }

        };

    } // namespace planner


} // namespace babrat