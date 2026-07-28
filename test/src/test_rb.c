/*
 * Copyright (c), Recep Aslantas.
 * MIT License (MIT), http://opensource.org/licenses/MIT
 */

#include "test_common.h"
#include <ds/rb.h>

/* keep top 2000 to test remove */
static char *inserted_items[2000];
static size_t test_rb_freed_nodes;

static
int
test_rb_cmp_float(void *key1, void *key2) {
  float key1f, key2f;

  key1f = *(float *)key1;
  key2f = *(float *)key2;

  if ((key1f - key2f) > FLT_EPSILON)
    return 1;
  else if ((key2f - key1f) > FLT_EPSILON)
    return -1;
  return 0;
}

static
void
test_rb_print_float(void *key) {
  printf("\t'%0.8f'\n", *(float *)key);
}

static
void
test_rb_freenode(RBTree *tree, RBNode *node) {
  assert(node);
  assert(node->val);

  test_rb_freed_nodes++;

  /* free value */
  free(node->val);
}

static
void
test_rb_foundkey(RBTree *tree, void* key, bool *replace) {
  printf("duplicated key: '%s'\n", (char *)key);
  *replace = true;
}

static
void
test_rb_walk(RBTree *tree, RBNode *node) {
  RBNode *parent;
  int     side;

  assert(node->chld[0]);
  assert(node->chld[1]);

  side = rb_parent(tree, node->key, &parent);
  assert(parent); /* because we are walking on it */

  assert(parent->chld[side] == node);
}

TEST_IMPL(rb_topdown_str) {
  RBTree *tree;
  RBNode *node;
  void   *found;
  char    keybuf[256];
  char   *key;
  int     count, i, k;

  count = 1000;
  tree  = rb_newtree_str();
  tree->onFound = test_rb_foundkey;

  srand((unsigned int)time(NULL));

  /* make sure it is samll size: max 32 */
  for (i = 0; i < count; i++) {
    /* random key length */
    k = rand() % (sizeof(keybuf) / 8 - 1);

    rand_str(keybuf, k);
    key = strdup(keybuf);

    rb_insert(tree, key, key);

    /* test balance */
    ASSERT(rb_assert(tree, tree->root->chld[1]));

    /* test find node */
    node = rb_find_node(tree, key);
    ASSERT(node);

    /* test find value */
    found = rb_find(tree, key);
    ASSERT(node);

    /* found values must be same */
    ASSERT(node->val == found);

    inserted_items[i] = node->key;
  }

  /* random size */
  for (i = 0; i < count; i++) {
    /* random key length */
    k = rand() % (sizeof(keybuf) / 8 - 1);

    rand_str(keybuf, k);
    key = strdup(keybuf);

    rb_insert(tree, key, key);

    /* test balance */
    ASSERT(rb_assert(tree, tree->root->chld[1]));

    /* test find node */
    node = rb_find_node(tree, key);
    ASSERT(node);

    /* test find value */
    found = rb_find(tree, key);
    ASSERT(node);

    /* found values must be same */
    ASSERT(node->val == found);

    inserted_items[i + 1000] = node->key;
  }

  /* test removing */
  for (i = 0; i < count * 2; i++) {
    rb_remove(tree, inserted_items[i]);

    /* test balance */
    ASSERT(rb_assert(tree, tree->root->chld[1]));
  }

  /* we remove all nodes */
  ASSERT(rb_isempty(tree));

  /* random size */
  for (i = 0; i < 100; i++) {
    /* random key length */
    k = rand() % (sizeof(keybuf) / 8 - 1);

    rand_str(keybuf, k);
    key = strdup(keybuf);

    rb_insert(tree, key, key);

    /* test balance */
    ASSERT(rb_assert(tree, tree->root->chld[1]));

    /* test find node */
    node = rb_find_node(tree, key);
    ASSERT(node);

    /* test find value */
    found = rb_find(tree, key);
    ASSERT(node);

    /* found values must be same */
    ASSERT(node->val == found);

    inserted_items[i] = node->key;
  }

  /* test walking */
  rb_walk(tree, test_rb_walk);

  /* we removed all nodes */
  rb_empty(tree);
  ASSERT(rb_isempty(tree));

  key = strdup(keybuf);
  rb_insert(tree, key, key);

  rb_print(tree);
  rb_destroy(tree);

  TEST_SUCCESS
}

