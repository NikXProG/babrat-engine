
#ifndef BABRAT_COLUMN_DEFINITION_H
#define BABRAT_COLUMN_DEFINITION_H

#include <babrat/statement/sql_statement.hpp>
#include <babrat/common/string_accumulator.hpp>

class ColumnDefinition : public SqlStatement {

    std::string m_columnName;
    std::string m_columnType;
    bool m_isNullable;
    bool m_isPrimaryKey;

public:

    explicit ColumnDefinition(
            std::string columnName,
            std::string columnType,
            bool isNullable,
            bool isPrimaryKey)  :
            m_columnName(std::move(columnName)),
            m_columnType(std::move(columnType)),
            m_isNullable(isNullable),
            m_isPrimaryKey(isPrimaryKey) {}

public:

    std::string const& getColumnName() const & {
        return m_columnName;
    }

    std::string getColumnName() && {
        return std::move(m_columnName);
    }

    std::string const& getColumnType() const & {
        return m_columnType;
    }

    std::string getColumnType() && {
        return std::move(m_columnType);
    }

    bool isPrimaryKey() const {
        return m_isPrimaryKey;
    }

    bool isNullable() const {
        return m_isNullable;
    }

public:

    bool operator==(const ColumnDefinition& other) const {
        return m_columnName == other.m_columnName &&
               m_columnType == other.m_columnType &&
               m_isNullable == other.m_isNullable &&
               m_isPrimaryKey == other.m_isPrimaryKey;
    }

    bool operator!=(const ColumnDefinition& other) const {
        return !(*this == other);
    }

public:

    std::string toString() const override {
        return StringAccumulator("ColumnDefinition")
               .accumulate("columnName", getColumnName())
               .accumulate("columnType", getColumnType())
               .accumulate("isNullable", isNullable())
               .accumulate("isPrimaryKey", isPrimaryKey())
               .str();
    }
};

#endif //BABRAT_COLUMN_DEFINITION_H
