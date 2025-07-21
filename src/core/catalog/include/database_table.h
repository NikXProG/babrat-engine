#ifndef BABRAT_DATABASE_TABLE_H
#define BABRAT_DATABASE_TABLE_H


#include "system_table.h"
/*

class DatabaseTable : public SystemTable {


public:

    ~DatabaseTable() override = default;

public:

    explicit DatabaseTable(
            const std::string & pool_name,
            const std::string & schema_name,
            const std::string & collection_name,
            babrat::DatabaseStorage<Column*, std::vector<Column*>, Traits>* storage)
    : SystemTable(pool_name,schema_name,collection_name,storage){}

public:

    void init_default() override {

        register_column_name("oid");
        register_column_name("db_name");

       register_type(
                babrat::to_column(make_oid(BabratSystemOID::BABRAT_DB)),
                {
                    babrat::to_column(std::string(DB_DEFAULT_NAME))
                });

    }

};
*/

#endif //BABRAT_DATABASE_TABLE_H
