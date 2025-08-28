
#include <iostream>
#include <babrat/logger/logger.hpp>
#include <babrat/logger/logger_manager.hpp>
#include <babrat/common/types/timestamp.hpp>

#include <babrat/main/database.hpp>
#include <babrat/main/client_context.hpp>

#include <utility>

namespace babrat {

    namespace logger {


        DefaultLogger::DefaultLogger(
            const std::string& loggerName, LoggerConfig& config, LoggerManager& manager) :
            Logger(manager),
            m_loggerName(loggerName),
            m_config(config) {
        }

        void DefaultLogger::log(const std::string& ctx, LogLevel level, const std::string& message)
        {
            m_manager.log(m_loggerName, ctx, getCurrentTimestamp(), level, message);
        }

        bool DefaultLogger::shouldLog(LogLevel level)
        {
            if (m_config.level > level) {
                return false;
            }

            return true;
        }

        void DefaultLogger::flush()
        {
           m_manager.flush();
        }

        const LoggerConfig& DefaultLogger::getConfig() const
        {
            return m_config;
        }

        const std::string& DefaultLogger::getName() const
        {
            return m_loggerName;
        }


        Logger& Logger::get(const std::shared_ptr<Logger>& logger)
        {
            return *logger;
        }

        Logger &Logger::get(const babrat::ClientContext &clientContext) {
            return clientContext.getLogger();
        }

        Logger &Logger::get(const babrat::DatabaseInstance &db) {
            return db.getLoggerManager().globalLoggerReference();
        }



    } // namespace logger

} // namespace babrat