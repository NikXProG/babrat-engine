#pragma once

#include <babrat/storage/database_storage.hpp>
#include <babrat/catalog/default_catalog.hpp>


namespace babrat {

    class DatabaseInstance;

    // class for abstracting types
    class DatabaseManager {

    private:

        std::unique_ptr<storage::DatabaseStorage<int,int>> m_databaseManager;

        std::unique_ptr<catalog::Catalog> m_systemCatalog;

        std::atomic<std::uint64_t> next_oid;

    public:

        explicit DatabaseManager() : m_databaseManager(
            std::make_unique<storage::DatabaseStorage<int,int>>(storage::DatabaseOptions{})) {}

    public:

        void getDatabaseType() const;

    public:

        void addPool(
            const std::string& poolName)
        {

            m_databaseManager->addPool(poolName);
        }

        void addSchema(
            const std::string& poolName,
            const std::string& schemaName)
        {
            m_databaseManager->addSchema(poolName, schemaName);
        }

        void addCollection(
            const std::string& poolName,
            const std::string& schemaName,
            const std::string& collectionName)
        {
            m_databaseManager->addCollection(poolName, schemaName, collectionName);
        }

        void add(
            const std::string& poolName,
            const std::string& schemaName,
            const std::string& collectionName,
            int key,
            int value)
        {
            m_databaseManager->add(poolName, schemaName, collectionName, key, value);
        }

    };

} // namespace babrat
