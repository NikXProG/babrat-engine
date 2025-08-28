#pragma once

#include <babrat/common/babrat_api.hpp>
#include <babrat/logger/ctx_type.hpp>
#include <babrat/common/utils/string_utils.hpp>
#include <babrat/logger/common/logger_config.hpp>
#include <utility>

namespace babrat {

    class DatabaseInstance;
    class ClientContext;

    namespace logger {


        #define BABRAT_LOG_INTERNAL(SOURCE, CONTEXT, LEVEL, ...) \
        { \
        auto &loggerRef = babrat::logger::Logger::get(SOURCE); \
        if (loggerRef.shouldLog(LEVEL)) { \
        loggerRef.log(CONTEXT, LEVEL, __VA_ARGS__); \
        } \
        }

        #define BABRAT_LOG_TRACE(SOURCE, ...) \
        BABRAT_LOG_INTERNAL(SOURCE, DefaultContextType::NAME, LogLevel::Trace, __VA_ARGS__)

        #define BABRAT_LOG_DEBUG(SOURCE, ...) \
        BABRAT_LOG_INTERNAL(SOURCE, DefaultContextType::NAME, LogLevel::Debug, __VA_ARGS__)

        #define BABRAT_LOG_INFO(SOURCE, ...) \
        BABRAT_LOG_INTERNAL(SOURCE, DefaultContextType::NAME, LogLevel::Info, __VA_ARGS__)

        #define BABRAT_LOG_WARN(SOURCE, ...) \
        BABRAT_LOG_INTERNAL(SOURCE, DefaultContextType::NAME, LogLevel::Warn, __VA_ARGS__)

        #define BABRAT_LOG_ERROR(SOURCE, ...) \
        BABRAT_LOG_INTERNAL(SOURCE, DefaultContextType::NAME, LogLevel::Error, __VA_ARGS__)

        #define BABRAT_LOG_FATAL(SOURCE, ...) \
        BABRAT_LOG_INTERNAL(SOURCE, DefaultContextType::NAME, LogLevel::Critical, __VA_ARGS__)


        #define BABRAT_LOG(SOURCE, TYPE_CLASS, ...) \
        BABRAT_LOG_INTERNAL(SOURCE, TYPE_CLASS::NAME, TYPE_CLASS::LEVEL, \
        TYPE_CLASS::ConstructLogMessage(__VA_ARGS__))


        class LoggerManager;

        /**
          * @brief Main class to interact with the logging interface
          */
        class Logger {

        protected:

            LoggerManager& m_manager;

        public:

            BABRAT_API explicit Logger(LoggerManager& manager) :
                m_manager(manager) {}

        public:

            virtual ~Logger() noexcept = default;

        public:

            BABRAT_API virtual void log(
                const string& ctx,
                LogLevel level,
                const string& message) = 0;

            BABRAT_API virtual bool shouldLog(
                LogLevel level) = 0;

        public:

            BABRAT_API virtual void flush() = 0;

        public:

            BABRAT_API virtual const LoggerConfig& getConfig() const = 0;

            BABRAT_API virtual const std::string& getName() const = 0;

        public:

            /**
              * @brief main method for logs a formatted string with a variable number of arguments
              */
            template<typename... Args>
            void log(
                const string& ctx,
                LogLevel level,
                const string &format,
                Args... args)
            {
                log(ctx, level, utils::format(format, args...));
            }

        public:

            //===--------------------------------------------------------------------===//
            // separate methods for logging each level
            //===--------------------------------------------------------------------===//

            template <typename... Args>
            void trace(const string& fmt, Args &&...args) {
                log(DefaultContextType::NAME, LogLevel::Trace, fmt, std::forward<Args>(args)...);
            }

            template <typename... Args>
            void debug(const string& fmt, Args &&...args) {
                log(DefaultContextType::NAME, LogLevel::Debug, fmt, std::forward<Args>(args)...);
            }

            template <typename... Args>
            void info(const string& fmt, Args &&...args) {
                log(DefaultContextType::NAME, LogLevel::Info, fmt, std::forward<Args>(args)...);
            }

            template <typename... Args>
            void warn(const string& fmt,  Args &&...args) {
                log(DefaultContextType::NAME, LogLevel::Warn, fmt, std::forward<Args>(args)...);
            }

            template <typename... Args>
            void error(const string& fmt, Args &&...args) {
                log(DefaultContextType::NAME, LogLevel::Error, fmt, std::forward<Args>(args)...);
            }

            template <typename... Args>
            void critical(const string& fmt, Args &&...args) {
                log(DefaultContextType::NAME, LogLevel::Critical, fmt, std::forward<Args>(args)...);
            }

        public:

            BABRAT_API static Logger &get(const std::shared_ptr<Logger> &logger);

            BABRAT_API static Logger &get(const ClientContext &clientContext);

            BABRAT_API static Logger &get(const DatabaseInstance &db);


        };


        class DefaultLogger : public Logger {

        public:

            explicit DefaultLogger(const std::string& loggerName, LoggerConfig& config, LoggerManager &manager);

        public:

            void log(const string& ctx, LogLevel level, const string& message) override;

            bool shouldLog(LogLevel level) override;

        public:

            void flush() override;

        public:

            const LoggerConfig& getConfig() const override;

            const std::string& getName() const override;

        protected:

            std::string m_loggerName;

            const LoggerConfig m_config;

        };

        class EmptyLogger : public Logger {

        public:

            explicit EmptyLogger(LoggerManager &manager) : Logger(manager) {
            }

        public:

            void log(
                const std::string& ctx [[maybe_unused]],
                LogLevel level [[maybe_unused]],
                const string& message [[maybe_unused]]) override{
            }

            bool shouldLog(LogLevel level [[maybe_unused]]) override {
                return false;
            }

        public:

            void flush() override{
            }

        public:

            const LoggerConfig& getConfig() const override{
                throw std::runtime_error("LoggerConfig::getConfig() is not implemented");
            }

            const std::string& getName() const override
            {
                throw std::runtime_error("LoggerConfig::getName() is not implemented");
            }

        };

    } // namespace logger

} // nnamespace babrat