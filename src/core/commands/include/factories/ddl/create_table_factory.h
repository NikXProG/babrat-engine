#ifndef BABRAT_CREATE_TABLE_FACTORY_H
#define BABRAT_CREATE_TABLE_FACTORY_H

#include "create_table.h"
#include "ddl_executor.h"
#include "create_table_command.h"

class CreateTableFactory final {

    std::shared_ptr<CommandContext> m_context;
    std::shared_ptr<IDdlExecutor> m_executor;

public:

    explicit CreateTableFactory(
            std::shared_ptr<CommandContext> context,
            std::shared_ptr<IDdlExecutor> executor)
            : m_context(std::move(context)), m_executor(std::move(executor)) {}

public:

    std::unique_ptr<CreateTableCommand> create(CreateTable const& statement) {

        return create(
                statement.getName(),
                statement.isNotExists()
        );
    }


private:

    std::unique_ptr<CreateTableCommand> create(
            std::string const& sourceName,
            bool ifExists)
    {
        return std::make_unique<CreateTableCommand>(m_executor, sourceName, ifExists);
    }


};

#endif //BABRAT_CREATE_TABLE_FACTORY_H
