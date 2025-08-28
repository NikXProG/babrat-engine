#pragma once

#include <babrat/statement/ddl/create_table.hpp>
#include <babrat/execution/ddl_executor.hpp>
#include <babrat/execution/command/ddl/create_table.hpp>
#include <babrat/
namespace babrat {

    namespace execution {

        class CreateTableFactory final {

            std::shared_ptr<CommandContext> m_context;

        public:

            explicit CreateTableFactory(
                    std::shared_ptr<CommandContext> context)
                    : m_context(std::move(context)) {}

        public:

            std::unique_ptr<CreateTableCommand> create(statement::CreateTable const& statement) {
                return create(
                        statement.getName(),
                        statement.isNotExists()
                );
            }


        private:

            std::unique_ptr<CreateTableCommand> create(
                    std::string const& sourceName,
                    bool ifExists)
            {
                return std::make_unique<CreateTableCommand>(
                        sourceName, ifExists);
            }


        };



    }

}