
#ifndef BABRAT_API_H
#define BABRAT_API_H

#ifdef BABRAT_STATIC_DEFINE
#  define BABRAT_API
#  define BABRAT_NO_EXPORT
#else
#  ifndef BABRAT_API
#    ifdef babrat_EXPORTS
        /* We are building this library */
#      define BABRAT_API __attribute__((visibility("default")))
#    else
        /* We are using this library */
#      define BABRAT_API __attribute__((visibility("default")))
#    endif
#  endif

#  ifndef BABRAT_NO_EXPORT
#    define BABRAT_NO_EXPORT __attribute__((visibility("hidden")))
#  endif
#endif

#ifndef BABRAT_DEPRECATED
#  define BABRAT_DEPRECATED 
#endif

#ifndef BABRAT_DEPRECATED_EXPORT
#  define BABRAT_DEPRECATED_EXPORT BABRAT_API BABRAT_DEPRECATED
#endif

#ifndef BABRAT_DEPRECATED_NO_EXPORT
#  define BABRAT_DEPRECATED_NO_EXPORT BABRAT_NO_EXPORT BABRAT_DEPRECATED
#endif

/* NOLINTNEXTLINE(readability-avoid-unconditional-preprocessor-if) */
#if 0 /* DEFINE_NO_DEPRECATED */
#  ifndef BABRAT_NO_DEPRECATED
#    define BABRAT_NO_DEPRECATED
#  endif
#endif

#endif /* BABRAT_API_H */
