#pragma once

#include <babrat/common/types/string.hpp>
#include <babrat/execution/ddl_command.hpp>

namespace babrat {

    namespace execution {

        class CreateStatementCommand : public DdlCommand {

            std::string m_name;
            bool m_isNotExists;

        protected:

            explicit CreateStatementCommand(
                    std::string name,
                    bool isNotExists):
                    m_name(std::move(name)),
                    m_isNotExists(isNotExists) {
                
                if (m_name.empty()) {
                    throw std::invalid_argument("Source name cannot be empty");
                }
                
            }

        public:

            const std::string& getName() const {
                return m_name;
            }

            bool isNotExists() const {
                return m_isNotExists;
            }

        public:

            CreateStatementCommand(const CreateStatementCommand&) = delete;
            CreateStatementCommand& operator=(const CreateStatementCommand&) = delete;

        public:

            CreateStatementCommand(CreateStatementCommand&&) = default;
            CreateStatementCommand& operator=(CreateStatementCommand&&) = default;

        public:

            ~CreateStatementCommand() override = default;

        };

    } // namespace execution

} // namespace babrat