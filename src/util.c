/*
 * Copyright (c), Recep Aslantas.
 * MIT License (MIT), http://opensource.org/licenses/MIT
 */

#include "common.h"
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <stdio.h>

DS_EXPORT
int
ds_cmp_str(void * key1, void *key2) {
  return strcmp((char *)key1, (char *)key2);
}

DS_EXPORT
int
ds_cmp_ptr(void *key1, void *key2) {
  if (key1 > key2)
    return 1;
  else if (key1 < key2)
    return -1;
  return 0;
}

DS_EXPORT
int
ds_cmp_i32(void *key1, void *key2) {
  int32_t a, b;

  a = *(int32_t *)key1;
  b = *(int32_t *)key2;

  return a - b;
}

DS_EXPORT
int
ds_cmp_i64(void *key1, void *key2) {
  int64_t a, b;

  a = *(int64_t *)key1;
  b = *(int64_t *)key2;

  return a - b;
}

DS_EXPORT
void
ds_print_str(void *key) {
  printf("\t'%s'\n", (const char *)key);
}

DS_EXPORT
void
ds_print_ptr(void *key) {
  printf("\t'%p'\n", key);
}
