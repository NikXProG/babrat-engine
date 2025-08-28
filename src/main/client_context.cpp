#include <babrat/common/utils/string_utils.hpp>
#include <babrat/main/database_manager.hpp>
#include <babrat/main/database.hpp>
#include <babrat/statement/ddl/create_database.hpp>
#include <babrat/statement/ddl/drop_database.hpp>
#include <babrat/statement/sql_statement.hpp>
#include <babrat/main/client_context.hpp>
#include <babrat/execution/ddl_command_result.hpp>
#include <babrat/common/constants.hpp>
#include <babrat/logger/logging.hpp>

namespace babrat {


    unique_ptr<execution::DdlCommandResult> ClientContext::query(
            unique_ptr<statement::SqlStatement> statement,
            bool streamingMode) {
        auto waitQuery = toQueueQuery(std::move(statement), streamingMode);
        if (!waitQuery->isSuccess()) {
            throw std::runtime_error("not implemented error");
            /*return ErrorResult<>(pending_query->GetErrorObject());*/
        }
        return waitQuery;
    }

    ClientContext::ClientContext(std::shared_ptr<DatabaseInstance> databaseInstance) :
            db(std::move(databaseInstance)), interrupted(false), m_connectionId(constants::INVALID_INDEX) {
        logger = db->getLoggerManager().globalLogger();
    }

    void ClientContext::beginQuery(const std::string &query) {

        BABRAT_LOG(*this,
                   logger::ConnectionContextType,
                   std::to_string(m_connectionId),
                   query);

    }

    unique_ptr<execution::DdlCommandResult> ClientContext::toQueueQuery(
            unique_ptr<statement::SqlStatement> statement, bool streamingMode) {

        BABRAT_LOG(
                *this,
                logger::QueryContextType,
                utils::format("waiting query with option {}", streamingMode));

        try {

            // cleaning up old tasks
            cleanUpTasks();
            interrupted = false;

            auto queryString = statement->query;

            unique_ptr<execution::DdlCommandResult> result =
                    std::make_unique<execution::DdlCommandResult>(true, "OK");

            beginQuery(queryString);

            // TODO: do planning

            // For now we'll make do with the default plan


            return result;

        } catch (std::exception &ex) {
            throw std::runtime_error("to queue canceling. Invalid parse statement rrror");
        }

    }

    void ClientContext::cleanUpTasks() {
        // TODO: here the cleaning of previous tasks will take place
    }

    logger::Logger &ClientContext::getLogger() const {
        return *logger;
    }

    connection_t ClientContext::getConnectionId() const {
        return m_connectionId;
    }


} // namespace babrat