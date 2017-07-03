/*
 * Copyright (c), Recep Aslantas.
 * MIT License (MIT), http://opensource.org/licenses/MIT
 */

#include "common.h"
#include "../include/ds/hash.h"

#include <assert.h>
#include <stdbool.h>

static
bool
hash_finditem(HTable      *htable,
              void        *key,
              uint32_t    *idx_out,
              HTableItem **found,
              HTableItem **prev);

DS_EXPORT
HTable*
hash_new(DsAllocator *allocator,
         HashFn       hashfn,
         DsCmpFn      cmp,
         uint32_t     capacity) {
  DsAllocator *alc;
  HTable   *htable;

  alc   = !allocator ? ds_def_alc() : allocator;
  htable = alc->calloc(sizeof(*htable), 1);

  htable->table = alc->calloc(sizeof(*htable->table), capacity);

  assert(htable);

  htable->alc      = alc;
  htable->hashfn   = hashfn;
  htable->cmp      = cmp ? cmp : ds_cmp_str;
  htable->count    = 0;
  htable->capacity = capacity;

  return htable;
}

DS_EXPORT
HTable*
hash_new_str(uint32_t capacity) {
  return hash_new(ds_def_alc(),
                  ds_hashfn_djb2,
                  ds_cmp_str,
                  capacity);
}

static
bool
hash_finditem(HTable      *htable,
              void        *key,
              uint32_t    *idx_out,
              HTableItem **found,
              HTableItem **prev) {
  HTableItem *item;
  uint32_t       idx;

  *prev    = NULL;
  *idx_out = idx = htable->hashfn(key) % htable->capacity;

  if ((item = htable->table[idx])) {
    do {
      *found = item;

      if (htable->cmp(key, item->key) == 0)
        return true;

      *prev = item;
    } while ((item = item->next));
  }

  *found = NULL;
  return false;
}

DS_EXPORT
void
hash_unset(HTable *htable,
           void      *key) {
  HTableItem *found, *item, *prev;
  uint32_t    idx;

  if (!hash_finditem(htable, key, &idx, &found, &prev) || !found)
    return;

  item = htable->table[idx];
  if (found == item)
    htable->table[idx] = found->next;

  if (prev)
    prev->next = found->next;

  htable->alc->free(found);
  htable->count--;
}

DS_EXPORT
void
hash_set(HTable *htable,
         void   *key,
         void   *value) {
  HTableItem *prev, *found, *item;
  uint32_t       idx;

  if (!value) {
    hash_unset(htable, key);
    return;
  }

  if (hash_finditem(htable, key, &idx, &found, &prev)) {
    found->data = value;
    return;
  }

  item = htable->alc->calloc(sizeof(*item), 1);
  item->key          = key;
  item->data         = value;

  if (!found) {
    item->next         = htable->table[idx];
    htable->table[idx] = item;
  }

  /* there is no need to access array (read + write),
     just append it */
  else {
    found->next = item;
  }

  htable->count++;
}

DS_EXPORT
void*
hash_get(HTable *htable,
         void   *key) {
  HTableItem *prev, *found;
  uint32_t       idx;

  if (!hash_finditem(htable, key, &idx, &found, &prev) || !found)
    return NULL;

  return found->data;
}
