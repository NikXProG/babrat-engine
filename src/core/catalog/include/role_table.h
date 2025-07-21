#ifndef BABRAT_CLASS_TABLE_H
#define BABRAT_CLASS_TABLE_H

#include "system_table.h"

/*

class RoleTable : public SystemTable {

public:

    ~RoleTable() override = default;

public:

    explicit RoleTable(
            const std::string& pool_name,
            const std::string& schema_name,
            const std::string& collection_name,
            babrat::DatabaseStorage<Column*, std::vector<Column*>, Traits>* storage)
            : SystemTable(pool_name,schema_name,collection_name,storage){}

public:

    void init_default() override {

        register_column_name("oid");
        register_column_name(  "role_name");
        register_column_name("superuser");
        register_type(
                babrat::to_column(make_oid(BabratSystemOID::BABRAT_ROLE)), // oid
                {
                    babrat::to_column(std::string(DEFAULT_ROLE_NAME)), // babrat_super_user
                    babrat::to_column(true), // superuser
                });

    }

};

*/



#endif //BABRAT_CLASS_TABLE_H
