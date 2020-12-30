#ifndef _LFU_SW_CACHE_H
#define _LFU_SW_CACHE_H

#include "cache.h"
#include <unordered_map>

template<typename Key, 
        typename T, 
        typename KeyHash = std::hash<Key>, 
        typename KeyEqual = std::equal_to<Key> >
class lfu : public cache<Key, T> {

    public:
    lfu(std::size_t _capacity) {};
    // ~ lfu() {};

    T get(Key k) override;
    void put(Key k, T val) override;
    
    void print() override;
    void resize(std::size_t new_capacity) override;

    private:

    std::unordered_map<Key, T, KeyHash, KeyEqual> cache_map;
    // other ds - doubly linked list

};

#endif