#ifndef _LRU_SW_CACHE_H
#define _LRU_SW_CACHE_H

#include "cache.hpp"
#include <iostream>
#include <list>
#include <unordered_map>

template<typename Key, typename T, 
        typename Hash = std::hash<Key>, 
        typename Eq = std::equal_to<Key> >
class lru : public cache<Key, T> {

    struct CacheEntry;

    using cache = cache<Key, T>;
    using entry_iter = typename std::list<CacheEntry>::iterator;

    public:

    lru(std::size_t _capacity) : cache(_capacity) {}
    // ~ lru() {};

    std::size_t size() override {
        return node_map.size();
    }

    T get(const Key &k) override {
        if (node_map.find(k) == node_map.end()) {
            throw std::out_of_range("key not found error");
        }
        touch(k);
        return node_map[k]->entry_val;
    }


    void put(const Key &k, const T &val) override {
        
        if (node_map.find(k) != node_map.end()) {
            // updating exisitng key
            node_map[k]->entry_val = val;
            touch(k);
            return;
        }

        if (size() >= cache::capacity()) {
            evict();
        }

        // Add new entry to front and update maps
        lru_queue.push_front(CacheEntry{k, val});
        node_map.insert(std::make_pair(k, lru_queue.begin()));
    }
    

    void print() override {
        std::cout << "---- Cache Content (in the order of mru to lru) -----\n"
                  << "key: value\n";
        for (const CacheEntry &ent :lru_queue) {
            std::cout << ent.entry_k << ": " << ent.entry_val << "\n";
        }
    }


    void resize(std::size_t new_capacity) override {
        size_t cap_diff = cache::capacity() - new_capacity;
        if (cap_diff > 0) {
            for (int i = 0; i < cap_diff; ++i) {
                evict();
            }
        }
        cache::cache_capacity = new_capacity;
    }

    private:

    struct CacheEntry {
        Key entry_k;
        T entry_val;
    };

    // Doubly-linked list to maintain ordering of entries
    // (the least recently used is always at the end)
    std::list<CacheEntry> lru_queue;
    // Maps entry key to iterator (points to entry node in list)
    std::unordered_map<Key, entry_iter> node_map;

    void touch(const Key &k) {
        // Move entry to the front (most recently used) of queue
        entry_iter it = node_map[k];
        lru_queue.splice(lru_queue.begin(), lru_queue, it);
    }

    void evict() {
        node_map.erase(lru_queue.back().entry_k);
        lru_queue.pop_back();
    }

};

#endif