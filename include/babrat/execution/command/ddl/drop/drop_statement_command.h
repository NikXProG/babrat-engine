#ifndef BABRAT_DROP_STATEMENT_COMMAND_H
#define BABRAT_DROP_STATEMENT_COMMAND_H
#include <iostream>
#include "drop_table.h"
#include "drop_schema.h"
#include "drop_database.h"

#include "create_table.h"
#include "create_schema.h"
#include "create_database.h"

#include <string>
#include <optional>
#include <memory>
#include <typeindex>
#include <utility>


#include "ddl_command.hpp"

#include "babrat/execution/ddl_executor.h"

class DropStatementCommand : public IDdlCommand {

    std::shared_ptr<IDdlExecutor> m_executor;
    std::string m_name;
    bool m_isNotExists;

public:

    explicit DropStatementCommand(std::shared_ptr<IDdlExecutor> executor, const std::string& name, bool isNotExists)
            : m_executor(std::move(executor)), m_name(name), m_isNotExists(isNotExists) {
        if (name.empty()) throw std::invalid_argument("name is empty");
    }

    DdlCommandResult execute() const override {
        return m_executor->executeDropStatement(*this);
    }

public:

    const std::string& getName() const {
        return m_name;
    }

    bool isNotExists() const {
        return m_isNotExists;
    }

};


#endif //BABRAT_DROP_STATEMENT_COMMAND_H
