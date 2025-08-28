#pragma once

#include <babrat/common/babrat_api.hpp>
#include <string>
#include <babrat/common/types/timestamp.hpp>
#include <babrat/logger/common/logger_config.hpp>

namespace babrat {

    namespace logger {

        namespace sinks {

            class LoggerSink {

            public:

                BABRAT_API virtual ~LoggerSink() noexcept = default;

            public:

                BABRAT_API virtual void log(
                        const std::string& loggerName,
                        const std::string& ctx,
                        timestamp_t timestamp,
                        LogLevel level,
                        const std::string& message) = 0;

                BABRAT_API virtual void flush() = 0;

            };


        } // namespace sinks

    } // namespace logger

} // namespace babrat