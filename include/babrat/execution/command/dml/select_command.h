
#ifndef BABRAT_SELECT_COMMAND_H
#define BABRAT_SELECT_COMMAND_H

#include "dml/select_query_model.h"

#include "babrat/commands/include/query_attribute/attribute_pipeline_builder.h"

#include "sql_command.h"

class RowVisitor : public ColumnVisitor{

    std::vector<std::string> row_;

public:

    explicit RowVisitor(std::vector<std::string> row) : row_(std::move(row)) {}

public:

    void visit(int val) override {
        row_.emplace_back(std::to_string(val));
    }

    void visit(unsigned int val) override {
        row_.emplace_back(std::to_string(val));
    }

    void visit(const std::string& val) override {
        row_.emplace_back(val);
    }

    void visit(const char* val) override {
        row_.emplace_back(val);
    }

    void visit(bool val) override {
        row_.emplace_back(val ? "true" : "false");
    }

public:

    std::vector<std::string> getRow(){
        return row_;
    }

    void clear(){
        row_.clear();
    }


};
/*
class SelectCommand : public SqlCommand<SelectQueryModel> {

public:

    using SqlCommand::SqlCommand;

protected:

    void execute_with_model(SelectQueryModel const& model) override {*/
     /*   auto catalog = context_.getCatalog();
        auto db = context_.getDb();


        auto key_table = catalog->getKeyByValue(CATALOG_COLLECTION_NAME,
                                                COLUMN(model.table_name));

        auto table_fields = catalog->findPairsByColumnValue(
                CATALOG_FIELD_NAME,
                "table_oid",
                key_table
        );

        std::vector<ColumnDefinition> column_meta;
        std::unordered_map<std::string, size_t> column_positions;

        for (size_t i = 0; i <
        table_fields.size(); ++i){

            const auto& field = table_fields[i];
            auto type_inf = catalog->getDataByValue(
                    CATALOG_TYPE_NAME,
                    field.value[3]);


            auto column_name = get_value<std::string>(table_fields[i].value[1]);
            auto column_type = get_value<std::string>(type_inf[1]);

            if (model.column_names.empty() ||
                std::ranges::find(model.column_names, column_name) != model.column_names.end())
            {
                column_meta.push_back({
                                              column_name,
                                              column_type,
                                              get_value<bool>(field.value[4]),
                                              get_value<bool>(field.value[5])
                                      });

                column_positions[column_name] = i;
            }

        }

        auto builder =
                (new AttributePipelineBuilder())
                        ->addWhereHandler()
                        .build();

        std::map<std::string, std::vector<Column*>> rows;


        std::vector<Column*> row;
        std::vector<std::vector<Column*>> vectors;


        for(auto it : column_positions){
            std::cout << it.first << " " << it.second << std::endl;
        }
        db->range(model.db_name,
                  model.schema_name,
                  model.table_name,
                  [&rows,&column_positions, &row, &vectors]
                  (const auto& pair){
                auto vector = pair.value;
                for(auto it : column_positions){
                    rows[it.first].emplace_back(vector[it.second]);
                    row.emplace_back(vector[it.second]);
                }
                vectors.emplace_back(row);
                row.clear();


        });

        PrintVisitor visitor(std::cout);

        for(auto it : vectors){
            for(auto item : column_meta){
               std::cout <<  column_positions[item.column_name] << " ";
            }

        }


        auto* context = new AttributeContext(rows,vectors);

        context->columns_index_ = column_positions;
        context->where_condition = model.where_condition;
        builder->handle(*context);*/

    /*    PrintVisitor visitor(std::cout);

        for(auto it : rows){
           std::cout << it.first << ": ";
           for(auto item : it.second){

               if (item->isNull()){
                   std::cout << "<null>" << " ";
                   continue;
               }

               item->accept(visitor);
           }
           std::cout << std::endl;
        }
*/
 /*   }

    void undo_with_model(SelectQueryModel const& model) override {
        // Для SELECT отмена обычно не требуется
        std::cout << "Undoing select from " << model.table_name << "\n";
    }

};
*/
#endif //BABRAT_SELECT_COMMAND_H
