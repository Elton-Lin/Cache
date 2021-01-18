#ifndef _LRU_SW_CACHE_H
#define _LRU_SW_CACHE_H

#include "cache.hpp"
#include <iostream>
#include <list>
#include <unordered_map>


namespace caches {

template<typename Key, typename T, 
        typename Hash = std::hash<Key>, 
        typename Eq = std::equal_to<Key> >
class lru : public cache<Key, T> {

    using cache_t = cache<Key, T>;
    using list_iter_t = typename std::list<Key>::iterator;

    public:

    lru(std::size_t _capacity) : cache_t(_capacity) {}

    private:

    // Doubly-linked list to maintain ordering of entries
    // (the least recently used is always at the end)
    std::list<Key> lru_queue;

    // Maps entry key to iterator (points to node in list queue)
    std::unordered_map<Key, list_iter_t, Hash, Eq> node_map;

    void policy_put(const Key &k, const T &val) override {
        // Add new entry to front and update map
        lru_queue.push_front(k);
        node_map.insert(std::make_pair(k, lru_queue.begin()));
    }


    void touch(const Key &k) override {
        // Move entry to the front (most recently used) of queue
        list_iter_t it = node_map[k];
        lru_queue.splice(lru_queue.begin(), lru_queue, it);
    }


    void evict() override {
        Key &evicted_key = lru_queue.back();
        cache_t::entry_storage.erase(evicted_key);
        node_map.erase(evicted_key);
        lru_queue.pop_back();
    }

};
} // namespace caches

#endif