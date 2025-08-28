#pragma once

#include <babrat/wal/default/wal_default.hpp>

namespace babrat {

    namespace wal {

        class WalFactory final {

        public:

            static std::unique_ptr<WAL> create();

            static std::unique_ptr<WAL> create(
                const std::string& path);

            static std::unique_ptr<WAL> create(
                const std::string& path,
                const Options& options);

        };

    } // namespace wal

} // namespace babrat

