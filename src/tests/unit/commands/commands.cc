#include "gtest/gtest.h"
#include "catalog_default.h"
#include "database_storage.h"
#include "database_context.h"
#include "db_babrat_controller.h"
#include "exception_util.h"

typedef int key_t;
typedef int value_t;
using namespace storage;

#pragma once

#include <cstdint>
#include <string>
#include <vector>

#include <fmt/format.h>
#include <regex>

uint64_t currentTimestamp(){
    return convert::timestamp_to_uint64(std::chrono::system_clock::now());
}

TEST(Command, testCommandProcess) {
    auto storage =
            std::make_shared<babrat::storage::DatabaseStorage<
                    key_t,
                    value_t,
                    std::less<>>>();

    storage->addPool("sds");

    storage->addSchema("sds",
                       "sdsdssssd");

    storage->addCollection(
            "sds",
            "sdsdssssd",
            "sdsdssds");

    for(int i = 0; i < 5; i++){

        storage->add("sds",
                     "sdsdssssd",
                     "sdsdssds",
                     i,
                     i);

    }
    storage->persist(1753112097337770);

  /* auto storage2 = std::make_shared<babrat::storage::DatabaseStorage<
            key_t,
            value_t,
            std::less<>>>();

    bool isNormal = storage2->restore(1753112097337770, true);

    if (isNormal){
        std::cout << "true" << std::endl;
    }else{
        std::cout << "false" << std::endl;
    }

    auto value2= storage2->search(
            "sds",
            "sdsdssssd",
            "sdsdssds",
            21);

    std::cout << value2 << std::endl;
*/

   /* storage2->restore(1753112097337770, true);

    storage->add("sds",
                 "sdsdssssd",
                 "sdsdssds",
                 100,
                 121);

    storage2->restore(1753112097337770, true);

    auto value2= storage2->search(
            "sds",
            "sdsdssssd",
            "sdsdssds",
            4);

    std::cout << value2 << std::endl;
*/

/*

    auto storage =
            std::make_shared<babrat::storage::DatabaseStorage<
                    key_t,
                    value_t>>();

    try{

        storage->addPool("sds");

        storage->addSchema("sds",
                           "sdsdssssd");

        storage->addCollection(
                    "sds",
                    "sdsdssssd",
                        "sdsdssds");

        storage->add(
                "sds",
                "sdsdssssd",
                "sdsdssds",
                4,
                4);


        auto value = storage->search(
                "sds",
                "sdsdssssd",
                "sdsdssds",
                4);

        std::cout << value << std::endl;
*/

/*        for(int i = 0; i < 200; i++){

            std::cout << i << std::endl;

            storage->add("sds",
                         "sdsdssssd",
                         "sdsdssds",
                         {

                         });
        }

        auto a = storage->search("sds",
                                 "sdsdssssd",
                                 "sdsdssds",
                         babrat::convert::to_column(4));*/

/*        std::cout << a.size();*/

/*
        storage->dispose("sds",
                         "sdsdssssd",
                         "sdsdssds",
                         babrat::convert::to_column(4));
*/

   /* }catch(const babrat::BabratStorageError& ex){
        std::cout << ENUM_TO_STRING(ex.getType())
        << ": " <<
        ex.what() << std::endl;
    }*/


  /*  auto storage =
            std::make_shared<babrat::storage::DatabaseStorage>();

    auto catalog = std::make_shared<babrat::db::Catalog>(storage);

    auto context =
            std::make_shared<DatabaseContext>(storage, catalog);
*/
   /* auto db = new babrat::db::DbController(
            "user",
            "password",
            "db_name"
            );

    std::cout << db->getDatabase() << std::endl;*/

/*
    auto catalog = new Catalog(storage);

    catalog->init();

    auto context =
            std::make_shared<DatabaseContext>(storage, *catalog);

    auto factory = new DdlCommandFactories(context);

    auto table_command = factory->create(
            DropTable("my_table", true, true));

    auto schema_command = factory->create(
            CreateTable("my_schema",
                        ColumnDefinitionCollection::create(
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
                        ), true));



    auto result = schema_command->execute();

    auto result2 = table_command->execute();

    std::cout << result.getMessage() << std::endl;*/

}