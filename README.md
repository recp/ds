# libds - Data Structures

Why? I've written a rbtree and used in two project (I duplicated it). 
I need to maintain same source in two project, 
I mean that if I fix a bug in one project then I need to copy-paste to another, it is hard to maintain.
Currently I need another data structures e.g. hashtable, octree... 
now it is better to separate common data structures as external library

I think `ds` namespace is very good choice, we can use it like this:
```C
#include <ds/rb.h>
#include <ds/hashtable.h>
#include <ds/octree.h>
```

# Todo
- [x] rbtree (top-down)
- [ ] hashtable
- [ ] octree
- [ ] quadtree

# Build 
todo

# License
MIT. check the LICENSE file
