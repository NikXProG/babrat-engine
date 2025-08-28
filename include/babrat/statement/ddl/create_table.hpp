#pragma once

#include <babrat/statement/ddl/create_statement.hpp>
#include <babrat/statement/common/column_definition_collection.hpp>
#include <babrat/statement/ddl/ddl_statement.hpp>

namespace babrat {

    namespace statement {

        class CreateTable final : public CreateStatement, public IDdlStatement {

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
                        .accumulate("columns", getColumnDefinitions().toString())
                        .accumulate("isNotExists", isNotExists())
                        .str();
            }


        };

    }

}

