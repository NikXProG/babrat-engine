#ifndef BABRAT_TABLE_QUERY_H
#define BABRAT_TABLE_QUERY_H
#include <iostream>
#include "create_statement.h"
#include <optional>
#include "column_definition_collection.h"
#include "ddl_statement.h"


class CreateTable : public CreateStatement, public IDdlStatement {

    ColumnDefinitionCollection m_columnDefinitions;

public:

    explicit CreateTable(const std::string& tableName,
                         ColumnDefinitionCollection columnDefinitions,
                         bool isNotExists) :
            CreateStatement(tableName, isNotExists),
            m_columnDefinitions(std::move(columnDefinitions)) {}

public:

    using CreateStatement::getName;
    using CreateStatement::isNotExists;

public:

    const ColumnDefinitionCollection& getColumnDefinitions() const & {
        return m_columnDefinitions;
    }

    ColumnDefinitionCollection getColumnDefinitions() && {
        return std::move(m_columnDefinitions);
    }

public:

    std::string toString() const override {
        return StringAccumulator("CreateTable")
                .accumulate("name", getName())
                .accumulate("elements", getColumnDefinitions().toString())
                .accumulate("isNotExists", isNotExists())
                .str();
    }


};



#endif //BABRAT_TABLE_QUERY_H
