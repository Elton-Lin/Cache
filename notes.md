cache library


API
- supports standard operations
    - put: inserts a new item into the cache
    - get: retrieves the value of an item in the cache by its key

- other operations
    - resize: change the capacity of a cache (not realisitc for a physical cache, but can be useful in a software cache)
    - print: prints the content of cache in CL (debugging, aid understanding)
    - I/O: import and export cache content as json? (saving cache to a persistent storage)

- exception handling
    - key not exist
        - unordered_map::at throws std::out_of_range
    - define custom exception types?

- internal operations:
    - eviction: depends on policy


template
- supports any types, even user defined custom types
    - a hash function needs to be provided for custom types because internally use unordered_map (use abseil?)
    - a eq function might also be needed
    - (boost has hash functions for pair, som standard containers types)

class
- implement big 3/5?

Testing
- unit testing
- use gtest?


project fs structure
http://www.open-std.org/jtc1/sc22/wg21/docs/papers/2018/p1204r0.html

others:
set up cmake to better organize the project (paths  and dependencies easier to manage)