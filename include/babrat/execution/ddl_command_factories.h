#pragma once

#include <babrat/execution/factories/create_table_factory.hpp>
#include <babrat/execution/factories/ddl_command_factory.hpp>

#include <typeindex>
#include "ddl_default_executor.h"
#include "drop_table.h"
#include "drop_database.h"
#include "drop_schema.h"
#include "create_table.h"
#include "demangle.h"

#include "command_context.h"

class DdlCommandFactories : public DdlCommandFactory {

private:

    /*DropStatementFactory m_dropStatementFactory;*/

    CreateTableFactory m_createStatementFactory;

private:

    std::unordered_map<std::type_index, std::unique_ptr<IDdlCommand>
    (DdlCommandFactories::*)(const IDdlStatement&)> m_handlers;

public:

    explicit DdlCommandFactories(
            const std::shared_ptr<CommandContext>& context,
            const std::shared_ptr<IDdlExecutor>& executor = std::make_shared<DdlExecutor>())
            : m_dropStatementFactory(context, executor),
              m_createStatementFactory(context, executor) {
        initializeHandlers();
    }

public:

    std::unique_ptr<DdlCommand> create(const IDdlStatement& statement) override{

        auto it = m_handlers.find(typeid(statement));
        if (it != m_handlers.end()) {
            return (this->*(it->second))(statement);
        }

        throw std::runtime_error(
                "Unable to find ddl command factory for statement. Valid statement: " +
                getRegisteredStatementTypes());

    }

private:
/*
    std::unique_ptr<IDdlCommand> handleDropTable(const IDdlStatement& stmt) {
        return m_dropStatementFactory.create(dynamic_cast<const DropTable&>(stmt));
    }

    std::unique_ptr<IDdlCommand> handleDropSchema(const IDdlStatement& stmt) {
        return m_dropStatementFactory.create(dynamic_cast<const DropSchema&>(stmt));
    }

    std::unique_ptr<IDdlCommand> handleDropDatabase(const IDdlStatement& stmt) {
        return m_dropStatementFactory.create(dynamic_cast<const DropDatabase&>(stmt));
    }*/

    std::unique_ptr<IDdlCommand> handleCreateTable(const IDdlStatement& stmt) {
        return m_createStatementFactory.create(dynamic_cast<const CreateTable&>(stmt));
    }


private:

    void initializeHandlers() {
        m_handlers[typeid(DropTable)] = &DdlCommandFactories::handleDropTable;
        m_handlers[typeid(DropSchema)] = &DdlCommandFactories::handleDropSchema;
        m_handlers[typeid(DropDatabase)] = &DdlCommandFactories::handleDropDatabase;
        m_handlers[typeid(CreateTable)] = &DdlCommandFactories::handleCreateTable;
    }

private:

    std::string getRegisteredStatementTypes() const {
        std::string result;
        bool first_el = true;

        for (const auto& [type, _] : m_handlers) {
            if (!first_el) {
                result += ", ";
            } else {
                first_el = false;
            }

            result += demangle(type.name());
        }

        return result;
    }

};

#endif //BABRAT_DDL_COMMAND_FACTORIES_H
