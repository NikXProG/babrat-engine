#pragma once

#include <babrat/common/babrat_api.hpp>
#include <babrat/common/types/weak_ptr.hpp>
#include <babrat/common/types/shared_ptr.hpp>
#include <babrat/common/types/unique_ptr.hpp>
#include <babrat/common/types/atomic.hpp>
#include <babrat/common/types/typedef.hpp>

namespace babrat {

    class DatabaseInstance;

    namespace logger {
        class Logger; // logger client for write log
    }

    namespace statement {
        class SqlStatement; // sql statement for communication with db
    }

    namespace execution {
        class DdlCommandResult;
    }

    class ClientContext : public enable_shared_from_this<ClientContext> {

        friend class ConnectionManager;

    public:

        /**
          * The database that this client is connected to
          */
        shared_ptr<DatabaseInstance> db;

        /**
          * The logger to be used by this ClientContext
          */
        shared_ptr<logger::Logger> logger;

        /**
          * Set to true if the user chooses to abort the request
          */
        atomic<bool> interrupted;

        /**
         *
         */

    public:

        BABRAT_API explicit ClientContext(shared_ptr<DatabaseInstance> db);

    public:

        BABRAT_API unique_ptr<execution::DdlCommandResult> query(
                unique_ptr<statement::SqlStatement> statement,
                bool streamingMode = false);

    private:

        unique_ptr<execution::DdlCommandResult> toQueueQuery(
            unique_ptr<statement::SqlStatement>,
            bool streamingMode);

    private:

        void beginQuery(const std::string& query);

        void endQuery();

    private:

        void cleanUpTasks();

    public:

        /**
          * @brief Return the current logger
          */
        logger::Logger &getLogger() const;

        connection_t getConnectionId() const;

    private:

        connection_t m_connectionId;

    };

} // namespace babrat