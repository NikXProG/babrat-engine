
#ifndef BABRAT_CREATE_STATEMENT_H
#define BABRAT_CREATE_STATEMENT_H

#include <utility>
#include "sql_statement.h"

class CreateStatement : public SqlStatement {

    std::string m_name;
    bool m_isNotExists;

public:

    std::string m_currentDb;
    std::string m_currentSchema;

public:

    explicit CreateStatement(
            std::string name,
            bool isNotExists):
            m_name(std::move(name)),
            m_isNotExists(isNotExists) {
        if (m_name.empty()) {
            throw std::invalid_argument("Source name cannot be empty");
        }
        if (m_currentDb.empty()){

        }
    }

public:

    std::string const& getName() const & {
        return m_name;
    }

    std::string getName() && {
        return std::move(m_name);
    }

    bool isNotExists() const {
        return m_isNotExists;
    }

public:

    CreateStatement(const CreateStatement&) = delete;
    CreateStatement& operator=(const CreateStatement&) = delete;

public:

    CreateStatement(CreateStatement&&) = default;
    CreateStatement& operator=(CreateStatement&&) = default;

public:

    ~CreateStatement() override = default;

};
#endif //BABRAT_CREATE_STATEMENT_H
