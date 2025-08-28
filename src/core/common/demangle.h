#pragma once

#include <string>
#include <memory>
#include <cstdlib>
#include <cstddef>

#if __has_include(<cxxabi.h>)
#include <cxxabi.h>
#endif

namespace detail {

    inline char const* demangle_alloc(char const* name) noexcept {
#if __has_include(<cxxabi.h>)
        int status = 0;
        std::size_t size = 0;
        return abi::__cxa_demangle(name, nullptr, &size, &status);
#else
        return name;
#endif
    }

    inline void demangle_free(char const* name) noexcept {
#if __has_include(<cxxabi.h>)
        std::free(const_cast<char*>(name));
#else
        (void)name;
#endif
    }

} // namespace detail

class scoped_demangled_name {
    char const* m_p;

public:
    explicit scoped_demangled_name(char const* name) noexcept
            : m_p(detail::demangle_alloc(name)) {}

    ~scoped_demangled_name() noexcept {
        detail::demangle_free(m_p);
    }

    char const* get() const noexcept { return m_p; }

    scoped_demangled_name(const scoped_demangled_name&) = delete;
    scoped_demangled_name& operator=(const scoped_demangled_name&) = delete;
};

inline std::string demangle(char const* name) {
    scoped_demangled_name demangled_name(name);
    char const* p = demangled_name.get();
    if (p) {
        return p;
    } else {
        return name;
    }
}

