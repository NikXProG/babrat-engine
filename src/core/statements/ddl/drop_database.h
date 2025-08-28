#ifndef BABRAT_DROP_DATABASE_H
#define BABRAT_DROP_DATABASE_H

#include "drop_statement.h"
#include "string_accumulator.h"
#include "ddl_statement.h"

class DropDatabase : public DropStatement, public IDdlStatement  {

    bool m_isForce;

public:

    explicit DropDatabase(
            const std::string& dbName,
            bool isExists,
            bool isForce) :
            DropStatement(dbName, isExists),
            m_isForce(isForce) {}

public:

    using DropStatement::getName;
    using DropStatement::isExists;

public:

    bool isForce() const {
        return m_isForce;
    }

public:

    std::string toString() const override {
        return StringAccumulator("DropDatabase")
            .accumulate("name", getName())
            .accumulate("isExists", isExists())
            .accumulate("isForce", isForce())
            .str();
    }


};


#endif //BABRAT_DROP_DATABASE_H
