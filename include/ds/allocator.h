/*
 * Copyright (c), Recep Aslantas.
 * MIT License (MIT), http://opensource.org/licenses/MIT
 */

#ifndef ds_allocator_h
#define ds_allocator_h

#include "common.h"
#include <stdlib.h>

typedef struct DsAllocator {
  void  *(*malloc)(size_t);
  void  *(*calloc)(size_t, size_t);
  void *(*realloc)(void *, size_t);
  int  (*memalign)(void **, size_t, size_t);
  char  *(*strdup)(const char *);
  void     (*free)(void *);
  size_t   (*size)(const void *);
} DsAllocator;

#endif /* ds_allocator_h */
