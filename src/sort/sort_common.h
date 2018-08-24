/*
 * Copyright (c), Recep Aslantas.
 * MIT License (MIT), http://opensource.org/licenses/MIT
 */

#ifndef sort_common_h
#define sort_common_h

#include "../common.h"
#include "../../include/ds/sort.h"

#define partition(p, TYPE, op1, op2)                                          \
  do {                                                                        \
    uint32_t i, j;                                                            \
    TYPE     tmp, PIVOT;                                                      \
                                                                              \
    PIVOT = A[lo];                                                            \
    i     = lo - 1;                                                           \
    j     = hi + 1;                                                           \
                                                                              \
    for (;;) {                                                                \
      do {                                                                    \
        i++;                                                                  \
      } while (A[i] op1 PIVOT);                                               \
                                                                              \
      do {                                                                    \
        j--;                                                                  \
      } while (A[j] op2 PIVOT);                                               \
                                                                              \
      if (i >= j) {                                                           \
        p = j;                                                                \
        break;                                                                \
      }                                                                       \
                                                                              \
      swap2(tmp, A[i], A[j]);                                                 \
    }                                                                         \
  } while(0)                                                                  \

#define partition_cb(p, TYPE, op1, op2)                                       \
  do {                                                                        \
    uint32_t i, j;                                                            \
    TYPE     tmp, PIVOT;                                                      \
                                                                              \
    PIVOT = A[lo];                                                            \
    i     = lo - 1;                                                           \
    j     = hi + 1;                                                           \
                                                                              \
    for (;;) {                                                                \
      do {                                                                    \
        i++;                                                                  \
      } while (A[i] op1 PIVOT);                                               \
                                                                              \
      do {                                                                    \
        j--;                                                                  \
      } while (A[j] op2 PIVOT);                                               \
                                                                              \
      if (i >= j) {                                                           \
        p = j;                                                                \
        break;                                                                \
      }                                                                       \
                                                                              \
      swap2(tmp, A[i], A[j]);                                                 \
      cb(A, i, j);                                                            \
    }                                                                         \
  } while(0)                                                                  \

#define partition_asc(TYPE, PIVOT)                                            \
  partition(PIVOT, TYPE, <, >)
#define partition_dsc(TYPE, PIVOT)                                            \
  partition(PIVOT, TYPE, >, <)
#define partition_asc_cb(TYPE, PIVOT)                                         \
  partition_cb(PIVOT, TYPE, <, >)
#define partition_dsc_cb(TYPE, PIVOT)                                         \
  partition_cb(PIVOT, TYPE, >, <)

#define DEFINE_SORT_FUNC(POSTFIX, TYPE, ORDER)                                \
  static                                                                      \
  void                                                                        \
  sort ## POSTFIX ## _ ## ORDER(TYPE * __restrict A,                          \
                                uint32_t lo,                                  \
                                uint32_t hi) {                                \
    uint32_t p;                                                               \
                                                                              \
    if (lo < hi) {                                                            \
      partition_ ## ORDER(TYPE, p);                                           \
      sort ## POSTFIX ## _ ## ORDER(A, lo, p);                                \
      sort ## POSTFIX ## _ ## ORDER(A, p + 1, hi);                            \
    }                                                                         \
  }                                                                           \
                                                                              \
  static                                                                      \
  void                                                                        \
  sort ## POSTFIX ## _ ## ORDER ## _cb(TYPE * __restrict A,                   \
                                       DsSortSwapFunc    cb,                  \
                                       uint32_t          lo,                  \
                                       uint32_t          hi) {                \
    uint32_t p;                                                               \
                                                                              \
    if (lo < hi) {                                                            \
      partition_ ## ORDER ## _cb(TYPE, p);                                    \
      sort ## POSTFIX ## _ ## ORDER ## _cb(A, cb, lo, p);                     \
      sort ## POSTFIX ## _ ## ORDER ## _cb(A, cb, p + 1, hi);                 \
    }                                                                         \
  }

#define DEFINE_FUNC(POSTFIX, TYPE)                                            \
  DS_EXPORT                                                                   \
  void                                                                        \
  sort ## POSTFIX(TYPE  * __restrict arr,                                     \
        uint32_t            count) {                                          \
    sort ## POSTFIX ## _asc(arr, 0, count - 1);                               \
  }                                                                           \
                                                                              \
  DS_EXPORT                                                                   \
  void                                                                        \
  sort ## POSTFIX ## _ex(TYPE * __restrict arr,                               \
                         uint32_t          count,                             \
                         bool              asc,                               \
                         DsSortSwapFunc    swapCb) {                          \
    if (!swapCb) {                                                            \
      if (asc)                                                                \
        sort ## POSTFIX ## _asc(arr, 0, count - 1);                           \
      else                                                                    \
        sort ## POSTFIX ## _dsc(arr, 0, count - 1);                           \
      return;                                                                 \
    }                                                                         \
                                                                              \
    if (asc)                                                                  \
      sort ## POSTFIX ## _asc_cb(arr, swapCb, 0, count - 1);                  \
    else                                                                      \
      sort ## POSTFIX ## _dsc_cb(arr, swapCb, 0, count - 1);                  \
  }

#define DEFINE_SORT(POSTFIX, TYPE)                                            \
  DEFINE_SORT_FUNC(POSTFIX, TYPE, asc)                                        \
  DEFINE_SORT_FUNC(POSTFIX, TYPE, dsc)                                        \
  DEFINE_FUNC(POSTFIX, TYPE)

#endif /* sort_common_h */
