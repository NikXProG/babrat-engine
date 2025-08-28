


/*#ifndef BABRAT_COMMAND_H
#define BABRAT_COMMAND_H

#include "sql_statement.h"
#include "catalog.h"*/
/*
enum TypeModel{
    //ddl
    CREATE_TABLE,
    CREATE_NAMESPACE,
    CREATE_DATABASE,
    DROP_DATABASE,
    DROP_NAMESPACE,
    DROP_TABLE,
    //dml
    INSERT,
    DELETE,
    UPDATE,
    SELECT
};*/
/*

class DatabaseContext {

    Catalog* catalog_;
    babrat::DatabaseStorage<Column*, std::vector<Column*>, Traits>* db_;

public:

    explicit DatabaseContext(
         Catalog* catalog,
         babrat::DatabaseStorage<Column*, std::vector<Column*>, Traits>* db)
            : catalog_(catalog), db_(db) {
    }

    [[nodiscard]] auto* getCatalog() const { return catalog_; }
    [[nodiscard]] auto* getDatabase() const { return db_; }

};
*/
/*


class Command  {

public:

    virtual ~Command() = default;

public:

    virtual void execute() = 0;
    virtual void undo() = 0;

};
*/




/*

#endif //BABRAT_COMMAND_H
*/
