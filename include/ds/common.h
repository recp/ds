/*
 * Copyright (c), Recep Aslantas.
 * MIT License (MIT), http://opensource.org/licenses/MIT
 */

#ifndef common_h
#define common_h

#if defined(_WIN32)
#  ifdef LIBDS_DLL
#    define DS_EXPORT __declspec(dllexport)
#  else
#    define DS_EXPORT __declspec(dllimport)
#  endif
#  define DS_HIDE
#  define DS_INLINE __forceinline
#  define DS_ALIGN(X) __declspec(align(X))
#else
#  define DS_EXPORT  __attribute__((visibility("default")))
#  define DS_HIDE    __attribute__((visibility("hidden")))
#  define DS_INLINE inline __attribute((always_inline))
#  define D_ALIGN(X) __attribute((aligned(X)))
#endif

#endif /* common_h */
