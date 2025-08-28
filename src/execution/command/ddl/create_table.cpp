#include <babrat/execution/ddl_executor.hpp>
#include <babrat/execution/ddl_command_result.hpp>
#include <babrat/execution/command/ddl/create_table.hpp>

namespace babrat {

    namespace execution {

        DdlCommandResult CreateTableCommand::execute(DdlExecutor &executor) const {
            return executor.executeCreateTable(*this);
        }

        CreateTableCommand::CreateTableCommand(const std::string &name, bool isNotExists) :
                CreateStatementCommand(name, isNotExists) {

        }

    } // namespace execution

} // namespace babrat