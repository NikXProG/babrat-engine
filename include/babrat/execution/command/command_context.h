#pragma once
/*
#include "database_storage.h"
#include "catalog_default.h"*/

namespace babrat {

    namespace execution {

        class CommandContext {

        public:

            virtual ~CommandContext() noexcept = default;

            /*
            public:

                virtual std::shared_ptr<ICatalog> getCatalog() const = 0;
                virtual std::shared_ptr<TypeStorage> getStorage() const = 0;
            */

        };

    } // namespace execution


} // namespace babrat
