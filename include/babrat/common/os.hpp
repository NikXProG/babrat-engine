#pragma once

#include <cstdio>
#include <cstdlib>
#include <unistd.h>

namespace babrat {

    namespace os {

        inline bool ifColorTerminal() {

        #ifdef _WIN32
                return ::_isatty(_fileno(stdout)) && std::getenv("NO_COLOR") == nullptr;
        #else
                return ::isatty(fileno(stdout)) && std::getenv("NO_COLOR") == nullptr;
        #endif

        }

    } // namespace macros

} // nnamespace babrat