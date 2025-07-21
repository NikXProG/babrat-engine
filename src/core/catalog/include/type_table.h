#ifndef BABRAT_TYPE_TABLE_H
#define BABRAT_TYPE_TABLE_H

#include "system_table.h"
/*

class TypeTable : public SystemTable {

public:

    ~TypeTable() override = default;

public:

    explicit TypeTable(
            const std::string& pool_name,
            const std::string& schema_name,
            const std::string& collection_name,
            babrat::DatabaseStorage<Column*, std::vector<Column*>, Traits>* storage)
            : SystemTable(pool_name,schema_name,collection_name,storage){}

public:

    void init_default() override {

        register_column_name(  "type_name");
        register_column_name("type_namespace");
        register_column_name( "type_role");

        register_basic_type(BabratSystemOID::BOOL,
                            babrat::to_string(BabratType::BOOL));
        register_basic_type(BabratSystemOID::INTEGER,
                            babrat::to_string(BabratType::INTEGER));
        register_basic_type(BabratSystemOID::UINT32,
                            babrat::to_string(BabratType::UINT32));
        register_basic_type(BabratSystemOID::TEXT,
                            babrat::to_string(BabratType::TEXT));
        register_basic_type(BabratSystemOID::VARCHAR,
                            babrat::to_string(BabratType::VARCHAR));


    }


    void register_basic_type(BabratSystemOID type,
                             const std::string& type_name) {

        auto id = make_oid(type);

        auto a = babrat::to_column(id);

        register_type(
                babrat::to_column(id),
                {
                        babrat::to_column(type_name),
                        babrat::to_column(
                                make_oid(BabratSystemOID::BABRAT_NAMESPACE)),
                        babrat::to_column(
                                make_oid(BabratSystemOID::BABRAT_ROLE))
                });

    }




};
*/


#endif //BABRAT_TYPE_TABLE_H
