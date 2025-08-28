#pragma once

#include "babrat/export_shared.h"

#include <babrat/wal/default/wal_default.hpp>

namespace babrat {

    namespace wal {

        class WalFactory final {

        public:

            BABRAT_API static std::unique_ptr<WAL> create();

            BABRAT_API static std::unique_ptr<WAL> create(
                const std::string& path);

            BABRAT_API static std::unique_ptr<WAL> create(
                const std::string& path,
                const WALOptions& options);

        };

    } // namespace wal

} // namespace babrat

