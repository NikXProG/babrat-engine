#pragma once

namespace babrat {

    namespace execution {

        class DdlCommandResult;
        class DdlExecutor;

        class DdlCommand {

        public:

            virtual ~DdlCommand() noexcept = default;

        public:

            virtual DdlCommandResult execute(DdlExecutor& executor) const = 0;

        };

    } // namespace execution

} // namespace babrat


