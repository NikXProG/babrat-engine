/*#ifndef BABRAT_COMMAND_FACTORY_H
#define BABRAT_COMMAND_FACTORY_H
#include <unordered_map>
#include "command.h"
#include "sql_command.h"
#include "functional"
#include "sql_statement.h"

#include "sql_commands/DML/insert_command.h"
#include "sql_commands//DML/delete_command.h"

#include "sql_commands/DDL/create_database_command.h"
#include "sql_commands/DDL/create_table_command.h"
#include "sql_commands/DDL/create_namespace_command.h"

#include "sql_commands/DDL/drop_database_command.h"
#include "sql_commands/DDL/drop_namespace_command.h"
#include "sql_commands/DDL/drop_table_command.h"
#include "dml/select_command.h"*/
/*
template<typename EnumType, typename Model>
class CommandFactory {

public:

    virtual ~CommandFactory() noexcept = default;

public:

    virtual void registerCommand(EnumType model,
                                 std::function<std::unique_ptr<Command<Model>>(DatabaseContext&)> command) = 0;

public:

    [[nodiscard]] virtual std::unique_ptr<Command<Model>> createCommand(
            EnumType type_command) const = 0;

};*/


/*
#endif //BABRAT_COMMAND_FACTORY_H
*/
