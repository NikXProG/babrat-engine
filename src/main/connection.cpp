
#include <babrat/main/connection.hpp>
#include <babrat/main/database.hpp>
#include <babrat/main/client_context.hpp>
#include <babrat/main/connection_manager.hpp>
#include <babrat/statement/sql_statement.hpp>
#include <babrat/execution/ddl_command_result.hpp>
#include <babrat/logger/logging.hpp>

namespace babrat {

    Connection::Connection(Database& database) : Connection(*database.instance) {
        // pass initializing
    }

    Connection::Connection(DatabaseInstance &databaseInstance) :
            m_clientContext(std::make_shared<ClientContext>(
                databaseInstance.shared_from_this()))
    {
        auto &connection_manager [[maybe_unused]] = ConnectionManager::get(databaseInstance);
        connection_manager.addConnection(*m_clientContext);
        connection_manager.assignConnectionId(*this);
    }

    unique_ptr<execution::DdlCommandResult> Connection::query(unique_ptr<statement::SqlStatement> sqlStatement)
    {
        return m_clientContext->query(std::move(sqlStatement));
    }

    Connection::Connection(Connection&& other) noexcept
    {
        std::swap(m_clientContext, other.m_clientContext);
    }

    Connection& Connection::operator=(Connection&& other) noexcept
    {
        std::swap(m_clientContext, other.m_clientContext);
        return *this;
    }

    Connection::~Connection() {

        if (!m_clientContext) {
            return;
        }

        ConnectionManager::get(*m_clientContext->db)
        .removeConnection(*m_clientContext);

    }



} // namespace babrat