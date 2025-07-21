
#ifndef BABRAT_DROP_TABLE_COMMAND_H
#define BABRAT_DROP_TABLE_COMMAND_H



#include "DDL/create_table.h"
#include "sql_command.h"
/*

class DropTableCommand : public SqlCommand<TableQueryModel> {

public:

    using SqlCommand::SqlCommand;

protected:

    void execute_with_model(TableQueryModel const& model) override {

        context_.getCatalog()->disposeCollection(
                model.db_name,
                model.schema_name,
                model.collection_name);

        context_.getDatabase()->disposeBatch(
                model.db_name,
                model.schema_name,
                model.collection_name);

    }

    void undo_with_model(TableQueryModel const& model) override {

    }

};
*/

#endif //BABRAT_DROP_TABLE_COMMAND_H
