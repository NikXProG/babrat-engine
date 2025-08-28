#ifndef BABRAT_INSERT_QUERY_MODEL_H
#define BABRAT_INSERT_QUERY_MODEL_H

#include <vector>
#include "sql_statement.h"

struct InsertRowValues{

    std::vector<std::string> values_;

    explicit InsertRowValues(std::vector<std::string> values)
    : values_(std::move(values)){
    }

};

struct InsertQueryModel : public SqlStatement {

    std::string db_name;
    std::string schema_name;
    std::string collection_name;
    std::vector<std::string> column_names;
    std::vector<std::vector<std::string>> rows;

    explicit InsertQueryModel(std::string db_name,
                              std::string schema_name,
                              std::string collection_name,
                              std::vector<std::string> column_names,
                              std::vector<std::vector<std::string>> rows) :
                db_name(std::move(db_name)),
                schema_name(std::move(schema_name)),
                collection_name(std::move(collection_name)),
                column_names(std::move(column_names)),
                rows(std::move(rows)){}

};

#endif //BABRAT_INSERT_QUERY_MODEL_H
