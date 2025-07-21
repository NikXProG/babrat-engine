
#ifndef BABRAT_CREATE_SCHEMA_H
#define BABRAT_CREATE_SCHEMA_H

#include "sql_statement.h"
#include "ddl_statement.h"
#include "create_statement.h"

class CreateSchema final : public CreateStatement, public IDdlStatement {

    std::string m_authRole;

public:

    explicit CreateSchema(const std::string& schemaName,
                          bool isNotExists, std::string authRole)
            : CreateStatement(schemaName, isNotExists),
            m_authRole(std::move(authRole)) {}


    using CreateStatement::getName;
    using CreateStatement::isNotExists;


public:


    std::string const& getAuthRole() const & {
        return m_authRole;
    }

    std::string getAuthRole() && {
        return std::move(m_authRole);
    }


public:

    std::string toString() const override  {
        return StringAccumulator("CreateSchema")
                .accumulate("name", getName())
                .accumulate("isNotExists", isNotExists())
                .accumulate("authorizationRole", getAuthRole())
                .str();
    }


};


#endif //BABRAT_CREATE_SCHEMA_H
