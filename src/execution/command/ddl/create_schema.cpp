#include <babrat/execution/ddl_executor.hpp>
#include <babrat/execution/ddl_command_result.hpp>
#include <babrat/execution/command/ddl/create_schema.hpp>

namespace babrat {

    namespace execution {

        DdlCommandResult CreateSchemaCommand::execute(DdlExecutor &executor) const {
            return executor.executeCreateSchema(*this);
        }

        CreateSchemaCommand::CreateSchemaCommand(
                const std::string &schemaName,
                bool isNotExists,
                std::string authRole) :
                CreateStatementCommand(schemaName, isNotExists),
                m_authRole(std::move(authRole)) {

        }

    } // namespace execution

} // namespace babrat