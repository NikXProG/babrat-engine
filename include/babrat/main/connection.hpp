#pragma once

#include <babrat/common/babrat_api.hpp>
#include <babrat/common/types/typedef.hpp>
#include <babrat/common/types/shared_ptr.hpp>
#include <babrat/common/types/unique_ptr.hpp>

namespace babrat {

    class Database;
    class DatabaseInstance;
    class ClientContext;

    namespace statement {
        class SqlStatement;
    }

    namespace execution {
        class DdlCommandResult;
    }

    class Connection {

        public:

            shared_ptr<ClientContext> m_clientContext;

        public:

            BABRAT_API explicit Connection(DatabaseInstance& databaseInstance);

            BABRAT_API explicit Connection(Database& database);

        public:

            BABRAT_API ~Connection();

        public:

            Connection(const Connection &other) = delete;
            Connection &operator=(const Connection &) = delete;

        public:

            BABRAT_API Connection(Connection &&other) noexcept;
            BABRAT_API Connection &operator=(Connection &&) noexcept;

        public:

            BABRAT_API unique_ptr<execution::DdlCommandResult> query(unique_ptr<statement::SqlStatement> statement);

    };


} // nnamespace babrat