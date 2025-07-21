#ifndef BABRAT_DROP_TABLE_H
#define BABRAT_DROP_TABLE_H
#include <iostream>
#include "drop_statement.h"
#include "string_accumulator.h"
#include "ddl_statement.h"

class DropTable : public DropStatement, public IDdlStatement  {

    bool m_isCascade;

public:

    explicit DropTable(
            const std::string& tableName,
            bool isExists,
            bool isCascade) :
            DropStatement(tableName, isExists),
            m_isCascade(isCascade) {}

public:

    using DropStatement::getName;
    using DropStatement::isExists;

public:

    bool isCascade() const {
        return m_isCascade;
    }

public:

    std::string toString() const override {
        return StringAccumulator("DropTable")
                .accumulate("name", getName())
                .accumulate("isExists", isExists())
                .accumulate("isCascade", isCascade())
                .str();
    }


};



#endif //BABRAT_DROP_TABLE_H
