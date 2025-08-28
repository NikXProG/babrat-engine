#ifndef BABRAT_INSERT_COMMAND_H
#define BABRAT_INSERT_COMMAND_H

#include "DML/insert_query_model.h"
#include "sql_command.h"
#include "unordered_set"
#include "column_key.h"

/*

class InsertCommand : public SqlCommand<InsertQueryModel> {


public:

private:

    std::vector<AssociativeContainer<Column*, std::vector<Column*>>::KeyValuePair> vector_pair_;

protected:

        void execute_with_model(InsertQueryModel const& model) override {

           vector_pair_ = context_.getCatalog()->add(
                    model.db_name,
                    model.schema_name,
                    model.collection_name,
                    model.column_names,
                    model.rows);

            context_.getDatabase()->add_batch(
                    model.db_name,
                    model.schema_name,
                    model.collection_name,
                    vector_pair_
                    );



        }

        void undo_with_model(InsertQueryModel const& model) override {

            try{


                for (const auto& pair : vector_pair_) {
                    context_.getDatabase()->dispose(
                            model.db_name,
                            model.schema_name,
                            model.collection_name,
                            pair.key
                    );
                }


                vector_pair_.clear();


            }catch(...){

            }







        }


private:



};*/

#endif //BABRAT_INSERT_COMMAND_H

/*

            auto catalog = context_.getCatalog();
            auto db = context_.getDb();

            auto key_table =
                    catalog->getKeyByValue(CATALOG_COLLECTION_NAME,
                                           COLUMN(model.collection_name));


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

                auto column_name = get_value<std::string>(field.value[1]);

                column_meta.push_back({
                    column_name,  // Имя столбца
                  get_value<std::string>(type_inf[1]),     // Тип данных
                  get_value<bool>(field.value[4]),         // is_primary
                  get_value<bool>(field.value[5])          // is_nullable
                                      });

                column_positions[column_name] = i;
            }


            //obsolete code
            std::unordered_map<std::string, size_t> request_col_map;
            for (size_t i = 0; i < model.column_names.size(); ++i) {
                request_col_map[model.column_names[i]] = i;
            }

            std::vector<Column*> primary_keys;

            std::vector<Column*> values(column_meta.size());

            Column* key = nullptr;


            for (const auto &row: model.rows) {


                if (row.size() != model.column_names.size()) {
                    throw std::runtime_error("Row size doesn't match column names size");
                }


                for (const auto &item: column_meta) {

                    if (item.isPrimaryKey && item.isNullable){
                        throw std::runtime_error("is primary key cannot be nullable");
                    }

                    auto it = request_col_map.find(item.column_name);

                    Column* column = ColumnFactory::create(string_to_type(item.column_type));

                    if (it != request_col_map.end()) {

                        size_t index = it->second;

                        if (index >= row.size()) {
                            throw std::runtime_error("Invalid row data");
                        }

                        bool hasValue = row[index].has_value();

                        if (!item.isNullable && !hasValue) {
                            throw std::runtime_error("Column '" + item.column_name + "' cannot be NULL");
                        }


                        if (hasValue) {
                            column->insert(row[index].value());
                        }

                    }

                    size_t pos = column_positions[item.column_name];
                    values[pos] = column;

                    if (item.isPrimaryKey) {


                        db->range(model.db_name,
                                  model.schema_name,
                                  model.collection_name,
                                  [column, item](const auto& pairValue){
                            if (pairValue.key->equals(column)){
                                throw std::runtime_error("Duplicate primary key value for column '" +
                                                                 item.column_name +  "'");
                            }
                        });

                        primary_keys.push_back(column);
                    }

                }

                for (size_t i = 0; i < values.size(); ++i) {
                    if (values[i] == nullptr) {

                        if (!column_meta[i].isNullable) {
                            throw std::runtime_error("Column '" + column_meta[i].column_name +
                                                     "' is required but not provided");
                        }

                        values[i] = ColumnFactory::create(
                                string_to_type(column_meta[i].column_type));

                    }
                }


                if (primary_keys.empty()){
                    key = COLUMN(context_.getOid());
                }

                key = (primary_keys.size() == 1) ?
                      primary_keys.back() :
                      new CompositeKeyColumn(
                              primary_keys
                      );

                db->add(
                        model.db_name,
                        model.schema_name,
                        model.collection_name,
                        key,
                        values);

                primary_keys.clear();
                values.clear();

            }
*/


/*    db->add(model.db_name, model.schema_name, model.collection_name, key, values);

*/
/*
 *
 *  for (const auto& [name, meta] : column_meta) {
       if (!row_values.count(name)) {
           if (meta.isPrimaryKey) {
               values.push_back(make_column("nullable"));
           } else {
               throw std::logic_error("Column validation failed");
           }
       } else {
           values.push_back(row_values[name]);
       }
   }

   for (const auto& [name, col] : row_values) {
       if (column_meta[name].isPrimaryKey) {
           key = col;
           break;
       }
   }
   for (size_t i = 0; i < it.size(); ++i) {

       auto column_data =
               catalog->getDataByValue(CATALOG_FIELD_NAME,
                                 COLUMN(model.column_names[i]));
       auto type_inf = catalog->getDataByValue(
               CATALOG_TYPE_NAME,
               column_data[3]);

       auto type = get_value<std::string>(type_inf[1]);

       auto isPrimaryKey =get_value<bool>(column_data[5]);

       auto isNullable =get_value<bool>(column_data[4]);

       auto oid = magic_enum::enum_cast<BabratSystemOID>(type);

       if (!oid.has_value()) {
           throw std::runtime_error("Unsupported type: " + type);
       }

       auto column = make_column(it[i] ,oid.value());


       if (isPrimaryKey) key = column;


       value.push_back(column);
   }

   context_.getDb()->add(
           model.db_name,
           model.schema_name,
           model.collection_name,
           key,
           value);

   key = nullptr;
   value.clear();*/


/*     for (const auto& row : model.rows) {
         std::vector<Column*> values;
         std::unordered_map<std::string, Column*> row_values;

         for (size_t i = 0; i < model.column_names.size(); ++i) {
             const auto& col_name = model.column_names[i];
             row_values[col_name] = make_column(
                     row[i],
                     TO_OID(column_meta[col_name].column_type).value()
                     );
         }

         for (const auto& [name, meta] : column_meta) {
             if (!row_values.count(name)) {
                 if (meta.isPrimaryKey) {
                     values.push_back(make_column("nullable"));
                 } else {
                     throw std::logic_error("Column validation failed");
                 }
             } else {
                 values.push_back(row_values[name]);
             }
         }

         for (const auto& [name, col] : row_values) {
             if (column_meta[name].isPrimaryKey) {
                 key = col;
                 break;
             }
         }

         db->add(model.db_name, model.schema_name, model.collection_name, key, values);
     }*/
