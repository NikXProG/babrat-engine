#pragma once

#include "catalog.h"
#include "persist_storage.h"
#include "db_enums.h"

namespace babrat {

    class Catalog : public ICatalog {

        std::shared_ptr<TypeStorage> m_storage;

        std::unique_ptr<DefaultOidGenerator> m_oidGen;

    public:

        ~Catalog() noexcept override = default;

    public:

        explicit Catalog(std::shared_ptr<TypeStorage> db) :
                m_storage(std::move(db)),
                m_oidGen(std::make_unique<DefaultOidGenerator>()) {}

    public:

        void init() override {

            auto a = CreateSchema(
                    DEFAULT_CATALOG_NAME,
                    false,
                    DEFAULT_ROLE_NAME);

            buildSchema(a);

        }

    private:

        std::string const& GetDatabase(){
            return DB_DEFAULT_NAME;
        }

        Oid getOidByColumn(
                Column* column,
                const std::string& catalog_name) override {
            return 0;
        }

        std::string const& GetSchema(){
            return "default";
        }

        bool buildSchema(CreateSchema& schema){

            m_storage
                    ->addSchema(GetDatabase(),
                                schema.getName());

            return true;
        }

        bool buildTable(const CreateTable& table){

            m_storage
                    ->addCollection(GetDatabase(),
                                    "public",
                                    table.getName());


            return true;
        }





    };




}



