#ifndef _LFU_SW_CACHE_H
#define _LFU_SW_CACHE_H

#include "cache.hpp"
#include <unordered_map>

template<typename Key, typename T, 
        typename KeyHash = std::hash<Key>, 
        typename KeyEqual = std::equal_to<Key> >
class lfu : public cache<Key, T> {

    using cache = cache<Key, T>;

    public:
    lfu(std::size_t _capacity) : cache(_capacity) {
        cache::m_size = 0;
    }
    // ~ lfu() {};

    T get(Key k) override {
        return T();
    }
    void put(Key k, T val) override {
        cache::m_size += 1;
        return;
    }
    
    void print() override {
        return;
    }
    void resize(std::size_t new_capacity) override {
        return;
    }

    private:

    std::unordered_map<Key, T, KeyHash, KeyEqual> cache_map;
    // other ds - doubly linked list

};

#endif