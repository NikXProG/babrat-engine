#pragma once

#include <babrat/common/types/string.hpp>
#include <babrat/execution/command/ddl/create_statement.hpp>

namespace babrat {

    namespace execution {

        class DdlExecutor;
        class DdlCommandResult;

        class CreateTableCommand : public CreateStatementCommand  {

        public:

            explicit CreateTableCommand(const string& name,bool isNotExists);

        public:
            
            DdlCommandResult execute(DdlExecutor& executor) const override;

        };

    } // namespace execution

} // namespace babrat





/*
class CreateTableCommand : public SqlCommand<TableQueryModel> {

public:

    using SqlCommand::SqlCommand;

protected:

    void execute_with_model(TableQueryModel const& model) override {

        if (!model.column_def.has_value()||
            model.column_def.value().empty()){
            throw std::runtime_error("Column definition vector must not be empty");
        }


        context_.getCatalog()
            ->addCollection(model.db_name,
                            model.schema_name,
                            model.collection_name,
                            model.column_def.value());

        context_.getDatabase()->addBatch(
                model.db_name,
                model.schema_name,
                model.collection_name);

    }

    void undo_with_model(TableQueryModel const& model) override {

        try{

            context_.getCatalog()
                    ->disposeCollection(model.collection_name,
                                    model.column_def.value());


            context_.getDatabase()->disposeBatch(
                    model.db_name,
                    model.schema_name,
                    model.collection_name);

        }
        catch(...){

            //pass throw

        }
    }


};*/
/*
        auto key_db =
                dynamic_cast<ColumnType<Oid>*>(catalog->findPairByValue(
                        CATALOG_DATABASE_NAME,
                        make_column(model.db_name)
                ).key)->getValue();

        auto key_schema =
                dynamic_cast<ColumnType<Oid>*>(catalog->findPairByValue(
                        CATALOG_SCHEMA_NAME,
                        make_column(model.schema_name)
                ).key)->getValue();*/

