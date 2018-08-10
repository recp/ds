# libds - Data Structures

[![Build Status](https://travis-ci.org/recp/ds.svg?branch=master)](https://travis-ci.org/recp/libds)
[![Build status](https://ci.appveyor.com/api/projects/status/yqpyll64woh39a23/branch/master?svg=true)](https://ci.appveyor.com/project/recp/libds/branch/master)
[![Coverage Status](https://coveralls.io/repos/github/recp/libds/badge.svg?branch=master)](https://coveralls.io/github/recp/libds?branch=master)
[![Codacy Badge](https://api.codacy.com/project/badge/Grade/50b73a3049b9497a97fe7468a9d79d08)](https://www.codacy.com/app/recp/libds?utm_source=github.com&amp;utm_medium=referral&amp;utm_content=recp/libds&amp;utm_campaign=Badge_Grade)

This library provides common data structures. It will also provide some data structures which needed in render or game engines. In the future I'll try to optimize memmory access. 

There may multiple way to implement a data structure but I tried to implement best way to do that. For instance Red-Black Trees are only implemented as top down insertion/deletion to make it faster. 

You can provide custom allocators, compare and hash functions.

```C
#include <ds/rb.h>
#include <ds/hash.h>
#include <ds/octree.h>
```

## Todo
- [x] rbtree
   - [x] top-down insertion
   - [x] top-down deletion
- [x] forward-list
- [x] hash-table
   - [x] builtin hash functions e.g. djb2
   - [x] resizing hash table
- [ ] octree
- [ ] quadtree

## Build

### Unix (Autotools)

```text
$ sh ./build-deps.sh # run only once (dependencies)
$
$ sh autogen.sh
$ ./configure
$ make
$ make install
$ [sudo] make install
```

### Windows (MSBuild)
Windows related build files, project files are located in `win` folder,
make sure that you are inside `libds/win` folder.
Code Analysis are enabled, it may take awhile to build

```Powershell
$ cd win
$ .\build.bat
```
if `msbuild` didn't work then you can try to build it with `devenv`:
```Powershell
$ devenv libds.sln /Build Release
```

## License
MIT. check the LICENSE file
