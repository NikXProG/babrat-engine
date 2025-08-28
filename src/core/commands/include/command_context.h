//
// Created by NikX on 10.07.2025.
//

#ifndef BABRAT_DATABASE_CONTEXT_H
#define BABRAT_DATABASE_CONTEXT_H
#include "database_storage.h"
#include "catalog_default.h"

class CommandContext {

public:

    virtual ~CommandContext() noexcept = default;

public:

    virtual std::shared_ptr<ICatalog> getCatalog() const = 0;
    virtual std::shared_ptr<TypeStorage> getStorage() const = 0;

};



#endif //BABRAT_DATABASE_CONTEXT_H
