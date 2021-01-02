#ifndef _SW_CACHE_H
#define _SW_CACHE_H

#include <cstddef> // size_t

namespace caches {

template<typename Key, typename T>
class cache {

    public:
    virtual T get(const Key &k) = 0;
    virtual void put(const Key &k, const T &val) = 0;
    
    virtual void print() = 0;
    virtual void resize(std::size_t new_capacity) = 0;

    virtual std::size_t size() = 0;

    std::size_t capacity() {
        return cache_capacity;
    }

    protected:
    
    std::size_t cache_capacity;

    cache(std::size_t _capacity) : cache_capacity(_capacity) {};
    // ~ cahce() {}; // virtual?

};
} // namespace caches

#endif