#pragma once

#include <babrat/statement/ddl/drop_statement.hpp>
#include <babrat/common/string_accumulator.hpp>
#include <babrat/statement/ddl/ddl_statement.hpp>

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
