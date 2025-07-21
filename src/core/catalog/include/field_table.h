#ifndef BABRAT_BBRT_FIELDS_H
#define BABRAT_BBRT_FIELDS_H



#include "system_table.h"
/*
class FieldTable : public SystemTable {


public:

    ~FieldTable() override = default;

public:

    explicit FieldTable(
            const std::string& pool_name,
            const std::string& schema_name,
            const std::string& collection_name,
            babrat::DatabaseStorage<Column*, std::vector<Column*>, Traits>* storage)
            : SystemTable(pool_name,schema_name,collection_name,storage){}

public:

    void init_default() override {

        register_column_name( "field_name");
        register_column_name("table_oid");
        register_column_name( "type_oid");
        register_column_name( "is_null");
        register_column_name( "is_primary");

    }

};*/



#endif //BABRAT_BBRT_FIELDS_H
