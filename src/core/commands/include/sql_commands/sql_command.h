#ifndef BABRAT_SQL_COMMAND_H
#define BABRAT_SQL_COMMAND_H

#include <iostream>
#include "sql_statement.h"
#include "../command.h"
#include "dml/insert_query_model.h"
#include <any>

#define COLUMN(VALUE) make_column(VALUE)

/*
class SqlCommand : Command{

public:

    virtual ~SqlCommand() = default;

public:

    void execute() override {
        execute_with_model(model);
    }

    void undo() override {
        undo_with_model(model);
    }

};
*/

enum class DdlCommandType : uint8_t {
    CREATE,
    DROP,
    ALTER,
    TRUNCATE
};

template<typename ModelType>
class SqlCommand : public Command {

protected:

    DatabaseContext context_;
    ModelType model_;

public:

    virtual ~SqlCommand() override = default;

public:

    explicit SqlCommand(const DatabaseContext& context,
                        const ModelType& model) :
                        context_(context),
                        model_(model){}

    explicit SqlCommand(const DatabaseContext& context,
                        ModelType&& model) :
            context_(context),
            model_(std::move(model)){}


};


template<typename ModelType, typename EnumType>
class TypedSqlCommand : public SqlCommand<ModelType> {

protected:

    DdlCommandType type_;

protected:

    explicit TypedSqlCommand(
            const DatabaseContext& context,
            EnumType type,
            const ModelType& model)
            : SqlCommand<ModelType>(context, model),
                    type_(type) {}

    explicit TypedSqlCommand(
            const DatabaseContext& context,
            EnumType type,
            ModelType&& model)
            : SqlCommand<ModelType>(context, model),
              type_(type) {}

};


#endif //BABRAT_SQL_COMMAND_H
