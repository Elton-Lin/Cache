#ifndef _SW_CACHE_H
#define _SW_CACHE_H

#include <cstddef> // size_t
#include <unordered_map>

template<typename Key, typename T, 
        typename Hash = std::hash<Key>, 
        typename Eq = std::equal_to<Key> >
class cache {

    public:
    virtual T get(const Key &k) = 0;
    virtual void put(const Key &k, const T &val) = 0;
    
    virtual void print() = 0;
    virtual void resize(std::size_t new_capacity) = 0;

    size_t size() {
        return entries_map.size();
    }
    
    size_t capacity() {
        return cache_capacity;
    }

    protected:
    
    std::size_t cache_capacity;
    std::unordered_map<Key, T, Hash, Eq> entries_map;

    cache(std::size_t _capacity) : cache_capacity(_capacity) {};
    // ~ cahce() {}; // virtual?

};

#endif