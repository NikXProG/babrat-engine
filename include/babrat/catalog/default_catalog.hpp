#pragma once

#include <babrat/catalog/catalog.hpp>

namespace babrat {

    namespace catalog {

        class Catalog : public ICatalog {

        public:

            ~Catalog() override = default;

        public:

            Catalog() {}

        };

    } // namespace catalog

} // namespace babrat