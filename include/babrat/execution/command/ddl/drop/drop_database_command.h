#ifndef BABRAT_DROP_DATABASE_COMMAND_H
#define BABRAT_DROP_DATABASE_COMMAND_H

#include "DDL/create_database.h"
#include "sql_command.h"
/*

class DropDatabaseCommand : public SqlCommand<CreateDatabase> {

public:

    using SqlCommand::SqlCommand;

protected:

    void execute_with_model(CreateDatabase const& model) override {

       context_.getCatalog()->disposeDatabase(model.db_name);

       context_.getDatabase()->dispose_pool(model.db_name);

    }

    void undo_with_model(CreateDatabase const& model) override {

    }

};
*/

#endif //BABRAT_DROP_DATABASE_COMMAND_H
