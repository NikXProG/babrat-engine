#pragma once

#include <babrat/statement/ddl/ddl_statement.hpp>
#include <babrat/statement/ddl/create_statement.hpp>
#include <babrat/common/string_accumulator.hpp>

namespace babrat {

    namespace statement {

        class CreateSchema final : public CreateStatement, public IDdlStatement {

            std::string m_authRole;

        public:

            explicit CreateSchema(const std::string& schemaName,
                                  bool isNotExists,
                                  std::string authRole)
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

    } // namespace statement

} // namespace babrat


