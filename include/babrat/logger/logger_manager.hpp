#pragma once

#include <babrat/common/babrat_api.hpp>
#include <unordered_map>
#include <babrat/common/types/timestamp.hpp>
#include <mutex>
#include <babrat/common/types/string_ignore_case.hpp>
#include <babrat/logger/common/logger_config.hpp>

#define LOG_DEFAULT_NAME ""

namespace babrat {

    namespace logger {

        class Logger;
        class ClientContext;
        class LoggerConfig;

        namespace sinks {
            class LoggerSink;
        }


        class LoggerManager : public std::enable_shared_from_this<LoggerManager>  {

            friend class DefaultLogger;

        protected:

            void log(
                const std::string& loggerName,
                const std::string& ctx,
                timestamp_t timestamp,
                LogLevel level,
                const std::string& message);

        protected:

            std::shared_ptr<sinks::LoggerSink> m_loggerSink;

            std::shared_ptr<Logger> m_globalLogger;

        protected:

            LoggerConfig m_config;

            std::string m_loggerName;

            std::mutex m_mutex;

        protected:

            ignore_case_map_t<std::shared_ptr<sinks::LoggerSink>> m_storageRegistry;

        public:

            LoggerManager(const LoggerManager&) = delete;
            LoggerManager& operator=(const LoggerManager&) = delete;

        public:

            BABRAT_API Logger &globalLoggerReference();
            BABRAT_API std::shared_ptr<Logger> globalLogger();

        public:

            BABRAT_API std::unique_ptr<logger::Logger> createLogger();

        public:

            explicit LoggerManager(std::string loggerName = LOG_DEFAULT_NAME, LoggerConfig config = LoggerConfig());

        public:

            BABRAT_API bool registerLoggerStorage(
                const std::string &storageName,
                std::shared_ptr<sinks::LoggerSink> &storageSink);

        public:

            BABRAT_API void flush();

        public:

            BABRAT_API void setLoggerStorage(
                const std::string &storageName);

            BABRAT_API void setLevel(
                LogLevel level);

        public:

            BABRAT_API LoggerConfig getConfig();

            BABRAT_API string getLoggerName();

        };


        /*
        class LoggerManager {

        protected:

            std::shared_ptr<spdlog::logger> logger;

        public:

            LoggerManager(const LoggerManager&) = delete;
            LoggerManager& operator=(const LoggerManager&) = delete;

        public:

            static spdlog::level::level_enum toSpdlogLevel(LogLevel level) {
                switch (level) {
                    case LogLevel::TRACE:
                        return spdlog::level::trace;
                    case LogLevel::DEBUG:
                        return spdlog::level::debug;
                    case LogLevel::INFO:
                        return spdlog::level::info;
                    case LogLevel::WARN:
                        return spdlog::level::warn;
                    case LogLevel::ERROR:
                        return spdlog::level::err;
                    case LogLevel::CRITICAL:
                        return spdlog::level::critical;
                    default:
                        throw std::invalid_argument("Unknown log level");
                }
            }


            void initLogger(const LoggerConfig& config) {

                std::vector<spdlog::sink_ptr> sinks;
                auto sink = std::make_shared<spdlog::sinks::stdout_color_sink_mt>();
                sinks.push_back(std::move(sink));

                logger = std::make_shared<spdlog::logger>("babrat_logger", sinks.begin(), sinks.end());
                logger->set_level(toSpdlogLevel(config.level));
                logger->set_pattern(config.pattern);
            }


            explicit LoggerManager(const LoggerConfig& config = LoggerConfig()) {

                if (!config.enabled) {
                    return;
                }

                initLogger(config);
                spdlog::register_logger(logger);
            }

            explicit LoggerManager(std::shared_ptr<spdlog::logger> logger) : logger(std::move(logger)){

            }

        public:

            void writeLog(
                std::int64_t timestamp,
                LogLevel level,
                const string& message)
            {
                if (!logger) {
                    return;
                }

                logger->log(toSpdlogLevel(level), "[{}] {}", timestamp, message);
            }

            void flush()
            {
                if (logger) {
                    logger->flush();
                }
            }


        };
        */

    } // namespace logger

} // namespace babrat