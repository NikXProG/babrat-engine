
#ifndef BABRAT_DDL_COMMAND_FACTORY_H
#define BABRAT_DDL_COMMAND_FACTORY_H

#include <memory>
#include "ddl_statement.h"
#include "sql_commands/ddl/ddl_command.h"

class DdlCommandFactory{

public:

    virtual ~DdlCommandFactory() = default;

    virtual std::unique_ptr<IDdlCommand> create(const IDdlStatement& statement) = 0;

};

#endif //BABRAT_DDL_COMMAND_FACTORY_H
