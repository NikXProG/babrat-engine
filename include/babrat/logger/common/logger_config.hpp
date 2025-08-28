#pragma once

#include <cstdint>
#include <string>


namespace babrat {

    namespace logger {

        enum class LogLevel : uint8_t {
            Trace = 0,
            Debug = 1,
            Info = 2,
            Warn = 3,
            Error = 4,
            Critical = 5
        };

        enum class LogSinkType : uint8_t {
            Console,
            File,
            All
        };

        enum class ColorMode {
            On, // always turns on backlight mode
            Auto, // checks if output via ascii color is available. If yes, then enable
            Off // never turn on color
        };

        class LoggerConfig {

        public:

            constexpr static LogLevel DefaultLogLevel = LogLevel::Info;
            constexpr static LogSinkType DefaultLogSink = LogSinkType::File;

        public:

            LoggerConfig() :
                enabled(true),
                level(DefaultLogLevel),
                typeSink(DefaultLogSink){
            }

            LoggerConfig(
                bool enabled,
                LogLevel level,
                LogSinkType sink) :
              enabled(enabled),
              level(level),
              typeSink(sink) {
            }

        public:

            // main settings
            bool enabled;
            LogLevel level;
            LogSinkType typeSink;

        };

    } // namespace logger

} // namespace babrat

