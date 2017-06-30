/*
 * Copyright (c), Recep Aslantas.
 * MIT License (MIT), http://opensource.org/licenses/MIT
 */

#ifndef ds_util_h
#define ds_util_h
#ifdef __cplusplus
extern "C" {
#endif

#include "common.h"
#include <stdint.h>

typedef int  (*DsCmpFn)(void *, void *);
typedef void (*DsPrintFn)(void *);

DS_EXPORT
int
ds_cmp_str(void * key1, void *key2);

DS_EXPORT
int
ds_cmp_ptr(void *key1, void *key2);

DS_EXPORT
int
ds_cmp_i32(void *key1, void *key2);

DS_EXPORT
int64_t
ds_cmp_i64(void *key1, void *key2);

DS_EXPORT
void
ds_print_str(void *key);

DS_EXPORT
void
ds_print_ptr(void *key);

#ifdef __cplusplus
}
#endif
#endif /* ds_util_h */
