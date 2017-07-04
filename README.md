# libds - Data Structures

[![Build Status](https://travis-ci.org/recp/libds.svg?branch=master)](https://travis-ci.org/recp/libds)
[![Build status](https://ci.appveyor.com/api/projects/status/yqpyll64woh39a23/branch/master?svg=true)](https://ci.appveyor.com/project/recp/libds/branch/master)
[![Coverage Status](https://coveralls.io/repos/github/recp/libds/badge.svg?branch=master)](https://coveralls.io/github/recp/libds?branch=master)
[![Codacy Badge](https://api.codacy.com/project/badge/Grade/50b73a3049b9497a97fe7468a9d79d08)](https://www.codacy.com/app/recp/libds?utm_source=github.com&amp;utm_medium=referral&amp;utm_content=recp/libds&amp;utm_campaign=Badge_Grade)

Why? I've written a rbtree and used in two project (I duplicated it).
I need to maintain same source in two project,
I mean that if I fix a bug in one project then I need to copy-paste to another, it is hard to maintain.
Currently I need another data structures e.g. hashtable, octree...
now it is better to separate common data structures as external library

I think `ds` namespace is very good choice, we can use it like this:
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
make sure you are inside `libds/win` folder.
Code Analysis are enabled, it may take awhile to build

```Powershell
$ cd win
$ .\build.bat
```
if `msbuild` won't work (because of multi version VS) then try to build with `devenv`:
```Powershell
$ devenv libds.sln /Build Release
```

## License
MIT. check the LICENSE file
