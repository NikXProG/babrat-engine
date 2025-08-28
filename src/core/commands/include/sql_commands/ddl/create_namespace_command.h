#ifndef BABRAT_CREATE_NAMESPACE_COMMAND_H
#define BABRAT_CREATE_NAMESPACE_COMMAND_H
#include "DDL/create_schema.h"
#include "sql_command.h"
/*

class CreateNamespaceCommand : public SqlCommand<CreateSchema> {

public:

    using SqlCommand::SqlCommand;

protected:

    void execute_with_model(CreateSchema const& model) override {

        context_.getCatalog()->addSchema(
                model.db_name,
                model.schema_name,
                model.role_name);

        context_.getDatabase()->add_schema(
                model.db_name,
                model.schema_name);

    }

    void undo_with_model(CreateSchema const& model) override {

        try{

            context_.getCatalog()->disposeSchema(
                    model.schema_name,
                    model.schema_name);

            context_.getDatabase()->dispose_schema(
                    model.db_name,
                    model.schema_name);

        }catch(...){

            //pass throw

        }


    }

};
*/


#endif //BABRAT_CREATE_NAMESPACE_COMMAND_H
