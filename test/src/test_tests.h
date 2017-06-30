/*
 * Copyright (c), Recep Aslantas.
 * MIT License (MIT), http://opensource.org/licenses/MIT
 */

#ifndef test_tests_h
#define test_tests_h

/* RB Tree */
void test_rb_topdown_str(void **state);
void test_rb_topdown_ptr(void **stater);
void test_rb_topdown_custom_cmp(void **state);
void test_rb_topdown_freeenode(void **state);

/* Forward List*/
void test_flist(void **state);

#endif /* test_tests_h */
