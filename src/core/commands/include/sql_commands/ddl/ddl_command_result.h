
#ifndef BABRAT_DDL_COMMAND_RESULT_H
#define BABRAT_DDL_COMMAND_RESULT_H

#include <string>

class DdlCommandResult {

    bool m_success;
    std::string m_message;

public:

    explicit DdlCommandResult(bool success, const std::string& message) :
            m_success(success),
            m_message(message) {
        if (message.empty()) {
            throw std::invalid_argument("Message is empty");
        }
    }

public:

    bool isSuccess() const {
        return m_success;
    }

    const std::string& getMessage() const {
        return m_message;
    }

};

#endif //BABRAT_DDL_COMMAND_RESULT_H
