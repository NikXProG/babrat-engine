#include "gtest/gtest.h"
#include "create_database.h"
#include "create_schema.h"
#include "create_table.h"
#include "drop_database.h"
#include "drop_schema.h"
#include "drop_table.h"

TEST(DBStatementsTest, createStatementTest) {

    auto database = new CreateDatabase("table_db", false);

    auto columns = ColumnDefinitionCollection::create(
            ColumnDefinition(
                    "id",
                    "INT",
                    false,
                    true),
            ColumnDefinition(
                    "name",
                    "VARCHAR(255)",
                    true,
                    false)
    );

    auto table = new CreateTable(
            "table_test",
            columns,
            true);

    auto schema = new CreateSchema(
            "public",
            false,
            "user");

    auto schema2 = new CreateSchema(
            "my_schema",
            true,
            "new_user");

    std::cout << database ->toString() << std::endl;

    std::cout << schema->toString() << std::endl;

    std::cout << table->toString() << std::endl;

    std::cout << schema2->toString() << std::endl;

    auto database_drop = new DropDatabase(
            "table_db",
            true,
            true);

    auto schema_drop = new DropSchema(
            "table_db",
            true,
            true);

    auto table_drop = new DropTable(
            "table_db",
            true,
            true);

    std::cout << database_drop->toString() << std::endl;
    std::cout << schema_drop->toString() << std::endl;
    std::cout << table_drop->toString() << std::endl;

    delete database_drop;
    delete database;
    delete schema;
    delete schema2;
    delete table;

}