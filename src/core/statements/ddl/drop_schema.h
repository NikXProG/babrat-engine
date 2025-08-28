
#ifndef BABRAT_DROP_SCHEMA_H
#define BABRAT_DROP_SCHEMA_H

#include "drop_statement.h"
#include "string_accumulator.h"
#include "ddl_statement.h"

class DropSchema : public DropStatement, public IDdlStatement {

    bool m_isCascade;

public:

    explicit DropSchema(const std::string& schemaName,
                        bool isExists,
                        bool isCascade) :
            DropStatement(schemaName, isExists),
            m_isCascade(isCascade){}

public:

    using DropStatement::getName;
    using DropStatement::isExists;

public:

    bool isCascade() const {
        return m_isCascade;
    }

public:

    std::string toString() const override {
        return StringAccumulator("DropSchema")
                .accumulate("name", getName())
                .accumulate("isExists", isExists())
                .accumulate("isCascade", isCascade())
                .str();
    }


};
#endif //BABRAT_DROP_SCHEMA_H
