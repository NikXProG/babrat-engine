#pragma once

#include <babrat/common/types/string.hpp>
#include "babrat/common/utils/string_utils.hpp"
#include <babrat/logger/common/logger_config.hpp>
#include <utility>

namespace babrat {

    namespace logger {

        class ContextType {

        private:

            string m_typeName;

            [[maybe_unused]] LogLevel m_logLevel;

        public:

            explicit ContextType(const std::string& typeName, const LogLevel& logLevel)
                : m_typeName(typeName), m_logLevel(logLevel) {
            }
        };

        class DefaultContextType : public ContextType {

        public:

            static constexpr std::string NAME = "";
            static constexpr LogLevel LEVEL = LogLevel::Info;

            DefaultContextType() : ContextType(NAME, LEVEL) {
            }

        };

        //===----------------------------------------------------------------------===//
        // Basic Context Types
        //===----------------------------------------------------------------------===//

        class DatabaseContextType : public ContextType {
        public:
            static constexpr const char *NAME = "DATABASE";
            static constexpr LogLevel LEVEL = LogLevel::Info;

            DatabaseContextType() : ContextType(NAME, LEVEL) {}

            static string ConstructLogMessage(
                const string& databaseStr) {
                return databaseStr;
            }

        };

        class ConnectionContextType : public ContextType {
        public:
            static constexpr const char *NAME = "CONNECTION";
            static constexpr LogLevel LEVEL = LogLevel::Debug;

            ConnectionContextType() : ContextType(NAME, LEVEL) {}

            static string ConstructLogMessage(
                const string& connectId,
                const string& details) {
                return utils::format(
                    "connection with id `{}` called query `{}`",
                    connectId,
                    details);
            }
        };

        class QueryContextType : public ContextType {

        public:

            static constexpr const char *NAME = "QUERY";
            static constexpr LogLevel LEVEL = LogLevel::Info;

            QueryContextType() : ContextType(NAME, LEVEL) {}

            static string ConstructLogMessage(const string& queryStr) {
                return queryStr;
            }
        };

        class StorageContextType : public ContextType {

        public:

            static constexpr const char *NAME = "STORAGE";
            static constexpr LogLevel LEVEL = LogLevel::Trace;

            StorageContextType() : ContextType(NAME, LEVEL) {}

            static string ConstructLogMessage(
                const string& details) {
                return utils::format(
                    "From context database storage called: {}",
                    details);
            }

        };


    } // namespace logger

} // namespace babrat
