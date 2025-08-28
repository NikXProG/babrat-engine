#pragma once

#include <babrat/planner/binder.hpp>
#include <babrat/main/client_context.hpp>

namespace babrat {

    namespace planner {

        class BinderFactory {

        public:

            static std::shared_ptr<Binder> create(ClientContext& context) {
                return std::make_shared<Binder>(context);
            }

        };

    } // namespace binder

} // namespace babrat