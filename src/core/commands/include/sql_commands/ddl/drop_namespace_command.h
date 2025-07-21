#ifndef BABRAT_DROP_NAMESPACE_COMMAND_H
#define BABRAT_DROP_NAMESPACE_COMMAND_H

#include "DDL/create_schema.h"
#include "sql_command.h"
/*

class DropNamespaceCommand : public SqlCommand<CreateSchema> {

public:

    using SqlCommand::SqlCommand;

protected:

    void execute_with_model(CreateSchema const& model) override {


        context_.getCatalog()->disposeSchema(
                model.db_name,
                model.schema_name);

        context_.getDatabase()->dispose_schema(
                model.db_name,
                model.schema_name);

    }

    void undo_with_model(CreateSchema const& model) override {

    }

};
*/


#endif //BABRAT_DROP_NAMESPACE_COMMAND_H
