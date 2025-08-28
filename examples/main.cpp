/*
#include <babrat/babrat.hpp>
*/

/*using namespace babrat;
using namespace statement;*/
/*
#include <babrat/execution/ddl_executor.hpp>
#include <babrat/execution/command/ddl/create_table.hpp>
#include <babrat/execution/command/ddl/create_schema.hpp>*/
/*using namespace execution;*/

#include <babrat/babrat.hpp>

#include <babrat/planner/planner.hpp>


using namespace babrat;
using namespace statement;

int main()
{

  /*  Database db(nullptr);

    Connection conn(db);*/

 /*   createDatabase->query =
            "CREATE DATABASE IF NOT EXISTS new_db";

    auto w = conn.query(
            std::move(createDatabase));
*/

   /* std::cout <<  w->getMessage() << std::endl;*/

    Database db(nullptr);

    Connection conn(db);

    std::unique_ptr<statement::SqlStatement> createDatabase = std::make_unique<statement::CreateDatabase>(
            "sadsadadas",true);

    createDatabase->query =
            "CREATE DATABASE IF NOT EXISTS new_db";

    auto planner =
            std::make_unique<babrat::planner::Planner>(*conn.m_clientContext);

    auto sql = planner->validate(
            std::move(createDatabase));

    planner->plan(
            std::move(sql));


    return 0;

}

/*  Connection conn(db);

    auto w =conn.query(
            std::make_unique<statement::CreateDatabase>(
                    "sadsadadas",true));

    std::cout <<  w->getMessage() << std::endl;
*/
/*


    auto w =conn.query(
            std::make_unique<statement::CreateDatabase>(
            "sadsadadas",true));

    std::cout <<  w->getMessage() << std::endl;*/
/*

    DdlExecutor* exec = new DdlCommandExec();

    auto* createSchemaCommand  = new CreateSchemaCommand(
            "table",
            true,
            "my-role");

    exec->execute(*createSchemaCommand);
*/

/*auto* createTableCommand  = new CreateTableCommand(
        "table",
        true);

auto er = exec->execute(*createTableCommand);

std::cout << er.getMessage() <<  std::endl;

std::cout << er.isSuccess() << std::endl;
*/

/*#include "babrat/statement/ddl/create_table.hpp"
#include "babrat/storage/database_storage.hpp"

#include <babrat/babrat.hpp>
#include <babrat/statement/ddl/create_database.hpp>*/

/*class Person {

public:

    std::string name;
    int age;
    std::uint64_t timestamp;

public:

    void serializeTo(serialization::Serializer& ser) const {
        ser.writeValue("name", name);
        ser.writeValue("age", age);
        ser.writeValue("timestamp", timestamp);
    }

};*/
