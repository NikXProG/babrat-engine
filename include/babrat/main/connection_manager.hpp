#pragma once

#include <babrat/common/types/typedef.hpp>
#include <babrat/common/types/mutex.hpp>
#include <babrat/common/types/reference.hpp>

namespace babrat {

    class DatabaseInstance;
    class ClientContext;
    class Connection;

    class ConnectionManager  {

    private:

        std::mutex m_mutex;

    private:

        std::atomic<count_t> m_countConnections;
        std::atomic<connection_t> m_currentConnectionId;

    private:

        reference_map_t<ClientContext, std::weak_ptr<ClientContext>> m_connections;

    public:

        ConnectionManager();

    public:

        count_t getConnectionCount() const;

    public:

        void addConnection(ClientContext &context);
        void removeConnection(ClientContext &context);

    public:

        static ConnectionManager &get(DatabaseInstance &db);
        static ConnectionManager &get(ClientContext &context);

    public:

        void assignConnectionId(Connection &connection);

    };

} // namespace babrat