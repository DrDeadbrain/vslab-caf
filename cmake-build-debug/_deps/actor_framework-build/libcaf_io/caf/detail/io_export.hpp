
#ifndef CAF_IO_EXPORT_H
#define CAF_IO_EXPORT_H

#ifdef LIBCAF_IO_STATIC_DEFINE
#  define CAF_IO_EXPORT
#  define LIBCAF_IO_NO_EXPORT
#else
#  ifndef CAF_IO_EXPORT
#    ifdef libcaf_io_EXPORTS
        /* We are building this library */
#      define CAF_IO_EXPORT __attribute__((visibility("default")))
#    else
        /* We are using this library */
#      define CAF_IO_EXPORT __attribute__((visibility("default")))
#    endif
#  endif

#  ifndef LIBCAF_IO_NO_EXPORT
#    define LIBCAF_IO_NO_EXPORT __attribute__((visibility("hidden")))
#  endif
#endif

#ifndef LIBCAF_IO_DEPRECATED
#  define LIBCAF_IO_DEPRECATED __attribute__ ((__deprecated__))
#endif

#ifndef LIBCAF_IO_DEPRECATED_EXPORT
#  define LIBCAF_IO_DEPRECATED_EXPORT CAF_IO_EXPORT LIBCAF_IO_DEPRECATED
#endif

#ifndef LIBCAF_IO_DEPRECATED_NO_EXPORT
#  define LIBCAF_IO_DEPRECATED_NO_EXPORT LIBCAF_IO_NO_EXPORT LIBCAF_IO_DEPRECATED
#endif

#if 0 /* DEFINE_NO_DEPRECATED */
#  ifndef LIBCAF_IO_NO_DEPRECATED
#    define LIBCAF_IO_NO_DEPRECATED
#  endif
#endif

#endif /* CAF_IO_EXPORT_H */
