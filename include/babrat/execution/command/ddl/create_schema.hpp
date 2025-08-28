#pragma once

#include <babrat/common/types/string.hpp>
#include <babrat/execution/command/ddl/create_statement.hpp>

namespace babrat {

    namespace execution {

        class DdlCommandResult;
        class DdlExecutor;

        class CreateSchemaCommand final : public CreateStatementCommand {

            string m_authRole;

        public:

            explicit CreateSchemaCommand(const string& schemaName,
                                         bool isNotExists,
                                         std::string authRole);

        public:

            const std::string& getRole() const {
                return m_authRole;
            }

        public:

            DdlCommandResult execute(DdlExecutor& executor) const override;

        };

/*

        class CreateSchemaCommand : public DdlCommand {

            string m_name;
            bool m_isNotExists;

        public:

            explicit CreateSchemaCommand(
                    const string& name,bool isNotExists);

        public:

            DdlCommandResult execute(DdlExecutor& executor) const override;

        public:

            const string& getName() const;

            bool isNotExists() const;

        };

        };*/

    } // namespace execution

} // namespace babrat



