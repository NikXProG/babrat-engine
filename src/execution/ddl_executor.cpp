#include <babrat/execution/ddl_executor.hpp>
#include <babrat/execution/ddl_command_result.hpp>
#include <babrat/execution/ddl_command.hpp>
#include <babrat/execution/command/ddl/create_table.hpp>
#include <babrat/execution/command/ddl/create_schema.hpp>

#include <iostream>

namespace babrat {

    namespace execution {

        DdlCommandResult DdlExecutor::execute(DdlCommand &command) {
            return command.execute(*this);
        }

        DdlCommandResult DdlCommandExec::executeCreateTable(
                const CreateTableCommand &createTableCommand) {

            std::cout << "CREATE TABLE `" <<
            createTableCommand.getName() <<
            std::format("` IS NOT EXISTS {}",(createTableCommand.isNotExists() ? "true" : "false")) <<
            std::endl;

            return DdlCommandResult(true, "Table created");
        }

        DdlCommandResult DdlCommandExec::executeDropStatement(
                const DropStatementCommand &dropStatementCommand [[maybe_unused]]) {
         /*   std::cout << "DROP STATEMENT " + dropStatementCommand.getName() << std::endl;
            std::cout << std::format("IS EXISTS {}",(dropStatementCommand.isNotExists() ? "true" : "false")) << std::endl;
*/
            return DdlCommandResult(true, "resource is dropped");
        }


        DdlCommandResult DdlCommandExec::executeCreateSchema(
                const CreateSchemaCommand &createSchemaCommand) {
            std::cout << "CREATE SCHEMA `" <<
                      createSchemaCommand.getName() <<
                      std::format("` AUTHORIZATION `{}`", createSchemaCommand.getRole()) <<
                      std::format(" IS NOT EXISTS {}",(createSchemaCommand.isNotExists() ? "true" : "false")) <<
                      std::endl;
            return DdlCommandResult(true, "schema created");
        }


    } // namespace execution

} // namespace babrat