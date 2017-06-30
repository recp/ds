/*
 * Copyright (c), Recep Aslantas.
 * MIT License (MIT), http://opensource.org/licenses/MIT
 */

#include "test_common.h"
#include <ds/forward-list.h>

static
void
test_flist_onfree(FList *flist, FListItem *item) {
  assert(item);
  assert(item->data);

  /* free value */
  free(item->data);
}

void
test_flist(void **state) {
  FList *flist;
  float *value;
  int    count, i;

  count = 200;
  flist = flist_new(NULL);
  flist->onFreeItem = test_flist_onfree;

  srand((unsigned int)time(NULL));

  for (i = 0; i < count; i++) {
    value  = malloc(sizeof(*value));
    *value = (float)drand48();

    /* test insert */
    if (i % 2 == 0) {
      flist_insert(flist, value);
      assert_non_null(flist->first);
      assert_non_null(flist->last);
    }

    /* test append */
    else {
      flist_append(flist, value);
      assert_non_null(flist->first);
      assert_non_null(flist->last);
    }

    /* test find node */
    assert_true(flist_contains(flist, value));

    /* test remove */
    if (i == 0) {
      flist_remove_by(flist, value);
      assert_false(flist_contains(flist, value));
    }

    /* test remove by item */
    if (i == 10) {
      FListItem *item;
      item = flist->first;
      flist_remove(flist, item);
      assert_false(flist_contains(flist, value));
    }

    /* test empty */
    if (i == 100) {
      flist_empty(flist);
      assert_true(flist_isempty(flist));
    }
  }

  flist_destroy(flist);
}
