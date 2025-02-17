
#ifndef CAF_CORE_EXPORT_H
#define CAF_CORE_EXPORT_H

#ifdef LIBCAF_CORE_STATIC_DEFINE
#  define CAF_CORE_EXPORT
#  define LIBCAF_CORE_NO_EXPORT
#else
#  ifndef CAF_CORE_EXPORT
#    ifdef libcaf_core_EXPORTS
        /* We are building this library */
#      define CAF_CORE_EXPORT __attribute__((visibility("default")))
#    else
        /* We are using this library */
#      define CAF_CORE_EXPORT __attribute__((visibility("default")))
#    endif
#  endif

#  ifndef LIBCAF_CORE_NO_EXPORT
#    define LIBCAF_CORE_NO_EXPORT __attribute__((visibility("hidden")))
#  endif
#endif

#ifndef LIBCAF_CORE_DEPRECATED
#  define LIBCAF_CORE_DEPRECATED __attribute__ ((__deprecated__))
#endif

#ifndef LIBCAF_CORE_DEPRECATED_EXPORT
#  define LIBCAF_CORE_DEPRECATED_EXPORT CAF_CORE_EXPORT LIBCAF_CORE_DEPRECATED
#endif

#ifndef LIBCAF_CORE_DEPRECATED_NO_EXPORT
#  define LIBCAF_CORE_DEPRECATED_NO_EXPORT LIBCAF_CORE_NO_EXPORT LIBCAF_CORE_DEPRECATED
#endif

#if 0 /* DEFINE_NO_DEPRECATED */
#  ifndef LIBCAF_CORE_NO_DEPRECATED
#    define LIBCAF_CORE_NO_DEPRECATED
#  endif
#endif

#endif /* CAF_CORE_EXPORT_H */
