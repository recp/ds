/*
 * Copyright (c), Recep Aslantas.
 *
 * MIT License (MIT), http://opensource.org/licenses/MIT
 * Full license can be found in the LICENSE file
 */

#ifndef tests_h
#define tests_h

#include "include/common.h"

/*
 * To register a test:
 *   1. use TEST_DECLARE() to forward declare test
 *   2. use TEST_ENTRY() to add test to list
 */

/* red black tree */
TEST_DECLARE(rb_topdown_str)
TEST_DECLARE(rb_topdown_ptr)
TEST_DECLARE(rb_topdown_custom_cmp)
TEST_DECLARE(rb_topdown_freeenode)
TEST_DECLARE(rb_topdown_custom_cmp_i32)
TEST_DECLARE(rb_topdown_custom_cmp_i64)

/* forward list */
TEST_DECLARE(flist)
TEST_DECLARE(flist_sep)

/* hash table */
TEST_DECLARE(htable)

/*****************************************************************************/

TEST_LIST {
  /* red black tree */
  TEST_ENTRY(rb_topdown_str)
  TEST_ENTRY(rb_topdown_ptr)
  TEST_ENTRY(rb_topdown_custom_cmp)
  TEST_ENTRY(rb_topdown_freeenode)
  TEST_ENTRY(rb_topdown_custom_cmp_i32)
  TEST_ENTRY(rb_topdown_custom_cmp_i64)

  /* forward list */
  TEST_ENTRY(flist)
  TEST_ENTRY(flist_sep)

  /* hash table */
  TEST_ENTRY(htable)
};

#endif /* tests_h */
