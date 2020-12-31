#ifndef _LRU_SW_CACHE_H
#define _LRU_SW_CACHE_H

#include "cache.hpp"
#include <list>

#include <iostream>

template<typename Key, typename T, 
        typename Hash = std::hash<Key>, 
        typename Eq = std::equal_to<Key> >
class lru : public cache<Key, T, Hash, Eq> {

    using cache = cache<Key, T, Hash, Eq>;
    using list_iter = typename std::list<Key>::iterator;

    public:

    lru(std::size_t _capacity) : cache(_capacity) {}
    // ~ lru() {};


    T get(const Key &k) override {
        if (cache::entries_map.find(k) == cache::entries_map.end()) {
            throw std::out_of_range("key not found error");
        }
        touch(k);
        return cache::entries_map[k];
    }


    void put(const Key &k, const T &val) override {
        
        if (cache::entries_map.find(k) != cache::entries_map.end()) {
            // updating exisitng key
            cache::entries_map[k] = val;
            return;
        }

        if (cache::size() >= cache::capacity()) {
            evict();
        }

        // Add new entry to front and update maps
        lru_queue.push_front(k);
        node_map.insert(std::make_pair(k, lru_queue.begin()));
        cache::entries_map.insert(std::make_pair(k, val));
    }
    
    
    void print() override {
        std::cout << "---- Cache Content (in the order of mru to lru) -----\n"
                  << "key: value\n";
        for (const Key &k :lru_queue) {
            std::cout << k << ": " << cache::entries_map[k] << "\n";
        }
    }


    void resize(std::size_t new_capacity) override {
        size_t cap_diff = cache::capacity() - new_capacity;
        if (cap_diff > 0) {
            for (int i = 0; i < cap_diff; ++i) {
                evict();
            }
        }
        cache::m_capacity = new_capacity;
    }

    private:

    // Doubly-linked list to maintain ordering of entries
    // (the least recently used is always at the end)
    std::list<Key> lru_queue;
    // Maps entry key to iterator (points to entry node in list)
    std::unordered_map<Key, list_iter, Hash, Eq> node_map;

    void touch(const Key &k) {
        // Move entry to the front (most recently used) of queue
        list_iter entry_it = node_map[k];
        lru_queue.splice(lru_queue.begin(), lru_queue, entry_it);
    }

    void evict() {
        cache::entries_map.erase(lru_queue.back());
        node_map.erase(lru_queue.back());
        lru_queue.pop_back();
    }

};

#endif