#ifndef BABRAT_DELETE_QUERY_MODEL_H
#define BABRAT_DELETE_QUERY_MODEL_H
#include <variant>
#include <optional>
#include <vector>
#include "sql_statement.h"
#include "../common/condition_model.h"

struct DeleteQueryModel : public SqlStatement {

    std::string db_name;
    std::string schema_name;
    std::string table_name;
    std::optional<ConditionModel> where_condition;

    DeleteQueryModel(std::string db,
                     std::string schema,
                     std::string table,
                     std::optional<ConditionModel> where = std::nullopt)
            : db_name(std::move(db)),
              schema_name(std::move(schema)),
              table_name(std::move(table)),
              where_condition(std::move(where))
              {}
};


#endif //BABRAT_DELETE_QUERY_MODEL_H