TEST_IMPL(rb_topdown_ptr) {
  RBTree   *tree;
  RBNode   *node;
  void     *key;
  void     *found;
  int       count, i;
  uintptr_t k;

  count = 1000;
  tree  = rb_newtree_ptr();

  srand((unsigned int)time(NULL));

  /* make sure it is samll size: max 32 */
  for (i = 0; i < count; i++) {
    /* random key length */
    k   = rand() % 10000;
    key = (void *)k;

    rb_insert(tree, key, key);

    /* test balance */
    ASSERT(rb_assert(tree, tree->root->chld[1]));

    /* test find node */
    node = rb_find_node(tree, key);
    ASSERT(node);

    /* test find value */
    found = rb_find(tree, key);
    ASSERT(node);

    /* found values must be same */
    ASSERT(node->val == found);
  }

  /* we removed all nodes */
  rb_empty(tree);
  ASSERT(rb_isempty(tree));

  k   =  rand() % 10000;
  key = (void *)k;
  rb_insert(tree, key, key);

  rb_print(tree);
  rb_destroy(tree);

  TEST_SUCCESS
}

TEST_IMPL(rb_topdown_custom_cmp) {
  RBTree   *tree;
  RBNode   *node;
  float    *key;
  void     *found;
  int       count, i;

  count = 1000;
  tree  = rb_newtree(NULL,
                     test_rb_cmp_float,
                     test_rb_print_float);

  srand((unsigned int)time(NULL));

  /* make sure it is samll size: max 32 */
  for (i = 0; i < count; i++) {
    key  = malloc(sizeof(*key));
    *key = (float)drand48();

    rb_insert(tree, key, key);

    /* test balance */
    ASSERT(rb_assert(tree, tree->root->chld[1]));

    /* test find node */
    node = rb_find_node(tree, key);
    ASSERT(node);

    /* test find value */
    found = rb_find(tree, key);
    ASSERT(node);

    /* found values must be same */
    ASSERT(node->val == found);
  }

  /* we removed all nodes */
  rb_empty(tree);
  ASSERT(rb_isempty(tree));

  key  = malloc(sizeof(*key));
  *key = (float)drand48();
  rb_insert(tree, key, key);
  rb_print(tree);

  rb_destroy(tree);

  TEST_SUCCESS
}

TEST_IMPL(rb_topdown_custom_cmp_i32) {
  RBTree   *tree;
  RBNode   *node;
  int32_t  *key;
  void     *found;
  int       count, i;

  count = 1000;
  tree  = rb_newtree(NULL,
                     ds_cmp_i32,
                     ds_print_i32);

  srand((unsigned int)time(NULL));

  /* make sure it is samll size: max 32 */
  for (i = 0; i < count; i++) {
    key  = malloc(sizeof(*key));
    *key = rand() % 10000;

    rb_insert(tree, key, key);

    /* test balance */
    ASSERT(rb_assert(tree, tree->root->chld[1]));

    /* test find node */
    node = rb_find_node(tree, key);
    ASSERT(node);

    /* test find value */
    found = rb_find(tree, key);
    ASSERT(node);

    /* found values must be same */
    ASSERT(node->val == found);
  }

  /* we removed all nodes */
  rb_empty(tree);
  ASSERT(rb_isempty(tree));

  key  = malloc(sizeof(*key));
  *key = rand() % 10000;
  rb_insert(tree, key, key);
  rb_print(tree);

  rb_destroy(tree);

  TEST_SUCCESS
}

TEST_IMPL(rb_topdown_custom_cmp_i64) {
  RBTree   *tree;
  RBNode   *node;
  int64_t  *key;
  void     *found;
  int       count, i;

  count = 1000;
  tree  = rb_newtree(NULL,
                     ds_cmp_i64,
                     ds_print_i64);

  srand((unsigned int)time(NULL));

  /* make sure it is samll size: max 32 */
  for (i = 0; i < count; i++) {
    key  = malloc(sizeof(*key));
    *key = rand() % 10000;

    rb_insert(tree, key, key);

    /* test balance */
    ASSERT(rb_assert(tree, tree->root->chld[1]));

    /* test find node */
    node = rb_find_node(tree, key);
    ASSERT(node);

    /* test find value */
    found = rb_find(tree, key);
    ASSERT(node);

    /* found values must be same */
    ASSERT(node->val == found);
  }

  /* we removed all nodes */
  rb_empty(tree);
  ASSERT(rb_isempty(tree));

  key  = malloc(sizeof(*key));
  *key = rand() % 10000;
  rb_insert(tree, key, key);
  rb_print(tree);

  rb_destroy(tree);

  TEST_SUCCESS
}

