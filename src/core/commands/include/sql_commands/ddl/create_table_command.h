#ifndef BABRAT_CREATE_TABLE_COMMAND_H
#define BABRAT_CREATE_TABLE_COMMAND_H
#include "ddl_command.h"
#include "ddl_executor.h"

class CreateTableCommand : public IDdlCommand {

    std::shared_ptr<IDdlExecutor> m_executor;
    std::string m_name;
    bool m_isNotExists;

public:

    explicit CreateTableCommand(
            std::shared_ptr<IDdlExecutor> executor,
            const std::string& name,
            bool isNotExists)
    : m_executor(std::move(executor)), m_name(name), m_isNotExists(isNotExists)  {
        if (name.empty()) throw std::invalid_argument("name is empty");
    }

    DdlCommandResult execute() const override {
        return m_executor->executeCreateTable(*this);
    }

public:

    const std::string& getName() const {
        return m_name;
    }

    bool isNotExists() const {
        return m_isNotExists;
    }

};



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


#endif //BABRAT_CREATE_TABLE_COMMAND_H
