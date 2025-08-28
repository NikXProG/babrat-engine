
#ifndef BABRAT_DDL_DEFAULT_EXECUTOR_H
#define BABRAT_DDL_DEFAULT_EXECUTOR_H

#include <iostream>

#include "ddl_executor.h"


class DdlExecutor : public IDdlExecutor {


public:

    DdlCommandResult executeCreateTable(const CreateTableCommand& createTableCommand) override {
        std::cout << "CREATE TABLE " + createTableCommand.getName() << std::endl;
        std::cout << std::format("IS NOT EXISTS {}",(createTableCommand.isNotExists() ? "true" : "false")) << std::endl;

        return DdlCommandResult(true, "Table created");
    }

    DdlCommandResult executeDropStatement(const DropStatementCommand& dropStatementCommand) override {

        std::cout << "DROP STATEMENT " + dropStatementCommand.getName() << std::endl;
        std::cout << std::format("IS EXISTS {}",(dropStatementCommand.isNotExists() ? "true" : "false")) << std::endl;

        return DdlCommandResult(true, "resource is dropped");
    }

};


#endif //BABRAT_DDL_DEFAULT_EXECUTOR_H
