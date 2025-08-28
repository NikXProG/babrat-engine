#ifndef BABRAT_CREATE_DATABASE_H
#define BABRAT_CREATE_DATABASE_H
#include <fmt/format.h>
#include "create_statement.h"
#include "../../common/string_accumulator.h"
#include "ddl_statement.h"

class CreateDatabase : public CreateStatement, public IDdlStatement {

public:

    explicit CreateDatabase(
            const std::string& dbName,
            bool isNotExists) :
            CreateStatement(dbName, isNotExists){}

public:

    using CreateStatement::getName;
    using CreateStatement::isNotExists;

public:

    std::string toString() const override {
        return StringAccumulator("CreateDatabase")
                .accumulate("name", getName())
                .accumulate("isNotExists", isNotExists())
                .str();
    }

};

#endif //BABRAT_CREATE_DATABASE_H
