#pragma once

#include <babrat/logger/sinks/logger_sink.hpp>
#include <babrat/logger/common/colors.hpp>

#include <iostream>
#include "babrat/common/utils/string_utils.hpp"
#include <babrat/common/os.hpp>

namespace babrat {

    namespace logger {

        namespace sinks {

            class StdOutLogSink : public LoggerSink {

            public:

                explicit StdOutLogSink(ColorMode colorMode = ColorMode::Auto)
                {
                    setColorMode(colorMode);

                    m_colors = {
                            details::color::White,    // Trace
                            details::color::Cyan,     // Debug
                            details::color::Green,    // Info
                            details::color::YellowBold, // Warn
                            details::color::RedBold,  // Error
                            details::color::BoldOnRed // Critical
                    };

                }

            private:

                std::array<std::string_view, 6> m_colors;

                bool m_useColors;

            public:

                void log(const string& loggerName, const string& ctx, timestamp_t timestamp,
                         LogLevel level, const std::string& message) override {

                    std::ostringstream oss;

                    // Format context (logger name and context)
                    formatContext(oss, loggerName, ctx);

                    auto timeInt64 = static_cast<int64_t>(timestamp);
                    auto levelString = levelToString(level);

                    // Format timestamp and level
                    if (m_useColors) {
                        oss << "[ts" << details::color::Yellow << ":" << details::color::LightYellow
                            << timeInt64 << details::color::Reset << "] "
                            << "[" << m_colors[static_cast<size_t>(level)]
                            << levelString << details::color::Reset << "] ";
                    } else {
                        oss << "[ts:" << timeInt64 << "] "
                            << "[" << levelString << "] ";
                    }

                    // Output the complete message
                    std::cout << oss.str() << message;
                    if (m_useColors) std::cout << details::color::Reset;
                    std::cout << std::endl;

                }

                void flush() override
                {

                }

            private:

                void formatContext(std::ostringstream& oss, const string& loggerName, const string& ctx) const {
                    auto formatPart = [&](const string& text, const string& colorCode) {
                        oss << "[";
                        if (m_useColors) oss << colorCode;
                        oss << text;
                        if (m_useColors) oss << details::color::Reset;
                        oss << "] ";
                    };

                    if (!loggerName.empty()) formatPart(loggerName, details::color::Ruby);
                    if (!ctx.empty()) formatPart(ctx, details::color::Cyan);
                }

                void setColorMode(ColorMode mode) {
                    switch (mode) {
                        case ColorMode::On:
                            m_useColors = true;
                            return;
                        case ColorMode::Auto:
                            m_useColors = babrat::os::ifColorTerminal();
                            return;
                        case ColorMode::Off:
                            m_useColors = false;
                            return;
                        default:
                            m_useColors = false;
                    }
                }

                static constexpr std::string_view levelToString(LogLevel level) {
                    constexpr std::array<std::string_view, 6> names = {
                            "TRACE", "DEBUG", "INFO", "WARN", "ERROR", "CRITICAL"
                    };
                    return names[static_cast<size_t>(level)];
                }

            };


        } // namespace sinks

    } // namespace logger

} // nnamespace babrat