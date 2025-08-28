#pragma once

#include <babrat/common/types/string.hpp>

namespace babrat {

    namespace execution {

        class DdlCommandResult {

            bool m_success;
            string m_message;

        public:

            explicit DdlCommandResult(bool success, const string& message);

        public:

            bool isSuccess() const;

            const string& getMessage() const;

        };

    } // namespace execution

} // namespace babrat

