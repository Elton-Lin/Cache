# libcache
libcache is a C++ header-only library for software based caches that implements common cache algorithms. See below for the cache replacement polocies currently supported.

## Cache Replacement Policy
Currently implemented:
- LRU - Least Recently Used
- LFU - Least Frequently Used

Time complexity:
| Policy | put  | get  | resize |
| -------|------|------|--------|
| LRU    | O(1) | O(1) | O(n)   |
| LFU    | O(1) | O(1) | O(n)   |

Space complexity is O(n) for all policies.

There are many other interesting [policies](https://en.wikipedia.org/wiki/Cache_replacement_policies); similar idea but for [pages](https://en.wikipedia.org/wiki/Page_replacement_algorithm)

## Features
- Supports standard cache operations: get, put
    - Both have optimal big-oh runtime: O(1)
- Supports custom (user-defined) types as key and/or value using templates
   - Like ``std::unordered_map``, ``operator==`` overload and hash functor on the custom type are required.
- Supports resize
    - It's unrealistic to resize a physical cache, but it could be useful for a software based cache
- Well tested (framework used: [Catch](https://github.com/catchorg/Catch2/tree/v2.x)). Tests are available [here](https://github.com/Elton-Lin/libcache/tree/master/tests)


## Usage
See [example.cpp](https://github.com/Elton-Lin/libcache/blob/master/tests/example.cpp) for a more complete example, which includes a demo of using custom type as key.
```cpp
#include "lfu.hpp"

void foo(...) {
    caches::lfu<int, int> cache(4);
    
    cache.put(1, 11);
    cache.put(3, 33);
    cache.put(5, 55);
    cache.put(3, 333);
    
    // Expected stdout: 333, 55
    std::cout << cache.get(3) << ", " << cache.get(5) << std::endl;
    
    // resizing down causes eviction, '1' will be evicted because it is the LFU
    cache.resize(2);
    
    // This call will cause an exception to be thrown because
    // '1' is not in the cache anymore
    cache.get(1);
}
```

## Setup
The only requirement setup is a C++11 compiler.

## References
- [O(1) LFU paper - 2010](http://dhruvbird.com/lfu.pdf)
- [LFU in Golang blog](https://ieftimov.com/post/when-why-least-frequently-used-cache-implementation-golang/)
- [A cool cache library](https://github.com/vpetrigo/caches)

## Contribution
Contribution and questions are welcome at [issues](https://github.com/Elton-Lin/libcache/issues) and eltonlin@umich.edu. 
Fork this repo to submit a pull request to the master branch. To implement another cache policy, visit cache.hpp to see the base class. Thanks!

## TODO
- [ ] Use cmake to improve organizing project structure and managing dependencies. It can also aid compiling process and time.
- [ ] Implement other policies.
