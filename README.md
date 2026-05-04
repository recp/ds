# 🔗 Data Structures and Algorithms

[![Build Status](https://travis-ci.org/recp/ds.svg?branch=master)](https://travis-ci.org/recp/ds)
[![Build status](https://ci.appveyor.com/api/projects/status/yqpyll64woh39a23/branch/master?svg=true)](https://ci.appveyor.com/project/recp/libds/branch/master)
[![Coverage Status](https://coveralls.io/repos/github/recp/libds/badge.svg?branch=master)](https://coveralls.io/github/recp/libds?branch=master)
[![Codacy Badge](https://api.codacy.com/project/badge/Grade/50b73a3049b9497a97fe7468a9d79d08)](https://www.codacy.com/app/recp/libds?utm_source=github.com&amp;utm_medium=referral&amp;utm_content=recp/libds&amp;utm_campaign=Badge_Grade)

This library provides common data structures. It will also provide some data structures which needed in render or game engines. In the future I'll try to optimize memmory access. 

There may multiple way to implement a data structure but I tried to implement best way to do that. For instance Red-Black Trees are only implemented as top down insertion/deletion to make it faster. 

There are some convenient constructors for common use cases. For instance `rb_newtree_str()` creates new rb-tree that uses strings as key, `rb_newtree_ptr()` creates new rb-tree that uses pointers as key. When you use these functions to alloc a data structure then you don't need to provide compare or print functions. 

This library prodives allocator api so you can override default allocator. 

## Usage

```C
#include <ds/rb.h> // Red-Black Tree

int
main(int argc, const char * argv[]) {
  RBTree *tree;
  /* ... */

  /* use string keys */
  tree = rb_newtree_str();

  rb_insert(tree, "key", value);

  /* ... */
  value = rb_find(tree, "key");

  rb_destroy(tree);
}

```

## Todo
- [x] rbtree
   - [x] top-down insertion
   - [x] top-down deletion
- [x] forward-list
  - [x] forward-list-separate (reduces FList struct)
- [x] hash-table
   - [x] builtin hash functions e.g. djb2
   - [x] resizing hash table
- [x] quick sort implementation for float, double, i32, u32, i64, u64
- [ ] queue (working on this)
- [ ] stack
- [ ] binary heap / priority queue
- [ ] dynamic array
- [ ] octree
- [ ] quadtree
- [ ] kd-tree
- [ ] bvh
- [ ] b-tree

## Build

ds uses CMake on macOS, Linux and Windows. It can be built as a standalone
project, embedded with `add_subdirectory()` or installed and used with
`find_package()`.

### Standalone CMake build

```bash
$ cmake -S . -B build -DCMAKE_BUILD_TYPE=Release
$ cmake --build build
$ cmake --install build # [Optional]
```

For Visual Studio or other multi-config generators:

```Powershell
$ cmake -S . -B build
$ cmake --build build --config Release
$ cmake --install build --config Release # [Optional]
```

Release builds use the platform compiler's optimized release mode. `DS_ENABLE_LTO`
can be enabled for link-time optimization when the compiler and generator support
it.

### Embedded in another CMake project

```cmake
cmake_minimum_required(VERSION 3.8.2)

project(my_app LANGUAGES C)

add_subdirectory(external/ds)

add_executable(my_app src/main.c)
target_link_libraries(my_app PRIVATE ds::ds)
```

When embedded, ds does not force a default build type on the parent project.

### Installed package

After `cmake --install`, consumers can use:

```cmake
find_package(ds CONFIG REQUIRED)
target_link_libraries(my_app PRIVATE ds::ds)
```

## License
MIT. check the LICENSE file
