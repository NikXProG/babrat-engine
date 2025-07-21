#ifndef BABRAT_DELETE_COMMAND_H
#define BABRAT_DELETE_COMMAND_H

#include "sql_command.h"
#include "DML/delete_query_model.h"


/*class DeleteCommand : public SqlCommand<DeleteQueryModel> {

public:

    using SqlCommand::SqlCommand;

protected:

    void execute_with_model(DeleteQueryModel const& model) override {

        if (!model.where_condition){

            context_.getDatabase()->clear_collection(
                    model.db_name,
                    model.schema_name,
                    model.table_name);

            return;
        }*/


/*
        auto catalog = context_.getCatalog();
        auto db = context_.getDb();

        // Получаем метаданные таблицы (аналогично InsertCommand)
        auto key_table = catalog->getKeyByValue(CATALOG_COLLECTION_NAME,
                                                COLUMN(model.table_name));

        auto table_fields = catalog->findPairsByColumnValue(
                CATALOG_FIELD_NAME,
                "table_oid",
                key_table
        );

        std::vector<ColumnDefinition> column_meta;
        std::unordered_map<std::string, size_t> column_positions;

        for (size_t i = 0; i < table_fields.size(); ++i) {
            const auto& field = table_fields[i];
            auto type_inf = catalog->getDataByValue(
                    CATALOG_TYPE_NAME,
                    field.value[3]);

            column_meta.push_back({
                                          get_value<std::string>(field.value[1]),  // column_name
                                          get_value<std::string>(type_inf[1]),    // column_type
                                          get_value<bool>(field.value[4]),        // is_primary
                                          get_value<bool>(field.value[5])          // is_nullable
                                  });

            column_positions[get_value<std::string>(field.value[1])] = i;
        }



        auto check_condition =
                [&](const auto& row_data, const ConditionModel& condition) {*/
          /*  if (condition.operator_ == "AND" || condition.operator_ == "OR") {
                bool result = (condition.operator_ == "AND");
                for (const auto& cond : condition.conditions) {
                    bool cond_result = check_condition(row_data, cond);
                    if (condition.operator_ == "AND") {
                        result = result && cond_result;
                        if (!result) break;
                    } else {
                        result = result || cond_result;
                        if (result) break;
                    }
                }
                return result;
            }

            if (!condition.field || !condition.operator_ || !condition.value) {
                throw std::runtime_error("Invalid condition structure");
            }

            auto col_it = column_positions.find(*condition.field);
            if (col_it == column_positions.end()) {
                throw std::runtime_error("Column '" + *condition.field + "' not found");
            }

            size_t col_pos = col_it->second;
            const auto& column = row_data[col_pos];

            if (*condition.operator_ == "=") {
                return column->equals(*condition.value);
            } else if (*condition.operator_ == "<>") {
                return !column->equals(*condition.value);
            } else if (*condition.operator_ == ">") {
                return column->greaterThan(*condition.value);
            } else if (*condition.operator_ == "<") {
                return column->lessThan(*condition.value);
            } else if (*condition.operator_ == ">=") {
                return column->greaterThan(*condition.value) || column->equals(*condition.value);
            } else if (*condition.operator_ == "<=") {
                return column->lessThan(*condition.value) || column->equals(*condition.value);
            } else {
                throw std::runtime_error("Unsupported operator: " + *condition.operator_);
            }*/
      /*  };

        if (!model.where_condition) {
            db->clear_collection(model.db_name,
                      model.schema_name,
                      model.table_name);
        }
*/

/*

    }

    void undo_with_model(DeleteQueryModel const& model) override {

    }

};

*/


#endif //BABRAT_DELETE_COMMAND_H
