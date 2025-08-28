#pragma once

#include <babrat/logger/common/logger_config.hpp>

namespace babrat {

    namespace logger {

        namespace details {

            namespace color {

                // Text styles and reset
                constexpr auto Reset      = "\033[0m";   // Reset all styles
                constexpr auto Bold       = "\033[1m";   // Bold/bright text
                constexpr auto Dark       = "\033[2m";   // Dim/faint text
                constexpr auto Underline  = "\033[4m";   // Underlined text
                constexpr auto Blink      = "\033[5m";   // Blinking text (not widely supported)
                constexpr auto Reverse    = "\033[7m";   // Reverse video (swap FG/BG colors)
                constexpr auto Concealed  = "\033[8m";   // Hidden text (password input)
                constexpr auto ClearLine  = "\033[K";    // Clear current terminal line

                // Standard foreground colors
                constexpr auto Black      = "\033[30m";  // Black text
                constexpr auto Red        = "\033[31m";  // Red text
                constexpr auto Green      = "\033[32m";  // Green text
                constexpr auto Yellow     = "\033[33m";  // Yellow text
                constexpr auto Blue       = "\033[34m";  // Blue text
                constexpr auto Magenta    = "\033[35m";  // Magenta/purple text
                constexpr auto Cyan       = "\033[36m";  // Cyan text
                constexpr auto White      = "\033[37m";  // White text

                // Standard background colors
                constexpr auto OnBlack    = "\033[40m";  // Black background
                constexpr auto OnRed      = "\033[41m";  // Red background
                constexpr auto OnGreen    = "\033[42m";  // Green background
                constexpr auto OnYellow   = "\033[43m";  // Yellow background
                constexpr auto OnBlue     = "\033[44m";  // Blue background
                constexpr auto OnMagenta  = "\033[45m";  // Magenta background
                constexpr auto OnCyan     = "\033[46m";  // Cyan background
                constexpr auto OnWhite    = "\033[47m";  // White background

                // Standard light foreground colors
                constexpr auto LightBlack   = "\033[90m";  // Light black (gray)
                constexpr auto LightRed     = "\033[91m";  // Light red
                constexpr auto LightGreen   = "\033[92m";  // Light green
                constexpr auto LightYellow  = "\033[93m";  // Light yellow
                constexpr auto LightBlue    = "\033[94m";  // Light blue
                constexpr auto LightMagenta = "\033[95m";  // Light magenta
                constexpr auto LightCyan    = "\033[96m";  // Light cyan
                constexpr auto LightWhite   = "\033[97m";  // Light white (bright white)

                // Common style combinations
                constexpr auto Ruby  = "\033[38;2;224;17;95m"; // Ruby color text
                constexpr auto YellowBold  = "\033[33;1m";  // Bold yellow text
                constexpr auto RedBold     = "\033[31;1m";  // Bold red text
                constexpr auto BoldOnRed   = "\033[1;41m";  // Bold text on red background
                constexpr auto WhiteOnRed  = "\033[37;41m"; // White text on red background

            } // namespace color

            inline const char* getLevelColor(LogLevel level) {

                switch(level) {

                    case LogLevel::Critical:
                        return color::BoldOnRed;
                    case LogLevel::Error:
                        return color::RedBold;
                    case LogLevel::Warn:
                        return color::Yellow;
                    case LogLevel::Info:
                        return color::Green;
                    case LogLevel::Debug:
                        return color::Blue;
                    default:
                        return color::Reset;

                }

            }

        } // namespace details

    } // namespace logger

} // namespace babrat