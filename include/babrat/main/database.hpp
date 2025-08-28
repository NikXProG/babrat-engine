#pragma once

#include <iostream>

#include <babrat/common/babrat_api.hpp>

#define BABRAT_LOG_NAME "BABRAT"

namespace babrat {

    class DatabaseManager;
    class ConnectionManager;
    class ClientContext;

    namespace logger {
        class LoggerManager;
    } // namespace logger

    class DatabaseInstance : public std::enable_shared_from_this<DatabaseInstance> {

        friend class Database;

    private:

        // required to manage the connection of the database storage
        // and also to disconnect the database storage
        std::unique_ptr<DatabaseManager> m_databaseManager;

        // required to manage the connection of the database storage
        // and also to disconnect the database storage
        std::unique_ptr<ConnectionManager> m_connectionManager;

        // required to manage the connection of the database storage
        // and also to disconnect the database storage
        std::shared_ptr<logger::LoggerManager> m_loggerManager;

    public:

        BABRAT_API DatabaseInstance();

    public:

        BABRAT_API ~DatabaseInstance();

    public:

        BABRAT_API logger::LoggerManager &getLoggerManager() const;
        BABRAT_API ConnectionManager& getConnectionManager();
        BABRAT_API DatabaseManager& getDatabaseManager();

    public:

        BABRAT_API static DatabaseInstance &getDatabase(ClientContext &context);
        BABRAT_API static const DatabaseInstance &getDatabase(const ClientContext &context);

    private:

        void init(const char *path);

    };

    /**
      * @brief Leading main database class for encapsulating DatabaseInstance
      */
    class Database {

    public:

        /**
          * @brief Reference to the actual database instance
          */
        std::shared_ptr<DatabaseInstance> instance;

    public:

        /**
         * @brief creates a basic database client and takes as
         * a parameter the path to serialize the database files
         *
         * @param[in] path if path equals `nullptr` ( default ) => memory database
         * In another case we pass the path where the file databases
         * will be stored `.db` (can be changed in the config)
         */
        BABRAT_API explicit Database(const char *path = nullptr);

        BABRAT_API explicit Database(const std::string &path);

        /**
         * @brief we can configure and feed our instance directly to the database client
         *
         * @param[in,out] databaseInstance reference to a database instance encapsulating control
         */
        BABRAT_API explicit Database(DatabaseInstance& databaseInstance);

    };

} // namespace babrat