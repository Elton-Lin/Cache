#ifndef _LFU_SW_CACHE_H
#define _LFU_SW_CACHE_H

#include "cache.hpp"
#include <iostream>
#include <list>
#include <unordered_map>


namespace caches {

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

        if (lfu_queue.empty() || lfu_queue.begin()->freq_count != INIT_COUNT) {
            lfu_queue.emplace_front(
                FreqBucket {INIT_COUNT, std::list<CacheEntry>()}
            );
        }

        // insert the new entry to the '1' bucket and node map
        std::list<CacheEntry> &bucket_one_entries = lfu_queue.begin()->entries;
        bucket_one_entries.emplace_front(CacheEntry {k, val, lfu_queue.begin()});
        node_map.insert(std::make_pair(k, bucket_one_entries.begin()));
    }


    void print() override {
        std::cout << "---- Cache Content (in the order of mru to lru) -----\n"
                  << "key: value (frequency count)\n";
        for (const FreqBucket &bucket : lfu_queue) {
            for (const CacheEntry &ent : bucket.entries) {
                std::cout << ent.entry_k << ": " << ent.entry_val 
                      << "(" << bucket.freq_count <<  ")\n";
            }
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
        bucket_iter bucket_it;
    };

    struct FreqBucket {
        std::size_t freq_count;
        std::list<CacheEntry> entries;
    };

    const std::size_t INIT_COUNT = 1;

    // Doubly-linked list storing frequency buckets in increasing order
    // Each bucket stores a doubly-linked list of cache entries
    std::list<FreqBucket> lfu_queue;
    std::unordered_map<Key, entry_iter, Hash> node_map;


    // Increment the freq count for this entry
    void touch(const Key &k) {
        entry_iter entry = node_map[k];

        bucket_iter cur_bucket_it = entry->bucket_it;
        bucket_iter next_bucket_it = cur_bucket_it; 
        ++next_bucket_it;// careful: can be end()

        if (next_bucket_it == lfu_queue.end() ||
            cur_bucket_it->freq_count + 1 < next_bucket_it->freq_count) {

            // Inserts a new bucket (with count + 1) right after current bucket
            lfu_queue.insert(next_bucket_it, FreqBucket {
                                    cur_bucket_it->freq_count + 1, 
                                    std::list<CacheEntry>()});
        }
        
        next_bucket_it = cur_bucket_it; 
        ++next_bucket_it;
        std::list<CacheEntry> &cur_entries = cur_bucket_it->entries;
        std::list<CacheEntry> &next_entries = next_bucket_it->entries;

        // Repoint entry's iter to new bucket 
        // then move entry to the next bucket's entries
        entry->bucket_it = next_bucket_it;
        next_entries.splice(next_entries.begin(), cur_entries, entry);

        if (cur_entries.empty()) {
            lfu_queue.erase(cur_bucket_it);
        }
    }

    void evict() {

        std::list<CacheEntry> &lfu_entries = lfu_queue.begin()->entries;
        node_map.erase(lfu_entries.back().entry_k);
        lfu_entries.pop_back();

        // remove the bucket if it's empty
        if (lfu_entries.empty()) {
            lfu_queue.pop_front();
        }
    }

};
} // namespace caches

#endif