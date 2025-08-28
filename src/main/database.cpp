#include "babrat/logger/logging.hpp"
#include "babrat/main/client_context.hpp"

#include <babrat/main/database.hpp>
#include <babrat/main/connection_manager.hpp>
#include <babrat/main/database_manager.hpp>
#include <babrat/logger/logger_manager.hpp>


namespace babrat {


    DatabaseInstance::DatabaseInstance() {
    }

    DatabaseInstance::~DatabaseInstance()
    {
        m_connectionManager.reset();
        m_loggerManager.reset();
        m_databaseManager.reset();
    }

    ConnectionManager& DatabaseInstance::getConnectionManager() {
        return *m_connectionManager;
    }

    logger::LoggerManager& DatabaseInstance::getLoggerManager() const {
        return *m_loggerManager;
    }

    DatabaseManager& DatabaseInstance::getDatabaseManager() {
        if (!m_databaseManager) {
            throw std::runtime_error("Missing DB manager");
        }
        return *m_databaseManager;
    }


    void DatabaseInstance::init(const char* path [[maybe_unused]])
    {
        m_databaseManager = std::make_unique<DatabaseManager>();
        m_connectionManager = std::make_unique<ConnectionManager>();

        auto config = logger::LoggerConfig();
        config.level = logger::LogLevel::Debug;

        m_loggerManager = std::make_shared<logger::LoggerManager>(
              BABRAT_LOG_NAME, config);

        BABRAT_LOG(*this, logger::DatabaseContextType, "database instance initialize");

    }

    Database::Database(DatabaseInstance& db) : instance(db.shared_from_this()) {
    }

    Database::Database(const char* path) : instance(std::make_shared<DatabaseInstance>())
    {
        instance->init(path);
    }

    Database::Database(const std::string& path) : Database(path.c_str()) {
    }

    DatabaseInstance& DatabaseInstance::getDatabase(ClientContext& context)
    {
        return *context.db;
    }

    const DatabaseInstance& DatabaseInstance::getDatabase(const ClientContext& context)
    {
        return *context.db;
    }




} // nnamespace babrat