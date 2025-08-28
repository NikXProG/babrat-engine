
#ifndef BABRAT_DDL_COMMAND_H
#define BABRAT_DDL_COMMAND_H

#include "ddl_command_result.h"

class IDdlCommand {

public:

    virtual ~IDdlCommand() noexcept = default;

public:

    virtual DdlCommandResult execute() const = 0;

};


#endif //BABRAT_DDL_COMMAND_H
