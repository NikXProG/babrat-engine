#ifndef BABRAT_SYSTEM_TABLE_H
#define BABRAT_SYSTEM_TABLE_H
#include <iostream>

#include "convert_column.h"
/*
#define DB_DEFAULT_NAME "babrat"
#define DEFAULT_ROLE_NAME "babrat_admin"*/


/*#define CATALOG_NAME ("bbrt_settings")
#define CATALOG_TYPE_NAME "bbrt_types"
#define CATALOG_SCHEMA_NAME "bbrt_schemas"
#define CATALOG_ROLE_NAME "babrat_roles"
#define CATALOG_DATABASE_NAME "babrat_databases"
#define CATALOG_FIELD_NAME "babrat_fields"
#define CATALOG_COLLECTION_NAME "babrat_tables"
#define CATALOG_AUTH_NAME "babrat_auth"*/

namespace babrat{

    const std::string DB_DEFAULT_NAME("babrat");

    const std::string DEFAULT_ROLE_NAME("babrat_admin");

    const std::string DEFAULT_CATALOG_NAME("catalog_registry");

    const std::string CATALOG_TYPE_NAME("bbrt_types");

    const std::string CATALOG_SCHEMA_NAME("bbrt_schemas");

    const std::string CATALOG_ROLE_NAME("babrat_roles");

    const std::string CATALOG_DATABASE_NAME("babrat_databases");

    const std::string CATALOG_FIELD_NAME("babrat_fields");

    const std::string CATALOG_COLLECTION_NAME("babrat_tables");

    const std::string CATALOG_AUTH_NAME("babrat_auth");

}

/**
 * @brief System table base interface
 *
 * Provides interface for registering registry_types in the system catalog
 *//*
class SystemTableBase {
public:
    virtual ~SystemTableBase() = default;

    *//**
     * @brief Register a new type in the system catalog
     * @param key The key column for the type
     * @param value Vector of columns describing the type
     * @throws std::invalid_argument if type already exists
     *//*
    virtual void register_type(
            Column* key,
            std::vector<Column*> value) = 0;
};*/


class SystemTable {

public:

    virtual ~SystemTable() noexcept = default;

};
/*

class SystemTable : public SystemTableBase {

    std::vector<std::string> columns_names_;

protected:

    std::string pool_name_;
    std::string schema_name_;
    std::string collection_name_;
    babrat::DatabaseStorage<Column*, std::vector<Column*>, Traits>* db_;

public:

    ~SystemTable() override = default;

protected:

    explicit SystemTable(
            const std::string & pool_name,
            const std::string & schema_name,
            const std::string & collection_name,
            babrat::DatabaseStorage<Column*, std::vector<Column*>, Traits>* storage)
            : db_(storage){

        db_->addBatch(
                pool_name,
                schema_name,
                collection_name);

        this->pool_name_ = pool_name;
        this->schema_name_ = schema_name;
        this->collection_name_ = collection_name;

    }

protected:

    void register_type(Column* key, std::vector<Column*> value) override {
        db_->add(pool_name_,
                 schema_name_,
                 collection_name_,
                 key,
                 value);
    }

    void register_column_name(const std::string& meta) {
        columns_names_.push_back(meta);
    }

public:

    virtual void init_default() = 0;

    [[nodiscard]] std::string const& get_type() const {
        return collection_name_;
    }

    [[nodiscard]] std::vector<std::string> const& get_column_names() const {
        return columns_names_;
    }


};

*/



#endif //BABRAT_SYSTEM_TABLE_H
