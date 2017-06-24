/*
 * Copyright (c), Recep Aslantas.
 * MIT License (MIT), http://opensource.org/licenses/MIT
 */

#ifndef ds_util_h
#define ds_util_h

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
int
ds_cmp_i64(void *key1, void *key2);

#endif /* ds_util_h */
