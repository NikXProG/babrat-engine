#ifndef BABRAT_SCHEMA_TABLE_H
#define BABRAT_SCHEMA_TABLE_H

#include "system_table.h"
using namespace babrat;

/*

class SchemaTable : public SystemTable {

public:

    ~SchemaTable() override = default;

public:

    explicit SchemaTable(
            const std::string& pool_name,
            const std::string& schema_name,
            const std::string& collection_name,
            babrat::DatabaseStorage<Column*, std::vector<Column*>, Traits>* storage)
            : SystemTable(pool_name,schema_name,collection_name,storage){}

public:

    void init_default() override {

        register_column_name("oid");
        register_column_name(  "schema_name");
        register_column_name("database_oid");
        register_column_name( "owner_oid");

        register_type(
                babrat::to_column(make_oid(BabratSystemOID::BABRAT_NAMESPACE)),
                {
                    babrat::to_column(std::string(CATALOG_SCHEMA_NAME)),
                    babrat::to_column(make_oid(BabratSystemOID::BABRAT_DB)),
                    babrat::to_column(make_oid(BabratSystemOID::BABRAT_ROLE))
                });

    }

};
*/




#endif //BABRAT_SCHEMA_TABLE_H
