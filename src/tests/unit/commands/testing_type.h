//
// Created by NikX on 13.06.2025.
//

#ifndef BABRAT_TESTING_TYPE_H
#define BABRAT_TESTING_TYPE_H

#include "ddl/create_table.h"
#include "ddl/create_database.h"
#include "dml/insert_query_model.h"

#include <typeindex>
#include "db_types.h"
#include <algorithm>
#include <string>
#include <cctype>

/*
enum class DdlCommandType : uint8_t {
    CREATE,
    DROP,
    ALTER,
    TRUNCATE
};*/
/*
class SqlCommandTest {

public:
    virtual ~SqlCommandTest() = default;

public:

    virtual void execute() = 0;
    virtual void undo() = 0;

};

class TableSqlCommandTest : public SqlCommandTest {

    TableQueryModel model_;
    DdlCommandType type;

public:

    explicit TableSqlCommandTest(
            DdlCommandType type,
            const TableQueryModel& model) :
     type(type), model_(model) {}

    explicit TableSqlCommandTest(
            DdlCommandType type,
            TableQueryModel&& model) :
      type(type), model_(std::move(model)) {}

public:

    ~TableSqlCommandTest() override = default;

public:

    void execute() override{

        switch(type){
            case DdlCommandType::CREATE:
                std::cout << "CREATE tableSQlCOmmand" << std::endl;
                return;
            case DdlCommandType::DROP:
                std::cout << "DROP tableSQlCOmmand" << std::endl;
                return;
            case DdlCommandType::ALTER:
                std::cout << "ALTER tableSQlCOmmand" << std::endl;
                return;
            case DdlCommandType::TRUNCATE:
                std::cout << "TRUNCATE tableSQlCOmmand" << std::endl;
                return;
        }

        throw std::runtime_error("This type is not supported");

    }

     void undo() override {
        std::cout << "undo last command" << std::endl;
    }


};


class SchemaSqlCommandTest : public SqlCommandTest {

    CreateSchema model_;
    DdlCommandType type;

public:

    explicit SchemaSqlCommandTest(
            DdlCommandType type,
            const CreateSchema& model) :
            type(type), model_(model) {}

    explicit SchemaSqlCommandTest(
            DdlCommandType type,
            CreateSchema&& model) :
            type(type), model_(std::move(model)) {}

public:

    ~SchemaSqlCommandTest() override = default;

public:

    void execute() override{

        switch(type){
            case DdlCommandType::CREATE:
                std::cout << "CREATE SCHEMA" << std::endl;
                return;
            case DdlCommandType::DROP:
                std::cout << "DROP SCHEMA" << std::endl;
                return;
            case DdlCommandType::ALTER:
                std::cout << "ALTER  SCHEMA" << std::endl;
                return;
            case DdlCommandType::TRUNCATE:
                std::cout << "TRUNCATE  SCHEMA" << std::endl;
                return;
        }

        throw std::runtime_error("This type is not supported");

    }

    void undo() override {
        std::cout << "undo last command" << std::endl;
    }


};*/


/*

class DatabaseCommandTest : public SqlCommandTest {

    CreateDatabase model_;
    DdlCommandType type;

public:

    explicit DatabaseCommandTest(DdlCommandType type, const CreateDatabase& model) : type(type), model_(model) {}

    explicit DatabaseCommandTest(DdlCommandType type, CreateDatabase&& model) : type(type), model_(std::move(model)) {}

public:

    ~DatabaseCommandTest() override = default;

public:

    void execute() override{

        switch(type){
            case DdlCommandType::CREATE:
                std::cout << "CREATE database" << std::endl;
                return;
            case DdlCommandType::DROP:
                std::cout << "DROP database" << std::endl;
                return;
            case DdlCommandType::ALTER:
                std::cout << "ALTER database" << std::endl;
                return;
            case DdlCommandType::TRUNCATE:
                std::cout << "TRUNCATE database" << std::endl;
                return;
        }

    }

    void undo() override {
        switch(type){
            case DdlCommandType::CREATE:
                std::cout << "UNDO CREATE database" << std::endl;
                return;
            case DdlCommandType::DROP:
                std::cout << "UNDO DROP database" << std::endl;
                return;
            case DdlCommandType::ALTER:
                std::cout << "UNDO ALTER database" << std::endl;
                return;
            case DdlCommandType::TRUNCATE:
                std::cout << "UNDO TRUNCATE database" << std::endl;
                return;
        }
    }


};
*/

/*

class InsertCommandTest : public SqlCommandTest {

    InsertQueryModel model_;

public:

    explicit InsertCommandTest(const InsertQueryModel& model) :  model_(model) {}

    explicit InsertCommandTest(InsertQueryModel&& model) : model_(std::move(model)) {}

public:

    ~InsertCommandTest() override = default;

public:

    void execute() override{

        std::cout << "Insert database" << std::endl;
    }

    void undo() override {
        std::cout << "UNDO Insert database" << std::endl;
    }


};
*/



/*
bool str_compare(const std::string& str1, const std::string& str2)
{
    return str1.size() == str2.size() &&
    std::equal(
            str1.begin(), str1.end(), str2.begin(),
            [](unsigned char c1, unsigned char c2)
    {
        return std::tolower(c1) == std::tolower(c2);
    });
}

enum class DbColumnType { INT, UINT, STRING, FIXED_STRING };

DbColumnType convert_db_enum(const std::string& db_type) {

    if (str_compare(db_type, "INT32")) {
        return DbColumnType::INT;
    }
    if (str_compare(db_type, "UINT32")) {
        return DbColumnType::UINT;
    }
    if (str_compare(db_type, "TEXT")) {
        return DbColumnType::STRING;
    }
    if (str_compare(db_type, "VARCHAR")) {
        return DbColumnType::FIXED_STRING;
    }

    throw std::runtime_error("Unsupported DB type: " + db_type);
}

DbTypeBase* create_column(DbColumnType type,
                                          const std::string& value,
                                          size_t length = 0) {
    switch (type) {
        case DbColumnType::INT:
            return new DbInt(std::stoi(value));
        case DbColumnType::UINT:
            return  new DbUint(static_cast<unsigned int>(std::stoul(value)));
        case DbColumnType::STRING:
            return new DbString(value);
        case DbColumnType::FIXED_STRING:
            return new DbFixedString(value, length);
        default:
            throw std::invalid_argument("Unknown column type");
    }

}


struct IntTag {};
struct UintTag {};
struct StringTag {};
struct FixedStringTag {};


inline DbTypeBase* create_column(IntTag, int value) {
    return new DbInt(value);
}

inline DbTypeBase* create_column(UintTag, unsigned int value) {
    return new DbUint(value);
}

inline DbTypeBase* create_column(StringTag, const std::string& value) {
    return new DbString(value);
}

inline DbTypeBase* create_column(FixedStringTag, const std::string& value, size_t length) {
    return new DbFixedString(value, length);
}*/



#endif //BABRAT_TESTING_TYPE_H
