#ifndef _LFU_SW_CACHE_H
#define _LFU_SW_CACHE_H

#include "cache.hpp"
#include <iostream>
#include <list>
#include <unordered_map>

template<typename Key, typename T, 
        typename Hash = std::hash<Key>, 
        typename Eq = std::equal_to<Key> >
class lfu : public cache<Key, T> {

    struct CacheEntry;
    struct FreqBucket;

    using cache = cache<Key, T>;
    using bucket_iter = typename std::list<FreqBucket>::iterator;
    using entry_iter = typename std::list<CacheEntry>::iterator;

    public:

    lfu(std::size_t _capacity) : cache(_capacity) {}
    // ~ lfu() {};


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
            touch(k);
            return;
        }

        if (cache::size() >= cache::capacity()) {
            evict();
        }

        // Add new entry to front and update maps
        lfu_queue.push_front(k);
        node_map.insert(std::make_pair(k, lfu_queue.begin()));
        cache::entries_map.insert(std::make_pair(k, val));
    }
    

    void print() override {
        std::cout << "---- Cache Content (in the order of mru to lru) -----\n"
                  << "key: value (frequency count)\n";
        for (const CacheEntry &ent :lru_queue) {
            std::cout << ent.entry_k << ": " << ent.entry_val 
                      << "(" << ent.bucket->freq_count <<  ")\n";
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
        bucket_iter bucket;
    };

    struct FreqBucket {
        std::size_t freq_count;
        std::list<CacheEntry> entries;
    };

    // Doubly-linked list storing frequency buckets in increasing order
    std::list<FreqBucket> lru_queue;
    // Maps entry key to iterator (points to entry node in list)
    std::unordered_map<Key, entry_iter> node_map;

    void touch(const Key &k) {
        // Move entry to the next (current count + 1) bucket
        entry_iter it = node_map[k];
        
    }

    void evict() {
        
    }

};

#endif