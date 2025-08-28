#include <babrat/execution/ddl_command_result.hpp>


namespace babrat {

    namespace execution {

        DdlCommandResult::DdlCommandResult(bool success, const std::string &message) :
                m_success(success),
                m_message(message) {
            if (message.empty()) {
                throw std::invalid_argument("Message is empty");
            }
        }

        const string &DdlCommandResult::getMessage() const  {
            return m_message;
        }

        bool DdlCommandResult::isSuccess() const {
            return m_success;
        }


    } // namespace execution

} // namespace babrat

