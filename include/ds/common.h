/*
 * Copyright (c), Recep Aslantas.
 * MIT License (MIT), http://opensource.org/licenses/MIT
 */

#ifndef common_h
#define common_h

#if defined(_MSC_VER) || defined(__MINGW32__) || defined(__MINGW64__)
#  ifdef DS_STATIC
#    define DS_EXPORT
#  elif defined(DS_EXPORTS)
#    define DS_EXPORT __declspec(dllexport)
#  else
#    define DS_EXPORT __declspec(dllimport)
#  endif
#  define DS_HIDE
#else
#  define DS_EXPORT  __attribute__((visibility("default")))
#  define DS_HIDE    __attribute__((visibility("hidden")))
#endif

#if defined(_MSC_VER)
#  define DS_INLINE __forceinline
#  define DS_ALIGN(X) __declspec(align(X))
#else
#  define DS_INLINE inline __attribute((always_inline))
#  define DS_ALIGN(X) __attribute((aligned(X)))
#endif

#define DS_ITOP(I)    ((void *)(uintptr_t)I)
#define DS_PTOI(I, T) ((T)(uintptr_t)I)

/* pack key (e.g non-null terminated string) and its length */
#define DS_KEYnLEN(KEY, LEN)                                                  \
   ((void *[]){(void *)KEY, (void *)(uintptr_t)LEN})

#endif /* common_h */
