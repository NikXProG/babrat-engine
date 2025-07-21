#pragma once

#include "type_table.h"
#include "schema_table.h"
#include "role_table.h"
#include "database_table.h"
#include "field_table.h"
#include "collection_table.h"
#include "auth_table.h"
#include <unordered_set>
#include <algorithm>

#include "common/column_definition_collection.h"
#include "create_schema.h"
#include "create_table.h"

#include "babrat_id.h"

namespace babrat {

    class ICatalog  {

    public:

        virtual ~ICatalog() noexcept = default;


    public:

        virtual void init() = 0;

    public:

        virtual Oid getOidByColumn(Column* column,
                                   const std::string& catalog_name) = 0;

        /*  virtual const SystemTable& findTableByOid(Oid tableOid) const = 0;

           virtual const SystemTable& findTableByColumnAndOid(const Column& searchColumn, Oid tableOid) const = 0;

           virtual std::vector<std::unique_ptr<Column>> findAllColumnsByOid(Oid oid) const = 0;*/

        /*   virtual const void createSchema(CreateSchema& schema) = 0;*/

        /*  virtual const void createTable(CreateTable& table) = 0;*/

    };





}


/* const SystemTable& findTableByOid(Oid tableOid) const override{
        return TypeTable();
    }

    const SystemTable& findTableByColumnAndOid(const Column& searchColumn, Oid tableOid) const override{
        return TypeTable();
    }

    std::vector<std::unique_ptr<Column>> findAllColumnsByOid(Oid oid) const override{
        std::vector<std::unique_ptr<Column>> result;
        return result;
    }*//*



   void createCatalog(const CatalogSet& newCatalog) const {

   }

};

/*

enum class CatalogType : uint8_t {
    TABLE_CATALOG,
    SCHEMA_CATALOG,
    DATABASE_CATALOG,
    ROLE_CATALOG,
    FIELD_CATALOG,
    VIEW_CATALOG,
    INDEX_CATALOG,
    TYPE_CATALOG
};


class CatalogSet {

    std::string catalog_name;

public:

    std::string const& getName() const{
        return catalog_name;
    }


};

#include "database_context.h"
#include "../registry_types/create_type.h"
#include "../registry_types/default_types.h"


class ICatalog  {

public:

    virtual ~ICatalog() noexcept = default;

  */
/*  virtual const SystemTable& findTableByOid(Oid tableOid) const = 0;

    virtual const SystemTable& findTableByColumnAndOid(const Column& searchColumn, Oid tableOid) const = 0;

    virtual std::vector<std::unique_ptr<Column>> findAllColumnsByOid(Oid oid) const = 0;
*//*

};

class Catalog : public ICatalog {

    std::shared_ptr<DatabaseStorage> storage_;

public:

     ~Catalog() noexcept override = default;

public:

    explicit Catalog(std::shared_ptr<DatabaseStorage> storage):
        storage_(std::move(storage)) {
    }

public:

   */
/* const SystemTable& findTableByOid(Oid tableOid) const override{
        return TypeTable();
    }

    const SystemTable& findTableByColumnAndOid(const Column& searchColumn, Oid tableOid) const override{
        return TypeTable();
    }

    std::vector<std::unique_ptr<Column>> findAllColumnsByOid(Oid oid) const override{
        std::vector<std::unique_ptr<Column>> result;
        return result;
    }*//*



   void createCatalog(const CatalogSet& newCatalog) const {

   }

};


class CatalogObject {

    std::string name;

public:

    virtual ~CatalogHeadObject() noexcept = default;

};


class TypeTableObject : public CatalogObject {

public:

    static constexpr const CatalogType TYPE = CatalogType::TYPE_CATALOG;

    static constexpr std::string_view NAME = "type";

public:

    std::unique_ptr<SqlPrimitiveType> type;

public:

    explicit TypeTable(Catalog &catalog, const CreateType& type) {

    }


};



class CatalogFactory {

    Catalog & m_catalog;

public:

    explicit CatalogFactory(Catalog &catalog) : m_catalog(catalog){}

public:

    virtual ~CatalogFactory() noexcept = default;

public:

    virtual std::unique_ptr<CatalogObject> create(
            const DatabaseContext& context,
            const std::string& name) = 0;

};

class CatalogTypeFactory : CatalogFactory{

public:




};



*/


