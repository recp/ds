/*
 * Copyright (c), Recep Aslantas.
 * MIT License (MIT), http://opensource.org/licenses/MIT
 */

#include "common.h"
#include "../include/ds/forward-list.h"

#include <assert.h>

DS_EXPORT
FList*
flist_new(DsAllocator *allocator) {
  DsAllocator *alc;
  FList       *flist;

  alc   = !allocator ? ds_def_alc() : allocator;
  flist = alc->calloc(sizeof(*flist), 1);

  assert(flist);

  flist->alc   = alc;
  flist->count = 0;

  return flist;
}

DS_EXPORT
void
flist_insert(FList *flist,
             void  *value) {
  FListItem *item;

  item = flist->alc->calloc(sizeof(*item), 1);
  item->data = value;

  item->next = flist->first;
  if (!flist->last)
    flist->last = item;

  flist->first = item;
  flist->count++;
}

DS_EXPORT
void
flist_append(FList *flist,
             void  *value) {
  FListItem *item;

  item = flist->alc->calloc(sizeof(*item), 1);
  item->data = value;

  if (flist->last)
    flist->last->next = item;

  if (!flist->first)
    flist->first = item;

  flist->last = item;
  flist->count++;
}

DS_EXPORT
void
flist_remove(FList *flist, FListItem *item) {
  DsAllocator *alc;
  FListItem   *prev, *tofree;

  alc  = flist->alc;
  prev = flist->first;

  tofree = NULL;
  while (prev) {
    if (prev == item) {
      tofree = prev;
      break;
    }
    prev = prev->next;
  }

  if (!tofree)
    return;

  if (flist->onFreeItem)
    flist->onFreeItem(flist, tofree);

  if (flist->first == tofree)
    flist->first = tofree->next;

  if (flist->last == tofree)
    flist->last = tofree->next;

  if (prev->next == tofree)
    prev->next = tofree->next;

  alc->free(tofree);
  flist->count--;
}

DS_EXPORT
void
flist_remove_by(FList *flist, void *value) {
  DsAllocator *alc;
  FListItem   *prev, *tofree;

  alc  = flist->alc;
  prev = flist->first;

  tofree = NULL;
  while (prev) {
    if (prev->data == value) {
      tofree = prev;
      break;
    }
    prev = prev->next;
  }

  if (!tofree)
    return;

  if (flist->onFreeItem)
    flist->onFreeItem(flist, tofree);

  if (flist->first == tofree)
    flist->first = tofree->next;

  if (flist->last == tofree)
    flist->last = tofree->next;

  if (prev->next == tofree)
    prev->next = tofree->next;

  alc->free(tofree);
  flist->count--;
}

DS_EXPORT
bool
flist_contains(FList *flist, void *value) {
  FListItem *item;

  item = flist->first;
  while (item) {
    if (item->data == value)
      return true;
    item = item->next;
  }

  return false;
}

DS_EXPORT
void
flist_empty(FList *flist) {
  DsAllocator *alc;
  FListItem   *item, *tofree;

  alc  = flist->alc;
  item = flist->first;

  tofree = NULL;
  while (item) {
    tofree = item;
    item   = item->next;

    if (flist->onFreeItem)
      flist->onFreeItem(flist, tofree);

    alc->free(tofree);
  }

  flist->first = NULL;
  flist->last  = NULL;
  flist->count = 0;
}

DS_EXPORT
bool
flist_isempty(FList *flist) {
  return flist->first == NULL
          && flist->last == NULL;
}

DS_EXPORT
void
flist_destroy(FList *flist) {
  flist_empty(flist);
  flist->alc->free(flist);
}
