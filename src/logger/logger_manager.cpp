

#include <iostream>
#include <babrat/main/client_context.hpp>
#include <babrat/logger/logger.hpp>
#include <babrat/logger/logger_manager.hpp>
#include <babrat/logger/sinks/logger_sink.hpp>
#include <babrat/logger/sinks/stdout_sink.hpp>
#include <utility>


namespace babrat {

    namespace logger {

        LoggerManager::LoggerManager(std::string loggerName, LoggerConfig config) :
            m_config(std::move(config)), m_loggerName(std::move(loggerName))
        {
            m_loggerSink = std::make_unique<sinks::StdOutLogSink>();

            m_globalLogger = createLogger();
        }


        unique_ptr<logger::Logger> LoggerManager::createLogger() {
            std::lock_guard<std::mutex> lck(m_mutex);

            if (!m_config.enabled) {
                return std::make_unique<EmptyLogger>(*this);
            }

            return std::make_unique<DefaultLogger>(m_loggerName, m_config, *this);

        }

        void LoggerManager::flush()
        {
            std::lock_guard<std::mutex> lck(m_mutex);
            m_loggerSink->flush();
        }

        void LoggerManager::log(
            const string& loggerName,
            const string& ctx,
            timestamp_t timestamp,
            LogLevel level,
            const std::string& message)
        {
            std::lock_guard<std::mutex> lck(m_mutex);
            m_loggerSink->log(loggerName, ctx, timestamp, level, message);
        }

        bool LoggerManager::registerLoggerStorage(
            const std::string& storageName,
            std::shared_ptr<sinks::LoggerSink>& storageSink)
        {

            if (m_storageRegistry.contains(storageName)) {
                return false;
            }

            m_storageRegistry.insert({storageName, std::move(storageSink)});
            return true;
        }

        void LoggerManager::setLoggerStorage(const std::string& storageName)
        {
            std::lock_guard<std::mutex> lck(m_mutex);

            auto storage_name_to_lower = utils::toLower(storageName);

            m_loggerSink->flush();

            if (m_storageRegistry.contains(storage_name_to_lower)) {
                m_loggerSink = m_storageRegistry[storage_name_to_lower];
            } else {
                throw std::runtime_error("Log storage '%s' is not yet registered");
            }

        }

        Logger &LoggerManager::globalLoggerReference() {
            return *m_globalLogger;
        }

        std::shared_ptr<Logger> LoggerManager::globalLogger() {
            return m_globalLogger;
        }


        LoggerConfig LoggerManager::getConfig()
        {
            std::lock_guard<std::mutex> lck(m_mutex);
            return m_config;
        }

        string LoggerManager::getLoggerName()
        {
            std::lock_guard<std::mutex> lck(m_mutex);
            return m_loggerName;
        }

        void LoggerManager::setLevel(LogLevel level)
        {
            std::lock_guard<std::mutex> lck(m_mutex);
            m_config.level = level;
        }











    } // namespace logger

} // namespace babrat