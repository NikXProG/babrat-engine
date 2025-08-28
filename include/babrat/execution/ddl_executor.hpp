#pragma once

namespace babrat {

    namespace execution  {

        class DdlCommandResult;
        class DdlCommand;
        class CreateTableCommand;
        class CreateSchemaCommand;
        class DropStatementCommand;

        class DdlExecutor {

        public:

            virtual ~DdlExecutor() noexcept = default;

        public:

            DdlCommandResult execute(DdlCommand& command);

        public:

            virtual DdlCommandResult executeCreateTable(
                    const CreateTableCommand& cmd)  = 0;

            virtual DdlCommandResult executeCreateSchema(
                    const CreateSchemaCommand& cmd)  = 0;

            virtual DdlCommandResult executeDropStatement(
                    const DropStatementCommand& cmd) = 0;

        };

        class DdlCommandExec : public DdlExecutor {

        public:

            DdlCommandResult executeCreateTable(
                    const CreateTableCommand& createTableCommand) override;

            DdlCommandResult executeCreateSchema(
                    const CreateSchemaCommand& createSchemaCommand) override;

            DdlCommandResult executeDropStatement(
                    const DropStatementCommand& dropStatementCommand) override;

        };

    } // namespace execution

} // namespace babrat


