#ifndef BABRAT_DDL_EXECUTOR_H
#define BABRAT_DDL_EXECUTOR_H
#include "ddl_command_result.h"

class CreateTableCommand;

class DropStatementCommand;

class IDdlExecutor {

public:

    virtual ~IDdlExecutor() noexcept = default;

public:

    virtual DdlCommandResult executeCreateTable(
            const CreateTableCommand& cmd)  = 0;

    virtual DdlCommandResult executeDropStatement(
            const DropStatementCommand& cmd) = 0;

};

#endif //BABRAT_DDL_EXECUTOR_H
