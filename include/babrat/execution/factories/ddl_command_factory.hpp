#pragma once

#include <memory>
#include <babrat/statement/ddl/ddl_statement.hpp>
#include <babrat/execution/ddl_command.hpp>

namespace babrat {

    namespace execution {

        class DdlCommandFactory {

        public:

            virtual ~DdlCommandFactory() = default;

            virtual std::unique_ptr<DdlCommand> create(const statement::IDdlStatement& statement) = 0;

        };

    } // namespace execution

} // namespace babrat