/*

    std::vector<AssociativeContainer<Column*, std::vector<Column*>>::KeyValuePair>
    findPairsByColumnValue(const std::string& collection_name,
                           const std::string& column_name,
                           Column* search_value)
    {
        std::vector<AssociativeContainer<Column*, std::vector<Column*>>::KeyValuePair> results;

        size_t column_index = getColumnIndex(collection_name, column_name);

        storage_->range(
                DB_DEFAULT_NAME,
                CATALOG_NAME,
                collection_name,
                        [&](const auto& kv_pair)
                        {

            if (column_index < kv_pair.value.size()) {
                if (kv_pair.value[column_index]->equals(search_value)) {
                    results.push_back(kv_pair);
                }
            }
        });

        if (results.empty()) {
            throw std::runtime_error("No records found with value in column '" +
                                     column_name + "'");
        }


        return results;
    }

    size_t getColumnIndex(const std::string& collection_name,
                          const std::string& column_name)
    {
        for (auto& table : tables_history_) {
            if (table->get_type() == collection_name) {
                const auto& columns = table->get_column_names();
                auto it = std::find(columns.begin(), columns.end(), column_name);
                if (it != columns.end()) {
                    return std::distance(columns.begin(), it);
                }
            }
        }
        throw std::runtime_error("Column '" + column_name + "' not found in collection '" +
                                 collection_name + "'");
    }

    std::vector<std::string> const& getColumnNames(const std::string& collection_name) {

        for(auto & it : tables_history_){
            if (it->get_type() == collection_name){
                return it->get_column_names();
            }
        }

        throw std::runtime_error("Not found resource collection "
                                 + collection_name);

    }


    Column* getOidByValueIf(
            const std::string& collection_name,
            Column* column,
            std::function<bool(Column* const&)> predicate_for_inner) {

        std::optional<Column*> result;

        bool pred = false;

        storage_->range(
                DB_DEFAULT_NAME,
                CATALOG_NAME,
                collection_name,
                [&](const auto& kv) {

                    for (Column* item : kv.value) {
                        pred = predicate_for_inner(item);
                        if (item->equals(column)) {
                            result = kv.key;
                        }
                    }

                    if (result.has_value() || pred ){
                        return;
                    }

                    pred = false;

                });

        if (result.has_value()) {
            return result.value();
        }

        throw std::runtime_error(
                std::format("Value not found in collection `{}` with predicate for oid", collection_name));

    }

    AssociativeContainer<Column*, std::vector<Column*>>::KeyValuePair
    findPairByValue(
            const std::string& collection_name,
            Column* column) {

        std::optional<AssociativeContainer<Column*, std::vector<Column*>>::KeyValuePair> result;

        storage_->range(
                DB_DEFAULT_NAME,
                CATALOG_NAME,
                collection_name,
                [&](const auto& kv) {

                    for (Column* item : kv.value) {
                        if (item->equals(column)) {
                            result = kv;
                            return;
                        }
                    }


                });

        if (result.has_value()) {
            return result.value();
        }

        throw std::runtime_error(
                std::format("Value not found in collection `{}`", collection_name));

    }

    std::vector<Column*> getDataByValue(
            const std::string& collection_name,
            Column* value) {
         return findPairByValue(collection_name, value).value;
    }

    Column* getKeyByValue(
            const std::string& collection_name,
            Column* value) {
        return findPairByValue(collection_name, value).key;
    }

public:

    void addDatabase(const std::string& db_name){

        auto oid = babrat::to_column(oidGen_->GetNewOid());

        insert(CATALOG_DATABASE_NAME,
                oid,{babrat::to_column(db_name)});

    }

    void addSchema(const  std::string& db_name,
                   const  std::string& schema_name,
                   const  std::string& role_name){

        auto oid = babrat::to_column(oidGen_->GetNewOid());

        auto key_db =
               getKeyByValue(
                        CATALOG_DATABASE_NAME,
                        babrat::to_column(db_name)
                );

        auto key_role = getKeyByValue(
                CATALOG_ROLE_NAME,
                babrat::to_column(role_name)
        );


        insert(CATALOG_SCHEMA_NAME,
                oid, {
                    babrat::to_column(schema_name),
                    key_db,
                    key_role});


    }

    void addCollection(
                    const  std::string& db_name,
                    const  std::string& schema_name,
                       const  std::string& collection_name,
                           std::vector<ColumnDefinition> const &columns){

        auto key_db =
                getKeyByValue(CATALOG_DATABASE_NAME,
                              babrat::to_column(db_name));

        auto oid = babrat::to_column(oidGen_->GetNewOid());

        auto key_schema =
                getOidByValueIf(
                        CATALOG_SCHEMA_NAME,
                        babrat::to_column(schema_name),
                        [&key_db](Column* const& value){
                            return (value->equals(key_db));
                        }
                );

        insert(CATALOG_COLLECTION_NAME,
               oid,
               {
                       oid,
                       babrat::to_column(collection_name),
                       key_schema
               });

        addColumnDefinitions(oid, columns);

    }


    auto add(const  std::string& db_name,
                 const  std::string& schema_name,
                 const  std::string& collection_name,
                 const std::vector<std::string>& column_names,
                 const std::vector<std::vector<std::string>>& rows){
/*
        auto key_db =
                getKeyByValue(CATALOG_DATABASE_NAME,
                              babrat::to_column(db_name));

        auto key_schema =
                getOidByValueIf(
                        CATALOG_SCHEMA_NAME,
                        babrat::to_column(schema_name),
                        [&key_db](Column* const& value){
                            return (value->equals(key_db));
                        }
                );

        auto key_table =
                getOidByValueIf(
                        CATALOG_COLLECTION_NAME,
                        babrat::to_column(collection_name),
                        [&key_schema](Column* const& value){
                            return (value->equals(key_schema));
                        }
                );

        auto table_fields = findPairsByColumnValue(
                CATALOG_FIELD_NAME,
                "table_oid",
                key_table
        );



        std::unordered_map<std::string, size_t> field_to_index;
        for (size_t i = 0; i < column_names.size(); ++i) {
            field_to_index[column_names[i]] = i;
        }

        std::vector<std::pair<std::optional<size_t>, ColumnDefinition>> index_mapping;


       for (auto field : table_fields) {

           auto column_name = get_value<std::string>(field.value[0]);

           auto column_type=
                  search(CATALOG_TYPE_NAME,field.value[2]);


           ColumnDefinition definition(column_name,
                                       get_value<std::string>(column_type[0]),
                                       get_value<bool>(field.value[3]),
                                       get_value<bool>(field.value[4]));

           if (field_to_index.count(column_name)) {

               index_mapping.emplace_back(
                       field_to_index[column_name],
                       definition);

           } else {
               index_mapping.emplace_back(
                       std::nullopt,
                       definition);
           }

        }


        std::vector<AssociativeContainer<Column*, std::vector<Column*>>::KeyValuePair>
                result;

        result.reserve(rows.size());

        for (auto i = 0; i < rows.size(); i++) {

            std::vector<Column*> primary_keys;
            Column* key = nullptr;
            std::vector<Column*> reordered_subvec;

            reordered_subvec.reserve(index_mapping.size());

            if (rows[i].size() != column_names.size()) {
                throw std::runtime_error("Row size doesn't match column names size");
            }

            for (auto j = 0; j < index_mapping.size(); j++) {

                auto second = index_mapping[j].second;

                Column* column = ColumnFactory::create(string_to_type(second.getColumnType()));


                if (index_mapping[j].first.has_value() && index_mapping[j].first.value() < rows[i].size()) {

             *//*       if (j == 0){
                        primary_keys.push_back(babrat::to_column(rows[i][index_mapping[j].first.value()]));
                        continue;
                    }*//*

                    if (rows[i][index_mapping[j].first.value()] != "NULL") {
                        column->insert(rows[i][index_mapping[j].first.value()]);
                        (second.isPrimaryKey()) ?
                        primary_keys.push_back(column) :
                        reordered_subvec.push_back(column);
                        continue;
                    }

                    if (!second.isNullable()){
                       throw std::runtime_error("Column must be not nullable due to nullable constraint");
                    }

                    if (second.isPrimaryKey()){
                        throw std::runtime_error("Key cannot be assigned a nullable constraint");
                    }

                    reordered_subvec.push_back(column);

                } else {

                    if (second.isPrimaryKey()){
                        throw std::runtime_error("the key cannot be empty if it is not serial");
                    }

                    if (!second.isNullable()){
                        throw std::runtime_error("Missing column must be not nullable");
                    }

                    reordered_subvec.push_back(column);
                }



            }

            if (primary_keys.empty()){
                throw std::runtime_error("does not support autoincrement for key yet");
            }

            if (primary_keys.size() > 1){
                throw std::runtime_error("does not support composite for key yet");
            }

            key = primary_keys.back();

            result.emplace_back(key, std::move(reordered_subvec));

        }


        return result;

}

private:

void addColumnDefinitions(Column* const &key_collection,
                          std::vector<ColumnDefinition> const &columns){


    std::unordered_set<std::string> added_columns;

    for(const auto & it : columns){

        auto column = it.getColumnType();

        if (added_columns.find(it.getColumnName()) != added_columns.end()) {
            throw std::runtime_error("Duplicate column: " + it.getColumnName());
        }

        auto key_type  = getKeyByValue(
                CATALOG_TYPE_NAME,
                babrat::to_column(column));

        auto oid = babrat::to_column(oidGen_->GetNewOid());

        added_columns.insert(it.getColumnName());

        insert(CATALOG_FIELD_NAME,
               oid,
               {
                       babrat::to_column(it.getColumnName()),  // name column table
                       key_collection, // key table
                       key_type, // oid base type
                       babrat::to_column(it.isNullable()), // primary key
                       babrat::to_column(it.isPrimaryKey()), // save data for nullable type
               });


    }

}

public:


void disposeDatabase(const std::string& db_name){

    auto key_db = getKeyByValue(
            CATALOG_DATABASE_NAME,
            babrat::to_column(db_name));

    remove(CATALOG_DATABASE_NAME, key_db);

}

void disposeSchema(
        const  std::string& db_name,
        const std::string& schema_name){

    auto key_db =
            getKeyByValue(CATALOG_DATABASE_NAME,
                          babrat::to_column(db_name));

    auto key_schema =
            getOidByValueIf(
                    CATALOG_SCHEMA_NAME,
                    babrat::to_column(schema_name),
                    [&key_db](Column* const& value){
                        return (value->equals(key_db));
                    }
            );

    remove(CATALOG_SCHEMA_NAME, key_schema);

}

void disposeCollection(
        const  std::string& db_name,
        const  std::string& schema_name,
        const  std::string& collection_name){

/*        auto key_db =
                getKeyByValue(CATALOG_DATABASE_NAME,
                              babrat::to_column(db_name));

        auto key_schema =
                getOidByValueIf(
                        CATALOG_SCHEMA_NAME,
                        babrat::to_column(schema_name),
                        [&key_db](Column* const& value){
                            return (value->equals(key_db));
                        }
                );

        auto key_table =
                getOidByValueIf(
                        CATALOG_COLLECTION_NAME,
                        babrat::to_column(collection_name),
                        [&key_schema](Column* const& value){
                            return (value->equals(key_schema));
                        }
                );

        auto table_fields = findPairsByColumnValue(
                CATALOG_FIELD_NAME,
                "table_oid",
                key_table
        );


        std::vector<ColumnDefinition> columns;


        for (auto field : table_fields) {

            auto column_name = get_value<std::string>(field.value[0]);

            auto column_type=
                    search(CATALOG_TYPE_NAME,field.value[2]);

            ColumnDefinition definition(column_name,
                                        get_value<std::string>(column_type[0]),
                                        get_value<bool>(field.value[3]),
                                        get_value<bool>(field.value[4]));

            columns.emplace_back(definition);

        }

        disposeCollection(collection_name,
                          columns);

}

void disposeCollection(const  std::string& collection_name,
                       std::vector<ColumnDefinition> const &columns){

    auto key = getKeyByValue(
            CATALOG_COLLECTION_NAME,
            babrat::to_column(collection_name));


    for(const auto & it : columns){

        auto key_field  = getKeyByValue(
                CATALOG_FIELD_NAME,
                babrat::to_column(it.getColumnName()));

        remove(CATALOG_FIELD_NAME,
               key_field);

    }

    remove(CATALOG_COLLECTION_NAME, key);

}



private:


void insert(
        const std::string& collection_name,
        Column* const &key,
        std::vector<Column*> const &value){

    storage_->add(
            db_parent_name_,
            CATALOG_NAME,
            collection_name,
            key,
            value);

}

void insert(
        const std::string& collection_name,
        Column* const &key,
        std::vector<Column*> &&value) {

    storage_->add(
            db_parent_name_,
            CATALOG_NAME,
            collection_name,
            key,
            std::move(value));

}

void update(
        Column* const &key,
        std::vector<Column*> const &value)  {

}

void update(
        Column* const &key,
        std::vector<Column*> &&value) {

}

std::vector<Column*> &search(
        const std::string& collection_name,
        Column* const &key)  {

    return storage_->search(
            db_parent_name_,
            CATALOG_NAME,
            collection_name,
            key);

}

void remove(
        const std::string& collection_name,
        Column* const &key)  {

    storage_->dispose(db_parent_name_,
                      CATALOG_NAME,
                      collection_name,
                      key);

}
 */


