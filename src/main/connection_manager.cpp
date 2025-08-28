
#include <babrat/main/client_context.hpp>
#include <babrat/main/connection.hpp>
#include <babrat/main/connection_manager.hpp>
#include <babrat/main/database.hpp>

namespace babrat {

    ConnectionManager::ConnectionManager() :
        m_countConnections(0), m_currentConnectionId(0) {

    }

    void ConnectionManager::addConnection(ClientContext &context)
    {
        std::lock_guard<std::mutex> lock(m_mutex);

        /*for (auto &callback : DBConfig::GetConfig(context).extension_callbacks) {
            callback->OnConnectionOpened(context);
        }*/

        m_connections[context] = std::weak_ptr<ClientContext>(context.shared_from_this());
        m_countConnections = m_connections.size();

    }

    void ConnectionManager::removeConnection(ClientContext &context) {
        std::lock_guard<std::mutex> lock(m_mutex);
        m_connections.erase(context);
        m_countConnections = m_connections.size();
    }

    ConnectionManager& ConnectionManager::get(ClientContext &context)
    {
        return ConnectionManager::get(DatabaseInstance::getDatabase(context));
    }

    ConnectionManager& ConnectionManager::get(DatabaseInstance &databaseInstance)
    {
        return databaseInstance.getConnectionManager();
    }

    void ConnectionManager::assignConnectionId(Connection& connection)
    {
        connection.m_clientContext->m_connectionId =
                m_currentConnectionId.fetch_add(1, std::memory_order_relaxed) + 1;
    }

    count_t ConnectionManager::getConnectionCount() const
    {
        return m_countConnections;
    }


} // nnamespace babrat