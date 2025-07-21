
#ifndef BABRAT_DROP_STATEMENT_FACTORY_H
#define BABRAT_DROP_STATEMENT_FACTORY_H


#include "ddl_executor.h"
#include "drop_statement_command.h"
#include "drop_table.h"
#include "drop_database.h"
#include "drop_schema.h"
#include "../command_context.h"


class DropStatementFactory final {

    std::shared_ptr<CommandContext> m_context;
    std::shared_ptr<IDdlExecutor> m_executor;

public:

    explicit DropStatementFactory(
            std::shared_ptr<CommandContext> context,
            std::shared_ptr<IDdlExecutor> executor)
            : m_context(std::move(context)), m_executor(std::move(executor)) {}

public:

    std::unique_ptr<DropStatementCommand> create(DropTable const& statement) {
        return create(
                statement.getName(),
                statement.isExists()
        );
    }

    std::unique_ptr<DropStatementCommand> create(DropSchema const& statement) {
        return create(
                statement.getName(),
                statement.isExists()
        );
    }

    std::unique_ptr<DropStatementCommand> create(DropDatabase const& statement) {
        return create(
                statement.getName(),
                statement.isExists()
        );
    }


private:

    std::unique_ptr<DropStatementCommand> create(
            std::string const& sourceName,
            bool ifExists)
    {
        return std::make_unique<DropStatementCommand>(
                m_executor, sourceName, ifExists);
    }


};


#endif //BABRAT_DROP_STATEMENT_FACTORY_H
