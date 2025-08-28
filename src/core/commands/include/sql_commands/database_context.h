#pragma once

#include <utility>

#include "../command_context.h"
#include "not_implemented.h"

class DatabaseContext : public CommandContext {

    std::shared_ptr<TypeStorage> m_db;
    std::shared_ptr<ICatalog> m_catalog;

public:

    explicit DatabaseContext(
            std::shared_ptr<DatabaseStorage> db,
            std::shared_ptr<Catalog> catalog) :
        m_db(std::move(db)),
        m_catalog(std::move(catalog)) {}

public:

    std::shared_ptr<TypeStorage> getStorage() const override{
        return m_db;
    }

    std::shared_ptr<ICatalog> getCatalog() const override {
        return m_catalog;
    }

};

