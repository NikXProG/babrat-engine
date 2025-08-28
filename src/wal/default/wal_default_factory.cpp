
#include <memory>

#include <babrat/wal/default/wal_default_factory.hpp>

namespace babrat {

    namespace wal {

        std::unique_ptr<WAL> WalFactory::create()
        {
            return std::make_unique<WALManager>(
                DEFAULT_DIRECTORY,
                WALOptions{});
        }

        std::unique_ptr<WAL> WalFactory::create(
            const std::string& path)
        {
            return std::make_unique<WALManager>(
                path,
                WALOptions{});
        }

        std::unique_ptr<WAL> WalFactory::create(
            const std::string& path,
            const WALOptions& options)
        {
            return std::make_unique<WALManager>(
                path,
                options);
        }

    } // namespace wal

} // namespace babrat