TEST_IMPL(rb_topdown_freeenode) {
  RBTree   *tree;
  RBNode   *node;
  float    *key;
  void     *found;
  size_t    insertedCount;
  int       count, i;

  count = 1000;
  tree  = rb_newtree(NULL,
                     test_rb_cmp_float,
                     test_rb_print_float);
  tree->onFreeNode = test_rb_freenode;
  test_rb_freed_nodes = 0u;

  srand((unsigned int)time(NULL));

  /* make sure it is samll size: max 32 */
  for (i = 0; i < count; i++) {
    key  = malloc(sizeof(*key));
    *key = (float)drand48();

    rb_insert(tree, key, key);

    /* test balance */
    ASSERT(rb_assert(tree, tree->root->chld[1]));

    /* test find node */
    node = rb_find_node(tree, key);
    ASSERT(node);

    /* test find value */
    found = rb_find(tree, key);
    ASSERT(node);

    /* found values must be same */
    ASSERT(node->val == found);
  }

  /* we removed all nodes */
  insertedCount = tree->count;
  rb_empty(tree);
  ASSERT(rb_isempty(tree));
  ASSERT(test_rb_freed_nodes == insertedCount);

  key  = malloc(sizeof(*key));
  *key = 2.0f;
  rb_insert(tree, key, key);
  rb_remove(tree, key);
  ASSERT(rb_isempty(tree));
  ASSERT(tree->count == 0u);
  ASSERT(test_rb_freed_nodes == insertedCount + 1u);

  key  = malloc(sizeof(*key));
  *key = 3.0f;
  rb_insert(tree, key, key);
  rb_destroy(tree);
  ASSERT(test_rb_freed_nodes == insertedCount + 2u);

  TEST_SUCCESS
}

static
uint64_t
test_rb_next_random(uint64_t *state) {
  uint64_t value;

  value = *state;
  value ^= value << 13;
  value ^= value >> 7;
  value ^= value << 17;
  *state = value;
  return value;
}

TEST_IMPL(rb_topdown_mixed_stress) {
  enum { KEY_COUNT = 512, OP_COUNT = 200000 };
  RBTree  *tree;
  uint64_t randomState;
  bool     present[KEY_COUNT];
  size_t   expectedCount;
  int      op;

  tree = rb_newtree_ptr();
  randomState = UINT64_C(0x9e3779b97f4a7c15);
  expectedCount = 0u;
  memset(present, 0, sizeof(present));

  for (op = 0; op < OP_COUNT; op++) {
    uintptr_t key;
    void     *pointerKey;

    key = (uintptr_t)(test_rb_next_random(&randomState) % KEY_COUNT);
    pointerKey = (void *)(key + 1u);

    if ((test_rb_next_random(&randomState) & 3u) != 0u) {
      rb_insert(tree, pointerKey, pointerKey);
      if (!present[key]) {
        present[key] = true;
        expectedCount++;
      }
    } else {
      rb_remove(tree, pointerKey);
      if (present[key]) {
        present[key] = false;
        expectedCount--;
      }
    }

    if ((op & 63) == 0) {
      uintptr_t probe;

      ASSERT(tree->count == expectedCount);
      ASSERT(rb_assert(tree, tree->root->chld[1]));
      for (probe = 0u; probe < KEY_COUNT; probe++) {
        void *found;

        found = rb_find(tree, (void *)(probe + 1u));
        ASSERT((found != NULL) == present[probe]);
      }
    }
  }

  rb_empty(tree);
  ASSERT(rb_isempty(tree));
  ASSERT(tree->count == 0u);
  rb_destroy(tree);

  TEST_SUCCESS
}

TEST_IMPL(rb_comparator_edges) {
  int32_t  i32min, i32max;
  uint32_t u32min, u32max;
  int64_t  i64min, i64max;
  uint64_t u64min, u64max;

  i32min = INT32_MIN;
  i32max = INT32_MAX;
  u32min = 0u;
  u32max = UINT32_MAX;
  i64min = INT64_MIN;
  i64max = INT64_MAX;
  u64min = 0u;
  u64max = UINT64_MAX;

  ASSERT(ds_cmp_i32(&i32min, &i32max) < 0);
  ASSERT(ds_cmp_i32(&i32max, &i32min) > 0);
  ASSERT(ds_cmp_ui32(&u32min, &u32max) < 0);
  ASSERT(ds_cmp_ui32(&u32max, &u32min) > 0);
  ASSERT(ds_cmp_i64(&i64min, &i64max) < 0);
  ASSERT(ds_cmp_i64(&i64max, &i64min) > 0);
  ASSERT(ds_cmp_ui64(&u64min, &u64max) < 0);
  ASSERT(ds_cmp_ui64(&u64max, &u64min) > 0);
  ASSERT(ds_cmp_ptr((void *)(uintptr_t)1u,
                    (void *)(uintptr_t)UINTPTR_MAX) < 0);
  ASSERT(ds_cmp_ptr((void *)(uintptr_t)UINTPTR_MAX,
                    (void *)(uintptr_t)1u) > 0);

  TEST_SUCCESS
}
