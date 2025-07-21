#ifndef BABRAT_SELECT_QUERY_MODEL_H
#define BABRAT_SELECT_QUERY_MODEL_H
#include "sql_statement.h"
#include <vector>
#include "../common/condition_model.h"

class SelectQueryModel : public SqlStatement {

public:

    std::string db_name;
    std::string schema_name;
    std::string table_name;
    std::vector<std::string> column_names; // Пустой вектор означает "SELECT *"
    std::optional<ConditionModel> where_condition;
    std::optional<std::string> order_by;
    std::optional<bool> ascending; // true = ASC, false = DESC
    std::optional<size_t> limit;
    std::optional<size_t> offset;

    SelectQueryModel(std::string db,
                     std::string schema,
                     std::string table,
                     std::vector<std::string> cols = {},
                     std::optional<ConditionModel> where = std::nullopt,
                     std::optional<std::string> order = std::nullopt,
                     std::optional<bool> asc = std::nullopt,
                     std::optional<size_t> lim = std::nullopt,
                     std::optional<size_t> off = std::nullopt)
            : db_name(std::move(db)),
              schema_name(std::move(schema)),
              table_name(std::move(table)),
              column_names(std::move(cols)),
              where_condition(std::move(where)),
              order_by(std::move(order)),
              ascending(asc),
              limit(lim),
              offset(off) {}

private:

    std::vector<std::vector<std::string>> rows_;

public:

    void setResult(std::vector<std::vector<std::string>> rows){
        rows_ = std::move(rows);
    }

    const std::vector<std::vector<std::string>>& getResult(){
        return rows_;
    }

};

#endif //BABRAT_SELECT_QUERY_MODEL_H
